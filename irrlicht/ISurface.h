// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __I_SURFACE_H_INCLUDED__
#define __I_SURFACE_H_INCLUDED__

#include "IUnknown.h"
#include "position2d.h"
#include "rect.h"
#include "color.h"

namespace irr
{
namespace video  
{

/*!
	interface for 16 bit A1R5G5B5 surface.
*/
class ISurface : public IUnknown
{
public:

	//! destructor
	virtual ~ISurface() {};

	//! lock function
	virtual s16* lock() = 0;

	//! unlock function
	virtual void unlock() = 0;

	//! returns dimension
	virtual const core::dimension2d<s32>& getDimension() = 0;

	//! sets a pixel
	virtual void setPixel(s32 x, s32 y, s16 color) = 0;

	//! gets a pixel
	virtual s16 getPixel(s32 x, s32 y) = 0;

	//! returns masks for pixels
	virtual s16 getRedMask() = 0;

	//! returns masks for pixels
	virtual s16 getGreenMask() = 0;

	//! returns masks for pixels
	virtual s16 getBlueMask() = 0;

	//! returns alpha mask
	virtual s16 getAlphaMask() = 0;

	//! fillst the surface with black or white
	virtual void fill(s16 color) = 0;

	//! draws a rectangle
	virtual void drawRectangle(const core::rectEx<s32>& rect, Color color) = 0;

	//! draws a rectangle
	virtual void drawRectangle(s32 x, s32 y, s32 x2, s32 y2, Color color) = 0;

	//! copies this surface into another
	virtual void copyTo(ISurface* target, s32 x, s32 y) = 0;

	//! copies this surface into another
	virtual void copyTo(ISurface* target, const core::position2d<s32>& pos) = 0;

	//! copies this surface into another
	virtual void copyTo(ISurface* target, const core::position2d<s32>& pos, const core::rectEx<s32>& sourceRect, const core::rectEx<s32>* clipRect=0) = 0;

	//! copies this surface into another, using the alpha mask
	virtual void copyToWithAlpha(ISurface* target, const core::position2d<s32>& pos, const core::rectEx<s32>& sourceRect) = 0;

	//! copies this surface into another, using the alpha mask, an cliprect and a color to add with
	virtual void copyToWithAlpha(ISurface* target, const core::position2d<s32>& pos, const core::rectEx<s32>& sourceRect, s16 color, const core::rectEx<s32>* clipRect = 0) = 0;

	//! copies this surface into another, scaling it to fit it.
	virtual void copyToScaling(ISurface* target) = 0;

	//! draws a line from to
	virtual void drawLine(const core::position2d<s32>& from, const core::position2d<s32>& to, s16 color) = 0;

	//! resizes the surface to a new size
	virtual void resizeTo(const core::dimension2d<s32>& size) = 0;
};


//! creates a 16 bit surface
ISurface* createSurface(const core::dimension2d<s32>& size);

} // end namespace video
} // end namespace irr

#endif