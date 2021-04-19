#ifdef WIN32
#   include <streambuf>
#else
#   include <streambuf.h>
#endif
#include <istream>
#include "Useless/File/OFile.h"
#include "Useless/Util/SPointer.h"

namespace Useless {

struct OFileStreamBuffer : public std::streambuf
{
public:
    enum { BUFFER_SIZE = 7 };
    
    OFileStreamBuffer( const SPointer<OFile> &file): _file(file)
    {
        setp( _buffer, _buffer + BUFFER_SIZE);
    }
    
    virtual ~OFileStreamBuffer()
    {
        flush_buffer();
    }

    virtual int overflow( int c = EOF)
    {
        flush_buffer();
        if ( c!=EOF)
        {
            _buffer[0] = c;
            pbump(1);
        }
        return c;
    }

private:
    SPointer<OFile> _file;
    char _buffer[BUFFER_SIZE];

    void flush_buffer()
    {
        size_t valid = pptr() - _buffer;
        _file->WriteUpto( _buffer, valid);
        setp( _buffer, _buffer + BUFFER_SIZE);
    }
};

};//namespace Useless

