// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __COLOR_H_INCLUDED__
#define __COLOR_H_INCLUDED__

#include "irrTypes.h"

namespace irr
{
namespace video  
{
	//! Creates a 16 bit A1R5G5B5 color 
	inline s16 RGB16(s32 r, s32 g, s32 b)
	{
		return (((r>>3) & 0x1F)<<10) | (((g>>3) & 0x1F)<<5) | ((b>>3) & 0x1F);
	}
	
	//! Converts a 32 bit (X8R8G8B8) color to a 16 A1R5G5B5 color
	inline s16 X8R8G8B8toA1R5G5B5(s32 color)
	{
		return RGB16(color>>16, color>>8, color);
	}

	//! Returns the red component from the color
	inline s32 getRed(s16 color)
	{
		return ((color >> 10)&0x1F);
	}


	//! Returns the green component from the color
	inline s32 getGreen(s16 color)
	{
		return ((color >> 5)&0x1F);
	}


	//! Returns the blue component from the color
	inline s32 getBlue(s16 color)
	{
		return ((color)&0x1F);
	}


	//! Returns A8R8G8B8 Color from A1R5G5B5 color
	inline s32 A1R5G5B5toA8R8G8B8(s16 color)
	{
		return	(((color >> 15)&0x1)<<31) |	(((color >> 10)&0x1F)<<19) |
				(((color >> 5)&0x1F)<<11) |	(color&0x1F)<<3;
	}


	//! Class representing a 32 bit ARGB color.
	/** The color values for red, green, blue
	and alpha are stored in a single s32. So all four values may be between 0 and 255.
	This class is used by most parts of the Irrlicht Engine
	to specify a color. An other way is using the class Colorf, which
	stores the color values in 4 floats.
	*/
	class Color
	{
	public:

		//! Constructor of the Color. Does nothing. The color value is not
		//! initialized to save time.
		inline Color() {}

		//! Constructs the color from 4 values representing the alpha, red, green and 
		//! blue components of the color. Must be values between 0 and 255.
		inline Color (s32 a, s32 r, s32 g, s32 b)
			: color(((a & 0xff)<<24) | ((r & 0xff)<<16) | ((g & 0xff)<<8) | (b & 0xff))	{}

		//! Constructs the color from a 32 bit value. Could be another color.
		inline Color(s32 clr)
			: color(clr) {}

		//! Returns the alpha component of the color. The alpha component
		//! defines how transparent a color should be.
		//! 0 means not transparent, 255 means fully transparent.
		inline s32 getAlpha() const	{	return (color>>24) & 0xff;	}

		//! Returns the red component of the color. 
		//! \return Returns a value between 0 and 255, specifying how red the color is.
		//! 0 means dark, 255 means full red.
		inline s32 getRed() const	{	return (color>>16) & 0xff;	}

		//! Returns the green component of the color. 
		//! \return Returns a value between 0 and 255, specifying how green the color is.
		//! 0 means dark, 255 means full green.
		inline s32 getGreen() const	{	return (color>>8) & 0xff;	}

		//! Returns the blue component of the color. 
		//! \return Returns a value between 0 and 255, specifying how blue the color is.
		//! 0 means dark, 255 means full blue.
		inline s32 getBlue() const	{	return color & 0xff;	}

		//! Sets the alpha comonent of the Color. The alpha component
		//! defines how transparent a color should be.
		//! \param a: Has to be a value between 0 and 255. 
		//! 0 means not transparent, 255 means fully transparent.
		inline void setAlpha(s32 a) { color = ((a & 0xff)<<24) | (((color>>16)& 0xff)<<16) | ((color>>8 & 0xff)<<8) | (color & 0xff); }

		//! Sets the red comonent of the Color. 
		//! \param r: Has to be a value between 0 and 255. 
		//! 0 means dark red (=black), 255 means full red.
		inline void setRed(s32 r) { color = (((color>>24) & 0xff)<<24) | ((r & 0xff)<<16) | ((color>>8 & 0xff)<<8) | (color & 0xff); }

		//! Sets the green comonent of the Color. 
		//! \param g: Has to be a value between 0 and 255. 
		//! 0 means dark green (=black), 255 means full green.
		inline void setGreen(s32 g) { color = (((color>>24) & 0xff)<<24) | (((color>>16)& 0xff)<<16) | ((g & 0xff)<<8) | (color & 0xff); }

		//! Sets the blue comonent of the Color. 
		//! \param b: Has to be a value between 0 and 255. 
		//! 0 means dark blue (=black), 255 means full blue.
		inline void setBlue(s32 b) { color = (((color>>24) & 0xff)<<24) | (((color>>16)& 0xff)<<16) | ((color>>8 & 0xff)<<8) | (b & 0xff); }

		//! Calculates a 16 bit A1R5G5B5 value of this color.
		//! \return Returns the 16 bit A1R5G5B5 value of this color.
		inline s16 toA1R5G5B5() const { return RGB16(color>>16, color>>8, color); };

