#include "UselessPch.h"

#pragma warning(disable:4250)

#include "Useless/GUI/Relations/MultiTabRelation.h"

namespace Useless {

MultiTabRelation::MultiTabRelation( HubWidget &group): GroupRelation(group)
{
}

MultiTabRelation::~MultiTabRelation()
{
}

void MultiTabRelation::Tie(EnumWidget *activator, Widget *appearee, bool is_default)
{
    AppearRelation *prelation = new AppearRelation( *appearee );
    prelation->AddSignal( OnBlur[activator], false );
    prelation->AddSignal( OnSelect[activator], true );
    _appear_relations.Insert(activator, prelation);
    activator->SetSelected(is_default);
    prelation->Appear(is_default);
}


};//namespace Useless
