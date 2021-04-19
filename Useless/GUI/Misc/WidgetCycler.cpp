#include "UselessPch.h"

#include "Useless/GUI/Misc/WidgetCycler.h"
#include "Useless/Errors1.h"

namespace Useless {

WidgetCycler::WidgetCycler( Widget *root, WidgetCycler *parent_cycler ): _root(root), _parent_cycler(parent_cycler)
{
    _iterator_stack.Insert( IteratorInsideWidget( root, root->FirstChild() ) );
}

WidgetCycler::~WidgetCycler()
{
}

#ifdef NDEBUG
    #define FUNCTION_HEADER( ret, name ) ret name()
    #define FUNCTION_HEADER_1( ret, name, arg, aname ) ret name( arg aname )
#else
    #define FUNCTION_HEADER( ret, name ) ret name() \
    { UpdateDebugInfo(); ret r=name##Impl(); UpdateDebugInfo(); return r; } \
    ret name##Impl()
    #define FUNCTION_HEADER_1( ret, name, arg, aname ) ret name( arg aname ) \
    { UpdateDebugInfo(); ret r=name##Impl( aname ); UpdateDebugInfo(); return r; } \
    ret name##Impl( arg aname )
#endif

bool WidgetCycler::RestoreIterator()
{
    if ( !IsValid() )
    {   // Brutal restore -- to avoid application crashes
        // need better implementation, which will find nearest widget
        _iterator_stack.Clear();
        _iterator_stack.Insert( IteratorInsideWidget( _root, _root->FirstChild() ) );
#       ifndef NDEBUG 
        UpdateDebugInfo();
#       endif
    }
    return !!Who();
}

bool WidgetCycler::IsValid() const
{
    CycleList::ConstIterator i = _iterator_stack.ConstBegin();
    Widget *parent, *child;
    if (!!i) do {
        parent = (*i).first;
        child  = (*i).second;
    } while ( parent->IsChild(child) && !!++i );

    return parent->IsChild(child);
}


//bool WidgetCycler::CycleForward()
FUNCTION_HEADER( bool, WidgetCycler::CycleForward )
{
    if ( !RestoreIterator() ) { return false; }
    
    if ( Who()->HasChilds() )
    {
        _iterator_stack.Insert( IteratorInsideWidget( Who(), Who()->FirstChild() ) );
    }
    else
    {
        while ( !++GetIterator() )
        {
            if ( _iterator_stack.Size() > 1 )
            {
                _iterator_stack.Last();
                _iterator_stack.Remove();
            }
            else 
            {
                PushIterator( GetContainer()->FirstChild() );

                if ( _parent_cycler )
                {
                    _parent_cycler->CycleForward();
                }
                return false;
            }
        }
    }
    return true;
}

//bool WidgetCycler::CycleBack()
FUNCTION_HEADER( bool, WidgetCycler::CycleBack )
{
    try
    {
        if ( !RestoreIterator() ) { return false; }

        bool was_previous = true;
        bool reached_end = false;

        if ( !--GetIterator() )
        {
            if ( _iterator_stack.Size() > 1 )
            {
                _iterator_stack.Last();
                _iterator_stack.Remove();
                was_previous = false;
            }
            else 
            {
                PushIterator( GetContainer()->LastChild() );
                reached_end = true;

                if ( _parent_cycler )
                {
                    _parent_cycler->CycleBack();
                }
            }
        }

        C_verify( Who());
        
        if (was_previous || reached_end)
        {
            int dbg_n_iterations=0;
            bool has_childs = Who()->HasChilds();
            while ( has_childs)
            {
                _iterator_stack.Insert( IteratorInsideWidget( Who(), Who()->LastChild()));
                PushIterator( GetContainer()->LastChild());
                ++dbg_n_iterations;
                C_verify( Who());
                has_childs = Who()->HasChilds();
            }
        }
        return !reached_end;
    }
    catch( const Error &e) { throw; }
    catch(...) {
        C_warnf( WidgetCycler, 0, false, ("who", (void*)Who()));
        bool restored = RestoreIterator();
        return restored;
    }
}

//bool WidgetCycler::Set(Widget *client)
FUNCTION_HEADER_1( bool, WidgetCycler::Set, Widget*, client )
{
    StupidVector<Widget*> hierarchy;
    StupidVector<Widget*>::Iterator ih;
    Widget *widget = client;

    do {
        hierarchy.Insert(widget);
        widget=widget->GetParent();

    } while ( !!*widget && _root!=widget );

    if ( _root!=widget ) { return false; }//If root is not the ancestor

    _iterator_stack.Clear();
    _iterator_stack.Insert( IteratorInsideWidget( _root, _root->FirstChild() ) );

    if ( Who()==client ) { return true; } //If client was a root

    for ( --(ih = hierarchy.End()); ; )
    {
        Widget::RefIter iw = GetIterator();
        for ( ; iw.Who()!=ih.Value(); ++iw ) { assert(!!iw && "WidgetCycler::Set()"); }
        
        if ( !!--ih )
        {
            assert( Who()->HasChilds() );
            _iterator_stack.Insert( IteratorInsideWidget( Who(), Who()->FirstChild() ) );
        }
        else break;
    };

    assert( Who()==client ); //This should be allways 'true' if we got to this point
    return true;
}

};//namespace Useless
