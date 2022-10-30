#ifndef __ENUM2STRING_H__
#define __ENUM2STRING_H__

#include "include/pch.h"

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif


class CEnum2String
{
public:
	template<typename T>
	static std::string enum2string(T name)
	{
		constexpr T tmp(static_cast<T>(name));
		return _enum2string<T, tmp>();
	}

public:
	template<typename T, T V>
	static std::string _enum2string()
	{
		std::string funStr = __PRETTY_FUNCTION__;
		int pos1 = funStr.find_last_of(',');
		int pos2 = funStr.find_last_of('>');
		return funStr.substr(pos1 + 1, pos2 - pos1 - 1);
	}
};

#endif