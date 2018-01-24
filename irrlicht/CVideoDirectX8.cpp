#ifdef WIN32

#include "CVideoDirectX8.h"
#include "os.h"
#include "S3DVertex.h"
#include "CDirectX8Texture.h"


namespace irr
{
namespace video
{

//! constructor
CVideoDirectX8::CVideoDirectX8(const core::dimension2d<s32>& screenSize, HWND window, 
								bool fullscreen, io::IFileSystem* io, bool pureSoftware)
: CVideoNull(io, screenSize), D3DLibrary(0), CurrentRenderMode(ERM_NONE), pID3DDevice(0),
 LastVertexType((video::E_VERTEX_TYPE)-1), ResetRenderStates(true), pID3D(0),
 LastSetLight(-1)
{

	#ifdef _DEBUG
	setDebugName("CVideoDirectX8");
	#endif

	CurrentTexture[0] = 0;
	CurrentTexture[1] = 0;

	// init direct 3d is done in the factory function
}




//! destructor
CVideoDirectX8::~CVideoDirectX8()
{

	if (CurrentTexture[0])
		CurrentTexture[0]->drop();

	if (CurrentTexture[1])
		CurrentTexture[1]->drop();

	// drop d3d8

	if (pID3DDevice)	
		pID3DDevice->Release();

	if (pID3D)		
		pID3D->Release();
}




//! initialises the Direct3D API
bool CVideoDirectX8::initDriver(const core::dimension2d<s32>& screenSize, HWND hwnd,
								u32 bits, bool fullScreen, bool pureSoftware)
{
	HRESULT hr;
	D3DLibrary = LoadLibrary( "d3d8.dll" );

	if (!D3DLibrary)
	{
		os::Debuginfo::print("Error, could not load d3d8.dll.");
		return false;
	}

	typedef IDirect3D9 * (__stdcall *D3DCREATETYPE)(UINT);
	D3DCREATETYPE d3dCreate = (D3DCREATETYPE) GetProcAddress(D3DLibrary, "Direct3DCreate8");

	if (!d3dCreate)
	{
		os::Debuginfo::print("Error, could not get proc adress of Direct3DCreate8.");
		return false;
	}

	pID3D = (*d3dCreate)(D3D_SDK_VERSION);
	//pID3D = Direct3DCreate8(D3D_SDK_VERSION);

	if (!pID3D)
	{
		os::Debuginfo::print("Error initializing D3D.");
		return false;
	}

	D3DDISPLAYMODE d3ddm;
	hr = pID3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	if (FAILED(hr))
	{
		os::Debuginfo::print("Could not get Adapter Display mode.");
		return false;
	}


	D3DPRESENT_PARAMETERS present;
	ZeroMemory(&present, sizeof(present));

	present.SwapEffect					= D3DSWAPEFFECT_FLIP;//D3DSWAPEFFECT_COPY;
	present.Windowed					= fullScreen ? FALSE : TRUE;
	present.BackBufferFormat			= d3ddm.Format;
	present.EnableAutoDepthStencil		= TRUE;
	present.AutoDepthStencilFormat		= D3DFMT_D16;

	if (fullScreen)
	{
		present.BackBufferWidth = screenSize.Width;
		present.BackBufferHeight = screenSize.Height;
		present.BackBufferFormat = D3DFMT_R5G6B5;
		present.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		present.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	}

	if (pureSoftware)
	{
		hr = pID3D->CreateDevice(	D3DADAPTER_DEFAULT, D3DDEVTYPE_REF,	hwnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING, &present, &pID3DDevice);

		if (FAILED(hr))
		{
			os::Warning::print("Could not create Direct3D8 software device.");
			return false;
		}
	}
	else
	{
		hr = pID3D->CreateDevice(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,	hwnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING, &present, &pID3DDevice);

		if(FAILED(hr))
		{
			hr = pID3D->CreateDevice(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,	hwnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, &present, &pID3DDevice);

			if(FAILED(hr))
			{
				hr = pID3D->CreateDevice(	D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hwnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, &present, &pID3DDevice);

				if (FAILED(hr))
				{
					os::Warning::print("Could not create Direct3D8 device.");
					return false;
				}

				os::Debuginfo::print("Running DirectX8 in software mode, no hardware available.");
			}
		}
	}

	// get caps
	pID3DDevice->GetDeviceCaps(&Caps);

	// set default vertex shader
	setVertexShader(EVT_STANDARD);

	ResetRenderStates = true;

	// set the renderstates
	setRenderStates3DMode();

	// so far so good.
	return true;
}




//! applications must call this method before performing any rendering. returns false if failed.
bool CVideoDirectX8::beginScene(bool backBuffer, bool zBuffer, Color color)
{
	CVideoNull::beginScene(backBuffer, zBuffer, color);

	DWORD flags = 0;

	if (backBuffer)
		flags |= D3DCLEAR_TARGET;

	if (zBuffer)
		flags |= D3DCLEAR_ZBUFFER;

	pID3DDevice->Clear( 0, NULL, flags, color.color, 1.0, 0);

	HRESULT hr = pID3DDevice->BeginScene();
	return !FAILED(hr);
}



//! applications must call this method after performing any rendering. returns false if failed.
bool CVideoDirectX8::endScene()
{
	CVideoNull::endScene();

	HRESULT hr = pID3DDevice->EndScene();
	if (FAILED(hr))
	{
		os::Warning::print("DirectX8 end scene failed.");
		return false;
	}

	hr = pID3DDevice->Present(NULL, NULL, NULL, NULL);
	return !FAILED(hr);
}



//! queries the features of the driver, returns true if feature is available
bool CVideoDirectX8::queryFeature(EK3D_VIDEO_DRIVER_FEATURE feature)
{
	switch (feature)
	{
	case EK3DVDF_BILINEAR_FILER:
		return true;
	case EK3DVDF_RENDER_TO_TARGET:
		return false;
	case EK3DVDF_HARDWARE_TL:
		return (Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) != 0;
	case EK3DVDF_MIP_MAP:
		return (Caps.TextureCaps & D3DPTEXTURECAPS_MIPMAP) != 0;
	};

	return false;
}



//! sets transformation
void CVideoDirectX8::setTransform(E_TRANSFORMATION_STATE state, const core::matrix4& mat)
{
	switch(state)
	{
	case TS_VIEW:
		pID3DDevice->SetTransform(D3DTS_VIEW, (D3DMATRIX*)((void*)&mat));
		break;
	case TS_WORLD:
		pID3DDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)((void*)&mat));
		break;
	case TS_PROJECTION:
		pID3DDevice->SetTransform( D3DTS_PROJECTION, (D3DMATRIX*)((void*)&mat));
		break;
	}

