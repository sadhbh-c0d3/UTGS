#ifndef __INCLUDED__USELESS__SAMPLE_SREAM_H__
#define __INCLUDED__USELESS__SAMPLE_SREAM_H__

#include "Useless/Sound/Device/SampleData.h"
#include "Useless/Sound/SampleType.h"
#include <assert.h>

namespace Useless {

/*! Sound sample streaming class.
 *  It operates on bytes ( does not look inside sample ).
 */
struct USELESS_API SampleStream
{
public:
    SampleStream(): _sample_data(0), _pos(0) {}
    SampleStream( SampleData *sample_data ): _sample_data( sample_data ) 
    {   
        Snd::Properties p = _sample_data->GetProperties();
        _width = ( p.bits * p.channels )/8;
        _size  = p.length * _width;
        _pos   = GetWriteCursor() + _width;//< first sample is one after the WriteCursor
    }

    // Return byte at which play cursor is
    int Played() const // return played byte (not sample)
    {
        return _sample_data->GetPlayPosition() * _width;
    }

    int GetWriteCursor() const
    {
        return _sample_data->GetWritePosition() * _width;
    }

    // Tell how many bytes may be written before play cursor is achieved
    int Free() const
    {
        int played = Played();
        int writeCursor = GetWriteCursor();
        if ( _pos < played )
        {
            return (played - _pos - _width);
        }
        else if ( _pos > writeCursor )
        {
            return (_size - _pos) + (played - _width);
        }
        else
        {
            return 0;
        }
    }

    // Tell if stream is ready to write new data
    bool Ready( int n ) const // n = bytes to write
    {
        return Free() >= n;
    }

    // Wait till ready to write new data
    void Wait( int n ) const // n = bytes to write
    {
        while ( Free() < n+4 ) {}
    }

    // Write to buffer, don't wait
    void Write( void *src, size_t n )// src = pointer to array, n = bytes to write
    {
        char *ptr;
        int   size;

        _sample_data->Lock( (void**)&ptr, &size );// obtain pointer to buffer
        assert( size==_size );// sound buffer size is constant non-zero

        char *p = ptr + _pos;
        if ( _pos + n >= (size_t)size )
        {
            memcpy( p, src, size-_pos );
            memcpy( ptr, src, n-(size-_pos) );
            _pos = n-(size-_pos);
        }
        else
        {
            memcpy( p, src, n );
            _pos += n;
        }

        _sample_data->Unlock();
    }
    
    int GetPos()   const  { return _pos; }
    int GetWidth() const  { return _width; }
    int GetSize()  const  { return _size; }
    int GetNumSamples() const { return _size / _width; }

private:
    SampleData *_sample_data;
    int         _pos;   // last written byte
    int         _width; // sample width in bytes
    int         _size;  // buffer size in bytes
};

};//namespace Useless
#endif//__INCLUDED__USELESS__SAMPLE_SREAM_H__
