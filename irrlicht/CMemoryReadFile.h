// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_MEMORY_READ_FILE_H_INCLUDED__
#define __C_MEMORY_READ_FILE_H_INCLUDED__

#include "IReadFile.h"
#include "irrstring.h"

namespace irr
{

namespace io
{

	/*!
		Class for reading from memory.
	*/
	class CMemoryReadFile : public IReadFile
	{
	public:

		CMemoryReadFile(void* memory, s32 len, const c8* fileName, bool deleteMemoryWhenDropped);

		virtual ~CMemoryReadFile();

		//! returns how much was read
		virtual size_t read(void* buffer, size_t sizeToRead);

		//! changes position in file, returns true if successful
		//! if relativeMovement==true, the pos is changed relative to current pos,
		//! otherwise from begin of file
		virtual bool seek(s32 finalPos, bool relativeMovement = false);

		//! returns size of file
		virtual size_t getSize();

		//! returns where in the file we are.
		virtual s32 getPos();

		//! returns name of file
		virtual const c8* getFileName();

	private:

		core::stringc Filename;
		void *Buffer;
		u32 Len;
		u32 Pos;
		bool deleteMemoryWhenDropped;
	};

} // end namespace io
} // end namespace irr

#endif