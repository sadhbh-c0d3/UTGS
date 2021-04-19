#include "UselessPch.h"

#include "FontSelector.h"

namespace Useless {

void FontSelector::Paint( const Painter &painter, const Text &text, Pos p )
{
    _manager.Paint( _available_state, painter, text, p );
}

};//namespace Useless


