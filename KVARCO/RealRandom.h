#pragma once

class RealRandom
{
	boost::mt19937		RandomGenerator;
	boost::uniform_01<>	Uniform;

	typedef boost::variate_generator<boost::mt19937&,boost::uniform_01<> > mt_01_gen;
	mt_01_gen		Dice;

	DWORD Seed_BackUp;

public:
	RealRandom(DWORD seed)
		:
		RandomGenerator(seed)			,
		Dice(RandomGenerator,Uniform)	,
		Seed_BackUp(0)
	{}

	RealRandom()
		:
		RandomGenerator(0)				,
		Dice(RandomGenerator,Uniform)	,
		Seed_BackUp(0)
	{}

	void Seed(DWORD seed)
		{
			RandomGenerator.seed(seed);
			Seed_BackUp=seed;
		}

	DWORD GetSeed()
		{
			return Seed_BackUp;
		}

	float Random(float min_,float max_)
	{
		if(min_>max_) return 0.0;
		return static_cast<float>(Dice()*abs(min_-max_)+min_);
	}
};