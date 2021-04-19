/*
**  $Id: SubIFile.cpp,v 1.7 2004/11/15 18:59:15 koolas Exp $
**
**  NAME
**      SubIFile
**
**  PURPOSE
**      Treat part of input file as separate logical file.
**
**  AUTHOR(S)
**      Jacek Trzmiel (jacekt@iavision.com)
*/

#include "UselessPch.h"

#include "Useless/File/SubIFile.h"
#include "Useless/Util/Math.h"

#include <limits>
#ifdef WIN32
#   undef min
#   undef max
#else
    template< typename T > struct numeric_limits;
    struct numeric_limits<int>
    {
        static int max() { return 0x7FFFFFFF;}
        static int min() { return 0x80000000;}
    };
#endif

namespace Useless {


SubIFile::SubIFile(): _baseoffset(0), _curoffset(0), _subsize(0)
{}

SubIFile::SubIFile( SPointer<IFile> file, const std::string &name, Types::QUAD baseoffset, Types::QUAD subsize )
    : _file(file), _name(name), _baseoffset(baseoffset), _subsize(subsize)
{
    assert(!! _file);
    assert(_baseoffset >= 0);
    assert(_subsize >= 0);
    assert(_file->GetLen() >= _baseoffset+_subsize );

    _curoffset = _baseoffset;
    _file->Seek( _curoffset, SEEKSET );
}



int SubIFile::ReadUpto( void *buffer, int size )
{
    //Types::QUAD pos = _file->GetPos();
    //if ( pos != _curoffset )
    //{
        _file->Seek( _curoffset, SEEKSET );
    //}

    assert(buffer);
    assert(size>=0);
    assert(_curoffset>=0);
    assert(_curoffset>=_baseoffset);
    assert(_curoffset<=_baseoffset+_subsize);

    const Types::QUAD quadmaxreadlen = _baseoffset + _subsize - _curoffset;
    const int maxreadlen = (int) SimpleMin<Types::QUAD>( quadmaxreadlen, std::numeric_limits<int>::max() );
    int readlen = SimpleMin(maxreadlen,size);
    //Types::QUAD postoffset = _curoffset;
    _curoffset = -1;    // invalidate current offset in case Read() throws an exception
    readlen = _file->ReadUpto( buffer, readlen );
    //Types::QUAD qReadLen = readlen;    
    //postoffset += qReadLen;
    //if ( _file->GetPos() != postoffset )
    //{
    //    throw Error("SubIFile::ReadUpto(): Bad postOffset");
    //}
    _curoffset = _file->GetPos();

    return readlen;
}



void SubIFile::Seek( Types::QUAD offset, SeekMode mode )
{
    assert(_curoffset>=0);
    assert(_curoffset>=_baseoffset);
    assert(_curoffset<=_baseoffset+_subsize);

    if(mode==SEEKSET)
    {
        assert(offset>=0);
        assert(offset<=_subsize);
        const Types::QUAD postoffset = _baseoffset + offset;
        _curoffset = -1;    // invalidate current offset in case Seek() throws an exception
        _file->Seek( postoffset, SEEKSET );
        _curoffset = postoffset;
    }
    else if(mode==SEEKCUR)
    {
        assert(offset >= (_baseoffset - _curoffset));
        assert(offset <= (_baseoffset - _curoffset + _subsize));
        const Types::QUAD postoffset = _curoffset + offset;
        _curoffset = -1;    // invalidate current offset in case Seek() throws an exception
        _file->Seek( postoffset, SEEKSET );
        _curoffset = postoffset;
        
    }
    else if(mode==SEEKEND)
    {
        assert(offset<=0);
        assert(offset>=-_subsize);
        const Types::QUAD postoffset = _baseoffset + _subsize + offset;
        _curoffset = -1;    // invalidate current offset in case Seek() throws an exception
        _file->Seek( postoffset, SEEKSET );
        _curoffset = postoffset;
    }
    else
    {
        assert(0);
    }
};



} // namespace Useless

