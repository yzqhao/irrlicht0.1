// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_GUI_STATIC_TEXT_H_INCLUDED__
#define __C_GUI_STATIC_TEXT_H_INCLUDED__

#include "IGUIElement.h"

namespace irr
{
namespace gui
{
	class CGUIStaticText : public IGUIElement
	{
	public:

		//! constructor
		CGUIStaticText(const wchar_t* text, bool border, IGUIEnvironment* environment, IGUIElement* parent, s32 id, const core::rectEx<s32>& rectangle);

		//! destructor
		~CGUIStaticText();

		//! draws the element and its children
		virtual void draw();

	private:

		bool Border;
	};

} // end namespace gui
} // end namespace irr

#endif