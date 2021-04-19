#include "UselessPch.h"

#include "ErrorDialog.h"

namespace Useless {

ErrorDialog::ErrorDialog()
{
   LPVOID lpMsgBuf;
    FormatMessage( 
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM | 
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPTSTR) &lpMsgBuf,
        0,
        NULL 
    );
    MessageBox( NULL, (LPCTSTR)lpMsgBuf, _T("Exception"), MB_OK | MB_ICONINFORMATION );
    LocalFree( lpMsgBuf );
}

};// namespace Useless