	Matrices[state] = mat;
}




//! sets the current Texture
void CVideoDirectX8::setTexture(s32 stage, video::ITexture* texture)
{
	if (CurrentTexture[stage] == texture)
		return;

	#ifdef _DEBUG
	if (texture && texture->getDriverType() != DT_DIRECTX8)
	{
		os::Debuginfo::print("Fatal Error: Tried to set a texture not owned by this driver.");
		return;
	}
	#endif

	if (CurrentTexture[stage])
		CurrentTexture[stage]->drop();

	CurrentTexture[stage] = texture;	

	if (!texture)
		pID3DDevice->SetTexture(stage, 0);
	else
	{
		pID3DDevice->SetTexture(stage, ((CDirectX8Texture*)texture)->getDX8Texture());
		texture->grab();
	}
}



//! sets a material
void CVideoDirectX8::setMaterial(const SMaterial& material)
{
	Material = material;

	setTexture(0, Material.Texture1);
	setTexture(1, Material.Texture2);
}



//! returns a device dependent texture from a software surface (ISurface)
//! THIS METHOD HAS TO BE OVERRIDDEN BY DERIVED DRIVERS WITH OWN TEXTURES
video::ITexture* CVideoDirectX8::createDeviceDependentTexture(ISurface* surface, bool generateMipLevels)
{
	return new CDirectX8Texture(surface, pID3DDevice,
		queryFeature(EK3DVDF_MIP_MAP) ? generateMipLevels : false);
}


