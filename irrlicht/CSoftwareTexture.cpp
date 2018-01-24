#include "CSoftwareTexture.h"
#include "os.h"

namespace irr
{
namespace video  
{

//! constructor
CSoftwareTexture::CSoftwareTexture(ISurface* surface)
: Surface(surface), Texture(0)
{
	#ifdef _DEBUG
	setDebugName("CSoftwareTexture");
	#endif	

	if (Surface)
	{
		Surface->grab();

		core::dimension2d<s32> optSize;
		core::dimension2d<s32> origSize = Surface->getDimension();

		optSize.Width = getTextureSizeFromSurfaceSize(origSize.Width);
		optSize.Height = getTextureSizeFromSurfaceSize(origSize.Height);

		if (optSize != origSize)
		{
			Texture = createSurface(optSize);
			Surface->copyToScaling(Texture);
		}
		else
		{
			Texture = Surface;
			Texture->grab();
		}
	}
}



//! destructor
CSoftwareTexture::~CSoftwareTexture()
{
	if (Surface)
		Surface->drop();

	if (Texture)
		Texture->drop();
}



//! lock function
void* CSoftwareTexture::lock()
{
	return Surface->lock();
}



//! unlock function
void CSoftwareTexture::unlock()
{
	if (Surface != Texture)
	{
		#ifdef _DEBUG
		os::Debuginfo::print("Performance warning, slow unlock of non power of 2 texture.");
		#endif
		Surface->copyToScaling(Texture);
	}

	Surface->unlock();
}



//! returns dimension of texture (=size)
const core::dimension2d<s32>& CSoftwareTexture::getDimension()
{
	return Surface->getDimension();
}



//! returns unoptimized surface
ISurface* CSoftwareTexture::getSurface()
{
	return Surface;
}



//! returns texture surface
ISurface* CSoftwareTexture::getTexture()
{
	return Texture;
}



//! returns the size of a texture which would be the optimize size for rendering it
inline s32 CSoftwareTexture::getTextureSizeFromSurfaceSize(s32 size)
{
	s32 ts = 0x01;
	while(ts < size)
		ts <<= 1;

	return ts;
}



//! returns driver type of texture (=the driver, who created the texture)
EDriverType CSoftwareTexture::getDriverType()
{
	return DT_SOFTWARE;
}



//! returns color format of texture
ECOLOR_FORMAT CSoftwareTexture::getColorFormat()
{
	return EHCF_R5G5B5;
}



//! returns pitch of texture (in bytes)
s32 CSoftwareTexture::getPitch()
{
	return Surface->getDimension().Width * 2;
}



} // end namespace video
} // end namespace irr
