#pragma once

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/uniform_int.hpp>

#ifndef BOOST_FILESYSTEM_VERSION
	#define BOOST_FILESYSTEM_VERSION 3
#endif
#include <boost/filesystem/path.hpp>

#include <boost/range/iterator_range.hpp>

#include <boost/interprocess/mapped_region.hpp>

#include <climits>	//for CHAR_BIT
#include <utility>	//for std::pair

#include "Header.h"

namespace kvarco
{
namespace crypted_zip
{
	
typedef unsigned long DWORD;
typedef unsigned char BYTE;

namespace ipc	=boost::interprocess;
namespace fsys	=boost::filesystem;
namespace rand	=boost::random;

namespace detail
{
	template<class Type>
	boost::iterator_range<Type*> make_mapped_range(const ipc::mapped_region& region)
	{
		Type* begin	=reinterpret_cast<Type*>(region.get_address());
		Type* end	=reinterpret_cast<Type*>(region.get_address()) + region.get_size();

		return boost::iterator_range<Type*>(begin,end);
	}

	template<typename Type>
	inline Type BitLeftRotate(Type x,BYTE rotate)
	{
		return (x<<rotate | x>>((sizeof(Type)*CHAR_BIT)-rotate));
	}

	template<typename Type>
	inline Type BitRightRotate(Type x,BYTE rotate)
	{
		return (x>>rotate | x<<(sizeof(Type)*CHAR_BIT-rotate));
	}

	class DiceSet
	{
		typedef rand::variate_generator<rand::mt19937&,boost::uniform_int<DWORD> >	mt_DWORD_gen;
		typedef rand::variate_generator<rand::mt19937&,boost::uniform_int<BYTE> >	mt_BYTE_gen;
		
		rand::mt19937		RandomGenerator;

		boost::uniform_int<DWORD>	UniformDWORD;
		boost::uniform_int<BYTE>	UniformBYTE;

	public:
		mt_BYTE_gen		DiceBYTE;
		mt_DWORD_gen	DiceDWORD;

		DiceSet()
			:
				UniformDWORD(0,UINT_MAX),				UniformBYTE(0,UCHAR_MAX),
				DiceDWORD(RandomGenerator,UniformDWORD),DiceBYTE(RandomGenerator,UniformBYTE)
		{}

		//
		DiceSet& InitByKey(DWORD key);
	};

	typedef std::list<fsys::path> PathList;

} //namespace detail
} //namespace crypted_zip
} //namespace kvarco
