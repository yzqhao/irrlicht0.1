// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __I_SURFACE_LOADER_H_INCLUDED__
#define __I_SURFACE_LOADER_H_INCLUDED__

#include "IUnknown.h"
#include "IReadFile.h"
#include "ISurface.h"

namespace irr
{
namespace video
{

/*!
	class which is able to create a surface from a file
*/
class ISurfaceLoader : public IUnknown
{
public:

	//! destructor
	virtual ~ISurfaceLoader() {};

	//! returns true if the file maybe is able to be loaded by this class
	//! based on the file extension (e.g. ".tga")
	virtual bool isALoadableFileExtension(const c8* fileName) = 0;

	//! returns true if the file maybe is able to be loaded by this class
	virtual bool isALoadableFileFormat(irr::io::IReadFile* file) = 0;

	//! creates a surface from the file
	virtual ISurface* loadImage(irr::io::IReadFile* file) = 0;
};


} // end namespace video
} // end namespace irr

#endif