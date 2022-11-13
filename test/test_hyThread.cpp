#include "../src/hyThread.h"

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

	void OnActive(hying::Thread* pThread) override
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

int main()
{
	TestThread th;
	th.start();
	Sleep(3000);
	return 0;
}