//! sets a render target
void CVideoDirectX8::setRenderTarget(video::ITexture* texture)
{
	#ifdef _DEBUG
	if (texture->getDriverType() != DT_DIRECTX8)
	{
		os::Debuginfo::print("Fatal Error: Tried to set a texture not owned by this driver.");
		return;
	}
	#endif
}



//! sets a viewport
void CVideoDirectX8::setViewPort(const core::rectEx<s32>& area)
{
	core::rectEx<s32> vp = area;
	core::rectEx<s32> rendert(0,0, ScreenSize.Width, ScreenSize.Height);
	vp.clipAgainst(rendert);

	D3DVIEWPORT9 viewPort;
	viewPort.X = vp.UpperLeftCorner.X;
	viewPort.Y = vp.UpperLeftCorner.Y;
	viewPort.Width = vp.getWidth();
	viewPort.Height = vp.getHeight();
	viewPort.MinZ = 0.0f;
	viewPort.MaxZ = 1.0f;

	HRESULT hr = D3DERR_INVALIDCALL;
	if (vp.getHeight()>0 && vp.getWidth()>0)
		hr = pID3DDevice->SetViewport(&viewPort);

	#ifdef _DEBUG
	if (FAILED(hr))
		os::Debuginfo::print("Failed setting the viewport.");
	#endif

	ViewPort = vp;
}



//! gets the area of the current viewport
const core::rectEx<s32>& CVideoDirectX8::getViewPort() const
{
	return ViewPort;
}


//! draws an indexed triangle list
void CVideoDirectX8::drawIndexedTriangleList(const S3DVertex* vertices, s32 vertexCount, const u16* indexList, s32 triangleCount)
{
	CVideoNull::drawIndexedTriangleList(vertices, vertexCount, indexList, triangleCount);

	if (!vertexCount || !triangleCount)
		return;

	setVertexShader(EVT_STANDARD);
	setRenderStates3DMode();

	pID3DDevice->DrawIndexedPrimitiveUP(	D3DPT_TRIANGLELIST, 0,
											vertexCount, 
											triangleCount,
											indexList,
											D3DFMT_INDEX16,
											vertices,
											sizeof(S3DVertex));
}



//! draws an indexed triangle list
void CVideoDirectX8::drawIndexedTriangleList(const S3DVertex2TCoords* vertices, s32 vertexCount, const u16* indexList, s32 triangleCount)
{
	CVideoNull::drawIndexedTriangleList(vertices, vertexCount, indexList, triangleCount);

	if (!vertexCount || !triangleCount)
		return;

	setVertexShader(EVT_2TCOORDS);
	setRenderStates3DMode();

	pID3DDevice->DrawIndexedPrimitiveUP(	D3DPT_TRIANGLELIST, 0,
											vertexCount, 
											triangleCount,
											indexList,
											D3DFMT_INDEX16,
											vertices,
											sizeof(S3DVertex2TCoords));
}


//! draws an 2d image
void CVideoDirectX8::draw2DImage(video::ITexture* texture, const core::position2d<s32>& destPos)
{
	if (!texture)
		return;

	draw2DImage(texture,destPos, core::rectEx<s32>(core::position2d<s32>(0,0), texture->getDimension()));
}



