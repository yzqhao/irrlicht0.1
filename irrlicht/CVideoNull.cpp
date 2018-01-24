#include "CVideoNull.H"
#include "CSoftwareTexture.H"
#include "os.h"

namespace irr
{
namespace video
{

//! creates a loader which is able to load windows bitmaps
ISurfaceLoader* createSurfaceLoaderBmp();
//! creates a loader which is able to load jpeg images
ISurfaceLoader* createSurfaceLoaderJPG();
//! creates a loader which is able to load targa images
ISurfaceLoader* createSurfaceLoaderTGA();
//! creates a loader which is able to load psd images
ISurfaceLoader* createSurfaceLoaderPSD();


//! constructor
CVideoNull::CVideoNull(io::IFileSystem* io, const core::dimension2d<s32>& screenSize)
: ScreenSize(screenSize), ViewPort(0,0,0,0), FileSystem(io), PrimitivesDrawn(0)
{
	#ifdef _DEBUG
	setDebugName("CVideoNull");
	#endif

	ViewPort = core::rectEx<s32>(core::position2d<s32>(0,0), screenSize);

	if (FileSystem)
		FileSystem->grab();

	// create surface loader

	SurfaceLoader.push_back(video::createSurfaceLoaderBmp());
	SurfaceLoader.push_back(video::createSurfaceLoaderJPG());
	SurfaceLoader.push_back(video::createSurfaceLoaderTGA());
	SurfaceLoader.push_back(video::createSurfaceLoaderPSD());
}



//! destructor
CVideoNull::~CVideoNull()
{
	// delete file system 

	if (FileSystem)
		FileSystem->drop();

	// delete textures

	deleteAllTextures();

	// delete surface loader

	for (u32 i=0; i<SurfaceLoader.size(); ++i)
		SurfaceLoader[i]->drop();
}



//! deletes all textures
void CVideoNull::deleteAllTextures()
{
	for (u32 i=0; i<Textures.size(); ++i)
		Textures[i].Surface->drop();

	Textures.clear();
}



//! applications must call this method before performing any rendering. returns false if failed.
bool CVideoNull::beginScene(bool backBuffer, bool zBuffer, Color color)
{
	PrimitivesDrawn = 0;
	return true;
}



//! applications must call this method after performing any rendering. returns false if failed.
bool CVideoNull::endScene()
{
	FPSCounter.registerFrame(os::Timer::getTime());
	return true;
}



//! queries the features of the driver, returns true if feature is available
bool CVideoNull::queryFeature(EK3D_VIDEO_DRIVER_FEATURE feature)
{
	return false;
}



//! sets transformation
void CVideoNull::setTransform(E_TRANSFORMATION_STATE state, const core::matrix4& mat)
{
}



//! sets a material
void CVideoNull::setMaterial(const SMaterial& material)
{
}



//! loads a Texture
ITexture* CVideoNull::getTexture(const c8* filename, bool generateMipLevels)
{
	ITexture* texture = findTexture(filename);

	if (texture)
		return texture;

	io::IReadFile* file = FileSystem->createAndOpenFile(filename);

	if (file)
	{
		texture = loadTextureFromFile(file, generateMipLevels);
		file->drop();

		if (texture)
		{
			addTexture(texture, filename);
			texture->drop(); // drop it becaus we created it, one grab to much
		}
	}

	if (!texture)
		os::Warning::print("Could not load texture", filename);

	return texture;
}



//! loads a Texture
ITexture* CVideoNull::getTexture(io::IReadFile* file, bool generateMipLevels)
{
	ITexture* texture = 0;

	if (file)
	{
		texture = findTexture(file->getFileName());

		if (texture)
			return texture;

		texture = loadTextureFromFile(file, generateMipLevels);

		if (texture)
		{
			addTexture(texture, file->getFileName());
			texture->drop(); // drop it because we created it, one grab to much
		}
	}

	if (!texture)
		os::Warning::print("Could not load texture", file->getFileName());

	return texture;
}



//! opens the file and loads it into the surface
video::ITexture* CVideoNull::loadTextureFromFile(io::IReadFile* file, bool generateMipLevels)
{
	ITexture* texture = 0;
	ISurface* surface = 0;

	// try to load file based on file extension

	for (u32 i=0; i<SurfaceLoader.size(); ++i)
		if (SurfaceLoader[i]->isALoadableFileExtension(file->getFileName()))
		{
			surface = SurfaceLoader[i]->loadImage(file);
			if (surface)
				break;
		}

	// try to load file based on what is in it
	if (!surface)
		for (int i=0; i<SurfaceLoader.size(); ++i)
		{
			if (i!=0)
				file->seek(0);

			if (SurfaceLoader[i]->isALoadableFileFormat(file))
			{
				file->seek(0);
				surface = SurfaceLoader[i]->loadImage(file);
				if (surface)
					break;
			}
		}


	if (surface)
	{
		// create texture from surface

		#ifdef _DEBUG
		os::Debuginfo::print("Loaded texture", file->getFileName());
		#endif

		texture = createDeviceDependentTexture(surface, generateMipLevels);
		surface->drop();
	}

	return texture;
}



//! adds a surface, not loaded or created by the Irrlicht Engine
void CVideoNull::addTexture(video::ITexture* texture, const c8* filename)
{
	if (texture)
	{
		if (!filename)
			filename = "";

		SSurface s;
		s.Filename = filename;
		s.Filename.make_lower();
		s.Surface = texture;
		texture->grab();

		Textures.push_back(s);
	}
}



//! looks if the image is already loaded
video::ITexture* CVideoNull::findTexture(const c8* filename)
{
	SSurface s;
	if (!filename)
		filename = "";

	s.Filename = filename;
	s.Filename.make_lower();

	s32 index = Textures.binary_search(s);
	if (index != -1)
		return Textures[index].Surface;

	return 0;
}



//! creates a Texture
ITexture* CVideoNull::addTexture(const core::dimension2d<s32>& size, const c8* name)
{
	ISurface* surface = createSurface(size);
	ITexture* t = createDeviceDependentTexture(surface, false);
	surface->drop();
	addTexture(t, name);
	t->drop();
	return t;
}



//! returns a device dependent texture from a software surface (ISurface)
//! THIS METHOD HAS TO BE OVERRIDDEN BY DERIVED DRIVERS WITH OWN TEXTURES
ITexture* CVideoNull::createDeviceDependentTexture(ISurface* surface, bool generateMipLevels)
{
	return new CSoftwareTexture(surface);
}



//! sets a render target
void CVideoNull::setRenderTarget(video::ITexture* texture)
{
}



//! sets a viewport
void CVideoNull::setViewPort(const core::rectEx<s32>& area)
{
}



//! gets the area of the current viewport
const core::rectEx<s32>& CVideoNull::getViewPort() const
{
	return ViewPort;
}



//! draws an indexed triangle list
void CVideoNull::drawIndexedTriangleList(const S3DVertex* vertices, s32 vertexCount, const u16* indexList, s32 triangleCount)
{
	PrimitivesDrawn += triangleCount;
}



//! draws an indexed triangle list
void CVideoNull::drawIndexedTriangleList(const S3DVertex2TCoords* vertices, s32 vertexCount, const u16* indexList, s32 triangleCount)
{
	PrimitivesDrawn += triangleCount;
}



//! draws an 2d image
void CVideoNull::draw2DImage(video::ITexture* texture, const core::position2d<s32>& destPos)
{
}



//! draws an 2d image, using a color (if color is other then Color(255,255,255,255)) and the alpha channel of the texture if wanted.
void CVideoNull::draw2DImage(video::ITexture* texture, const core::position2d<s32>& destPos, const core::rectEx<s32>& sourceRect, const core::rectEx<s32>* clipRect, Color color, bool useAlphaChannelOfTexture)
{
}



//! draw an 2d rectangle
void CVideoNull::draw2DRectangle(Color color, const core::rectEx<s32>& pos, const core::rectEx<s32>* clip)
{
}



//! returns screen size
core::dimension2d<s32> CVideoNull::getScreenSize()
{
	return ScreenSize;
}



// returns current frames per second value
s32 CVideoNull::getFPS()
{
	return FPSCounter.getFPS();
}



//! returns amount of primitives (mostly triangles) were drawn in the last frame.
//! very useful method for statistics.
u32 CVideoNull::getPrimitiveCountDrawed()
{
	return PrimitivesDrawn;
}



//! deletes all dynamic lights there are
void CVideoNull::deleteAllDynamicLights()
{
}



//! adds a dynamic light
void CVideoNull::addDynamicLight(const SLight& light)
{
}



//! returns the maximal amount of dynamic lights the device can handle
s32 CVideoNull::getMaximalDynamicLightAmount()
{
	return 0;
}



//! creates a video driver
IVideoDriver* createNullDriver(io::IFileSystem* io, const core::dimension2d<s32>& screenSize)
{
	return new CVideoNull(io, screenSize);
}


} // end namespace
} // end namespace