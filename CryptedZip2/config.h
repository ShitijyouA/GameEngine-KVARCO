#ifndef KVARCO_CRYPTED_ZIP_CONFIG_H_
#define KVARCO_CRYPTED_ZIP_CONFIG_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

/// ���k����Ȃ�define����
/// ���Ȃ���ΈÍ��������
#define KVARCO_CRYPTED_ZIP_USE_COMP

/**
*	���k��zlib��p����ꍇ�Adefine����
*	define���Ȃ��Ȃ�gzip���g����
*/
#ifdef KVARCO_CRYPTED_ZIP_USE_COMP
#	define KVARCO_CRYPTED_ZIP_USE_ZLIB
#endif

#endif