// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __I_SCENE_NODE_H_INCLUDED__
#define __I_SCENE_NODE_H_INCLUDED__

#include "IUnknown.h"
#include "ISceneNodeAnimator.h"
#include "SMaterial.h"
#include "irrstring.h"
#include "aabbox3d.h"
#include "matrix4.h"
#include "list.h"

namespace irr
{
namespace scene
{

	class ISceneManager;

	//! Scene node interface.
	/** A scene node is a node in the hirachical scene graph. Every scene node may have children,
	which are other scene nodes. Children move relative the their parents position. If the parent of a node is not
	visible, its children won't be visible too. In this way, it is for example easily possible
	to attach a light to a moving car, or to place a walking character on a moving platform
	on a moving ship. */
	class ISceneNode : public IUnknown
	{
	public:



		//! Constructor
		ISceneNode(	ISceneNode* parent, ISceneManager* mgr, s32 id=-1,
					const core::vector3df& position = core::vector3df(0,0,0),
					const core::vector3df& rotation = core::vector3df(0,0,0),
					const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f))
			: IsVisible(true), ID(id), Parent(parent), SceneManager(mgr)
		{
			if (Parent)
				Parent->addChild(this);

			RelativeTransformation.setRotationDegrees(rotation);
			RelativeTransformation.setTranslation(position);

			core::matrix4 mat;
			mat.setScale(scale);
			RelativeTransformation *= mat;

			AnimatedRelativeTransformation = RelativeTransformation;
			updateAbsolutePosition();			
		}



		//! Destructor
		virtual ~ISceneNode()
		{
			// delete all children
			core::list<ISceneNode*>::Iterator it = Children.begin();
			for (; it != Children.end(); ++it)
				(*it)->drop();

			// delete all animators
			core::list<ISceneNodeAnimator*>::Iterator ait = Animators.begin();
				for (; ait != Animators.end(); ++ait)
					(*ait)->drop();
		}


		//! This method is called just before the rendering process of the whole scene.
		//! Nodes may register themselves in the render pipeline during this call,
		//! precalculate the geometry which should be renderered, and prevent their
		//! children from being able to register them selfes if they are clipped by simply
		//! not calling their OnPreRender-Method.
		virtual void OnPreRender() 
		{
			if (IsVisible)
			{
				core::list<ISceneNode*>::Iterator it = Children.begin();
				for (; it != Children.end(); ++it)
					(*it)->OnPreRender();
			}
		}


		//! OnPostRender() is called just after rendering the whole scene.
		//! nodes may calculate or store animations here, and may do other useful things,
		//! dependent on what they are.
		//! \param timeMs: Current time in milli seconds.
		virtual void OnPostRender(u32 timeMs)
		{
			AnimatedRelativeTransformation = RelativeTransformation;

			if (IsVisible)
			{
				// animate this node with all animators

				core::list<ISceneNodeAnimator*>::Iterator ait = Animators.begin();
				for (; ait != Animators.end(); ++ait)
					(*ait)->animateNode(this, timeMs);

				// update absolute position
				updateAbsolutePosition();

				// perform the post render process on all children
				
				core::list<ISceneNode*>::Iterator it = Children.begin();
				for (; it != Children.end(); ++it)
					(*it)->OnPostRender(timeMs);
			}
		}


		//! Renders the node.
		virtual void render() = 0;


		//! Returns the name of the node.
		//! \return Returns name as wide character string.
		virtual const wchar_t* getName() const
		{
			return Name.c_str();
		}


		//! Sets the name of the node.
		//! \param name: New name of the scene node.
		virtual void setName(const wchar_t* name)
		{
			Name = name;
		}


		//! Returns the axis aligned bounding box of this node.
		virtual const core::aabbox3d<f32>& getBoundingBox() const = 0;


		//! returns the absolute transformation of the node. Is recalculated every OnPostRender()-call.
		core::matrix4& getAbsoluteTransformation()
		{
			return AbsoluteTransformation;
		}


		//! Returns the transformation relative to the parent of the node.
		//! May be recalculated every OnPostRender()-call by one or more NodeAnimators.
		core::matrix4& getRelativeTransformation()
		{
			return RelativeTransformation;
		}


		//! Returns the animated transformation relative to the parent of the node.
		core::matrix4& getAnimatedRelativeTransformation()
		{
			return AnimatedRelativeTransformation;
		}


		//! Returns true if the node is visible. This is only an option, set by the user and has
		//! nothing to do with geometry culling
		virtual bool isVisible()
		{
			return IsVisible;
		}


		//! Sets if the node should be visible or not. All children of this node won't be visible too.
		virtual void setVisible(bool isVisible)
		{
			IsVisible = isVisible;
		}


		//! Returns the id of the scene node. This id can be used to identify the node.
		virtual s32 getID()
		{
			return ID;
		}


		//! sets the id of the scene node. This id can be used to identify the node.
		virtual void setID(s32 id)
		{
			ID = id;
		}


		//! Adds a child to this scene node.
		virtual void addChild(ISceneNode* child)
		{
			if (child)
			{
				Children.push_back(child);
				child->grab();
			}
		}


		//! Removes a child from this scene node.
		virtual void removeChild(ISceneNode* child)
		{
			core::list<ISceneNode*>::Iterator it = Children.begin();
			for (; it != Children.end(); ++it)
				if ((*it) == child)
				{
					(*it)->drop();
					Children.erase(it);
					return;
				}
		}


		//! Removes this scene node from the scene, deleting it.
		virtual void remove()
		{
			if (Parent)
				Parent->removeChild(this);
		}


