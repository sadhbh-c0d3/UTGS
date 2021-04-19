#include "UselessPch.h"


#include "Sample.h"

namespace Useless {

void Sample::SetData( SampleData *data )
{
    _sh_data.SetTarget( data );
    Load();
}

SampleData* Sample::GetData() const
{
    return _sh_data.GetTarget();
}

};//namespace Useless
