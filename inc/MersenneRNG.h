#ifndef MERSENNE_RNG_H
#define MERSENNE_RNG_H

/////////////////////////////
//(C) Will Cunningham 2018 //
//    Perimeter Institute  //
/////////////////////////////

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

typedef boost::mt19937 Engine;
typedef boost::uniform_real<double> UDistribution;
typedef boost::variate_generator< Engine &, UDistribution > UGenerator;

struct MersenneRNG {
	MersenneRNG() : dist(0.0, 1.0), rng(eng, dist)  {}

	Engine eng;
	UDistribution dist;
	UGenerator rng;
};

#endif

