// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __I_SURFACE_PRESENTER_H_INCLUDED__
#define __I_SURFACE_PRESENTER_H_INCLUDED__

#include "ISurface.h"

namespace irr
{
namespace video  
{

/*!
	Interface for a class which is able to present an ISurface 
	an the Screen. Usually only implemented by an KDevice for
	presenting Software Device Rendered images.

	This class should be used internal only.
*/

	class ISurfacePresenter
	{
	public:

		//! presents a surface in the client area
		virtual void present(video::ISurface* surface) = 0;
	};

} // end namespace video
} // end namespace irr

#endif