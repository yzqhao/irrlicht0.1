#include "CVideoOpenGL.h"
#include "COpenGLTexture.h"
#include "os.h"

namespace irr
{
namespace video
{


#ifdef WIN32
//! win32 constructor and init code
CVideoOpenGL::CVideoOpenGL(const core::dimension2d<s32>& screenSize, HWND window, bool fullscreen, io::IFileSystem* io)
: CVideoNull(io, screenSize), HDc(0), HRc(0), Window(window)
{
	#ifdef _DEBUG
	setDebugName("CVideoOpenGL");
	#endif
}

//! inits the open gl driver
bool CVideoOpenGL::initDriver(const core::dimension2d<s32>& screenSize, HWND window, bool fullscreen)
{
	static	PIXELFORMATDESCRIPTOR pfd =	{
		sizeof(PIXELFORMATDESCRIPTOR),	// Size Of This Pixel Format Descriptor
		1,							// Version Number
		PFD_DRAW_TO_WINDOW |		// Format Must Support Window
		PFD_SUPPORT_OPENGL |		// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,			// Must Support Double Buffering
		PFD_TYPE_RGBA,				// Request An RGBA Format
		16,							// Select Our Color Depth
		0, 0, 0, 0, 0, 0,			// Color Bits Ignored
		0,							// No Alpha Buffer
		0,							// Shift Bit Ignored
		0,							// No Accumulation Buffer
		0, 0, 0, 0,					// Accumulation Bits Ignored
		16,							// 16Bit Z-Buffer (Depth Buffer)
		0,							// No Stencil Buffer
		0,							// No Auxiliary Buffer
		PFD_MAIN_PLANE,				// Main Drawing Layer
		0,							// Reserved
		0, 0, 0						// Layer Masks Ignored
	};

	// get hdc
	if (!(HDc=GetDC(window)))
	{
		os::Warning::print("Cannot create a GL device context.");
		return false;
	}

	GLuint PixelFormat;

	// choose pixelformat
	if (!(PixelFormat = ChoosePixelFormat(HDc, &pfd)))
	{
		os::Warning::print("Cannot find a suitable pixelformat.");
		return false;
	}

	// set pixel format
	if(!SetPixelFormat(HDc, PixelFormat, &pfd))
	{
		os::Warning::print("Cannot set the pixel format.");
		return false;
	}

	// create rendering context
	if (!(HRc=wglCreateContext(HDc)))
	{
		os::Warning::print("Cannot create a GL rendering context.");
		return false;
	}

	// activate rendering context
	if(!wglMakeCurrent(HDc, HRc))
	{
		os::Warning::print("Cannot activate GL rendering context");
		return false;
	}

	glViewport(0, 0, screenSize.Width, screenSize.Height); // Reset The Current Viewport

	//glEnable(GL_TEXTURE_2D); // Enable Texture Mapping
	glShadeModel(GL_SMOOTH);				// Enable Smooth Shading

	glDisable(GL_LIGHTING);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	glClearDepth(1.0f); 
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LEQUAL);

	return true;
}

//! win32 destructor
CVideoOpenGL::~CVideoOpenGL()
{
	// I get a blue screen on my laptop, when I do not delete the
	// textures manually before releasing the dc. Oh how I love this.

	deleteAllTextures();

	if (HRc)
	{
		if (!wglMakeCurrent(0, 0))
			os::Warning::print("Release of dc and rc failed.");

		if (!wglDeleteContext(HRc))
			os::Warning::print("Release of rendering context failed.");

		HRc = 0;
	}

	if (HDc && !ReleaseDC(Window, HDc))
		os::Warning::print("Could not release opengl dc.");

	HDc = 0;
}
#endif


//! presents the rendered scene on the screen, returns false if failed
bool CVideoOpenGL::endScene()
{
	CVideoNull::endScene();

#ifdef WIN32
	return SwapBuffers(HDc) == TRUE;
#endif
}



//! clears the zbuffer
bool CVideoOpenGL::beginScene(bool backBuffer, bool zBuffer, Color color)
{
	CVideoNull::beginScene(backBuffer, zBuffer, color);

	GLbitfield mask = 0;

	if (backBuffer)
	{
		f32 inv = 1.0f / 255.0f;
		glClearColor(color.getRed() * inv, color.getGreen() * inv,
				color.getBlue() * inv, color.getAlpha() * inv);

		mask |= GL_COLOR_BUFFER_BIT;
	}

	if (zBuffer)
		mask |= GL_DEPTH_BUFFER_BIT;

	glClear(mask);
	return true;
}


//! sets transformation
void CVideoOpenGL::setTransform(E_TRANSFORMATION_STATE state, const core::matrix4& mat)
{
	GLfloat glmat[16];
	Matrizes[state] = mat;

	switch(state)
	{
	case TS_VIEW:
		// OpenGL only has a model matrix, view and world is not existent. so lets fake these two.
		createGLMatrix(glmat, Matrizes[TS_VIEW] * Matrizes[TS_WORLD]);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glmat);
		break;
	case TS_WORLD:
		// OpenGL only has a model matrix, view and world is not existent. so lets fake these two.
		createGLMatrix(glmat, Matrizes[TS_VIEW] * Matrizes[TS_WORLD]);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glmat);
		break;
	case TS_PROJECTION:
		{
			createGLMatrix(glmat, mat);
			
			// flip z to compensate OpenGLs right-hand coordinate system
			glmat[12]*= -1.0f;

			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(glmat);
		}
		break;
	}
}


