// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_VIDEO_DIRECTX_8_H_INCLUDED__
#define __C_VIDEO_DIRECTX_8_H_INCLUDED__

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include "CVideoNull.h"
#include <windows.h>
#include <d3d9.h>

namespace irr
{
namespace video
{
	class CVideoDirectX8 : public CVideoNull
	{
	public:
		//! constructor
		CVideoDirectX8(const core::dimension2d<s32>& screenSize, HWND window, bool fullscreen, io::IFileSystem* io, bool pureSoftware=false);

		//! destructor
		virtual ~CVideoDirectX8();

		//! applications must call this method before performing any rendering. returns false if failed.
		virtual bool beginScene(bool backBuffer, bool zBuffer, Color color);

		//! applications must call this method after performing any rendering. returns false if failed.
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

		//! initialises the Direct3D API
		bool initDriver(const core::dimension2d<s32>& screenSize, HWND hwnd, 
						u32 bits, bool fullScreen, bool pureSoftware);

	private:

		// enumeration for rendering modes such as 2d and 3d for minizing the switching of renderStates.
		enum E_RENDER_MODE
		{
			ERM_NONE = 0,	// no render state has been set yet.
			ERM_2D,			// 2d drawing rendermode
			ERM_3D			// 3d rendering mode
		};

		//! sets right vertex shader
		void setVertexShader(video::E_VERTEX_TYPE newType);

		//! sets the needed renderstates
		void setRenderStates3DMode();

		//! sets the needed renderstates
		void setRenderStates2DMode(bool alpha, bool texture, bool alphaChannel);

		//! sets the current Texture
		void setTexture(s32 stage, video::ITexture* texture);

		//! returns a device dependent texture from a software surface (ISurface)
		//! THIS METHOD HAS TO BE OVERRIDDEN BY DERIVED DRIVERS WITH OWN TEXTURES
		virtual video::ITexture* createDeviceDependentTexture(ISurface* surface, bool generateMipLevels);

		//! deletes all dynamic lights there are
		virtual void deleteAllDynamicLights();

		//! adds a dynamic light
		virtual void addDynamicLight(const SLight& light);

		//! returns the maximal amount of dynamic lights the device can handle
		virtual s32 getMaximalDynamicLightAmount();

		inline D3DCOLORVALUE colorToD3D(const Color& col)
		{
			const f32 f = 1 / 255.0f;
			D3DCOLORVALUE v;
			v.r = col.getRed() * f;
			v.g = col.getGreen() * f;
			v.b = col.getBlue() * f;
			v.a = col.getAlpha() * f;
			return v;
		}


		E_RENDER_MODE CurrentRenderMode;
		

		core::matrix4 Matrices[TS_COUNT]; // matrizes of the 3d mode we need to restore when we switch back from the 2d mode.

		SMaterial Material, LastMaterial;
		bool ResetRenderStates; // bool to make all renderstates be reseted if set.
		ITexture* CurrentTexture[2];

		HINSTANCE D3DLibrary;
		IDirect3D9* pID3D;
		IDirect3DDevice9* pID3DDevice;

		D3DCAPS9 Caps;

		E_VERTEX_TYPE LastVertexType;

		s32 LastSetLight;
#endif
	};

} // end namespace video
} // end namespace irr


#endif