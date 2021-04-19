#include "UselessPch.h"

#include "Useless/GUI/Widgets/ProgressBar.h"

namespace Useless {

ProgressBar::ProgressBar( int range, int size, const Skin &frame, const Skin &interrior, Orientation orient )
    :ProgressWidget( range ), _sp_frame(&CopyOf<Skin>(frame)), _sp_interrior(&CopyOf<Skin>(interrior))
{
    //Get common resize direction of both skins
    Pos mask = (*_sp_frame).GetDirMask() * (*_sp_frame).GetDirection();
    mask *= (*_sp_interrior).GetDirMask() * (*_sp_interrior).GetDirection();

    //Unify resize direction in both skins
    (*_sp_frame).SetDirMask(mask); 
    (*_sp_interrior).SetDirMask(mask);

    if ( mask.y & !mask.x ) { orient=VERTICAL; }

    switch( orient )
    {
        case HORIZONTAL: assert(mask.x); _horizontal=true; break;
        case VERTICAL:   assert(mask.y); _horizontal=false; break;
    }
    SetDimensions( (*_sp_frame).GetWidth(), (*_sp_frame).GetHeight() );
}

ProgressBar::~ProgressBar()
{
}

void ProgressBar::Repaint( const WidgetPainter &painter )
{
    (*_sp_frame).Paint(painter);

    int w = (*_sp_frame).GetWidth();
    int h = (*_sp_frame).GetHeight();
    int x = GetPixelAdvance();
    Pos d = (_horizontal)? Pos(x,h) : Pos(w,x);
    Pos p = (_horizontal)? Pos(0,0) : Pos(0, h-x );

    (*_sp_interrior).SetSize(d);
    (*_sp_interrior).Paint(painter,p);
}

void ProgressBar::Resize( int w, int h )
{
    Dim2i d( w, h );
    (*_sp_frame).SafeResize( d );
    (*_sp_interrior).SafeResize( d );
    SetDimensions( (*_sp_frame).GetWidth(), (*_sp_frame).GetHeight() );
}

int ProgressBar::GetSize() const
{
    return (_horizontal)? (*_sp_frame).GetWidth() : (*_sp_frame).GetHeight();
}

};//namespace Useless
