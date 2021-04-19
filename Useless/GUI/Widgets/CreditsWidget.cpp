#include "UselessPch.h"

#include "CreditsWidget.h"
#include "Useless/Functional/SignalSwitch.h"

namespace Useless {

CreditsWidget::CreditsWidget( const char *xml_file, int pixel_speed, int margins )
    : Paylist(xml_file), _advance(0), _margins(margins), _pixel_speed(pixel_speed)
{
    Tie2Signal( OnIdle, this, &CreditsWidget::Advance );
    Tie2Signal( OnVisible, this, &CreditsWidget::Restart );
}

CreditsWidget::CreditsWidget( XMLIterator i, int pixel_speed, int margins )
    : Paylist( i ), _advance(0), _margins(margins), _pixel_speed(pixel_speed)
{
    Tie2Signal( OnIdle, this, &CreditsWidget::Advance );
    Tie2Signal( OnVisible, this, &CreditsWidget::Restart );
}

CreditsWidget::~CreditsWidget()
{
}

void CreditsWidget::Setup()
{
    Restart();
}

void CreditsWidget::Repaint( const WidgetPainter &painter )
{
    int w = GetWidth();
    int m = (w*_margins)/100;
    Rect area( m, _advance, w-2*m, 0); //height is ommited

    Paylist::Paint( painter, area );
}

void CreditsWidget::Resize (int w, int h)
{
    SetDimensions( w, h );
    Restart();
}

void CreditsWidget::Restart()
{
    _advance = GetHeight();
    _time = Timer();
}

void CreditsWidget::Advance()
{
    Timer t;
    double dt = t - _time;
    _time = t;
    _advance -= _pixel_speed * dt;
    SetDirty();
}

bool CreditsWidget::Finished() const
{
    return _advance < -GetLength();
}


};//namespace Useless
