// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_SURFACE_H_INCLUDED__
#define __C_SURFACE_H_INCLUDED__

#include "ISurface.h"

namespace irr
{
namespace video  
{

/*!
	16 bit surface
	A1R5G5B5	
*/
class CSurface : public ISurface
{
public:

	//! constructor
	CSurface(const core::dimension2d<s32>& size);

	//! destructor
	virtual ~CSurface();

	//! lock function
	virtual s16* lock();

	//! unlock function
	virtual void unlock();

	//! returns dimension
	virtual const core::dimension2d<s32>& getDimension();

	//! sets a pixel
	virtual void setPixel(s32 x, s32 y, s16 color);

	//! gets a pixel
	virtual s16 getPixel(s32 x, s32 y);

	//! returns masks for pixels
	virtual s16 getRedMask();

	//! returns masks for pixels
	virtual s16 getGreenMask();

	//! returns masks for pixels
	virtual s16 getBlueMask();

	//! returns alpha mask
	virtual s16 getAlphaMask();

	//! fills the surface with black or white
	virtual void fill(s16 color);

	//! draws a rectangle
	virtual void drawRectangle(const core::rectEx<s32>& rect, Color color);

	//! draws a rectangle
	void drawRectangle(s32 x, s32 y, s32 x2, s32 y2, Color color);

	//! copies this surface into another
	virtual void copyTo(ISurface* target, s32 x, s32 y);

	//! copies this surface into another
	virtual void copyTo(ISurface* target, const core::position2d<s32>& pos);

	//! copies this surface into another
	virtual void copyTo(ISurface* target, const core::position2d<s32>& pos, const core::rectEx<s32>& sourceRect, const core::rectEx<s32>* clipRect=0);

	//! copies this surface into another, using the alpha mask
	virtual void copyToWithAlpha(ISurface* target, const core::position2d<s32>& pos, const core::rectEx<s32>& sourceRect);

	//! copies this surface into another, using the alpha mask, an cliprect and a color to add with
	virtual void copyToWithAlpha(ISurface* target, const core::position2d<s32>& pos, const core::rectEx<s32>& sourceRect, s16 color, const core::rectEx<s32>* clipRect = 0);

	//! copies this surface into another, scaling it to fit it.
	virtual void copyToScaling(ISurface* target);

	//! draws a line from to
	virtual void drawLine(const core::position2d<s32>& from, const core::position2d<s32>& to, s16 color);
	
	//! resizes the surface to a new size
	virtual void resizeTo(const core::dimension2d<s32>& size);

private:

	//! clips a x coordinate into the screen
	inline void clipX(s32 &x);

	//! clips a y coordinate into the screen
	inline void clipY(s32 &y);

	//! exchanges two ints
	inline void exchange(s32& a, s32& b);

	//! clips coordinates into the rect. returns false if the rects is completely outside
	//! and does not need to be drawn.
	inline bool clipRect(const s32 targetWidth, const s32 targetHeight, s32& targetX, s32& targetY, s32& xInSource, s32& yInSource, s32& sourceWidth, s32& soureHeight);

	//! sets a pixel very fast and inline
	inline void setPixelFast(s32 x, s32 y, s16 color);

	s16* Data;
	core::dimension2d<s32> Size;
	s32 DataSize;
	s32 DataSizeInBytes;
};

} // end namespace video
} // end namespace irr


#endif