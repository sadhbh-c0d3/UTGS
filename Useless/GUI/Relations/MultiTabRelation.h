#ifndef __INCLUDED_CODE_MULTI_TAB_RELATION_H__
#define __INCLUDED_CODE_MULTI_TAB_RELATION_H__

#include "Useless/GUI/Widgets/EnumWidget.h"
#include "Useless/GUI/Relations/GroupRelation.h"
#include "Useless/GUI/Relations/AppearRelation.h"
#include "Useless/Util/Storage/AssocVector.h"
#include "Useless/Util/SPointer.h"

namespace Useless {

/*! \ingroup Relations
 *  MultiTabRelation compounds of GroupRelation and few AppearRelations.
 *  It makes it easier to create multi-tab view. Tab-buttons are child
 *  widgets of group (passed to constructor) and widgets to be switched
 *  between are added via Tie command. 
 *
 *  Don't worry about BufferWidget behavior. In Tie method \a apearee 
 *  may be whatever widget you want.
 */
class USELESS_API MultiTabRelation : GroupRelation
{
public:
    MultiTabRelation( HubWidget &group);
    virtual ~MultiTabRelation();

    void Tie(EnumWidget *activator, Widget *appearee, bool is_default=true);

private:
    typedef SPointer<AppearRelation> __SP_Relation;
    AssocVector< Widget*, __SP_Relation> _appear_relations;
};

};//namespace Useless

#endif//__INCLUDED_CODE_MULTI_TAB_RELATION_H__
