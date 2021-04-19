#include "Useless/UselessConfig.h"
#include "Useless/File/IFile.h"
#include "Useless/Util/SPointer.h"
#include "Useless/Util/Storage/MemBlock.h"
#include "Useless/Util/Math.h"


namespace Useless {

class USELESS_API CFileStream : public CAsyncStream
{
public:
    CFileStream( const SPointer<IFile> &file, int max_buffer_size_in_kB = 256 )
    {
        _file = file;
        _offset = 0;
        _buffer.Alloc( SimpleMin( _file->GetLen(), max_buffer_size_in_kB*1024));
        _limiter = _buffer.GetSize()-1;
        _length = _file->GetLen();
        FillBuffer();
    }

    HRESULT SetPointer( LONGLONG offset)
    {
        if ( offset<0 || offset>_length )
        {   return S_FALSE;
        }
        _offset = offset%_buffer.GetSize();

        int block_no = offset/_buffer.GetSize();
        LONGLONG file_offset = block_no*_buffer.GetSize();
        LONGLONG file_p = _file->GetPos();

        if ( file_p != file_offset+(_limiter+1) )
        {
            _file->Seek( file_offset, IFile::SEEKSET );
            FillBuffer();
        }
        return S_OK;
    }

    HRESULT Read( PBYTE dest,
                  DWORD bytes_to_read,
                  BOOL aligned,
                  LPDWORD p_read_bytes )
    {
        CAutoLock lck(&_lock);

        int bytes_in_buffer = _limiter - _offset +1;
        int bytes_from_buffer = SimpleMin( bytes_in_buffer, bytes_to_read);
        int bytes_from_file = SimpleMax( bytes_to_read-bytes_from_buffer, 0);

        memcpy( dest, _buffer.GetPtrUB()+_offset, bytes_from_buffer );
        dest += bytes_from_buffer;
        _offset += bytes_from_buffer;

        if ( bytes_from_file>0 )
        {
            bytes_from_file = _file->ReadUpto( dest, bytes_from_file);
            FillBuffer();
        }

        *p_read_bytes = bytes_from_buffer + bytes_from_file;
        return S_OK;
    }
    LONGLONG Size( LONGLONG* = 0 )
    {
        return _length;//_limiter+1;
    }
    DWORD Alignment()
    {
        return 1;
    }
    void Lock()
    {
        _lock.Lock();
    }
    void Unlock()
    {
        _lock.Unlock();
    }
    void FillBuffer()
    {
        int read_bytes = _file->ReadUpto( _buffer.GetPtrUB(), _buffer.GetSize());
        _limiter = read_bytes-1;
    }

private:
    CCritSec        _lock;
    MemBlock        _buffer;
    int             _offset;
    int             _limiter;
    SPointer<IFile> _file;
    LONGLONG        _length;
};

};//namespace Useless