//! draws an 2d image, using a color (if color is other then Color(255,255,255,255)) and the alpha channel of the texture if wanted.
void CVideoDirectX8::draw2DImage(video::ITexture* texture, const core::position2d<s32>& pos, const core::rectEx<s32>& sourceRect, const core::rectEx<s32>* clipRect, Color color, bool useAlphaChannelOfTexture)
{
	if (!texture)
		return;

	if (texture)
	{
		#ifdef _DEBUG
		if (texture->getDriverType() != DT_DIRECTX8)
		{
			os::Debuginfo::print("Fatal Error: Tried to copy from a surface not owned by this driver.");
			return;
		}
		#endif
	}

	if (sourceRect.UpperLeftCorner.X >= sourceRect.LowerRightCorner.X ||
		sourceRect.UpperLeftCorner.Y >= sourceRect.LowerRightCorner.Y)
		return;

	core::position2d<s32> targetPos = pos;
	core::position2d<s32> sourcePos = sourceRect.UpperLeftCorner;
	core::dimension2d<s32> sourceSize(sourceRect.getWidth(), sourceRect.getHeight());
	const core::dimension2d<s32> targetSurfaceSize = ScreenSize;
	
	if (clipRect)
	{
		if (targetPos.X < clipRect->UpperLeftCorner.X)
		{
			sourceSize.Width += targetPos.X - clipRect->UpperLeftCorner.X;
			if (sourceSize.Width <= 0)
				return;

			sourcePos.X -= targetPos.X - clipRect->UpperLeftCorner.X;
			targetPos.X = clipRect->UpperLeftCorner.X;
		}
		
		if (targetPos.X + sourceSize.Width > clipRect->LowerRightCorner.X)
		{
			sourceSize.Width -= (targetPos.X + sourceSize.Width) - clipRect->LowerRightCorner.X;
			if (sourceSize.Width <= 0)
				return;
		}

		if (targetPos.Y < clipRect->UpperLeftCorner.Y)
		{
			sourceSize.Height += targetPos.Y - clipRect->UpperLeftCorner.Y;
			if (sourceSize.Height <= 0)
				return;

			sourcePos.Y -= targetPos.Y - clipRect->UpperLeftCorner.Y;
			targetPos.Y = clipRect->UpperLeftCorner.Y;
		}
		
		if (targetPos.Y + sourceSize.Height > clipRect->LowerRightCorner.Y)
		{
			sourceSize.Height -= (targetPos.Y + sourceSize.Height) - clipRect->LowerRightCorner.Y;
			if (sourceSize.Height <= 0)
				return;
		}
	}

	// clip these coordinates

	if (targetPos.X<0)
	{
		sourceSize.Width += targetPos.X;
		if (sourceSize.Width <= 0)
			return;

		sourcePos.X -= targetPos.X;
		targetPos.X = 0;
	}
	
	if (targetPos.X + sourceSize.Width > targetSurfaceSize.Width)
	{
		sourceSize.Width -= (targetPos.X + sourceSize.Width) - targetSurfaceSize.Width;
		if (sourceSize.Width <= 0)
			return;
	}

	if (targetPos.Y<0)
	{
		sourceSize.Height += targetPos.Y;
		if (sourceSize.Height <= 0)
			return;

		sourcePos.Y -= targetPos.Y;
		targetPos.Y = 0;
	}
	
	if (targetPos.Y + sourceSize.Height > targetSurfaceSize.Height)
	{
		sourceSize.Height -= (targetPos.Y + sourceSize.Height) - targetSurfaceSize.Height;
		if (sourceSize.Height <= 0)
			return;
	}

	// ok, we've clipped everything.
	// now draw it.

	if (useAlphaChannelOfTexture)
		setRenderStates2DMode(false, true, true);
	else
		setRenderStates2DMode(false, true, false);
	
	setTexture(0, texture);
	
	core::rectEx<s32> poss(targetPos, sourceSize);

	s32 xPlus = -(ScreenSize.Width>>1);
	f32 xFact = 1.0f / (ScreenSize.Width>>1);

	s32 yPlus = ScreenSize.Height-(ScreenSize.Height>>1);
	f32 yFact = 1.0f / (ScreenSize.Height>>1);

	const core::dimension2d<s32> sourceSurfaceSize = texture->getDimension();
	core::rectEx<f32> tcoords;
	tcoords.UpperLeftCorner.X = (((f32)sourcePos.X)+0.5f) / texture->getDimension().Width ;
	tcoords.UpperLeftCorner.Y = (((f32)sourcePos.Y)+0.5f) / texture->getDimension().Height;
	tcoords.LowerRightCorner.X = (((f32)sourcePos.X +0.5f + (f32)sourceSize.Width)) / texture->getDimension().Width;
	tcoords.LowerRightCorner.Y = (((f32)sourcePos.Y +0.5f + (f32)sourceSize.Height)) / texture->getDimension().Height;

    S3DVertex vtx[4];
	vtx[0] = S3DVertex((f32)(poss.UpperLeftCorner.X+xPlus + 0.5f) * xFact, (f32)(yPlus-poss.UpperLeftCorner.Y + 0.5f) * yFact , 0.0f, 0.0f, 0.0f, 0.0f, color, tcoords.UpperLeftCorner.X, tcoords.UpperLeftCorner.Y);
	vtx[1] = S3DVertex((f32)(poss.LowerRightCorner.X+xPlus + 0.5f) * xFact, (f32)(yPlus- poss.UpperLeftCorner.Y + 0.5f) * yFact, 0.0f, 0.0f, 0.0f, 0.0f, color, tcoords.LowerRightCorner.X, tcoords.UpperLeftCorner.Y);
	vtx[2] = S3DVertex((f32)(poss.LowerRightCorner.X+xPlus + 0.5f) * xFact, (f32)(yPlus-poss.LowerRightCorner.Y + 0.5f) * yFact, 0.0f, 0.0f, 0.0f, 0.0f, color, tcoords.LowerRightCorner.X, tcoords.LowerRightCorner.Y);
	vtx[3] = S3DVertex((f32)(poss.UpperLeftCorner.X+xPlus + 0.5f) * xFact, (f32)(yPlus-poss.LowerRightCorner.Y + 0.5f) * yFact, 0.0f, 0.0f, 0.0f, 0.0f, color, tcoords.UpperLeftCorner.X, tcoords.LowerRightCorner.Y);	

	s16 indices[6] = {0,1,2,0,2,3};

	setVertexShader(EVT_STANDARD);
	
	pID3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, &indices[0],
										D3DFMT_INDEX16,&vtx[0],	sizeof(S3DVertex));
}



