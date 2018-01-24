#include "CGUIStaticText.h"
#include "IGUISkin.h"
#include "IGUIEnvironment.h"
#include "IGUIFont.h"
#include "IVideoDriver.H"

namespace irr
{
namespace gui
{



//! constructor
CGUIStaticText::CGUIStaticText(const wchar_t* text, bool border, IGUIEnvironment* environment, IGUIElement* parent, s32 id, const core::rectEx<s32>& rectangle)
: IGUIElement(environment, parent, id, rectangle), Border(border)
{
	#ifdef _DEBUG
	setDebugName("CGUIStaticText");
	#endif

	Text = text;
}


//! destructor
CGUIStaticText::~CGUIStaticText()
{
}


//! draws the element and its children
void CGUIStaticText::draw()
{
	if (!IsVisible)
		return;

	IGUISkin* skin = Environment->getSkin();
	irr::video::IVideoDriver* driver = Environment->getVideoDriver();

	core::rectEx<s32> frameRect(AbsoluteRect);

	// draw the border

	if (Border)
	{
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

		frameRect = AbsoluteRect;
		frameRect.UpperLeftCorner.X += 3;
	}


	// draw the text
	if (Text.size())
	{
		IGUIFont* font = skin->getFont();
		if (font)
			font->draw(Text.c_str(), frameRect, skin->getColor(EGDC_BUTTON_TEXT), false, true, &AbsoluteRect);
	}

	IGUIElement::draw();
}



} // end namespace gui
} // end namespace irr

