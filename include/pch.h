#ifndef __FCH_H__
#define __FCH_H__

#include <iostream>
#include <assert.h>
#include <stdio.h>

#ifdef WIN32
#define UNICODE
#include <Windows.h>
#include <shlwapi.h>
#include <inttypes.h>

// 使用PRld64
#define __STDC_FORMAT_MACROS


#pragma comment(lib, "shlwapi.lib")

// 与日志的ERROR枚举重定义
#undef ERROR

#define __thread thread_local
#define strerror_r(err,buf,len) strerror_s(buf,len,err)
#define gmtime_r(sec, tmt) gmtime_s(tmt, sec) 

#else
#include <sys/time.h>
#endif


#endif
