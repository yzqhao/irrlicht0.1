#include "CGUISkin.h"
#include "IGUIFont.h"

namespace irr
{
namespace gui
{

CGUISkin::CGUISkin(EGUI_SKIN_TYPE type)
: Font(0)
{
	#ifdef _DEBUG
	setDebugName("CGUISkin");
	#endif


	Colors[EGDC_3D_DARK_SHADOW] =	video::Color(101,50,50,50);
	Colors[EGDC_3D_SHADOW] =		video::Color(101,130,130,130);
	Colors[EGDC_3D_FACE] =			video::Color(101,210,210,210);
	Colors[EGDC_3D_HIGH_LIGHT] =	video::Color(101,255,255,255);
	Colors[EGDC_3D_LIGHT] =			video::Color(101,210,210,210);
	Colors[EGDC_ACTIVE_BORDER] =	video::Color(101,16,14,115);
	Colors[EGDC_ACTIVE_CAPTION] =	video::Color(101,255,255,255);
	Colors[EGDC_APP_WORKSPACE] =	video::Color(101,100,100,100);
	Colors[EGDC_BUTTON_TEXT] =		video::Color(101,0,0,0);
	Colors[EGDC_GRAY_TEXT] =		video::Color(101,130,130,130);
	Colors[EGDC_HIGH_LIGHT] =		video::Color(101,8,36,107);
	Colors[EGDC_HIGH_LIGHT_TEXT] =	video::Color(101,255,255,255);
	Colors[EGDC_INACTIVE_BORDER] =	video::Color(101,165,165,165);
	Colors[EGDC_INACTIVE_CAPTION] = video::Color(101,210,210,210);
	Colors[EGDC_TOOLTIP] =			video::Color(101,255,255,230);
	Colors[EGDC_SCROLLBAR] =		video::Color(101,230,230,230);
	Colors[EGDC_WINDOW] =			video::Color(101,255,255,255);

	Sizes[EGDS_SCROLLBAR_SIZE] = 14;
	Sizes[EGDS_MENU_HEIGHT] = 18;
	Sizes[EGDS_WINDOW_BUTTON_WIDTH] = 15;
	Sizes[EGDS_CHECK_BOX_WIDTH] = 18;
}


//! destructor
CGUISkin::~CGUISkin()
{
	if (Font)
		Font->drop();
}



//! returns default color
video::Color CGUISkin::getColor(EGUI_DEFAULT_COLOR color)
{
	return Colors[color];
}


//! sets a default color
void CGUISkin::setColor(EGUI_DEFAULT_COLOR which, video::Color newColor)
{
	if (which>=0 && which<= EGDC_COUNT)
		Colors[which] = newColor;
}


//! returns default color
s32 CGUISkin::getSize(EGUI_DEFAULT_SIZE size)
{
	return Sizes[size];
}



//! sets a default size
void CGUISkin::setSize(s32 size)
{
	if (size>=0 && size <= EGDS_COUNT)
		Sizes[size] = size;
}



//! returns the default font
IGUIFont* CGUISkin::getFont()
{
	return Font;
}


//! sets a default font
void CGUISkin::setFont(IGUIFont* font)
{
	if (Font)
		Font->drop();

	Font = font;

	if (Font)
		Font->grab();
}



//! creates a color skin
IGUISkin* createSkin(EGUI_SKIN_TYPE type)
{
	return new CGUISkin(type);
}


} // end namespace gui
} // end namespace klib