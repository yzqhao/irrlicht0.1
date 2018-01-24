#include "CGUIListBox.h"
#include "IGUISkin.h"
#include "IGUIEnvironment.h"
#include "IVideoDriver.h"
#include "IGUIFont.h"
#include "IGUIScrollBar.h"

namespace irr
{
namespace gui
{

//! constructor
CGUIListBox::CGUIListBox(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rectEx<s32> rectangle)
: IGUIListBox(environment, parent, id, rectangle), Selected(-1), ScrollBar(0), 
	ItemHeight(0), TotalItemHeight(0), Selecting(false), Font(0), IconFont(0), ItemsIconWidth(0)
{
	#ifdef _DEBUG
	setDebugName("CGUIListBox");
	#endif

	IGUISkin* skin = Environment->getSkin();
	s32 s = skin->getSize(EGDS_SCROLLBAR_SIZE);
	ScrollBar = Environment->addScrollBar(false, core::rectEx<s32>(RelativeRect.getWidth() - s, 0, RelativeRect.getWidth(), RelativeRect.getHeight()), this);
	ScrollBar->setPos(0);
	ScrollBar->grab();
	
	recalculateItemHeight();
}


//! destructor
CGUIListBox::~CGUIListBox()
{
	if (ScrollBar)
		ScrollBar->drop();

	if (Font)
		Font->drop();

	if (IconFont)
		IconFont->drop();
}



//! returns amount of list items
s32 CGUIListBox::getItemCount()
{
	return Items.size();
}



//! returns string of a list item. the may be a value from 0 to itemCount-1
const wchar_t* CGUIListBox::getListItem(s32 id)
{
	if (id<0 || id>((s32)Items.size())-1)
		return 0;

	return Items[id].text.c_str();
}



//! adds an list item, returns id of item
s32 CGUIListBox::addItem(const wchar_t* text)
{
	ListItem i;
	i.text = text;

	Items.push_back(i);
	recalculateItemHeight();
	return Items.size() - 1;
}



//! clears the list
void CGUIListBox::clear()
{
	Items.clear();
	ItemsIconWidth = 0;
	Selected = -1;

	if (ScrollBar)
		ScrollBar->setPos(0);

	recalculateItemHeight();
}



void CGUIListBox::recalculateItemHeight()
{
	IGUISkin* skin = Environment->getSkin();

	if (Font != skin->getFont())
	{
		if (Font)
			Font->drop();

		Font = skin->getFont();
		ItemHeight = 0;

		if (Font)
		{
			ItemHeight = Font->getDimension(L"A").Height + 4;
			Font->grab();
		}
	}

	TotalItemHeight = ItemHeight * Items.size();
	ScrollBar->setMax(TotalItemHeight - AbsoluteRect.getHeight());
}



//! returns id of selected item. returns -1 if no item is selected.
s32 CGUIListBox::getSelected()
{
	return Selected;
}



//! sets the selected item. Set this to -1 if no item should be selected
void CGUIListBox::setSelected(s32 id)
{
	if (id<0 || id>((s32)Items.size())-1)
		Selected = -1;
	else
		Selected = id;
}



//! called if an event happened.
bool CGUIListBox::OnEvent(SEvent event)
{
	switch(event.EventType)
	{
	case EET_GUI_EVENT:
		switch(event.GUIEvent.EventType)
		{
		case gui::EGET_SCROLL_BAR_CHANGED:
			if (event.GUIEvent.Caller == ScrollBar)
			{
				s32 pos = ((gui::IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
				return true;
			}
			break;
		case gui::EGET_ELEMENT_MOUSE_FOCUS_LOST:
			{
				Selecting = false;
				return true;
			}
			break;
		}
		break;
	case EET_MOUSE_INPUT_EVENT:
		switch(event.MouseInput.Event)
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			Selecting = true;
			Environment->setMouseFocus(this);
			return true;
		case EMIE_LMOUSE_LEFT_UP:
			Selecting = false;
			Environment->removeMouseFocus(this);
			selectNew(event.MouseInput.Y);
			return true;
		case EMIE_MOUSE_MOVED:
			if (Selecting)
			{
				selectNew(event.MouseInput.Y);
				return true;
			}
		}
		break;
	}


	return Parent ? Parent->OnEvent(event) : false;
}


void CGUIListBox::selectNew(s32 ypos)
{
	s32 oldSelected = Selected;

	// find new selected item.
	if (ItemHeight!=0)
		Selected = ((ypos - AbsoluteRect.UpperLeftCorner.Y - 1) + ScrollBar->getPos()) / ItemHeight;

	if (Selected >= (s32)Items.size())
		Selected = Items.size() - 1;
	else 
	if (Selected<0)
		Selected = 0;

	// post the news

	if (Parent)
	{
		SEvent event;
		event.EventType = EET_GUI_EVENT;
		event.GUIEvent.Caller = this;
		event.GUIEvent.EventType = (Selected != oldSelected) ? EGET_LISTBOX_CHANGED : EGET_LISTBOX_SELECTED_AGAIN;
		Parent->OnEvent(event);
	}
}


//! draws the element and its children
void CGUIListBox::draw()
{
	if (!IsVisible)
		return;

	recalculateItemHeight(); // if the font changed

	IGUISkin* skin = Environment->getSkin();
	irr::video::IVideoDriver* driver = Environment->getVideoDriver();

	// draw the border
	core::rectEx<s32> frameRect(AbsoluteRect);
	frameRect.LowerRightCorner.Y = frameRect.UpperLeftCorner.Y + 1;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_SHADOW), frameRect, &AbsoluteClippingRect);

