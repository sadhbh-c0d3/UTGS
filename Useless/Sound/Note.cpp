#include "UselessPch.h"


#include "Note.h"

namespace Useless {

Note::Note( long sample_rate, int note, int octave ):
    _sample_rate(sample_rate), _note(note), _octave(octave-4)
{}

Note::operator long () const
{
    double root12 = 1.0597;
    int i;
    int f = A4;
    
    if (_octave > 0)
    for ( i=0; i!=_octave; ++i )  {  f *= 2;  }

    else
    for ( i=0; i!=_octave; --i )  {  f /= 2;  }

    int zero = C + 12-A;
    int k = _note + zero;

    if (k>0) for ( i=0; i!=k; ++i ) { f*=root12; }
    else     for ( i=0; i!=k; --i ) { f/=root12; }

    return (_sample_rate * f)/A4;
}


};//namespace Useless

