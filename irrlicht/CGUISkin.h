// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_GUI_SKIN_H_INCLUDED__
#define __C_GUI_SKIN_H_INCLUDED__

#include "IGUISkin.h"

namespace irr
{
namespace gui
{

	class CGUISkin : public IGUISkin
	{
	public:

		CGUISkin(EGUI_SKIN_TYPE type);

		//! destructor
		~CGUISkin();

		//! returns default color
		virtual video::Color getColor(EGUI_DEFAULT_COLOR color);

		//! sets a default color
		virtual void setColor(EGUI_DEFAULT_COLOR which, video::Color newColor);

		//! returns default color
		virtual s32 getSize(EGUI_DEFAULT_SIZE size);

		//! sets a default size
		virtual void setSize(s32 size);

		//! returns the default font
		virtual IGUIFont* getFont();

		//! sets a default font
		virtual void setFont(IGUIFont* font);

	private:

		video::Color Colors[EGDC_COUNT];
		s32 Sizes[EGDS_COUNT];
		IGUIFont* Font;
	};




} // end namespace gui
} // end namespace irr

#endif