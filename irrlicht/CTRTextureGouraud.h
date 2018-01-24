// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_TRIANGLE_RENDERER_TEXTURE_GOURAUD_H_INCLUDED__
#define __C_TRIANGLE_RENDERER_TEXTURE_GOURAUD_H_INCLUDED__

#include "IK3DTriangleRenderer.h"
#include "rect.h"

namespace irr
{
namespace video
{

	class CTRTextureGouraud : public IK3DTriangleRenderer
	{
	public:

		//! constructor
		CTRTextureGouraud(IZBuffer* zbuffer);
	
		//! destructor
		virtual ~CTRTextureGouraud();

		//! sets a render target
		virtual void setRenderTarget(video::ISurface* surface, const core::rectEx<s32>& viewPort);

		//! draws an indexed triangle list
		virtual void drawIndexedTriangleList(S2DVertex* vertices, s32 vertexCount, const u16* indexList, s32 triangleCount);

		//! en or disables the backface culling
		virtual void setBackfaceCulling(bool enabled = true);

		//! sets the Texture
		virtual void setTexture(video::ISurface* texture);

	protected:

		//! vertauscht zwei vertizen
		inline void swapVertices(const S2DVertex** v1, const S2DVertex** v2)
		{
			const S2DVertex* b = *v1;
			*v1 = *v2;
			*v2 = b;
		}

		video::ISurface* RenderTarget;
		core::rectEx<s32> ViewPortRect;

		IZBuffer* ZBuffer;

		s32 SurfaceWidth;
		s32 SurfaceHeight;
		bool BackFaceCullingEnabled;
		TZBufferType* lockedZBuffer;
		s16* lockedSurface;
		s16* lockedTexture;
		s32 lockedTextureWidth;
		s32 textureXMask, textureYMask;
		video::ISurface* Texture;
	};

} // end namespace video
} // end namespace irr

#endif