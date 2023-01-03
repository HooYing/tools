#include "../src/AsyncLogging.h"
#include "../src/Log.h"

off_t kRollSize = 500 * 1000 * 1000;

hying::AsyncLogging* g_asyncLog = NULL;

void asyncOutput(const char* msg, int len)
{
    g_asyncLog->append(msg, len);
}

void bench(bool longLog)
{
    hying::Logger::setOutput(asyncOutput);

    int cnt = 0;
    const int kBatch = 1000;
    std::string empty = " ";
    std::string longStr(3000, 'X');
    longStr += " ";

    for (int t = 0; t < 30; ++t)
    {
        LOG_INFO << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz "
            << cnt;
        ++cnt;
    }
}

int main(int argc, char** argv)
{
	char name[256] = { '\0' };
	strncpy(name, argv[0], sizeof name - 1);
	hying::AsyncLogging log("hello", kRollSize);
	log.start();
	g_asyncLog = &log;

	bool longLog = argc > 1;
	bench(longLog);
}

