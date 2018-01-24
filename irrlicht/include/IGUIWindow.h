// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __I_GUI_WINDOW_H_INCLUDED__
#define __I_GUI_WINDOW_H_INCLUDED__

#include "IGUIElement.h"

namespace irr
{
namespace gui
{
	//! Default moveable window GUI element with border, caption and close icons.
	class IGUIWindow : public IGUIElement
	{
	public:

		//! constructor
		IGUIWindow(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rectEx<s32> rectangle)
			: IGUIElement(environment, parent, id, rectangle) {}

		//! destructor
		virtual ~IGUIWindow() {};
	};


} // end namespace gui
} // end namespace irr

#endif