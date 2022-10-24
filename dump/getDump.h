#ifndef __GETDUMP_H__
#define __GETDUMP_H__

#include "include/pch.h"
#include <DbgHelp.h>

#pragma comment(lib, "dbghelp.lib")

class CDumpHelper
{
public:
	static LONG ExceptionHandler(PEXCEPTION_POINTERS pExceptionPointers)
	{
		wchar_t processName[MAX_PATH] = { 0 };
		::GetModuleFileName(NULL, processName, MAX_PATH);
		::PathStripPath(processName);

		SYSTEMTIME st = { 0 };
		GetLocalTime(&st);

		wchar_t szFile[MAX_PATH] = { 0 };
		swprintf_s(szFile, L"%s_%d%02d%02d_%02d%02d%02d.dmp", processName,
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

		HANDLE hDmpFile = ::CreateFile(szFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ,
			0, CREATE_ALWAYS, 0, 0);
		MINIDUMP_EXCEPTION_INFORMATION stInfo = { 0 };
		stInfo.ThreadId = GetCurrentThreadId();
		stInfo.ExceptionPointers = pExceptionPointers;
		stInfo.ClientPointers = TRUE;

		return MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDmpFile, MiniDumpNormal, &stInfo, NULL, NULL);
	}
};


#endif