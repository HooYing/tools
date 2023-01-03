#include "../src/Thread.h"

class TestThread : public hying::IThreadCallBack
{
public:
	TestThread()
		: m_thread(this)
	{

	}

	~TestThread()
	{

	}

	void start()
	{
		m_thread.start();
	}

private:

	void OnActive() override
	{
		while (m_thread.isRunning())
		{
			std::cout << "hello" << std::endl;
			Sleep(1000);
		}
	}


private:
	hying::Thread m_thread;

};

//int main()
//{
//	TestThread th;
//	th.start();
//	Sleep(10000);
//
//}