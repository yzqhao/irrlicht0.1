// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __I_GUI_ENVIRNMENT_H_INCLUDED__
#define __I_GUI_ENVIRNMENT_H_INCLUDED__

#include "rect.h"
#include "IUnknown.h"
#include "IEventReceiver.h"
#include "irrTypes.h"

namespace irr
{

	namespace video
	{
		class IVideoDriver;
	}

namespace gui
{

class IGUIElement;
class IGUISkin;
class IGUIButton;
class IGUIWindow;
class IGUIFont;
class IGUIScrollBar;
class IGUIImage;
class IGUIMeshViewer;
class IGUICheckBox;
class IGUIListBox;
class IGUIFileOpenDialog;

//! GUI Environment. Used as factory and manager of all other GUI elements.
class IGUIEnvironment : public IUnknown
{
public:

	//! destructor
	virtual ~IGUIEnvironment() {};

	//! Draws all gui elements.
	virtual void drawAll() = 0;

	//! Sets the key focus to an element.
	virtual void setKeyFocus(IGUIElement* element) = 0;

	//! Removes the key focus from an element.
	virtual void removeKeyFocus(IGUIElement* element) = 0;

	//! Sets the mouse focus to an element.
	virtual void setMouseFocus(IGUIElement* element) = 0;

	//! Removes the mouse focus from an element.
	virtual void removeMouseFocus(IGUIElement* element) = 0;

	//! Returns the current video driver.
	virtual video::IVideoDriver* getVideoDriver() = 0;

	//! Posts an input event to the environment.
	virtual void postEventFromUser(SEvent event) = 0;

	//! Returns pointer to the current gui skin.
	virtual IGUISkin* getSkin() = 0;

	//! Returns pointer to the font with the specified file name. Loads the font if 
	//! it was not loaded before. Returns 0 if the font could not be loaded.
	//! \return
	//! returns a pointer to the font.
	//! This pointer should not be dropped. See IUnknown::drop() for more information.
	virtual IGUIFont* getFont(const c8* filename) = 0;

	//! Returns the default build-in font.
	virtual IGUIFont* getBuildInFont() = 0;

	//! Adds an button element. 
	//! \return
	//! Returns a pointer to the created button. Returns 0 if an error occured.
	//! This pointer should not be dropped. See IUnknown::drop() for more information.
	virtual IGUIButton* addButton(const core::rectEx<s32>& rectangle, IGUIElement* parent=0, s32 id=-1, const wchar_t* text=0) = 0;

	//! Adds an empty window element. 
	//! \return
	//! Returns a pointer to the created window. Returns 0 if an error occured.
	//! This pointer should not be dropped. See IUnknown::drop() for more information.
	virtual IGUIWindow* addWindow(const core::rectEx<s32>& rectangle, IGUIElement* parent=0, s32 id=-1, const wchar_t* text=0) = 0;

	//! Adds a scrollbar. 
	//! \return
	//! Returns a pointer to the created scrollbar. Returns 0 if an error occured.
	//! This pointer should not be dropped. See IUnknown::drop() for more information.
	virtual IGUIScrollBar* addScrollBar(bool horizontal, const core::rectEx<s32>& rectangle, IGUIElement* parent=0, s32 id=-1) = 0;

	//! Adds an image element. 
	//! \return
	//! Returns a pointer to the created image element. Returns 0 if an error occured.
	//! This pointer should not be dropped. See IUnknown::drop() for more information.
	virtual IGUIImage* addImage(const core::rectEx<s32>& rectangle, IGUIElement* parent=0, s32 id=-1, const wchar_t* text=0) = 0;

	//! Adds a checkbox element.
	//! \return
	//! Returns a pointer to the created check box. Returns 0 if an error occured.
	//! This pointer should not be dropped. See IUnknown::drop() for more information.
	virtual IGUICheckBox* addCheckBox(bool checked, const core::rectEx<s32>& rectangle, IGUIElement* parent=0, s32 id=-1, const wchar_t* text=0) = 0;

	//! Adds a list box element.
	//! \return
	//! Returns a pointer to the created list box. Returns 0 if an error occured.
	//! This pointer should not be dropped. See IUnknown::drop() for more information.
	virtual IGUIListBox* addListBox(const core::rectEx<s32>& rectangle, IGUIElement* parent=0, s32 id=-1) = 0;

	//! Adds an mesh viewer. Not 100% implemented yet.
	//! \return
	//! Returns a pointer to the created mesh viewer. Returns 0 if an error occured.
	//! This pointer should not be dropped. See IUnknown::drop() for more information.
	virtual IGUIMeshViewer* addMeshViewer(const core::rectEx<s32>& rectangle, IGUIElement* parent=0, s32 id=-1, const wchar_t* text=0) = 0;

	//! Adds a file open dialog.
	//! \return
	//! Returns a pointer to the created file open dialog. Returns 0 if an error occured.
	//! This pointer should not be dropped. See IUnknown::drop() for more information.
	virtual IGUIFileOpenDialog* addFileOpenDialog(const wchar_t* title = 0, IGUIElement* parent=0, s32 id=-1) = 0;

	//! Adds a static text. The returned pointer must not be dropped.
	//! \param text is the text to be displayed. Can be altered after creation with SetText().
	//! \param border has to be set to true if the static text should have a 3d border.
	//! \param rectangle is the position of the static text.
	//! \param parent is the parent item of the element. E.g. a window. Set it to 0 to place the static text directly in the environment.
	//! \param id is a s32 to identify the text.
	//! \return
	//! Returns a pointer to the created static text. Returns 0 if an error occured.
	//! This pointer should not be dropped. See IUnknown::drop() for more information.
	virtual IGUIElement* addStaticText(const wchar_t* text, bool border, const core::rectEx<s32>& rectangle, IGUIElement* parent=0, s32 id=-1) = 0;

};


} // end namespace gui
} // end namespace irr

#endif