#ifndef KVARCO_CRYPTED_ZIP_CRYPTED_FILTER_H_
#define KVARCO_CRYPTED_ZIP_CRYPTED_FILTER_H_

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

struct CryptFilter_base
	: public boost::iostreams::output_filter
{
	template<typename SinkType>
    bool put(SinkType& dest,int c) { return true; }
};

template<typename BlockType>
class CryptFilter_impl : public CryptFilter_base
{
	typedef BlockType type;

	type prev_block_;
	DiceSet& dices_;

	inline type FirstBlock()
		{
			return (dices_.DiceDWORD()^0xAC6BC4A1)^(dices_.DiceDWORD()^0x677B048B);
		}

public:
	explicit CryptFilter_impl(DiceSet& dices)
		:dices_(dices)
	{
		prev_block_=FirstBlock();
	}

	template<typename SinkType>
    bool put(SinkType& dest,int c)
		{
			type apply_CBC	=c^prev_block_;						//CBC mode
			type cipher		=BitLeftRotate<type>(apply_CBC,5);	//bit rotate
			cipher			=cipher^dices_.DiceDWORD();			//XOR with random byte

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

typedef CryptFilter_impl<int> CryptFilter;

//
struct NoCryptFilter : public CryptFilter_base
{
	template<typename SinkType>
    bool put(SinkType& dest,int c)
		{
			return boost::iostreams::put(dest,c);
		}

	template<typename Sink>
    void close(Sink&) {}
};

} //namespace detail
} //namespace crypted_zip
} //namespace kvarco

#endif