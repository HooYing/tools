// tools.cpp: 定义应用程序的入口点。
//

#include "tools.h"
#include "dump/getDump.h"
#include "dump/enum2string.h"
using namespace std;

int main()
{
	::SetUnhandledExceptionFilter(CDumpHelper::ExceptionHandler);
	
	enum Color{red, green, blue};
	Color b = red;
	std::cout << CEnum2String::enum2string(b) << endl;

	return 0;
}
