#ifndef KVARCO_CRYPTED_ZIP_CONFIG_H_
#define KVARCO_CRYPTED_ZIP_CONFIG_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

/// ˆ³k‚·‚é‚È‚çdefine‚·‚é
/// ‚µ‚È‚¯‚ê‚ÎˆÃ†‰»‚³‚ê‚é
#define KVARCO_CRYPTED_ZIP_USE_COMP

/**
*	ˆ³k‚Ézlib‚ğ—p‚¢‚éê‡Adefine‚·‚é
*	define‚µ‚È‚¢‚È‚çgzip‚ªg‚í‚ê‚é
*/
#ifdef KVARCO_CRYPTED_ZIP_USE_COMP
#	define KVARCO_CRYPTED_ZIP_USE_ZLIB
#endif

#endif