#include "CGUIScrollBar.h"
#include "IGUISkin.h"
#include "IGUIEnvironment.h"
#include "IVideoDriver.h"
#include "GUIIcons.h"

namespace irr
{
namespace gui
{


//! constructor
CGUIScrollBar::CGUIScrollBar(bool horizontal, IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rectEx<s32> rectangle)
: IGUIScrollBar(environment, parent, id, rectangle), UpButton(0), DownButton(0),
	Horizontal(horizontal), Pos(0), Max(100), SmallStep(2), DrawHeight(0),
	DrawPos(0), Dragging(false)
{
	#ifdef _DEBUG
	setDebugName("CGUIScrollBar");
	#endif

	if (horizontal)
	{
		s32 h = RelativeRect.getHeight();
		UpButton = Environment->addButton(core::rectEx<s32>(0,0, h, h), this, -1, GUI_ICON_CURSOR_LEFT);
		DownButton = Environment->addButton(core::rectEx<s32>(RelativeRect.getWidth()-h, 0, RelativeRect.getWidth(), h), this, -1, GUI_ICON_CURSOR_RIGHT);
	}
	else
	{
		s32 w = RelativeRect.getWidth();
		UpButton = Environment->addButton(core::rectEx<s32>(0,0, w, w), this, -1, GUI_ICON_CURSOR_UP);
		DownButton = Environment->addButton(core::rectEx<s32>(0,RelativeRect.getHeight()-w, w, RelativeRect.getHeight()), this, -1, GUI_ICON_CURSOR_DOWN);
	}

	if (UpButton)
	{
		UpButton->setOverrideFont(Environment->getBuildInFont());
		UpButton->grab();
	}

	if (DownButton)
	{
		DownButton->setOverrideFont(Environment->getBuildInFont());
		DownButton->grab();
	}

	setPos(50);
}


//! destructor
CGUIScrollBar::~CGUIScrollBar()
{
	if (UpButton)
		UpButton->drop();

	if (DownButton)
		DownButton->drop();
}


//! called if an event happened.
bool CGUIScrollBar::OnEvent(SEvent event)
{
	switch(event.EventType)
	{
	case EET_GUI_EVENT:
		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED)
		{
			if (event.GUIEvent.Caller == UpButton)
				setPos(Pos-SmallStep);
			else
			if (event.GUIEvent.Caller == DownButton)
				setPos(Pos+SmallStep);

			SEvent event;
			event.EventType = EET_GUI_EVENT;
			event.GUIEvent.Caller = this;
			event.GUIEvent.EventType = EGET_SCROLL_BAR_CHANGED;
			Parent->OnEvent(event);

			return true;
		}
		else
		if (event.GUIEvent.EventType == EGET_ELEMENT_MOUSE_FOCUS_LOST)
		{
			Dragging = false;
			return true;
		}
		break;
	case EET_MOUSE_INPUT_EVENT:
		switch(event.MouseInput.Event)
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			Dragging = true;
			Environment->setMouseFocus(this);
			return true;
		case EMIE_LMOUSE_LEFT_UP:
			Dragging = false;
			Environment->removeMouseFocus(this);
			return true;
		case EMIE_MOUSE_MOVED:
			if (Dragging)
			{
				s32 oldPos = Pos;
				setPosFromMousePos(event.MouseInput.X, event.MouseInput.Y);
				if (Pos != oldPos && Parent)
				{
					SEvent event;
					event.EventType = EET_GUI_EVENT;
					event.GUIEvent.Caller = this;
					event.GUIEvent.EventType = EGET_SCROLL_BAR_CHANGED;
					Parent->OnEvent(event);
				}
				return true;
			}
		}
		break;
	}

	return Parent ? Parent->OnEvent(event) : false;
}

//! draws the element and its children
void CGUIScrollBar::draw()
{
	if (!IsVisible)
		return;

	IGUISkin* skin = Environment->getSkin();
	irr::video::IVideoDriver* driver = Environment->getVideoDriver();

	core::rectEx<s32> rect = AbsoluteRect;

	// draws the background
	driver->draw2DRectangle(skin->getColor(EGDC_SCROLLBAR), rect, &AbsoluteClippingRect);

	if (Max!=0)
	{
		// draw thumb
		if (Horizontal)
		{
			rect.UpperLeftCorner.X = AbsoluteRect.UpperLeftCorner.X + DrawPos + RelativeRect.getHeight() - DrawHeight/2;
			rect.LowerRightCorner.X = rect.UpperLeftCorner.X + DrawHeight;	
		}
		else
		{
			rect.UpperLeftCorner.Y = AbsoluteRect.UpperLeftCorner.Y + DrawPos + RelativeRect.getWidth() - DrawHeight/2;
			rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + DrawHeight;	
		}

		driver->draw2DRectangle(skin->getColor(EGDC_3D_DARK_SHADOW), rect, &AbsoluteClippingRect);

		rect.LowerRightCorner.X -= 1;
		rect.LowerRightCorner.Y -= 1;
		driver->draw2DRectangle(skin->getColor(EGDC_3D_HIGH_LIGHT), rect, &AbsoluteClippingRect);

		rect.UpperLeftCorner.X += 1;
		rect.UpperLeftCorner.Y += 1;
		driver->draw2DRectangle(skin->getColor(EGDC_3D_SHADOW), rect, &AbsoluteClippingRect);

		rect.LowerRightCorner.X -= 1;
		rect.LowerRightCorner.Y -= 1;
		driver->draw2DRectangle(skin->getColor(EGDC_3D_FACE), rect, &AbsoluteClippingRect);

	}

	// draw buttons
	IGUIElement::draw();
}



void CGUIScrollBar::setPosFromMousePos(s32 x, s32 y)
{
	if (Horizontal)
	{
		f32 f = (RelativeRect.getWidth() - ((f32)RelativeRect.getHeight()*3.0f)) / (f32)Max;	
		setPos((s32)(((f32)(x - AbsoluteRect.UpperLeftCorner.X - RelativeRect.getHeight())) / f));
	}
	else
	{
		f32 f = (RelativeRect.getHeight() - ((f32)RelativeRect.getWidth()*3.0f)) / (f32)Max;
		setPos((s32)(((f32)y - AbsoluteRect.UpperLeftCorner.Y - RelativeRect.getWidth()) / f));
	}
}



//! sets the position of the scrollbar
void CGUIScrollBar::setPos(s32 pos)
{
	Pos = pos;
	if (Pos < 0)
		Pos = 0;
	if (Pos > Max)
		Pos = Max;

    if (Horizontal)
	{
		f32 f = (RelativeRect.getWidth() - ((f32)RelativeRect.getHeight()*3.0f)) / (f32)Max;	
		DrawPos = (s32)((Pos * f) + ((f32)RelativeRect.getHeight() * 0.5f));
		DrawHeight = RelativeRect.getHeight();
	}
	else
	{
		f32 f = (RelativeRect.getHeight() - ((f32)RelativeRect.getWidth()*3.0f)) / (f32)Max;
		DrawPos = (s32)((Pos * f) + ((f32)RelativeRect.getWidth() * 0.5f));
		DrawHeight = RelativeRect.getWidth();
	}
}



//! sets the maximum value of the scrollbar. must be > 0
void CGUIScrollBar::setMax(s32 max)
{
	if (max > 0)
		Max = max;
	else
		Max = 0;
}



//! gets the current position of the scrollbar
s32 CGUIScrollBar::getPos()
{
	return Pos;
}


} // end namespace gui
} // end namespace klib