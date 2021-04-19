#ifndef __INCLUDED_USELESS_DATA_FILTER_H__
#define __INCLUDED_USELESS_DATA_FILTER_H__

#include "Useless/Util/SPointer.h"
#include "Useless/Util/Visitor.h"

namespace Useless {

/*! DataFilter - common interface for any data filters
 *  used to filter data in database to show it in data table.
 *
 *  It is only a structure, database will know what to do with
 *  concrete instance of it.
 */
class USELESS_API DataFilter : public BaseVisitable<>
{
public:
    virtual ~DataFilter() {}
    DEFINE_VISITABLE();

    SPointer< DataFilter > _previous;

protected:
    DataFilter(){}
private:
    DataFilter( const DataFilter& );
};


};// namespace Useless

#endif//__INCLUDED_USELESS_DATA_FILTER_H__
