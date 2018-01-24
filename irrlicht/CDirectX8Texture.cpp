#include "CDirectX8Texture.h"
#include "os.h"

#include <stdio.h>
#include <d3dx9tex.h>
#pragma comment(lib, "d3dx9.lib")

namespace irr
{
namespace video  
{

//! constructor
CDirectX8Texture::CDirectX8Texture(ISurface* surface, IDirect3DDevice9* device,
								   bool generateMipLevels)
: Surface(surface), Device(device), TextureSize(0,0), Texture(0), Pitch(0), SurfaceSize(0,0)
{
	#ifdef _DEBUG
    setDebugName("CDirectX8Texture");
	#endif

	if (Device)
		Device->AddRef();

	if (Surface)
	{
		Surface->grab();

		core::dimension2d<s32> optSize;
		SurfaceSize = Surface->getDimension();

		optSize.Width = getTextureSizeFromSurfaceSize(SurfaceSize.Width);
		optSize.Height = getTextureSizeFromSurfaceSize(SurfaceSize.Height);

		HRESULT hr = Device->CreateTexture(optSize.Width, optSize.Height,
			generateMipLevels ? 0 : 1, 0, D3DFMT_A1R5G5B5, D3DPOOL_MANAGED, &Texture, 0);

		if (Texture)
		{
			if (copyTexture() && generateMipLevels)
			{
				// create mip maps.
				hr  = D3DXFilterTexture(Texture, NULL, 0 , D3DX_DEFAULT );
				if (FAILED(hr))
					os::Warning::print("Could not create mip map levels.");
			}				
		}
		else
			os::Warning::print("Could not create DirectX8 Texture.");
	}
}



//! copies the surface to the texture
bool CDirectX8Texture::copyTexture()
{
	if (Texture && Surface)
	{
		D3DSURFACE_DESC desc;
		Texture->GetLevelDesc(0, &desc);
		TextureSize.Width = desc.Width;
		TextureSize.Height = desc.Height;

		SufaceHasSameSize = (TextureSize == SurfaceSize);

		if (desc.Format != D3DFMT_A1R5G5B5)
		{
			os::Warning::print("The directX8 device does not seem to support A1R5G5B5 textures. Texture not loaded.");
			return false;
		}

		D3DLOCKED_RECT rect;
		HRESULT hr = Texture->LockRect(0, &rect, 0, 0);
		if (FAILED(hr))
		{
			os::Warning::print("Could not lock DirectX8 Texture.");
			return false;
		}

		s16* dest = (s16*)rect.pBits;
		s16* source = Surface->lock();
		Pitch = rect.Pitch;


		if (SufaceHasSameSize)
		{
			s32 pitch = rect.Pitch/2;

			for (s32 x=0; x<SurfaceSize.Width; ++x)
				for (s32 y=0; y<SurfaceSize.Height; ++y)
					dest[x + y*pitch] = source[x + y * SurfaceSize.Width];
		}
		else
		{
			f32 sourceXStep = (f32)SurfaceSize.Width / (f32)TextureSize.Width;
			f32 sourceYStep = (f32)SurfaceSize.Height / (f32)TextureSize.Height;
			f32 sy;
			s32 pitch = rect.Pitch/2;

			for (s32 x=0; x<TextureSize.Width; ++x)
			{
				sy = 0.0f;

				for (s32 y=0; y<TextureSize.Height; ++y)
				{
					dest[(s32)(y*pitch + x)] = source[(s32)(((s32)sy)*SurfaceSize.Width + x*sourceXStep)];
					sy+=sourceYStep;
				}
			}
		}		

		Surface->unlock();

		hr = Texture->UnlockRect(0);
		if (FAILED(hr))
		{
			os::Warning::print("Could not unlock DirectX8 Texture.");
			return false;
		}
	}

	return true;
}



//! destructor
CDirectX8Texture::~CDirectX8Texture()
{
	if (Device)
		Device->Release();

	if (Surface)
		Surface->drop();

	if (Texture)
		Texture->Release();
}



//! lock function
void* CDirectX8Texture::lock()
{
	if (!Texture)
		return 0;

	D3DLOCKED_RECT rect;
	HRESULT hr = Texture->LockRect(0, &rect, 0, 0);
	if (FAILED(hr))
	{
		os::Warning::print("Could not lock DirectX8 Texture.");
		return 0;
	}

	return rect.pBits; 
}



//! unlock function
void CDirectX8Texture::unlock()
{
	if (!Texture)
		return;

	Texture->UnlockRect(0);
}



//! returns dimension of texture (=size)
const core::dimension2d<s32>& CDirectX8Texture::getDimension()
{
	return SurfaceSize;
}



//! returns the size of a texture which would be the optimize size for rendering it
inline s32 CDirectX8Texture::getTextureSizeFromSurfaceSize(s32 size)
{
	s32 ts = 0x01;
	while(ts < size)
		ts <<= 1;

	return ts;
}



//! returns driver type of texture (=the driver, who created the texture)
EDriverType CDirectX8Texture::getDriverType()
{
	return DT_DIRECTX8;
}



//! returns color format of texture
ECOLOR_FORMAT CDirectX8Texture::getColorFormat()
{
	return EHCF_R5G5B5;
}



//! returns pitch of texture (in bytes)
s32 CDirectX8Texture::getPitch()
{
	return Pitch;
}



//! returns the DirectX8 Texture
IDirect3DTexture9* CDirectX8Texture::getDX8Texture()
{
	return Texture;
}



} // end namespace video
} // end namespace irr
