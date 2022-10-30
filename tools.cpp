// tools.cpp: 定义应用程序的入口点。
//

#include "dump/hyLog.h"

int main()
{
	//::SetUnhandledExceptionFilter(CDumpHelper::ExceptionHandler);
	struct timeval tv;
	gettimeofday(&tv, NULL);
	std::cout << tv.tv_sec << " " << tv.tv_usec << std::endl;

	std::cout << hying::Logger::logLevel() << std::endl;
	std::cout << hying::Logger::TRACE << std::endl;

	LOG_WARN << "trace";

	return 0;
}
