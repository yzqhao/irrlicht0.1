#include "CGUIFileOpenDialog.h"
#include "IGUISkin.h"
#include "IGUIEnvironment.h"
#include "IVideoDriver.h"
#include "IGUIButton.h"
#include "IGUIFont.h"
#include "GUIIcons.h"
#include "IFileList.h"
#include "os.h"

namespace irr
{
namespace gui
{

const s32 FOD_WIDTH = 350;
const s32 FOD_HEIGHT = 250;


//! constructor
CGUIFileOpenDialog::CGUIFileOpenDialog(io::IFileSystem* fs, const wchar_t* title, IGUIEnvironment* environment, IGUIElement* parent, s32 id)
: IGUIFileOpenDialog(environment, parent, id,
 core::rectEx<s32>((parent->getAbsolutePosition().getWidth()-FOD_WIDTH)/2,
					(parent->getAbsolutePosition().getHeight()-FOD_HEIGHT)/2,	
					(parent->getAbsolutePosition().getWidth()-FOD_WIDTH)/2+FOD_WIDTH,
					(parent->getAbsolutePosition().getHeight()-FOD_HEIGHT)/2+FOD_HEIGHT)),	
  Dragging(false), FileNameText(0), FileSystem(fs), FileList(0)
{
	#ifdef _DEBUG
	IGUIElement::setDebugName("CGUIFileOpenDialog");
	#endif

	Text = title;

	s32 buttonw = environment->getSkin()->getSize(EGDS_WINDOW_BUTTON_WIDTH);
	s32 posx = RelativeRect.getWidth() - buttonw - 4;

	CloseButton = Environment->addButton(core::rectEx<s32>(posx, 3, posx + buttonw, 3 + buttonw), this, -1, GUI_ICON_WINDOW_CLOSE);
	CloseButton->setOverrideFont(Environment->getBuildInFont());
	CloseButton->grab();

	OKButton = Environment->addButton(core::rectEx<s32>(RelativeRect.getWidth()-80, 30, RelativeRect.getWidth()-10, 50), this, -1, L"OK");
	OKButton->grab();

	CancelButton = Environment->addButton(core::rectEx<s32>(RelativeRect.getWidth()-80, 55, RelativeRect.getWidth()-10, 75), this, -1, L"Cancel");
	CancelButton->grab();

	FileBox = Environment->addListBox(core::rectEx<s32>(10, 55, RelativeRect.getWidth()-90, 230), this);
	FileBox->grab();

	FileNameText = Environment->addStaticText(0, true, core::rectEx<s32>(10, 30, RelativeRect.getWidth()-90, 50), this);
	FileNameText->grab();

	if (FileSystem)
		FileSystem->grab();

	fillListBox();
}



//! destructor
CGUIFileOpenDialog::~CGUIFileOpenDialog()
{
	if (CloseButton)
		CloseButton->drop();

	if (OKButton)
		OKButton->drop();

	if (CancelButton)
		CancelButton->drop();

	if (FileBox)
		FileBox->drop();

	if (FileNameText)
		FileNameText->drop();

	if (FileSystem)
		FileSystem->drop();

	if (FileList)
		FileList->drop();
}


//! returns the filename of the selected file. Returns NULL, if no file was selected.
const wchar_t* CGUIFileOpenDialog::getFilename()
{
	return 0;
}



//! called if an event happened.
bool CGUIFileOpenDialog::OnEvent(SEvent event)
{
	switch(event.EventType)
	{
	case EET_GUI_EVENT:
		switch(event.GUIEvent.EventType)
		{
		case EGET_ELEMENT_MOUSE_FOCUS_LOST:
			Dragging = false;
			break;
		case EGET_BUTTON_CLICKED:
			if (event.GUIEvent.Caller == CloseButton)
			{
				remove();
				return true;
			}
			break;
		case EGET_LISTBOX_SELECTED_AGAIN:
			{
				s32 selected = FileBox->getSelected();
				if (FileList && FileSystem)
				{
					if (FileList->isDirectory(selected))
					{
						FileSystem->changeWorkingDirectoryTo(FileList->getFileName(selected));
						fillListBox();
					}

				}
			}
			break;
		}
		break;
	case EET_MOUSE_INPUT_EVENT:
		switch(event.MouseInput.Event)
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			DragStart.X = event.MouseInput.X;
			DragStart.Y = event.MouseInput.Y;
			Dragging = true;
			Environment->setMouseFocus(this);
			return true;
		case EMIE_LMOUSE_LEFT_UP:
			Dragging = false;
			Environment->removeMouseFocus(this);
			return true;
		case EMIE_MOUSE_MOVED:
			if (Dragging)
			{
				move(core::position2d<s32>(event.MouseInput.X - DragStart.X, event.MouseInput.Y - DragStart.Y));
				DragStart.X = event.MouseInput.X;
				DragStart.Y = event.MouseInput.Y;
				return true;
			}
			break;
		}
	}

