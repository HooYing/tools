#ifndef __FCH_H__
#define __FCH_H__

#include <iostream>
#include <assert.h>

#ifdef WIN32
#define UNICODE
#include <Windows.h>
#include <shlwapi.h>
#include <inttypes.h>
#define __STDC_FORMAT_MACROS
#pragma comment(lib, "shlwapi.lib")
#undef ERROR

static int gettimeofday(struct timeval* tp, void* tzp)
{
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tp->tv_sec = clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;
	return (0);
}

#define __thread thread_local
#define strerror_r(err,buf,len) strerror_s(buf,len,err)
#define gmtime_r(sec, tmt) gmtime_s(tmt, sec) 

#else
#include <sys/time.h>
#endif


#include <iostream>

constexpr int32_t basename_index(
	const char* const path, const int32_t index = 0, const int32_t slash_index = -1
)
{
	return path[index]
		? ((path[index] == '/' || path[index] == '\\')  // (see below)
			? basename_index(path, index + 1, index)
			: basename_index(path, index + 1, slash_index)
			)
		: (slash_index + 1)
		;
}

template <int32_t Value>
struct require_at_compile_time
{
	static constexpr const int32_t value = Value;
};

#define JUST_FILENAME (__FILE__ + require_at_compile_time<basename_index(__FILE__)>::value)


#endif
