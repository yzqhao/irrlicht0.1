// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_SCENE_MANAGER_H_INCLUDED__
#define __C_SCENE_MANAGER_H_INCLUDED__

#include "ISceneManager.h"
#include "ISceneNode.h"
#include "irrstring.h"
#include "array.h"

namespace irr
{
namespace scene
{

	/*!
		The Scene Manager manages scene nodes, mesh recources, cameras and all the other stuff.
	*/
	class CSceneManager : public ISceneManager, ISceneNode
	{
	public:

		//! constructor
		CSceneManager(video::IVideoDriver* driver, io::IFileSystem* fs);

		//! destructor
		virtual ~CSceneManager();

		//! gets an animateable mesh. loads it if needed. returned pointer must not be dropped.
		virtual IAnimatedMesh* getMesh(const c8* filename);

		//! returns the video driver
		virtual video::IVideoDriver* getVideoDriver();

		//! adds a test scene node for test purposes to the scene. It is a simple cube of (1,1,1) size. 
		//! the returned pointer must not be dropped.
		virtual ISceneNode* addTestSceneNode(f32 size=10.0f, ISceneNode* parent=0, s32 id=-1,
			const core::vector3df& position = core::vector3df(0,0,0),	const core::vector3df& rotation = core::vector3df(0,0,0),	const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

		//! adds a scene node for rendering an animated mesh model
		virtual IAnimatedMeshSceneNode* addAnimatedMeshSceneNode(IAnimatedMesh* mesh, ISceneNode* parent=0, s32 id=-1,
			const core::vector3df& position = core::vector3df(0,0,0),	const core::vector3df& rotation = core::vector3df(0,0,0),	const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

		//! adds a scene node for rendering a static mesh
		//! the returned pointer must not be dropped.
		virtual ISceneNode* addMeshSceneNode(IMesh* mesh, ISceneNode* parent=0, s32 id=-1,
			const core::vector3df& position = core::vector3df(0,0,0),	const core::vector3df& rotation = core::vector3df(0,0,0),	const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

		//! renders the node.
		virtual void render();

		//! returns the axis aligned bounding box of this node
		virtual const core::aabbox3d<f32>& getBoundingBox() const;

		//! registers a node for rendering it at a specific time.
		virtual void registerNodeForRendering(ISceneNode* node, ESceneNodeRenderTime = SNRT_DEFAULT);

		//! draws all scene nodes
		virtual void drawAll();

		//! Adds a scene node for rendering using a binary space partition tree.
		virtual IBspTreeSceneNode* addBspTreeSceneNode(IMesh* mesh, ISceneNode* parent=0, s32 id=-1);

		//! Adss a scene node for rendering using a octtree. This a good method for rendering 
		//! scenes with lots of geometry. The Octree is built on the fly from the mesh, much
		//! faster then a bsp tree.
		//! \param mesh: The mesh containing all geometry from which the octtree will be build.
		//! \param parent: Parent node of the octtree node.
		//! \param id: id of the node.
		//! \return Returns the pointer to the octtree if successful, otherwise 0. 
		virtual ISceneNode* addOctTreeSceneNode(IMesh* mesh, ISceneNode* parent=0, s32 id=-1);

		//! Adds a camera scene node to the tree and sets it as active camera.
		//! \param position: Position of the space relative to its parent where the camera will be placed.
		//! \param lookat: Position where the camera will look at. Also known as target.
		//! \param parent: Parent scene node of the camera. Can be null. If the parent moves,
		//! the camera will move too.
		//! \return Returns pointer to interface to camera
		virtual ICameraSceneNode* addCameraSceneNode(ISceneNode* parent = 0,
			const core::vector3df& position = core::vector3df(0,0,0), 
			const core::vector3df& lookat = core::vector3df(0,0,0), s32 id=-1);

		//! Adds a camera scene node which is able to be controlle with the mouse similar
		//! like in the 3D Software Maya by Alias Wavefront.
		//! The returned pointer must not be dropped.
		virtual ICameraSceneNode* addCameraSceneNodeMaya(ISceneNode* parent = 0,
			f32 rotateSpeed = -1500.0f, f32 zoomSpeed = 200.0f, f32 translationSpeed = 100.0f, s32 id=-1);

		//! Adds a dynamic light scene node. The light will cast dynamic light on all
		//! other scene nodes in the scene, which have the material flag video::MTF_LIGHTING
		//! turned on. (This is the default setting in most scene nodes).
		virtual ILightSceneNode* addLightSceneNode(ISceneNode* parent = 0,
			const core::vector3df& position = core::vector3df(0,0,0),
			video::Colorf color = video::Colorf(1.0f, 1.0f, 1.0f),	f32 range=100.0f, s32 id=-1);

		//! Adds a billboard scene node to the scene. A billboard is like a 3d sprite: A 2d element,
		//! which always looks to the camera. It is usually used for things like explosions, fire,
		//! lensflares and things like that.
		virtual IBillboardSceneNode* addBillboardSceneNode(ISceneNode* parent = 0,
			const core::dimension2d<f32>& size = core::dimension2d<f32>(10.0f, 10.0f),
			const core::vector3df& position = core::vector3df(0,0,0), s32 id=-1);

		//! Returns the current active camera.
		//! \return The active camera is returned. Note that this can be NULL, if there
		//! was no camera created yet.
		virtual ICameraSceneNode* getActiveCamera();

		//! Sets the active camera. The previous active camera will be deactivated.
		//! \param camera: The new camera which should be active.
		virtual void setActiveCamera(ICameraSceneNode* camera);

		//! creates a rotation animator, which rotates the attached scene node around itself.
		//! \param rotationPerSecond: Specifies the speed of the animation
		//! \return Returns the animator. Attach it to a scene node with ISceneNode::addAnimator()
		//! and the animator will animate it.
		virtual ISceneNodeAnimator* createRotationAnimator(const core::vector3df& rotationPerSecond);

		//! creates a fly circle animator, which lets the attached scene node fly 
		//! around a center. The center is the position of the scene node.
		//! \param rotationSpeed: 
		//! \return Returns the animator. Attach it to a scene node with ISceneNode::addAnimator()
		//! and the animator will animate it.
		virtual ISceneNodeAnimator* createFlyCircleAnimator(const core::vector3df& normal, f32 radius, f32 speed);

	private:

		//! adds a mesh to the list
		void addMesh(const c8* filename, IAnimatedMesh* mesh);

		//! returns an already loaded mesh
		IAnimatedMesh* findMesh(const c8* lowerMadeFilename);
		

		struct MeshEntry
		{
			core::stringc Name;
			IAnimatedMesh* Mesh;

			bool operator < (const MeshEntry& other) const
			{
				return (Name < other.Name);
			}
		};

		struct DefaultNodeEntry
		{
			DefaultNodeEntry() {};

			DefaultNodeEntry(ISceneNode* n)
			{
				textureValue = 0;

				if (n->getMaterialCount())
					textureValue = (s32)(n->getMaterial(0).Texture1);

				node = n;
			}

			ISceneNode* node;
			s32 textureValue;

			bool operator < (const DefaultNodeEntry& other) const
			{
				return (textureValue < other.textureValue);
			}
		};


		struct TransparentNodeEntry
		{
			TransparentNodeEntry() {};

			TransparentNodeEntry(ISceneNode* n, core::vector3df camera)
			{
				node = n;

				// TODO: this could be optimized, by not using sqrt
				distance = (f32)(node->getAbsoluteTransformation().getTranslation().getDistanceFrom(camera));
			}

			ISceneNode* node;
			f32 distance;

			bool operator < (const TransparentNodeEntry& other) const
			{
				return (distance < other.distance);
			}
		};

		//! loaded meshes
		core::array<MeshEntry> Meshes;

		//! video driver
		video::IVideoDriver* Driver;

		//! file system
		io::IFileSystem* FileSystem;

		//! render pass lists
		core::array<ISceneNode*> LightAndCameraList;
		core::array<DefaultNodeEntry> DefaultNodeList;
		core::array<TransparentNodeEntry> TransparentNodeList;

		//! current active camera
		ICameraSceneNode* ActiveCamera;
		core::vector3df camTransPos; // Position of camera for transparent nodes.
		
	};

} // end namespace video
} // end namespace scene

#endif