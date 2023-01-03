#include "ProcessInfo.h"

#ifdef WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif

namespace hying
{
	int pid()
	{
#ifdef WIN32
		return static_cast<int>(GetCurrentProcessId());
#else
		return getpid();
#endif
	}

	std::string hostname()
	{
		char buf[256];
		if (::gethostname(buf, sizeof buf) == 0)
		{
			buf[sizeof(buf) - 1] = '\0';
			return buf;
		}
		else
		{
			return "unknownhost";
		}
	}
}