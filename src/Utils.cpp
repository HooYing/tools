#include "Utils.h"

namespace hying
{
	__thread char t_errnobuf[512];
	// ������������Ϣ
	const char* strerror_tl(int savedErrno)
	{
		strerror_r(savedErrno, t_errnobuf, sizeof t_errnobuf);
		return t_errnobuf;
	}

}