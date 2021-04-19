#include "UselessPch.h"

#pragma warning(disable:4786)
#include "Hatchery.h"
#include "Useless/File/StdIFile.h"
#include "Useless/Util/Math.h"
#include "Useless/Error/Error.h"
#include "Useless/Error/ErrorMessage.h"
#include "Useless/HatcheryConfig.h" // - hatchery configuration
#include <iostream>

namespace Useless {
    
    // The key to hatchery files
    const char *g_szKey = "aZkkRimvCgv5a$9!";

using namespace std;

const ArgAt<0> HatcheryBase::PATH;
const ArgAt<1> HatcheryBase::OFFSET;
const ArgAt<2> HatcheryBase::SIZE;


static std::string g_Key;

void Hatchery::SetGlobalKey( const std::string &key )
{
    g_Key = key;
    g_szKey = g_Key.c_str();
}

void Hatchery::SetKey( const std::string &key )
{
    _key = key;
}

void Hatchery::ResetKey()
{
    _key = g_szKey;
}

Hatchery::Hatchery(): _key( g_szKey )
{
}

Hatchery::Hatchery( const std::string &key ): _key( key )
{
}

void Hatchery::DumpHeader( std::ostream &out, const std::string &in_name )
{
    HatcheryCipher::In hatch( new StdIFile( in_name ), _key.c_str() );
    Types::QUAD position = HatcheryCipher::BLOCKSIZE;
    Types::QUAD baseoffset = hatch.GetLen();
    Types::QUAD lastoffset = 0;

    char cipherType[ HatcheryCipher::BLOCKSIZE ] = { 0, };
    hatch.Read( cipherType, HatcheryCipher::BLOCKSIZE );
    if ( std::string( cipherType ) != std::string( __cipherType ))
    {
        throw Error("Bad file signature (%c%c%c%c)."
                , tolower(__cipherType[0])
                , tolower(__cipherType[1])
                , tolower(__cipherType[2])
                , tolower(__cipherType[3]) );
    }

    for ( ; position + HatcheryCipher::BLOCKSIZE < baseoffset; )
    {
        IOHeader head;
        head.Load( hatch );
        
        std::string path = head[ PATH ].Get();
       
        Types::QUAD offset = head[ OFFSET ].Get();
        Types::QUAD size = head[ SIZE ].Get();

        out << "[File]" << std::endl;
        out << "path = " << path << std::endl;
        out << "offset = " << offset << std::endl;
        out << "size = " << size << std::endl;

        position += head.Size();
        baseoffset = SimpleMin( baseoffset, offset);
    }
}

void Hatchery::Open( const std::string &in_name, const std::string &base )
{
    HatcheryCipher::In hatch( new StdIFile( in_name ), _key.c_str() );
    Types::QUAD position = HatcheryCipher::BLOCKSIZE;
    Types::QUAD baseoffset = hatch.GetLen();
    Types::QUAD lastoffset = 0;

    char cipherType[ HatcheryCipher::BLOCKSIZE ] = { 0, };
    hatch.Read( cipherType, HatcheryCipher::BLOCKSIZE );
    if ( std::string( cipherType ) != std::string( __cipherType ))
    {
        throw Error("Bad file signature (%c%c%c%c)."
                , tolower(__cipherType[0])
                , tolower(__cipherType[1])
                , tolower(__cipherType[2])
                , tolower(__cipherType[3]) );
    }

    for ( ; position + HatcheryCipher::BLOCKSIZE < baseoffset; )
    {
        IOHeader head;
        head.Load( hatch );
        
        std::string path = head[ PATH ].Get();
       
        FileNode node;
        node.offset = head[ OFFSET ].Get();
        node.size = head[ SIZE ].Get();
        
        assert( base.empty() || *base.rbegin()!='/' );
        std::string full_path = (base.empty())? path : base+'/'+path;
        AddNode( full_path, node );

        position += head.Size();
        baseoffset = SimpleMin( baseoffset, node.offset);
        lastoffset = SimpleMax( lastoffset, node.offset);
    }

    _in_name = in_name;
}



void Hatchery::AddNode( const std::string &name, const FileNode &node )
{
    FileName fileName( name );
    pair< Iterator, bool > result = _nodes.insert( make_pair( fileName, node ));
    bool inserted = result.second;    
    if (! inserted) { throw Error("Filename conflict for: '%s'.", name.c_str() ); }
    Iterator prev = result.first;
    if ( prev != _nodes.begin())
    {
        --prev;
        std::string prevname = prev->first.GetSearchName() + '/';
        if( fileName.GetSearchName().substr( 0, prevname.size() ) == prevname )
        {
            _nodes.erase( prev );
        }
    }
}

SPointer<IFile> Hatchery::GainAccess()
{
    if (!_file)
    {
#       ifndef __ENCRYPT_DATA__
        _file = new StdIFile(_in_name);
#       else
        _file = new HatcheryCipher::In( new StdIFile( _in_name ), _key.c_str() );
#       endif
    }
    return _file;
}

void Hatchery::RevokeAccess()
{
    /*if ( _file.Unique() )
    {
        _file.Release();
    }*/
}

SPointer<IFile> Hatchery::OpenFile( const std::string &name )
{
    return new HatchedIFile( *this, name);
}

bool Hatchery::Exists( const std::string &name )
{
    FileName fileName( name );
    int nameLen = fileName.GetSearchName().size();
    Iterator i=_nodes.lower_bound( fileName ), iend=_nodes.end();
    if ( i==iend )
        { return false; }
    bool is = (*i).first.GetSearchName().substr( 0, nameLen) == fileName.GetSearchName();
    if ( is && (*i).first.Get() != name )
    {
        C_message( Hatchery, 0, ("Name case does not match", name.c_str(), (*i).first.Get().c_str() ));
    }
    return is;
}

bool Hatchery::IsDir( const std::string &name )
{
    FileName argName( name );
    Iterator i=_nodes.lower_bound( argName ), iend=_nodes.end();
    const FileNode &fileNode( (*i).second );
    const FileName &fileName( (*i).first );
    int argLen = argName.GetSearchName().size();
    
    if ( i!=iend && fileNode.size < 0 )
    {
        return true;
    }
    std::string cmpName1 = fileName.GetSearchName().substr(0, argLen);
    std::string cmpName2 = argName.GetSearchName();
    
    return ( cmpName1 == cmpName2 );
}

void Hatchery::ScanDir( ScanCBType cb, const std::string &name, bool recursive )
{
    assert( IsDir( name ));
    FileName argName( name );
    int argLen = argName.GetSearchName().size();
    assert( argLen == name.size() );
    Iterator i=_nodes.lower_bound( argName ), iend=_nodes.end();

    // for each entry which name begins from 'name'
    for ( ; i!=iend &&
            (*i).first.GetSearchName().substr( 0, argLen ) == argName.GetSearchName();
            ++i )
    {
        //FileName relativeName( (*i).first );
        //relativeName -= argName; // cut off base to create relative

        std::string rel_path = (*i).first.Get().substr( argLen, -1 );
        int name_slash = rel_path.rfind('/', rel_path.size() );
        if ( name_slash<1 || recursive )
        {
            std::string file_name = rel_path.substr( name_slash+1, -1);
            std::string dir_name = (*i).first.Get().substr(0, name.size()+name_slash);
            Types::QUAD size = (*i).second.size;
            
            if ( file_name.empty() ) // dir
                (*cb)( dir_name, size, true );
            else // regular file
                (*cb)( dir_name+'/'+file_name, size, false );
        }
    }
}

};//namespace Useless

