#ifndef __INCLUDED__USELESS__NUMERIC_FIELD_H__
#define __INCLUDED__USELESS__NUMERIC_FIELD_H__

#include "Useless/GUI/Logic/NumericEditLogic.h"
#include "Useless/GUI/Widgets/TextField.h"
#include "Useless/GUI/Widgets/TemplateWidget.h"

namespace Useless {

//typedef TemplateWidget< TextField, NumericEditLogic > NumericField;
class USELESS_API NumericField : public TextField, virtual public NumericEditLogic
{
public:
    NumericField( int w, int h, const FontSet &fonts, const SkinMan &skins, const Text &text )
        :   TextField( w, h, fonts, skins, text )
        {}

    virtual bool Validate( int &ch ) const
        {
            if ( TextField::Validate( ch ))
            {
                return NumericEditLogic::Validate( ch );
            }
            else
            {
                return false;
            }
        }
};

};//namespace Useless
#endif//__INCLUDED__USELESS__NUMERIC_FIELD_H__
