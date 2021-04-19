#ifndef __INCLUDED__USELESS_NOTE_H__
#define __INCLUDED__USELESS_NOTE_H__

#include <string>

namespace Useless {

/*! Calculates play rate for given note_name and sample_rate
 */
class USELESS_API Note
{
public:
    enum { C=0, D=2, E=4, F=5, G=7, A=8, H=11, SHARP=1, FLAT=-1, A4=440 };

    Note( long sample_rate, int note, int octave );
    operator long () const;

private:
    long _sample_rate;
    int  _note;
    int  _octave;
};

};//namespace Useless
#endif//__INCLUDED__USELESS_NOTE_H__
