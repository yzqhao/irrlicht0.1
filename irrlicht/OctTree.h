// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_OCT_TREE_H_INCLUDED__
#define __C_OCT_TREE_H_INCLUDED__

#include "aabbox3d.h"
#include "array.h"
#include <memory.h>

namespace irr
{

//! template octtree. T must be a vertex type which has a member
//! called .Pos, which is a core::vertex3df position.
template <class T>
class OctTree
{
public:

	s32 nodeCount;

	struct SMeshChunk
	{
		core::array<T> Vertices;
		core::array<u16> Indices;
		s32 MaterialId;
	};

	struct SIndexChunk
	{
		core::array<u16> Indices;
		s32 MaterialId;
	};

	struct SIndexData
	{
		u16* Indices;
		s32 CurrentSize;
		s32 MaxSize;
	};

	//! consructor
	OctTree(const core::array<SMeshChunk>& meshes)
	{
		nodeCount = 0;

		IndexDataCount = meshes.size();
		IndexData = new SIndexData[IndexDataCount];

		// construct array of all indices

		core::array<SIndexChunk>* indexChunks = new core::array<SIndexChunk>;
		SIndexChunk ic;

		for (u32 i=0; i<meshes.size(); ++i)
		{
			IndexData[i].CurrentSize = 0;
			IndexData[i].MaxSize = meshes[i].Indices.size();
			IndexData[i].Indices = new u16[IndexData[i].MaxSize];

			ic.MaterialId = meshes[i].MaterialId;
			(*indexChunks).push_back(ic);

			SIndexChunk& tic = (*indexChunks)[i];

			for (u32 t=0; t<meshes[i].Indices.size(); ++t)
				tic.Indices.push_back(meshes[i].Indices[t]);			
		}

		// create tree

		Root = new OctTreeNode(nodeCount, meshes, indexChunks);
	}

	// returns all ids of polygons partially or full enclosed 
	// by this bounding box.
	void calculatePolys(const core::aabbox3d<f32>& box)
	{
		for (s32 i=0; i<IndexDataCount; ++i)
			IndexData[i].CurrentSize = 0;

		Root->getPolys(box, IndexData);
	}

	SIndexData* getIndexData()
	{
		return IndexData;
	}

	s32 getIndexDataCount()
	{
		return IndexDataCount;
	}

	//! destructor
	~OctTree()
	{
		for (s32 i=0; i<IndexDataCount; ++i)
			delete [] IndexData[i].Indices;

		delete [] IndexData;
		delete Root;
	}

private:


	// private inner class
	class OctTreeNode
	{
	public:

		// constructor
		OctTreeNode(s32& nodeCount, const core::array<SMeshChunk>& allmeshdata,
			core::array<SIndexChunk>* indices) : IndexData(0)
		{
			++nodeCount;

			for (u32 i=0; i<8; ++i)
				Children[i] = 0;

			if ((*indices).empty())
			{
				delete indices;
				return;
			}

			bool found = false;

			// init für bounding box finden

			for (s32 i = 0; i<(*indices).size(); ++i)
				if (!(*indices)[i].Indices.empty())
				{
					Box.reset(allmeshdata[i].Vertices[(*indices)[i].Indices[0]].Pos);
					found = true;
					break;
				}

			if (!found)
			{
				delete indices;
				return;
			}

			s32 totalPrimitives = 0;

			// now letz calculate our bounding box
			for (s32 i = 0; i<(*indices).size(); ++i)
			{
				totalPrimitives += (*indices)[i].Indices.size();
				for (u32 j=0; j<(*indices)[i].Indices.size(); ++j)
					Box.addInternalPoint(allmeshdata[i].Vertices[(*indices)[i].Indices[j]].Pos);
			}

			// calculate middle point
			core::vector3df middle = (Box.MinEdge + Box.MaxEdge) / 2;
			core::vector3df diag = Box.MinEdge - middle;

			core::vector3df edges[8];
			edges[0].set(middle.X + diag.X, middle.Y + diag.Y, middle.Z + diag.Z);
			edges[1].set(middle.X + diag.X, middle.Y - diag.Y, middle.Z + diag.Z);
			edges[2].set(middle.X + diag.X, middle.Y + diag.Y, middle.Z - diag.Z);
			edges[3].set(middle.X + diag.X, middle.Y - diag.Y, middle.Z - diag.Z);
			edges[4].set(middle.X - diag.X, middle.Y + diag.Y, middle.Z + diag.Z);
			edges[5].set(middle.X - diag.X, middle.Y - diag.Y, middle.Z + diag.Z);
			edges[6].set(middle.X - diag.X, middle.Y + diag.Y, middle.Z - diag.Z);
			edges[7].set(middle.X - diag.X, middle.Y - diag.Y, middle.Z - diag.Z);

			// calculate all children
			core::aabbox3d<f32> box;

			if (totalPrimitives > 128)
			for (s32 ch=0; ch<8; ++ch)
			{
				box.reset(middle);
				box.addInternalPoint(edges[ch]);

				// create indices for child
				core::array<SIndexChunk>* cindexChunks = new core::array<SIndexChunk>;

				bool added = false;

				for (u32 i=0; i<allmeshdata.size(); ++i)
				{
					SIndexChunk ic;
					ic.MaterialId = allmeshdata[i].MaterialId;
					(*cindexChunks).push_back(ic);

					SIndexChunk& tic = (*cindexChunks)[i];

					for (u32 t=0; t<(*indices)[i].Indices.size(); t+=3)
					{
						if (box.isPointTotalInside(allmeshdata[i].Vertices[(*indices)[i].Indices[t]].Pos) &&
							box.isPointTotalInside(allmeshdata[i].Vertices[(*indices)[i].Indices[t+1]].Pos) &&
							box.isPointTotalInside(allmeshdata[i].Vertices[(*indices)[i].Indices[t+2]].Pos))
						{
							tic.Indices.push_back((*indices)[i].Indices[t]);
							tic.Indices.push_back((*indices)[i].Indices[t+1]);
							tic.Indices.push_back((*indices)[i].Indices[t+2]);

							(*indices)[i].Indices.erase(t, 3);

							t-=3;

							added = true;
						}
					}
				}

				if (added)
					Children[ch] = new OctTreeNode(nodeCount, allmeshdata, cindexChunks);
				else
					delete cindexChunks;

			} // end for all possible children

			IndexData = indices;
		}

		// destructor
		~OctTreeNode()
		{
			delete IndexData;

			for (s32 i=0; i<8; ++i)
				delete Children[i];
		}

		// returns all ids of polygons partially or full enclosed 
		// by this bounding box.
		void getPolys(const core::aabbox3d<f32>& box, SIndexData* idxdata)
		{
			if (Box.intersectsWithBox(box))
			{
				s32 cnt = (*IndexData).size();
				for (s32 i=0; i<cnt; ++i)
				{
					s32 idxcnt = (*IndexData)[i].Indices.size();

					if (idxcnt)
					{
						memcpy(&idxdata[i].Indices[idxdata[i].CurrentSize], 
							&(*IndexData)[i].Indices[0], idxcnt * sizeof(s16));
						idxdata[i].CurrentSize += idxcnt;
					}
				}

				for (s32 i = 0; i<8; ++i)
					if (Children[i])
						Children[i]->getPolys(box, idxdata);
			}
		}

	private:

		core::aabbox3d<f32> Box;
		core::array<SIndexChunk>* IndexData;
		OctTreeNode* Children[8];
	};


	OctTreeNode* Root;
	SIndexData* IndexData;
	s32 IndexDataCount;
	
};

} // end namespace

#endif