// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __IRR_LINE_3D_H_INCLUDED__
#define __IRR_LINE_3D_H_INCLUDED__

#include "irrTypes.h"

namespace irr
{
namespace core
{

//! 3D line between two points with intersection methods.
template <class T>
class line3d
{
	public:

		// Constructors

		line3d(): start(0,0,0), end(1,1,1) {};
		line3d(T xa, T ya, T za, T xb, T yb, T zb) : start(xa, ya, za), end(xb, yb, zb) {};
		line3d(const vector3d<T>& start, const vector3d<T>& end) : start(start), end(end) {};
		line3d(const line3d<T>& other) :start(other.start), end(other.end) {};

		// operators

		line3d<T> operator+(const vector3d<T>& point) const { return line3d<T>(start + point, end + point;) };
		line3d<T>& operator+=(const vector3d<T>& point) { start += point; end += point; return *this; };

		line3d<T> operator-(const vector3d<T>& point) const { return line3d<T>(start - point, end - point;) };
		line3d<T>& operator-=(const vector3d<T>& point) { start -= point; end -= point; return *this; };

		bool operator==(const line3d<T>& other) const { return (start==other.start && end==other.end) || (end==other.start && start==other.end);};
		bool operator!=(const line3d<T>& other) const { return !(start==other.start && end==other.end) || (end==other.start && start==other.end);};

		// functions

		void setLine(const T& xa, const T& ya, const T& za, const T& xb, const T& yb, const T& zb){start.set(xa, ya, za); end.set(xb, yb, zb);}
		void setLine(const vector3d<T>& nstart, const vector3d<T>& nend){start.set(nstart); end.set(nend);}
		void setLine(const line3d<T>& line){start.set(line.start); end.set(line.end);}

		//! Returns length of line
		//! \return Returns lenght of line.
		f64 getLenght() { return start.getDistanceFrom(end); };

		// member variables
		
		vector3d<T> start;
		vector3d<T> end;
};


} // end namespace core
} // end namespace irr

#endif
