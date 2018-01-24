// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __I_SCENE_MANAGER_H_INCLUDED__
#define __I_SCENE_MANAGER_H_INCLUDED__

#include "IUnknown.h"
#include "vector3d.h"
#include "dimension2d.h"
#include "Color.h"

namespace irr
{

namespace io
{
	class IFileSystem;
}

namespace video
{
	class IVideoDriver;
}



namespace scene
{
	//! Enumeration with render times.
	/** A parameter passed to the registerNodeForRendering() method of the ISceneManager, 
	specifiing when the mode wants to be drawed in relation to the other nodes. */
	enum ESceneNodeRenderTime
	{
		//! scene nodes which are lights or camera should use this,
		//! the very first pass.
		SNRT_LIGHT_AND_CAMERA,	

		//! Default render time, all normal objects should use this.
		//! The scene Manager will determine by itself if an object is 
		//! transparent, and do everything necessary by itself.
		SNRT_DEFAULT,			

		//! Drawn after the transparent nodes, the time for drawing shadow volumes						
		SNRT_SHADOW,			

		//! Never used, value specifing how much parameters there are
		SNRT_COUNT				
	};

	class IMesh;
	class IAnimatedMesh;
	class ISceneNode;
	class ICameraSceneNode;
	class IAnimatedMeshSceneNode;
	class IBspTreeSceneNode;
	class ISceneNodeAnimator;
	class ILightSceneNode;
	class IBillboardSceneNode;

	//!	The Scene Manager manages scene nodes, mesh recources, cameras and all the other stuff.
	/** All Scene nodes can be created only here. There is a always growing list of scene 
	nodes for lots of purposes: Indoor rendering scene nodes like the Octree 
	(addOctTreeSceneNode()) or the binary space partition tree (addBspTreeSceneNode()),
	different Camera scene nodes (addCameraSceneNode(), addCameraSceneNodeMaya()),
	scene nodes for Light (addLightSceneNode()), Billboards (addBillboardSceneNode())
	and so on.
	A scene node is a node in the hirachical scene graph. Every scene node may have children,
	which are other scene nodes. Children move relative the their parents position. If the parent of a node is not
	visible, its children won't be visible too. In this way, it is for example easily possible
	to attach a light to a moving car, or to place a walking character on a moving platform
	on a moving ship.
	*/
	class ISceneManager : public IUnknown
	{
	public:

		//! destructor
		virtual ~ISceneManager() {};

		//! Gets pointer to an animateable mesh. Loads it if needed.
		//! Currently there are the following mesh formats supported:
		//! .obj(Alias Wavefront Maya), .ms3d(Milkshape3D), .bsp(Quake3 Level),
		//! and .md2(Quake2 Model). More formats coming soon, make a feature request on
		//! the Irrlicht Engine homepage if you like.
		//! \param filename: Filename of the mesh to load.
		//! \return Returns NULL if failed and the pointer to the mesh if 
		//! successful. 
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual IAnimatedMesh* getMesh(const c8* filename) = 0;

		//! Returns the video driver.
		//! \return Returns pointer to the video Driver.
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual video::IVideoDriver* getVideoDriver() = 0;

		//! Adds a test scene node for test purposes of the scene. It is a simple cube of (1,1,1) size. 
		//! \param size: Size of the cube.
		//! \param parent: Parent of the scene node. Can be NULL if no parent.
		//! \param id: Id of the node. This id can be used to identify the scene node.
		//! \param position: Position of the space relative to its parent where the
		//! scene node will be placed.
		//! \param rotation: Initital rotation of the scene node.
		//! \param scale: Initial scale of the scene node.
		//! \return Returns pointer to the created test scene node.
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual ISceneNode* addTestSceneNode(f32 size=10.0f, ISceneNode* parent=0, s32 id=-1,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f)) = 0;

