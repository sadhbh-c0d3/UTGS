#ifndef __INCLUDED__USELESS_KEY_PREDICATES_H__
#define __INCLUDED__USELESS_KEY_PREDICATES_H__

#include "Useless/System/KeyCodes.h"
#include "Useless/Functional/Expression.h"
#include "Useless/GUI/Widgets/ActiveWidget.h"

namespace Useless {

/*! Predicate who checks if a given key 
    via code '_key' was pressed
---------------------------------------*/
struct WhenKey : public Evaluator< bool, ActiveWidget*, WhenKey >
{
public:
    WhenKey( int key_code ): _key(key_code) {}

    bool operator ()( ActiveWidget *p ) const
    {
        assert(p);
		return p->IsKeyPressed( _key );
    }

private:
    int _key;
};

/*! Predicate who asks if a given key 
    via code '_key' was released
---------------------------------------*/
struct NotKey : public Evaluator< bool, ActiveWidget*, NotKey >
{
public:
    NotKey( int key_code ): _key(key_code) {}

    bool operator ()( ActiveWidget *p ) const
    {
        assert(p);
		return !p->IsKeyPressed( _key );
    }

private:
    int _key;
};

/*! Predicate who asks if mouse was over
    given ActiveWidget
---------------------------------------*/
struct WhenOver : public Evaluator< bool, ActiveWidget*, WhenOver >
{
public:
    bool operator ()( ActiveWidget *p ) const
    {
		return p->WasOver();
    }
};

/*! Predicate who asks if a given 
 *  Widget has focus
---------------------------------------*/
struct WhenFocussed : public Evaluator< bool, ActiveWidget*, WhenFocussed >
{
public:
    bool operator ()( ActiveWidget *p ) const
    {
		return p->HasFocus();
    }
};

/*! Predicate who asks if any mouse button
    was preessed at given ActiveWidget
---------------------------------------*/
struct AnyButton : public Evaluator< bool, ActiveWidget*, AnyButton >
{
public:
    bool operator ()( ActiveWidget *p ) const
    {
		return !p->NoButtons();
    }
};

/*! Predicate who asks if given
    ActiveWidget likes focuss
---------------------------------------*/
struct LikesFocus : public Evaluator< bool, ActiveWidget*, LikesFocus >
{
public:
    bool operator ()( ActiveWidget *p ) const
    {
		return p->IsFocusAdaptive();
    }
};

/*! Predicate who asks if given
    ActiveWidget is RealyVisible
---------------------------------------*/
struct WhenVisible : public Evaluator< bool, ActiveWidget*, WhenVisible >
{
public:
    bool operator ()( ActiveWidget *p ) const
    {
		return p->IsRealyVisible();
    }
};



/*! ActiveWidget predicate evaluator
--------------------------------------*/
template< class StatePredicate >
class StatePredicateEvaluator
{
public:
	StatePredicateEvaluator( ActiveWidget *ptr, const StatePredicate &state_pred )
        : _ptr(ptr), _state_pred(state_pred) {}

	bool operator()() {	return _state_pred(_ptr); }

private:
	ActiveWidget   *_ptr;
	StatePredicate  _state_pred;
};

/*! Creates ActiveWidget StatePredicateEvaluator
------------------------------------------------*/
template< class StatePredicate >
StatePredicateEvaluator< StatePredicate >
WhenState( ActiveWidget *ptr, const StatePredicate &state_pred )
{
    return StatePredicateEvaluator< StatePredicate >( ptr, state_pred);
}



};//namespace Useless
#endif//__INCLUDED__USELESS_KEY_PREDICATES_H__
