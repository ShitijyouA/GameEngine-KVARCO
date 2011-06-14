#ifndef KVARCO_CRYPTED_ZIP_LOGGER_H_
#define KVARCO_CRYPTED_ZIP_LOGGER_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <string>
#include <iostream>

namespace kvarco
{
namespace crypted_zip
{

namespace concept
{
	struct Logger
	{
		virtual void operator()(const std::string&)=0;
		void Log(const std::string& str)
			{
				operator()(str);
			}
	};
} // namespace concept

namespace logger
{
	struct NullLogger : public concept::Logger
	{
		void operator()(const std::string&){}
	};

	struct StdLogger : public concept::Logger
	{
		void operator()(const std::string& msg)
			{
				std::cout<<msg<<std::endl;
			}
	};

} // namespace detail
} // namespace crypted_zip
} // namespace kvarco

#endif