// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#import "C:\Program Files\Common Files\System\ADO\msado15.dll" \
	no_namespace rename("EOF", "EndOfFile")

#include <stdio.h>
#include <tchar.h>
#include <ole2.h>
