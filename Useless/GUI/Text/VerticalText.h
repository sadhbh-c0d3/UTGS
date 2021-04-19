#ifndef __INCLUDED_USELESS_VERTICAL_TEXT_H__
#define __INCLUDED_USELESS_VERTICAL_TEXT_H__

#include "Useless/GUI/Text/Text.h"

namespace Useless {

class USELESS_API VerticalText : public Text
{
public:
    VerticalText( const Text &text )
    {
        const UniCode *p=&*text.begin(), *q=&*text.end();
        for ( ; p!=q; )
        {
            operator += (*p);
            if (++p != q)
            {
                operator += ("\n");
            }
        }
    }
};

};//namespace Useless
#endif//__INCLUDED_USELESS_VERTICAL_TEXT_H__
