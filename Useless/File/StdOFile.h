#ifndef __INCLUDED_USELESS_STDOFILE_H__
#define __INCLUDED_USELESS_STDOFILE_H__

/*
**  $Id: StdOFile.h,v 1.5 2002/09/12 14:27:36 koolas Exp $
**
**  NAME
**      StdOFile
**
**  PURPOSE
**      Output file using standarc C file io.
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#ifndef __INCLUDED_USELESS_OFILE_H__
#include "Useless/File/OFile.h"
#endif  __INCLUDED_USELESS_OFILE_H__

#pragma warning(disable:4786)       // disable 'indentifier was truncated...'
#include <stdio.h>
#include <string>

namespace Useless {

/*! \ingroup File
 *  \class StdOFile
 *
 *  This class uses libstdc++ to access files
 */
class USELESS_API StdOFile : public OFile
{
public:
    StdOFile( const std::string &name, bool removeonfail=false );
    virtual ~StdOFile() throw();

    virtual int WriteUpto( const void *buffer, int size );
    virtual void Seek( Types::QUAD offset, SeekMode mode=SEEKCUR );
    virtual Types::QUAD GetPos();
    virtual Types::QUAD GetLen();
    virtual const std::string &GetName();

    virtual void Close();


private:
    FILE        *_file;
    std::string  _name;
    bool         _removeonfail;

    StdOFile( const StdOFile& );            // prevent copy
    StdOFile& operator=( const StdOFile& ); // prevent copy
};


} // namespace Useless

#endif // __INCLUDED_USELESS_STDOFILE_H__
