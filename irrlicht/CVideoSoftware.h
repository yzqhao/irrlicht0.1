// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_K_VIDEO_SOFTWARE_H_INCLUDED__
#define __C_K_VIDEO_SOFTWARE_H_INCLUDED__

#include "IK3DTriangleRenderer.h"
#include "CVideoNull.h"

namespace irr
{
namespace video
{
	class CVideoSoftware : public CVideoNull
	{
	public:

		//! constructor
		CVideoSoftware(const core::dimension2d<s32>& windowSize, bool fullscreen, io::IFileSystem* io, video::ISurfacePresenter* presenter);

		//! destructor
		virtual ~CVideoSoftware();

		//! presents the rendered scene on the screen, returns false if failed
		virtual bool endScene();

		//! queries the features of the driver, returns true if feature is available
		virtual bool queryFeature(EK3D_VIDEO_DRIVER_FEATURE feature);

		//! sets transformation
		virtual void setTransform(E_TRANSFORMATION_STATE state, const core::matrix4& mat);

		//! sets a material
		virtual void setMaterial(const SMaterial& material);

		//! sets a render target
		virtual void setRenderTarget(video::ITexture* texture);

		//! sets a viewport
		virtual void setViewPort(const core::rectEx<s32>& area);

		//! clears the zbuffer
		virtual bool beginScene(bool backBuffer, bool zBuffer, Color color);

		//! draws an indexed triangle list
		virtual void drawIndexedTriangleList(const S3DVertex* vertices, s32 vertexCount, const u16* indexList, s32 triangleCount);

		//! draws an indexed triangle list
		virtual void drawIndexedTriangleList(const S3DVertex2TCoords* vertices, s32 vertexCount, const u16* indexList, s32 triangleCount);

		//! draws an 2d image
		virtual void draw2DImage(video::ITexture* texture, const core::position2d<s32>& destPos);

		//! draws an 2d image, using a color (if color is other then Color(255,255,255,255)) and the alpha channel of the texture if wanted.
		virtual void draw2DImage(video::ITexture* texture, const core::position2d<s32>& destPos, const core::rectEx<s32>& sourceRect, const core::rectEx<s32>* clipRect = 0, Color color=Color(255,255,255,255), bool useAlphaChannelOfTexture=false);

		//! draw an 2d rectangle
		virtual void draw2DRectangle(Color color, const core::rectEx<s32>& pos, const core::rectEx<s32>* clip = 0);

	protected:

		//! sets a render target
		void setRenderTarget(video::ISurface* surface);

		//! sets the current Texture
		void setTexture(video::ITexture* texture);

		video::ISurface* BackBuffer;
		video::ISurfacePresenter* Presenter;

		//! switches to a triangle renderer
		void switchToTriangleRenderer(ETriangleRenderer renderer);

		//! void selects the right triangle renderer based on the render states.
		void selectRightTriangleRenderer();

		core::array<S2DVertex> TransformedPoints;

		video::ITexture* RenderTargetTexture;	
		video::ISurface* RenderTargetSurface;	
		core::position2d<s32> Render2DTranslation;
		core::dimension2d<s32> RenderTargetSize;
		core::dimension2d<s32> ViewPortSize;

		core::matrix4 TransformationMatrix[TS_COUNT];

		IK3DTriangleRenderer* CurrentTriangleRenderer;
		IK3DTriangleRenderer* TriangleRenderers[ETR_COUNT];
		ETriangleRenderer CurrentRenderer;

		IZBuffer* ZBuffer;

		video::ITexture* Texture;
		
		SMaterial Material;
	};

} // end namespace video
} // end namespace irr


#endif