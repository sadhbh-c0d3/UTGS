#ifndef __INCLUDED__USELESS_SAMPLE_H__
#define __INCLUDED__USELESS_SAMPLE_H__

#include "Useless/Sound/Device/SampleData.h"
#include "Useless/Util/Shares.h"

namespace Useless
{

/*! Sample is SampleData manager.
 *  Derivates would overload Load() method to supply data.
 */
class USELESS_API Sample
{
public:
    virtual ~Sample() {}

    void         SetData( SampleData *data );
    SampleData*  GetData() const;
    virtual void GetProperties( Snd::Properties& ) const = 0;

protected:
    virtual void Load() = 0;

private:
    Share<SampleData> _sh_data;
};


};//namespace Useless
#endif//__INCLUDED__USELESS_SAMPLE_H__
