// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __I_ANIMATED_MESH_H_INCLUDED__
#define __I_ANIMATED_MESH_H_INCLUDED__

#include "IUnknown.h"
#include "IMesh.h"

namespace irr
{
namespace scene
{

	//! Interface for an animated mesh.
	class IAnimatedMesh : public IUnknown
	{
	public:

		//! destructor
		virtual ~IAnimatedMesh() {};

		//! Gets the frame count of the animated mesh.
		//! \return Returns the amount of frames. If the amount is 1, it is a static, non animated mesh.
		virtual s32 getFrameCount() = 0;

		//! Returns the IMesh interface for a frame.
		//! \param frame: Frame number as zero based index. The maximum frame number is
		//! getFrameCount() - 1;
		//! \param detailLevel: Level of detail. 0 is the lowest,
		//! 255 the highest level of detail. Most meshes will ignore the detail level.
		//! \return Returns the animated mesh based on a detail level. 
		virtual IMesh* getMesh(s32 frame, s32 detailLevel=255) = 0;
	};

} // end namespace scene
} // end namespace irr

#endif