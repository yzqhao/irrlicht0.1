#include "CAnimatedMeshSceneNode.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "S3DVertex.h"
#include "os.h"

namespace irr
{
namespace scene
{



//! constructor
CAnimatedMeshSceneNode::CAnimatedMeshSceneNode(IAnimatedMesh* mesh, ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& position, const core::vector3df& rotation,	const core::vector3df& scale)
: IAnimatedMeshSceneNode(parent, mgr, id, position, rotation, scale), Mesh(mesh), 
	BeginFrameTime(0), StartFrame(0), EndFrame(0), FramesPerSecond(100)
{
	BeginFrameTime = os::Timer::getTime();

	if (Mesh)
	{
		// get materials.

		IMesh* m = Mesh->getMesh(0,0);
		if (m)
		{
			video::SMaterial mat;
			for (s32 i=0; i<m->getMeshBufferCount(); ++i)
			{
				IMeshBuffer* mb = m->getMeshBuffer(i);
				if (mb)
					mat = mb->getMaterial();

				Materials.push_back(mat);
			}
		}

		// get start and begin time

		StartFrame = 0;
		EndFrame = Mesh->getFrameCount();

		// grab the mesh

		Mesh->grab();
	}
}



//! destructor
CAnimatedMeshSceneNode::~CAnimatedMeshSceneNode()
{
	if (Mesh)
		Mesh->drop();
}



//! sets the current frame. from now on the animation is played from this frame.
void CAnimatedMeshSceneNode::setCurrentFrame(s32 frame)
{
}



//! frame
void CAnimatedMeshSceneNode::OnPreRender()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnPreRender();
}



//! renders the node.
void CAnimatedMeshSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	if (!Mesh || !driver)
		return;

	driver->setTransform(video::TS_WORLD, AbsoluteTransformation);

	s32 frame = StartFrame + 
		( (s32)((os::Timer::getTime() - BeginFrameTime) * (FramesPerSecond/1000.0f)) 
		% (EndFrame - StartFrame));

	scene::IMesh* m = Mesh->getMesh(frame);

	if (m)
	{
		Box = m->getBoundingBox();

		for (s32 i=0; i<m->getMeshBufferCount(); ++i)
		{
			scene::IMeshBuffer* mb = m->getMeshBuffer(i);
			
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
	#ifdef _DEBUG
	else
		os::Warning::print("Animated Mesh returned no mesh to render.", Mesh->getDebugName());
	#endif
}



//! sets the frames between the animation is looped.
//! the default is 0 - MaximalFrameCount of the mesh.
bool CAnimatedMeshSceneNode::setFrameLoop(s32 begin, s32 end)
{
	if (!Mesh)
		return false;

	s32 frameCount = Mesh->getFrameCount();

	if (!(begin < end && begin < frameCount && end < frameCount &&
		 end - begin > 0))
		return false;

	StartFrame = begin;
	EndFrame = end;
	BeginFrameTime = os::Timer::getTime();

	return true;
}



//! sets the speed with witch the animation is played
void CAnimatedMeshSceneNode::setAnimationSpeed(s32 framesPerSecond)
{
	FramesPerSecond = framesPerSecond;
}




//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CAnimatedMeshSceneNode::getBoundingBox() const
{
	return Box;
}


//! returns the material based on the zero based index i. To get the amount
//! of materials used by this scene node, use getMaterialCount().
//! This function is needed for inserting the node into the scene hirachy on a
//! optimal position for minimizing renderstate changes, but can also be used
//! to directly modify the material of a scene node.
video::SMaterial& CAnimatedMeshSceneNode::getMaterial(s32 i)
{
	if (i < 0 || i >= (s32)Materials.size())
		return ISceneNode::getMaterial(i);

	return Materials[i];
}



//! returns amount of materials used by this scene node.
s32 CAnimatedMeshSceneNode::getMaterialCount()
{
	return Materials.size();
}



} // end namespace scene
} // end namespace irr