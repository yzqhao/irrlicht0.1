// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_GUI_BUTTON_H_INCLUDED__
#define __C_GUI_BUTTON_H_INCLUDED__

#include "IGUIButton.h"

namespace irr
{
namespace gui
{

	class CGUIButton : public IGUIButton
	{
	public:

		//! constructor
		CGUIButton(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rectEx<s32> rectangle);

		//! destructor
		~CGUIButton();

		//! called if an event happened.
		virtual bool OnEvent(SEvent event);

		//! draws the element and its children
		virtual void draw();

		//! sets another skin independent font. if this is set to zero, the button uses the font of the skin.
		virtual void setOverrideFont(IGUIFont* font=0);

	private:

		bool Pressed;
		IGUIFont* OverrideFont;
	};

} // end namespace gui
} // end namespace irr

#endif