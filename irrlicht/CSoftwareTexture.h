// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_SOFTWARE_TEXTURE_H_INCLUDED__
#define __C_SOFTWARE_TEXTURE_H_INCLUDED__

#include "ITexture.h"
#include "ISurface.h"

namespace irr
{
namespace video  
{

/*!
	interface for a Video Driver dependent Texture.
*/
class CSoftwareTexture : public ITexture
{
public:

	//! constructor
	CSoftwareTexture(ISurface* surface);

	//! destructor
	virtual ~CSoftwareTexture();

	//! lock function
	virtual void* lock();

	//! unlock function
	virtual void unlock();

	//! returns dimension of texture (=size)
	virtual const core::dimension2d<s32>& getDimension();

	//! returns unoptimized surface
	virtual ISurface* getSurface();

	//! returns texture surface
	virtual ISurface* getTexture();

	//! returns driver type of texture (=the driver, who created the texture)
	virtual EDriverType getDriverType();

	//! returns color format of texture
	virtual ECOLOR_FORMAT getColorFormat();

	//! returns pitch of texture (in bytes)
	virtual s32 getPitch();

private:

	//! returns the size of a texture which would be the optimize size for rendering it
	inline s32 getTextureSizeFromSurfaceSize(s32 size);

	ISurface* Surface;
	ISurface* Texture;

};


} // end namespace video
} // end namespace irr

#endif