		//! Converts color to open gl color format.
		//! \return Returns the 32 bit openGL color value.
		inline s32 toOpenGLColor() const 
		{
			return (((color>>24) & 0xff)<<24) |
					(((color)& 0xff)<<16) |
					((color>>8 & 0xff)<<8) |
					((color>>16) & 0xff);
		};

		//! Sets all four components of the color at once.
		//! Constructs the color from 4 values representing the alpha, red, green and 
		//! blue components of the color. Must be values between 0 and 255.
		//! \param a: Alpha component of the color. 
		//! The alpha component defines how transparent a color should be.
		//! Has to be a value between 0 and 255. 
		//! 0 means not transparent, 255 means fully transparent.
		//! \param r: Sets the red comonent of the Color. 
		//! Has to be a value between 0 and 255. 
		//! 0 means dark red (=black), 255 means full red.
		//! \param g: Sets the green comonent of the Color. 
		//! Has to be a value between 0 and 255. 
		//! 0 means dark green (=black), 255 means full green.
		//! \param b: Sets the blue comonent of the Color. 
		//! Has to be a value between 0 and 255. 
		//! 0 means dark blue (=black), 255 means full blue.
		inline void set(s32 a, s32 r, s32 g, s32 b) { color = (((a & 0xff)<<24) | ((r & 0xff)<<16) | ((g & 0xff)<<8) | (b & 0xff)); }

		//! Compares the color to another color.
		//! \return Returns true if the colors are the same, and false if not.
		inline bool operator==(const Color& other) const { return other.color == color; }

		//! Compares the color to another color.
		//! \return Returns true if the colors are different, and false if they are the same.
		inline bool operator!=(const Color& other) const { return other.color != color; }

		//! color in A8R8G8B8 Format
		s32 color; 
	};


	//! Class representing a color with four floats.
	/**	The color values for red, green, blue
	and alpha are each stored in a 32 bit floating point variable.
	So all four values may be between 0.0f and 1.0f.
	This class is used rarely used by the Irrlicht Engine
	to specify a color. An other, faster way is using the class Color, which
	stores the color values in a single 32 bit integer. 
	*/
	class Colorf
	{
	public:

		//! Constructs a color. All values are initialised with 0.0f, resulting
		//! in a black color.
		Colorf() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) {};

		//! Constructs a color from three color values: red, green and blue.
		//! \param r: Red color component. Should be a value between 0.0f meaning
		//! very dark red (=black) and 1.0f, meaning full red.
		//! \param g: Green color component. Should be a value between 0.0f meaning
		//! very dark green (=black) and 1.0f, meaning full green.
		//! \param b: Blue color component. Should be a value between 0.0f meaning
		//! very dark blue (=black) and 1.0f, meaning full blue.
		Colorf(f32 r, f32 g, f32 b) : r(r), g(g), b(b), a(1.0f) {};

		//! Constructs a color from four color values: alpha, red, green and blue.
		//! \param a: Alpha color component of the color.
		//! The alpha component defines how transparent a color should be.
		//! Has to be a value between 0.0f and 1.0f,  
		//! 0.0f means not transparent, 1.0f means fully transparent.
		//! \param r: Red color component. Should be a value between 0.0f meaning
		//! very dark red (=black) and 1.0f, meaning full red.
		//! \param g: Green color component. Should be a value between 0.0f meaning
		//! very dark green (=black) and 1.0f, meaning full green.
		//! \param b: Blue color component. Should be a value between 0.0f meaning
		//! very dark blue (=black) and 1.0f, meaning full blue.
		Colorf(f32 r, f32 g, f32 b, f32 a) : r(r), g(g), b(b), a(a) {};

		//! Constructs a color from 32 bit Color.
		//! \param c: 32 bit color value from which this Colorf class is 
		//! constructed from.
		Colorf(Color c) { const f32 inv = 1.0f / 255.0f; r = c.getRed() * inv; g = c.getGreen() * inv; b = c.getBlue() * inv;	a = c.getAlpha() * inv;	};

		//! red color component
		f32 r;

		//! green color component
		f32 g;

		//! blue component
		f32 b;

		//! alpha color component
		f32 a;

		//! Sets the three color comonents to new values at once.
		//! \param rr: Red color component. Should be a value between 0.0f meaning
		//! very dark red (=black) and 1.0f, meaning full red.
		//! \param gg: Green color component. Should be a value between 0.0f meaning
		//! very dark green (=black) and 1.0f, meaning full green.
		//! \param bb: Blue color component. Should be a value between 0.0f meaning
		//! very dark blue (=black) and 1.0f, meaning full blue.
		void set(f32 rr, f32 gg, f32 bb) {r = rr; g =gg; b = bb; };
	};

} // end namespace video
} // end namespace irr

#endif