#ifndef __INCLUDED__USELESS__DATA_STREAM_H__
#define __INCLUDED__USELESS__DATA_STREAM_H__

#include "Useless/Util/Storage/PointerStorage.h"
#include "Useless/Util/Visitor.h"

namespace Useless {

//-- Completely abstract object
//-- It is visitable
//-- Using AcceptVisitor( Visitor & )
//-- is the only way to add virtual methods
class USELESS_API DataObject : public BaseVisitable<>
{
public:
    virtual ~DataObject() {}
    DEFINE_VISITABLE();
    virtual DataObject *CloneData() const = 0;

protected:
    DataObject() {}
private:
    //DataObject( const DataObject& );
};

//-- A list of DataObjects
typedef PtrStorage< DataObject >::Vector DataStream;
typedef InternalPointer< DataObject >    DataPointer;


};//namespace Useless
#endif//__INCLUDED__USELESS__DATA_STREAM_H__
