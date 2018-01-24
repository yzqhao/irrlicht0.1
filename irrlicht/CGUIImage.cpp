#include "CGUIImage.h"
#include "IGUISkin.h"
#include "IGUIEnvironment.h"
#include "IVideoDriver.h"

namespace irr
{
namespace gui
{



//! constructor
CGUIImage::CGUIImage(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rectEx<s32> rectangle)
: IGUIImage(environment, parent, id, rectangle), Texture(0)
{
	#ifdef _DEBUG
	setDebugName("CGUIImage");
	#endif
}



//! destructor
CGUIImage::~CGUIImage()
{
	if (Texture)
		Texture->drop();
}



//! sets an image
void CGUIImage::setImage(video::ITexture* image)
{
	if (Texture)
		Texture->drop();

	Texture = image;

	if (Texture)
		Texture->grab();
}



//! draws the element and its children
void CGUIImage::draw()
{
	if (!IsVisible)
		return;

	IGUISkin* skin = Environment->getSkin();
	irr::video::IVideoDriver* driver = Environment->getVideoDriver();

	core::rectEx<s32> rect = AbsoluteRect;

	if (Texture)
		driver->draw2DImage(Texture, AbsoluteRect.UpperLeftCorner, 
							core::rectEx<s32>(core::position2d<s32>(0,0), Texture->getDimension()),
							&AbsoluteClippingRect);
	else
		driver->draw2DRectangle(skin->getColor(EGDC_3D_DARK_SHADOW), AbsoluteRect, &AbsoluteClippingRect);
}



} // end namespace gui
} // end namespace klib