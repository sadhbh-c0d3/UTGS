#include "Useless/UselessConfig.h"

/* Just check if we've got threads on board
------------------------------------------------------------*/
/* Just check if we've got threads on board
------------------------------------------------------------*/
#if (defined(__MT__) || defined(_MT) || defined(_REENTRANT) || defined(_PTHREADS))
#   ifndef BOOST_HAS_THREADS
#       define BOOST_HAS_THREADS
#   endif
#   ifndef USELESS_HAS_THREADS
#       define USELESS_HAS_THREADS
#   endif
#endif

/*
#ifdef USELESS_HAS_THREADS
# ifndef THREADMON_LIB
#   ifndef THREADMON_LIB_PATH
#       define THREADMON_LIB_PATH "../Tools"
#   endif
#   ifdef NDEBUG
#       define THREADMON_LIB THREADMON_LIB_PATH##"/Release/boost_threadmon.lib"
#   else
#       define THREADMON_LIB THREADMON_LIB_PATH##"/Debug/boost_threadmon.lib"
#   endif
#   pragma comment (lib, THREADMON_LIB)
# endif
#endif*/

#ifdef WIN32
#   pragma comment (lib,"kernel32.lib")
#   pragma comment (lib,"user32.lib")
#   pragma comment (lib,"gdi32.lib")

#   pragma comment (lib,"winmm.lib")
#   pragma comment (lib,"winspool.lib")
#   pragma comment (lib,"comdlg32.lib")
#   pragma comment (lib,"advapi32.lib")
#   pragma comment (lib,"shell32.lib")
#   pragma comment (lib,"ole32.lib")
#   pragma comment (lib,"oleaut32.lib")
#   pragma comment (lib,"uuid.lib")
#   pragma comment (lib,"odbc32.lib")
#   pragma comment (lib,"odbccp32.lib")

#   pragma comment (lib,"d3d9.lib")
#   pragma comment (lib,"d3dx9.lib")

#   pragma comment (lib,"dxguid.lib")
#   pragma comment (lib,"dsound.lib")

#   ifdef USE_WINGDI
#       pragma comment(lib,"Gdi32.lib")
#       pragma comment(lib,"Msimg32.lib")
#   endif
#endif



