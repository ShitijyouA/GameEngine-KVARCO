#ifndef KVARCO_CRYPTED_ZIP_CONFIG_H_
#define KVARCO_CRYPTED_ZIP_CONFIG_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

/**	圧縮にzlibを用いる場合、defineする
*	defineしないならgzipが使われる
*/
#ifndef KVARCO_CRYPTED_ZIP_USE_ZLIB
//#define KVARCO_CRYPTED_ZIP_USE_ZLIB
#endif

#endif