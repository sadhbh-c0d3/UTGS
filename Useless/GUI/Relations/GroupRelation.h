#ifndef __INCLUDED_USELESS_GROUP_RELATION_H__
#define __INCLUDED_USELESS_GROUP_RELATION_H__

#include "Useless/GUI/Relations/Relation.h"
#include "Useless/GUI/Widgets/HubWidget.h"
#include "Useless/Util/Storage/StupidVector.h"
#include "Useless/Util/Storage/AssocVector.h"

namespace Useless {

/*! \ingroup Relations
 *  GroupRelation keeps an eye on all EnumWidget inside hub. It allows
 *  only one child to be selected(checked). You can use this to
 *  create radio-group or multi-tab view.
 */
class USELESS_API GroupRelation : public Relation
{
public:
    GroupRelation( HubWidget &group);
    GroupRelation( Widget &group);
    virtual ~GroupRelation();

    
    AssocVector< Widget*, Signal > OnSelect;
    AssocVector< Widget*, Signal > OnBlur;

private:
    HubWidget       *_group;
    int              _z;
    int              _z_front;
    Widget          *_selected;

    void Update    ( Widget *widget );
    void IterUpdate( Widget *widget );
    void IterTie   ( Widget *widget );

    StupidVector< Signal::FuncID > _fids;
};

};//namespace Useless

#endif  __INCLUDED_USELESS_GROUP_RELATION_H__
