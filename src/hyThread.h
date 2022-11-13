#ifndef __HYTHREAD_H__
#define __HYTHREAD_H__

#include "pch.h"
#include <thread>
#include <functional>
#include "hyCondCount.h"

namespace hying
{
	class Thread;

	class IThreadCallBack
	{
	public:
		virtual void OnActive(Thread* pThread) = 0;
	};

	class Thread
	{
	public:
		explicit Thread(IThreadCallBack* pCallBack, const std::string& name = std::string());
		~Thread();

		void start();
		void stop();

		bool isRunning();

	private:

		void _run();

		CondCount m_cond;
		std::thread m_thread;
		int m_tid;
		std::string m_name;
		std::atomic<bool> m_running;
		IThreadCallBack* m_pCallBack;

		static std::atomic_int numCreated;
	};
}

#endif