		//! Adds a scene node for rendering an animated mesh model.
		//! \param mesh: Pointer to the loaded animated mesh to be displayed.
		//! \param parent: Parent of the scene node. Can be NULL if no parent.
		//! \param id: Id of the node. This id can be used to identify the scene node.
		//! \param position: Position of the space relative to its parent where the
		//! scene node will be placed.
		//! \param rotation: Initital rotation of the scene node.
		//! \param scale: Initial scale of the scene node.
		//! \return Returns pointer to the created scene node.
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual IAnimatedMeshSceneNode* addAnimatedMeshSceneNode(IAnimatedMesh* mesh, ISceneNode* parent=0, s32 id=-1,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f)) = 0;

		//! Adds a scene node for rendering a static mesh.
		//! \param mesh: Pointer to the loaded static mesh to be displayed.
		//! \param parent: Parent of the scene node. Can be NULL if no parent.
		//! \param id: Id of the node. This id can be used to identify the scene node.
		//! \param position: Position of the space relative to its parent where the
		//! scene node will be placed.
		//! \param rotation: Initital rotation of the scene node.
		//! \param scale: Initial scale of the scene node.
		//! \return Returns pointer to the created scene node.
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual ISceneNode* addMeshSceneNode(IMesh* mesh, ISceneNode* parent=0, s32 id=-1,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f)) = 0;

		//! Adds a scene node for rendering using a binary space partition tree.
		//! This is a nice method for rendering indoor scenes. Based on a mesh given as parameter, the
		//! tree will be build. Note that The build process can be slow, if there are lots of polygons.
		//! Note that the implementation of the BspTree in the Irrlicht Engine is currently
		//! not complete a little bit buggy. You may want to use the OctTree instead, it already works.
		//! Use ISceneManager::addOctTreeSceneNode() for that.
		//! \param mesh: The mesh containing all geometry from which the binary space partition tree will be build.
		//! \param parent: Parent node of the bsp tree node.
		//! \param id: id of the node. This id can be used to identify the node.
		//! \return Returns the pointer to the IBspTreeSceneNode if successful, otherwise 0. 
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual IBspTreeSceneNode* addBspTreeSceneNode(IMesh* mesh, ISceneNode* parent=0, s32 id=-1) = 0;

		//! Adss a scene node for rendering using a octtree. This a good method for rendering 
		//! scenes with lots of geometry. The Octree is built on the fly from the mesh, much
		//! faster then a bsp tree.
		//! \param mesh: The mesh containing all geometry from which the octtree will be build.
		//! \param parent: Parent node of the octtree node.
		//! \param id: id of the node. This id can be used to identify the node.
		//! \return Returns the pointer to the octtree if successful, otherwise 0. 
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual ISceneNode* addOctTreeSceneNode(IMesh* mesh, ISceneNode* parent=0, s32 id=-1) = 0;

		//! Adds a camera scene node to the scene and sets it as active camera.
		//! \param position: Position of the space relative to its parent where the camera will be placed.
		//! \param lookat: Position where the camera will look at. Also known as target.
		//! \param parent: Parent scene node of the camera. Can be null. If the parent moves,
		//! the camera will move too.
		//! \param id: id of the camera. This id can be used to identify the camera.
		//! \return Returns pointer to interface to camera if successful, otherwise 0.
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual ICameraSceneNode* addCameraSceneNode(ISceneNode* parent = 0,
			const core::vector3df& position = core::vector3df(0,0,0), 
			const core::vector3df& lookat = core::vector3df(0,0,100), s32 id=-1) = 0;

		//! Adds a camera scene node which is able to be controlle with the mouse similar
		//! like in the 3D Software Maya by Alias Wavefront.
		//! \param parent: Parent scene node of the camera. Can be null.
		//! \param rotateSpeed: Rotation speed of the camera.
		//! \param zoomSpeed: Zoom speed of the camera.
		//! \param tranlationSpeed: TranslationSpeed of the camera.
		//! \param id: id of the camera. This id can be used to identify the camera.
		//! \return Returns pointer to interface to camera if successful, otherwise 0.
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual ICameraSceneNode* addCameraSceneNodeMaya(ISceneNode* parent = 0,
			f32 rotateSpeed = -1500.0f, f32 zoomSpeed = 200.0f, f32 translationSpeed = 1500.0f, s32 id=-1) = 0;

		//! Adds a dynamic light scene node. The light will cast dynamic light on all
		//! other scene nodes in the scene, which have the material flag video::MTF_LIGHTING
		//! turned on. (This is the default setting in most scene nodes).
		//! \param parent: Parent scene node of the light. Can be null. If the parent moves,
		//! the light will move too.
		//! \param position: Position of the space relative to its parent where the light will be placed.
		//! \param color: Diffuse color of the light. Ambient or Specular colors can be set manually with
		//! the ILightSceneNode::getLightData() method.
		//! \param range: Range of the light.
		//! \param id: id of the node. This id can be used to identify the node.
		//! \return Returns pointer to the interface of the light if successful, otherwise NULL.
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual ILightSceneNode* addLightSceneNode(ISceneNode* parent = 0,
			const core::vector3df& position = core::vector3df(0,0,0),
			video::Colorf color = video::Colorf(1.0f, 1.0f, 1.0f),	f32 range=100.0f, s32 id=-1) = 0;

		//! Adds a billboard scene node to the scene. A billboard is like a 3d sprite: A 2d element,
		//! which always looks to the camera. It is usually used for things like explosions, fire,
		//! lensflares and things like that.
		//! \param parent: Parent scene node of the billboard. Can be null. If the parent moves,
		//! the billboard will move too.
		//! \param position: Position of the space relative to its parent where the billboard will be placed.
		//! \param size: Size of the billboard. This size is 2 dimensional because a billboard only has
		//! width and height.
		//! \param id: An id of the node. This id can be used to identify the node.
		//! \return Returns pointer to the billboard if successful, otherwise NULL.
		//! This pointer should not be dropped. See IUnknown::drop() for more information.
		virtual IBillboardSceneNode* addBillboardSceneNode(ISceneNode* parent = 0,
			const core::dimension2d<f32>& size = core::dimension2d<f32>(10.0f, 10.0f),
			const core::vector3df& position = core::vector3df(0,0,0), s32 id=-1) = 0;

		//! Returns the current active camera.
		//! \return The active camera is returned. Note that this can be NULL, if there
		//! was no camera created yet.
		virtual ICameraSceneNode* getActiveCamera() = 0;

		//! Sets the active camera. The previous active camera will be deactivated.
		//! \param camera: The new camera which should be active.
		virtual void setActiveCamera(ICameraSceneNode* camera) = 0;

		//! Registers a node for rendering it at a specific time.
		//! This method should only be used by SceneNodes when they get a 
		//! ISceneNode::OnPreRender() call.
		virtual void registerNodeForRendering(ISceneNode* node, ESceneNodeRenderTime time = SNRT_DEFAULT) = 0;

		//! Draws all scene nodes.
		virtual void drawAll() = 0;

		//! Creates a rotation animator, which rotates the attached scene node around itself.
		//! \param rotationPerSecond: Specifies the speed of the animation
		//! \return Returns the animator. Attach it to a scene node with ISceneNode::addAnimator()
		//! and the animator will animate it.
		//! If you no longer need the animator, you should call ISceneNodeAnimator::drop().
		//! See IUnknown::drop() for more information.
		virtual ISceneNodeAnimator* createRotationAnimator(const core::vector3df& rotationPerSecond) = 0;

		//! creates a fly circle animator, which lets the attached scene node fly 
		//! around a center. The center is the position of the scene node.
		//! \param rotationPerSecond: Specifies the speed of the animation
		//! \return Returns the animator. Attach it to a scene node with ISceneNode::addAnimator()
		//! and the animator will animate it.
		//! If you no longer need the animator, you should call ISceneNodeAnimator::drop().
		//! See IUnknown::drop() for more information.
		virtual ISceneNodeAnimator* createFlyCircleAnimator(const core::vector3df& normal, f32 radius, f32 speed=0.001f) = 0;
	};


	// creates a scenemanager
	ISceneManager* createSceneManager(video::IVideoDriver* driver, io::IFileSystem* fs);

} // end namespace scene
} // end namespace irr

#endif