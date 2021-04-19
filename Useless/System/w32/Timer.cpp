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

#include "UselessPch.h"

#include "Useless/System/Timer.h"

namespace Useless {

Timer::Timer()
{
    LARGE_INTEGER licounter;
    QueryPerformanceCounter( &licounter );
    counter = licounter.QuadPart;
}

Timer::Timer(const Timer& t): counter(t.counter)
{}

///
/// friend SYS_Timer::operator-

double operator- ( const Timer &t1, const Timer &t2 )
{
    return ConvertFromCPUCycles(t1.counter-t2.counter);
}

double Timer::GetTimeDelta()
{
    Timer t1;
    double dt = t1 - (*this);
    (*this) = t1;
    return dt;
}

Types::QUAD ConvertToCPUCycles( double time )
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency( &frequency );

    return (Useless::Types::QUAD)( time*double(frequency.QuadPart) );
}

double ConvertFromCPUCycles( Useless::Types::QUAD time )
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency( &frequency );
    return double(time)/double(frequency.QuadPart);
}

};// namespace Useless

