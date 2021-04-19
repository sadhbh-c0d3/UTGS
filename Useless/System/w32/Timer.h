#ifndef __INCLUDED_USELESS_TIMER_H__
#define __INCLUDED_USELESS_TIMER_H__

/*
**
**  NAME
**      Timer
**
**  PURPOSE
**      Class used to get time and calculate diff between two times.
**
**  AUTHOR(S)
**      Jacek Trzmiel (sc0rp@iaviision.com)
*/

#include "Useless/System/Types.h"

namespace Useless {
/*! \ingroup Util
 *  Measures time in cycles of PC.
 */
class USELESS_API Timer
{
public:
    //! default constructor does init object with current time
    Timer();
    Timer(const Timer&);

    //! minus operator return number of seconds between two times
    friend double operator- ( const Timer &t1, const Timer &t2 );

    //! get time delta between current time, and recorded one, and
    //  set counter to current time
    double GetTimeDelta();

    Types::QUAD counter;
};



Types::QUAD USELESS_API ConvertToCPUCycles( double time );
double USELESS_API ConvertFromCPUCycles( Useless::Types::QUAD time );


};// namespace Useless

#endif //__INCLUDED_USELESS_TIMER_H__
