#include "CBillboardSceneNode.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "ICameraSceneNode.h"
#include "os.h"

namespace irr
{
namespace scene
{

//! constructor
CBillboardSceneNode::CBillboardSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,	
const core::vector3df& position, const core::dimension2d<f32>& size)
: IBillboardSceneNode(parent, mgr, id, position)
{
	os::Debuginfo::print("Warning: The billboard scene node is not yet 100% implemented!");

	setSize(size);

	indices[0] = 0;
	indices[1] = 2;
	indices[2] = 1;
	indices[3] = 0;
	indices[4] = 3;
	indices[5] = 2;

	vertices[0].TCoords.set(0.0f, 0.0f);
	vertices[0].Color = 0xffffffff;

	vertices[1].TCoords.set(0.0f, 1.0f);
	vertices[1].Color = 0xffffffff;

	vertices[2].TCoords.set(1.0f, 1.0f);
	vertices[2].Color = 0xffffffff;

	vertices[3].TCoords.set(1.0f, 0.0f);
	vertices[3].Color = 0xffffffff;
}



CBillboardSceneNode::~CBillboardSceneNode()
{
}



//! pre render event
void CBillboardSceneNode::OnPreRender()
{
	if (IsVisible)
	{
		SceneManager->registerNodeForRendering(this, scene::SNRT_DEFAULT);
		ISceneNode::OnPreRender();
	}
}


//! render
void CBillboardSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	if (!driver)
		return;

	ICameraSceneNode* camera = SceneManager->getActiveCamera();
	if (!camera)
		return;

	// here, I do not create a standard billboard, but a billboard 
	// which is always looking up. So it can be used for trees or something.
	// TODO: make standard billboard

	// make billboard look to Camera

	core::vector3df pos = getAbsolutePosition();

	core::vector3df nvect = pos - camera->getAbsolutePosition();
	core::vector3df up = camera->getUpVector();

	core::vector3df myVect = nvect.crossProduct(up);
	myVect.normalize();

	myVect *= 0.5f * Size.Width;
	up *= 0.5f * Size.Height;	

	vertices[0].Pos = pos + myVect + up;
	vertices[1].Pos = pos + myVect - up;
	vertices[2].Pos = pos - myVect - up;
	vertices[3].Pos = pos - myVect + up;

	nvect *= -1.0f;

	for (s32 i=0; i<4; ++i)
		vertices[i].Normal = nvect;

	core::matrix4 mat;
    driver->setTransform(video::TS_WORLD, mat);
	driver->setMaterial(Material);

	driver->drawIndexedTriangleList(vertices, 4, indices, 2);
}


//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CBillboardSceneNode::getBoundingBox() const
{
	return BBox;
}


//! sets the size of the billboard
void CBillboardSceneNode::setSize(const core::dimension2d<f32>& size)
{
	Size = size;

	/*vertices[0].Pos.Z = 0;
	vertices[1].Pos.Z = 0;
	vertices[2].Pos.Z = 0;
	vertices[3].Pos.Z = 0;

	vertices[0].Pos.Y = Size.Height * 0.5f;
	vertices[1].Pos.Y = Size.Height * 0.5f;
	vertices[2].Pos.Y = Size.Height * -0.5f;
	vertices[3].Pos.Y = Size.Height * -0.5f;

	vertices[0].Pos.X = Size.Width * 0.5f;
	vertices[1].Pos.X = Size.Width * -0.5f;
	vertices[2].Pos.X = Size.Width * -0.5f;
	vertices[3].Pos.X = Size.Width * 0.5f;*/
}


video::SMaterial& CBillboardSceneNode::getMaterial(s32 i)
{
	return Material;
}


//! returns amount of materials used by this scene node.
s32 CBillboardSceneNode::getMaterialCount()
{
	return 1;
}


//! gets the size of the billboard
const core::dimension2d<f32>& CBillboardSceneNode::getSize()
{
	return Size;
}

} // end namespace scene
} // end namespace irr
