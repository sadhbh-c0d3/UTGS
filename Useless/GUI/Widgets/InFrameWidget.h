#ifndef __INCLUDED__USELESS_IN_FRAME_WIDGET_H__
#define __INCLUDED__USELESS_IN_FRAME_WIDGET_H__

#include "Useless/GUI/Skins/FrameSkin.h"
#include "Useless/GUI/Widgets/CompoundWidget.h"
#include "Useless/Util/TemplateDefines.h"

namespace Useless {

template< class WidgetType >
class USELESS_API InFrameWidget : public CompoundWidget
{
public:
    USELESS_SPECIALIZATION InFrameWidget( const Skin &frame_skin, int alignment )
            :_sp_skin( &CopyOf<Skin>(frame_skin) ), _alignment(alignment) 
            {
                _widget.OnResize.TieVoid( this, &InFrameWidget::UpdateAlignment );
                _widget.SetParent(this);
                FitFrame();
            }

#define USELESS_IN_FRAME_WIDGET_CTOR(n) \
    _TPL_##n InFrameWidget( const Skin &frame_skin, int alignment, _TPL_ARGS_##n ) \
            :_sp_skin( &CopyOf<Skin>(frame_skin) ), _alignment(alignment), _widget(_TPL_PASS_ARGS_##n)\
            {\
                _widget.OnResize.TieVoid( this, &InFrameWidget::UpdateAlignment );\
                _widget.SetParent(this);\
                FitFrame();\
            }

USELESS_IN_FRAME_WIDGET_CTOR(1)
USELESS_IN_FRAME_WIDGET_CTOR(2)
USELESS_IN_FRAME_WIDGET_CTOR(3)
USELESS_IN_FRAME_WIDGET_CTOR(4)
USELESS_IN_FRAME_WIDGET_CTOR(5)
USELESS_IN_FRAME_WIDGET_CTOR(6)
USELESS_IN_FRAME_WIDGET_CTOR(7)
USELESS_IN_FRAME_WIDGET_CTOR(8)


    virtual Rect GetClientRect( const Widget *widget )
    {
        if ( widget == &_widget )
        {
            int lt,tp,rt,bt;
            GetMargins( lt, tp, rt, bt );
            return Rect( lt, tp, GetWidth()-(lt+rt), GetHeight()-(tp+bt) );
        }
        else { return GetBoundingRect(); }
    }

    virtual void RepaintAction(const WidgetPainter &painter)
    {
        _sp_skin->Paint( painter );
    }

    virtual void Resize (int w, int h)
    {
        DirtyLock guard(&_widget);

        (*_sp_skin).SafeResize( Dim2i(w,h) );
        w = _sp_skin->GetWidth();
        h = _sp_skin->GetHeight();

        int lt,tp,rt,bt;
        GetMargins( lt, tp, rt, bt );
        _widget.Resize( w-(lt+rt), h-(tp+bt) );
        SetDimensions( w, h );
        UpdateAlignment();
    }

    void FitFrame()
    {
        DirtyLock guard(&_widget);

        int w = _widget.GetWidth();
        int h = _widget.GetHeight();

        int lt,tp,rt,bt;
        GetMargins( lt, tp, rt, bt );
        (*_sp_skin).SafeResize( Dim2i( w+(lt+rt),h+(tp+bt)) );
        SetDimensions( w+(lt+rt), h+(tp+bt) );
        UpdateAlignment();
    }

    void GetMargins( int &lt, int &tp, int &rt, int &bt )
    {
        lt = _sp_skin->GetDimension(LEFT_MARGIN);
        rt = _sp_skin->GetDimension(RIGHT_MARGIN);
        tp = _sp_skin->GetDimension(TOP_MARGIN);
        bt = _sp_skin->GetDimension(BOTTOM_MARGIN);
    }

    void UpdateAlignment()
    {
        int lt,tp,rt,bt;
        GetMargins( lt, tp, rt, bt );
        Rect client_area( lt, tp, GetWidth()-(lt+rt), GetHeight()-(tp+bt) );
        Rect client_bound = _widget.GetBoundingRect();
        client_bound.Align( client_area, (Useless::Rect::Alignment)_alignment );
        _widget.SetPosition( client_bound.GetPos() );
    }

    WidgetType& Get() { return _widget; }
    const WidgetType& Get() const { return _widget; }


    virtual   bool  IsChild      (const Widget *widget) const { return widget==&_widget; }
    virtual   bool  HasChilds    ()                     const { return true; }
    virtual    int  ChildrenCount()                     const { return 1; }

    virtual RefIter  FirstChild() const { return new Iterator((Widget*)&_widget); } 
    virtual RefIter  LastChild () const { return new Iterator((Widget*)&_widget); } 

    class Iterator : public Widget::Iterator
    {
    public:
        Iterator( Widget *p ) { _widget=p; }
        virtual     void  Next()     { _widget=NULL; }
        virtual     void  Previous() { _widget=NULL; }
    };

private:
    WidgetType      _widget;
    SPointer<Skin>  _sp_skin;
    int             _alignment;
};

};//namespace Useless
#endif//__INCLUDED__USELESS_IN_FRAME_WIDGET_H__