	frameRect.LowerRightCorner.Y = AbsoluteRect.LowerRightCorner.Y;
	frameRect.LowerRightCorner.X = frameRect.UpperLeftCorner.X + 1;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_SHADOW), frameRect, &AbsoluteClippingRect);

	frameRect = AbsoluteRect;
	frameRect.UpperLeftCorner.X = frameRect.LowerRightCorner.X - 1;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_HIGH_LIGHT), frameRect, &AbsoluteClippingRect);

	frameRect = AbsoluteRect;
	frameRect.UpperLeftCorner.Y = AbsoluteRect.LowerRightCorner.Y - 1;
	frameRect.LowerRightCorner.Y = AbsoluteRect.LowerRightCorner.Y;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_HIGH_LIGHT), frameRect, &AbsoluteClippingRect);

	// draw items

	core::rectEx<s32> clientClip(AbsoluteRect);
	clientClip.UpperLeftCorner.Y += 1;
	clientClip.UpperLeftCorner.X += 1;
	clientClip.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X - skin->getSize(EGDS_SCROLLBAR_SIZE);
	clientClip.LowerRightCorner.Y -= 1;
	clientClip.clipAgainst(AbsoluteClippingRect);

	frameRect = AbsoluteRect;
	frameRect.UpperLeftCorner.X += 1;
	frameRect.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X - skin->getSize(EGDS_SCROLLBAR_SIZE);
	frameRect.LowerRightCorner.Y = AbsoluteRect.UpperLeftCorner.Y + ItemHeight;

	frameRect.UpperLeftCorner.Y -= ScrollBar->getPos();
	frameRect.LowerRightCorner.Y -= ScrollBar->getPos();
	

	for (s32 i=0; i<(s32)Items.size(); ++i)
	{
		if (frameRect.LowerRightCorner.Y >= AbsoluteRect.UpperLeftCorner.Y &&
			frameRect.UpperLeftCorner.Y <= AbsoluteRect.LowerRightCorner.Y)
		{
			if (i == Selected)
				driver->draw2DRectangle(skin->getColor(EGDC_HIGH_LIGHT), frameRect, &clientClip);

			core::rectEx<s32> textRect = frameRect;
			textRect.UpperLeftCorner.X += 3;

			if (Font)
			{
				if (IconFont && Items[i].icon.size())
					IconFont->draw(Items[i].icon.c_str(), textRect, skin->getColor((i==Selected) ? EGDC_HIGH_LIGHT_TEXT : EGDC_BUTTON_TEXT), false, true, &clientClip);

				textRect.UpperLeftCorner.X += ItemsIconWidth+3;

				Font->draw(Items[i].text.c_str(), textRect, skin->getColor((i==Selected) ? EGDC_HIGH_LIGHT_TEXT : EGDC_BUTTON_TEXT), false, true, &clientClip);

				textRect.UpperLeftCorner.X -= ItemsIconWidth+3;
			}
		}

		frameRect.UpperLeftCorner.Y += ItemHeight;
		frameRect.LowerRightCorner.Y += ItemHeight;
	}

	IGUIElement::draw();
}



//! adds an list item with an icon
//! \param
//! text: Text of list entry
//!  icon: Text of the Icon. This text can be for example one of the texts defined in
//!  GUIIcons.h. 
//! \return
//! returns the id of the new created item
s32 CGUIListBox::addItem(const wchar_t* text, const wchar_t* icon)
{
	ListItem i;
	i.text = text;
	i.icon = icon;

	Items.push_back(i);
	recalculateItemHeight();

	if (IconFont)
	{
		s32 w = IconFont->getDimension(icon).Width;
		if (w > ItemsIconWidth)
			ItemsIconWidth = w;
	}

    return Items.size() - 1;
}


//! Sets the font which should be used as icon font. This font is set to the Irrlicht engine
//! built-in-font by default. Icons can be displayed in front of every list item.
//! An icon is a string, displayed with the icon font. When using the build-in-font of the
//! Irrlicht engine as icon font, the icon strings defined in GUIIcons.h can be used.
void CGUIListBox::setIconFont(IGUIFont* font)
{
	if (IconFont)
		IconFont->drop();

	IconFont = font;
	if (IconFont)
		IconFont->grab();
}




} // end namespace gui
} // end namespace irr
