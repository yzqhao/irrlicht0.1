#include "CSceneNodeAnimatorRotation.h"

namespace irr
{
namespace scene
{

//! constructor
CSceneNodeAnimatorRotation::CSceneNodeAnimatorRotation(const core::vector3df& rotation)
: Rotation(rotation)
{
	#ifdef _DEBUG
	setDebugName("CSceneNodeAnimatorRotation");
	#endif
}


//! destructor
CSceneNodeAnimatorRotation::~CSceneNodeAnimatorRotation()
{
}



//! animates a scene node
void CSceneNodeAnimatorRotation::animateNode(ISceneNode* node, u32 timeMs)
{
	core::matrix4 mat;
	mat.setRotationDegrees(Rotation* (timeMs/10.0f));

	core::matrix4& animatedRelativeTransformation = node->getAnimatedRelativeTransformation();

	animatedRelativeTransformation *= mat;
}


} // end namespace scene
} // end namespace irr
