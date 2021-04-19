#include "UselessPch.h"

#include "Useless/GUI/Text/NamedFontSet.h"

namespace Useless {

NamedFontSet::NamedFontSet()
{}

NamedFontSet::~NamedFontSet()
{}

void  NamedFontSet::Insert( const Text &symbolic_name, const Font &font )
{
    assert( !Exists(symbolic_name) );
    SetName( FontSet::Insert( font), symbolic_name );
}

void  NamedFontSet::SetName( int id, const Text &symbolic_name )
{
    assert( Exists(id) );
    
    NameMap::ConstIterator iter;
    if ( !!_symbolic_names.FindID(id, &iter) ) { _symbolic_names.RemoveAt(iter); }

    _symbolic_names[symbolic_name] = id;
}

int  NamedFontSet::GetByName( const Text &symbolic_name ) const
{
    int *p_id = _symbolic_names.Find(symbolic_name);

    return (!p_id )? -1 : *p_id;
}

bool  NamedFontSet::Exists( const Text &symbolic_name )  const
{
    int *p_id;
    if ( p_id = _symbolic_names.Find(symbolic_name) )
    {
        return Exists( !p_id );
    }
    return false;
}

};//namespace Useless
