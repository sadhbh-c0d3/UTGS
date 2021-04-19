#ifndef __INCLUDED__USELESS_LINEWRAPPER_H__
#define __INCLUDED__USELESS_LINEWRAPPER_H__

#include "Useless/GUI/Text/TextInfo.h"
#include "Useless/GUI/Text/Text.h"

namespace Useless {

/*! LineWrapper tries to best fit words into lines of limited width
 */
template< class WordStorage >
class LineWrapper
{
public:
    //template<>
        LineWrapper() {}

        LineWrapper( const WordStorage &word_storage, int width, const Text &text );

    typedef TextInfo::Row Row;
    const Row&  GetRow( int n) const { assert(n>=0 && n<_rows.Size()); return _rows[n]; }
           int  GetRowHeight( int n) const { return GetRow(n).height; }
           int  GetNumRows() const { return _rows.Size(); }
           int  GetWidth () const { return _width;  }
           int  GetHeight() const { return _height; }

private:
    StupidVector<Row>  _rows;
    int                _width;
    int                _height;
};


#include "Useless/GUI/Text/LineWrapper_inline.h"

};//namespace Useless
#endif//__INCLUDED__USELESS_LINEWRAPPER_H__
