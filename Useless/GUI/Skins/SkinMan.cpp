#include "UselessPch.h"

#include "Useless/GUI/Skins/SkinMan.h"
#include "Useless/ErrorConfig.h"

namespace Useless {

bool SkinMan::SafeResize( int id, const Dim2i& d)
{
    if ( Exists(id) )
    {
        Pos d2 = d * GetDirection(id) * GetDirMask(id);
        if ( !!d2 ) { SetSize( id, d2 ); return true; }

        else { APP_WARNING(Error("Cannot resize skin %i",id),"Skin"); }
    }
    else { APP_WARNING(Error("Cannot resize skin %i (no such skin)",id),"SkinMan"); }

    return false;
}

};//namespace Useless
