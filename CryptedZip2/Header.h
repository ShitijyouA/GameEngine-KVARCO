#ifndef KVARCO_CRYPTED_ZIP_HEADER_H_
#define KVARCO_CRYPTED_ZIP_HEADER_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#ifndef BOOST_FILESYSTEM_VERSION
	#define BOOST_FILESYSTEM_VERSION 3
#endif
#include <boost/filesystem/path.hpp>
#include <boost/unordered_map.hpp>
#include <boost/cstdint.hpp>

#include <list>

namespace kvarco
{
namespace crypted_zip
{
	
typedef unsigned long DWORD;
typedef unsigned char BYTE;
typedef boost::intmax_t intmax_t;
namespace fsys	=boost::filesystem;
namespace header
{

struct TerminalHeader
{
	typedef char* HeaderType;

	HeaderType	header_;

	DWORD		file_num_;
	intmax_t	central_headers_pos_;
};

struct CentralHeader
{
	typedef fsys::path::value_type NameCharType;

	intmax_t	data_pos_;
	intmax_t	size_;
	DWORD		key_;

	DWORD			name_len_;
	NameCharType*	name_;
};

typedef std::pair<fsys::path,CentralHeader>				PathAndCentralHeader;
typedef std::list<PathAndCentralHeader>					CentralHeaderList;
typedef boost::unordered_map<fsys::path,CentralHeader>	CentralHeaderMap;

} //namespace header
} //namespace crypted_zip
} //namespace kvarco

#endif