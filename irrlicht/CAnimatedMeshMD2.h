// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_ANIMATED_MESH_MD2_H_INCLUDED__
#define __C_ANIMATED_MESH_MD2_H_INCLUDED__

#include "IAnimatedMesh.h"
#include "IReadFile.h"
#include "S3DVertex.h"

namespace irr
{
namespace scene
{

	class CAnimatedMeshMD2 : public IAnimatedMesh, IMesh, IMeshBuffer
	{
	public:

		//! constructor
		CAnimatedMeshMD2();

		//! destructor
		virtual ~CAnimatedMeshMD2();

		//! loads an md2 file
		virtual bool loadFile(io::IReadFile* file);

		//! returns the amount of frames in milliseconds. If the amount is 1, it is a static (=non animated) mesh.
		virtual s32 getFrameCount();

		//! returns the animated mesh based on a detail level. 0 is the lowest, 255 the highest detail. Note, that some Meshes will ignore the detail level.
		virtual IMesh* getMesh(s32 frame, s32 detailLevel=255);

		//! returns amount of mesh buffers.
		virtual s32 getMeshBufferCount();

		//! returns pointer to a mesh buffer
		virtual IMeshBuffer* getMeshBuffer(s32 nr);

		 //! returns the material of this meshbuffer
        virtual const video::SMaterial& getMaterial() const;

		 //! returns the material of this meshbuffer
        virtual video::SMaterial& getMaterial();

		//! returns pointer to vertices
		virtual const void* getVertices() const; 

		//! returns pointer to vertices
		virtual void* getVertices(); 

		//! returns which type of vertex data is stored.
		virtual video::E_VERTEX_TYPE getVertexType() const;

		//! returns amount of vertices
		virtual s32 getVertexCount() const;

		//! returns pointer to Indices
		virtual const u16* getIndices() const;

		//! returns amount of indices
		virtual s32 getIndexCount() const;

		//! returns an axis aligned bounding box
		virtual const core::aabbox3d<f32>& getBoundingBox() const;

		//! sets a flag of all contained materials to a new value
		virtual void setMaterialFlag(video::E_MATERIAL_FLAG flag, bool newvalue);

	private:

		//! returns max element
		inline s32 max(s32 a, s32 b);

		//! updates the interpolation buffer
		void updateInterpolationBuffer();

		//! calculates the bounding box
		virtual void calculateBoundingBox();

		//! calculates normals
		void calculateNormals();

		core::array<u16> Indices;
		core::array<video::S3DVertex> *FrameList;
		u32 FrameCount;
		s32 TriangleCount;
		s32 LastFrame;
		video::SMaterial Material;

		core::array<video::S3DVertex> InterpolateBuffer;
		core::aabbox3d<f32> BoundingBox;

		s32 LastUpdatedFrame;
	};

} // end namespace scene
} // end namespace irr

#endif