#include "CMeshSceneNode.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "S3DVertex.h"

namespace irr
{
namespace scene
{



//! constructor
CMeshSceneNode::CMeshSceneNode(IMesh* mesh, ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& position, const core::vector3df& rotation,	const core::vector3df& scale)
: ISceneNode(parent, mgr, id, position, rotation, scale), Mesh(mesh)
{
	if (Mesh)
	{
		// get materials.
		video::SMaterial mat;
		for (s32 i=0; i<Mesh->getMeshBufferCount(); ++i)
		{
			IMeshBuffer* mb = Mesh->getMeshBuffer(i);
			if (mb)
				mat = mb->getMaterial();

			Materials.push_back(mat);
		}

		// grab the mesh

		Mesh->grab();
	}
}



//! destructor
CMeshSceneNode::~CMeshSceneNode()
{
	if (Mesh)
		Mesh->drop();
}



//! frame
void CMeshSceneNode::OnPreRender()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnPreRender();
}



//! renders the node.
void CMeshSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	if (!Mesh || !driver)
		return;

	driver->setTransform(video::TS_WORLD, AbsoluteTransformation);
	Box = Mesh->getBoundingBox();

	for (s32 i=0; i<Mesh->getMeshBufferCount(); ++i)
	{
		scene::IMeshBuffer* mb = Mesh->getMeshBuffer(i);
		
		driver->setMaterial(Materials[i]);

		switch(mb->getVertexType())
		{
		case video::EVT_STANDARD:
			driver->drawIndexedTriangleList((video::S3DVertex*)mb->getVertices(), mb->getVertexCount(), mb->getIndices(), mb->getIndexCount()/ 3);
			break;
		case video::EVT_2TCOORDS:
			driver->drawIndexedTriangleList((video::S3DVertex2TCoords*)mb->getVertices(), mb->getVertexCount(), mb->getIndices(), mb->getIndexCount()/ 3);
			break;				
		}
		
	}			
}


//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CMeshSceneNode::getBoundingBox() const
{
	return Box;
}


//! returns the material based on the zero based index i. To get the amount
//! of materials used by this scene node, use getMaterialCount().
//! This function is needed for inserting the node into the scene hirachy on a
//! optimal position for minimizing renderstate changes, but can also be used
//! to directly modify the material of a scene node.
video::SMaterial& CMeshSceneNode::getMaterial(s32 i)
{
	if (i < 0 || i >= (s32)Materials.size())
		return ISceneNode::getMaterial(i);

	return Materials[i];
}



//! returns amount of materials used by this scene node.
s32 CMeshSceneNode::getMaterialCount()
{
	return Materials.size();
}



} // end namespace scene
} // end namespace irr