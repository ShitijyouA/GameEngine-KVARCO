#ifndef KVARCO_ARCFOUR_ARCFOUR_H_
#define KVARCO_ARCFOUR_ARCFOUR_H_

#if defined(_MSC_VER)&&(_MSC_VER>=1020)
#	pragma once
#endif

#include <algorithm> // for std::swap
#include <boost/array.hpp>
#include <boost/range/algorithm/transform.hpp>

namespace kvarco
{
namespace arcfour
{

namespace detail
{

template<typename IntType>
IntType NearestMultiple(double x,IntType t)
{
	static const double neary_one=0.9999999999999999;
	return static_cast<IntType>(x/static_cast<double>(t)+neary_one)*t;
}

}

class Arcfour
{
public:
	typedef unsigned char Type;

private:
	friend class Arcfour;

	typedef unsigned short ImplType;

	bool do_encryption_;
	ImplType x_;
	ImplType y_;
	boost::array<ImplType,256> state_;

public:

	template<typename KeyType>
	explicit Arcfour(const KeyType& key)
		: x_(0),y_(0),do_encryption_(!key.empty())
	{
		if(!do_encryption_) return;

		ImplType state_index=0;
		ImplType key_index	=0;
		for(ImplType i=0; i<256; ++i) state_[i]=i;

		typedef unsigned long DWORD;
		const DWORD loop_time_tmp=detail::NearestMultiple<DWORD>(key.size(),256);
		const DWORD loop_time=(loop_time_tmp>2048) ? 2048:loop_time_tmp;
		for(ImplType i=0; i<loop_time; ++i)
		{
			state_index=(state_index+key[key_index]+state_[i])&0xff;
			std::swap(state_[state_index],state_[i]);

			if (++key_index>=key.size()) key_index=0;
		}
	}

	// move constractor
	void operator=(Arcfour& value)
	{
		do_encryption_	=value.do_encryption_;
		x_				=value.x_;
		y_				=value.y_;
		state_.swap(value.state_);
	}

	Type operator()()
	{
		if(!do_encryption_) return 0;

		ImplType x=(x_+1		)&0xff;
		ImplType y=(y_+state_[x])&0xff;

		x_=x;
		y_=y;
		std::swap(state_[x],state_[y]);

		return state_[(state_[x]+state_[y])&0xff]&0xff;
	}

};

namespace detail
{
	template<typename Type,typename KeyGenType>
	struct AssignXOR
	{
		KeyGenType& gen_;

		AssignXOR(KeyGenType& gen)
			:gen_(gen)
		{}

		Type operator()(const Type lvalue)
		{
			return lvalue^gen_();
		}
	};
}

// SrcRangeTypeはstd::istream_rangeなどを想定している
// Tips : vectorを出力バッファに使いたい場合はback_insert_iteratorを使うと良い
template<typename SrcRangeType,typename KeyType,typename DstIteratorType>
void EncryptByArcfour(const SrcRangeType& src_range, const KeyType& key, DstIteratorType& dst_ite)
{
	Arcfour gen(key);
	boost::transform(src_range,dst_ite,detail::AssignXOR<Arcfour::Type,Arcfour>(gen));
}

template<typename SrcRangeType,typename KeyType,typename DstIteratorType>
void DecryptByArcfour(const SrcRangeType& src_range, const KeyType& key, DstIteratorType& dst_ite)
{
	EncryptByArcfour(src_range,key,dst_ite);
}

template<typename SrcIteratorType,typename KeyType,typename DstIteratorType>
void EncryptByArcfour(const SrcIteratorType& src_begin, const SrcIteratorType& src_end, const KeyType& key, DstIteratorType& dst_ite)
{
	Arcfour gen(key);
	std::transform(src_begin,src_end,dst_ite,detail::AssignXOR<Arcfour::Type,Arcfour>(gen));
}

template<typename SrcIteratorType,typename KeyType,typename DstIteratorType>
void DecryptByArcfour(const SrcIteratorType& src_begin, const SrcIteratorType& src_end, const KeyType& key, DstIteratorType& dst_ite)
{
	EncryptByArcfour(src_begin,src_end,key,dst_ite);
}

}
}

#endif