// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_VIDEO_NULL_H_INCLUDED__
#define __C_VIDEO_NULL_H_INCLUDED__

#include "IVideoDriver.h"
#include "IFileSystem.h"
#include "ISurfacePresenter.h"
#include "array.h"
#include "irrstring.h"
#include "ISurfaceLoader.h"
#include "CFPSCounter.h"
#include "S3DVertex.h"

namespace irr
{
namespace video
{
	class CVideoNull : public IVideoDriver
	{
	public:

		//! constructor
		CVideoNull(io::IFileSystem* io, const core::dimension2d<s32>& screenSize);

		//! destructor
		virtual ~CVideoNull();

		virtual bool beginScene(bool backBuffer, bool zBuffer, Color color);

		virtual bool endScene();

		//! queries the features of the driver, returns true if feature is available
		virtual bool queryFeature(EK3D_VIDEO_DRIVER_FEATURE feature);

		//! sets transformation
		virtual void setTransform(E_TRANSFORMATION_STATE state, const core::matrix4& mat);

		//! sets a material
		virtual void setMaterial(const SMaterial& material);

		//! loads a Texture
		virtual ITexture* getTexture(const c8* filename, bool generateMipLevels);

		//! loads a Texture
		virtual ITexture* getTexture(io::IReadFile* file, bool generateMipLevels);

		//! creates a Texture
		virtual ITexture* addTexture(const core::dimension2d<s32>& size, const c8* name);

		//! sets a render target
		virtual void setRenderTarget(video::ITexture* texture);

		//! sets a viewport
		virtual void setViewPort(const core::rectEx<s32>& area);

		//! gets the area of the current viewport
		virtual const core::rectEx<s32>& getViewPort() const;

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

		//! returns screen size
		virtual core::dimension2d<s32> getScreenSize();

		// returns current frames per second value
		virtual s32 getFPS();

		//! returns amount of primitives (mostly triangles) were drawn in the last frame.
		//! very useful method for statistics.
		virtual u32 getPrimitiveCountDrawed();

		//! deletes all dynamic lights there are
		virtual void deleteAllDynamicLights();

		//! adds a dynamic light
		virtual void addDynamicLight(const SLight& light);

		//! returns the maximal amount of dynamic lights the device can handle
		virtual s32 getMaximalDynamicLightAmount();

	protected:

		//! deletes all textures
		void deleteAllTextures();

		//! looks if the image is already loaded
		video::ITexture* findTexture(const c8* filename);

		//! opens the file and loads it into the surface
		video::ITexture* loadTextureFromFile(io::IReadFile* file, bool generateMipLevels);

		//! adds a surface, not loaded or created by the Irrlicht Engine
		void addTexture(video::ITexture* surface, const c8* filename);

		//! returns a device dependent texture from a software surface (ISurface)
		//! THIS METHOD HAS TO BE OVERRIDDEN BY DERIVED DRIVERS WITH OWN TEXTURES
		virtual video::ITexture* createDeviceDependentTexture(ISurface* surface, bool generateMipLevels);

		struct SSurface
		{
			core::stringc Filename;
			video::ITexture* Surface;

			bool operator < (const SSurface& other) const
			{
				return (Filename < other.Filename);
			}
		};

		core::array<SSurface> Textures;
		core::array<video::ISurfaceLoader*> SurfaceLoader;

		io::IFileSystem* FileSystem;

		core::rectEx<s32> ViewPort;
		core::dimension2d<s32> ScreenSize;
	
		CFPSCounter FPSCounter;

		u32 PrimitivesDrawn;
	};

} // end namespace video
} // end namespace irr


#endif
