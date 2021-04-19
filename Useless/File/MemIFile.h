#ifndef __INCLUDED_USELESS_MEMIFILE_H__
#define __INCLUDED_USELESS_MEMIFILE_H__

/*
**  $Id: MemIFile.h,v 1.7 2002/09/12 14:27:36 koolas Exp $
**
**  NAME
**      MemIFile
**
**  PURPOSE
**      Retreve data from memory using file-routine-interface
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#ifndef __INCLUDED_USELESS_IFILE_H__
#include "Useless/File/IFile.h"
#endif  __INCLUDED_USELESS_IFILE_H__

#ifndef __INCLUDED_USELESS_MEMBLOCK_H__
#include "Useless/Util/Storage/MemBlock.h"
#endif __INCLUDED_USELESS_MEMBLOCK_H__

#pragma warning(disable:4786)       // disable 'indentifier was truncated...'
#include <stdio.h>
#include <string>

namespace Useless {

/*! \class MemIFile
 *  Used as buffer for some load actions
 */
class USELESS_API MemIFile : public IFile
{
public:
    MemIFile(const MemBlock& data);
    virtual ~MemIFile() throw();

    virtual int ReadUpto( void *buffer, int size );
    virtual void Seek( Types::QUAD offset, SeekMode mode=SEEKCUR );
    virtual Types::QUAD GetPos() { return _p; }
    virtual Types::QUAD GetLen();
    virtual const std::string &GetName();

private:
    const MemBlock &_data;
    Types::QUAD _p;

    static const std::string _name;           // should be null or something

    MemIFile(const MemIFile&);            // prevent copy
    MemIFile& operator=(const MemIFile&); // prevent copy
};


} // namespace Useless

#endif // __INCLUDED_USELESS_MEMIFILE_H__
