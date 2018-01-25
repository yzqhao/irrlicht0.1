/* Irrlicht.h -- interface of the 'Irrlicht Engine' version 0.1

  Copyright (C) 2002-2003 Nikolaus Gebhardt

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef __IRRLICHT_H_INCLUDED__
#define __IRRLICHT_H_INCLUDED__

#include "aabbox3d.h"
#include "array.h"
#include "Color.h"
#include "SLight.h"
#include "dimension2d.h"
#include "EDriverTypes.h"
#include "heapsort.h"
#include "IAnimatedMesh.h"
#include "IAnimatedMeshSceneNode.h"
#include "IBspTreeSceneNode.h"
#include "ICameraSceneNode.h"
#include "IEventReceiver.h"
#include "IFileList.h"
#include "IFileSystem.h"
#include "IGUIButton.h"
#include "IGUICheckbox.h"
#include "IGUIElement.h"
#include "IGUIEnvironment.h"
#include "IGUIFileOpenDialog.h"
#include "IGUIFont.h"
#include "IGUIImage.h"
#include "IGUIListBox.h"
#include "IGUIMeshViewer.h"
#include "IGUIScrollBar.h"
#include "IGUISkin.h"
#include "IGUIWindow.h"
#include "IMesh.h"
#include "IMeshBuffer.h"
#include "IQ3LevelMesh.h"
#include "IReadFile.H"
#include "IrrlichtDevice.h"
#include "irrmath.h"
#include "irrstring.h"
#include "ISceneManager.h"
#include "ISceneNode.h"
#include "ILightSceneNode.h"
#include "ISceneNodeAnimator.h"
#include "IBillboardSceneNode.h"
#include "ITexture.h"
#include "IUnknown.h"
#include "IVideoDriver.h"
#include "Keycodes.h"
#include "line3d.h"
#include "list.h"
#include "matrix4.h"
#include "plane3d.h"
#include "plane3dex.h"
#include "vector2d.h"
#include "vector3d.h"
#include "position2d.h"
#include "rect.h"
#include "S3DVertex.h"
#include "SMaterial.h"
#include "SMesh.h"
#include "SMeshBuffer.h"
#include "SMeshBufferLightMap.h"
#include "irrTypes.h"

#ifdef IRRLICHT_EXPORTS
#define IRRLICHT_API __declspec(dllexport)
#else
#define IRRLICHT_API __declspec(dllimport)
#endif

//! Everything in the Irrlicht Engine can be found in this namespace.
namespace irr
{
	//! Creates an Irrlicht device. The Irrlicht device is the root object for using the engine.
	/** \param deviceType: Type of the device. This can currently be video::DT_NULL, 
	video::DT_SOFTWARE,	video::DT_DIRECTX8 and video::DT_OPENGL.
	\param windowSize: Size of the window or the video mode in fullscreen mode.
	\param bits: Bits per pixel in fullscreen mode. Ignored if windowed mode.
	\param fullscreen: Should be set to true if the device should run in fullscreen. Otherwise
		the device runs in window mode.
	\param receiver: A user created event receiver.
	\return Returns pointer to the created IrrlichtDevice or null if the 
	device could not be created.
	*/
	IRRLICHT_API IrrlichtDevice* createDevice(video::EDriverType deviceType, 
		const core::dimension2d<s32>& windowSize, u32 bits, bool fullscreen, IEventReceiver* receiver = 0);

	// THE FOLLOWING IS AN EMPTY LIST OF ALL SUB NAMESPACES
	// EXISTING ONLY FOR THE DOCUMENTION SOFTWARE DOXYGEN.

	//! In this namespace can be found basic classes like vectors, planes, arrays, lists and so on.
	namespace core
	{
	}

	//! The gui namespace contains useful classes for easy creation of a graphical user interface.
	namespace gui
	{
	}

	//! This namespace provides interfaces for input/output: Reading and writing files, accessing zip archives, and so on.
	namespace io
	{
	}

	//! All scene management can be found in this namespace: Mesh loading, special scene nodes like octrees and billboards, and so on.
	namespace scene
	{
	}

	//! The video namespace contains classes for accessing the video driver. All 2d and 3d rendering is done here.
	namespace video
	{
	}
}

#endif