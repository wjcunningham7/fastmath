/* Copyright 2014-2022 Will Cunningham
 * 
 * This file is part of FastMath.
 *
 * Licensed under the GNU General Public License 3.0 (the "License").
 * A copy of the License may be obtained with this software package or at
 *
 *      https://www.gnu.org/licenses/gpl-3.0.en.html
 *
 * Use of this file is prohibited except in compliance with the License. Any
 * modifications or derivative works of this file must retain this copyright
 * notice, and modified files must contain a notice indicating that they have
 * been altered from the originals.
 *
 * FastMath is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE. See the License for more details. */

#ifndef FASTMATH_MERSENNE_H
#define FASTMATH_MERSENNE_H

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

namespace fastmath {

typedef boost::mt19937 Engine;
typedef boost::uniform_real<double> UDistribution;
typedef boost::variate_generator<Engine &, UDistribution> UGenerator;

class MersenneRNG {
  public:
    MersenneRNG() : dist(0.0, 1.0), rng(eng, dist) {}
    ~MersenneRNG() {}

    void seed(long _seed) {
        rng.engine().seed(_seed);
        rng.distribution().reset();
    }

    void thermalize(uint64_t iterations) {
        for (unsigned int i = 0; i < iterations; i++)
            rng();
    }

    Engine eng;
    UDistribution dist;
    UGenerator rng;
};

} // namespace fastmath

#endif
