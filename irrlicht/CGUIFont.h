// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_GUI_FONT_H_INCLUDED__
#define __C_GUI_FONT_H_INCLUDED__

#include "IGUIFont.h"
#include "irrstring.h"
#include "IVideoDriver.h"
#include "array.h"

namespace irr
{
namespace gui
{

class CGUIFont : public IGUIFont
{
public:

	//! constructor
	CGUIFont(video::IVideoDriver* Driver);

	//! destructor
	virtual ~CGUIFont();

	//! loads a font file
	bool load(const c8* filename);

	//! loads a font file
	bool load(io::IReadFile* file);

	//! draws an text and clips it to the specified rectangle if wanted
	virtual void draw(const wchar_t* text, const core::rectEx<s32>& position, video::Color color, bool hcenter=false, bool vcenter=false, const core::rectEx<s32>* clip=0);

	//! returns the dimension of a text
	virtual core::dimension2d<s32> getDimension(const wchar_t* text);

private:

	//! load & prepare font from ITexture
	bool loadTexture(video::ITexture* texture);

	video::IVideoDriver* Driver;
	core::array< core::rectEx<s32> > Positions;
	video::ITexture* Texture;
	s32 WrongCharacter;
};

} // end namespace gui
} // end namespace irr

#endif