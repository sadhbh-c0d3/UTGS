#include "UselessPch.h"

#include "Useless/Graphic/Planes/IGraphics.h"
#include "Useless/Graphic/Planes/OGraphics.h"
#include "Useless/Graphic/Device/Surface.h"

namespace Useless {

void IGraphics::BlitTo( int x, int y, OGraphics& output )
{
    Surface &dest = *output.GetSurface();
    Cooperate( dest );

    Surf::BlitFX fx;
    fx.width = GetDestWidth();
    fx.height = GetDestHeight();
    fx.stretch = (fx.width != GetWidth()) || (fx.height != GetHeight());

    dest.Blit( x, y, *GetSurface(), GetRect(), fx );
}

void IGraphics::MultiBlitTo( OGraphics& output, const PointList &pnts, const RectList &rcs )
{
    Surface &dest = *output.GetSurface();
    Cooperate( dest );
    const Surface &source = *GetSurface();

    Surf::BlitFX fx;
    dest.MultiBlit( source, pnts, rcs, fx );
}

};
