#ifndef __FCH_H__
#define __FCH_H__

#include <iostream>

#ifdef WIN32
#define UNICODE
#include <Windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#endif

#endif