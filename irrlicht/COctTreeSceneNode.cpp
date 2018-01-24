#include "COctTreeSceneNode.h"
#include "OctTree.h"
#include "ISceneManager.h"
#include "IVideoDriver.h"
#include "ICameraSceneNode.h"

#include "os.h"
#include <stdio.h>

namespace irr
{
namespace scene
{


//! constructor
COctTreeSceneNode::COctTreeSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id)
: ISceneNode(parent, mgr, id), StdOctTree(0), LightMapOctTree(0)
{
#ifdef _DEBUG
	setDebugName("COctTreeSceneNode");
#endif

	vertexType = (video::E_VERTEX_TYPE)-1;
}



//! destructor
COctTreeSceneNode::~COctTreeSceneNode()
{
	delete StdOctTree;
	delete LightMapOctTree;
}



void COctTreeSceneNode::OnPreRender()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnPreRender();
}



//! renders the node.
void COctTreeSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	if (vertexType == -1 || !driver)
		return;

	ICameraSceneNode* camera = SceneManager->getActiveCamera();
	if (!camera)
		return;

	driver->setTransform(video::TS_WORLD, AbsoluteTransformation);

	// transform the frustrum to the current absolute transformation

	core::matrix4 invTrans(AbsoluteTransformation);
	invTrans.makeInverse();

	core::aabbox3d<float> box;

	core::vector3df p;

	invTrans.transformVect(camera->getAbsolutePosition(), p);
	box.reset(p);

	invTrans.transformVect(camera->getViewFrustrum()->rightFarDown, p);
	box.addInternalPoint(p);
	invTrans.transformVect(camera->getViewFrustrum()->leftFarDown, p);
	box.addInternalPoint(p);
	invTrans.transformVect(camera->getViewFrustrum()->rightFarUp, p);
	box.addInternalPoint(p);
	invTrans.transformVect(camera->getViewFrustrum()->leftFarUp, p);
	box.addInternalPoint(p);

	switch(vertexType)
	{
	case video::EVT_STANDARD:
		{
			StdOctTree->calculatePolys(box);
			OctTree<video::S3DVertex>::SIndexData* d =  StdOctTree->getIndexData();

			for (u32 i=0; i<Materials.size(); ++i)
			{
				driver->setMaterial(Materials[i]);
				driver->drawIndexedTriangleList(
					&StdMeshes[i].Vertices[0], StdMeshes[i].Vertices.size(),
					d[i].Indices, d[i].CurrentSize/ 3);
			}
			break;

		}
	case video::EVT_2TCOORDS:
		{
			LightMapOctTree->calculatePolys(box);
			OctTree<video::S3DVertex2TCoords>::SIndexData* d =  LightMapOctTree->getIndexData();

			for (u32 i=0; i<Materials.size(); ++i)
			{
				driver->setMaterial(Materials[i]);
				driver->drawIndexedTriangleList(
					&LightMapMeshes[i].Vertices[0], LightMapMeshes[i].Vertices.size(),
					d[i].Indices, d[i].CurrentSize/ 3);
			}
		}
		break;
	};
}


//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& COctTreeSceneNode::getBoundingBox() const
{
	return Box;
}


//! creates the tree
bool COctTreeSceneNode::createTree(IMesh* mesh)
{
#ifdef _DEBUG
	u32 beginTime = os::Timer::getTime();
	os::Debuginfo::print("generatring OctTree...");
#endif

	s32 nodeCount;

	if (mesh->getMeshBufferCount())
	{
		vertexType = mesh->getMeshBuffer(0)->getVertexType();

		switch(vertexType)
		{
		case video::EVT_STANDARD:
			{
				for (s32 i=0; i<mesh->getMeshBufferCount(); ++i)
				{
					IMeshBuffer* b = mesh->getMeshBuffer(i);
					Materials.push_back(b->getMaterial());

					OctTree<video::S3DVertex>::SMeshChunk chunk;
					chunk.MaterialId = i;
					StdMeshes.push_back(chunk);
					OctTree<video::S3DVertex>::SMeshChunk &nchunk = StdMeshes[StdMeshes.size()-1];

					for (s32 v=0; v<b->getVertexCount(); ++v)
						nchunk.Vertices.push_back(((video::S3DVertex*)b->getVertices())[v]);

					for (s32 v=0; v<b->getIndexCount(); ++v)
						nchunk.Indices.push_back(b->getIndices()[v]);
				}

				StdOctTree = new OctTree<video::S3DVertex>(StdMeshes);
				nodeCount = StdOctTree->nodeCount;
			}
			break;
		case video::EVT_2TCOORDS:
			{
				for (s32 i=0; i<mesh->getMeshBufferCount(); ++i)
				{
					IMeshBuffer* b = mesh->getMeshBuffer(i);
					Materials.push_back(b->getMaterial());

					OctTree<video::S3DVertex2TCoords>::SMeshChunk chunk;
					chunk.MaterialId = i;
					LightMapMeshes.push_back(chunk);
					OctTree<video::S3DVertex2TCoords>::SMeshChunk& nchunk = 
						LightMapMeshes[LightMapMeshes.size()-1];

					for (s32 v=0; v<b->getVertexCount(); ++v)
						nchunk.Vertices.push_back(((video::S3DVertex2TCoords*)b->getVertices())[v]);

					for (int v=0; v<b->getIndexCount(); ++v)
						nchunk.Indices.push_back(b->getIndices()[v]);
				}

				LightMapOctTree = new OctTree<video::S3DVertex2TCoords>(LightMapMeshes);
				nodeCount = LightMapOctTree->nodeCount;
			}
			break;
		}
	}

#ifdef _DEBUG
	u32 endTime = os::Timer::getTime();
	c8 tmp[255];
	sprintf(tmp, "needed %dms to create OctTree. (created %d nodes)", 
		endTime - beginTime, nodeCount);
	os::Debuginfo::print(tmp);
#endif

	return true;
}


//! returns the material based on the zero based index i. To get the amount
//! of materials used by this scene node, use getMaterialCount().
//! This function is needed for inserting the node into the scene hirachy on a
//! optimal position for minimizing renderstate changes, but can also be used
//! to directly modify the material of a scene node.
video::SMaterial& COctTreeSceneNode::getMaterial(s32 i)
{
	if (i < 0 || i >= (s32)Materials.size())
		return ISceneNode::getMaterial(i);

	return Materials[i];
}

//! returns amount of materials used by this scene node.
s32 COctTreeSceneNode::getMaterialCount()
{
	return Materials.size();
}


} // end namespace scene
} // end namespace irr
