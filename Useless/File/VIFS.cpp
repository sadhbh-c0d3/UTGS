#include "UselessPch.h"

#include "VIFS.h"
#include "Useless/File/StdIFileSystem.h"
#include "Useless/File/SubIFileSystem.h"
#include "Useless/File/StdIFile.h"
#include "Useless/Util/Crypto.h"
#include "Useless/Error/IOError.h"
#include <algorithm>
#include "Useless/Util/Storage/Independent.h"
#include "Useless/Util/Math.h"

namespace Useless {

int  G_StdIFileSystemPriority = VIFS::STD;
bool G_MountStdIFileSystem = true;

INIT_SINGLETON( IFS, USELESS_API );
INIT_PREINITIALIZER( IFSAutoMounter, IFSAutoMounter, USELESS_API );

VIFS::VIFS():
    _std_min_max( STD, STD),
    _htc_min_max( HTC, HTC),
    _ref_min_max( REF, REF),
    _user_min_max( USER, USER)
{
    if ( G_MountStdIFileSystem )
    {
        _mounted.Insert( G_StdIFileSystemPriority, new StdIFileSystem );
    }
}

VIFS::~VIFS()
{
}

void VIFS::Mount( const std::string &hatchery_file, const std::string &base, int priority )
{
    _mounted.Insert( priority, OpenHatchery( hatchery_file, base ));

    _htc_min_max.first  = SimpleMin( priority, _htc_min_max.first);
    _htc_min_max.second = SimpleMax( priority, _htc_min_max.second);
}

void VIFS::Mount( const SPointer<IFileSystem> &fs, int priority )
{
    _mounted.Insert( priority, fs);

    _user_min_max.first  = SimpleMin( priority, _user_min_max.first);
    _user_min_max.second = SimpleMax( priority, _user_min_max.second);
}

void VIFS::SubMount( const std::string &real_path, const std::string &base, int priority )
{
    SPointer<IFileSystem> *p = _mounted.Find( STD );
    _mounted.Insert( priority, new SubIFileSystem(*p, real_path, base));

    _ref_min_max.first  = SimpleMin( priority, _ref_min_max.first);
    _ref_min_max.second = SimpleMax( priority, _ref_min_max.second);
}

int VIFS::MinPri( IFSType ifs_type ) const
{
    if ( ifs_type == STD ) { return _std_min_max.first; } else
    if ( ifs_type == HTC ) { return _htc_min_max.first; } else
    if ( ifs_type == REF ) { return _ref_min_max.first; } else
    return _user_min_max.first;
}

int VIFS::MaxPri( IFSType ifs_type ) const
{
    if ( ifs_type == STD ) { return _std_min_max.second; } else
    if ( ifs_type == HTC ) { return _htc_min_max.second; } else
    if ( ifs_type == REF ) { return _ref_min_max.second; } else
    return _user_min_max.second;
}


SPointer<IFile> VIFS::OpenFile( const std::string &orig_name )
{
    std::string name = orig_name;
    std::replace( name.begin(), name.end(), '\\', '/' );

    __FOR_EACH_USELESS( MountedFS, i, _mounted)
    {
        if ( (*i)->Exists( name) )
        {
            return (*i)->OpenFile( name);
        }
    }
    // try original name (allow SAMBA paths)
    __FOR_EACH_USELESS( MountedFS, i, _mounted)
    {
        if ( (*i)->Exists( orig_name ))
        {
            return (*i)->OpenFile( orig_name );
        }
    }
    throw IOError("VIFS: File '%s' was not found.\n"
                  "(*) vifs is allways case sensitive\n"
                  "(*) directory separator is '/' (not '\\')", name.c_str());
}

bool VIFS::Exists( const std::string &name )
{
    __FOR_EACH_USELESS( MountedFS, i, _mounted)
    {
        if ( (*i)->Exists( name) ) { return true; }
    }
    return false;
}

bool VIFS::IsDir( const std::string &name )
{
    __FOR_EACH_USELESS( MountedFS, i, _mounted)
    {
        if ( (*i)->Exists(name) )
        {
            if ( (*i)->IsDir(name) ) { return true; }
            return false;
        }
    }
    return false;
}

void VIFS::ScanDir( ScanCBType cb, const std::string &name, bool recursive )
{
    __FOR_EACH_USELESS( MountedFS, i, _mounted)
    {
        if ( (*i)->Exists(name) )
        {
            (*i)->ScanDir( cb, name, recursive);
        }
    }
}

int VIFS::ChangePriority( int initialPriority, int desiredPriority )
{
    if ( initialPriority == STD )
    {
        throw Error("StdIFileSystem priority must not be changed !!!");
    }

    MountedFS newVIFS;
    int count = 0;
    
    __FOR_EACH_USELESS( MountedFS, i, _mounted)
    {
        if ( i.Key() == initialPriority )
        {
            newVIFS.Insert( desiredPriority, i.Value() );
            ++count;
        }
        else
        {
            newVIFS.Insert( i.Key(), i.Value() );
        }
    }

    _mounted = newVIFS;
    return count;
}

int VIFS::Umount( int atPriority )
{
    if ( atPriority == STD )
    {
        throw Error("StdIFileSystem must not be Umount-ed !!!");
    }

    MountedFS newVIFS;
    int count = 0;
    
    __FOR_EACH_USELESS( MountedFS, i, _mounted)
    {
        if ( i.Key() == atPriority )
        {
            ++count;
        }
        else
        {
            newVIFS.Insert( i.Key(), i.Value() );
        }
    }

    _mounted = newVIFS;
    return count;
}

};//namespace Useless
