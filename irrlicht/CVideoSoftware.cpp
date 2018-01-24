#include "CVideoSoftware.h"
#include "CSoftwareTexture.h"
#include "os.h"
#include "S3DVertex.h"

namespace irr
{
namespace video
{


//! constructor
CVideoSoftware::CVideoSoftware(const core::dimension2d<s32>& windowSize, bool fullscreen, io::IFileSystem* io, video::ISurfacePresenter* presenter)
: CVideoNull(io, windowSize), CurrentTriangleRenderer(0), Texture(0),
	 ZBuffer(0), RenderTargetTexture(0), RenderTargetSurface(0)
{
	#ifdef _DEBUG
	setDebugName("CVideoSoftware");
	#endif

	os::Debuginfo::print("Warning: The 3d pipeline of the software device is not 100% implemented yet!");

	// create backbuffer

	BackBuffer = video::createSurface(windowSize);
	BackBuffer->fill(0);
	
	// get presenter

	Presenter = presenter;

	// create z buffer

	ZBuffer = irr::video::createZBuffer(BackBuffer->getDimension());

	// create triangle renderers

	TriangleRenderers[ETR_FLAT] = createTriangleRendererFlat(ZBuffer);
	TriangleRenderers[ETR_FLAT_WIRE] = createTriangleRendererFlatWire(ZBuffer);
	TriangleRenderers[ETR_GOURAUD] = createTriangleRendererGouraud(ZBuffer);
	TriangleRenderers[ETR_GOURAUD_WIRE] = createTriangleRendererGouraudWire(ZBuffer);
	TriangleRenderers[ETR_TEXTURE_FLAT] = createTriangleRendererTextureFlat(ZBuffer);
	TriangleRenderers[ETR_TEXTURE_FLAT_WIRE] = createTriangleRendererTextureFlatWire(ZBuffer);
	TriangleRenderers[ETR_TEXTURE_GOURAUD] = createTriangleRendererTextureGouraud(ZBuffer);
	TriangleRenderers[ETR_TEXTURE_GOURAUD_WIRE] = createTriangleRendererTextureGouraudWire(ZBuffer);

	// select render target

	setRenderTarget(BackBuffer);

	// select the right renderer

	selectRightTriangleRenderer();
}



//! destructor
CVideoSoftware::~CVideoSoftware()
{
	// delete Backbuffer
	BackBuffer->drop();

	// delete triangle renderers

	for (s32 i=0; i<ETR_COUNT; ++i)
		if (TriangleRenderers[i])
			TriangleRenderers[i]->drop();

	// delete zbuffer

	if (ZBuffer)
		ZBuffer->drop();

	// delete current texture

	if (Texture)
		Texture->drop();

	if (RenderTargetTexture)
		RenderTargetTexture->drop();

	if (RenderTargetSurface)
		RenderTargetSurface->drop();
}



//! switches to a triangle renderer
void CVideoSoftware::switchToTriangleRenderer(ETriangleRenderer renderer)
{
	video::ISurface* s = 0;
	if (Texture)
		s = ((CSoftwareTexture*)Texture)->getTexture();

	CurrentTriangleRenderer = TriangleRenderers[renderer];
	CurrentTriangleRenderer->setBackfaceCulling(Material.BackfaceCulling == true);
	CurrentTriangleRenderer->setTexture(s);
	CurrentTriangleRenderer->setRenderTarget(RenderTargetSurface, ViewPort);
}


//! void selects the right triangle renderer based on the render states.
void CVideoSoftware::selectRightTriangleRenderer()
{
	ETriangleRenderer renderer = ETR_FLAT;

	if (Texture)
	{
		if (!Material.GouraudShading)
			renderer = (!Material.Wireframe) ? ETR_TEXTURE_FLAT : ETR_TEXTURE_FLAT_WIRE;
		else
			renderer = (!Material.Wireframe) ? ETR_TEXTURE_GOURAUD : ETR_TEXTURE_GOURAUD_WIRE;
	}
	else
	{
		if (!Material.GouraudShading)
			renderer = (!Material.Wireframe) ? ETR_FLAT : ETR_FLAT_WIRE;
		else
			renderer = (!Material.Wireframe) ? ETR_GOURAUD : ETR_GOURAUD_WIRE;
	}

	switchToTriangleRenderer(renderer);
}




//! presents the rendered scene on the screen, returns false if failed
bool CVideoSoftware::endScene()
{
	CVideoNull::endScene();

	Presenter->present(BackBuffer);
	return true;
}



//! queries the features of the driver, returns true if feature is available
bool CVideoSoftware::queryFeature(EK3D_VIDEO_DRIVER_FEATURE feature)
{
	switch (feature)
	{
	case EK3DVDF_BILINEAR_FILER:
		return false;
	case EK3DVDF_RENDER_TO_TARGET:
		return true;
	case EK3DVDF_HARDWARE_TL:
		return false;
	case EK3DVDF_MIP_MAP:
		return false;
	};

	return false;
}



//! sets transformation
void CVideoSoftware::setTransform(E_TRANSFORMATION_STATE state, const core::matrix4& mat)
{
	TransformationMatrix[state] = mat;
}




//! sets the current Texture
void CVideoSoftware::setTexture(video::ITexture* texture)
{
	#ifdef _DEBUG
	if (texture && texture->getDriverType() != DT_SOFTWARE)
	{
		os::Debuginfo::print("Fatal Error: Tried to set a texture not owned by this driver.");
		return;
	}
	#endif

	video::ITexture* oldTexture = texture;

	if (Texture)
		Texture->drop();

	Texture = texture;

	if (Texture)
		Texture->grab();

	selectRightTriangleRenderer();
}



//! sets a material
void CVideoSoftware::setMaterial(const SMaterial& material)
{
	Material = material;
	setTexture(Material.Texture1);
}


//! clears the zbuffer
bool CVideoSoftware::beginScene(bool backBuffer, bool zBuffer, Color color)
{
	CVideoNull::beginScene(backBuffer, zBuffer, color);

	if (backBuffer)
		BackBuffer->fill(color.toA1R5G5B5());

	if (ZBuffer)
		ZBuffer->clear();

	return true;
}


//! sets a render target
void CVideoSoftware::setRenderTarget(video::ITexture* texture)
{
	#ifdef _DEBUG
	if (texture->getDriverType() != DT_SOFTWARE)
	{
		os::Debuginfo::print("Fatal Error: Tried to set a texture not owned by this driver.");
		return;
	}
	#endif

	if (RenderTargetTexture)
		RenderTargetTexture->drop();

	RenderTargetTexture = texture;

	if (RenderTargetTexture)
	{
		RenderTargetTexture->grab();
		setRenderTarget(((CSoftwareTexture*)RenderTargetTexture)->getTexture());
	}
	else
		setRenderTarget((video::ISurface*)0);
}


//! sets a render target
void CVideoSoftware::setRenderTarget(video::ISurface* surface)
{
	if (RenderTargetSurface)
		RenderTargetSurface->drop();

	RenderTargetSurface = surface;
	RenderTargetSize.Width = 0;
	RenderTargetSize.Height = 0;
	Render2DTranslation.X = 0;
	Render2DTranslation.Y = 0;

	if (RenderTargetSurface)
	{
		RenderTargetSurface->grab();
		RenderTargetSize = RenderTargetSurface->getDimension();
	}

	setViewPort(core::rectEx<s32>(0,0,RenderTargetSize.Width,RenderTargetSize.Height));

	if (ZBuffer)
		ZBuffer->setSize(RenderTargetSize);
}



//! sets a viewport
void CVideoSoftware::setViewPort(const core::rectEx<s32>& area)
{
	ViewPort = area;

	//TODO: the clipping is not correct, because the projection is affected.
	// to correct this, ViewPortSize and Render2DTranslation will have to be corrected.
	core::rectEx<s32> rendert(0,0,RenderTargetSize.Width,RenderTargetSize.Height);
	ViewPort.clipAgainst(rendert);

	ViewPortSize.Width = ViewPort.getWidth();
	ViewPortSize.Height = ViewPort.getHeight();
	Render2DTranslation.X = (ViewPortSize.Width / 2) + ViewPort.UpperLeftCorner.X;
	Render2DTranslation.Y = ViewPort.UpperLeftCorner.Y + ViewPortSize.Height - (ViewPortSize.Height / 2);// + ViewPort.UpperLeftCorner.Y;

	if (CurrentTriangleRenderer)
		CurrentTriangleRenderer->setRenderTarget(RenderTargetSurface, ViewPort);
}



//! draws an indexed triangle list
void CVideoSoftware::drawIndexedTriangleList(const S3DVertex* vertices, s32 vertexCount, const u16* indexList, s32 triangleCount)
{
	if (!RenderTargetSurface || !ZBuffer)
		return;

	CVideoNull::drawIndexedTriangleList(vertices, vertexCount, indexList, triangleCount);

	if ((s32)TransformedPoints.size() < vertexCount)
		TransformedPoints.set_used(vertexCount);

	const S3DVertex* currentVertex = vertices;
	S2DVertex* tp = &TransformedPoints[0];

	core::dimension2d<s32> textureSize(0,0);
	f32 zDiv;

	if (Texture)
		textureSize = ((CSoftwareTexture*)Texture)->getTexture()->getDimension();

	f32 transformedPos[4]; // transform all points in the list

	core::matrix4 matrix(TransformationMatrix[TS_PROJECTION]);
	matrix *= TransformationMatrix[TS_VIEW];
	matrix *= TransformationMatrix[TS_WORLD];

	s32 ViewTransformWidth = (ViewPortSize.Width>>1);
	s32 ViewTransformHeight = (ViewPortSize.Height>>1);

	for (s32 i=0; i<vertexCount; ++i)
	{
		transformedPos[0] = currentVertex->Pos.X;
		transformedPos[1] = currentVertex->Pos.Y;
		transformedPos[2] = currentVertex->Pos.Z;
		transformedPos[3] = 1.0f;

		matrix.multiplyWith1x4Matrix(transformedPos);
		zDiv = transformedPos[3] == 0.0f ? 1.0f : (1.0f / transformedPos[3]);

		tp->Pos.X = (s32)(ViewTransformWidth * (transformedPos[0] * zDiv) + (Render2DTranslation.X));
		tp->Pos.Y = (Render2DTranslation.Y - (s32)(ViewTransformHeight * (transformedPos[1] * zDiv)));
		tp->Color = currentVertex->Color.toA1R5G5B5();
		tp->ZValue = (TZBufferType)(32767.0f * zDiv);

		tp->TCoords.X = (s32)(currentVertex->TCoords.X * textureSize.Width);
		tp->TCoords.X <<= 8;
		tp->TCoords.Y = (s32)(currentVertex->TCoords.Y * textureSize.Height);
		tp->TCoords.Y <<= 8;

		++currentVertex;
		++tp;
	}


	// draw all transformed points from the index list
	CurrentTriangleRenderer->drawIndexedTriangleList(&TransformedPoints[0],
		vertexCount, indexList, triangleCount);
}



//! draws an indexed triangle list
void CVideoSoftware::drawIndexedTriangleList(const S3DVertex2TCoords* vertices, s32 vertexCount, const u16* indexList, s32 triangleCount)
{
	if (!RenderTargetSurface || !ZBuffer)
		return;

	CVideoNull::drawIndexedTriangleList(vertices, vertexCount, indexList, triangleCount);

	if ((s32)TransformedPoints.size() < vertexCount)
		TransformedPoints.set_used(vertexCount);

	const S3DVertex2TCoords* currentVertex = vertices;
	S2DVertex* tp = &TransformedPoints[0];

	core::dimension2d<s32> textureSize(0,0);
	f32 zDiv;

	if (Texture)
		textureSize = ((CSoftwareTexture*)Texture)->getTexture()->getDimension();

	f32 transformedPos[4]; // transform all points in the list

	core::matrix4 matrix(TransformationMatrix[TS_PROJECTION]);
	matrix *= TransformationMatrix[TS_VIEW];
	matrix *= TransformationMatrix[TS_WORLD];

	s32 ViewTransformWidth = (ViewPortSize.Width>>1);
	s32 ViewTransformHeight = (ViewPortSize.Height>>1);

	for (s32 i=0; i<vertexCount; ++i)
	{
		transformedPos[0] = currentVertex->Pos.X;
		transformedPos[1] = currentVertex->Pos.Y;
		transformedPos[2] = currentVertex->Pos.Z;
		transformedPos[3] = 1.0f;

		matrix.multiplyWith1x4Matrix(transformedPos);
		zDiv = transformedPos[3] == 0.0f ? 1.0f : (1.0f / transformedPos[3]);

		tp->Pos.X = (s32)(ViewTransformWidth * (transformedPos[0] * zDiv) + (Render2DTranslation.X));
		tp->Pos.Y = (Render2DTranslation.Y - (s32)(ViewTransformHeight * (transformedPos[1] * zDiv)));
		tp->Color = currentVertex->Color.toA1R5G5B5();
		tp->ZValue = (TZBufferType)(32767.0f * zDiv);

		tp->TCoords.X = (s32)(currentVertex->TCoords.X * textureSize.Width);
		tp->TCoords.X <<= 8;
		tp->TCoords.Y = (s32)(currentVertex->TCoords.Y * textureSize.Height);
		tp->TCoords.Y <<= 8;

		++currentVertex;
		++tp;
	}


	// draw all transformed points from the index list
	CurrentTriangleRenderer->drawIndexedTriangleList(&TransformedPoints[0],
		vertexCount, indexList, triangleCount);
}



//! draws an 2d image
void CVideoSoftware::draw2DImage(video::ITexture* texture, const core::position2d<s32>& destPos)
{
	if (texture)
	{
		#ifdef _DEBUG
		if (texture->getDriverType() != DT_SOFTWARE)
		{
			os::Debuginfo::print("Fatal Error: Tried to copy from a surface not owned by this driver.");
			return;
		}
		#endif

		((CSoftwareTexture*)texture)->getSurface()->copyTo(BackBuffer, destPos);
	}
}




//! draws an 2d image, using a color (if color is other then Color(255,255,255,255)) and the alpha channel of the texture if wanted.
void CVideoSoftware::draw2DImage(video::ITexture* texture, const core::position2d<s32>& destPos, const core::rectEx<s32>& sourceRect, const core::rectEx<s32>* clipRect, Color color, bool useAlphaChannelOfTexture)
{
	if (texture)
	{
		#ifdef _DEBUG
		if (texture->getDriverType() != DT_SOFTWARE)
		{
			os::Debuginfo::print("Fatal Error: Tried to copy from a surface not owned by this driver.");
			return;
		}
		#endif

		if (useAlphaChannelOfTexture)
			((CSoftwareTexture*)texture)->getSurface()->copyToWithAlpha(BackBuffer, destPos, sourceRect, color.toA1R5G5B5(), clipRect);
		else
			((CSoftwareTexture*)texture)->getSurface()->copyTo(BackBuffer, destPos, sourceRect, clipRect);
	}
}



//! draw an 2d rectangle
void CVideoSoftware::draw2DRectangle(Color color, const core::rectEx<s32>& pos, const core::rectEx<s32>* clip)
{
	if (clip)
	{
		core::rectEx<s32> p(pos);
		p.clipAgainst(*clip);
		BackBuffer->drawRectangle(p, color);
	}
	else
		BackBuffer->drawRectangle(pos, color);
}



//! creates a video driver
IVideoDriver* createSoftwareDriver(const core::dimension2d<s32>& windowSize, bool fullscreen, io::IFileSystem* io, video::ISurfacePresenter* presenter)
{
	return new CVideoSoftware(windowSize, fullscreen, io, presenter);
}


} // end namespace video
} // end namespace irr
