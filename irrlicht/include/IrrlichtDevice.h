// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __I_K_DEVICE_H_INCLUDED__
#define __I_K_DEVICE_H_INCLUDED__

#include "irrTypes.h"
#include "IUnknown.h"
#include "dimension2d.h"
#include "IFileSystem.h"
#include "IVideoDriver.h"
#include "EDriverTypes.h"
#include "IGUIEnvironment.h"
#include "IEventReceiver.h"
#include "ISceneManager.h"

namespace irr
{
	//! The Irrlicht device. You can create it with createDevice(). 
	class IrrlichtDevice : public IUnknown
	{
	public:

		//! destructor
		virtual ~IrrlichtDevice() {};

		//! Runs the device. Returns false if device wants to be deleted. Use it in this way:
		//! while(device->run()) { drawEveryThing(); };
		virtual bool run() = 0;

		//! \return Returns a pointer the video driver.
		virtual video::IVideoDriver* getVideoDriver() = 0;

		//! \return Returns a pointer to the file system.
		virtual io::IFileSystem* getFileSystem() = 0;

		//! \return Returns a pointer to the gui environment.
		virtual gui::IGUIEnvironment* getGUIEnvironment() = 0;

		//! \return Returns a pointer to the scene manager.
		virtual scene::ISceneManager* getSceneManager() = 0;

		//! Sets the caption of the window.
		//! \param text: New text of the window caption.
		virtual void setWindowCaption(const wchar_t* text) = 0;

		//! \return Returns true if window is active. If the window is inactive,
		//! nothing need to be drawn.
		virtual bool isWindowActive() = 0;

		//! Notifies the device that it should close itself.
		//! IrrlichtDevice::run() will always return false after closeDevice() was called.
		virtual void closeDevice() = 0;
	};

} // end namespace

#endif