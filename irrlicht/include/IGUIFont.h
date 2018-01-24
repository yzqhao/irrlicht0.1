// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __I_GUI_FONT_H_INCLUDED__
#define __I_GUI_FONT_H_INCLUDED__

#include "IUnknown.h"
#include "rect.h"
#include "irrTypes.h"
#include "Color.h"

namespace irr
{
namespace gui
{

//! Font interface.
class IGUIFont : public IUnknown
{
public:

	//! destructor
	virtual ~IGUIFont() {};

	//! draws an text and clips it to the specified rectangle if wanted
	virtual void draw(const wchar_t* text, const core::rectEx<s32>& position, video::Color color, bool hcenter=false, bool vcenter=false, const core::rectEx<s32>* clip=0) = 0;

	//! returns the dimension of a text
	virtual core::dimension2d<s32> getDimension(const wchar_t* text) = 0;
};

} // end namespace gui
} // end namespace irr

#endif