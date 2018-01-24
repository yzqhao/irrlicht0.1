// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __I_TEXTURE_H_INCLUDED__
#define __I_TEXTURE_H_INCLUDED__

#include "IUnknown.h"
#include "dimension2d.h"
#include "EDriverTypes.h"

namespace irr
{
namespace video  
{
	//! An enum for the color format of textures used by the Irrlicht Engine.
	/** A color format specifies how color information is stored. The Irrlicht Engine
	Mostly uses the format EHCF_R5G5B5.
	*/
	enum ECOLOR_FORMAT
	{
		//! 16 bit color format used by the software driver, and thus preferred
		//! by all other irrlicht engine video drivers. There are 5 bits for every
		//! color component, and a single bit is left for alpha information.
		EHCF_R5G5B5 = 0,	

		//! Standard 16 bit color format.
		EHCF_R5G6B5,

		//! Default 32 bit color format. 8 bits are used for every component:
		//! red, green, blue and alpha.
		EHCF_A8R8G8B8,
	};


//! Interface for a Video Driver dependent Texture.
/**
	An ITexture is created by an IVideoDriver by using IVideoDriver::addTexture or
	IVideoDriver::getTexture. After this, this texture may only be used by this VideoDriver.
	As you can imagine, textures of the DirectX and the OpenGL device will not be compatible,
	for example.
	An exception is the Software device and the NULL device, their textures are compatible.
	If you try to use a texture created by one device with an other device, the device
	will refuse to do that and write a warning or an error message to the output buffer.
*/
class ITexture : public IUnknown
{
public:

	//! destructor
	virtual ~ITexture() {};

	//! Lock function. Locks the Texture and returns a pointer to access the 
	//! pixels. After lock() has been called and all operations on the pixels
	//! are done, you must call unlock().
	//! \return Returns a pointer to the pixel data. The format of the pixel can
	//! be determinated by using getColorFormat(). NULL is returned, if
	//! the texture cannot be locked.
	virtual void* lock() = 0;

	//! Unlock function. Must be called after a lock() to the texture.
	virtual void unlock() = 0;

	//! Returns dimension of texture (=size)
	//! \return Returns the size of the texture.
	virtual const core::dimension2d<s32>& getDimension() = 0;

	//! Returns driver type of texture. This is the driver, which created the texture.
	//! This method is used internally by the video devices, to check, if they may
	//! use a texture because textures may be incompatible between different
	//! devices.
	//! \return Returns driver type of texture.
	virtual EDriverType getDriverType() = 0;

	//! Returns the color format of texture. This format is EHCF_R5G5B5 in most cases.
	//! \return Returns the color format of texture.
	virtual ECOLOR_FORMAT getColorFormat() = 0;

	//! Returns pitch of texture (in bytes). The pitch is the amount of bytes
	//! used for a row of pixels in a texture.
	//! \return Returns pitch of texture in bytes.
	virtual s32 getPitch() = 0;
};


} // end namespace video
} // end namespace irr

#endif