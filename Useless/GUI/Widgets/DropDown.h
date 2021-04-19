#ifndef __INCLUDED__USELESS_DOROP_DOWN_H__
#define __INCLUDED__USELESS_DOROP_DOWN_H__

#include "Useless/GUI/Widgets/CaptionWidget.h" // for caption style (compatibility) - W/A (*)
#include "Useless/GUI/Widgets/ContentWidget.h"
#include "Useless/GUI/Widgets/ActiveWidget.h"

namespace Useless {

class USELESS_API DropDown : public ContentWidget, virtual public ActiveWidget, virtual public IWEnum
{
public:
    DropDown( const CaptionStyle &style, bool menu_above = false );
    virtual ~DropDown();

//#   include "DropDown.inl" - ParmList Ctor - not implemented

    /*! Feedback method - must return new DataObject (used to present selection) */
    virtual DataPointer GetSelectedData() const = 0;

    /*! Feedback method - should resize menu if it is possible */
    virtual void SetMenuHeight( int h ) {}


    virtual EnumWidget& Enumerator() { return _p_list->Enumerator(); }


    virtual void SetEnvironment( const WidgetEnvironment *enviro);

protected:
    IWEnum *_p_list;
    int _selected;

    void Tie2Signals( IWEnum *p_list );
    void Drop();
    void DragDrop();
    void MenuVisible( bool visible );
    void SetMenuWidth(int w);

    //- comaptibility W/A (*)
    int Place( int alignment )
    {
        return ((alignment == Useless::CaptionWidget::INNER_LEFT)? 0 :
                (alignment == Useless::CaptionWidget::INNER_RIGHT)? 100 : 50);
    }
    int _placement;
    bool _menu_above;
};


template< class _DropDown, class _ButtonLogic >
struct DropButton : _DropDown, virtual _ButtonLogic
{
    template< class _A1 >
    DropButton( const _A1 &a1 ): _DropDown( a1 )
        { Tie2Signals(); }
    
    template< class _A1, class _A2 >
    DropButton( const _A1 &a1, const _A2 &a2 ): _DropDown( a1, a2 )
        { Tie2Signals(); }
    
    template< class _A1, class _A2, class _A3 >
    DropButton( const _A1 &a1, const _A2 &a2, const _A3 &a3 ): _DropDown( a1, a2, a3 )
        { Tie2Signals(); }
    
    template< class _A1, class _A2, class _A3, class _A4 >
    DropButton( const _A1 &a1, const _A2 &a2, const _A3 &a3, const _A4 &a4 ): _DropDown( a1, a2, a3, a4 )
        { Tie2Signals(); }
    
    EnumWidget & Enumerator() { return _DropDown::Enumerator(); }

private:
    void Tie2Signals()
    {
        if ( EnumWidget *e = dynamic_cast< EnumWidget *>( this ))
        {
            Tie2Signal( _p_list->Atom().OnVisible, e, &EnumWidget::SetSelected );
        }
    }
};

};//namespace Useless
#endif//__INCLUDED__USELESS_DOROP_DOWN_H__
