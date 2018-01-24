// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#include "CLimitReadFile.h"
#include "irrstring.h"

namespace irr
{
namespace io
{


CLimitReadFile::CLimitReadFile(IReadFile* alreadyOpenedFile, size_t areaSize, const c8* name)
: AreaSize(areaSize), File(alreadyOpenedFile), Filename(name)
{
	#ifdef _DEBUG
	setDebugName("CLimitReadFile");
	#endif

	if (File)
		File->grab();

	init();
}


void CLimitReadFile::init()
{
	if (!File)
		return;

	AreaStart = File->getPos();
	AreaEnd = AreaStart + AreaSize;
}



CLimitReadFile::~CLimitReadFile()
{
	if (File)
		File->drop();
}



//! returns how much was read
size_t CLimitReadFile::read(void* buffer, size_t sizeToRead)
{
	size_t pos = File->getPos();

	if (pos >= AreaEnd)
		return 0;

	if (pos + sizeToRead >= AreaEnd)
		sizeToRead = AreaEnd - pos;

	return File->read(buffer, sizeToRead);
}



//! changes position in file, returns true if successful
//! if relativeMovement==true, the pos is changed relative to current pos,
//! otherwise from begin of file
bool CLimitReadFile::seek(s32 finalPos, bool relativeMovement)
{
	size_t pos = File->getPos();

	if (relativeMovement)
	{
		if (pos + finalPos > AreaEnd)
			finalPos = AreaEnd - pos;
	}
	else
	{
		finalPos += AreaStart;
		if ((size_t)finalPos > AreaEnd)
			return false;
	}

	return File->seek(finalPos, relativeMovement);	
}



//! returns size of file
size_t CLimitReadFile::getSize()
{
	return AreaSize;
}



//! returns where in the file we are.
s32 CLimitReadFile::getPos()
{
	return File->getPos() - AreaStart;
}



//! returns name of file
const c8* CLimitReadFile::getFileName()
{
	return Filename.c_str();
}


IReadFile* createLimitReadFile(const c8* fileName, IReadFile* alreadyOpenedFile, size_t areaSize)
{
	return new CLimitReadFile(alreadyOpenedFile, areaSize, fileName);
}


} // end namespace io
} // end namespace irr
