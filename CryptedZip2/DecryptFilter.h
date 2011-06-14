#ifndef KVARCO_CRYPTED_ZIP_DECRYPTED_FILTER_H_
#define KVARCO_CRYPTED_ZIP_DECRYPTED_FILTER_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#	pragma once
#endif

#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/operations.hpp>
#include "detail.h"

namespace kvarco
{
namespace crypted_zip
{	
namespace detail
{

template<typename BlockType>
class DecryptFilter_impl
	: public boost::iostreams::input_filter
{
	typedef BlockType type;

	type prev_block_;
	DiceSet& dices_;

	inline type FirstBlock()
		{
			return static_cast<type>((dices_.RollDice<DWORD>()^0xAC6BC4A1)^(dices_.RollDice<DWORD>()^0x677B048B));
		}

public:
	explicit DecryptFilter_impl(DiceSet& dices)
		:dices_(dices)
		{
			prev_block_=FirstBlock();
		}

	template<typename SourceType>
    int get(SourceType& src)
		{
			type chiper=boost::iostreams::get(src);

			type rand_byte	=dices_.RollDice<BYTE>();
			type tmp		=chiper^rand_byte;				//XOR with random byte
			//tmp				=BitRightRotate<type>(tmp,5);	//bit rotate
			type plain		=tmp^prev_block_;				//CBC mode

			prev_block_=chiper;
			return plain;
		}

	template<typename Sink>
    void close(Sink&)
		{
			prev_block_=0;
			dices_.InitByKey(0);
		}
};

typedef DecryptFilter_impl<char> DecryptFilter;

} //namespace detail
} //namespace crypted_zip
} //namespace kvarco

#endif