		//! Adds an animator which should animate this node.
		virtual void addAnimator(ISceneNodeAnimator* animator)
		{
			if (animator)
			{
				Animators.push_back(animator);
				animator->grab();
			}
		}



		//! Removes an animator from this scene node.
		virtual void removeAnimator(ISceneNodeAnimator* animator)
		{
			core::list<ISceneNodeAnimator*>::Iterator it = Animators.begin();
			for (; it != Animators.end(); ++it)
				if ((*it) == animator)
				{
					(*it)->drop();
					Animators.erase(it);
					return;
				}
		}


		//! Removes all animators from this scene node.
		virtual void removeAnimators()
		{
			core::list<ISceneNodeAnimator*>::Iterator it = Animators.begin();
			for (; it != Animators.end(); ++it)
				(*it)->drop();

			Animators.clear();	
		}


		//! Returns the material based on the zero based index i. To get the amount
		//! of materials used by this scene node, use getMaterialCount().
		//! This function is needed for inserting the node into the scene hirachy on a
		//! optimal position for minimizing renderstate changes, but can also be used
		//! to directly modify the material of a scene node.
		//! \param i: Zero based index i. The maximal value for this may be getMaterialCount() - 1.
		//! \return Returns the material of that index.
		virtual video::SMaterial& getMaterial(s32 i)
		{
			return *((video::SMaterial*)0);
		}

		//! Returns amount of materials used by this scene node.
		//! \return Returns current count of materials used by this scene node.
		virtual s32 getMaterialCount()
		{
			return 0;
		}

		//! Sets all material flags at once to a new value. Helpful for
		//! example, if you want to be the the whole mesh to be lighted by 
		//! \param flag: Which flag of all materials to be set.
		//! \param newvalue: New value of the flag.
		void setMaterialFlag(video::E_MATERIAL_FLAG flag, bool newvalue)
		{
			for (s32 i=0; i<getMaterialCount(); ++i)
				getMaterial(i).Flags[flag] = newvalue;
		}

		//! Sets the texture of the specified layer in all materials of this
		//! scene node to the new texture.
		//! \param textureLayer: Layer of texture to be set. Must be a value greater or
		//! equal than 0 and smaller than MATERIAL_MAX_TEXTURES.
		//! \param texture: Texture to be used.
		void setMaterialTexture(s32 textureLayer, video::ITexture* texture)
		{
			if (textureLayer<0 || textureLayer>= video::MATERIAL_MAX_TEXTURES)
				return;

			for (s32 i=0; i<getMaterialCount(); ++i)
				getMaterial(i).Textures[textureLayer] = texture;
		}

			
		//! Sets the material type of all materials s32 this scene node
		//! to a new material type.
		//! \param newType: New type of material to be set.
		void setMaterialType(video::E_MATERIAL_TYPE newType)
		{
			for (s32 i=0; i<getMaterialCount(); ++i)
				getMaterial(i).MaterialType = newType;
		}	


		//! Sets the position of the node. Note that the position is
		//! relative to the parent. 
		//! This method exists only for easier use of the scene node.
		//! You can also modify the position by yourself,
		//! using the GetRelativeTransformation() method. 
		//! \param position: New position of the node, relative to its parent.
		virtual void setRelativePosition(const core::vector3df& position)
		{
			RelativeTransformation.setTranslation(position);
		}



		//! Sets the position of the node. Note that the position is
		//! relative to the parent. 
		//! This method exists only for easier use of the scene node.
		//! You can also modify the position by yourself,
		//! using the GetRelativeTransformation() method. 
		//! \param position: New scale of the node
		virtual void setScale(const core::vector3df& scale)
		{
			RelativeTransformation.setScale(scale);
		}


		//! Gets the position of the node. Note that the position is
		//! relative to the parent and not animated.
		//! \return Returns the current not animated position of the node.
		virtual core::vector3df getRelativePosition() const
		{
			core::vector3df pos(0.0f, 0.0f, 0.0f);
			RelativeTransformation.transformVect(pos);
			return pos;
		}

		//! Gets the abolute position of the node. The position is absolute
		//! and animated.
		//! \return Returns the current absolute animated position of the node
		virtual core::vector3df getAbsolutePosition() const
		{
			core::vector3df pos(0.0f, 0.0f, 0.0f);
			AbsoluteTransformation.transformVect(pos);
			return pos;
		}


	protected:

		//! updates the absolute position based on the relative and the parents position
		void updateAbsolutePosition()
		{
			if (Parent)
				AbsoluteTransformation = Parent->getAbsoluteTransformation() * AnimatedRelativeTransformation;
			else
				AbsoluteTransformation = AnimatedRelativeTransformation;
		}


		//! name of the scene node.
		core::stringw Name;	

		//! absolute transformation of the node.
		core::matrix4 AbsoluteTransformation; 

		//! transformation relative to the parent of the node.
		core::matrix4 RelativeTransformation; 

		//! animated transformation relative to the parent of the node.
		core::matrix4 AnimatedRelativeTransformation; 

		//! Pointer to the parent
		ISceneNode* Parent;

		//! List of all children of this node
		core::list<ISceneNode*> Children;

		//! List of all children of this node
		core::list<ISceneNodeAnimator*> Animators;

		//! is the node visible?
		bool IsVisible;

		//! id of the node. 
		s32 ID;

		//! pointer to the scene manager
		ISceneManager* SceneManager;
	};

} // end namespace scene
} // end namespace irr

#endif