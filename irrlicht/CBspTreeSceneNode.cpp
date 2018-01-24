#include "CBspTreeSceneNode.h"
#include "ISceneManager.h"
#include "CBspTree.h"
#include "os.h"

namespace irr
{
namespace scene
{

//! constructor
CBspTreeSceneNode::CBspTreeSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id)
: IBspTreeSceneNode(parent, mgr, id), Tree(0)
{
#ifdef _DEBUG
	setDebugName("CBspTreeSceneNode");
#endif

	os::Debuginfo::print("Warning: The BspTreeNode is not bug free yet. Use the OctTree instead.");
}

//! destructor
CBspTreeSceneNode::~CBspTreeSceneNode()
{
	delete Tree;
}


void CBspTreeSceneNode::OnPreRender()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnPreRender();
}



//! renders the node.
void CBspTreeSceneNode::render()
{
	
}



//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CBspTreeSceneNode::getBoundingBox() const
{
	return Box;
}



//! creates the tree
bool CBspTreeSceneNode::createTree(IMesh* mesh)
{
	if (Tree)
		delete Tree;

	Tree = 0;

	s32 count = mesh->getMeshBufferCount();

	if (!count)
		return false;

	Tree = new CBspTree(mesh->getMeshBuffer(0), count);

	return true;
}


} // end namespace scene
} // end namespace irr
