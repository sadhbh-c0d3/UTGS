#ifndef __INCLUDED_USELESS_DT_STATE_CHANGER_H__
#define __INCLUDED_USELESS_DT_STATE_CHANGER_H__

#include "Useless/GUI/Misc/DataTypes.h"

namespace Useless {

/*! Data Types Display State Changer - changes state of given DT instance
 *  -- Currently there are no data types supported --
 */
class USELESS_API DT_StateChanger :
    public Visitor< DataObject >
{
public:
    DT_StateChanger( int state_no ): _state_no(state_no) {}

    /*! Cannot change display state of this data object
     */
    virtual int Visit( DataObject & )
    {
        return 0;
    }

private:
    int _state_no;
};

};//namespace Useless
#endif//__INCLUDED_USELESS_DT_STATE_CHANGER_H__
