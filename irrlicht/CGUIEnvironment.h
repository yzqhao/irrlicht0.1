// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_GUI_ENVIRNMENT_H_INCLUDED__
#define __C_GUI_ENVIRNMENT_H_INCLUDED__

#include "IGUIEnvironment.h"
#include "IGUIElement.h"
#include "array.h"
#include "IFileSystem.h"

namespace irr
{
namespace gui
{

class CGUIEnvironment : public IGUIEnvironment, public IGUIElement
{
public:

	//! constructor
	CGUIEnvironment(io::IFileSystem* fs, video::IVideoDriver* driver, irr::IEventReceiver* userReceiver);

	//! destructor
	virtual ~CGUIEnvironment();

	//! draws all gui elements
	virtual void drawAll();

	//! sets the key focus to an element
	virtual void setKeyFocus(IGUIElement* element);

	//! removes the key focus from an element
	virtual void removeKeyFocus(IGUIElement* element);

	//! returns the current video driver
	virtual video::IVideoDriver* getVideoDriver();

	//! posts an input event to the environment
	virtual void postEventFromUser(SEvent event);

	//! called if an event happened.
	virtual bool OnEvent(SEvent event);

	//! returns the current gui skin
	virtual IGUISkin* getSkin();

	//! returns the font
	virtual IGUIFont* getFont(const c8* filename);

	//! adds an button. The returned pointer must not be dropped.
	virtual IGUIButton* addButton(const core::rectEx<s32>& rectangle, IGUIElement* parent=0, s32 id=-1, const wchar_t* text=0);

	//! adds a window. The returned pointer must not be dropped.
	virtual IGUIWindow* addWindow(const core::rectEx<s32>& rectangle, IGUIElement* parent=0, s32 id=-1, const wchar_t* text=0);

	//! adds a scrollbar. The returned pointer must not be dropped.
	virtual IGUIScrollBar* addScrollBar(bool horizontal, const core::rectEx<s32>& rectangle, IGUIElement* parent=0, s32 id=-1);

	//! adds an image. The returned pointer must not be dropped.
	virtual IGUIImage* addImage(const core::rectEx<s32>& rectangle, IGUIElement* parent=0, s32 id=-1, const wchar_t* text=0);

	//! adds a checkbox
	virtual IGUICheckBox* addCheckBox(bool checked, const core::rectEx<s32>& rectangle, IGUIElement* parent=0, s32 id=-1, const wchar_t* text=0);

	//! adds a list box
	virtual IGUIListBox* addListBox(const core::rectEx<s32>& rectangle, IGUIElement* parent=0, s32 id=-1);

	//! adds an mesh viewer. The returned pointer must not be dropped.
	virtual IGUIMeshViewer* addMeshViewer(const core::rectEx<s32>& rectangle, IGUIElement* parent=0, s32 id=-1, const wchar_t* text=0);

	//! adds a file open dialog. The returned pointer must not be dropped.
	virtual IGUIFileOpenDialog* addFileOpenDialog(const wchar_t* title = 0, IGUIElement* parent=0, s32 id=-1);

	//! adds a static text. The returned pointer must not be dropped.
	virtual IGUIElement* addStaticText(const wchar_t* text, bool border, const core::rectEx<s32>& rectangle, IGUIElement* parent=0, s32 id=-1);

	//! sets the mouse focus to an element
	virtual void setMouseFocus(IGUIElement* element);

	//! removes the mouse focus from an element
	virtual void removeMouseFocus(IGUIElement* element);

	//! returns default font
	virtual IGUIFont* getBuildInFont();

private:

	struct SFont
	{
		core::stringc Filename;
		IGUIFont* Font;

		bool operator < (const SFont& other) const
		{
			return (Filename < other.Filename);
		}
	};

	void updateHoveredElement(core::position2d<s32> mousePos);

	void loadBuidInFont();

	core::array<SFont> Fonts;
	video::IVideoDriver* Driver;
	IEventReceiver* UserReceiver;
	IGUIElement* Hovered;
	IGUIElement* MouseFocus;
	IGUIElement* KeyFocus;
	IGUISkin* CurrentSkin;
	io::IFileSystem* FileSystem;
};

} // end namespace gui
} // end namespace irr

#endif