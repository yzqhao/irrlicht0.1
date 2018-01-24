// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __IRR_I_VIDEO_DRIVER_H_INCLUDED__
#define __IRR_I_VIDEO_DRIVER_H_INCLUDED__

#include "rect.h"
#include "Color.h"
#include "ITexture.h"
#include "matrix4.h"
#include "dimension2d.h"
#include "position2d.h"
#include "IReadFile.h"
#include "SMaterial.h"
#include "SLight.h"

namespace irr
{

namespace video
{
	struct S3DVertex;
	struct S3DVertex2TCoords;

	enum EK3D_VIDEO_DRIVER_FEATURE
	{
		//! Is driver able to render to a surface?
		EK3DVDF_RENDER_TO_TARGET = 0,	
		//! Is driver able to render with a bilinear filter applied?
		EK3DVDF_BILINEAR_FILER,			
		//! Is hardeware transform and lighting supported?
		EK3DVDF_HARDWARE_TL,			
		//! Can the driver handle mip maps?
		EK3DVDF_MIP_MAP,				
	};

	enum E_TRANSFORMATION_STATE
	{
		//! View transformation
		TS_VIEW = 0,
		//! World transformation
		TS_WORLD,
		//! Projection transformation
		TS_PROJECTION,
		//! Not used
		TS_COUNT
	};

	//! Interface to driver which is able to perform 2d and 3d gfx functions.
	/** The IVideoDriver interface is one of the most important interfaces of
	the Irrlicht Engine: All rendering and texture manipulating is done with
	this interface. You are able to use the Irrlicht Engine by only invoking methods
	of this interface if you would like to, although the irr::scene::ISceneManager interface
	provides a lot of powerful classes and methods to make the programmers life
	easier.
	*/
	class IVideoDriver : public IUnknown
	{
	public:

		//! destructor
		virtual ~IVideoDriver() {}

		//! Applications must call this method before performing any rendering. 
		//! \param backBuffer: Specifies if the back buffer should be cleared, which
		//! means that the screen is filled with a color specified with the parameter color.
		//! If this parameter is false, the back buffer will not be cleared and the color
		//! parameter is ignored.
		//! \param zBuffer: Speciefies if the depth or z buffer should be cleared. It is
		//! not nesesarry to do so, if only 2d drawing is used.
		//! \return Returns false if failed. Begin Scene can clear the back- and the z-buffer.
		virtual bool beginScene(bool backBuffer, bool zBuffer, Color color) = 0;

		//! Presents the rendered image on the screen.
		//! Applications must call this method after performing any rendering. 
		//! \return Returns false if failed and true if succeeded.
		virtual bool endScene() = 0;

		//! Queries the features of the driver, returns true if a feature is available
		//! \param feature: A feature to query.
		//! \return Returns true if the feature is available, false if not.
		virtual bool queryFeature(EK3D_VIDEO_DRIVER_FEATURE feature) = 0;

		//! Sets the view, world or projection transformation. 
		//! \param state: Transformation type to be set. Can be view, world or projection.
		//! \param mat: Matrix describing the transformation.
		virtual void setTransform(E_TRANSFORMATION_STATE state, const core::matrix4& mat) = 0;

		//! Sets a material. All 3d drawing functions draw geometry now
		//! using this material.
		//! \param material: Material to be used from now on.
		virtual void setMaterial(const SMaterial& material) = 0;

		//! Returns a pointer to a texture. Loads the texture if it is not
		//! already loaded, and generates mipmap levels if wished.
		//! The texture can be in BMP, JPG, TGA and PSD format.
		//! For loading BMP, TGA and PSD files, the engine uses its own methods.
		//! For loading JPG-Files the JPEG LIB 6b, written by 
		//! The Independent JPEG Group is used by the engine. Thanx for such a great
		//! library!
		//! \param filename: Filename of the texture to be loaded.
		//! \param generateMipLevels: Automaticly generates mip map levels if set to true.
		//! \return Returns a pointer to the texture and NULL if the texture
		//! could not be loaded. 
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual ITexture* getTexture(const c8* filename, bool generateMipLevels=false) = 0;

		//! Returns a pointer to a texture. Loads the texture if it is not
		//! already loaded, and generates mipmap levels if wished.
		//! The texture can be in BMP, JPG, TGA and PSD format.
		//! For loading BMP, TGA and PSD files, the engine uses its own methods.
		//! For loading JPG-Files the JPEG LIB 6b, written by 
		//! The Independent JPEG Group is used by the engine. Thanx for such a great
		//! library!
		//! \param file: Pointer to an already opened file.
		//! \param generateMipLevels: Automaticly generates mip map levels if set to true.
		//! \return Returns a pointer to the texture and NULL if the texture
		//! could not be loaded. 
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual ITexture* getTexture(io::IReadFile* file, bool generateMipLevels=false) = 0;

		//! Creates a Texture.
		//! \param size: Size of the texture.
		//! \param name: A name for the texture. Later calls of getTexture() with this name
		//! will return this texture
		//! \param generateMipLevels: If true, mipmap levels will be generated by the driver.
		//! \return Returns a pointer to the new created Texture. 
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		//! The format of the new texture will be chosen by the driver, and will in most 
		//! cases have the EHCF_R5G5B5 format.
		virtual ITexture* addTexture(const core::dimension2d<s32>& size, const c8* name) = 0;

