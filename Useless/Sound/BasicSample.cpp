#include "UselessPch.h"

#include "BasicSample.h"

namespace Useless {

BasicSample::BasicSample( const Snd::Properties &p )
{
    _properties = new Snd::Properties(p);
}

BasicSample::~BasicSample()
{
}

void BasicSample::GetProperties( Snd::Properties &p ) const
{
    p = *_properties;
}

};//namespace Useless
