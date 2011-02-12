#pragma once
#define _CRT_SECURE_NO_WARNINGS

//STL
#include <ctime>
#include <cstdio>

#include <string>
#include <exception>
#include <list>
using namespace std;

//Boost
#include <boost/random.hpp>
#include <boost/foreach.hpp>
#include <boost/unordered_map.hpp>
#include <boost/optional.hpp>
#include <boost/assert.hpp>
using namespace boost;

#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>
using namespace boost::filesystem;

//dKingyoUtillity
#pragma warning(disable:4244)

#include <dkcCryptograph.h>
#ifdef _DEBUG
	#pragma comment(lib, "dkcCryptographd.lib")
	#pragma comment(lib, "dkcCommond.lib")
#else
	#pragma comment(lib, "dkcCryptograph.lib")
	#pragma comment(lib, "dkcCommon.lib")
#endif

//typedef
typedef unsigned long DWORD;
typedef unsigned char BYTE;