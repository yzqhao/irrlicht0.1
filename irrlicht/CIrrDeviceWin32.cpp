// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifdef WIN32

#include "CIrrDeviceWin32.h"
#include "IEventReceiver.h"
#include "list.h"
#include "os.h"
#include <cstdio>

using namespace std;


namespace irr
{
	namespace io
	{
		//! creates a filesystem which is able to open files from the ordinary file system,
		//! and out of zipfiles, which are able to be added to the filesystem.
		IFileSystem* createFileSystem();
	}

	namespace gui
	{
		IGUIEnvironment* createGUIEnvironment(io::IFileSystem* fs, video::IVideoDriver* Driver, IEventReceiver* userReceiver);
	}


	namespace video
	{
		IVideoDriver* createSoftwareDriver(const core::dimension2d<s32>& windowSize, bool fullscreen, io::IFileSystem* io, video::ISurfacePresenter* presenter);
		IVideoDriver* createDirectX8Driver(const core::dimension2d<s32>& screenSize, HWND window, u32 bits, bool fullscreen, io::IFileSystem* io, bool pureSoftware);
		IVideoDriver* createNullDriver(io::IFileSystem* io, const core::dimension2d<s32>& screenSize);
		IVideoDriver* createOpenGLDriver(const core::dimension2d<s32>& screenSize, HWND window, bool fullscreen, io::IFileSystem* io);
	}


} // end namespace irr



struct SEnvMapper
{
	HWND hWnd;
	irr::gui::IGUIEnvironment* Env;
};

irr::core::list<SEnvMapper> EnvMap;

