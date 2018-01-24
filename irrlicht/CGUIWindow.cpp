#include "CGUIWindow.h"
#include "IGUISkin.h"
#include "IGUIEnvironment.h"
#include "IVideoDriver.h"
#include "IGUIButton.h"
#include "IGUIFont.h"
#include "GUIIcons.h"

namespace irr
{
namespace gui
{

//! constructor
CGUIWindow::CGUIWindow(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rectEx<s32> rectangle)
: IGUIWindow(environment, parent, id, rectangle), Dragging(false)
{
	#ifdef _DEBUG
	setDebugName("CGUIWindow");
	#endif

	s32 buttonw = environment->getSkin()->getSize(EGDS_WINDOW_BUTTON_WIDTH);
	s32 posx = RelativeRect.getWidth() - buttonw - 4;

	CloseButton = Environment->addButton(core::rectEx<s32>(posx, 3, posx + buttonw, 3 + buttonw), this, -1, GUI_ICON_WINDOW_CLOSE);
	CloseButton->setOverrideFont(Environment->getBuildInFont());
	posx -= buttonw + 2;

	RestoreButton = Environment->addButton(core::rectEx<s32>(posx, 3, posx + buttonw, 3 + buttonw), this, -1, GUI_ICON_WINDOW_RESTORE);
	RestoreButton->setOverrideFont(Environment->getBuildInFont());
	posx -= buttonw + 2;

	MinButton = Environment->addButton(core::rectEx<s32>(posx, 3, posx + buttonw, 3 + buttonw), this, -1, GUI_ICON_WINDOW_MINIMIZE);
	MinButton->setOverrideFont(Environment->getBuildInFont());

	MinButton->grab();
	RestoreButton->grab();
	CloseButton->grab();
}



//! destructor
CGUIWindow::~CGUIWindow()
{
	if (MinButton)
		MinButton->drop();

	if (RestoreButton)
		RestoreButton->drop();

	if (CloseButton)
		CloseButton->drop();
}



//! called if an event happened.
bool CGUIWindow::OnEvent(SEvent event)
{
	switch(event.EventType)
	{
	case EET_GUI_EVENT:
		if (event.GUIEvent.EventType == EGET_ELEMENT_MOUSE_FOCUS_LOST)
		{
			Dragging = false;
			return true;
		}
		else
		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED)
		{
			if (event.GUIEvent.Caller == CloseButton)
			{
				remove();
				return true;
			}
		}
		break;
	case EET_MOUSE_INPUT_EVENT:
		switch(event.MouseInput.Event)
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			DragStart.X = event.MouseInput.X;
			DragStart.Y = event.MouseInput.Y;
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
				move(core::position2d<s32>(event.MouseInput.X - DragStart.X, event.MouseInput.Y - DragStart.Y));
				DragStart.X = event.MouseInput.X;
				DragStart.Y = event.MouseInput.Y;
				return true;
			}
			break;
		}
	}

	return Parent ? Parent->OnEvent(event) : false;
}



//! draws the element and its children
void CGUIWindow::draw()
{
	if (!IsVisible)
		return;

	IGUISkin* skin = Environment->getSkin();
	irr::video::IVideoDriver* driver = Environment->getVideoDriver();

	core::rectEx<s32> rect = AbsoluteRect;

	// draw body

	/*driver->draw2DRectangle(skin->getColor(EGDC_3D_DARK_SHADOW), rect, AbsoluteClippingRect);

	rect.LowerRightCorner.X -= 1;
	rect.LowerRightCorner.Y -= 1;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_HIGH_LIGHT), rect, AbsoluteClippingRect);

	rect.UpperLeftCorner.X += 1;
	rect.UpperLeftCorner.Y += 1;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_SHADOW), rect, AbsoluteClippingRect);

	rect.LowerRightCorner.X -= 1;
	rect.LowerRightCorner.Y -= 1;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_FACE), rect, AbsoluteClippingRect);*/

	// draw body fast
	
	rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + 1;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_HIGH_LIGHT), rect, &AbsoluteClippingRect);

	rect.LowerRightCorner.Y = AbsoluteRect.LowerRightCorner.Y;
	rect.LowerRightCorner.X = rect.UpperLeftCorner.X + 1;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_HIGH_LIGHT), rect, &AbsoluteClippingRect);

	rect.UpperLeftCorner.X = AbsoluteRect.LowerRightCorner.X - 1;
	rect.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X;
	rect.UpperLeftCorner.Y = AbsoluteRect.UpperLeftCorner.Y;
    rect.LowerRightCorner.Y = AbsoluteRect.LowerRightCorner.Y;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_DARK_SHADOW), rect, &AbsoluteClippingRect);

	rect.UpperLeftCorner.X -= 1;
	rect.LowerRightCorner.X -= 1;
	rect.UpperLeftCorner.Y += 1;
	rect.LowerRightCorner.Y -= 1;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_SHADOW), rect, &AbsoluteClippingRect);

	rect.UpperLeftCorner.X = AbsoluteRect.UpperLeftCorner.X;
	rect.UpperLeftCorner.Y = AbsoluteRect.LowerRightCorner.Y - 1;
	rect.LowerRightCorner.Y = AbsoluteRect.LowerRightCorner.Y;
	rect.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_DARK_SHADOW), rect, &AbsoluteClippingRect);

	rect.UpperLeftCorner.X += 1;
	rect.LowerRightCorner.X -= 1;
	rect.UpperLeftCorner.Y -= 1;
	rect.LowerRightCorner.Y -= 1;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_SHADOW), rect, &AbsoluteClippingRect);

	rect = AbsoluteRect;
	rect.UpperLeftCorner.X +=1;
	rect.UpperLeftCorner.Y +=1;
	rect.LowerRightCorner.X -= 2;
	rect.LowerRightCorner.Y -= 2;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_FACE), rect, &AbsoluteClippingRect);

	// draw title bar

	rect = AbsoluteRect;
	rect.UpperLeftCorner.X += 2;
	rect.UpperLeftCorner.Y += 2;
	rect.LowerRightCorner.X -= 2;
	rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + skin->getSize(EGDS_WINDOW_BUTTON_WIDTH) + 2;
	driver->draw2DRectangle(skin->getColor(EGDC_ACTIVE_BORDER), rect, &AbsoluteClippingRect);

	if (Text.size())
	{
		rect.UpperLeftCorner.X += 2;
		rect.LowerRightCorner.X -= skin->getSize(EGDS_WINDOW_BUTTON_WIDTH) + 5;

		IGUIFont* font = skin->getFont();
		if (font)
			font->draw(Text.c_str(), rect, skin->getColor(EGDC_ACTIVE_CAPTION), false, true, &rect);
	}

	IGUIElement::draw();
}


} // end namespace gui
} // end namespace irr
