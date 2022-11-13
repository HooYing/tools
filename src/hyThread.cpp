#include "hyThread.h"
#include "hyCurrentThread.h"

namespace hying
{
	std::atomic_int Thread::numCreated;
	Thread::Thread(IThreadCallBack* pCallBack, const std::string& name)
		: m_pCallBack(pCallBack),
		  m_name(name),
		  m_tid(0),
		  m_running(false),
		  m_cond(1)
	{
		int num = ++numCreated;
		if (m_name.empty())
		{
			char buf[32];
			snprintf(buf, sizeof buf, "Thread%d", num);
			m_name = buf;
		}
	}

	Thread::~Thread()
	{
		std::cout << "~Thread()" << std::endl;
		if (m_running)
		{
			m_thread.detach();
		}
	}

	void Thread::start()
	{
		assert(!m_running);
		m_running = true;
		m_thread = std::thread(&Thread::_run, this);
		m_cond.wait();
		assert(m_tid > 0);
	}

	void Thread::stop()
	{
		m_running = false;
	}

	bool Thread::isRunning()
	{
		return m_running;
	}

	void Thread::_run()
	{
		m_tid = CurrentThread::tid();
		m_cond.countDown();
		// 设置线程名
		CurrentThread::t_threadName = m_name.empty() ? "hyingThread" : m_name.c_str();
#ifndef WIN32
		::prctl(PR_SET_NAME, CurrentThread::t_threadName);
#else
		SetThreadDescription(GetCurrentThread(), gbk_to_utf16(CurrentThread::t_threadName).c_str());
#endif

		try {
			m_pCallBack->OnActive(this);
			m_running = false;
			CurrentThread::t_threadName = "finished";
		}
		catch (const std::exception& ex) {
			CurrentThread::t_threadName = "crashed";
			fprintf(stderr, "exception caught in Thread %s\n", m_name.c_str());
			fprintf(stderr, "reason: %s\n", ex.what());
			abort();
		}
		catch (...) {
			CurrentThread::t_threadName = "crashed";
			fprintf(stderr, "unknown exception caught in Thread %s\n", m_name.c_str());
			throw; //重新丢出
		}
	}

}