// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __I_ANIMATED_MESH_SCENE_NODE_H_INCLUDED__
#define __I_ANIMATED_MESH_SCENE_NODE_H_INCLUDED__

#include "ISceneNode.h"

namespace irr
{
namespace scene
{

	//! Scene node capable of displaying an animated mesh.
	class IAnimatedMeshSceneNode : public ISceneNode
	{
	public:

		//! Constructor
		IAnimatedMeshSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f))
			: ISceneNode(parent, mgr, id, position, rotation, scale) {}

		//! Destructor
		virtual ~IAnimatedMeshSceneNode() {};

		//! Sets the current frame number. 
		//! From now on the animation is played from this frame. 
		//! \param frame: Number of the frame to let the animation be started from.
		//! The frame number must be a valid frame number of the IMesh used by this 
		//! scene node. Set IAnimatedMesh::getMesh() for details.
		virtual void setCurrentFrame(s32 frame) = 0;

		//! Sets the frame numbers between the animation is looped.
		//! The default is 0 - MaximalFrameCount of the mesh.
		//! \param begin: Start frame number of the loop.
		//! \param end: End frame number of the loop.
		//! \return Returns true if successful, false if not.
		virtual bool setFrameLoop(s32 begin, s32 end) = 0;

		//! Sets the speed with witch the animation is played.
		//! \param framesPerSecond: Frames per second played.
		virtual void setAnimationSpeed(s32 framesPerSecond) = 0;
	};

} // end namespace scene
} // end namespace irr

#endif