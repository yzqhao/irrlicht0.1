#include "CSceneManager.h"
#include "IVideoDriver.h"
#include "IFileSystem.h"
#include "IAnimatedMesh.h"
#include <string.h>
#include "os.h"

#include "CAnimatedMeshMD2.h"
#include "CStaticMeshOBJ.h"
#include "CAnimatedMeshMS3D.h"
#include "CQ3LevelMesh.h"

#include "CTestSceneNode.h"
#include "CAnimatedMeshSceneNode.h"
#include "CBspTreeSceneNode.h"
#include "COctTreeSceneNode.h"
#include "CCameraSceneNode.h"
#include "CCameraMayaSceneNode.h"
#include "CLightSceneNode.h"
#include "CBillboardSceneNode.h"
#include "CMeshSceneNode.h"

#include "CSceneNodeAnimatorRotation.H"
#include "CSceneNodeAnimatorFlyCircle.H"

namespace irr
{
namespace scene
{


//! constructor
CSceneManager::CSceneManager(video::IVideoDriver* driver, io::IFileSystem* fs)
: ISceneNode(0, 0), Driver(driver), FileSystem(fs), ActiveCamera(0)
{
	#ifdef _DEBUG
	ISceneManager::setDebugName("CSceneManager ISceneManager");
	ISceneNode::setDebugName("CSceneManager ISceneNode");
	#endif

	if (Driver)
		Driver->grab();

	if (FileSystem)
		FileSystem->grab();
}

//! destructor
CSceneManager::~CSceneManager()
{
	if (Driver)
		Driver->drop();

	if (FileSystem)
		FileSystem->drop();

	for (u32 i=0; i<Meshes.size(); ++i)
		Meshes[i].Mesh->drop();
}


//! adds a mesh to the list
void CSceneManager::addMesh(const c8* filename, IAnimatedMesh* mesh)
{
	mesh->grab();

	MeshEntry e;
	e.Mesh = mesh;
	e.Name = filename;
	e.Name.make_lower();

	Meshes.push_back(e);
}


//! returns an already loaded mesh
IAnimatedMesh* CSceneManager::findMesh(const c8* lowerMadeFilename)
{
	MeshEntry e;
	e.Name = lowerMadeFilename;
	s32 id = Meshes.binary_search(e);
	return (id != -1) ? Meshes[id].Mesh : 0;
}


//! gets an animateable mesh. loads it if needed. returned pointer must not be dropped.
IAnimatedMesh* CSceneManager::getMesh(const c8* filename)
{
	IAnimatedMesh* msh = 0;

	core::stringc name = filename;
	name.make_lower();

	msh = findMesh(name.c_str());
	if (msh)
		return msh;

	io::IReadFile* file = FileSystem->createAndOpenFile(filename);
	if (!file)
	{
		#ifdef _DEBUG
		os::Warning::print("Could not load mesh, because file could not be opened.", filename);
		#endif
		return 0;
	}

	bool success = false;

	if (strstr(filename, ".md2"))
	{
		// load md2 version
		file->seek(0);
	
		msh = new CAnimatedMeshMD2();
		success = ((CAnimatedMeshMD2*)msh)->loadFile(file);
		if (success)
			addMesh(filename, msh);

		msh->drop();
	}
	
	if (!success && strstr(filename, ".obj"))
	{
		// load obj
		file->seek(0);

		msh = new CStaticMeshOBJ();
		success = ((CStaticMeshOBJ*)msh)->loadFile(file);
		if (success)
			addMesh(filename, msh);

		msh->drop();
	}

	if (!success && strstr(filename, ".ms3d"))
	{
		file->seek(0);

		msh = new CAnimatedMeshMS3D();
		success = ((CAnimatedMeshMS3D*)msh)->loadFile(file);
		if (success)
			addMesh(filename, msh);

		msh->drop();
	}

	if (!success && strstr(filename, ".bsp"))
	{
		file->seek(0);

		msh = new CQ3LevelMesh(FileSystem, Driver);
		success = ((CQ3LevelMesh*)msh)->loadFile(file);
		if (success)
			addMesh(filename, msh);

		msh->drop();
	}


	file->drop();

	#ifdef _DEBUG
	if (!msh)
		os::Debuginfo::print("Could not find mesh", filename);
	else
		os::Debuginfo::print("Loaded mesh", filename);
	#endif

	return success ? msh : 0;
}



//! returns the video driver
video::IVideoDriver* CSceneManager::getVideoDriver()
{
	return Driver;
}


//! adds a test scene node for test purposes to the scene. It is a simple cube of (1,1,1) size. 
//! the returned pointer must not be dropped.
ISceneNode* CSceneManager::addTestSceneNode(f32 size, ISceneNode* parent, s32 id,
	const core::vector3df& position, const core::vector3df& rotation, const core::vector3df& scale)
{
	if (!parent)
		parent = this;

	ISceneNode* node = new CTestSceneNode(size, parent, this, id, position, rotation, scale);
	node->drop();

	return node;	
}


//! adds a scene node for rendering a static mesh
//! the returned pointer must not be dropped.
ISceneNode* CSceneManager::addMeshSceneNode(IMesh* mesh, ISceneNode* parent, s32 id,
	const core::vector3df& position, const core::vector3df& rotation,
	const core::vector3df& scale)
{
	if (!mesh)
		return 0;

	if (!parent)
		parent = this;

	ISceneNode* node = new CMeshSceneNode(mesh, parent, this, id, position, rotation, scale);
	node->drop();

	return node;
}

	
//! adds a scene node for rendering an animated mesh model
IAnimatedMeshSceneNode* CSceneManager::addAnimatedMeshSceneNode(IAnimatedMesh* mesh, ISceneNode* parent, s32 id,
	const core::vector3df& position, const core::vector3df& rotation , const core::vector3df& scale)
{
	if (!mesh)
		return 0;

	if (!parent)
		parent = this;

	IAnimatedMeshSceneNode* node = new CAnimatedMeshSceneNode(mesh, parent, this, id, position, rotation, scale);
	node->drop();

	return node;	
}



//! Adds a scene node for rendering using a binary space partition tree.
IBspTreeSceneNode* CSceneManager::addBspTreeSceneNode(IMesh* mesh, ISceneNode* parent, s32 id)
{
	if (!mesh)
		return 0;

	if (!parent)
		parent = this;

	CBspTreeSceneNode* node = new CBspTreeSceneNode(parent, this, id);
	
	node->createTree(mesh);

	node->drop();

    return node;
}



//! Adss a scene node for rendering using a octtree. This a good method for rendering 
//! scenes with lots of geometry. The Octree is built on the fly from the mesh, much
//! faster then a bsp tree.
//! \param mesh: The mesh containing all geometry from which the octtree will be build.
//! \param parent: Parent node of the octtree node.
//! \param id: id of the node.
//! \return Returns the pointer to the octtree if successful, otherwise 0. 
ISceneNode* CSceneManager::addOctTreeSceneNode(IMesh* mesh, ISceneNode* parent, s32 id)
{
	if (!mesh)
		return 0;

	if (!parent)
		parent = this;

	COctTreeSceneNode* node = new COctTreeSceneNode(parent, this, id);
	node->createTree(mesh);
	node->drop();

    return node;
}



//! Adds a camera scene node to the tree and sets it as active camera.
//! \param position: Position of the space relative to its parent where the camera will be placed.
//! \param lookat: Position where the camera will look at. Also known as target.
//! \param parent: Parent scene node of the camera. Can be null. If the parent moves,
//! the camera will move too.
//! \return Returns pointer to interface to camera
ICameraSceneNode* CSceneManager::addCameraSceneNode(ISceneNode* parent,
	const core::vector3df& position, const core::vector3df& lookat, s32 id)
{
	if (!parent)
		parent = this;

	ICameraSceneNode* node = new CCameraSceneNode(parent, this, id, position, lookat);
	node->drop();

	if (!ActiveCamera)
		ActiveCamera = node;

	return node;
}



//! Adds a camera scene node which is able to be controlle with the mouse similar
//! like in the 3D Software Maya by Alias Wavefront.
//! The returned pointer must not be dropped.
ICameraSceneNode* CSceneManager::addCameraSceneNodeMaya(ISceneNode* parent, 
	f32 rotateSpeed, f32 zoomSpeed, f32 translationSpeed, s32 id)
{
	if (!parent)
		parent = this;

	ICameraSceneNode* node = new CCameraMayaSceneNode(parent, this, id, rotateSpeed,
		zoomSpeed, translationSpeed);
	node->drop();

	if (!ActiveCamera)
		ActiveCamera = node;

	return node;
}



//! Adds a dynamic light scene node. The light will cast dynamic light on all
//! other scene nodes in the scene, which have the material flag video::MTF_LIGHTING
//! turned on. (This is the default setting in most scene nodes).
ILightSceneNode* CSceneManager::addLightSceneNode(ISceneNode* parent,
	const core::vector3df& position, video::Colorf color, f32 range, s32 id)
{
	if (!parent)
		parent = this;

	ILightSceneNode* node = new CLightSceneNode(parent, this, id, position, color, range);
	node->drop();

	return node;
}



//! Adds a billboard scene node to the scene. A billboard is like a 3d sprite: A 2d element,
//! which always looks to the camera. It is usually used for things like explosions, fire,
//! lensflares and things like that.
IBillboardSceneNode* CSceneManager::addBillboardSceneNode(ISceneNode* parent,
	const core::dimension2d<f32>& size, const core::vector3df& position, s32 id)
{
	if (!parent)
		parent = this;

	IBillboardSceneNode* node = new CBillboardSceneNode(parent, this, id, position, size);
	node->drop();

	return node;
}


//! Returns the current active camera.
//! \return The active camera is returned. Note that this can be NULL, if there
//! was no camera created yet.
ICameraSceneNode* CSceneManager::getActiveCamera()
{
	return ActiveCamera;
}



//! Sets the active camera. The previous active camera will be deactivated.
//! \param camera: The new camera which should be active.
void CSceneManager::setActiveCamera(ICameraSceneNode* camera)
{
	ActiveCamera = camera;
}



//! renders the node.
void CSceneManager::render()
{
}


//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CSceneManager::getBoundingBox() const
{
	#ifdef _DEBUG
	os::Warning::print("Bounding Box of Scene Manager wanted.");
	_asm int 3;
	#endif

	// should never be used.
	return *((core::aabbox3d<f32>*)0);
}



//! registers a node for rendering it at a specific time.
void CSceneManager::registerNodeForRendering(ISceneNode* node, ESceneNodeRenderTime time)
{
	switch(time)
	{
	case SNRT_LIGHT_AND_CAMERA:
		LightAndCameraList.push_back(node);
		break;
	case SNRT_DEFAULT:
		{
			s32 count = node->getMaterialCount();

			for (s32 i=0; i<count; ++i)
				if (node->getMaterial(i).isTransparent())
				{
					TransparentNodeEntry e(node, camTransPos);
					TransparentNodeList.push_back(e);
					return;
				}
			
			DefaultNodeList.push_back(node);
		}
		break;
	}
}


//! draws all scene nodes
void CSceneManager::drawAll()
{
	if (!Driver)
		return;

	// calculate camera pos.
	camTransPos.set(0,0,0);
	if (ActiveCamera)
		camTransPos = ActiveCamera->getAbsolutePosition();

	// let all nodes register themselfes
	OnPreRender();

	//render lights and cameras

	Driver->deleteAllDynamicLights();

	for (u32 i=0; i<LightAndCameraList.size(); ++i)
		LightAndCameraList[i]->render();

	LightAndCameraList.clear();

	// render default objects

	DefaultNodeList.sort(); // sort by textures

	for (s32 i = 0; i<DefaultNodeList.size(); ++i)
		DefaultNodeList[i].node->render();

	DefaultNodeList.clear();

	// render transparent objects.

	TransparentNodeList.sort(); // sort by distance from camera

	for (s32 i = 0; i<TransparentNodeList.size(); ++i)
		TransparentNodeList[i].node->render();

	TransparentNodeList.clear();

	// do animations and other stuff.
	OnPostRender(os::Timer::getTime());
}



//! creates a rotation animator, which rotates the attached scene node around itself.
//! \param rotationPerSecond: Specifies the speed of the animation
//! \return Returns the animator. Attach it to a scene node with ISceneNode::addAnimator()
//! and the animator will animate it.
ISceneNodeAnimator* CSceneManager::createRotationAnimator(const core::vector3df& rotationPerSecond)
{
	ISceneNodeAnimator* anim = new CSceneNodeAnimatorRotation(rotationPerSecond);
	return anim;
}

//! creates a fly circle animator, which lets the attached scene node fly around a center.
//! \param center: Center of the cirle
//! \param rotationSpeed: 
//! \return Returns the animator. Attach it to a scene node with ISceneNode::addAnimator()
//! and the animator will animate it.
ISceneNodeAnimator* CSceneManager::createFlyCircleAnimator(const core::vector3df& normal, f32 radius, f32 speed)
{
	ISceneNodeAnimator* anim = new CSceneNodeAnimatorFlyCircle(normal, radius, speed);
	return anim;
}



// creates a scenemanager
ISceneManager* createSceneManager(video::IVideoDriver* driver, io::IFileSystem* fs)
{
	return new CSceneManager(driver, fs);
}


} // end namespace scene
} // end namespace klib