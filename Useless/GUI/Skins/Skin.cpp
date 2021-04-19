#include "UselessPch.h"

#include "Useless/System/Assert.h"
#include "Useless/GUI/Skins/Skin.h"
#include "Useless/ErrorConfig.h"

namespace Useless {

#ifndef NDEBUG
int Skin::_skin_counter = 0;
#endif


bool Skin::SafeResize( const Dim2i &d )
{
    Pos dir   = GetDirection() * GetDirMask();
    Pos ortho = Pos(1,1) - dir;
    Pos d1;
    d1.x = GetWidth();
    d1.y = GetHeight();
    d1 *= ortho;
    Pos d2 = d * dir;
    Pos result = d1 + d2;
    if ( !!result ) { SetSize( result); return true; }
    else { HUGE_LOG(Error("Cannot resize skin"),"Skin"); }
    return false;
}

Dim2i Skin::GetSize() const
{
    return Dim2i( GetWidth(), GetHeight() );
}

int Skin::GetLength() const
{
    return ( GetSize() * GetDirMask() * GetDirection() ).XSum();
}

void Skin::SetLength( int l )
{
    Pos dir = GetDirMask() * GetDirection();
    Dim2i d = dir * l + GetSize() * dir.Swap();
    SetSize(d);
}


};//namespace Useless
