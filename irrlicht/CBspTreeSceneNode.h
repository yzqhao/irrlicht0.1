// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_BSP_TREE_SCENE_NODE_H_INCLUDED__
#define __C_BSP_TREE_SCENE_NODE_H_INCLUDED__

#include "IBspTreeSceneNode.h"

namespace irr
{
namespace scene
{
	class CBspTree;

	//! implementation of the IBspTreeSceneNode
	class CBspTreeSceneNode : public IBspTreeSceneNode
	{
	public:

		//! constructor
		CBspTreeSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id);

		//! destructor
		virtual ~CBspTreeSceneNode();

		virtual void OnPreRender();

		//! renders the node.
		virtual void render();

		//! returns the axis aligned bounding box of this node
		virtual const core::aabbox3d<f32>& getBoundingBox() const;

		//! creates the tree
		bool createTree(IMesh* mesh);

	private:

		CBspTree* Tree;
		core::aabbox3d<f32> Box;
	};

} // end namespace scene
} // end namespace irr

#endif