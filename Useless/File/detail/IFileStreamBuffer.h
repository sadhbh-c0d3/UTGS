#ifdef WIN32
#   include <streambuf>
#else
#   include <streambuf.h>
#endif
#include <istream>
#include "Useless/File/IFile.h"
#include "Useless/Util/SPointer.h"

namespace Useless {

struct IFileStreamBuffer : public std::streambuf
{
public:
    enum { BUFFER_SIZE = 7 };
    
    IFileStreamBuffer( const SPointer<IFile> &file): _file(file)
    {
        fill_buffer(0x00000000);
    }
    
    virtual ~IFileStreamBuffer()
    {
        int count = _buffer + _valid - gptr();
        _file->Seek( -count);
    }
    
    virtual int underflow()
    {
        fill_buffer(0);
        return (_valid>0)? _buffer[0] : EOF;
    }
    
    virtual int uflow()
    {
        fill_buffer(0);
        gbump(1);
        return (_valid>0)? _buffer[0] : EOF;
    }
    
    virtual int pbackfail(int c)
    {
        fill_buffer(1);
        _buffer[0] = c;
        char *gp = gptr();
        return c;
    }

    //virtual streampos seekoff(streamoff, _seek_dir, int mode=ios::in|ios::out) {}
    //virtual streampos seekpos(streampos pos, int mode = ios::in|ios::out) {}
    
private:
    SPointer<IFile> _file;
    char _buffer[BUFFER_SIZE];
    int  _valid;

    void fill_buffer( int start_offset)
    {
        int bytes_to_read = BUFFER_SIZE-start_offset;
        _valid = (*_file).ReadUpto( _buffer+start_offset, bytes_to_read);
        setg( _buffer, _buffer, _buffer + _valid);
    }
};

};//namespace Useless

