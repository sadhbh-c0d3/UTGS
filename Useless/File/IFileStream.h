#ifndef __INCLUDED_USELESS_I_FILE_STREAM_H__
#define __INCLUDED_USELESS_I_FILE_STREAM_H__

#include "Useless/File/detail/IFileStreamBuffer.h"
namespace Useless {

    class USELESS_API IFileStream : public std::istream
    {
    public:
        IFileStream( const SPointer<IFile> &file ): _streambuffer(file), std::istream(&_streambuffer)
        {}
        
    private:
        IFileStreamBuffer _streambuffer;
    };
    
};//namespace Useless

#endif//__INCLUDED_USELESS_I_FILE_STREAM_H__
