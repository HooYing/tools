#ifndef __HYTHREAD_H__
#define __HYTHREAD_H__

#include "pch.h"
#include <thread>
#include <condition_variable>

namespace hying
{
	// ������װ������һ�����д�������߳�
	class Thread;

	class IThreadCallBack
	{
	public:
		virtual void OnActivate(Thread* pThread) = 0;
	};

	class Thread
	{
	public:
		Thread();
		virtual ~Thread();

		bool Startup(IThreadCallBack* pCallBack);

		bool DeActivate();

		bool WaitForStop(long milliseconds = -1);

	private:

		void run();
		
	private:
		
		mutable std::mutex m_mutex;
		IThreadCallBack* m_pCallBack;
		std::thread m_thread;
		std::condition_variable m_condition;
	};
}

#endif
