#ifndef KVARCO_CRYPTED_ZIP_DECRYPTED_FILTER_H_
#define KVARCO_CRYPTED_ZIP_DECRYPTED_FILTER_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/operations.hpp>
#include "detail.h"
#include "Arcfour.h"

namespace kvarco
{
namespace crypted_zip
{	
namespace detail
{

namespace ios=boost::iostreams;

template<typename CharType>
class CryptingFilter_impl
	: public ios::dual_use_filter
{
public:
	typedef CharType type;

private:
	//type prev_block_;
	arcfour::Arcfour arcfour_;

	inline type AssignXorKey(type chiper)
	{
#ifndef EOF
		static const type EOF=static_cast<type>(-1);
#endif
		type result=0;
		if(chiper==EOF)	result=chiper;
		else			result=chiper^arcfour_();

		return result;
	}

public:
	inline static type InitialBlock(DiceSet& dices)
		{
			return static_cast<type>((dices.RollDice<DWORD>()^0xAC6BC4A1)^(dices.RollDice<DWORD>()^0x677B048B));
		}

	CryptingFilter_impl(type prev_block,arcfour::Arcfour& arcfour)
		//:prev_block_(prev_block&0xff),arcfour_(arcfour)
		:arcfour_(arcfour)
	{}

	template<typename SourceType>
    int get(SourceType& src)
		{
			type chiper=ios::get(src);
			type result=AssignXorKey(chiper);

			//prev_block_=chiper;

			return result;
		}

	template<typename SinkType>
	bool put(SinkType& dest,type chiper)
		{	
			type result=chiper^arcfour_();

			//prev_block_=chiper;

			return ios::put(dest,result);
		}

	template<typename Sink>
	void close(Sink&,std::ios::openmode which)
		{
			//prev_block_=0;
		}
};

typedef CryptingFilter_impl<char>		CryptingFilter;
typedef CryptingFilter_impl<wchar_t>	wCryptingFilter;

template<typename StreamType>
struct CryptingFilterTypeByStreamType
{
	typedef typename ios::int_type_of<StreamType>::type CharType;
	typedef typename CryptingFilter_impl<CharType> type;
};

} //namespace detail
} //namespace crypted_zip
} //namespace kvarco

#endif
