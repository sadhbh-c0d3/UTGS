#ifndef __INCLUDED_USELESS_STDIFILE_H__
#define __INCLUDED_USELESS_STDIFILE_H__

/*
**  $Id: StdIFile.h,v 1.5 2003/01/20 21:42:21 koolas Exp $
**
**  NAME
**      StdIFile
**
**  PURPOSE
**      Input file using standarc C file io.
**
**  AUTHOR(S)
**      Jacek Trzmiel (jacekt@iavision.com)
*/

#ifndef __INCLUDED_USELESS_IFILE_H__
#include "Useless/File/IFile.h"
#endif//__INCLUDED_USELESS_IFILE_H__

#include <stdio.h>


namespace Useless {

/*! \ingroup File
 *  \class StdIFile
 *
 *  This class uses libstdc++ to access files
 */
class USELESS_API StdIFile : public IFile
{
public:
    StdIFile( const std::string &name );
    virtual ~StdIFile() throw();

    virtual int ReadUpto( void *buffer, int size );
    virtual void Seek( Types::QUAD offset, SeekMode mode=SEEKCUR );
    virtual Types::QUAD GetPos();
    virtual Types::QUAD GetLen();
    virtual const std::string &GetName();

private:
    FILE *_file;
    std::string _name;

    StdIFile(const StdIFile&);            // prevent copy
    StdIFile& operator=(const StdIFile&); // prevent copy
};


} // namespace Useless

#endif // __INCLUDED_USELESS_STDIFILE_H__
