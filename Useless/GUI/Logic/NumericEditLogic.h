#ifndef __INCLUDED__USELESS__NUMERIC_EDIT_LOGIC_H__
#define __INCLUDED__USELESS__NUMERIC_EDIT_LOGIC_H__

#include "Useless/GUI/Logic/TextEditLogic.h"

namespace Useless {

class USELESS_API NumericEditLogic : virtual public TextEditLogic
{
public:
    enum Type { INTEGER, FLOAT };
    NumericEditLogic( Type=INTEGER, double max_value=2000000000 );

    //Validates incoming characters to be numbers
    virtual bool Validate( int &ch ) const;

    void SetType( Type );
    void SetMaxValue( double x );
    void SetMinValue( double x );

    double GetMinValue() const { return _min_value; }
    double GetMaxValue() const { return _max_value; }
    double GetNumber  () const;

private:
    Type   _type;
    double _max_value;
    double _min_value;

    void ValidateNumber();
};

};//namespace Useless
#endif//__INCLUDED__USELESS__NUMERIC_EDIT_LOGIC_H__