irr::gui::IGUIEnvironment* getEnvironmentFromHWnd(HWND hWnd)
{
	irr::core::list<SEnvMapper>::Iterator it = EnvMap.begin();
	for (; it!= EnvMap.end(); ++it)
		if ((*it).hWnd == hWnd)
			return (*it).Env;

	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	irr::gui::IGUIEnvironment* Env = 0;
	irr::SEvent event;

	switch (message) 
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		return 0;

	case WM_ERASEBKGND:
		return 0;

	case WM_LBUTTONDOWN:
        event.EventType = irr::EET_MOUSE_INPUT_EVENT;
		event.MouseInput.Event = irr::EMIE_LMOUSE_PRESSED_DOWN;
		event.MouseInput.X = LOWORD(lParam);
		event.MouseInput.Y = HIWORD(lParam);
		Env = getEnvironmentFromHWnd(hWnd);
		if (Env)
			Env->postEventFromUser(event);
		return 0;

	case WM_LBUTTONUP:
		event.EventType = irr::EET_MOUSE_INPUT_EVENT;
		event.MouseInput.Event = irr::EMIE_LMOUSE_LEFT_UP;
		event.MouseInput.X = LOWORD(lParam);
		event.MouseInput.Y = HIWORD(lParam);
		Env = getEnvironmentFromHWnd(hWnd);
		if (Env)
			Env->postEventFromUser(event);
		return 0;

	case WM_RBUTTONDOWN:
		event.EventType = irr::EET_MOUSE_INPUT_EVENT;
		event.MouseInput.Event = irr::EMIE_RMOUSE_PRESSED_DOWN;
		event.MouseInput.X = LOWORD(lParam);
		event.MouseInput.Y = HIWORD(lParam);
		Env = getEnvironmentFromHWnd(hWnd);
		if (Env)
			Env->postEventFromUser(event);
		return 0;

	case WM_RBUTTONUP:
		event.EventType = irr::EET_MOUSE_INPUT_EVENT;
		event.MouseInput.Event = irr::EMIE_RMOUSE_LEFT_UP;
		event.MouseInput.X = LOWORD(lParam);
		event.MouseInput.Y = HIWORD(lParam);
		Env = getEnvironmentFromHWnd(hWnd);
		if (Env)
			Env->postEventFromUser(event);
		return 0;

	case WM_MBUTTONDOWN:
		event.EventType = irr::EET_MOUSE_INPUT_EVENT;
		event.MouseInput.Event = irr::EMIE_MMOUSE_PRESSED_DOWN;
		event.MouseInput.X = LOWORD(lParam);
		event.MouseInput.Y = HIWORD(lParam);
		Env = getEnvironmentFromHWnd(hWnd);
		if (Env)
			Env->postEventFromUser(event);
		return 0;

	case WM_MBUTTONUP:
		event.EventType = irr::EET_MOUSE_INPUT_EVENT;
		event.MouseInput.Event = irr::EMIE_MMOUSE_LEFT_UP;
		event.MouseInput.X = LOWORD(lParam);
		event.MouseInput.Y = HIWORD(lParam);
		Env = getEnvironmentFromHWnd(hWnd);
		if (Env)
			Env->postEventFromUser(event);
		return 0;

	case WM_MOUSEMOVE:
		event.EventType = irr::EET_MOUSE_INPUT_EVENT;
		event.MouseInput.Event = irr::EMIE_MOUSE_MOVED;
		event.MouseInput.X = LOWORD(lParam);
		event.MouseInput.Y = HIWORD(lParam);
		Env = getEnvironmentFromHWnd(hWnd);
		if (Env)
			Env->postEventFromUser(event);
		return 0;

	case WM_KEYDOWN:
		event.EventType = irr::EET_KEY_INPUT_EVENT;
		event.KeyInput.Key = (irr::s32)wParam;
		event.KeyInput.PressedDown = true;
		Env = getEnvironmentFromHWnd(hWnd);
		if (Env)
			Env->postEventFromUser(event);
		return 0;

	case WM_KEYUP:
		event.EventType = irr::EET_KEY_INPUT_EVENT;
		event.KeyInput.Key = (irr::s32)wParam;
		event.KeyInput.PressedDown = false;
		Env = getEnvironmentFromHWnd(hWnd);
		if (Env)
			Env->postEventFromUser(event);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SYSCOMMAND:
		// prevent screensaver or monitor powersave mode from starting
		if (wParam == SC_SCREENSAVE ||
			wParam == SC_MONITORPOWER)
			return 0;
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

namespace irr
{

//! constructor
CIrrDeviceWin32::CIrrDeviceWin32(video::EDriverType driverType, 
								 const core::dimension2d<s32>& windowSize,
								 u32 bits, bool fullscreen, IEventReceiver* receiver)
: VideoDriver(0), SceneManager(0), HWnd(0), ChangedToFullScreen(false)
{
	// create filesystem
	FileSystem = io::createFileSystem();

	// create window

	HINSTANCE hInstance = GetModuleHandle(0);

	#ifdef _DEBUG
	setDebugName("CIrrDeviceWin32");	
	#endif

	if (driverType != video::DT_NULL)
	{
		// create the window, only if we do not use the null device

		const c8* ClassName = "CIrrDeviceWin32";

		// Register Class

		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX); 
		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= (WNDPROC)WndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= hInstance;
		wcex.hIcon			= NULL;
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= 0;
		wcex.lpszClassName	= ClassName;
		wcex.hIconSm		= 0;

		RegisterClassEx(&wcex);

		// calculate client size

		RECT clientSize;
		clientSize.top = 0;
		clientSize.left = 0;
		clientSize.right = windowSize.Width;
		clientSize.bottom = windowSize.Height;
		
		DWORD style = WS_POPUP;

		if (!fullscreen)
			style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;			

		AdjustWindowRect(&clientSize, style, FALSE);

		const s32 WIDTH = clientSize.right - clientSize.left;
		const s32 HEIGHT = clientSize.bottom - clientSize.top;

		s32 WINDOW_LEFT = (GetSystemMetrics(SM_CXSCREEN) - WIDTH) / 2;
		s32 WINDOW_TOP = (GetSystemMetrics(SM_CYSCREEN) - HEIGHT) / 2;

		if (fullscreen)
		{
			WINDOW_LEFT = 0;
			WINDOW_TOP = 0;
		}

		// create window

		HWnd = CreateWindow( ClassName, "", style, WINDOW_LEFT, WINDOW_TOP, 
					WIDTH, HEIGHT,	NULL, NULL, hInstance, NULL);

		ShowWindow(HWnd , SW_SHOW);
		UpdateWindow(HWnd);
	}

	// create driver

	createDriver(driverType, windowSize, bits, fullscreen);

	// create gui environment

	GUIEnvironment = gui::createGUIEnvironment(FileSystem, VideoDriver, receiver);

	// create Scene manager

	SceneManager = scene::createSceneManager(VideoDriver, FileSystem);

	// register environment

	SEnvMapper em;
	em.Env = GUIEnvironment;
	em.hWnd = HWnd;
	EnvMap.push_back(em);

}



//! destructor
CIrrDeviceWin32::~CIrrDeviceWin32()
{
	FileSystem->drop();

	if (GUIEnvironment)
		GUIEnvironment->drop();

	if (VideoDriver)
		VideoDriver->drop();

	if (SceneManager)
		SceneManager->drop();

	// unregister environment

	irr::core::list<SEnvMapper>::Iterator it = EnvMap.begin();
	for (; it!= EnvMap.end(); ++it)
		if ((*it).hWnd == HWnd)
		{
			EnvMap.erase(it);
			break;
		}

	if (ChangedToFullScreen)
		ChangeDisplaySettings(NULL,0);
}



//! create the driver
void CIrrDeviceWin32::createDriver(video::EDriverType driverType,
								   const core::dimension2d<s32>& windowSize,
								   u32 bits, 
								   bool fullscreen)
{
	switch(driverType)
	{
	case video::DT_DIRECTX8:
		VideoDriver = video::createDirectX8Driver(windowSize, HWnd, bits, fullscreen, FileSystem, false);
		break;
	case video::DT_SOFTWARE:
		if (fullscreen)	switchToFullScreen(windowSize.Width, windowSize.Height, bits);
		VideoDriver = video::createSoftwareDriver(windowSize, fullscreen, FileSystem, this);
		break;
	case video::DT_OPENGL:
		if (fullscreen)	switchToFullScreen(windowSize.Width, windowSize.Height, bits);
		VideoDriver = video::createOpenGLDriver(windowSize, HWnd, fullscreen, FileSystem);
		break;
	default:
		// create null driver if a bad programmer made a mistake
		VideoDriver = video::createNullDriver(FileSystem, windowSize);
		break;
	}
}



//! runs the device. Returns false if device wants to be deleted
bool CIrrDeviceWin32::run()
{
	MSG msg;

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (msg.message != WM_QUIT);
}



//! sets the caption of the window
void CIrrDeviceWin32::setWindowCaption(const wchar_t* text)
{
	SetWindowTextW(HWnd, text);
}



//! returns the video driver
video::IVideoDriver* CIrrDeviceWin32::getVideoDriver()
{
	return VideoDriver;
}



//! return file system
io::IFileSystem* CIrrDeviceWin32::getFileSystem()
{
	return FileSystem;
}



//! returns the gui environment
gui::IGUIEnvironment* CIrrDeviceWin32::getGUIEnvironment()
{
	return GUIEnvironment;
}



//! returns the scene manager
scene::ISceneManager* CIrrDeviceWin32::getSceneManager()
{
	return SceneManager;
}



//! presents a surface in the client area
void CIrrDeviceWin32::present(video::ISurface* surface)
{
	HDC dc = GetDC(HWnd);
	RECT rect;
	GetClientRect(HWnd, &rect);

	s16* memory = surface->lock();
			
	BITMAPV4HEADER bi;
	ZeroMemory (&bi, sizeof(bi));
	bi.bV4Size          = sizeof(BITMAPINFOHEADER);
	bi.bV4BitCount      = 16;
	bi.bV4Planes        = 1;
	bi.bV4Width         = surface->getDimension().Width;
	bi.bV4Height        = -surface->getDimension().Height;
	bi.bV4V4Compression = BI_BITFIELDS;
	bi.bV4AlphaMask     = surface->getAlphaMask();
	bi.bV4RedMask       = surface->getRedMask();
	bi.bV4GreenMask     = surface->getGreenMask();
	bi.bV4BlueMask      = surface->getBlueMask();

	StretchDIBits(dc, 0,0, rect.right, rect.bottom,
						0, 0, surface->getDimension().Width, surface->getDimension().Height,
						memory, (const BITMAPINFO*)(&bi), DIB_RGB_COLORS, SRCCOPY);

	surface->unlock();

	ReleaseDC(HWnd, dc);
}



//! notifies the device that it should close itself
void CIrrDeviceWin32::closeDevice()
{
	DestroyWindow(HWnd);
	//PostQuitMessage(0);
}



//! returns if window is active. if not, nothing need to be drawn
bool CIrrDeviceWin32::isWindowActive()
{
	return (GetActiveWindow() == HWnd);
}



//! switchs to fullscreen
bool CIrrDeviceWin32::switchToFullScreen(s32 width, s32 height, s32 bits)
{
	DEVMODE dm;
	memset(&dm, 0, sizeof(dm));
	dm.dmSize = sizeof(dm);
	dm.dmPelsWidth = width;
	dm.dmPelsHeight	= height;
	dm.dmBitsPerPel	= bits;
	dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	LONG ret = ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
	
	switch(ret)
	{
	case DISP_CHANGE_SUCCESSFUL:
		ChangedToFullScreen = true;
		return true;
	case DISP_CHANGE_RESTART:
		os::Warning::print("Switch to fullscreen: The computer must be restarted in order for the graphics mode to work.");
		return false;
	case DISP_CHANGE_BADFLAGS:
		os::Warning::print("Switch to fullscreen: An invalid set of flags was passed in.");
		return false;
	case DISP_CHANGE_BADPARAM:
		os::Warning::print("Switch to fullscreen: An invalid parameter was passed in. This can include an invalid flag or combination of flags.");
		return false;
	case DISP_CHANGE_FAILED:
		os::Warning::print("Switch to fullscreen: The display driver failed the specified graphics mode.");
		return false;
	case DISP_CHANGE_BADMODE:
		os::Warning::print("Switch to fullscreen: The graphics mode is not supported.");
		return false;
	}

	os::Warning::print("An unknown error occured while changing to fullscreen.");
	return false;
}



#ifdef IRRLICHT_EXPORTS
#define IRRLICHT_API __declspec(dllexport)
#else
#define IRRLICHT_API __declspec(dllimport)
#endif

IRRLICHT_API IrrlichtDevice* createDevice(video::EDriverType driverType,
										  const core::dimension2d<s32>& windowSize,
										  u32 bits, bool fullscreen, IEventReceiver* res)
{
	return new CIrrDeviceWin32(driverType, windowSize, bits, fullscreen, res);
}


} // end namespace 

#endif