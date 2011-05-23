#include "detail.h"
namespace czip=kvarco::crypted_zip;

czip::detail::DiceSet& czip::detail::DiceSet::InitByKey(DWORD key)
{
	RandomGenerator.seed(key);

	//truncat to make sure (at least 7 times)
	int truncat;
	truncat=DiceDWORD()&0xffff	+7;
	for(int i=0; i<truncat; i++) DiceDWORD();

	truncat=DiceBYTE()&0xffff	+7;
	for(int i=0; i<truncat; i++) DiceBYTE();

	return *this;
}