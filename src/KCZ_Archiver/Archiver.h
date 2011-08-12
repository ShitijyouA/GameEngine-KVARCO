#ifndef KVARCO_ARCHIVER_H_
#define KVARCO_ARCHIVER_H_

#define _CRT_SECURE_NO_WARNINGS

#include "../CryptedZip/EncryptedZip.h"
#ifdef _DEBUG
#	pragma comment(lib,"../../lib/CryptedZip_d.lib")
#else
#	pragma comment(lib,"../../lib/CryptedZip.lib")
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
#	pragma comment(lib,"xtallib_d.lib")
#else
#	pragma comment(lib,"xtallib.lib")
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

typedef std::pair<fsys::path,fsys::path> AbsAndRltPathType;
typedef std::list<AbsAndRltPathType> PathListType;

void InitXtal();
bool CompileAllXtalScript(PathListType& path_listz,const fsys::path& dst_dir);
void InitEncryptedZip();
void InsertAllFile(const fsys::path& parent_folder,PathListType& path_listz);

//
typedef ipc::unique_ptr
	<
		kcz::EncryptedZip,
		boost::checked_deleter<kcz::EncryptedZip>
	> EncryptedZipPtr;

#endif
