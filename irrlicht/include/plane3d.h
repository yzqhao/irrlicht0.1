// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __IRR_PLANE_3D_H_INCLUDED__
#define __IRR_PLANE_3D_H_INCLUDED__

#include "irrmath.h"
#include "vector3d.h"

namespace irr
{
namespace core
{

//! Enumeration for intersection relations of 3d objects
enum EIntersectionRelation3D 
{
	ISREL3D_FRONT = 0,
	ISREL3D_BACK,
	ISREL3D_PLANAR,
	ISREL3D_SPANNING,
	ISREL3D_CLIPPED
};


//! Template plane class.
template <class T>
class plane3d
{
	public:

		// Constructors

		plane3d(): MPoint(0,0,0), Normal(0,1,0) {};
		plane3d(const vector3d<T>& MPoint, const vector3d<T>& Normal) : MPoint(MPoint), Normal(Normal) {};
		plane3d(T px, T py, T pz, T nx, T ny, T nz) : MPoint(px, py, pz), Normal(nx, ny, nz) {};
		plane3d(const plane3d<T>& other) :MPoint(other.MPoint), Normal(other.Normal) {};
		plane3d(const vector3d<T>& point1, const vector3d<T>& point2, const vector3d<T>& point3)	{ setPlane(point1, point2, point3);	};

		// operators

		inline bool operator==(const plane3d<T>& other) const { return (MPoint==other.MPoint && Normal==other.Normal);};
		inline bool operator!=(const plane3d<T>& other) const { return !(MPoint==other.MPoint && Normal==other.Normal);};

		// functions

		void setPlane(const vector3d<T>& point, const vector3d<T>& nvector)
		{
			Normal = nvector;
			Normal.normalize();
			MPoint = point;
		}

		//! creates the plane from 3 memberpoints
		void setPlane(const vector3d<T>& point1, const vector3d<T>& point2, const vector3d<T>& point3)
		{
			Normal = (point2 - point1).crossProduct(point3 - point1);
			Normal.normalize();

			MPoint = point1;
		}

		//! Returns an intersection with a 3d line.
		//! If you are doing intersections very often with the
		//! same planes, use plane3dex, it is faster.
		//! \param lineVect: Vector of the line to intersect with.
		//! \param linePoint: Point of the line to intersect with.
		//! \param outIntersection: Place to store the intersection point, if there is one.
		//! \return Returns true if there was an intersection, false if there was not.
		bool getIntersectionWithLine(const vector3d<T>& linePoint, const vector3d<T>& lineVect, vector3d<T>& outIntersection) const
		{
			T t2 = Normal.dotProduct(lineVect);

			if (t2 == 0)
				return false;

			T t = - (Normal.dotProduct(linePoint) - MPoint.dotProduct(Normal)) / t2;
			outIntersection = linePoint + (lineVect * t);
			return true;
		}

		//! Returns an intersection with a 3d line, limited between two 3d points.
		//! \param linePoint1: Point 1 of the line.
		//! \param linePoint2: Point 2 of the line.
		//! \param outIntersection: Place to store the intersection point, if there is one.
		//! \return Returns true if there was an intersection, false if there was not.
		bool getIntersectionWithLimitedLine(const vector3d<T>& linePoint1, const vector3d<T>& linePoint2, vector3d<T>& outIntersection) const
		{
			return (	getIntersectionWithLine(linePoint1, linePoint2 - linePoint1, outIntersection) &&
						outIntersection.isBetweenPoints(linePoint1, linePoint2));
		}

		//! Classifies the relation of a point to this plane.
		//! \param point: Point to classify its relation.
		//! \return Returns ISREL3D_FRONT if the point is in front of the plane,
		//! ISREL3D_BACK if the point is behind of the plane, and
		//! ISREL3D_PLANAR if the point is within the plane.
		EIntersectionRelation3D classifyPointRelation(const vector3d<T>& point) const
		{
			f32 pktprd = (MPoint - point).dotProduct(Normal);
			
			if (pktprd < -ROUNDING_ERROR)
				return ISREL3D_FRONT;

			if (pktprd > ROUNDING_ERROR)
				return ISREL3D_BACK;

			return ISREL3D_PLANAR;
		}

		// member variables
		
		vector3d<T> MPoint;	// member point of plane
		vector3d<T> Normal;		// normal vector
};



} // end namespace core
} // end namespace irr

#endif
