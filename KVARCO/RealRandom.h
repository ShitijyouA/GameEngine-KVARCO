#pragma once
#include <boost/random.hpp>
using namespace boost;

class CRealRandom
{
	mt19937			RandomGenerator;
	uniform_01<>	Uniform;

	typedef variate_generator<mt19937&,uniform_01<> > mt_01_gen;
	mt_01_gen		Dice;

	DWORD Seed_BackUp;

public:
	CRealRandom(DWORD seed)
		:RandomGenerator(seed)	,Dice(RandomGenerator,Uniform)
	{}

	CRealRandom()
		:RandomGenerator()		,Dice(RandomGenerator,Uniform)
	{}

	void Seed(DWORD seed)
		{	RandomGenerator.seed(seed);	}

	DWORD GetSeed()
		{	return Seed_BackUp;			}

	float Random(float min_,float max_)
	{
		if(min_>max_) return 0.0;
		return static_cast<float>(Dice()*abs(min_-max_)+min_);
	}
};