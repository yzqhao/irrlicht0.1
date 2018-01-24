// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#include "CMemoryReadFile.h"
#include <memory.H>

namespace irr
{
namespace io
{


CMemoryReadFile::CMemoryReadFile(void* memory, s32 len, const c8* fileName, bool d)
: Buffer(memory), Len(len), Pos(0), deleteMemoryWhenDropped(d)
{
	#ifdef _DEBUG
	setDebugName("CReadFile");
	#endif

	Filename = fileName;
}



CMemoryReadFile::~CMemoryReadFile()
{
	if (deleteMemoryWhenDropped)
		delete [] Buffer;
}



//! returns how much was read
size_t CMemoryReadFile::read(void* buffer, size_t sizeToRead)
{
	size_t amount = sizeToRead;
	if (Pos + amount > Len)
		amount -= Pos + amount - Len;

	if (amount < 0)
		amount = 0;

	c8* p = (c8*)Buffer;
	memcpy(buffer, p + Pos, amount);
	
	Pos += static_cast<u32> ( amount );

	return amount;
}



//! changes position in file, returns true if successful
//! if relativeMovement==true, the pos is changed relative to current pos,
//! otherwise from begin of file
bool CMemoryReadFile::seek(s32 finalPos, bool relativeMovement)
{
	if (relativeMovement)
	{
		if (Pos + finalPos > Len)
			return false;

		Pos += finalPos;
	}
	else
	{
		if ( (unsigned) finalPos > Len)
			return false;
		
        Pos = finalPos;
	}

	return true;
}



//! returns size of file
size_t CMemoryReadFile::getSize()
{
	return Len;
}



//! returns where in the file we are.
s32 CMemoryReadFile::getPos()
{
	return Pos;
}



//! returns name of file
const c8* CMemoryReadFile::getFileName()
{
	return Filename.c_str();
}



IReadFile* createMemoryReadFile(void* memory, s32 size, const c8* fileName, bool deleteMemoryWhenDropped)
{
	CMemoryReadFile* file = new CMemoryReadFile(memory, size, fileName, deleteMemoryWhenDropped);
	return file;
}


} // end namespace io
} // end namespace klib