//! draw an 2d rectangle
void CVideoDirectX8::draw2DRectangle(Color color, const core::rectEx<s32>& position, const core::rectEx<s32>* clip)
{
	core::rectEx<s32> pos = position;

	if (clip)
		pos.clipAgainst(*clip);

	s32 xPlus = -(ScreenSize.Width>>1);
	f32 xFact = 1.0f / (ScreenSize.Width>>1);

	s32 yPlus = ScreenSize.Height-(ScreenSize.Height>>1);
	f32 yFact = 1.0f / (ScreenSize.Height>>1);

    S3DVertex vtx[4];
	vtx[0] = S3DVertex((f32)(pos.UpperLeftCorner.X+xPlus) * xFact, (f32)(yPlus-pos.UpperLeftCorner.Y) * yFact , 0.0f, 0.0f, 0.0f, 0.0f, color, 0.0f, 0.0f);
	vtx[1] = S3DVertex((f32)(pos.LowerRightCorner.X+xPlus) * xFact, (f32)(yPlus- pos.UpperLeftCorner.Y) * yFact, 0.0f, 0.0f, 0.0f, 0.0f, color, 0.0f, 1.0f);
	vtx[2] = S3DVertex((f32)(pos.LowerRightCorner.X+xPlus) * xFact, (f32)(yPlus-pos.LowerRightCorner.Y) * yFact, 0.0f, 0.0f, 0.0f, 0.0f, color, 1.0f, 0.0f);
	vtx[3] = S3DVertex((f32)(pos.UpperLeftCorner.X+xPlus) * xFact, (f32)(yPlus-pos.LowerRightCorner.Y) * yFact, 0.0f, 0.0f, 0.0f, 0.0f, color, 1.0f, 1.0f);	

	s16 indices[6] = {0,1,2,0,2,3};

	setRenderStates2DMode(color.getAlpha() < 255, false, false);
	setTexture(0,0);

	setVertexShader(EVT_STANDARD);

	pID3DDevice->DrawIndexedPrimitiveUP(	D3DPT_TRIANGLELIST, 0, 4, 2, &indices[0],
											D3DFMT_INDEX16, &vtx[0], sizeof(S3DVertex));
}


