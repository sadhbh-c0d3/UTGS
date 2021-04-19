#include "ResourcesPch.h"

#include "Useless/XML/Resources/GUIBinder.h"
#include "Useless/XML/Resources/CreateSignalEvents.h"

namespace Useless {

void GUIBinder::BindTo( ActiveWidget *source )
{
    for ( BindDefinitions::iterator it = _defs.begin(); it != _defs.end(); ++it )
    {
        Useless::Signal &sigRef = SignalMap::Instance().GetSignalByName( (*it).first, source );
        SignalEventResource::Type event = QuerySignalEvent( (*it).second._eventToRaise );
        sigRef.Tie
            ( PackFunctor
              ( BinaryCall
//TODO: 'event.get()' might be unsafe here
                ( event.get(), &SignalEvent::Raise ), &sigRef, (*it).second._stringToPass )
            );
    }
}

};//Useless
