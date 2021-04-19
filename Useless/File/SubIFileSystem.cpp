#include "UselessPch.h"

#include "SubIFileSystem.h"
#include "Useless/Error/IOError.h"
#include <algorithm>

namespace Useless {

SubIFileSystem::SubIFileSystem(
    const SPointer<IFileSystem> &ifs,
    const std::string &cut_off,
    const std::string &base )
{
    _base = base;
    _cut_off = cut_off;
    _ifs = ifs;
}

SPointer<IFile> SubIFileSystem::OpenFile( const std::string &name )
{
    if ( RealName(name))
        return _ifs->OpenFile( _real_name);
    else
        throw IOError("File or directory '%s' not found.",name.c_str());
}   
    
bool SubIFileSystem::Exists( const std::string &name )
{
    if ( RealName(name))
        return _ifs->Exists( _real_name);
    else
        return false;
}

bool SubIFileSystem::IsDir( const std::string &name )
{
    if ( RealName(name))
        return _ifs->IsDir( _real_name);
    else
        return false;
}

void SubIFileSystem::ScanDir( ScanCBType cb,
                const std::string &parent_dir,
                 bool recursive )
{
    if ( RealName(parent_dir))
    {
        _cb = cb;
        _ifs->ScanDir( TernaryCall( this, &SubIFileSystem::FoundDir ), std::string(_real_name), recursive);
    }
    else
        throw IOError("File or directory '%s' not found.",parent_dir.c_str());
}

bool SubIFileSystem::RealName( const std::string &name)
{
    if ( 0 != name.find( _base))
    {   return false;
    }
    else
    {
        _real_name = name;
        _real_name -= _base;
        _real_name = _cut_off + _real_name;
        return true;
    }
}


void SubIFileSystem::FoundDir( const std::string &real_name, Types::QUAD size, bool is_dir )
{
    _real_name = real_name;
    _real_name -= _cut_off;
    _real_name = _base + _real_name;
    (*_cb)( _real_name, size, is_dir);
}

};//namespace Useless
