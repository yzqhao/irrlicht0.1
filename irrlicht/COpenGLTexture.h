// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_OPEN_GL_TEXTURE_H_INCLUDED__
#define __C_OPEN_GL_TEXTURE_H_INCLUDED__

#include "ITexture.h"
#include "ISurface.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#endif


namespace irr
{
namespace video  
{

//! OpenGL texture.
class COpenGLTexture : public ITexture
{
public:

	//! constructor
	COpenGLTexture(ISurface* surface, bool generateMipLevels);

	//! destructor
	virtual ~COpenGLTexture();

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

	//! return open gl texture name
	GLuint getOpenGLTextureName();

private:

	//! copies the the texture into an open gl texture.
	void copyTexture();
	
	//! returns the size of a texture which would be the optimize size for rendering it
	inline s32 getTextureSizeFromSurfaceSize(s32 size);

	ISurface* Surface;
	core::dimension2d<s32> SurfaceSize;
	s32 Pitch;
	bool SufaceHasSameSize; // true if Surface has the same dimension as texture.
	
	GLuint textureName;
};


} // end namespace video
} // end namespace irr

#endif