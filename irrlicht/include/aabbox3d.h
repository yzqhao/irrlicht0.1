// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __IRR_AABBOX_3D_H_INCLUDED__
#define __IRR_AABBOX_3D_H_INCLUDED__

#include "irrTypes.h"
#include "plane3dex.h"

namespace irr
{
namespace core
{

//! Axis aligned bounding box in 3d dimensional space.
/** Has some useful methods used with occlusion culling or clipping.
*/
template <class T>
class aabbox3d
{
	public:

		// Constructors

		aabbox3d(): MinEdge(-1,-1,-1), MaxEdge(1,1,1) {};
		aabbox3d(const vector3d<T>& min, const vector3d<T>& max): MinEdge(min), MaxEdge(max) {};
		aabbox3d(T minx, T miny, T minz, T maxx, T maxy, T maxz): MinEdge(minx, miny, minz), MaxEdge(maxx, maxy, maxz), MaxEdge(max) {};

		// operators

		inline bool operator==(const aabbox3d<T>& other) const { return (MinEdge == other.MinEdge && other.MaxEdge == MaxEdge);};
		inline bool operator!=(const aabbox3d<T>& other) const { return !(MinEdge == other.MinEdge && other.MaxEdge == MaxEdge);};

		// functions

		//! Adds a point to the bounding box, causing it to grow bigger, 
		//! if point is outside of the box
		//! \param p: Point to add into the box.
		void addInternalPoint(const vector3d<T>& p)
		{
			addInternalPoint(p.X, p.Y, p.Z);
		}

		//! Adds an other bounding box to the bounding box, causing it to grow bigger,
		//! if the box is outside of the box
		//! \param b: Other bounding box to add into this box.
		void addInternalBox(const aabbox3d<T>& b)
		{
			addInternalPoint(b.MaxEdge);
			addInternalPoint(b.MinEdge);
		}

		//! Resets the bounding box.
		void reset(T x, T y, T z)
		{
			MaxEdge.set(x,y,z);
			MinEdge = MaxEdge;
		}

		//! Resets the bounding box.
		void reset(const vector3d<T>& initValue)
		{
			MaxEdge = initValue;
			MinEdge = initValue;
		}

		//! Adds a point to the bounding box, causing it to grow bigger, 
		//! if point is outside of the box.
		//! \param x: X Coordinate of the point to add to this box.
		//! \param y: Y Coordinate of the point to add to this box.
		//! \param z: Z Coordinate of the point to add to this box.
		void addInternalPoint(T x, T y, T z)
		{
			if (x>MaxEdge.X) MaxEdge.X = x;
			if (y>MaxEdge.Y) MaxEdge.Y = y;
			if (z>MaxEdge.Z) MaxEdge.Z = z;

			if (x<MinEdge.X) MinEdge.X = x;
			if (y<MinEdge.Y) MinEdge.Y = y;
			if (z<MinEdge.Z) MinEdge.Z = z;
		}

		//! Determinates if a point is within this box.
		//! \param p: Point to check.
		//! \return Returns true if the point is withing the box, and false if it is not.
		bool isPointInside(const vector3d<T>& p) const
		{
			return (	p.X >= MinEdge.X && p.X <= MaxEdge.X &&
						p.Y >= MinEdge.Y && p.Y <= MaxEdge.Y &&
						p.Z >= MinEdge.Z && p.Z <= MaxEdge.Z);
		};

		//! Determinates if a point is within this box and its borders.
		//! \param p: Point to check.
		//! \return Returns true if the point is withing the box, and false if it is not.
		bool isPointTotalInside(const vector3d<T>& p) const
		{
			return (	p.X > MinEdge.X && p.X < MaxEdge.X &&
						p.Y > MinEdge.Y && p.Y < MaxEdge.Y &&
						p.Z > MinEdge.Z && p.Z < MaxEdge.Z);
		};

		//! Determinates if the box intersects with an other box.
		//! \param other: Other box to check a intersection with.
		//! \return Returns true if there is a intersection with the other box, 
		//! otherwise false.
		bool intersectsWithBox(const aabbox3d<T>& other) const
		{
			return (MinEdge <= other.MaxEdge && MaxEdge >= other.MinEdge);
		}

		//! Classifies a relation with a plane.
		//! \param plane: Plane to classify relation to.
		//! \return Returns ISREL3D_FRONT if the box is in front of the plane,
		//! ISREL3D_BACK if the box is back of the plane, and
		//! ISREL3D_CLIPPED if is on both sides of the plane.
		EIntersectionRelation3D classifyPlaneRelation(const plane3dex<f32>& plane) const
		{
			vector3d<T> nearPoint(MaxEdge);
			vector3d<T> farPoint(MinEdge);

			if (plane.Normal.X > (T)0)
			{
				nearPoint.X = MinEdge.X;
				farPoint.X = MaxEdge.X;
			}

			if (plane.Normal.Y > (T)0)
			{
				nearPoint.Y = MinEdge.Y;
				farPoint.Y = MaxEdge.Y;
			}

			if (plane.Normal.Z > (T)0)
			{
				nearPoint.Z = MinEdge.Z;
				farPoint.Z = MaxEdge.Z;
			}

			if (plane.Normal.dotProduct(nearPoint) + plane.D > (T)0)
				return ISREL3D_FRONT;

			if (plane.Normal.dotProduct(farPoint) + plane.D > (T)0)
				return ISREL3D_CLIPPED;

			return ISREL3D_BACK;
		}

	
		// member variables
		
		vector3d<T> MinEdge;
		vector3d<T> MaxEdge;
};



} // end namespace core
} // end namespace irr

#endif