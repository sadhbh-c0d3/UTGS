#ifdef WIN32

#pragma warning(disable:4786)       // disable 'indentifier was truncated...'

#define MEAN_AND_LEAN
#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <winuser.h>
#include <zmouse.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <string>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <memory>

#include "Useless/UselessConfig.h"

#include "Useless/Functional/Functor.h"
#include "Useless/Functional/PackedFunctor.h"
#include "Useless/Functional/Holder.h"
#include "Useless/Functional/Signal.h"
#include "Useless/Functional/SignalSwitch.h"

#include "Useless/Util/CopyingFactory.h"

#include "Useless/Util/Shares.h"
#include "Useless/Util/CommonReference.h"

#include "Useless/Error/Error.h"
#include "Useless/Util/SPointer.h"

#include "Useless/XML/Resources/CreateFromXML.h"
#include "Useless/XML/Resources/CreateHints.h"
#include "Useless/XML/Resources/CreateFonts.h"
#include "Useless/XML/Resources/CreateSkins.h"
#include "Useless/XML/Resources/CreateStyle.h"
#include "Useless/XML/Resources/CreateText.h"
#include "Useless/XML/Resources/CreateWidgets.h"

#endif//WIN32

