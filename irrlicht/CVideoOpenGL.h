// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_VIDEO_OPEN_GL_H_INCLUDED__
#define __C_VIDEO_OPEN_GL_H_INCLUDED__

#include "CVideoNull.h"

#ifdef WIN32
// include windows headers for HWND
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLu32.lib")
#endif

namespace irr
{
namespace video
{
	class CVideoOpenGL : public CVideoNull
	{
	public:

		#ifdef WIN32
		//! win32 constructor
		CVideoOpenGL(const core::dimension2d<s32>& screenSize, HWND window, bool fullscreen, io::IFileSystem* io);

		//! inits the open gl driver
		bool initDriver(const core::dimension2d<s32>& screenSize, HWND window, bool fullscreen);
		#endif

		//! destructor
		virtual ~CVideoOpenGL();

		//! presents the rendered scene on the screen, returns false if failed
		virtual bool endScene();

		//! clears the zbuffer
		virtual bool beginScene(bool backBuffer, bool zBuffer, Color color);

		//! sets transformation
		virtual void setTransform(E_TRANSFORMATION_STATE state, const core::matrix4& mat);

		//! draws an indexed triangle list
		virtual void drawIndexedTriangleList(const S3DVertex* vertices, s32 vertexCount, const u16* indexList, s32 triangleCount);

		//! draws an indexed triangle list
		virtual void drawIndexedTriangleList(const S3DVertex2TCoords* vertices, s32 vertexCount, const u16* indexList, s32 triangleCount);

		//! queries the features of the driver, returns true if feature is available
		bool queryFeature(EK3D_VIDEO_DRIVER_FEATURE feature);

		//! Sets a material. All 3d drawing functions draw geometry now
		//! using this material.
		//! \param material: Material to be used from now on.
		virtual void setMaterial(const SMaterial& material);

	private:

		//! returns a device dependent texture from a software surface (ISurface)
		//! THIS METHOD HAS TO BE OVERRIDDEN BY DERIVED DRIVERS WITH OWN TEXTURES
		virtual video::ITexture* createDeviceDependentTexture(ISurface* surface, bool generateMipLevels);

		inline void createGLMatrix(GLfloat gl_matrix[16], const core::matrix4& m)
		{
			s32 i = 0;
			for (s32 r=0; r<4; ++r)
				for (s32 c=0; c<4; ++c)
					gl_matrix[i++] = m(c,r);
		}

		core::matrix4 Matrizes[TS_COUNT];
		core::array<s32> ColorBuffer;

		#ifdef WIN32		
		HDC HDc; // Private GDI Device Context
		HWND Window;
		#endif

		HGLRC HRc; // Permanent Rendering Context

	};

} // end namespace video
} // end namespace irr


#endif
