#ifndef __INCLUDED_USELESS_LIBC_H__
#define __INCLUDED_USELESS_LIBC_H__
#include <crtdbg.h>

namespace Useless {

struct LibC {
    static void CrtDbgCheckAlways( bool en )
        {
            static int flags = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
            int newFlags;
            if ( en )
            {
                newFlags = flags | _CRTDBG_CHECK_ALWAYS_DF;
            }
            else
            {
                newFlags = (flags & 0x0000FFFF) | (0xffff << 16);
            }
            _CrtSetDbgFlag( newFlags );
        }
};

}; //Useless
#endif//__INCLUDED_USELESS_LIBC_H__
