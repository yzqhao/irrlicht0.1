// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_IRR_DEVICE_WIN32_H_INCLUDED__
#define __C_IRR_DEVICE_WIN32_H_INCLUDED__

#ifdef WIN32

#include "IrrlichtDevice.h"
#include "ISurfacePresenter.h"
#include "IGUIEnvironment.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace irr
{

	class CIrrDeviceWin32 : public IrrlichtDevice, video::ISurfacePresenter
	{
	public:

		//! constructor
		CIrrDeviceWin32(video::EDriverType deviceType, 
			const core::dimension2d<s32>& windowSize, u32 bits,
			bool fullscreen, IEventReceiver* receiver=0);

		//! destructor
		virtual ~CIrrDeviceWin32();

		//! runs the device. Returns false if device wants to be deleted
		virtual bool run();

		//! returns the video driver
		virtual video::IVideoDriver* getVideoDriver();

		//! return file system
		virtual io::IFileSystem* getFileSystem();

		//! returns the gui environment
		virtual gui::IGUIEnvironment* getGUIEnvironment();

		//! returns the scene manager
		virtual scene::ISceneManager* getSceneManager();

		//! sets the caption of the window
		virtual void setWindowCaption(const wchar_t* text);

		//! returns if window is active. if not, nothing need to be drawn
		virtual bool isWindowActive();

		//! presents a surface in the client area
		virtual void present(video::ISurface* surface);

		//! notifies the device that it should close itself
		virtual void closeDevice();

	private:

		//! create the driver
		void createDriver(video::EDriverType driverType,
			const core::dimension2d<s32>& windowSize, u32 bits, bool fullscreen);

		//! switchs to fullscreen
		bool switchToFullScreen(s32 width, s32 height, s32 bits);

		HWND HWnd;

		io::IFileSystem* FileSystem;
		video::IVideoDriver* VideoDriver;
		gui::IGUIEnvironment* GUIEnvironment;
		scene::ISceneManager* SceneManager;

		bool ChangedToFullScreen;
	};


} // end namespace irr

#endif
#endif
