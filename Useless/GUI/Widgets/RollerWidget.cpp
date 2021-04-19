#include "UselessPch.h"

#include "Useless/GUI/Widgets/RollerWidget.h"
#include "Useless/Functional/SignalHelp.h"
#undef min
#undef max
#include <cmath>

namespace Useless {

RollerWidget::RollerWidget( const RefImage &ref_image, int range )
{
    _range = range;
    _value = 0;
    _offset = 0;
    _currentValue = 0;
    _rollSpeed = 1.0;
    _rollDir = 1.0;
    _sp_image = dynamic_cast<ImageBase*>( ref_image.get().Copy().Release() );
    SetDimensions( _sp_image->GetWidth(), _sp_image->GetHeight()/(_range+1) );
    Tie2Signal( OnIdle, this, &RollerWidget::Tick );
}

RollerWidget::~RollerWidget()
{
}

void RollerWidget::Repaint( const WidgetPainter& painter )
{    
    float k = std::max( 0.0f, std::min( (float)_currentValue, (float)_range ));
    int y = k * GetHeight();
    painter.ProjectImage( 0, -y, *_sp_image);
}

void RollerWidget::Resize ( int w, int h )
{
    _sp_image->SetDestWidth( w );
    _sp_image->SetDestHeight( h * (_range+1) );
    SetDimensions( w, h );
}

void RollerWidget::SetValue( int value, int rollDir )
{
    _rollDir = rollDir;
    _value = value;
    if ( 0 == rollDir )
    {
        _offset = 0;
        _currentValue = _value;
        while ( std::ceil( _currentValue ) > (float)_range )
        {
            _currentValue -= _range;
            _offset += _range;
        }
        while ( _currentValue < 0.f )
        {
            _currentValue += _range;
            _offset -= _range;
        }
    }
    Widget::SetDirty();
}

void RollerWidget::SetRollSpeed( float speed )
{
    _rollSpeed = speed;
}

void RollerWidget::Tick( int T, int DT )
{
    float realValue = _currentValue + (float)_offset;
    if ( _rollDir > 0 )
    {
        if ( realValue < (float)_value )
        {
            float inc = (DT/1000.0) * _rollSpeed;
            float maxInc = (float)_value - realValue;

            _currentValue += std::min( inc, maxInc );

            while ( std::ceil( _currentValue ) > (float)_range )
            {
                _currentValue -= _range;
                _offset += _range;
                OnOverRoll.Send();
            }
        }
        else
        {
            SetValue( _value, 0 );
        }
        Widget::SetDirty();
    }
    else if ( _rollDir < 0 )
    {
        if ( realValue > (float)_value )
        {
            float inc = (DT/1000.0) * _rollSpeed;
            float maxInc = realValue - (float)_value;

            _currentValue -= std::min( inc, maxInc );
            while ( _currentValue < 0.f )
            {
                _currentValue += _range;
                _offset -= _range;
                OnUnderRoll.Send();
            }
        }
        else
        {
            SetValue( _value, 0 );
        }
        Widget::SetDirty();
    }
}


ScoreRoller::ScoreRoller( const RollerWidget::RefImage &image, int numRollers )
{
    int w = image.get().GetWidth();
    int h = image.get().GetHeight();
    SetDimensions( numRollers * w, h/11 );

    for ( int i=0; i<numRollers; ++i )
    {
        RollerWidget *roller = new RollerWidget( image, 10 );
        Insert( roller, (numRollers - i - 1) * w, 0 );
        TieAt( roller->OnOverRoll, this, &ScoreRoller::OverRoll, i );
        TieAt( roller->OnUnderRoll, this, &ScoreRoller::UnderRoll, i );
        _rollers.push_back( roller );
    }
    _score = 0;
    _rollSpeed = 1.0;
}

ScoreRoller::~ScoreRoller()
{
}

void ScoreRoller::SetRollSpeed( float rollSpeed )
{
    _rollSpeed = rollSpeed;
}

void ScoreRoller::SetScore( int score )
{
    int dir = ( _score < score ? 1 : ( _score > score ? -1 : 0 ));
    if ( 0 == dir )
    {
        int numRollers = _rollers.size();
        for ( int i=0, base=1; i<numRollers; ++i, base *=10 )
        {
            int value = (score/base)%10;
            _rollers[i]->SetValue( value, 0);
        }
    }
    else
    {
        int numRollers = _rollers.size();
        std::vector< int > values( numRollers );
        std::vector< float > speeds( numRollers );
        int baseZero = 10;
        for ( int i=0; i<numRollers; ++i )
        {
            baseZero *= 10;
        }
        for ( int i=0, base=1; i<numRollers; ++i, base *=10 )
        {
            int oldValue = ((_score+baseZero)/base)%10;
            int value = ((score+baseZero)/base)%10;
            if ( oldValue != value )
            {
                int base1 = 10;
                for ( int j=0; j<i; ++j )
                {
                    values[i-j-1] += base1*dir;
                    base1 += 10;
                }
                values[i] = _rollers[i]->GetValue() + (value - oldValue);
            }
            else
            {
                values[i] = _rollers[i]->GetValue();
            }
            speeds[i] = _rollSpeed;
        }

        {
            int oldValue = ((_score+baseZero)/(baseZero/10))%10;
            int value = ((score+baseZero)/(baseZero/10))%10;
            if ( oldValue != value )
            {
                int base1 = 10;
                for ( int j=0; j<numRollers; ++j )
                {
                    values[numRollers-j-1] += base1*dir;
                    base1 += 10;
                }
            }
        }


        if ( dir > 0 )
        {
            for ( int i=1; i<numRollers; ++i )
            {
                float d1 = values[i-1] - _rollers[i-1]->GetValue();
                float d2 = values[i] - _rollers[i]->GetValue();
                if ( d1 > 0 && d2 > 0 )
                {
                    float k = d1 / d2;

                    for ( int j=0; j<i; ++j )
                    {
                        speeds[j] *= k;
                    }
                }
            }
        }
        else if ( dir < 0 )
        {
            for ( int i=1; i<numRollers; ++i )
            {
                float d1 = values[i-1] - _rollers[i-1]->GetValue();
                float d2 = values[i] - _rollers[i]->GetValue();
                if ( d1 < 0 && d2 < 0 )
                {
                    float k = d1 / d2;

                    for ( int j=0; j<i; ++j )
                    {
                        speeds[j] *= k;
                    }
                }
            }
        }

        for ( int i=0; i<numRollers; ++i )
        {
            _rollers[i]->SetValue( values[i], dir );
            _rollers[i]->SetRollSpeed( speeds[i] );
        }
    }

    _score = score;
}

void ScoreRoller::Resize ( int w, int h )
{
    HubWidget::Resize( w, h );

    float iw = w / _rollers.size();
    for ( int i=0; i<_rollers.size(); ++i )
    {
        int x = (_rollers.size() - i - 1) * iw;
        _rollers[ i ]->SetPosition( Pos( x, 0 ));
        _rollers[ i ]->Resize( iw, h );
    }
}


RollerWidget* ScoreRoller::GetRoller( int rollerNo )
{
    if ( _rollers.size() > rollerNo && rollerNo >=0 )
    {
        return _rollers[ rollerNo ];
    }
    else
    {
        return 0;
    }
}

void ScoreRoller::OverRoll( int rollerNo )
{
}

void ScoreRoller::UnderRoll( int rollerNo )
{
}




}; //Useless
