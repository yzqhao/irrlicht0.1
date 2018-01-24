// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#include "CSurface.h"
#include <memory.h>
#include "Color.h"


namespace irr
{
namespace video  
{

//! constructor
CSurface::CSurface(const core::dimension2d<s32>& size)
: Size(size)
{
	#ifdef _DEBUG
	setDebugName("CSurface");
	#endif

	DataSize = Size.Width*Size.Height;
	DataSizeInBytes = DataSize * sizeof(s16);
	Data = new s16[DataSize];
}



//! destructor
CSurface::~CSurface()
{
	delete [] Data;
}



//! lock function
s16* CSurface::lock()
{
	return Data;
}



//! unlock function
void CSurface::unlock()
{
}



//! returns dimension
const core::dimension2d<s32>& CSurface::getDimension()
{
	return Size;
}


//! gets a pixel
s16 CSurface::getPixel(s32 x, s32 y)
{
	if (x<0 || y<0 || x>Size.Width || y>Size.Height)
		return 0;
	
	return Data[y*Size.Width + x];
}




//! sets a pixel
void CSurface::setPixel(s32 x, s32 y, s16 color)
{
	clipX(x);
	clipY(y);
	Data[y*Size.Width + x] = color;
}


//! sets a pixel very fast and inline
inline void CSurface::setPixelFast(s32 x, s32 y, s16 color)
{
	Data[y*Size.Width + x] = color;
}



//! returns masks for pixels
s16 CSurface::getRedMask()
{
	return 0x1F<<10;
}



//! returns masks for pixels
s16 CSurface::getGreenMask()
{
	return 0x1F<<5;
}



//! returns masks for pixels
s16 CSurface::getBlueMask()
{
	return 0x1F;
}



//! returns alpha mask
s16 CSurface::getAlphaMask()
{
	return (s16)(0x1<<15);
}



//! fills the surface with black or white
void CSurface::fill(s16 color)
{
	s32* p = (s32*)Data;
	s32* bufferEnd = p + (DataSize>>1);
	s32 c = ((color & 0x0000ffff)<<16) | (color & 0x0000ffff);
	while(p != bufferEnd)
	{
		*p = c;
		++p;
	}
}



//! clips a x coordinate into the screen
inline void CSurface::clipX(s32 &x)
{
	if (x<0)
		x = 0;
	else
		if (x > Size.Width)
			x = Size.Width;
}



//! clips a y coordinate into the screen
inline void CSurface::clipY(s32 &y)
{
	if (y<0)
		y = 0;
	else
		if (y > Size.Height)
			y = Size.Height;
}



//! exchanges two ints
inline void CSurface::exchange(s32& a, s32& b)
{
	s32 tmp;
	tmp = a;
	a = b;
	b = tmp;
}


//! draws a rectangle
void CSurface::drawRectangle(const core::rectEx<s32>& rect, Color color)
{
	drawRectangle(	rect.UpperLeftCorner.X, rect.UpperLeftCorner.Y, 
					rect.LowerRightCorner.X, rect.LowerRightCorner.Y, color);
}


//! draws a rectangle
void CSurface::drawRectangle(s32 x, s32 y, s32 x2, s32 y2, Color color)
{
	// clip

	clipX(x); clipX(x2);
	clipY(y); clipY(y2);

	// switch x and x2 if neccecary

	if (x > x2)
		exchange(x,x2);

	if (y > y2)
		exchange(y,y2);

	// draw

	s32 l=y*Size.Width;
	s32 ix;

	if (color.getAlpha()==255)
	{
		// quickly draw without alpha.

		s16 c = color.toA1R5G5B5();

		for (s32 iy=y; iy<y2; ++iy)
		{
			for (ix=x; ix<x2; ++ix)
				Data[l + ix] = c;

			l += Size.Width;
		}
	}
	else
	{
		// draw with alpha

		s32 ia = color.getAlpha();
		s32 a = 255-ia;

		s32 r = getRed(color.toA1R5G5B5()) * ia;
		s32 g = getGreen(color.toA1R5G5B5()) * ia;
		s32 b = getBlue(color.toA1R5G5B5()) * ia;

		s16 *src;

		for (s32 iy=y; iy<y2; ++iy)
		{
			for (ix=x; ix<x2; ++ix)
			{
				src = &Data[l + ix];
				*src =	video::RGB16(	
					(video::getRed(*src)*a + r)>>5,
					(video::getGreen(*src)*a + g)>>5,
					(video::getBlue(*src)*a + b)>>5);
			}

			l += Size.Width;
		}
	}
}



//! clips coordinates into the rect. returns false if the rects is completely outside
//! and does not need to be drawn.
inline bool CSurface::clipRect(const s32 targetWidth, const s32 targetHeight, s32& targetX, s32& targetY, s32& xInSource, s32& yInSource, s32& sourceWidth, s32& soureHeight)
{
	if (targetX<0)
	{
		sourceWidth += targetX;
		if (sourceWidth <= 0)
			return false;

		xInSource -= targetX;
		targetX = 0;
	}
	
	if (targetX+sourceWidth>targetWidth)
	{
		sourceWidth -= (targetX + sourceWidth) - targetWidth;
		if (sourceWidth <= 0)
			return false;
	}

	if (targetY<0)
	{
		soureHeight += targetY;
		if (soureHeight <= 0)
			return false;

		yInSource -= targetY;
		targetY = 0;
	}
	
	if (targetY+soureHeight>targetHeight)
	{
		soureHeight -= (targetY + soureHeight) - targetHeight;
		if (soureHeight <= 0)
			return false;
	}

	return true;
}



//! copies this surface into another
void CSurface::copyTo(ISurface* target, const core::position2d<s32>& pos)
{
	copyTo(target, pos.X, pos.Y);
}



//! copies this surface into another
void CSurface::copyTo(ISurface* target, s32 x, s32 y)
{
	s16* data = target->lock();
	core::dimension2d<s32> size = target->getDimension();

	// clip

	s32 ownWidth = Size.Width;
	s32 ownHeight = Size.Height;
	s32 ownX = 0;
	s32 ownY = 0;

	if (!clipRect(size.Width, size.Height, x, y, ownX, ownY, ownWidth, ownHeight))
		return;

	// copy

	s32 ltarget=y*size.Width + x;
	s32 lown=ownY*Size.Width + ownX;
	size_t dataRowSizeInBytes = ownWidth*sizeof(s16);

	for (s32 iy=0; iy<ownHeight; ++iy)
	{
		memcpy(&data[ltarget], &Data[lown], dataRowSizeInBytes); 

		lown += Size.Width;
		ltarget += size.Width;
	}

	target->unlock();
}



//! copies this surface into another
void CSurface::copyTo(ISurface* target, const core::position2d<s32>& pos, const core::rectEx<s32>& sourceRect, const core::rectEx<s32>* clipRect)
{
	if (sourceRect.UpperLeftCorner.X >= sourceRect.LowerRightCorner.X ||
		sourceRect.UpperLeftCorner.Y >= sourceRect.LowerRightCorner.Y)
		return;

	core::position2d<s32> targetPos = pos;
	core::position2d<s32> sourcePos = sourceRect.UpperLeftCorner;
	core::dimension2d<s32> sourceSize(sourceRect.getWidth(), sourceRect.getHeight());
	const core::dimension2d<s32> targetSurfaceSize = target->getDimension();

	if (clipRect)
	{
		if (targetPos.X < clipRect->UpperLeftCorner.X)
		{
			sourceSize.Width += targetPos.X - clipRect->UpperLeftCorner.X;
			if (sourceSize.Width <= 0)
				return;

			sourcePos.X -= targetPos.X - clipRect->UpperLeftCorner.X;
			targetPos.X = clipRect->UpperLeftCorner.X;
		}
		
		if (targetPos.X + sourceSize.Width > clipRect->LowerRightCorner.X)
		{
			sourceSize.Width -= (targetPos.X + sourceSize.Width) - clipRect->LowerRightCorner.X;
			if (sourceSize.Width <= 0)
				return;
		}

		if (targetPos.Y < clipRect->UpperLeftCorner.Y)
		{
			sourceSize.Height += targetPos.Y - clipRect->UpperLeftCorner.Y;
			if (sourceSize.Height <= 0)
				return;

			sourcePos.Y -= targetPos.Y - clipRect->UpperLeftCorner.Y;
			targetPos.Y = clipRect->UpperLeftCorner.Y;
		}
		
		if (targetPos.Y + sourceSize.Height > clipRect->LowerRightCorner.Y)
		{
			sourceSize.Height -= (targetPos.Y + sourceSize.Height) - clipRect->LowerRightCorner.Y;
			if (sourceSize.Height <= 0)
				return;
		}
	}

	// clip these coordinates

	if (targetPos.X<0)
	{
		sourceSize.Width += targetPos.X;
		if (sourceSize.Width <= 0)
			return;

		sourcePos.X -= targetPos.X;
		targetPos.X = 0;
	}
	
	if (targetPos.X + sourceSize.Width > targetSurfaceSize.Width)
	{
		sourceSize.Width -= (targetPos.X + sourceSize.Width) - targetSurfaceSize.Width;
		if (sourceSize.Width <= 0)
			return;
	}

	if (targetPos.Y<0)
	{
		sourceSize.Height += targetPos.Y;
		if (sourceSize.Height <= 0)
			return;

		sourcePos.Y -= targetPos.Y;
		targetPos.Y = 0;
	}
	
	if (targetPos.Y + sourceSize.Height > targetSurfaceSize.Height)
	{
		sourceSize.Height -= (targetPos.Y + sourceSize.Height) - targetSurfaceSize.Height;
		if (sourceSize.Height <= 0)
			return;
	}

	// draw everything

	s16* targetData = target->lock();
	s32 ltarget = targetPos.Y * targetSurfaceSize.Width + targetPos.X;
	s32 lsource = sourcePos.Y * Size.Width + sourcePos.X;
	size_t dataRowSizeInBytes = sourceSize.Width * sizeof(s16);

	for (s32 iy=0; iy<sourceSize.Height; ++iy)
	{
		memcpy(&targetData[ltarget], &Data[lsource], dataRowSizeInBytes); 
		lsource += Size.Width;
		ltarget += targetSurfaceSize.Width;
	}
}



//! copies this surface into another, using the alpha mask
void CSurface::copyToWithAlpha(ISurface* target, const core::position2d<s32>& pos, const core::rectEx<s32>& sourceRect)
{
	if (sourceRect.UpperLeftCorner.X >= sourceRect.LowerRightCorner.X ||
		sourceRect.UpperLeftCorner.Y >= sourceRect.LowerRightCorner.Y)
		return;

	core::position2d<s32> targetPos = pos;
	core::position2d<s32> sourcePos = sourceRect.UpperLeftCorner;
	core::dimension2d<s32> sourceSize(sourceRect.getWidth(), sourceRect.getHeight());
	const core::dimension2d<s32> targetSurfaceSize = target->getDimension();

	// clip these coordinates

	if (targetPos.X<0)
	{
		sourceSize.Width += targetPos.X;
		if (sourceSize.Width <= 0)
			return;

		sourcePos.X -= targetPos.X;
		targetPos.X = 0;
	}
	
	if (targetPos.X + sourceSize.Width > targetSurfaceSize.Width)
	{
		sourceSize.Width -= (targetPos.X + sourceSize.Width) - targetSurfaceSize.Width;
		if (sourceSize.Width <= 0)
			return;
	}

	if (targetPos.Y<0)
	{
		sourceSize.Height += targetPos.Y;
		if (sourceSize.Height <= 0)
			return;

		sourcePos.Y -= targetPos.Y;
		targetPos.Y = 0;
	}
	
	if (targetPos.Y + sourceSize.Height > targetSurfaceSize.Height)
	{
		sourceSize.Height -= (targetPos.Y + sourceSize.Height) - targetSurfaceSize.Height;
		if (sourceSize.Height <= 0)
			return;
	}

	// draw everything

	s16* targetData = target->lock();
	s32 ltarget = targetPos.Y * targetSurfaceSize.Width + targetPos.X;
	s32 lsource = sourcePos.Y * Size.Width + sourcePos.X;
	size_t dataRowSizeInBytes = sourceSize.Width * sizeof(s16);

	s16 alphaMask = getAlphaMask();
	s16* p;
	s16* end;
	s16* t;

	for (s32 iy=0; iy<sourceSize.Height; ++iy)
	{
		p = &Data[lsource];
		end = p + (dataRowSizeInBytes>>1);
		t = &targetData[ltarget];

		while(p != end)
		{
			if (*p & alphaMask)
				*t = *p;

			++t;
			++p;
		}
		lsource += Size.Width;
		ltarget += targetSurfaceSize.Width;
	}

}




//! copies this surface into another, using the alpha mask, an cliprect and a color to add with
void CSurface::copyToWithAlpha(ISurface* target, const core::position2d<s32>& pos, const core::rectEx<s32>& sourceRect, s16 color, const core::rectEx<s32>* clipRect)
{
	if (sourceRect.UpperLeftCorner.X >= sourceRect.LowerRightCorner.X ||
		sourceRect.UpperLeftCorner.Y >= sourceRect.LowerRightCorner.Y)
		return;

	core::position2d<s32> targetPos = pos;
	core::position2d<s32> sourcePos = sourceRect.UpperLeftCorner;
	core::dimension2d<s32> sourceSize(sourceRect.getWidth(), sourceRect.getHeight());
	const core::dimension2d<s32> targetSurfaceSize = target->getDimension();

	// clip to cliprect if there is one
	if (clipRect)
	{
		if (targetPos.X < clipRect->UpperLeftCorner.X)
		{
			sourceSize.Width += targetPos.X - clipRect->UpperLeftCorner.X;
			if (sourceSize.Width <= 0)
				return;

			sourcePos.X -= targetPos.X - clipRect->UpperLeftCorner.X;
			targetPos.X = clipRect->UpperLeftCorner.X;
		}
		
		if (targetPos.X + sourceSize.Width > clipRect->LowerRightCorner.X)
		{
			sourceSize.Width -= (targetPos.X + sourceSize.Width) - clipRect->LowerRightCorner.X;
			if (sourceSize.Width <= 0)
				return;
		}

		if (targetPos.Y < clipRect->UpperLeftCorner.Y)
		{
			sourceSize.Height += targetPos.Y - clipRect->UpperLeftCorner.Y;
			if (sourceSize.Height <= 0)
				return;

			sourcePos.Y -= targetPos.Y - clipRect->UpperLeftCorner.Y;
			targetPos.Y = clipRect->UpperLeftCorner.Y;
		}
		
		if (targetPos.Y + sourceSize.Height > clipRect->LowerRightCorner.Y)
		{
			sourceSize.Height -= (targetPos.Y + sourceSize.Height) - clipRect->LowerRightCorner.Y;
			if (sourceSize.Height <= 0)
				return;
		}
	}

	// clip these coordinates to screen

	if (targetPos.X<0)
	{
		sourceSize.Width += targetPos.X;
		if (sourceSize.Width <= 0)
			return;

		sourcePos.X -= targetPos.X;
		targetPos.X = 0;
	}
	
	if (targetPos.X + sourceSize.Width > targetSurfaceSize.Width)
	{
		sourceSize.Width -= (targetPos.X + sourceSize.Width) - targetSurfaceSize.Width;
		if (sourceSize.Width <= 0)
			return;
	}

	if (targetPos.Y<0)
	{
		sourceSize.Height += targetPos.Y;
		if (sourceSize.Height <= 0)
			return;

		sourcePos.Y -= targetPos.Y;
		targetPos.Y = 0;
	}
	
	if (targetPos.Y + sourceSize.Height > targetSurfaceSize.Height)
	{
		sourceSize.Height -= (targetPos.Y + sourceSize.Height) - targetSurfaceSize.Height;
		if (sourceSize.Height <= 0)
			return;
	}

	// draw everything

	s16* targetData = target->lock();
	s32 ltarget = targetPos.Y * targetSurfaceSize.Width + targetPos.X;
	s32 lsource = sourcePos.Y * Size.Width + sourcePos.X;
	size_t dataRowSizeInBytes = sourceSize.Width * sizeof(s16);

	s16 alphaMask = getAlphaMask();
	s16* p;
	s16* end;
	s16* t;

	s32 r = getRed(color);
	s32 g = getGreen(color);
	s32 b = getBlue(color);

	for (s32 iy=0; iy<sourceSize.Height; ++iy)
	{
		p = &Data[lsource];
		end = p + (dataRowSizeInBytes>>1);
		t = &targetData[ltarget];

		while(p != end)
		{
			if (*p & alphaMask)
				*t = video::RGB16(video::getRed(*p) * (r) >>2, video::getGreen(*p) * (g) >>2, video::getBlue(*p) * (b) >>2);

			++t;
			++p;
		}
		lsource += Size.Width;
		ltarget += targetSurfaceSize.Width;
	}
}



//! draws a line from to
void CSurface::drawLine(const core::position2d<s32>& from, const core::position2d<s32>& to, s16 color)
{
	s32 x = from.X;
    s32 y = from.Y;

	s32 deltax = to.X - from.X;
	s32 deltay = to.Y - from.Y;
 
	s32 stepx = deltax < 0 ? -1 : 1;
	s32 stepy = deltay < 0 ? -1 : 1;
 
	deltax *= stepx; // wie  deltax = abs(deltax);
	deltay *= stepy; // wie  deltay = abs(deltay);
 
	if (deltax > deltay)
	{
		 s32 d = deltax >> 1;
 
		 while (x != to.X)
		 {
			if (x>=0 && y>=0 && x<Size.Width && y<Size.Height)
				setPixelFast(x, y, color);

			d -= deltay;

			if (d < 0)
			{
				y += stepy;
				d += deltax;
			}

			x += stepx;       
		 }
	}
	else
	{
		 s32 d = deltay >> 1;
 
		 while (y != to.Y)
		 {
			if (x>=0 && y>=0 && x<Size.Width && y<Size.Height)
				setPixelFast(x, y, color);

			d -= deltax;
			if (d < 0)
			{
				x += stepx;
				d += deltay;
			}

			y += stepy;
		 }
	}
}


//! resizes the surface to a new size
void CSurface::resizeTo(const core::dimension2d<s32>& size)
{
	// note: this is very very slow. (i didn't want to write a fast version.
	// but hopefully, nobody wants to scale surfaces every frame.

	if (!size.Width || !size.Height)
		return;

	s32 nDataSize = size.Width * size.Height;
	s32 nDataSizeInBytes = nDataSize * sizeof(s16);
	s16* nData = new s16[nDataSize];

	f32 sourceXStep = (f32)Size.Width / (f32)size.Width;
	f32 sourceYStep = (f32)Size.Height / (f32)size.Height;
	f32 sy;

    for (s32 x=0; x<size.Width; ++x)
	{
		sy = 0.0f;

		for (s32 y=0; y<size.Height; ++y)
		{
			nData[(s32)(y*size.Width + x)] = Data[(s32)(((s32)sy)*Size.Width + x*sourceXStep)];
			sy+=sourceYStep;
		}
	}

	delete [] Data;
    DataSize = nDataSize;
	DataSizeInBytes = nDataSizeInBytes;
	Size = size;
	Data = nData;
}



//! copies this surface into another, scaling it to fit it.
void CSurface::copyToScaling(ISurface* target)
{
	// note: this is very very slow. (i didn't want to write a fast version.
	// but hopefully, nobody wants to scale surfaces every frame.

	core::dimension2d<s32> size = target->getDimension();

	s32 nDataSize = size.Width * size.Height;
	s32 nDataSizeInBytes = nDataSize * sizeof(s16);

	if (!size.Width || !size.Height)
		return;

	s16* nData = target->lock();

	f32 sourceXStep = (f32)Size.Width / (f32)size.Width;
	f32 sourceYStep = (f32)Size.Height / (f32)size.Height;
	f32 sy;

    for (s32 x=0; x<size.Width; ++x)
	{
		sy = 0.0f;

		for (s32 y=0; y<size.Height; ++y)
		{
			nData[(s32)(y*size.Width + x)] = Data[(s32)(((s32)sy)*Size.Width + x*sourceXStep)];
			sy+=sourceYStep;
		}
	}

	target->unlock();
}



//! creates a 16 bit surface
ISurface* createSurface(const core::dimension2d<s32>& size)
{
	return new CSurface(size);
}


} // end namespace video
} // end namespace irr
