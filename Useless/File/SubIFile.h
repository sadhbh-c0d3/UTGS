#ifndef __INCLUDED_USELESS_SUBIFILE_H__
#define __INCLUDED_USELESS_SUBIFILE_H__

/*
**  $Id: SubIFile.h,v 1.5 2004/06/23 17:41:22 koolas Exp $
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

#include "Useless/File/IFile.h"
#include "Useless/Util/SPointer.h"

//#include <string>


namespace Useless {



class USELESS_API SubIFile : public IFile
{
public:
    SubIFile();
    SubIFile( SPointer<IFile> file, const std::string &name, Types::QUAD baseoffset, Types::QUAD subsize );
    virtual ~SubIFile() throw() {}

    virtual int ReadUpto( void *buffer, int size );
    virtual void Seek( Types::QUAD offset, SeekMode mode=SEEKCUR );
    virtual Types::QUAD GetPos() { return _curoffset-_baseoffset; }
    virtual Types::QUAD GetLen() { return _subsize; }
    virtual const std::string &GetName() { return _name; }


protected:
    SPointer<IFile> _file;
    std::string     _name;
    Types::QUAD     _baseoffset;
    Types::QUAD     _subsize;
    Types::QUAD     _curoffset;


private:
    SubIFile( const SubIFile& );            // prevent copy
    SubIFile& operator=( const SubIFile& ); // prevent copy
};



} // namespace Useless

#endif // __INCLUDED_USELESS_SUBIFILE_H__
