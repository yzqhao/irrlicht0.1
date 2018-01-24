#include "CGUIEnvironment.h"
#include "IVideoDriver.h"
#include "IGUISkin.h"

#include "CGUIButton.h"
#include "CGUIWindow.h"
#include "CGUIScrollBar.h"
#include "CGUIFont.h"
#include "CGUIImage.h"
#include "CGUIMeshViewer.h"
#include "CGUICheckBox.h"
#include "CGUIListBox.h"
#include "CGUIFileOpenDialog.h"
#include "CGUIStaticText.h"
#include "BuildInFont.h"
#include "os.h"

namespace irr
{
namespace gui
{

//! constructor
CGUIEnvironment::CGUIEnvironment(io::IFileSystem* fs, video::IVideoDriver* driver, IEventReceiver* userReceiver)
: IGUIElement(0, 0, 0, core::rectEx<s32>(core::position2d<s32>(0,0), driver ? driver->getScreenSize() : core::dimension2d<s32>(0,0))),
	UserReceiver(userReceiver), Hovered(0), CurrentSkin(0), Driver(driver),
	MouseFocus(0), KeyFocus(0), FileSystem(fs)
{
	if (Driver)
		Driver->grab();

	if (FileSystem)
		FileSystem->grab();

	#ifdef _DEBUG
	IGUIEnvironment::setDebugName("CGUIEnvironment IGUIEnvironment");
	IGUIElement::setDebugName("CGUIEnvironment IGUIElement");
	#endif

	loadBuidInFont();

	CurrentSkin = createSkin(EGST_WINDOWS_STANDARD);
	CurrentSkin->setFont(getBuildInFont());
}


//! destructor
CGUIEnvironment::~CGUIEnvironment()
{
	if (Hovered)
		Hovered->drop();

	if (CurrentSkin)
		CurrentSkin->drop();

	if (Driver)
		Driver->drop();

	if (MouseFocus)
		MouseFocus->drop();

	if (KeyFocus)
		KeyFocus->drop();

	if (FileSystem)
		FileSystem->drop();

	// delete all fonts

	for (u32 i=0; i<Fonts.size(); ++i)
		Fonts[i].Font->drop();
}



void CGUIEnvironment::loadBuidInFont()
{
	const c8* filename = "#DefaultFont";

	io::IReadFile* file = io::createMemoryReadFile(BuildInFontData, BuildInFontDataSize, filename, false);

	CGUIFont* font = new CGUIFont(Driver);
	if (!font->load(file))
	{
		#ifdef _DEBUG
		os::Debuginfo::print("Error: Could not load built-in Font.");
		#endif
		font->drop();
		file->drop();
		return;
	}

	SFont f;
	f.Filename = filename;
	f.Font = font;
	Fonts.push_back(f);

	file->drop();
}



//! draws all gui elements
void CGUIEnvironment::drawAll()
{
	draw();
}


//! sets the key focus to an element
void CGUIEnvironment::setKeyFocus(IGUIElement* element)
{
	removeKeyFocus(KeyFocus);

	KeyFocus = element;
	if (KeyFocus)
		KeyFocus->grab();
}


//! removes the key focus from an element
void CGUIEnvironment::removeKeyFocus(IGUIElement* element)
{
	if (KeyFocus && KeyFocus==element)
	{
		SEvent e;
		e.EventType = EET_GUI_EVENT;
		e.GUIEvent.Caller = this;
		e.GUIEvent.EventType = EGET_ELEMENT_KEY_FOCUS_LOST;
		KeyFocus->OnEvent(e);
		KeyFocus->drop();
		KeyFocus = 0;
	}
}



//! sets the mouse focus to an element
void CGUIEnvironment::setMouseFocus(IGUIElement* element)
{
	removeMouseFocus(MouseFocus);

	MouseFocus = element;
	if (MouseFocus)
		MouseFocus->grab();

}



//! removes the mouse focus from an element
void CGUIEnvironment::removeMouseFocus(IGUIElement* element)
{
	if (MouseFocus && MouseFocus==element)
	{
		SEvent e;
		e.EventType = EET_GUI_EVENT;
		e.GUIEvent.Caller = this;
		e.GUIEvent.EventType = EGET_ELEMENT_MOUSE_FOCUS_LOST;
		MouseFocus->OnEvent(e);
		MouseFocus->drop();
		MouseFocus = 0;
	}
}



//! returns the current video driver
video::IVideoDriver* CGUIEnvironment::getVideoDriver()
{
	return Driver;
}



//! called by ui if an event happened.
bool CGUIEnvironment::OnEvent(SEvent event)
{
	if (UserReceiver && event.GUIEvent.Caller != this)
		return UserReceiver->OnEvent(event);

	return false;
}


void CGUIEnvironment::updateHoveredElement(core::position2d<s32> mousePos)
{
	IGUIElement* lastHovered = Hovered;
	if (Hovered)
		Hovered->drop();

	Hovered = getElementFromPoint(mousePos);

	if (Hovered)
	{
		Hovered->grab();
		if (Hovered != lastHovered)
		{
			SEvent event; 
			event.EventType = EET_GUI_EVENT;
			event.GUIEvent.Caller = Hovered;
			event.GUIEvent.EventType = EGET_ELEMENT_HOVERED;
			Hovered->OnEvent(event);
		}
	}	
}


//! posts an input event to the environment
void CGUIEnvironment::postEventFromUser(SEvent event)
{
	switch(event.EventType)
	{
	case EET_GUI_EVENT:
		// hey, why is the user sending gui events..?
		break;
	case EET_MOUSE_INPUT_EVENT:
		if (MouseFocus)
			MouseFocus->OnEvent(event);
		else
		{
			bool absorbed = false;

			updateHoveredElement(core::position2d<s32>(event.MouseInput.X, event.MouseInput.Y));

			if (Hovered && Hovered != this)
				absorbed = Hovered->OnEvent(event);

			if (!absorbed && UserReceiver)
				UserReceiver->OnEvent(event);

		}
		break;
	case EET_KEY_INPUT_EVENT:
		{
			bool absorbed = false;

			if (KeyFocus && KeyFocus != this)
				absorbed = KeyFocus->OnEvent(event);

			if (!absorbed && UserReceiver)
				UserReceiver->OnEvent(event);
			break;
		}
	default:
		return;
	}
}



//! returns the current gui skin
IGUISkin* CGUIEnvironment::getSkin()
{
	return CurrentSkin;
}


//! adds an button. The returned pointer must not be dropped.
IGUIButton* CGUIEnvironment::addButton(const core::rectEx<s32>& rectangle, IGUIElement* parent, s32 id, const wchar_t* text)
{
	IGUIButton* button = new CGUIButton(this, parent ? parent : this, id, rectangle);
	if (text)
		button->setText(text);
	button->drop();
	return button;
}


//! adds a window. The returned pointer must not be dropped.
IGUIWindow* CGUIEnvironment::addWindow(const core::rectEx<s32>& rectangle, IGUIElement* parent, s32 id, const wchar_t* text)
{
	IGUIWindow* win = new CGUIWindow(this, parent ? parent : this, id, rectangle);
	if (text)
		win->setText(text);
	win->drop();
	return win;
}


//! adds a scrollbar. The returned pointer must not be dropped.
IGUIScrollBar* CGUIEnvironment::addScrollBar(bool horizontal, const core::rectEx<s32>& rectangle, IGUIElement* parent, s32 id)
{
	IGUIScrollBar* bar = new CGUIScrollBar(horizontal, this, parent ? parent : this, id, rectangle);
	bar->drop();
	return bar;
}


//! adds an image. The returned pointer must not be dropped.
IGUIImage* CGUIEnvironment::addImage(const core::rectEx<s32>& rectangle, IGUIElement* parent, s32 id, const wchar_t* text)
{
	IGUIImage* img = new CGUIImage(this, parent ? parent : this, id, rectangle);
	if (text)
		img->setText(text);
	img->drop();
	return img;
}


//! adds an mesh viewer. The returned pointer must not be dropped.
IGUIMeshViewer* CGUIEnvironment::addMeshViewer(const core::rectEx<s32>& rectangle, IGUIElement* parent, s32 id, const wchar_t* text)
{
	IGUIMeshViewer* v = new CGUIMeshViewer(this, parent ? parent : this, id, rectangle);
	if (text)
		v->setText(text);
	v->drop();
	return v;
}


//! adds a checkbox
IGUICheckBox* CGUIEnvironment::addCheckBox(bool checked, const core::rectEx<s32>& rectangle, IGUIElement* parent, s32 id, const wchar_t* text)
{
	IGUICheckBox* b = new CGUICheckBox(checked, this, parent ? parent : this , id , rectangle);
	if (text)
		b->setText(text);
	b->drop();
	return b;
}



//! adds a list box
IGUIListBox* CGUIEnvironment::addListBox(const core::rectEx<s32>& rectangle, IGUIElement* parent, s32 id)
{
	IGUIListBox* b = new CGUIListBox(this, parent ? parent : this, id, rectangle);
	b->setIconFont(getBuildInFont());
	b->drop();
	return b;

}



//! adds a file open dialog. The returned pointer must not be dropped.
IGUIFileOpenDialog* CGUIEnvironment::addFileOpenDialog(const wchar_t* title, IGUIElement* parent, s32 id)
{
	IGUIFileOpenDialog* d = new CGUIFileOpenDialog(FileSystem, title, this, parent ? parent : this, id);
	d->drop();
	return d;
}



//! adds a static text. The returned pointer must not be dropped.
IGUIElement* CGUIEnvironment::addStaticText(const wchar_t* text, bool border, const core::rectEx<s32>& rectangle, IGUIElement* parent, s32 id)
{
	IGUIElement* d = new CGUIStaticText(text, border, this, parent ? parent : this, id, rectangle);
	d->drop();
	return d;
}



//! returns the font
IGUIFont* CGUIEnvironment::getFont(const c8* filename)
{
	// search existing font

	SFont f;
	if (!filename)
		filename = "";

	f.Filename = filename;
	f.Filename.make_lower();

	s32 index = Fonts.binary_search(f);
	if (index != -1)
		return Fonts[index].Font;

    // not existing yet. try to load font.

	CGUIFont* font = new CGUIFont(Driver);
	if (!font->load(filename))
	{
		font->drop();
		return 0;
	}

	// add to fonts.

	f.Font = font;
	Fonts.push_back(f);

	return font;
}



//! returns default font
IGUIFont* CGUIEnvironment::getBuildInFont()
{
	if (Fonts.empty())
		return 0;

	return  Fonts[0].Font;
}



//! creates an GUI Environment
IGUIEnvironment* createGUIEnvironment(io::IFileSystem* fs, video::IVideoDriver* Driver, IEventReceiver* userReceiver)
{
	return new CGUIEnvironment(fs, Driver, userReceiver);
}

} // end namespace gui
} // end namespace irr
