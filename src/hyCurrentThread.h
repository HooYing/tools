#ifndef __HYCURRENTTHREAD_H__
#define __HYCURRENTTHREAD_H__

#include "pch.h"

namespace hying
{
	namespace CurrentThread
	{
		extern __thread int t_cachedTid;
		extern __thread char t_tidString[32];
		extern __thread int t_tidStringLength;
		extern __thread const char* t_threadName;
		void cacheTid();

		inline int tid()
		{
			if (t_cachedTid == 0)
			{
				cacheTid();
			}
			return t_cachedTid;
		}

		inline const char* tidString() // for logging
		{
			return t_tidString;
		}

		inline int tidStringLength() // for logging
		{
			return t_tidStringLength;
		}

	}
}

#endif