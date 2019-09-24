#ifndef FASTMATH_MERSENNE_H
#define FASTMATH_MERSENNE_H

/////////////////////////////
//(C) Will Cunningham 2018 //
//    Perimeter Institute  //
/////////////////////////////

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

namespace fastmath {

typedef boost::mt19937 Engine;
typedef boost::uniform_real<double> UDistribution;
typedef boost::variate_generator< Engine &, UDistribution > UGenerator;

class MersenneRNG {
public:
	MersenneRNG() : dist(0.0, 1.0), rng(eng, dist)  {}
	~MersenneRNG() {}

	void seed(long _seed)
	{
		rng.engine().seed(_seed);
		rng.distribution().reset();
	}

	void thermalize(uint64_t iterations)
	{
		for (unsigned int i = 0; i < iterations; i++)
			rng();
	}

	Engine eng;
	UDistribution dist;
	UGenerator rng;
};

}

#endif

