#include "irrTypes.h"
#include "COpenGLTexture.h"
#include "os.h"
#include "CColorConverter.h"

namespace irr
{
namespace video  
{

//! constructor
COpenGLTexture::COpenGLTexture(ISurface* surface, bool generateMipLevels)
: Surface(surface), Pitch(0), SurfaceSize(0,0)
{
	#ifdef _DEBUG
    setDebugName("COpenGLTexture");
	#endif

	if (Surface)
	{
		Surface->grab();

		SurfaceSize = Surface->getDimension();

		glGenTextures(1, &textureName);

		copyTexture();
	}
}


//! destructor
COpenGLTexture::~COpenGLTexture()
{
	if (Surface)
		Surface->drop();

	glDeleteTextures(1, &textureName);
}


//! copies the the texture into an open gl texture.
void COpenGLTexture::copyTexture()
{
	core::dimension2d<s32> nSurfaceSize;
	nSurfaceSize.Width = getTextureSizeFromSurfaceSize(SurfaceSize.Width);
	nSurfaceSize.Height = getTextureSizeFromSurfaceSize(SurfaceSize.Height);

	if (!nSurfaceSize.Width || !nSurfaceSize.Height ||
		!SurfaceSize.Width || !SurfaceSize.Height)
	{
		os::Warning::print("Could not create OpenGL Texture.");
		return;
	}

	s32* imageData = new s32[nSurfaceSize.Width * nSurfaceSize.Height];

	glBindTexture(GL_TEXTURE_2D, textureName);

	CColorConverter::convert16bitToA8R8G8B8andResize(
		Surface->lock(), imageData, nSurfaceSize.Width, nSurfaceSize.Height,
		SurfaceSize.Width, SurfaceSize.Height);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, nSurfaceSize.Width, 
		nSurfaceSize.Height, 0, GL_BGRA_EXT , GL_UNSIGNED_BYTE, imageData);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	delete [] imageData;
}



//! returns the size of a texture which would be the optimize size for rendering it
inline s32 COpenGLTexture::getTextureSizeFromSurfaceSize(s32 size)
{
	s32 ts = 0x01;
	while(ts < size)
		ts <<= 1;

	return ts;
}


//! lock function
void* COpenGLTexture::lock()
{
	return Surface ? Surface->lock() : 0;
}



//! unlock function
void COpenGLTexture::unlock()
{
	if (Surface)
	{
		Surface->unlock();
		copyTexture();
	}
}



//! returns dimension of texture (=size)
const core::dimension2d<s32>& COpenGLTexture::getDimension()
{
	return SurfaceSize;
}




//! returns driver type of texture (=the driver, who created the texture)
EDriverType COpenGLTexture::getDriverType()
{
	return DT_OPENGL;
}



//! returns color format of texture
ECOLOR_FORMAT COpenGLTexture::getColorFormat()
{
	return EHCF_R5G5B5;
}



//! returns pitch of texture (in bytes)
s32 COpenGLTexture::getPitch()
{
	return Surface ? Surface->getDimension().Width * 2 : 0;
}



//! return open gl texture name
GLuint COpenGLTexture::getOpenGLTextureName()
{
	return textureName;
}




} // end namespace video
} // end namespace irr
