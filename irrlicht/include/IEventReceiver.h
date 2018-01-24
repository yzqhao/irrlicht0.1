// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __I_EVENT_RECEIVER_H_INCLUDED__
#define __I_EVENT_RECEIVER_H_INCLUDED__

#include "position2d.h"

namespace irr
{
	//! Enumeration for all event types there are.
	enum EEVENT_TYPE
	{
		//! An event of the graphical user interface.
		EET_GUI_EVENT = 0,

		//! A mouse input event.
		EET_MOUSE_INPUT_EVENT,

		//! A key input evant.
		EET_KEY_INPUT_EVENT
	};

	//! Enumeration for all mouse input events
	enum EMOUSE_INPUT_EVENT
	{
		//! Left mouse button was pressed down.
		EMIE_LMOUSE_PRESSED_DOWN = 0,
		//! Right mouse button was pressed down.
		EMIE_RMOUSE_PRESSED_DOWN,
		//! Middle mouse button was pressed down.
		EMIE_MMOUSE_PRESSED_DOWN,
		//! Left mouse button was left up.
		EMIE_LMOUSE_LEFT_UP,
		//! Right mouse button was left up.
		EMIE_RMOUSE_LEFT_UP,
		//! Middle mouse button was left up.
		EMIE_MMOUSE_LEFT_UP,
		//! The mouse cursor changed its position.
		EMIE_MOUSE_MOVED
	};

	namespace gui
	{

		class IGUIElement;

		//! Enumeration for all events which are sendable by the gui system
		enum EGUI_EVENT_TYPE
		{
			//! A gui element has lost its key focus.
			EGET_ELEMENT_KEY_FOCUS_LOST = 0,
			//! A gui element has lost its mouse focus.
			EGET_ELEMENT_MOUSE_FOCUS_LOST,
			//! A gui element was hovered.
			EGET_ELEMENT_HOVERED,
			//! A button was clicked.
			EGET_BUTTON_CLICKED,
			//! A scrollbar has changed its position.
			EGET_SCROLL_BAR_CHANGED,
			//! A checkbox has changed its check state.
			EGET_CHECKBOX_CHANGED,
			//! A new item in a listbox was seleted.
			EGET_LISTBOX_CHANGED, 
			//! An item in the listbox was selected, which was already selected.
			EGET_LISTBOX_SELECTED_AGAIN, 
		};
	} // end namespace gui


//! Struct for holding event data. An event can be a gui, mouse or keyboard event.
struct SEvent
{
	EEVENT_TYPE EventType;

	union
	{
		struct 
		{
			gui::IGUIElement* Caller;
			gui::EGUI_EVENT_TYPE EventType;			
		} GUIEvent;

		struct
		{
			s32 X, Y; // mouse positions;
			EMOUSE_INPUT_EVENT Event;
		} MouseInput;

		struct
		{
			s32 Key; // TODO key
			bool PressedDown; // if not pressed, then left up
		} KeyInput;

	};

};

//! Interface of an object wich can receive events.
class IEventReceiver
{
public:

	//! called if an event happened. returns true if event was processed
	virtual bool OnEvent(SEvent event) = 0;
};


} // end namespace

#endif