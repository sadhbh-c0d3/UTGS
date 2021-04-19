#ifndef __INCLUDED_USELESS_OFILE_H__
#define __INCLUDED_USELESS_OFILE_H__

/*
**  $Id: OFile.h,v 1.6 2002/09/13 16:22:11 koolas Exp $
**
**  NAME
**      OFile
**
**  PURPOSE
**      Output file abstract class.
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include "Useless/System/Types.h"

#pragma warning(disable:4786)       // disable 'indentifier was truncated...'
#include <string>

namespace Useless {

/*! \ingroup File
 *  \class OFile
 *
 *  Abstract for class for use by save functions.
 */
class USELESS_API OFile
{
public:
    enum SeekMode { SEEKSET=-1, SEEKCUR, SEEKEND };

    OFile() {}
    virtual ~OFile() throw() {}

    virtual int WriteUpto( const void *buffer, int size ) = 0;
    virtual void Seek( Types::QUAD offset, SeekMode mode=SEEKCUR ) = 0;
    virtual Types::QUAD GetPos() = 0;
    virtual Types::QUAD GetLen() = 0;
    virtual const std::string &GetName() = 0;

    virtual void Close() = 0;

    void Write( const void *buffer, int size );
    void WriteByte( Types::UBYTE );
    void WriteWordHE( Types::UWORD );
    void WriteLongHE( Types::ULONG );
    void WriteQuadHE( Types::UQUAD );
    void WriteWordLE( Types::UWORD );
    void WriteLongLE( Types::ULONG );
    void WriteQuadLE( Types::UQUAD );

private:
    OFile( const OFile& );            // prevent copy
    OFile& operator=( const OFile& ); // prevent copy
};


} // namespace Useless

#endif // __INCLUDED_USELESS_OFILE_H__
