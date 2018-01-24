// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_BSPTREE_H_INCLUDED__
#define __C_BSPTREE_H_INCLUDED__

#include "array.h"
#include "plane3d.h"
#include "SMaterial.h"
#include "aabbox3d.h"
#include "IMeshBuffer.h"
#include "ICameraSceneNode.h"

// The Statistics are used to locate errors and to 
// find a good dimension for the renderbuffer.

namespace irr
{
namespace scene
{

	struct SBspTreeStats
	{
		u32 leafCounts;
		u32 polySplits;
		u32 nodeCount;

		u32 originalTotalVertexCount;
		u32 originalTotalIndexCount;
		u32 totalIndexCount;
		u32 totalVertexCount;

		u32 *indexCount; // counts for every material type
		u32 *vertexCount;
		video::SMaterial *materialMap; // map for all materials

		u32 renderCullings;
		u32 materialCount;
		u32 renderedIndices;

		core::array<core::aabbox3d<f32> > boxes; // bounding boxes of all leafs.
	};

	const f32 BSP_ROUND_ERROR = 0.00001f;

	class CBspTree  
	{
		struct BspPoly
		{
			BspPoly() {};

			BspPoly& operator = (const BspPoly& other)
			{
				vertices = other.vertices;
				indices = other.indices;
				wasSplitter = other.wasSplitter;
				plane = other.plane;
				material = other.material;

				return * this;
			}

			core::array<video::S3DVertex> vertices;
			core::array<u16> indices;

			bool wasSplitter;           // was the poly already a splitter?
			core::plane3d<f32> plane; // plane of this poly

			u32 material;      // number of material index

			bool operator==(const BspPoly& other) const
			{
				if (vertices.size()  != other.vertices.size() ||
					indices.size() != other.indices.size() ||
					plane != other.plane)
				return false;

				// compare indices

				for (u32 i=0; i<indices.size(); ++i)
					if (indices[i] != other.indices[i])
						return false;

				// compare vertices

				for (int i=0; i<vertices.size(); ++i)
					if (vertices[i] != other.vertices[i])
						return false;

				return true;
			}
		};

		struct BspNode
		{
			BspNode()
				: isLeaf(false), front(0), back(0)
			{}

			core::plane3d<f32> plane; // Teilungsebene
			core::aabbox3d<f32> bbox; // Bounding box

			bool isLeaf;                // Node oder Leaf

			core::array<BspPoly> polys; // pointer to bsp polys
			BspNode* front;             // frontliste
			BspNode* back;              // backliste
		};

		enum BoxCullingResult
		{
			BCR_COMPLETE_INSIDE,
			BCR_COMPLETE_OUTSIDE,
			BCP_CLIPPED
		};

	public:

		struct RenderBuffer
		{
			core::array< video::S3DVertex > vertices;
			core::array< u16 > indices;
		};

		//! creates multi material bsptree, each buffer has another material
		CBspTree(const scene::IMeshBuffer* array, u32 bufferCount);

		//! destructor
		virtual ~CBspTree();

		const RenderBuffer* getRenderBuffer(u32 materialNr) const;

		u32 getMaterialCount();

		//! aabbRendering enables fast but inaccurate aabb-box rendering
		void render(const SViewFrustrum* camArea, const core::vector3df& camPos, bool aabbRendering = false); 

		//! returns stats of the bsptree
		const SBspTreeStats* getStats();
		
	 
	private:

		//! creates root of tree
		void createRoot(const scene::IMeshBuffer* array, u32 bufferCount);

		void createBspTree(BspNode& node);

		// liefert die anzahl der gesamten vertices im node zurück
		s32 createBoundingBox(const BspPoly* poly, u32 polyCount, core::aabbox3d<f32>& outBox);

		bool findSplitter(BspPoly* poly, u32 polyCount, core::plane3d<f32>& outSplitter);
		void sortPolys(const BspNode& parent);

		void render(BspNode& node, const SViewFrustrum* camArea, const core::vector3df& camPos);
		void renderBoxCulling(BspNode& node, const SViewFrustrum* camArea, const core::vector3df& camPos);
		void renderLeaf(BspNode& node);

		core::EIntersectionRelation3D classifyPoly(const core::plane3d<f32>& plane, const BspPoly& poly);
		core::plane3d<f32> planeFromPoly(const BspPoly& poly);
		void splitPoly(const BspPoly& poly, const core::plane3d<f32>& plane, BspPoly& frontSplit, BspPoly& backSplit);

		void deleteTree(BspNode *pNode);
		void createStats(const scene::IMeshBuffer* array, u32 MaterialCount);
		void deleteStats();

		void createRenderBuffer();
		void countPolyVertizes(const BspNode& node);
		void countPolyIndizes(const BspNode& node);
		BoxCullingResult cullBoxAgainsView(const core::aabbox3d<f32>& box, const SViewFrustrum* camArea);

		bool polyExistsInNode(BspPoly* poly, BspNode* node, u32 polysToCheck);

		#ifdef _DEBUG
		void debugOutputLeaf(const BspNode& node);
		void debugValidateLeaf(const BspNode& node);
		#endif
		
		BspNode root;
		SBspTreeStats stats;
		RenderBuffer* renderBuffer;
	};


} // end namespace
} // end namespace 


#endif // __BSPTREE_H_INCLUDED__
