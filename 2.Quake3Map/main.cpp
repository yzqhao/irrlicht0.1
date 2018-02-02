
#include <irrlicht.h>
#include <stdio.h>

using namespace irr;

#pragma comment(lib, "Irrlicht.lib")

scene::ICameraSceneNode* camera = 0;

class MyEventReceiver : public IEventReceiver
{
public:
	virtual bool OnEvent(SEvent event)
	{
		if (camera)
			return camera->OnEvent(event);

		return false;
	}
};


/*
Ok, lets start. Again, we use the main() method as start, not the
WinMain(), because its shorter to write.
*/
void main()
{
	MyEventReceiver receiver;

	IrrlichtDevice *device =
		createDevice(video::DT_DIRECTX8, core::dimension2d<s32>(640, 480), 16, false, &receiver);

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	device->getFileSystem()->addZipFileArchive("..\\media\\map-20kdm2.pk3");

	scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
	scene::ISceneNode* node = 0;
	
	if (mesh)
		node = smgr->addOctTreeSceneNode(mesh->getMesh(0));

	if (node)
		node->setRelativePosition(core::vector3df(-1300,-144,-1249));

	camera = smgr->addCameraSceneNodeMaya();

	int lastFPS = -1;

	while(device->run())
	{
		driver->beginScene(true, true, video::Color(0,100,100,100));
		smgr->drawAll();
		driver->endScene();

		int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			wchar_t tmp[255];
			swprintf(tmp, L"Quake 3 Map Example - Irrlicht Engine (fps:%d) Triangles:%d", 
				fps, driver->getPrimitiveCountDrawed());

			device->setWindowCaption(tmp);
			lastFPS = fps;
		}
	}

	device->drop();
}

