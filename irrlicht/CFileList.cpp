#include "CFileList.h"

namespace irr
{
namespace io
{

// TODO: Code a Linux version

#ifdef WIN32
#include <stdio.h>
#include <io.h>
#endif

CFileList::CFileList()
{
	#ifdef WIN32
	struct _finddata_t c_file;
	s32 hFile;
	FileEntry entry;

	if( (hFile = _findfirst( "*", &c_file )) != -1L )
	{
		do
		{
			entry.Name = c_file.name;
			entry.Size = c_file.size;
			entry.isDirectory = (_A_SUBDIR & c_file.attrib) != 0;
			Files.push_back(entry);
		}
		while( _findnext( hFile, &c_file ) == 0 );

		_findclose( hFile );
	}

	//TODO add drives
	//entry.Name = "E:\\";
	//entry.isDirectory = true;
	//Files.push_back(entry);
	#endif

}


CFileList::~CFileList()
{
}


s32 CFileList::getFileCount()
{
	return Files.size();
}


const c8* CFileList::getFileName(s32 index)
{
	if (index < 0 || index > (s32)Files.size())
		return 0;

	return Files[index].Name.c_str();
}


bool CFileList::isDirectory(s32 index)
{
	if (index < 0 || index > (s32)Files.size())
		return false;

	return Files[index].isDirectory;
}

} // end namespace irr
} // end namespace io
