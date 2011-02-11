#pragma once

#include <OpenAL/alut.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <vorbis/vorbisfile.h>
#include <ogg/ogg.h>

#pragma comment( lib, "OpenAL32.lib" )
#pragma comment( lib, "alut.lib" )
#pragma comment( lib, "libvorbis_static.lib" )
#pragma comment( lib, "libvorbisfile_static.lib" )
#pragma comment( lib, "libogg_static.lib" )

#ifdef __BIG_ENDIAN__
#define ENDIAN 1
#else
#define ENDIAN 0
#endif