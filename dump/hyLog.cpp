#include "hyLog.h"

namespace hying 
{
	const char digits[] = "9876543210123456789";
	const char* zero = digits + 9;
	static_assert(sizeof(digits) == 20, "wrong number of digits");

	const char digitsHex[] = "0123456789ABCDEF";
	static_assert(sizeof digitsHex == 17, "wrong number of digitsHex");

	template<typename T>
	size_t convert(char buf[], T value)
	{
		T i = value;
		char* p = buf;

		do
		{
			int lsd = static_cast<int>(i % 10);
			i /= 10;
			*p++ = zero[lsd];
		} while (i != 0);

		if (value < 0)
		{
			*p++ = '-';
		}
		*p = '\0';
		std::reverse(buf, p);

		return p - buf;
	}

	size_t convertHex(char buf[], uintptr_t value)
	{
		uintptr_t i = value;
		char* p = buf;

		do
		{
			int lsd = static_cast<int>(i % 16);
			i /= 16;
			*p++ = digitsHex[lsd];
		} while (i != 0);

		*p = '\0';
		std::reverse(buf, p);

		return p - buf;
	}

	template<int SIZE>
	const char* FixedBuffer<SIZE>::debugString()
	{
		*cur_ = '\0';
		return data_;
	}

	template<int SIZE>
	void FixedBuffer<SIZE>::cookieStart()
	{
	}

	template<int SIZE>
	void FixedBuffer<SIZE>::cookieEnd()
	{
	}

	void LogStream::staticCheck()
	{
		static_assert(kMaxNumericSize - 10 > std::numeric_limits<double>::digits10,
			"kMaxNumericSize is large enough");
		static_assert(kMaxNumericSize - 10 > std::numeric_limits<long double>::digits10,
			"kMaxNumericSize is large enough");
		static_assert(kMaxNumericSize - 10 > std::numeric_limits<long>::digits10,
			"kMaxNumericSize is large enough");
		static_assert(kMaxNumericSize - 10 > std::numeric_limits<long long>::digits10,
			"kMaxNumericSize is large enough");
	}

	template<typename T>
	void LogStream::formatInteger(T v)
	{
		if (buffer_.avail() >= kMaxNumericSize)
		{
			size_t len = convert(buffer_.current(), v);
			buffer_.add(len);
		}
	}

	LogStream& LogStream::operator<<(const void* p)
	{
		uintptr_t v = reinterpret_cast<uintptr_t>(p);
		if (buffer_.avail() >= kMaxNumericSize)
		{
			char* buf = buffer_.current();
			buf[0] = '0';
			buf[1] = 'x';
			size_t len = convertHex(buf + 2, v);
			buffer_.add(len + 2);
		}
		return *this;
	}


	
}