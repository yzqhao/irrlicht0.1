// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __I_GUI_SKIN_H_INCLUDED__
#define __I_GUI_SKIN_H_INCLUDED__

#include "IUnknown.h"
#include "Color.h"
#include "IGUISkin.h"

namespace irr
{
namespace gui
{
	class IGUIFont;

	//! Enumeration of available default skins.
	enum EGUI_SKIN_TYPE
	{
		EGST_WINDOWS_STANDARD,
		EGST_BLACK_WINDOWS,
	};

	enum EGUI_DEFAULT_COLOR
	{
		//! Dark shadow for three-dimensional display elements. 
		EGDC_3D_DARK_SHADOW = 0,
		//! Shadow color for three-dimensional display elements (for edges facing away from the light source). 
		EGDC_3D_SHADOW,			
		//! Face color for three-dimensional display elements and for dialog box backgrounds. 
		EGDC_3D_FACE,			
		//! Highlight color for three-dimensional display elements (for edges facing the light source.) 
		EGDC_3D_HIGH_LIGHT,		
		//! Light color for three-dimensional display elements (for edges facing the light source.) 
		EGDC_3D_LIGHT,			
		
		//! Active window border. 
        EGDC_ACTIVE_BORDER,		
		//! Active window title bar text.
		EGDC_ACTIVE_CAPTION,	
		//! Background color of multiple document interface (MDI) applications.
		EGDC_APP_WORKSPACE,		
		//! Text on a button
		EGDC_BUTTON_TEXT,		
		//! Grayed (disabled) text. 
		EGDC_GRAY_TEXT,			
		//! Item(s) selected in a control. 
		EGDC_HIGH_LIGHT,		
		//! Text of item(s) selected in a control. 
		EGDC_HIGH_LIGHT_TEXT,	
		//! Inactive window border. 
		EGDC_INACTIVE_BORDER,	
		//! Inactive window caption. 
		EGDC_INACTIVE_CAPTION,	
		//! Tool tip color
		EGDC_TOOLTIP,			
		//! Scrollbar gray area
		EGDC_SCROLLBAR,			
		//! Window background
		EGDC_WINDOW,			

		EGDC_COUNT
	};

	//! Enumeration for default sizes.
	enum EGUI_DEFAULT_SIZE
	{
		//! default with / height of scrollbar
		EGDS_SCROLLBAR_SIZE = 0,	
		//! height of menu
		EGDS_MENU_HEIGHT,			
		EGDS_WINDOW_BUTTON_WIDTH,	
		//! width of a checkbox check
		EGDS_CHECK_BOX_WIDTH,		
		
		EGDS_COUNT
	};

	//! A skin modifies the look of the GUI elements.
	class IGUISkin : public IUnknown
	{
	public:

		//! destructor
		~IGUISkin() {};

		//! returns default color
		virtual video::Color getColor(EGUI_DEFAULT_COLOR color) = 0;

		//! sets a default color
		virtual void setColor(EGUI_DEFAULT_COLOR which, video::Color newColor) = 0;

		//! returns default color
		virtual s32 getSize(EGUI_DEFAULT_SIZE size) = 0;

		//! sets a default size
		virtual void setSize(s32 size) = 0;

		//! returns the default font
		virtual IGUIFont* getFont() = 0;

		//! sets a default font
		virtual void setFont(IGUIFont* font) = 0;
	};


	//! creates a color skin
	IGUISkin* createSkin(EGUI_SKIN_TYPE type);

} // end namespace gui
} // end namespace irr

#endif