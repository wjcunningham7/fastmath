/* Copyright 2014-2022 Will Cunningham
 *
 * This file is part of FastMath.
 *
 * Licensed under the MIT License (the "License"). A copy of the
 * License may be obtained with this software package or at
 *
 *     https://opensource.org/licenses/MIT
 *
 * FastMath is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef FASTMATH_MERSENNE_H
#define FASTMATH_MERSENNE_H

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

