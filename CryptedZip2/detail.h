#ifndef KVARCO_CRYPTED_ZIP_DETAIL_H_
#define KVARCO_CRYPTED_ZIP_DETAIL_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/uniform_int.hpp>

#ifndef BOOST_FILESYSTEM_VERSION
	#define BOOST_FILESYSTEM_VERSION 3
#endif
#include <boost/filesystem/path.hpp>

#include <boost/range/iterator_range.hpp>

#include <boost/interprocess/mapped_region.hpp>

#include <boost/type_traits/make_unsigned.hpp>

#include <limits>	//for CHAR_BIT
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
	template<typename Type>
	inline Type GetLowBitsMask(BYTE n)
	{
		if(n>sizeof(Type)*CHAR_BIT) return static_cast<Type>(0xFFFFFFFFFFFFFFFF);
		Type result=0;

		for(int i=0; i<n; ++i) result|=(1<<i);
		return result;
	}

	//if x is not unsigned type, becomes an arithmetic shift
	template<typename Type>
	inline Type BitLeftRotate(Type x__,BYTE rotate)
	{
		boost::make_unsigned<Type>::type x=x__;
		return (x<<rotate | x>>((sizeof(Type)*CHAR_BIT)-rotate));
	}

	template<typename Type>
	inline Type BitRightRotate(Type x__,BYTE rotate)
	{
		boost::make_unsigned<Type>::type x=x__;
		return (x>>rotate | x<<(sizeof(Type)*CHAR_BIT-rotate));
	}

	class DiceSet
	{
		typedef rand::mt19937 RandomGeneratorType;
		
		template<typename NumType>
		struct GeneratorType
		{
			typedef typename rand::variate_generator<rand::mt19937&,boost::uniform_int<NumType> > Type;
		};

		RandomGeneratorType random_generator_;

	public:

		template<typename Type>
		Type RollDice()
			{
				boost::uniform_int<Type> uniform(0,std::numeric_limits<Type>::max());
				GeneratorType<Type>::Type Dice(random_generator_,uniform);

				return Dice();
			}

		DiceSet& InitByKey(DWORD key)
			{
				random_generator_.seed(key);

				//truncat to make sure (at least 7 times)
				int truncat=(RollDice<DWORD>()&0xff)+7;
				for(int i=0; i<truncat; i++) random_generator_();

				return *this;
			}
	};

	typedef std::list<fsys::path> PathList;

} //namespace detail
} //namespace crypted_zip
} //namespace kvarco

#endif