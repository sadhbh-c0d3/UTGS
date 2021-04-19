/*
**  $Id: StdIFileSystem.cpp,v 1.10 2005/01/05 19:18:46 koolas Exp $
**
**  NAME
**      StdIFileSystem
**
**  PURPOSE
**      Standard file system input access class.
**
**  AUTHOR(S)
**      Jacek Trzmiel (jacekt@iavision.com)
*/

#include "UselessPch.h"

#include "Useless/File/StdIFileSystem.h"
#include "Useless/File/StdIFile.h"
#include "Useless/Error/IOError.h"
#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
#   include <unistd.h>
#endif

namespace Useless {



SPointer<IFile> StdIFileSystem::OpenFile( const std::string &name )
{
    return new StdIFile( name );
}



// Windows version
#ifdef WIN32
bool StdIFileSystem::Exists( const std::string &name )
{
    if ( name == "-" )
    {
        return true; //stdin
    }
    struct _stat buf;
    int result = _stat( name.c_str(), &buf );
    return !result;
}



bool StdIFileSystem::IsDir( const std::string &name )
{
    struct _stat buf;
    int result = _stat( name.c_str(), &buf );
    if(result != 0) { throw IOError("File or directory '%s' not found.",name.c_str()); }
    return (buf.st_mode & _S_IFDIR);
}

char op_convert(TCHAR tc)
{
    return (char)tc;
}

void StdIFileSystem::ScanDir( ScanCBType cb, const std::string &name, bool recursive )
{
    std::string findname;
    // build findname from name for FindFirstFile function
    if((name.length()>2) && (name.substr(name.length()-2,2)==":\\")) { findname = name + "*"; }
    else if((name.length()>=1) && (name.substr(name.length()-1,1)=="\\")) { findname = name + "*"; }
    else if(name=="") { findname = "*"; }
    else { findname = name + "\\*"; }

    WIN32_FIND_DATA finddata;
    
    std::basic_string<TCHAR> findNameTC;
    std::copy( findname.begin(), findname.end(),
            std::back_inserter( findNameTC));
        
    HANDLE h = FindFirstFile( findNameTC.c_str(), &finddata );
    if(h == INVALID_HANDLE_VALUE) { throw IOError("Failed to list '%s' directory.",name.c_str()); }
    try {
        do {
            using Types::QUAD;
            TCHAR *findName = finddata.cFileName;
#ifdef UNICODE
            size_t nameLen = wcslen(finddata.cFileName);
#else
			size_t nameLen = strlen(finddata.cFileName);
#endif
            std::string foundname;
            std::transform( findName, findName+nameLen,
                     std::back_inserter(foundname), &op_convert);
            
            if((foundname!=".") && (foundname!=".."))
            {
                if((name.length()>0) && (name[name.length()-1]!='\\')) { foundname = name + "/" + foundname; }
                else { foundname = name + foundname; }
                QUAD size = (QUAD(finddata.nFileSizeHigh)<<16<<16) + finddata.nFileSizeLow;
                bool dir = finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
                (*cb)( foundname, size, dir );
                if(recursive && dir)
                {
                    ScanDir( cb, foundname, recursive );
                }
            }
        } while( FindNextFile( h, &finddata ) );

        if(GetLastError()!=ERROR_NO_MORE_FILES) { throw IOError("Failed to list '%s' directory.",name.c_str()); }
    } catch(...) {
        FindClose(h);
        throw;
    }
    FindClose(h);
}
// GCC version should come here
#else
bool StdIFileSystem::Exists( const std::string &name )
{
    struct stat buf;
    int result = stat( name.c_str(), &buf );
    return !result;
}



bool StdIFileSystem::IsDir( const std::string &name )
{
    struct stat buf;
    int result = stat( name.c_str(), &buf );
    if(result != 0) { throw IOError("File or directory '%s' not found.",name.c_str()); }
    return S_ISDIR(buf.st_mode);
}

void StdIFileSystem::ScanDir( ScanCBType cb, const std::string &name, bool recursive )
{
}
#endif


} // namespace Useless
