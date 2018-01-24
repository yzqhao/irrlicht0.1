// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __I_K_3D_TRIANGLE_RENDERER_H_INCLUDED__
#define __I_K_3D_TRIANGLE_RENDERER_H_INCLUDED__

#include "IUnknown.h"
#include "ISurface.h"
#include "S2DVertex.h"
#include "rect.h"
#include "IZBuffer.h"
#include "ISurface.h"

namespace irr
{
namespace video
{

	enum ETriangleRenderer
	{
		ETR_FLAT = 0,
		ETR_FLAT_WIRE,
		ETR_GOURAUD,
		ETR_GOURAUD_WIRE,
		ETR_TEXTURE_FLAT,
		ETR_TEXTURE_FLAT_WIRE,
		ETR_TEXTURE_GOURAUD,
		ETR_TEXTURE_GOURAUD_WIRE,
		ETR_COUNT
	};

	class IK3DTriangleRenderer : public IUnknown
	{
	public:
	
		//! destructor
		virtual ~IK3DTriangleRenderer() {};

		//! sets a render target
		virtual void setRenderTarget(video::ISurface* surface, const core::rectEx<s32>& viewPort) = 0;

		//! en or disables the backface culling
		virtual void setBackfaceCulling(bool enabled = true) = 0;

		//! sets the Texture
		virtual void setTexture(video::ISurface* texture) = 0;

		//! draws an indexed triangle list
		virtual void drawIndexedTriangleList(S2DVertex* vertices, s32 vertexCount, const u16* indexList, s32 triangleCount) = 0;
	};


	IK3DTriangleRenderer* createTriangleRendererTextureGouraud(IZBuffer* zbuffer);
	IK3DTriangleRenderer* createTriangleRendererTextureGouraudWire(IZBuffer* zbuffer);
	IK3DTriangleRenderer* createTriangleRendererGouraud(IZBuffer* zbuffer);
	IK3DTriangleRenderer* createTriangleRendererGouraudWire(IZBuffer* zbuffer);
	IK3DTriangleRenderer* createTriangleRendererTextureFlat(IZBuffer* zbuffer);
	IK3DTriangleRenderer* createTriangleRendererTextureFlatWire(IZBuffer* zbuffer);
	IK3DTriangleRenderer* createTriangleRendererFlat(IZBuffer* zbuffer);
	IK3DTriangleRenderer* createTriangleRendererFlatWire(IZBuffer* zbuffer);


} // end namespace video
} // end namespace irr

#endif