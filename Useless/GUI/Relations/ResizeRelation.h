#ifndef __INCLUDED__USELESS_RESIZE_RELATION_H__
#define __INCLUDED__USELESS_RESIZE_RELATION_H__

#include "Useless/GUI/Relations/Relation.h"
#include "Useless/GUI/Widgets/Widget.h"

namespace Useless {

class USELESS_API ResizeRelation : public Relation
{
public:
    ResizeRelation( Widget &copier, Widget &copied, bool copy_hsize, bool copy_vsize );
    virtual ~ResizeRelation();

private:
    Signal::FuncID _on_resize;

};

};//namespace Useless

#endif//__INCLUDED__USELESS_RESIZE_RELATION_H__