	return Parent ? Parent->OnEvent(event) : false;
}


//! draws the element and its children
void CGUIFileOpenDialog::draw()
{
	if (!IsVisible)
		return;

	IGUISkin* skin = Environment->getSkin();
	irr::video::IVideoDriver* driver = Environment->getVideoDriver();

	core::rectEx<s32> rect = AbsoluteRect;

	// draw body fast
	
	rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + 1;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_HIGH_LIGHT), rect, &AbsoluteClippingRect);

	rect.LowerRightCorner.Y = AbsoluteRect.LowerRightCorner.Y;
	rect.LowerRightCorner.X = rect.UpperLeftCorner.X + 1;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_HIGH_LIGHT), rect, &AbsoluteClippingRect);

	rect.UpperLeftCorner.X = AbsoluteRect.LowerRightCorner.X - 1;
	rect.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X;
	rect.UpperLeftCorner.Y = AbsoluteRect.UpperLeftCorner.Y;
    rect.LowerRightCorner.Y = AbsoluteRect.LowerRightCorner.Y;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_DARK_SHADOW), rect, &AbsoluteClippingRect);

	rect.UpperLeftCorner.X -= 1;
	rect.LowerRightCorner.X -= 1;
	rect.UpperLeftCorner.Y += 1;
	rect.LowerRightCorner.Y -= 1;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_SHADOW), rect, &AbsoluteClippingRect);

	rect.UpperLeftCorner.X = AbsoluteRect.UpperLeftCorner.X;
	rect.UpperLeftCorner.Y = AbsoluteRect.LowerRightCorner.Y - 1;
	rect.LowerRightCorner.Y = AbsoluteRect.LowerRightCorner.Y;
	rect.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_DARK_SHADOW), rect, &AbsoluteClippingRect);

	rect.UpperLeftCorner.X += 1;
	rect.LowerRightCorner.X -= 1;
	rect.UpperLeftCorner.Y -= 1;
	rect.LowerRightCorner.Y -= 1;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_SHADOW), rect, &AbsoluteClippingRect);

	rect = AbsoluteRect;
	rect.UpperLeftCorner.X +=1;
	rect.UpperLeftCorner.Y +=1;
	rect.LowerRightCorner.X -= 2;
	rect.LowerRightCorner.Y -= 2;
	driver->draw2DRectangle(skin->getColor(EGDC_3D_FACE), rect, &AbsoluteClippingRect);

	// draw title bar

	rect = AbsoluteRect;
	rect.UpperLeftCorner.X += 2;
	rect.UpperLeftCorner.Y += 2;
	rect.LowerRightCorner.X -= 2;
	rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + skin->getSize(EGDS_WINDOW_BUTTON_WIDTH) + 2;
	driver->draw2DRectangle(skin->getColor(EGDC_ACTIVE_BORDER), rect, &AbsoluteClippingRect);

	if (Text.size())
	{
		rect.UpperLeftCorner.X += 2;
		rect.LowerRightCorner.X -= skin->getSize(EGDS_WINDOW_BUTTON_WIDTH) + 5;

		IGUIFont* font = skin->getFont();
		if (font)
			font->draw(Text.c_str(), rect, skin->getColor(EGDC_ACTIVE_CAPTION), false, true, &rect);
	}

	IGUIElement::draw();
}


//! fills the listbox with files.
void CGUIFileOpenDialog::fillListBox()
{
	if (!FileSystem || !FileBox)
		return;

	if (FileList)
		FileList->drop();

	FileBox->clear();

	FileList = FileSystem->createFileList();
	core::stringw s;

	for (s32 i=0; i<FileList->getFileCount(); ++i)
	{
		s = FileList->getFileName(i);
		FileBox->addItem(s.c_str(), FileList->isDirectory(i) ? GUI_ICON_DIRECTORY : GUI_ICON_FILE);
	}

	if (FileNameText)
	{
		s = FileSystem->getWorkingDirectory();
		FileNameText->setText(s.c_str());
	}
}

} // end namespace gui
} // end namespace irr
