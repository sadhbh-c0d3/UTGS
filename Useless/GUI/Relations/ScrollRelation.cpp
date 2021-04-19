#include "UselessPch.h"

#include "Useless/GUI/Relations/ScrollRelation.h"

#include "Useless/GUI/Widgets/ScrolledWidget.h"
#include "Useless/GUI/Widgets/ScrollbarWidget.h"

namespace Useless {

ScrollRelation::ScrollRelation( IWScrollee &iscrw, IWScrollbar &isbw, bool use_mouse_wheel)
{
    ScrolledWidget &scrw = iscrw.Scrollee();
    ScrollbarWidget &sbw = isbw.Scroller();

    if (sbw.IsHorizontal())
    {
        sbw.SetRange  ( scrw.GetHRange()   );
        sbw.SetSize   ( scrw.GetHSize()    );
        sbw.SetAdvance( scrw.GetHAdvance() );

        _on_scrollbar_advance = sbw.OnAdvance.TieUnary( &scrw, &Useless::ScrolledWidget::HScrollTo, sbw.OnAdvance.GetVar1() );
        _on_size_change       = Useless::Tie2Signal( scrw.OnHSize, &sbw , &Useless::ScrollbarWidget::SetSize );
        _on_range_change      = Useless::Tie2Signal( scrw.OnHRange, &sbw , &Useless::ScrollbarWidget::SetRange );
        _on_advance           = Useless::Tie2Signal( scrw.OnHAdvance, &sbw , &Useless::ScrollbarWidget::SetAdvance );
        if (use_mouse_wheel)
        {
            _on_wheel           = scrw.OnWheel.TieBinary( &scrw, &Useless::ScrolledWidget::Scroll, scrw.OnWheel.GetVar2(), 0 );
            _on_scrollbar_wheel = sbw.OnWheel.TieBinary( &scrw, &Useless::ScrolledWidget::Scroll, sbw.OnWheel.GetVar2(), 0 );
        }
    }
    else
    {
        sbw.SetRange  ( scrw.GetVRange()   );
        sbw.SetSize   ( scrw.GetVSize()    );
        sbw.SetAdvance( scrw.GetVAdvance() );

        _on_scrollbar_advance = sbw.OnAdvance.TieUnary( &scrw, &Useless::ScrolledWidget::VScrollTo, sbw.OnAdvance.GetVar1() );
        _on_size_change       = Useless::Tie2Signal( scrw.OnVSize, &sbw , &Useless::ScrollbarWidget::SetSize );
        _on_range_change      = Useless::Tie2Signal( scrw.OnVRange, &sbw , &Useless::ScrollbarWidget::SetRange );
        _on_advance           = Useless::Tie2Signal( scrw.OnVAdvance, &sbw , &Useless::ScrollbarWidget::SetAdvance );
        if (use_mouse_wheel)
        {
            _on_wheel           = scrw.OnWheel.TieBinary( &scrw, &Useless::ScrolledWidget::Scroll, 0, scrw.OnWheel.GetVar2() );
            _on_scrollbar_wheel = sbw.OnWheel.TieBinary( &scrw, &Useless::ScrolledWidget::Scroll, 0, sbw.OnWheel.GetVar2() );
        }
    }
}

ScrollRelation::~ScrollRelation()
{
    _on_scrollbar_advance.Untie();
    _on_size_change.Untie();
    _on_range_change.Untie();
    _on_advance.Untie();
    _on_scrollbar_wheel.Untie();
    _on_wheel.Untie();
}

};//namespace Useless