//! sets right vertex shader
void CVideoDirectX8::setVertexShader(E_VERTEX_TYPE newType)
{
	if (newType != LastVertexType)
	{
		LastVertexType = newType;
		HRESULT hr = 0;

		switch(newType)
		{
		case EVT_STANDARD:
			//pID3DDevice->SetVertexShader(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);		
			break;
		case EVT_2TCOORDS:
			//hr = pID3DDevice->SetVertexShader(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2);
			break;
		}

		if (FAILED(hr))
		{
			os::Warning::print("Could not set vertex Shader.");
			return;
		}
	}
}


//! sets the needed renderstates
void CVideoDirectX8::setRenderStates3DMode()
{
	if (!pID3DDevice)
		return;

	if (CurrentRenderMode == ERM_2D)
	{
		// switch back the matrices
		pID3DDevice->SetTransform(D3DTS_VIEW, (D3DMATRIX*)((void*)&Matrices[TS_VIEW]));
		pID3DDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)((void*)&Matrices[TS_WORLD]));
		pID3DDevice->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)((void*)&Matrices[TS_PROJECTION]));

		ResetRenderStates = true;
	}

	if (ResetRenderStates ||
		LastMaterial.AmbientColor != Material.AmbientColor ||
		LastMaterial.DiffuseColor != Material.DiffuseColor ||
		LastMaterial.SpecularColor != Material.SpecularColor ||
		LastMaterial.EmissiveColor != Material.EmissiveColor ||
		LastMaterial.Shininess != Material.Shininess)
	{
		D3DMATERIAL9 mat;
		mat.Diffuse = colorToD3D(Material.DiffuseColor);
		mat.Ambient = colorToD3D(Material.AmbientColor);
		mat.Specular = colorToD3D(Material.SpecularColor);
		mat.Emissive = colorToD3D(Material.EmissiveColor);
		mat.Power = Material.Shininess;
		pID3DDevice->SetMaterial(&mat);
	}

	// blendmode

	if (ResetRenderStates || LastMaterial.MaterialType != Material.MaterialType)
	{
		switch(Material.MaterialType)
		{
		case EMT_SOLID:
			pID3DDevice->SetTextureStageState (0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			pID3DDevice->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			pID3DDevice->SetTextureStageState (0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
			pID3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP,  D3DTOP_DISABLE);

			pID3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
			pID3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			pID3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			break;
		case EMT_TRANSPARENT_ADD_COLOR:
			pID3DDevice->SetTextureStageState (0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			pID3DDevice->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			pID3DDevice->SetTextureStageState (0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
			pID3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP,  D3DTOP_DISABLE);

			pID3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
			pID3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			pID3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			pID3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
			pID3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			break;
		case EMT_TRANSPARENT_ALPHA_CHANNEL:
			// not implemented yet.
			break;
		case EMT_LIGHTMAP:
			pID3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			pID3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			pID3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,  D3DTOP_DISABLE);

			pID3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE4X);
			pID3DDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			pID3DDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);

			pID3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);		
			pID3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			break;
		}
	}

	// bilinear

	if (ResetRenderStates || LastMaterial.BilinearFilter != Material.BilinearFilter)
	{
		if (Material.BilinearFilter)
		{
			//pID3DDevice->SetTextureStageState (0, D3DTSS_MAGFILTER,  D3DTEXF_LINEAR);
			//pID3DDevice->SetTextureStageState (0, D3DTSS_MINFILTER,  D3DTEXF_LINEAR);
			//pID3DDevice->SetTextureStageState (0, D3DTSS_MIPFILTER, D3DTEXF_POINT);
			
			//pID3DDevice->SetTextureStageState (1, D3DTSS_MAGFILTER,  D3DTEXF_LINEAR);
			//pID3DDevice->SetTextureStageState (1, D3DTSS_MINFILTER,  D3DTEXF_LINEAR);
			//pID3DDevice->SetTextureStageState (1, D3DTSS_MIPFILTER, D3DTEXF_POINT);

			pID3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR); //改动后，DX9
			pID3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR); //改动后，DX9
			pID3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT); //改动后，DX9

			pID3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR); //改动后，DX9
			pID3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR); //改动后，DX9
			pID3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_POINT); //改动后，DX9
		}
		else
		{
			//pID3DDevice->SetTextureStageState (0, D3DTSS_MINFILTER,  D3DTEXF_POINT);
			//pID3DDevice->SetTextureStageState (0, D3DTSS_MIPFILTER, D3DTEXF_NONE);
			//pID3DDevice->SetTextureStageState (0, D3DTSS_MAGFILTER,  D3DTEXF_POINT);
			
			//pID3DDevice->SetTextureStageState (1, D3DTSS_MINFILTER,  D3DTEXF_POINT);
			//pID3DDevice->SetTextureStageState (1, D3DTSS_MIPFILTER, D3DTEXF_NONE);
			//pID3DDevice->SetTextureStageState (1, D3DTSS_MAGFILTER,  D3DTEXF_POINT);

			pID3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT); //改动后，DX9
			pID3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE); //改动后，DX9
			pID3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT); //改动后，DX9

			pID3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_POINT); //改动后，DX9
			pID3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_NONE); //改动后，DX9
			pID3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_POINT); //改动后，DX9
		}
	}

	// fillmode

	if (ResetRenderStates || LastMaterial.Wireframe != Material.Wireframe)
	{
		if (!Material.Wireframe)
			pID3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		else
			pID3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}

	// shademode

	if (ResetRenderStates || LastMaterial.GouraudShading != Material.GouraudShading)
	{
		if (Material.GouraudShading)
			pID3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		else
			pID3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
	}

	// lighting

	if (ResetRenderStates || LastMaterial.Lighting != Material.Lighting)
	{
		if (Material.Lighting)
			pID3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		else
			pID3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}

	// zbuffer

	if (ResetRenderStates || LastMaterial.ZBuffer != Material.ZBuffer)
	{
		if (Material.ZBuffer)
			pID3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		else
			pID3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	}

	// zwrite

	if (!Material.isTransparent())
	{
		if (ResetRenderStates || LastMaterial.ZWriteEnable != Material.ZWriteEnable)
		{
			if (Material.ZWriteEnable)
				pID3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
			else
				pID3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
		}
	}

	// back face culling

	if (ResetRenderStates || LastMaterial.BackfaceCulling != Material.BackfaceCulling)
	{
		if (Material.BackfaceCulling)
			pID3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
		else
			pID3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);
	}

	LastMaterial = Material;

	ResetRenderStates = false;

	CurrentRenderMode = ERM_3D;
}


