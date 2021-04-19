#ifndef __INCLUDED_USELESS_O_FILE_STREAM_H__
#define __INCLUDED_USELESS_O_FILE_STREAM_H__

#include "Useless/File/detail/OFileStreamBuffer.h"
namespace Useless {

    class USELESS_API OFileStream : public std::ostream
    {
    public:
        OFileStream( const SPointer<OFile> &file ): _streambuffer(file), std::ostream(&_streambuffer)
        {}
        
    private:
        OFileStreamBuffer _streambuffer;
    };
    
};//namespace Useless
#endif//__INCLUDED_USELESS_O_FILE_STREAM_H__
