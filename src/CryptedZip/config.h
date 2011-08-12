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
//#	define KVARCO_CRYPTED_ZIP_USE_ZLIB
#endif

/**
*	親フォルダまでアーカイブする場合、defineする
*	ex.EncryptedZipに C:\sample\ を渡した場合
*	defineした場合	 : sample以下のファイル/フォルダがアーカイブされ、sampleフォルダは含まれない
*	defineしない場合 : sampleフォルダごとアーカイブされる
*	KVARCOで使用する場合はdefine
**/
#define KVARCO_CRYPTED_ZIP_REMOVE_PARENT_DIR

#endif
