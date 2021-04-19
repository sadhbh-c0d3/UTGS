#ifndef __INCLUDED_USELESS_RESOURCES_SIGNAL_MAP_H__
#define __INCLUDED_USELESS_RESOURCES_SIGNAL_MAP_H__

#include "Useless/GUI/Widgets/ActiveWidget.h"
#include "Useless/Util/CTC.h"

namespace Useless {

struct SignalMap_ActiveWidget
{
    typedef Signal (ActiveWidget::*SigPtr);
    typedef std::map< std::string, SigPtr > SignalMap;
    SignalMap _signalMap;
    
    
    SignalMap_ActiveWidget()
    {
        _signalMap["onpress"]       = (SigPtr)( &ActiveWidget::OnPress );
        _signalMap["onrelease"]     = (SigPtr)( &ActiveWidget::OnRelease );
        _signalMap["onclick"]       = (SigPtr)( &ActiveWidget::OnClick );
        _signalMap["ondoubleclick"] = (SigPtr)( &ActiveWidget::OnDoubleClick );
        _signalMap["onmovein"]      = (SigPtr)( &ActiveWidget::OnMoveIn );
        _signalMap["onmoveout"]     = (SigPtr)( &ActiveWidget::OnMoveOut );
        _signalMap["onmoveover"]    = (SigPtr)( &ActiveWidget::OnMoveOver );
        _signalMap["onmoveoutside"] = (SigPtr)( &ActiveWidget::OnMoveOutside );
        _signalMap["whilemoving"]   = (SigPtr)( &ActiveWidget::WhileMoving );
        _signalMap["oncatch"]       = (SigPtr)( &ActiveWidget::OnCatch );
        _signalMap["ondrag"]        = (SigPtr)( &ActiveWidget::OnDrag );
        _signalMap["ondrop"]        = (SigPtr)( &ActiveWidget::OnDrop );
        _signalMap["onactivate"]    = (SigPtr)( &ActiveWidget::OnActivate );
        _signalMap["whilehold"]     = (SigPtr)( &ActiveWidget::WhileHold );
        _signalMap["whiledrag"]     = (SigPtr)( &ActiveWidget::WhileDrag );
        _signalMap["onwheel"]       = (SigPtr)( &ActiveWidget::OnWheel );
        _signalMap["offwheel"]      = (SigPtr)( &ActiveWidget::OffWheel );
        _signalMap["offpress"]      = (SigPtr)( &ActiveWidget::OffPress );
        _signalMap["onchar"]        = (SigPtr)( &ActiveWidget::OnChar );
        _signalMap["onkeypress"]    = (SigPtr)( &ActiveWidget::OnKeyPress );
        _signalMap["onkeyrelease"]  = (SigPtr)( &ActiveWidget::OnKeyRelease );
        _signalMap["whilekey"]      = (SigPtr)( &ActiveWidget::WhileKey );
        _signalMap["whileoffkey"]   = (SigPtr)( &ActiveWidget::WhileOffKey );
        _signalMap["offchar"]       = (SigPtr)( &ActiveWidget::OffChar );
        _signalMap["offkeypress"]   = (SigPtr)( &ActiveWidget::OffKeyPress );
    }

    static const SignalMap_ActiveWidget & Instance()
    {
        static SignalMap_ActiveWidget s;
        return s;
    }
    
    Useless::Signal & GetSignalByName( std::string signalName, ActiveWidget *p ) const
    {
        SignalMap::const_iterator it = _signalMap.find( signalName );
        if ( it == _signalMap.end() )
        {
            throw Error("GetSignalByName: Signal not found %s.", signalName.c_str() );
        }
        
        SigPtr onSignal = (*it).second;
        Useless::Signal & s = ((*p).*onSignal);
        return s;
    }
};

struct SignalMap
{
    static const SignalMap & Instance()
    {
        static SignalMap s;
        return s;
    }

    Useless::Signal & GetSignalByName( std::string signalName, Widget *p ) const
    {
        if ( IWActive *a = dynamic_cast< IWActive *>(p) )
        {
            return SignalMap_ActiveWidget::Instance().GetSignalByName( signalName, &a->Control() );
        }

        throw Error("GetSignalByName: Unsupported widget type '%s'.", typeid(*p).name() );
    }
};


};//Useless
#endif//__INCLUDED_USELESS_RESOURCES_SIGNAL_MAP_H__
