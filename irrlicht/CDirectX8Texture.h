// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_DIRECTX8_TEXTURE_H_INCLUDED__
#define __C_DIRECTX8_TEXTURE_H_INCLUDED__

#include "ITexture.h"
#include "ISurface.h"
#include <d3d9.h>

namespace irr
{
namespace video  
{

/*!
	interface for a Video Driver dependent Texture.
*/
class CDirectX8Texture : public ITexture
{
public:

	//! constructor
	CDirectX8Texture(ISurface* surface, IDirect3DDevice9* device, bool generateMipLevels);

	//! destructor
	virtual ~CDirectX8Texture();

	//! lock function
	virtual void* lock();

	//! unlock function
	virtual void unlock();

	//! returns dimension of texture (=size)
	virtual const core::dimension2d<s32>& getDimension();

	//! returns driver type of texture (=the driver, who created the texture)
	virtual EDriverType getDriverType();

	//! returns color format of texture
	virtual ECOLOR_FORMAT getColorFormat();

	//! returns pitch of texture (in bytes)
	virtual s32 getPitch();

	//! returns the DirectX8 Texture
	IDirect3DTexture9* getDX8Texture() ;

private:

	//! returns the size of a texture which would be the optimize size for rendering it
	inline s32 getTextureSizeFromSurfaceSize(s32 size);

	//! copies the surface to the texture
	bool copyTexture();

	ISurface* Surface;
	IDirect3DDevice9* Device;
	IDirect3DTexture9* Texture;
	core::dimension2d<s32> TextureSize;
	core::dimension2d<s32> SurfaceSize;
	s32 Pitch;
	bool SufaceHasSameSize; // true if Surface has the same dimension as texture.
};


} // end namespace video
} // end namespace irr

#endif