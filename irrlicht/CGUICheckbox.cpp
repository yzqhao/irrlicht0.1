#include "CGUICheckBox.h"
#include "IGUISkin.h"
#include "IGUIEnvironment.h"
#include "IVideoDriver.h"
#include "IGUIFont.h"
#include "GUIIcons.h"

namespace irr
{
namespace gui
{

//! constructor
CGUICheckBox::CGUICheckBox(bool checked, IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rectEx<s32> rectangle)
: IGUICheckBox(environment, parent, id, rectangle), Pressed(false), Checked(checked)
{
	#ifdef _DEBUG
	setDebugName("CGUICheckBox");
	#endif
}



//! destructor
CGUICheckBox::~CGUICheckBox()
{
}



//! called if an event happened.
bool CGUICheckBox::OnEvent(SEvent event)
{
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
				Checked = !Checked;
				event.GUIEvent.EventType = EGET_CHECKBOX_CHANGED;
				Parent->OnEvent(event);
			}

			return true;
		}
		break;
	}

	return Parent ? Parent->OnEvent(event) : false;
}



//! draws the element and its children
void CGUICheckBox::draw()
{
	if (!IsVisible)
		return;

	IGUISkin* skin = Environment->getSkin();
	irr::video::IVideoDriver* driver = Environment->getVideoDriver();

	core::rectEx<s32> rect = AbsoluteRect;

	s32 height = skin->getSize(EGDS_CHECK_BOX_WIDTH);

	core::rectEx<s32> checkRect(AbsoluteRect.UpperLeftCorner.X,
								((AbsoluteRect.getHeight() - height) / 2) + AbsoluteRect.UpperLeftCorner.Y,
								 0, 0);
	checkRect.LowerRightCorner.X = checkRect.UpperLeftCorner.X + height;
	checkRect.LowerRightCorner.Y = checkRect.UpperLeftCorner.Y + height;



	driver->draw2DRectangle(skin->getColor(EGDC_3D_HIGH_LIGHT), checkRect, &AbsoluteClippingRect);

	checkRect.LowerRightCorner.X -= 1;
	checkRect.LowerRightCorner.Y -= 1;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_SHADOW), checkRect, &AbsoluteClippingRect);

	checkRect.UpperLeftCorner.X += 1;
	checkRect.UpperLeftCorner.Y += 1;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_LIGHT), checkRect, &AbsoluteClippingRect);

	checkRect.LowerRightCorner.X -= 1;
	checkRect.LowerRightCorner.Y -= 1;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_DARK_SHADOW), checkRect, &AbsoluteClippingRect);

	checkRect.UpperLeftCorner.X += 1;
	checkRect.UpperLeftCorner.Y += 1;
	driver->draw2DRectangle(skin->getColor(Pressed ? EGDC_3D_FACE : EGDC_ACTIVE_CAPTION), checkRect, &AbsoluteClippingRect);

	if (Checked && Environment->getBuildInFont())
		Environment->getBuildInFont()->draw(GUI_ICON_CHECK_BOX_CHECKED, checkRect, skin->getColor(EGDC_BUTTON_TEXT), true, true, &checkRect);

	if (Text.size())
	{
		checkRect = AbsoluteRect;
		checkRect.UpperLeftCorner.X += height + 5;

		IGUIFont* font = skin->getFont();
		if (font)
			font->draw(Text.c_str(), checkRect, skin->getColor(EGDC_BUTTON_TEXT), false, true, &checkRect);
	}

	IGUIElement::draw();
}


//! set if box is checked
void CGUICheckBox::setChecked(bool checked)
{
	Checked = checked;
}


//! returns if box is checked
bool CGUICheckBox::isChecked()
{
	return Checked;
}



} // end namespace gui
} // end namespace irr
