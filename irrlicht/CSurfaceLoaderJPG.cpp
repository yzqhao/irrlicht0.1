#include "CSurfaceLoaderJPG.h"
#include <string.h>
#include "CColorConverter.h"

namespace irr
{
namespace video
{

//! constructor
CSurfaceLoaderJPG::CSurfaceLoaderJPG()
{
	#ifdef _DEBUG
	setDebugName("CSurfaceLoaderJPG");
	#endif
}



//! destructor
CSurfaceLoaderJPG::~CSurfaceLoaderJPG()
{
}



//! returns true if the file maybe is able to be loaded by this class
//! based on the file extension (e.g. ".tga")
bool CSurfaceLoaderJPG::isALoadableFileExtension(const c8* fileName)
{
	return strstr(fileName, ".jpg") != 0;
}



void CSurfaceLoaderJPG::init_source (j_decompress_ptr cinfo)
{
	// DO NOTHING
}



u8 CSurfaceLoaderJPG::fill_input_buffer (j_decompress_ptr cinfo)
{
	// DO NOTHING
	return 1;
}



void CSurfaceLoaderJPG::skip_input_data (j_decompress_ptr cinfo, long count)
{
	jpeg_source_mgr * src = cinfo->src;
	if(count > 0)
	{
		src->bytes_in_buffer -= count;
		src->next_input_byte += count;
	}
}



void CSurfaceLoaderJPG::resync_to_restart (j_decompress_ptr cinfo, long desired)
{
	// DO NOTHING
}



void CSurfaceLoaderJPG::term_source (j_decompress_ptr cinfo)
{
	// DO NOTHING
}


//! returns true if the file maybe is able to be loaded by this class
bool CSurfaceLoaderJPG::isALoadableFileFormat(irr::io::IReadFile* file)
{
	if (!file)
		return false;

	s32 jfif = 0;
	file->seek(6);
	file->read(&jfif, sizeof(s32));
	return (jfif == 'JFIF' || jfif == 'FIFJ');
}



//! creates a surface from the file
ISurface* CSurfaceLoaderJPG::loadImage(irr::io::IReadFile* file)
{
	file->seek(0);
	u8* input = new u8[file->getSize()];
	file->read(input, file->getSize());

	struct jpeg_decompress_struct cinfo;

	// allocate and initialize JPEG decompression object
	struct jpeg_error_mgr jerr;

	//We have to set up the error handler first, in case the initialization
	//step fails.  (Unlikely, but it could happen if you are out of memory.)
	//This routine fills in the contents of struct jerr, and returns jerr's
	//address which we place into the link field in cinfo.
	
	cinfo.err = jpeg_std_error(&jerr);
	// Now we can initialize the JPEG decompression object. 
	jpeg_create_decompress(&cinfo);

	// specify data source
	jpeg_source_mgr jsrc;

	// Set up data pointer
	jsrc.bytes_in_buffer = file->getSize();
	jsrc.next_input_byte = (JOCTET*)input;
	cinfo.src = &jsrc;

	jsrc.init_source = init_source;
	//jsrc.fill_input_buffer = (int)fill_input_buffer;  //±‡“Î≤ªπ˝£¨œ»…æµÙ
	jsrc.skip_input_data = skip_input_data;
	jsrc.resync_to_restart = jpeg_resync_to_restart;
	jsrc.term_source = term_source;

	// Decodes JPG input from whatever source
	// Does everything AFTER jpeg_create_decompress
	// and BEFORE jpeg_destroy_decompress
	// Caller is responsible for arranging these + setting up cinfo

	// read file parameters with jpeg_read_header() 
	(void) jpeg_read_header(&cinfo, TRUE);

	// Start decompressor
	(void) jpeg_start_decompress(&cinfo);

	// Get image data
	u16 rowspan = cinfo.image_width * cinfo.num_components;
	unsigned width = cinfo.image_width;
	unsigned height = cinfo.image_height;

	bool has_alpha= false;  //(JPEG never has alpha)
	bool greyscale;

	if (cinfo.jpeg_color_space == JCS_GRAYSCALE)
		greyscale = true;
	else
		greyscale = false;

	// Allocate memory for buffer
	u8 *output = new u8[rowspan * height];

	// Here we use the library's state variable cinfo.output_scanline as the
	// loop counter, so that we don't have to keep track ourselves.
	// Create array of row pointers for lib
	u8 **rowPtr = new u8 * [height];

	for( unsigned i = 0; i < height; i++ )
		rowPtr[i] = &output[ i * rowspan ];

	unsigned rowsRead = 0;

	while( cinfo.output_scanline < cinfo.output_height )
		rowsRead += jpeg_read_scanlines( &cinfo, &rowPtr[rowsRead], cinfo.output_height - rowsRead );

	delete [] rowPtr;
	// Finish decompression

	(void) jpeg_finish_decompress(&cinfo);

	// Release JPEG decompression object

	// This is an important step since it will release a good deal of memory.
	jpeg_destroy_decompress(&cinfo);


	// convert image
	
	video::ISurface* surface = video::createSurface(core::dimension2d<s32>(width, height));

	//CColorConverter::convert24BitTo16BitColorShuffle((c8*)((void*)output), surface->lock(), width, height, 0);
	CColorConverter::convert24BitTo16BitFlipColorShuffle((c8*)((void*)output),
		surface->lock(), width, height, 0);
	
	delete [] input;
	delete [] output;

	return surface;
}



//! creates a loader which is able to load jpeg images
ISurfaceLoader* createSurfaceLoaderJPG()
{
	return new CSurfaceLoaderJPG;
}

} // end namespace video
} // end namespace irr