		//! Sets a new render target. This will only work, if the driver
		//! supports the EK3DVDF_RENDER_TO_TARGET feature, which can be 
		//! queried with queryFeature().
		//! \param texture: New render target.
		virtual void setRenderTarget(video::ITexture* texture) = 0;

		//! Sets a new viewport. Every rendering operation is done into this
		//! new area.
		//! \param Rectangle defining the new area of rendering operations.
		virtual void setViewPort(const core::rectEx<s32>& area) = 0;

		//! Gets the area of the current viewport. 
		//! \return Returns rectangle of the current vieport.
		virtual const core::rectEx<s32>& getViewPort() const = 0;

		//! Draws an indexed triangle list.
		//! Note that there may be at maximum 65536 vertices, because the
		//! index list is a array of 16 bit values each with a maximum value 
		//! of 65536. If there are more than 65536 vertices in the list, 
		//! results of this operation are not defined.
		//! \param vertices: Pointer to array of vertices.
		//! \param vertexCount: Amount of vertices in the array.
		//! \param indexList: Pointer to array of indizes.
		//! \param triangleCount: amount of Triangles. Usually amount of indizes / 3.
		virtual void drawIndexedTriangleList(const S3DVertex* vertices, s32 vertexCount, const u16* indexList, s32 triangleCount) = 0;

		//! Draws an indexed triangle list.
		//! Note that there may be at maximum 65536 vertices, because the
		//! index list is a array of 16 bit values each with a maximum value 
		//! of 65536. If there are more than 65536 vertices in the list, 
		//! results of this operation are not defined.
		//! \param vertices: Pointer to array of vertices.
		//! \param vertexCount: Amount of vertices in the array.
		//! \param indexList: Pointer to array of indizes.
		//! \param triangleCount: amount of Triangles. Usually amount of indizes / 3.
		virtual void drawIndexedTriangleList(const S3DVertex2TCoords* vertices, s32 vertexCount, const u16* indexList, s32 triangleCount) = 0;

		//! Simply draws an 2d image, without any special effects
		//! \param texture: Pointer to texture to use.
		//! \param destPos: upper left 2d destination position where the image will be drawn.
		virtual void draw2DImage(video::ITexture* texture, const core::position2d<s32>& destPos) = 0;

		//! Draws an 2d image, using a color (if color is other then 
		//! Color(255,255,255,255)) and the alpha channel of the texture if wanted.
		//! \param texture: Texture to be drawn.
		//! \param destPos: Upper left 2d destination position where the image will be drawn.
		//! \param sourceRect: Source rectangle in the image.
		//! \param clipRect: Pointer to rectangle on the screen where the image is clipped to.
		//! This pointer can be NULL. Then the image is not clipped.
		//! \param color: Color with wich the image is colored. If the color equals 
		//! Color(255,255,255,255), the color is ignored. Note that the alpha component
		//! is used: If alpha is other than 255, the image will be transparent.
		//! \param useAlphaChannelOfTexture: If true, the alpha channel of the texture is 
		//! used to draw the image.
		virtual void draw2DImage(video::ITexture* texture, const core::position2d<s32>& destPos,
			const core::rectEx<s32>& sourceRect, const core::rectEx<s32>* clipRect = 0,
			Color color=Color(255,255,255,255), bool useAlphaChannelOfTexture=false) = 0;

		//! Draws an 2d rectangle.
		//! \param color: Color of the rectangle to draw. The alpha component will not
		//! be ignored and specifies how transparent the rectangle will be.
		//! \param pos: Position of the rectangle.
		//! \param clip: Pointer to rectangle against which the rectangle will be clipped.
		//! If the pointer is null, no clipping will be performed.
		virtual void draw2DRectangle(Color color, const core::rectEx<s32>& pos,
			const core::rectEx<s32>* clip = 0) = 0;

		//! Returns the size of the screen or render window.
		//! \return Size of screen or render window.
		virtual core::dimension2d<s32> getScreenSize() = 0;

		//! Returns current frames per second value.
		//! \return Returns amount of frames per second drawed.
		virtual s32 getFPS() = 0;

		//! Returns amount of primitives (mostly triangles) which were drawn in the last frame.
		//! Together with getFPS() very useful method for statistics.
		//! \return Amount of primitives drawn in the last frame.
		virtual u32 getPrimitiveCountDrawed() = 0;

		//! Deletes all dynamic lights which were previously added with addDynamicLight().
		virtual void deleteAllDynamicLights() = 0;

		//! Adds a dynamic light.
		//! \param light: Data specifing the dynamic light.
		virtual void addDynamicLight(const SLight& light) = 0;

		//! Returns the maximal amount of dynamic lights the device can handle
		//! \return Maximal amount of dynamic lights.
		virtual s32 getMaximalDynamicLightAmount() = 0;
	};

} // end namespace video
} // end namespace irr


#endif