//! sets the needed renderstates
void CVideoDirectX8::setRenderStates2DMode(bool alpha, bool texture, bool alphaChannel)
{
	if (!pID3DDevice)
		return;

	if (CurrentRenderMode != ERM_2D)
	{
		core::matrix4 mat;
		pID3DDevice->SetTransform(D3DTS_VIEW, (D3DMATRIX*)((void*)&mat));
		pID3DDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)((void*)&mat));
		pID3DDevice->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)((void*)&mat));

		pID3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		pID3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
		pID3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		pID3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		pID3DDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
		pID3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,  D3DTOP_DISABLE );

	}

	if (texture)
	{
		//pID3DDevice->SetTextureStageState (0, D3DTSS_MINFILTER,  D3DTEXF_POINT);
		//pID3DDevice->SetTextureStageState (0, D3DTSS_MIPFILTER, D3DTEXF_NONE);
		//pID3DDevice->SetTextureStageState (0, D3DTSS_MAGFILTER,  D3DTEXF_POINT);

		pID3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT); //改动后，DX9
		pID3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE); //改动后，DX9
		pID3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);	//改动后，DX9


		if (alphaChannel)
		{
			pID3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
			pID3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			pID3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

			pID3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP,  D3DTOP_SELECTARG1 );
			pID3DDevice->SetTextureStageState (0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

			pID3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			pID3DDevice->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ONE);
			pID3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
		else
		{
			if (alpha)
			{
				pID3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
				pID3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
				pID3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				pID3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pID3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			}
			else
			{
				pID3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
				pID3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
				pID3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
				pID3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP,  D3DTOP_DISABLE);
				pID3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			}
		}

	}
	else
	{
		if (alpha)
		{
			pID3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
			pID3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			pID3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
			pID3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP,  D3DTOP_SELECTARG1);
			pID3DDevice->SetTextureStageState (0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
			pID3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			pID3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pID3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		}
		else
		{
			pID3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
			pID3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			pID3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
			pID3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP,  D3DTOP_DISABLE);
			pID3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		}
	}

	CurrentRenderMode = ERM_2D;
}


