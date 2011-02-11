//STLƒwƒbƒ_
#include <list>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <cfloat>
#include <map>
#include <utility>
#include <set>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <sstream>
#include <complex>
#include <stack>
#include <queue>

using namespace std;

#define	STRICT
#include <windows.h>

//DxLib::
#include <DxLib.h>

#ifdef _DEBUG
	#pragma comment(lib,"DxLib_d.lib")
#else
	#pragma comment(lib,"DxLib.lib")
#endif

//std::
#include <ctime>

//boost::
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/key_extractors.hpp>
using namespace boost;
using namespace boost::multi_index;

#include <boost/unordered_map.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

#define BOOST_DATE_TIME_NO_LIB
#include <boost/thread.hpp>

//xtal::
#define DONOT_USE_STRING_LT
#define XTAL_DEBUG_PRINT
#define _CRT_SECURE_NO_WARNINGS
#include <xtal.h>
#include <xtal_macro.h>

//Œ³TYEPDEF.h
typedef unsigned long		DWORD;
typedef unsigned char		BYTE;
typedef unsigned short		WORD;

typedef long long int			LLONG;
typedef unsigned long long int	QWORD;