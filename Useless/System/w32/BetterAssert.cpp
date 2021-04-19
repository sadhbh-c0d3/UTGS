#include "UselessPch.h"

#include "BetterAssert.h"

#pragma message("To make Useless using Bugslayer library define: USELESS_HAS_BUGSLAYER")

#ifndef USELESS_HAS_BUGSLAYER
	int __stdcall DiagAssert ( unsigned long dwOverrideOpts,
                 const char * szMsg,
                 const char * szFile,
                 unsigned long dwLine,
                 AssertCallbackFunction *callbackFunction )
	{
		return 0;
	}
#endif

namespace Useless {
}; //Useless