//! draws an indexed triangle list
void CVideoOpenGL::drawIndexedTriangleList(const S3DVertex* vertices, s32 vertexCount, const u16* indexList, s32 triangleCount)
{
	CVideoNull::drawIndexedTriangleList(vertices, vertexCount, indexList, triangleCount);

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY );
	glEnableClientState(GL_NORMAL_ARRAY );

	// convert colors to gl color format.

	const S3DVertex* p = vertices;
	ColorBuffer.set_used(vertexCount);
	for (s32 i=0; i<vertexCount; ++i)
	{
		ColorBuffer[i] = p->Color.toOpenGLColor();
		++p;
	}

	// draw everything

	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(video::Color), &ColorBuffer[0]);
	glNormalPointer(GL_FLOAT, sizeof(S3DVertex), &vertices[0].Normal);
	glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex), &vertices[0].TCoords);
	glVertexPointer(3, GL_FLOAT, sizeof(S3DVertex),  &vertices[0].Pos);

	glDrawElements(GL_TRIANGLES, triangleCount * 3, GL_UNSIGNED_SHORT, indexList);

	glFlush();
}



//! draws an indexed triangle list
void CVideoOpenGL::drawIndexedTriangleList(const S3DVertex2TCoords* vertices, s32 vertexCount, const u16* indexList, s32 triangleCount)
{
	CVideoNull::drawIndexedTriangleList(vertices, vertexCount, indexList, triangleCount);

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY );
	glEnableClientState(GL_NORMAL_ARRAY );

	// convert colors to gl color format.

	const S3DVertex2TCoords* p = vertices;
	ColorBuffer.set_used(vertexCount);
	for (s32 i=0; i<vertexCount; ++i)
	{
		ColorBuffer[i] = p->Color.toOpenGLColor();
		++p;
	}

	// draw everything

	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(video::Color), &ColorBuffer[0]);
	glNormalPointer(GL_FLOAT, sizeof(S3DVertex2TCoords), &vertices[0].Normal);
	glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex2TCoords), &vertices[0].TCoords);
	glVertexPointer(3, GL_FLOAT, sizeof(S3DVertex2TCoords),  &vertices[0].Pos);

	glDrawElements(GL_TRIANGLES, triangleCount * 3, GL_UNSIGNED_SHORT, indexList);

	glFlush();
}



//! queries the features of the driver, returns true if feature is available
bool CVideoOpenGL::queryFeature(EK3D_VIDEO_DRIVER_FEATURE feature)
{
	// nothing implemented yet.

	switch (feature)
	{
	case EK3DVDF_BILINEAR_FILER:
		return false;
	case EK3DVDF_RENDER_TO_TARGET:
		return false;
	case EK3DVDF_HARDWARE_TL:
		return false;
	case EK3DVDF_MIP_MAP:
		return false;
	};

	return false;
}


//! returns a device dependent texture from a software surface (ISurface)
//! THIS METHOD HAS TO BE OVERRIDDEN BY DERIVED DRIVERS WITH OWN TEXTURES
video::ITexture* CVideoOpenGL::createDeviceDependentTexture(ISurface* surface, bool generateMipLevels)
{
	return new COpenGLTexture(surface, generateMipLevels);
}



//! Sets a material. All 3d drawing functions draw geometry now
//! using this material.
//! \param material: Material to be used from now on.
void CVideoOpenGL::setMaterial(const SMaterial& material)
{
	if (material.Texture1 == 0)
		glDisable(GL_TEXTURE_2D);
	else
	{
		if (material.Texture1->getDriverType() != DT_OPENGL)
		{
			glDisable(GL_TEXTURE_2D);
			os::Debuginfo::print("Fatal Error: Tried to set a texture not owned by this driver.");
			return;
		}

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,
			((COpenGLTexture*)material.Texture1)->getOpenGLTextureName());
	}
}



#ifdef WIN32
IVideoDriver* createOpenGLDriver(const core::dimension2d<s32>& screenSize, HWND window, bool fullscreen, io::IFileSystem* io)
{
	CVideoOpenGL* ogl =  new CVideoOpenGL(screenSize, window, fullscreen, io);
	if (!ogl->initDriver(screenSize, window, fullscreen))
	{
		ogl->drop();
		ogl = 0;
	}

	return ogl;
}
#endif

} // end namespace
} // end namespace