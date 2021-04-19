#include "UselessPch.h"

#include "Useless/Graphic/Planes/AdvImageBuffer.h"

namespace Useless {

AdvImageBuffer::AdvImageBuffer(): ImageBuffer() 
{
    TieSignals();
}

AdvImageBuffer::AdvImageBuffer(int w, int h): ImageBuffer(w,h)
{
    TieSignals();
    SetClipper( Rect(0,0,w,h) );
}

AdvImageBuffer::AdvImageBuffer(const AdvImageBuffer& image)
    :ImageBuffer(image), _my_on_paint(image._my_on_paint),
    _simulated_region(image._simulated_region)
{
    _scheduled = image._scheduled;
    TieSignals();
}

AdvImageBuffer::AdvImageBuffer(const AdvImageBuffer& image, const Rect &rect):
    ImageBuffer(image, rect), _my_on_paint(image._my_on_paint)
{
    _scheduled = image._scheduled;
    TieSignals();
}

AdvImageBuffer::~AdvImageBuffer()
{}

void AdvImageBuffer::TieSignals()
{
    _simulated_region = new RectList();
    if ( (bool)_my_on_paint ) { OnPaint.Untie( (int)_my_on_paint ); }
    _my_on_paint = OnPaint.TieVoid(this, &AdvImageBuffer::Flush );
}

void AdvImageBuffer::Flush()
{
    _simulated_region->clear();
    _scheduled.Execute(*this);
    _scheduled.Clear();
}

void AdvImageBuffer::Clear( NormalPixel color, const Rect &area )
{
    if ( ImageBuffer::IsValid() )
    {
        ImageBuffer::Clear( color, area );
    }
    else
    {
        _scheduled.AddDuty( &ImageBuffer::Clear, color, area );
    }
}

void AdvImageBuffer::SetClipper(const Useless::RectList &rl)
{
    if ( ImageBuffer::IsValid() )
    {
        ImageBuffer::SetClipper( rl );
    }
    else
    {
        _simulated_region->clear(); (*_simulated_region)=rl;
        _scheduled.AddDuty( (void (ImageBuffer::*)(const RectList&) )&ImageBuffer::SetClipper, rl );
    }
}

RectList AdvImageBuffer::GetClipper( const Rect &crop ) const
{
    if ( ImageBuffer::IsValid() )
    {
        return ImageBuffer::GetClipper( crop );
    }
    else
    {
        if ( !crop )
            return *_simulated_region;
        else
        {
            RectList::const_iterator i = (*_simulated_region).begin(),
                i2=(*_simulated_region).end();

            RectList out;

            for ( ; i!=i2; ++i )
            {
                Rect r = crop & (*i);
                if (!!r)
                {   out.push_back( r );
                }
            }
            return out;
        }
    }
}

};//namespace Useless
