#include "CSurfaceLoaderTGA.H"
#include <string.h>
#include "os.h"
#include "CColorConverter.h"

namespace irr
{
namespace video
{


//! constructor
CSurfaceLoaderTGA::CSurfaceLoaderTGA()
{
}



//! destructor
CSurfaceLoaderTGA::~CSurfaceLoaderTGA()
{
}



//! returns true if the file maybe is able to be loaded by this class
//! based on the file extension (e.g. ".tga")
bool CSurfaceLoaderTGA::isALoadableFileExtension(const c8* fileName)
{
	return strstr(fileName, ".tga") != 0;
}



//! returns true if the file maybe is able to be loaded by this class
bool CSurfaceLoaderTGA::isALoadableFileFormat(irr::io::IReadFile* file)
{
	if (!file)
		return false;

	u8 type[3];
	file->read(&type, sizeof(u8)*3);
	return (type[2]==2); // we currently only handle tgas of type 2.
}



//! creates a surface from the file
ISurface* CSurfaceLoaderTGA::loadImage(irr::io::IReadFile* file)
{
	STGAHeader header;

	file->seek(0);
	file->read(&header, sizeof(STGAHeader));

	if (header.ImageType != 2)
	{
		os::Warning::print("Unsupported TGA file type", file->getFileName());
		return 0; 
	}

	// skip image identifikation field
	if (header.IdLength)
		file->seek(header.IdLength, true);

	// skip color map data
	if (header.ColorMapType != 0)
	{
		// skip color map
		file->seek((header.ColorMapEntrySize/8 * header.ColorMapLength), true);
	}

	// read image

	s32 bytesPerPixel = header.PixelDepth/8;
	s32 imageSize = header.ImageHeight * header.ImageWidth * bytesPerPixel;
	c8* data = new c8[imageSize];

	file->read(data, imageSize);

	video::ISurface* surface = video::createSurface(
		core::dimension2d<s32>(header.ImageWidth, header.ImageHeight));

	switch(bytesPerPixel)
	{
	case 1: 
		os::Warning::print("Unsupported TGA format, 8 bit", file->getFileName());
		break;
	case 2:
		{
			s16* src = (s16*)((void*)data);
			s16* start = (s16*)((void*)surface->lock());
			s16* end = start + (header.ImageWidth * header.ImageHeight)-1;

			for (; end>start; --end, ++src)
				*end = *src;

			surface->unlock();
		}
		break;
	case 3:
		{
			CColorConverter::convert24BitTo16BitFlipMirror(
				data, surface->lock(), header.ImageWidth, header.ImageHeight, 0);
			surface->unlock();
		}
		break;
	case 4:
		{
			CColorConverter::convert32BitTo16BitFlipMirrorColorShuffle(
				data, surface->lock(), header.ImageWidth, header.ImageHeight, 0);
			surface->unlock();
		}		
		break;
	}

	delete [] data;

	return surface;
}



//! creates a loader which is able to load tgas
ISurfaceLoader* createSurfaceLoaderTGA()
{
	return new CSurfaceLoaderTGA;
}


} // end namespace video
} // end namespace irr