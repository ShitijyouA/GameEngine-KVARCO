#ifndef KVARCO_CRYPTED_ZIP_CONFIG_H_
#define KVARCO_CRYPTED_ZIP_CONFIG_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

/// 圧縮するならdefineする
/// しなければ暗号化される
#define KVARCO_CRYPTED_ZIP_USE_COMP

/**
*	圧縮にzlibを用いる場合、defineする
*	defineしないならgzipが使われる
*/
#ifdef KVARCO_CRYPTED_ZIP_USE_COMP
#	define KVARCO_CRYPTED_ZIP_USE_ZLIB
#endif

#endif