//! deletes all dynamic lights there are
void CVideoDirectX8::deleteAllDynamicLights()
{
	for (s32 i=0; i<LastSetLight+1; ++i)
		pID3DDevice->LightEnable(i, false);

	LastSetLight = -1;
}



//! adds a dynamic light
void CVideoDirectX8::addDynamicLight(const SLight& dl)
{
	if ((u32)LastSetLight == Caps.MaxActiveLights-1)
		return;

	D3DLIGHT9 light;

	light.Type = D3DLIGHT_POINT;
	light.Diffuse = *(D3DCOLORVALUE*)((void*)(&dl.DiffuseColor));
	light.Specular = *(D3DCOLORVALUE*)((void*)(&dl.SpecularColor));
	light.Ambient = *(D3DCOLORVALUE*)((void*)(&dl.AmbientColor));
	light.Position = *(D3DVECTOR*)((void*)(&dl.Position));
	light.Range = dl.Range;
	light.Attenuation0 = dl.ConstantAttenuation;
	light.Attenuation1 = dl.LinearAttenuation;
	light.Attenuation2 = dl.QuadraticAttenuation;

	++LastSetLight;
	pID3DDevice->SetLight(LastSetLight, &light);
	pID3DDevice->LightEnable(LastSetLight, true);
}



//! returns the maximal amount of dynamic lights the device can handle
s32 CVideoDirectX8::getMaximalDynamicLightAmount()
{
	return Caps.MaxActiveLights;
}



//! creates a video driver
IVideoDriver* createDirectX8Driver(const core::dimension2d<s32>& screenSize, HWND window, 
								   u32 bits, bool fullscreen, io::IFileSystem* io, bool pureSoftware)
{
	CVideoDirectX8* dx8 =  new CVideoDirectX8(screenSize, window, fullscreen, io, pureSoftware);
	if (!dx8->initDriver(screenSize, window, bits, fullscreen, pureSoftware))
	{
		dx8->drop();
		dx8 = 0;
	}

	return dx8;
}

} // end namespace video
} // end namespace irr

#endif