// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __I_GUI_BUTTON_H_INCLUDED__
#define __I_GUI_BUTTON_H_INCLUDED__

#include "IGUIElement.h"

namespace irr
{
namespace gui
{
	class IGUIFont;

	//! GUI Button interface.
	class IGUIButton : public IGUIElement
	{
	public:

		//! constructor
		IGUIButton(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rectEx<s32> rectangle)
			: IGUIElement(environment, parent, id, rectangle) {}

		//! destructor
		~IGUIButton() {};

		//! Sets another skin independent font.
		//! If this is set to zero, the button uses the font of the skin.
		//! \param font: New font to set.
		virtual void setOverrideFont(IGUIFont* font=0) = 0;
	};


} // end namespace gui
} // end namespace irr

#endif