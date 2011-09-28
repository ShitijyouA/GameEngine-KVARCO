#pragma once

template<typename RandGenType,typename ResType>
class BasicRealRandom
{
	typedef RandGenType												GeneratorType;
	typedef boost uniform_01<>										UniformType;
	typedef boost::variate_generator<GeneratorType&,UniformType>	UniformedGeneratorType;
	typedef GeneratorType::result_type								SeedType;
	typedef ResType													ResultType;

	GeneratorType generator_;
	UniformType uniform_;

	UniformedGeneratorType uniformed_generator_;

	SeedType backup_seed_;

public:
	RealRandom(SeedType seed=0)
		:
		generator_(seed)							,
		uniformed_generator_(generator_,uniform_)	,
		backup_seed_(seed)
		{}

	void Seed(SeedType seed)
		{
			generator_.seed(seed);
			backup_seed_=seed;
		}

	SeedType GetSeed()
		{
			return backup_seed_;
		}

	/// [min,max)
	ResultType Random(ResultType min,ResultType max)
		{
			if(min>max) return static_cast<ResultType>(0.0);
			return static_cast<ResultType>(uniformed_generator_()*std::abs(min-max)+min);
		}

	ResultType operator()(ResultType min,ResultType max)
		{
			return Random(min,max);
		}
};

typedef BasicRealRandom<boost::mt19937,kvarco::float_t> RealRandom;
