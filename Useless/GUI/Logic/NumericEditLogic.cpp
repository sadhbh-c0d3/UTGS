#include "UselessPch.h"

#include "Useless/GUI/Logic/NumericEditLogic.h"
#include <stdlib.h>
#include <sstream>

namespace Useless {

Types::QUAD to_quad( const std::string &s )
{
#   ifdef WIN32
    return _atoi64( s.c_str() );
#   else
    return atoll( s.c_str() );
#   endif
}

std::string from_quad( Types::QUAD x )
{
#   ifdef WIN32
    char buffer[20];
    return _i64toa( x, buffer, 10);
#   else
    std::stringstream ss;
    ss << x;
    return ss.str();
#   endif
}


NumericEditLogic::NumericEditLogic( Type t, double max_value )
    : _type(t), _min_value(0), _max_value(max_value)
{
    Tie2Signal( OnTextChange, this, &NumericEditLogic::ValidateNumber );
}

bool NumericEditLogic::Validate( int &ch ) const
{
    bool is_digit = (ch >= '0') && (ch <= '9');
    bool is_comma = ch == '.';
    bool was_comma = GetText().find('.') != -1;
    return (is_digit || _type==FLOAT && is_comma && !was_comma);
}

void NumericEditLogic::ValidateNumber()
{
    if ( _min_value && _max_value || !GetText().empty() )
    {
        SetMaxValue( _max_value );
        SetMinValue( _min_value );
    }
}

void NumericEditLogic::SetType( Type t )
{
    _type = t;
    if (_type==INTEGER)
    {
        Types::QUAD a = to_quad( GetText().GetString() );
        SetText( from_quad(a) );
    }
}

void NumericEditLogic::SetMaxValue( double x )
{
    _max_value = x;
    if ( GetNumber() > _max_value )
    {
        std::string s;
        std::stringstream interpreter;

        if (_type==INTEGER)
        {
            interpreter << from_quad(_max_value);
        }
        else
        {
            interpreter << _max_value;
        }

        interpreter >> s;
        SetText( s );
    }
    if ( _max_value < _min_value ) { SetMinValue(_max_value); }
}

void NumericEditLogic::SetMinValue( double x )
{
    _min_value = x;
    if ( GetNumber() < _min_value )
    {
        std::string s;
        std::stringstream interpreter;

        if (_type==INTEGER)
        {
            interpreter << from_quad(_min_value);
        }
        else
        {
            interpreter << _min_value;
        }

        interpreter >> s;
        SetText( s );
    }
    if ( _max_value < _min_value ) { SetMaxValue(_min_value); }
}

double NumericEditLogic::GetNumber() const
{
    double a;
    std::stringstream interpreter;
    interpreter << GetText().GetString();
    interpreter >> a;
    return a;
}

};//namespace Useless
