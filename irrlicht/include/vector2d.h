// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __IRR_POINT_2D_H_INCLUDED__
#define __IRR_POINT_2D_H_INCLUDED__

#include "irrTypes.h"

namespace irr
{
namespace core
{


template <class T>
inline void rotateBy(f64 degrees, T& x, T&y, T centerx, T centery)
{
	degrees *=GRAD_PI2;
	T cs = (T)cos(degrees);
	T sn = (T)sin(degrees);

	x -= centerx;
	y -= centery;

	T nx = x*cs - y*sn;
	y = x*sn + y*cs;
	x = nx;

	x += centerx;
	y += centery;
}

//! 2d vector template class with lots of operators and methods.
template <class T>
class vector2d
{
public:

	vector2d(): X(0), Y(0) {};
	vector2d(T nx, T ny) : X(nx), Y(ny) {};
	vector2d(const vector2d<T>& other)	:X(other.X), Y(other.Y) {};

	// operators

	vector2d<T>& operator=(const vector2d<T>& other)	{ X = other.X; Y = other.Y; return *this; }

	vector2d<T> operator+(const vector2d<T>& other) const { return vector2d<T>(X + other.X, Y + other.Y);	}
	vector2d<T>& operator+=(const vector2d<T>& other)	{ X+=other.X; Y+=other.Y; return *this; }

	vector2d<T> operator-(const vector2d<T>& other) const { return vector2d<T>(X - other.X, Y - other.Y);	}
	vector2d<T>& operator-=(const vector2d<T>& other)	{ X-=other.X; Y-=other.Y; return *this; }

	vector2d<T> operator*(const vector2d<T>& other) const { return vector2d<T>(X * other.X, Y * other.Y);	}
	vector2d<T>& operator*=(const vector2d<T>& other)	{ X*=other.X; Y*=other.Y; return *this; }
	vector2d<T> operator*(const T v) const { return vector2d<T>(X * v, Y * v);	}
	vector2d<T>& operator*=(const T v) { X*=v; Y*=v; return *this; }

	vector2d<T> operator/(const vector2d<T>& other) const { return vector2d<T>(X / other.X, Y / other.Y);	}
	vector2d<T>& operator/=(const vector2d<T>& other)	{ X/=other.X; Y/=other.Y; return *this; }
	vector2d<T> operator/(const T v) const { return vector2d<T>(X / v, Y / v);	}
	vector2d<T>& operator/=(const T v) { X/=v; Y/=v; return *this; }

	bool operator==(const vector2d<T>& other) const { return other.X==X && other.Y==Y; }
	bool operator!=(const vector2d<T>& other) const { return other.X!=X || other.Y!=Y; }

	// functions

	void set(const T& nx, const T& ny) {X=nx; Y=ny; }
	void set(const vector2d<T>& p) { X=p.X; Y=p.Y;}

	//! Returns the length of the vector
	//! \return Returns the length of the vector.
	f64 getLength() const { return sqrt(X*X + Y*Y); }

	//! Returns the dot product of this vector with an other.
	T dotProduct(const vector2d<T>& other) const
	{
		return X*other.X + Y*other.Y;
	}

	//! Returns distance from an other point. Here, the vector is interpreted as
	//! point in 2 dimensional space.
	f64 getDistanceFrom(const vector2d<T>& other) const
	{
		f64 vx = X - other.X; f64 vy = Y - other.Y;
		return sqrt(vx*vx + vy*vy);
	}

	//! rotates the point around a center by an amount of degrees.
	void rotateBy(f64 degrees, const vector2d<T>& center)
	{
		degrees *=gradPI2;
		T cs = (T)cos(degrees);
		T sn = (T)sin(degrees);

		X -= center.X;
		Y -= center.Y;

		set(X*cs - Y*sn, X*sn + Y*cs);

		X += center.X;
		Y += center.Y;
	}

	//! normalizes the vector.
	void normalize()
	{
		f64 len = getLenght();
		X /= len;
		Y /= len;
	}

	// member variables
	T X, Y;
};

	//! Typedef for f32 2d vector.
	typedef vector2d<f32> vector2df;
	//! Typedef for integer 2d vector.
	typedef vector2d<s32> vector2di;

} // end namespace core
} // end namespace irr

#endif