// Copyright (C) 2002-2003 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#include "CFileSystem.h"
#include "IReadFile.h"
#include "CZipReader.h"
#include "CFileList.h"
#include "stdio.h"
#include "os.h"

#ifdef WIN32
#include <direct.h> // for _chdir
#endif

namespace irr
{
namespace io
{


//! constructor
CFileSystem::CFileSystem()
{
	#ifdef _DEBUG
	setDebugName("CKFileSystem");
	#endif
}



//! destructor
CFileSystem::~CFileSystem()
{
	for (u32 i=0; i<ZipFileSystems.size(); ++i)
		ZipFileSystems[i]->drop();
}



//! opens a file for read access
IReadFile* CFileSystem::createAndOpenFile(const c8* filename)
{
	IReadFile* file = 0;

	for (u32 i=0; i<ZipFileSystems.size(); ++i)
	{
		file = ZipFileSystems[i]->openFile(filename);
		if (file)
			return file;
	}

	file = createReadFile(filename);
	return file;
}



//! adds an zip archive to the filesystem
bool CFileSystem::addZipFileArchive(const c8* filename, bool ignoreCase, bool ignorePaths)
{
	IReadFile* file = createReadFile(filename);
	if (file)
	{
		CZipReader* zr = new CZipReader(file, ignoreCase, ignorePaths);
		if (zr)
			ZipFileSystems.push_back(zr);

		file->drop();
		return zr != 0;
	}

	#ifdef _DEBUG
	os::Warning::print("Could not open file. Zipfile not added", filename);
	#endif
	return false;
}



//! Returns the string of the current working directory
const c8* CFileSystem::getWorkingDirectory()
{
#ifdef WIN32
	_getcwd(WorkingDirectory, FILE_SYSTEM_MAX_PATH);
	return WorkingDirectory;
#endif

	return 0;
}



//! Changes the current Working Directory to the string given.
//! The string is operating system dependent. Under Windows it will look
//! like this: "drive:\directory\sudirectory\"
//! \return
//! Returns true if successful, otherwise false.
bool CFileSystem::changeWorkingDirectoryTo(const c8* newDirectory)
{
#ifdef WIN32
	return (_chdir(newDirectory) == 0);
#endif
	return false;
}


//! Creates a list of files and directories in the current working directory 
IFileList* CFileSystem::createFileList()
{
	return new CFileList();
}


//! determinates if a file exists and would be able to be opened.
bool CFileSystem::existFile(const c8* filename)
{
	for (u32 i=0; i<ZipFileSystems.size(); ++i)
		if (ZipFileSystems[i]->findFile(filename)!=-1)
			return true;

	FILE* f = fopen(filename, "rb");
	if (f) 
	{
		fclose(f);
		return true;
	}

	return false;
}



//! creates a filesystem which is able to open files from the ordinary file system,
//! and out of zipfiles, which are able to be added to the filesystem.
IFileSystem* createFileSystem()
{
	return new CFileSystem();
}







} // end namespace irr
} // end namespace fs