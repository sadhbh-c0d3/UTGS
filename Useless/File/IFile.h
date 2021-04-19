#ifndef __INCLUDED_USELESS_IFILE_H__
#define __INCLUDED_USELESS_IFILE_H__

/*
**  $Id: IFile.h,v 1.8 2003/01/20 21:42:21 koolas Exp $
**
**  NAME
**      IFile
**
**  PURPOSE
**      Input file abstract class.
**
**  AUTHOR(S)
**      Jacek Trzmiel (jacekt@iavision.com)
*/

#include "Useless/System/Types.h"

#include <string>


namespace Useless {
/*! \defgroup File File I/O*/

/*! \ingroup File 
 *  \class IFile
 *
 *  Abstract for class for use by load functions.
 */
class USELESS_API IFile
{
public:
    enum SeekMode { SEEKSET=-1, SEEKCUR, SEEKEND };

    IFile() {}
    virtual ~IFile() throw() {}

    virtual int ReadUpto( void *buffer, int size ) = 0;
    virtual void Seek( Types::QUAD offset, SeekMode mode=SEEKCUR ) = 0;
    virtual Types::QUAD GetPos() = 0;
    virtual Types::QUAD GetLen() = 0;
    virtual const std::string &GetName() = 0;

    void Read( void *buffer, int size );
    void Begin() { Seek( 0, SEEKSET); }
    Types::BYTE ReadByte();
    Types::WORD ReadWordHE();
    Types::LONG ReadLongHE();
    Types::QUAD ReadQuadHE();
    Types::WORD ReadWordLE();
    Types::LONG ReadLongLE();
    Types::QUAD ReadQuadLE();

private:
    IFile(const IFile&);            // prevent copy
    IFile& operator=(const IFile&); // prevent copy
};


} // namespace Useless

#endif // __INCLUDED_USELESS_IFILE_H__
