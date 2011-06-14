#ifndef KVARCO_CRYPTED_ZIP_ENCRYPTED_FILTER_H_
#define KVARCO_CRYPTED_ZIP_ENCRYPTED_FILTER_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
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
class EncryptFilter_impl
	: public boost::iostreams::output_filter
{
	typedef BlockType type;

	type prev_block_;
	DiceSet& dices_;

	inline type FirstBlock()
		{
			return static_cast<type>((dices_.RollDice<DWORD>()^0xAC6BC4A1)^(dices_.RollDice<DWORD>()^0x677B048B));
		}

public:
	explicit EncryptFilter_impl(DiceSet& dices)
		:dices_(dices)
	{
		prev_block_=FirstBlock();
	}

	template<typename SinkType>
    bool put(SinkType& dest,int c)
		{
			type apply_CBC	=c^prev_block_;						//CBC mode
			type cipher		=apply_CBC;
			//chiper			=BitLeftRotate<type>(apply_CBC,5);	//bit rotate
			type rand_byte	=dices_.RollDice<BYTE>();
			cipher			=cipher^rand_byte;					//XOR with random byte

			prev_block_=cipher;
			return boost::iostreams::put(dest,cipher);
		}

	template<typename Sink>
    void close(Sink&)
		{
			prev_block_=0;
			dices_.InitByKey(0);
		}
};

typedef EncryptFilter_impl<char> EncryptFilter;

} //namespace detail
} //namespace crypted_zip
} //namespace kvarco

#endif