#ifndef __HYTHREAD_H__
#define __HYTHREAD_H__

#include "pch.h"
#include <thread>
#include <condition_variable>

namespace hying
{
	// 这样封装可以在一个类中创建多个线程
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
