#include "UselessPch.h"

#include "Useless/GUI/Relations/GroupRelation.h"
#include "Useless/GUI/Widgets/EnumWidget.h"

namespace Useless {

GroupRelation::GroupRelation( HubWidget &container): _group(&container), _z_front(0), _selected(0)
{
    Widget::RefIter it = container.FirstChild();
    it.EvalFwd( PackFunctor( UnaryCall( this, &GroupRelation::IterTie), it ) );
    it.EvalFwd( PackFunctor( UnaryCall( this, &GroupRelation::IterUpdate), it ) );
}

GroupRelation::GroupRelation( Widget &container): _group(0), _z_front(0), _selected(0)
{
    Widget::RefIter it = container.FirstChild();
    it.EvalFwd( PackFunctor( UnaryCall( this, &GroupRelation::IterTie), it ) );
    it.EvalFwd( PackFunctor( UnaryCall( this, &GroupRelation::IterUpdate), it ) );
}

GroupRelation::~GroupRelation()
{
}

void GroupRelation::Update( Widget *widget)
{
    if ( EnumWidget *ew = dynamic_cast<EnumWidget*>(widget) )
    {
        if ( ew->GetSelected() )
        {
            if ( ew!=_selected )
            {
                if ( Signal *sig = OnBlur.Find(_selected) ) { sig->Send(); }
                
                if (_group) 
                { 
                    _group->SetChildZIndex( _z, _selected);
                    _z = _group->GetChildZIndex( widget);
                    _group->SetChildZIndex( _z_front+10, widget); 
                }

                _selected = widget;
                {
                    Widget::RefIter it = _selected->GetParent()->FirstChild();
                    it.EvalFwd( PackFunctor( 
                                   UnaryCall( this, &GroupRelation::IterUpdate),
                                   it )
                              );
                }

                if ( Signal *sig = OnSelect.Find(_selected) ) { sig->Send(); }
            }
        }
        else
        {
            if ( ew==_selected )
            {
                if ( Signal *sig = OnBlur.Find(_selected) ) { sig->Send(); }
                _selected=NULL;
            }
        }
    }
}

void GroupRelation::IterUpdate( Widget *widget )
{
    if ( widget != _selected)
    {
        if ( EnumWidget *ew = dynamic_cast<EnumWidget*>(widget) )
        {
            ew->SetSelected(0);
        }
    }
}

void GroupRelation::IterTie( Widget *widget )
{
    if ( EnumWidget *ew = dynamic_cast<EnumWidget*>(widget) )
    {
        ew->OnChange.TieUnary( this, &GroupRelation::Update, ew);

        if (_group)
        {
            int z = _group->GetChildZIndex( ew);
            if ( _z_front < z) { _z_front = z; }
        }
    }
}


};//namespace Useless
