// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __IRR_RECT_H_INCLUDED__
#define __IRR_RECT_H_INCLUDED__

#include "irrTypes.h"
#include "dimension2d.h"
#include "position2d.h"

namespace irr
{
namespace core
{

	//! Rectangle template class with position and dimension.
	/**	Mostly used by 2D Interface Classes.
	*/
	template <class T>
	class rect
	{
	public:

		rect()
			: dimension(0,0), position(0,0) {};


		rect(T x, T y, T w, T h)
			: dimension(w,h), position(x,y) {};


		rect(const rect<T>& other)
			: dimension(other.dimension), position(other.position) {};


		rect(const position2d<T>& pos, const dimension2d<T>& dim)
			: position(pos), dimension(dim) {};


        bool operator == (const rect<T>& other) const
		{
			return dimension == other.dimension && position == other.position;
		}


		bool operator != (const rect<T>& other) const
		{
			return dimension != other.dimension || position != other.position;
		}

		const rect<T>& operator = (const rect<T>& other)
		{
			dimension = other.dimension;
			position = other.position;
			return *this;
		}


		//! Returns if a 2d point is within this rectangle.
		//! \param pos: Position to test if it lies within this rectangle.
		//! \return Returns true if the position is within the rectangle, false if not.
		bool isPointInside(const position2d<T>& pos) const
		{
			return position.X <= pos.X && position.Y <= pos.Y &&
				dimension.Width + position.X >= pos.X && dimension.Height + position.Y >= pos.Y;
		}

		dimension2d<T> dimension;
		position2d<T> position;
	};



	//!	Extendet rectangle template.
	/** It has 2 positions instead of position and dimension
		and a faster method for collision detection with other rectangles and points.
	*/
	template <class T>
	class rectEx
	{
	public:

		rectEx()
			: UpperLeftCorner(0,0), LowerRightCorner(0,0) {};


		rectEx(T x, T y, T x2, T y2)
			: UpperLeftCorner(x,y), LowerRightCorner(x2,y2) {};


		rectEx(const position2d<T>& upperLeft, const position2d<T>& lowerRight)
			: UpperLeftCorner(upperLeft), LowerRightCorner(lowerRight) {};


		rectEx(const rect<T>& other)
			: UpperLeftCorner(other.position), 
				LowerRightCorner(	other.position.X + other.dimension.Width,
									other.position.Y + other.dimension.Height) {};


		rectEx(const rectEx<T>& other)
			: UpperLeftCorner(other.UpperLeftCorner), LowerRightCorner(other.LowerRightCorner) {};


		rectEx(const position2d<T>& pos, const dimension2d<T>& size)
			: UpperLeftCorner(pos), LowerRightCorner(pos.X + size.Width, pos.Y + size.Height) {};


		rectEx<T> operator+(const position2d<T>& pos) const
		{
			rectEx<T> ret(*this);
			ret.UpperLeftCorner += pos;
			ret.LowerRightCorner += pos;
			return ret;
		}

		const rectEx<T>& operator+=(const position2d<T>& pos)
		{
			UpperLeftCorner += pos;
			LowerRightCorner += pos;
			return *this;
		}

        bool operator == (const rectEx<T>& other) const
		{
			return UpperLeftCorner == other.UpperLeftCorner && LowerRightCorner == other.LowerRightCorner;
		}


		bool operator != (const rectEx<T>& other) const
		{
			return UpperLeftCorner != other.UpperLeftCorner || LowerRightCorner != other.LowerRightCorner;
		}

		const rectEx<T>& operator = (const rectEx<T>& other)
		{
			UpperLeftCorner = other.UpperLeftCorner;
			LowerRightCorner = other.LowerRightCorner;
			return *this;
		}

		//! Returns if a 2d point is within this rectangle.
		//! \param pos: Position to test if it lies within this rectangle.
		//! \return Returns true if the position is within the rectangle, false if not.
		bool isPointInside(const position2d<T>& pos) const
		{
			return UpperLeftCorner.X <= pos.X && UpperLeftCorner.Y <= pos.Y &&
				LowerRightCorner.X >= pos.X && LowerRightCorner.Y >= pos.Y;
		}

		//! Returns if the rectangle collides with an other rectangle.
		bool isRectCollided(const rectEx<T>& other) const
		{
			return (LowerRightCorner.Y > other.UpperLeftCorner.Y && UpperLeftCorner.Y < other.LowerRightCorner.Y &&
					LowerRightCorner.X > other.UpperLeftCorner.X && UpperLeftCorner.X < other.LowerRightCorner.X);
		}

		//! Clips this rectangle with another one.
		void clipAgainst(const rectEx<T>& other) 
		{
			if (other.LowerRightCorner.X < LowerRightCorner.X)
				LowerRightCorner.X = other.LowerRightCorner.X;
			if (other.LowerRightCorner.Y < LowerRightCorner.Y)
				LowerRightCorner.Y = other.LowerRightCorner.Y;

			if (other.UpperLeftCorner.X > UpperLeftCorner.X)
				UpperLeftCorner.X = other.UpperLeftCorner.X;
			if (other.UpperLeftCorner.Y > UpperLeftCorner.Y)
				UpperLeftCorner.Y = other.UpperLeftCorner.Y;
		}

		//! Returns width of rectangle.
		T getWidth() const
		{
			return LowerRightCorner.X - UpperLeftCorner.X;
		}

		//! Returns height of rectangle.
		T getHeight() const
		{
			return LowerRightCorner.Y - UpperLeftCorner.Y;
		}

		//! If the lower right corner of the rect is smaller then the upper left,
		//! the points are swapped.
		void repair()
		{
			if (LowerRightCorner.X < UpperLeftCorner.X)
			{
				s32 t = LowerRightCorner.X;
				LowerRightCorner.X = UpperLeftCorner.X;
				UpperLeftCorner.X = t;
			}

			if (LowerRightCorner.Y < UpperLeftCorner.Y)
			{
				s32 t = LowerRightCorner.Y;
				LowerRightCorner.Y = UpperLeftCorner.Y;
				UpperLeftCorner.Y = t;
			}
		}

		position2d<T> UpperLeftCorner;
		position2d<T> LowerRightCorner;
	};

} // end namespace core
} // end namespace irr


#endif

