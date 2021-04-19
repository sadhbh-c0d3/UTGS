#ifndef __INCLUDED__USELESS_BASIC_SAMPLE_H__
#define __INCLUDED__USELESS_BASIC_SAMPLE_H__

#include "Useless/Sound/Sample.h"
#include "Useless/Util/SPointer.h"

namespace Useless {

/*! BasicSample is a Sample built from given properties
*/
class USELESS_API BasicSample : public Sample
{
public:
    BasicSample( const Snd::Properties &p );
    virtual ~BasicSample();

    virtual void GetProperties( Snd::Properties& ) const;

protected:
    virtual void Load() {}

private:
    SPointer< Snd::Properties > _properties;
};

};//namespace Useless
#endif//__INCLUDED__USELESS_BASIC_SAMPLE_H__
