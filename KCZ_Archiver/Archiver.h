#ifndef KVARCO_ARCHIVER_H_
#define KVARCO_ARCHIVER_H_

#define _CRT_SECURE_NO_WARNINGS

#include "../CryptedZip2/EncryptedZip.h"
#ifdef _DEBUG
#	pragma comment(lib,"../lib/CryptedZip2_d.lib")
#else
#	pragma comment(lib,"../lib/CryptedZip2.lib")
#endif

//xtal::
#include <xtal.h>
#include <xtal_macro.h>

//Win32
#include <xtal_lib/xtal_cstdiostream.h>
#include <xtal_lib/xtal_winthread.h>
#include <xtal_lib/xtal_winfilesystem.h>
#include <xtal_lib/xtal_chcode.h>
#include <xtal_lib/xtal_errormessage.h>

#ifdef _DEBUG
	#pragma comment(lib,"xtallib_d.lib")
#else
	#pragma comment(lib,"xtallib.lib")
#endif

//std::
#include <list>
#include <iostream>
#include <string>

//boost::interprocess
#include <boost/interprocess/smart_ptr/unique_ptr.hpp>
#include <boost/checked_delete.hpp>

//boost::
#include <boost/cstdint.hpp>
#include <boost/lexical_cast.hpp>

//
namespace kcz	=kvarco::crypted_zip;
namespace ipc	=boost::interprocess;
namespace fsys	=boost::filesystem;

typedef std::list<fsys::path> PathListType;

void InitXtal();
bool CompileXtalScript(const fsys::path& src_path,const fsys::path& dst_path);
void InitEncryptedZip(PathListType& path_list);

//
typedef ipc::unique_ptr
	<
		kcz::EncryptedZip,
		boost::checked_deleter<kcz::EncryptedZip>
	> EncryptedZipPtr;

#endif