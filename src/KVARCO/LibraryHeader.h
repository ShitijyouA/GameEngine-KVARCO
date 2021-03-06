﻿//STLヘッダ
#pragma once

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <ctime>

#include <utility>
#include <fstream>
#include <string>
//#include <list>
#include <vector>
#include <queue>
#include <algorithm>
//#include <sstream>
//#include <complex>

#define	STRICT
#include <windows.h>

//DxLib::
#include <DxLib.h>

#ifdef _DEBUG
#	pragma comment(lib,"DxLib_d.lib")
#else
#	pragma comment(lib,"DxLib.lib")
#endif

#undef max
#undef min

//boost::
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/key_extractors.hpp>
using namespace boost::multi_index;

#define BOOST_DATE_TIME_NO_LIB
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
namespace posix_time=boost::posix_time;

#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>
namespace fsys=boost::filesystem;

#include <boost/unordered_map.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/math/constants/constants.hpp>
namespace math=boost::math;

//boost.random
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/uniform_01.hpp>

//kvarco::crypted_zip
//#include "../CryptedZip/EncryptedZip.h"
#include "../CryptedZip/DecryptedZip.h"
#ifdef _DEBUG
#	pragma comment(lib,"../../lib/CryptedZip_d.lib")
#else
#	pragma comment(lib,"../../lib/CryptedZip.lib")
#endif

//xtal::
#define DONOT_USE_STRING_LT
#define XTAL_DEBUG_PRINT
#include <xtal.h>
#include <xtal_macro.h>

typedef unsigned long		DWORD;
typedef unsigned char		BYTE;
typedef unsigned short		WORD;

typedef long long int			LLONG;
typedef unsigned long long int	QWORD;
