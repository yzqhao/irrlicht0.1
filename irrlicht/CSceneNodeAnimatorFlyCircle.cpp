#include "CSceneNodeAnimatorFlyCircle.h"

namespace irr
{
namespace scene
{

//! constructor
CSceneNodeAnimatorFlyCircle::CSceneNodeAnimatorFlyCircle(const core::vector3df& normal, f32 radius, f32 speed)
: Radius(radius), Normal(normal), Speed(speed)
{
	#ifdef _DEBUG
	setDebugName("CSceneNodeAnimatorFlyCircle");
	#endif
}



//! destructor
CSceneNodeAnimatorFlyCircle::~CSceneNodeAnimatorFlyCircle()
{
}



//! animates a scene node
void CSceneNodeAnimatorFlyCircle::animateNode(ISceneNode* node, u32 timeMs)
{
	core::matrix4 mat;

	f32 t = timeMs * Speed;

	core::vector3df pos( (f32)(Radius * sin(t)), 0, (f32)(Radius * cos(t)));
	mat.setTranslation(pos);

	// TODO: den errechneten punkt kippen, und zwar mit der normalen.

	core::matrix4& animatedRelativeTransformation = node->getAnimatedRelativeTransformation();

	animatedRelativeTransformation *= mat;
}


} // end namespace scene
} // end namespace irr