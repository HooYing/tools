#include "hyThread.h"

namespace hying
{
	bool Thread::Startup(IThreadCallBack* pCallBack)
	{
		if (DeActivate())
		{
			m_pCallBack = pCallBack;
			std::unique_lock<std::mutex> lck(m_mutex);
			m_thread = std::thread(std::bind(&Thread::run, this));
		}
		else
		{

		}
	}

	void Thread::run()
	{
		if (m_pCallBack == NULL)
		{

		}

		m_pCallBack->OnActivate(this);

		std::unique_lock<std::mutex> lck(m_mutex);
		m_condition.notify_all();
	}

	bool Thread::DeActivate()
	{
		
	}

	bool Thread::WaitForStop(long milliseconds)
	{
		if (milliseconds == -1)
		{
			std::unique_lock<std::mutex> lck(m_mutex);
			m_condition.wait(lck);
			return true;
		}
		else
		{
			std::unique_lock<std::mutex> lck(m_mutex);
			std::cv_status status = m_condition.wait_for(lck, std::chrono::milliseconds(milliseconds));
			if (status == std::cv_status::timeout)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}
}