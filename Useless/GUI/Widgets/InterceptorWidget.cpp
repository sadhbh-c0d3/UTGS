#include "UselessPch.h"

#include "Useless/GUI/Widgets/InterceptorWidget.h"
#include "Useless/GUI/DebugGUI.h"

namespace Useless {

void InterceptorWidget::InputIntercept(Widget *sender)
{
    if (sender==this) { sender = NULL; }
    if (_input_interceptor && sender!=_input_interceptor)
    {
        _input_interceptor->InputRelease(this);
    }
    _input_interceptor = sender;
    DBG_IntCur
}

void InterceptorWidget::InputRelease(Widget *sender)
{
    if (sender==this) { sender = NULL; }
    if (_input_interceptor && sender!=_input_interceptor) 
    { 
        _input_interceptor->InputRelease(this);
    }
    _input_interceptor = NULL;
    DBG_RelCur
}

void InterceptorWidget::Attach()
{
    DBG_Attach;
    if ( GetParent() ) GetParent()->InputIntercept(this);
}

void InterceptorWidget::Detach()
{
    DBG_Detach;
    Accept( InputReset(this) );
    if ( GetParent() ) GetParent()->InputRelease(this);
}


};//namespace Useless
