#include "CGUIButton.h"
#include "IGUISkin.h"
#include "IGUIEnvironment.h"
#include "IVideoDriver.h"
#include "IGUIFont.h"

namespace irr
{
namespace gui
{

//! constructor
CGUIButton::CGUIButton(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rectEx<s32> rectangle)
: IGUIButton(environment, parent, id, rectangle), Pressed(false), OverrideFont(0)
{
	#ifdef _DEBUG
	setDebugName("CGUIButton");
	#endif
}



//! destructor
CGUIButton::~CGUIButton()
{
	if (OverrideFont)
		OverrideFont->drop();
}



//! called if an event happened.
bool CGUIButton::OnEvent(SEvent event)
{
	if (!IsEnabled)
		return Parent ? Parent->OnEvent(event) : false;

	switch(event.EventType)
	{
	case EET_GUI_EVENT:
		if (event.GUIEvent.EventType == EGET_ELEMENT_MOUSE_FOCUS_LOST)
		{
			Pressed = false;
			return true;
		}
		break;
	case EET_MOUSE_INPUT_EVENT:
		if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
		{
			Pressed = true;
			Environment->setMouseFocus(this);
			return true;
		}
		else
		if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
		{
			bool wasPressed = Pressed;
			Environment->removeMouseFocus(this);
			Pressed = false;

			if (wasPressed && Parent)
			{
				SEvent event;
				event.EventType = EET_GUI_EVENT;
				event.GUIEvent.Caller = this;
				event.GUIEvent.EventType = EGET_BUTTON_CLICKED;
				Parent->OnEvent(event);
			}

			return true;
		}
		break;
	}

	return Parent ? Parent->OnEvent(event) : false;
}



//! draws the element and its children
void CGUIButton::draw()
{
	if (!IsVisible)
		return;

	IGUISkin* skin = Environment->getSkin();
	irr::video::IVideoDriver* driver = Environment->getVideoDriver();

	IGUIFont* font = OverrideFont;
	if (!OverrideFont)
		font = skin->getFont();

	core::rectEx<s32> rect = AbsoluteRect;

	if (!Pressed)
	{
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
	else
	{
		driver->draw2DRectangle(skin->getColor(EGDC_3D_HIGH_LIGHT), rect, &AbsoluteClippingRect);

		rect.LowerRightCorner.X -= 1;
		rect.LowerRightCorner.Y -= 1;
		driver->draw2DRectangle(skin->getColor(EGDC_3D_DARK_SHADOW), rect, &AbsoluteClippingRect);

		rect.UpperLeftCorner.X += 1;
		rect.UpperLeftCorner.Y += 1;
		driver->draw2DRectangle(skin->getColor(EGDC_3D_SHADOW), rect, &AbsoluteClippingRect);

		rect.UpperLeftCorner.X += 1;
		rect.UpperLeftCorner.Y += 1;
		driver->draw2DRectangle(skin->getColor(EGDC_3D_FACE), rect, &AbsoluteClippingRect);
	}

	if (Text.size())
	{
		rect = AbsoluteRect;
        if (Pressed)
			rect.UpperLeftCorner.Y += 2;

		if (font)
			font->draw(Text.c_str(), rect,
			skin->getColor(IsEnabled ? EGDC_BUTTON_TEXT : EGDC_GRAY_TEXT), true, true, &rect);
	}

	IGUIElement::draw();
}



//! sets another skin independent font. if this is set to zero, the button uses the font of the skin.
void CGUIButton::setOverrideFont(IGUIFont* font)
{
	if (OverrideFont)
		OverrideFont->drop();

	OverrideFont = font;

	if (OverrideFont)
		OverrideFont->grab();
}


} // end namespace gui
} // end namespace irr
