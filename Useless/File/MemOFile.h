#ifndef __INCLUDED_USELESS_MEMOFILE_H__
#define __INCLUDED_USELESS_MEMOFILE_H__

/*
**  $Id: MemOFile.h,v 1.9 2002/09/12 14:27:36 koolas Exp $
**
**  NAME
**      MemOFile
**
**  PURPOSE
**      Output data to memory using file-routine-interface
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#ifndef __INCLUDED_USELESS_OFILE_H__
#include "Useless/File/OFile.h"
#endif  __INCLUDED_USELESS_OFILE_H__

#ifndef __INCLUDED_USELESS_MEMBLOCK_H__
#include "Useless/Util/Storage/MemBlock.h"
#endif __INCLUDED_USELESS_MEMBLOCK_H__

#pragma warning(disable:4786)       // disable 'indentifier was truncated...'
#include <stdio.h>
#include <string>

namespace Useless {

/*! \class MemOFile
 *  Used as buffer for some save actions
 */
class USELESS_API MemOFile : public OFile
{
public:
    MemOFile(int size);
    virtual ~MemOFile() throw();

    virtual int WriteUpto( const void *buffer, int size );
    virtual void Seek( Types::QUAD offset, SeekMode mode=SEEKCUR );
    virtual Types::QUAD GetLen();
    virtual Types::QUAD GetPos() { return _p; }
    virtual const std::string &GetName();
    const MemBlock &GetData() const;

    virtual void Close() {}


private:
    MemBlock _data;
    Types::QUAD _p;

    static const std::string _name;           // should be null or something

    MemOFile(const MemOFile&);            // prevent copy
    MemOFile& operator=(const MemOFile&); // prevent copy
};


} // namespace Useless

#endif // __INCLUDED_USELESS_MEMOFILE_H__
