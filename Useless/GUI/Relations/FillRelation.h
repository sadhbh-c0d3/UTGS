#ifndef __INCLUDED_USELESS_FILL_RELATION_H__
#define __INCLUDED_USELESS_FILL_RELATION_H__

#include "Useless/GUI/Relations/Relation.h"
#include "Useless/GUI/Widgets/Widget.h"

namespace Useless {

class USELESS_API FillRelation : public Relation
{
public:
    FillRelation( Widget &widget, const Rect &padding = Rect(0,0,0,0));
    virtual ~FillRelation();

private:
    Signal::FuncID _on_client_area_resize;
};

};//namespace Useless

#endif//__INCLUDED_USELESS_FILL_RELATION_H__
