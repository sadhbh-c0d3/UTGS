#ifndef __INCLUDED_USELESS_PAYLIST_H__
#define __INCLUDED_USELESS_PAYLIST_H__

#include "Useless/Graphic/Planes/Painter.h"
#include "Useless/GUI/Misc/DataStream.h"
#include "Useless/GUI/Text/Font.h"
#include "Useless/XML/XMLParser.h"

namespace Useless {

/*! Simplified document type
 *  Each entry is supposed to be separate row.
 */
class USELESS_API Paylist : public DataStream
{
public:
    Paylist();
    Paylist( const char *paylist_file );
    Paylist( XMLIterator i );
    ~Paylist();

    //this ctor is subject to change
    Paylist( const Font &header_font, const Font &normal_font, int distance );

    void Paint( const Painter &painter, Rect area );

    //length is initialized after first Paint()
    int  GetLength() const { return _length; }

private:
    int   _distance;
    Font  _header_font;
    Font  _normal_font;
    int   _length;

    void LoadFromXML( XMLIterator i );
};

};//namespace Useless
#endif//__INCLUDED_USELESS_PAYLIST_H__
