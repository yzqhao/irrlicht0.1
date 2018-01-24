// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_FILE_SYSTEM_H_INCLUDED__
#define __C_FILE_SYSTEM_H_INCLUDED__

#include "IFileSystem.h"
#include "array.h"

namespace irr
{
namespace io
{

	class CZipReader;
	const s32 FILE_SYSTEM_MAX_PATH = 1024;

/*!
	FileSystem which uses normal files and one zipfile
*/
class CFileSystem : public IFileSystem
{
public:

	//! constructor
	CFileSystem();

	//! destructor
	virtual ~CFileSystem();

	//! opens a file for read access
	virtual IReadFile* createAndOpenFile(const c8* filename);

	//! adds an zip archive to the filesystem
	virtual bool addZipFileArchive(const c8* filename, bool ignoreCase = true, bool ignorePaths = true);

	//! Returns the string of the current working directory
	virtual const c8* getWorkingDirectory();

	//! Changes the current Working Directory to the string given.
	//! The string is operating system dependent. Under Windows it will look
	//! like this: "drive:\directory\sudirectory\"
	virtual bool changeWorkingDirectoryTo(const c8* newDirectory);

	//! Creates a list of files and directories in the current working directory 
	//! and returns it.
	virtual IFileList* createFileList();

	//! determinates if a file exists and would be able to be opened.
	virtual bool existFile(const c8* filename);

private:

	core::array<CZipReader*> ZipFileSystems;
	c8 WorkingDirectory[FILE_SYSTEM_MAX_PATH];
};



} // end namespace irr
} // end namespace io

#endif