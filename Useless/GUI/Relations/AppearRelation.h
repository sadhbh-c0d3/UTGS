#ifndef __INCLUDED_USELESS_APPEAR_RELATION_H__
#define __INCLUDED_USELESS_APPEAR_RELATION_H__

#include "Useless/GUI/Relations/Relation.h"
#include "Useless/GUI/Widgets/BridgeWidget.h"
#include "Useless/GUI/Widgets/HubWidget.h"
#include "Useless/Util/Storage/StupidVector.h"

namespace Useless {

/*! \ingroup Relations
 *  AppearRelation is used to manage insertions and retrivals of widget into compound.
 *  As an effect you may create some set of widgets inside one hub, which is dependant
 *  from enviroment. You can make some button show that hub or hide. You may
 *  create multi-tab view using this class.
 */
class USELESS_API AppearRelation : public Relation
{
public:
    AppearRelation( Widget &widget, bool input_eager=false, bool use_hard_appear=false);
    AppearRelation( CompoundWidget &container, Widget &widget, bool input_eager=false);
    virtual ~AppearRelation();

    void AddSignal( Signal &signal, bool show = true, bool doXor=false);

    void Appear( bool show=true, bool doXor=false);

    Widget* Apearee() const { return _widget;  }
    Widget* Pioneer() const { return _pioneer; }

    Signal OnShow;
    Signal OnHide;

private:
    BridgeWidget    *_bridge;
    HubWidget       *_hub;
    Widget          *_widget;
    Widget          *_pioneer;
    Pos              _position;
    int              _z;
    bool             _input_eager;
    bool             _appeared;
    bool             _use_hard_appear;

    StupidVector< Signal::FuncID > _on_appear;

    AppearRelation(const AppearRelation&);
};

};//namespace Useless

#endif  __INCLUDED_USELESS_APPEAR_RELATION_H__
