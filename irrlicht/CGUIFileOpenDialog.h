// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_GUI_FILE_OPEN_DIALOG_H_INCLUDED__
#define __C_GUI_FILE_OPEN_DIALOG_H_INCLUDED__

#include "IGUIFileOpenDialog.h"
#include "IGUIButton.h"
#include "IGUIListBox.h"
#include "IFileSystem.h"

namespace irr
{
namespace gui
{

	class CGUIFileOpenDialog : public IGUIFileOpenDialog
	{
	public:

		//! constructor
		CGUIFileOpenDialog(io::IFileSystem* fs, const wchar_t* title, IGUIEnvironment* environment, IGUIElement* parent, s32 id);

		//! destructor
		virtual ~CGUIFileOpenDialog();

		//! returns the filename of the selected file. Returns NULL, if no file was selected.
		virtual const wchar_t* getFilename();

		//! called if an event happened.
		virtual bool OnEvent(SEvent event);

		//! draws the element and its children
		virtual void draw();

	private:

		//! fills the listbox with files.
		void fillListBox();

		core::position2d<s32> DragStart;
		bool Dragging;
		IGUIButton* CloseButton;
		IGUIButton* OKButton;
		IGUIButton* CancelButton;
		IGUIListBox* FileBox;
		IGUIElement* FileNameText;
		io::IFileSystem* FileSystem;

		io::IFileList* FileList;
	};


} // end namespace gui
} // end namespace irr

#endif