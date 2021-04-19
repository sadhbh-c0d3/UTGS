#include "UselessPch.h"

#include "Useless/Util/CopyingFactory.h"

namespace Useless {

RTCopyingFactory::__Creators *RTCopyingFactory::_creators;

INIT_PREINITIALIZER( RTCopyingFactory, RTCopyingFactory, USELESS_API );

/*! Register copier-creator-function.
 *  Creator-funcion is added to list only if it is not duplicated.
 *  One type (key) may have many unique registered creators.
 */
void RTCopyingFactory::Register( __Key key, __Creator creator )
{
    __Creators::Iterator it;

    _creators->Find( key, (__Creators::ConstIterator*)&it );

    for ( ; !!it && it.Key()==key; ++it )
    {
        if ( it.Value()==creator ) { return; } //< was duplicate copier
    }

    (*_creators).Insert( key, creator );
}

};//namespace Useless
