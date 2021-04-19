#ifndef __INCLUDED_USELESS_RESOURCES_SIGNAL_EVENTS_H__
#define __INCLUDED_USELESS_RESOURCES_SIGNAL_EVENTS_H__

#include "Useless/XML/Resources/CreateFromXML.h"
#include "Useless/Functional/SignalEvent.h"

namespace Useless {

USELESS_RESOURCES_API Resource* CreatePlaySoundEvent( XMLIterator i, XMLCreatorEnv *env );
static RegisterXMLCreator reg_create_play_sound_event( "events::sound", CreatePlaySoundEvent );

typedef ResourceQuery< SPointer< SignalEvent > > SignalEventResource;

inline SPointer< SignalEvent > QuerySignalEvent( const std::string &id )
{
    SPointer< SignalEvent > *pt;
    Resources::Instance().Query("events", id, pt);
    return *pt;
}


};//Useless

#endif//__INCLUDED_USELESS_RESOURCES_SIGNAL_EVENTS_H__
