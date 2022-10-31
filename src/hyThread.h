#ifndef __HYTHREAD_H__
#define __HYTHREAD_H__

#include "pch.h"
#include <thread>
#include <functional>
#include "hyCondCount.h"

namespace hying
{
	class Thread
	{
	public:
		typedef	std::function<void()> ThreadFunc;
		explicit Thread(ThreadFunc, const std::string& name = std::string());
		~Thread();

		void start();
		void join();

		bool started() const { return m_started; }

		int tid() const { return m_tid; }

		const std::string& name() const { return m_name; }

		static int numCreated() { return m_numCreated; }

		
	private:
		
		void setDefaultName();
		bool m_started;
		bool m_joined;
		std::thread m_thread;
		int m_tid;
		ThreadFunc m_func;
		std::string m_name;
		CondCount m_latch;

		static std::atomic<int> m_numCreated;
		
	};
}

#endif
