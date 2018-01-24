// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __E_DRIVER_TYPES_H_INCLUDED__
#define __E_DRIVER_TYPES_H_INCLUDED__

namespace irr
{
namespace video  
{
	//! An enum for all types of drivers the Irrlicht Engine supports.
	enum EDriverType
	{
		//! Null device, useful for applications to run the engine without visualisation. 
		//! The null device is able to load textures, but does not render and display
		//! any graphics.
		DT_NULL,     

		//! Software device, runs on all platforms, with every hardware. Is slow and does not support every renderstate correctly.
		//! It should only be used for 2d graphics, but it can also perform some
		//! primitive 3d functions.
		DT_SOFTWARE, 

		//! DirectX 8 device, only available on Win32 platforms including Win95, Win98, WinNT,
		//! Win2K, WinXP.
		DT_DIRECTX8, 

		//! OpenGL device not 100% implemented yet.
		DT_OPENGL    
	};

} // end namespace video
} // end namespace irr


#endif