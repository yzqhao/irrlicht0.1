// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __I_GUI_ELEMENT_H_INCLUDED__
#define __I_GUI_ELEMENT_H_INCLUDED__

#include "IUnknown.h"
#include "list.h"
#include "rect.h"
#include "irrstring.h"
#include "IEventReceiver.h"

namespace irr
{
namespace gui
{

class IGUIEnvironment;

//! Base class of all GUI elements.
class IGUIElement : public IUnknown, public IEventReceiver
{
public:

	//! Constructor
	IGUIElement(IGUIEnvironment* environment, IGUIElement* parent, s32 id, core::rectEx<s32> rectangle)
		: Environment(environment), IsVisible(true), IsEnabled(true),
			Parent(parent), RelativeRect(rectangle), ID(id)
	{
		AbsoluteRect = RelativeRect;
		AbsoluteClippingRect = AbsoluteRect;

		if (Parent)
		{
			AbsoluteRect += Parent->getAbsolutePosition().UpperLeftCorner;
			AbsoluteClippingRect = AbsoluteRect;
			AbsoluteClippingRect.clipAgainst(Parent->AbsoluteClippingRect);
		}

		if (Parent)
			Parent->addChild(this);
	}


	//! Destructor
	virtual ~IGUIElement()
	{
		// delete all children
		core::list<IGUIElement*>::Iterator it = Children.begin();
		for (; it != Children.end(); ++it)
			(*it)->drop();
	};


	//! Returns parent of this element.
	IGUIElement* getParent()
	{
		return Parent;
	}


	//! Returns the relative rectangle of this element.
	core::rectEx<s32> getRelativePosition()
	{
		return RelativeRect;
	}


	//! Returns the absolute rectangle of element.
	core::rectEx<s32> getAbsolutePosition()
	{
		return AbsoluteRect;
	}


	//! Updates the absolute position.
	void updateAbsolutePosition(const core::rectEx<s32>& parentAbsolute)
	{
		AbsoluteRect = RelativeRect + parentAbsolute.UpperLeftCorner;

		AbsoluteClippingRect = AbsoluteRect;
		AbsoluteClippingRect.clipAgainst(parentAbsolute);		

		// update all children
		core::list<IGUIElement*>::Iterator it = Children.begin();
		for (; it != Children.end(); ++it)
			(*it)->updateAbsolutePosition(AbsoluteRect);
	}


	//! Returns the child element, which is at the position of the point.
	IGUIElement* getElementFromPoint(const core::position2d<s32>& point)
	{
		IGUIElement* target = 0;

		// we have to search from back to front.

		core::list<IGUIElement*>::Iterator it = Children.getLast();
		while(it != Children.end())
		{
			target = (*it)->getElementFromPoint(point);
			if (target)
				return target;

			--it;
		}

		if (AbsoluteRect.isPointInside(point))
			target = this;
		
		return target;
	}


	//! Adds a GUI element as new child of this element.
	void addChild(IGUIElement* child)
	{
		if (child)
		{
			Children.push_back(child);
			child->grab();
		}
	}


	//! Removes a child.
	void removeChild(IGUIElement* child)
	{
		core::list<IGUIElement*>::Iterator it = Children.begin();
		for (; it != Children.end(); ++it)
			if ((*it) == child)
			{
				(*it)->drop();
				Children.erase(it);
				return;
			}
	}


	//! Removes this element.
	void remove()
	{
		if (Parent)
			Parent->removeChild(this);
	}


	//! Draws the element and its children.
	virtual void draw()
	{
		core::list<IGUIElement*>::Iterator it = Children.begin();
		for (; it != Children.end(); ++it)
			(*it)->draw();
	}


	//! Moves this element.
	virtual void move(core::position2d<s32> absoluteMovement)
	{
		RelativeRect += absoluteMovement;
		core::rectEx<s32> parentRect(0,0,0,0);

		if (Parent)
			parentRect = Parent->getAbsolutePosition();

		updateAbsolutePosition(parentRect);
	}


	//! Returns true if element is visible.
	virtual bool isVisible()
	{
		return IsVisible;
	}



	//! Sets the visible state of this element.
	virtual void setVisible(bool visible)
	{
		IsVisible = visible;
	}


	//! Returns true if element is enabled.
	virtual bool isEnabled()
	{
		return IsEnabled;
	}


	//! Sets the enabled state of this element.
	virtual void setEnabled(bool enabled)
	{
		IsEnabled = enabled;
	}


	//! Sets the new caption of this element.
	virtual void setText(const wchar_t* text)
	{
		Text = text;
	}


	//! Returns caption of this element.
	virtual const wchar_t* getText()
	{
		return Text.c_str();
	}


	//! Returns id. Can be used to identify the element.
	s32 getID()
	{
		return ID;
	}


	//! Called if an event happened.
	virtual bool OnEvent(SEvent event)
	{
		if (Parent)
			Parent->OnEvent(event);
		return true;
	}
	

protected:

	//! List of all children of this element
	core::list<IGUIElement*> Children;

	//! Pointer to the parent
	IGUIElement* Parent;

	//! relative rect of element
	core::rectEx<s32> RelativeRect;

	//! absolute rect of element
	core::rectEx<s32> AbsoluteRect;

	//! absolute clipping rect of element
	core::rectEx<s32> AbsoluteClippingRect;

	//! is visible?
	bool IsVisible;

	//! is enabled?
	bool IsEnabled;

	//! caption
	core::stringw Text;

	//! id
	s32 ID;

	//! GUI Environment
	IGUIEnvironment* Environment;
};


} // end namespace gui
} // end namespace irr

#endif