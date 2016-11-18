// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <string>
#include <sstream>

#ifdef XSPECTRA_CRL
using namespace System::Threading;
#else
#include <thread>
#endif
