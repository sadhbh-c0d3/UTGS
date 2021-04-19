#include "UselessPch.h"


#include "SampleType.h"
#include <assert.h>

namespace Useless {
namespace Snd {

void SampleTypeTest()
{
    Snd::Unsigned8 v8;
    Snd::Signed16  v16;
    Snd::Float32   v32;

    v8 = (char)100;
    v8 = (char)-100;

    v8 = (int)32000;
    v8 = (int)-32000;

    v8 = (unsigned char)200;
    v8 = (unsigned int)64000;

    v8 = (float)0.5;
    v8 = (float)-0.5;

    v16 = (char)100;
    v16 = (char)-100;

    v16 = (int)32000;
    v16 = (int)-32000;

    v16 = (unsigned char)200;
    v16 = (unsigned int)64000;

    v16 = (float)0.5;
    v16 = (float)-0.5;

    v32 = (char)100;
    v32 = (char)-100;

    v32 = (int)32000;
    v32 = (int)-32000;

    v32 = (unsigned char)200;
    v32 = (unsigned int)64000;

    v32 = (float)0.5;
    v32 = (float)-0.5;

    {for ( unsigned char k=0; k!=255; ++k )
    {
        v8 = k;
        v16 = v8.Get();
        v8 = v16.Get();
        assert( k==v8 );

        v8 = k;
        v32 = v8.Get();
        v8 = v32.Get();
        assert( k==v8.Get() );
    }}

    {for ( int k=-32768; k!=32768; ++k )
    {
        v16 = k;
        assert( k==v16 );

        v8 = v16.Get();
        v16 = v8.Get();
        assert( ((v16.Get()-k)>-256) && ((v16.Get()-k)<256) );

        v16 = k;
        v32 = v16.Get();
        v16 = v32.Get();
        assert( k==v16.Get() );
    }}
}

};//namespace Snd
};//namespace Useless
