// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_SCENE_NODE_ANIMATOR_ROTATION_H_INCLUDED__
#define __C_SCENE_NODE_ANIMATOR_ROTATION_H_INCLUDED__

#include "ISceneNode.h"

namespace irr
{
namespace scene
{
	class ISceneNode;

	class CSceneNodeAnimatorRotation : public ISceneNodeAnimator
	{
	public:

		//! constructor
		CSceneNodeAnimatorRotation(const core::vector3df& rotation);

		//! destructor
		virtual ~CSceneNodeAnimatorRotation();

		//! animates a scene node
		virtual void animateNode(ISceneNode* node, u32 timeMs);

	private:

		core::vector3df Rotation;

	};


} // end namespace scene
} // end namespace irr

#endif