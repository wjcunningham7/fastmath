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

#include "fastbitset.h"
#include <stdlib.h>

using namespace fastmath;

void initialize(FastBitset &f, FastBitset &g) {
    f.reset();
    g.reset();
    for (int i = 0; i < f.size() >> 1; i += 2)
        f.set(i);
    for (int i = f.size() >> 1; i < f.size(); i++)
        f.set(i);
    for (int i = 1; i < g.size(); i += 2)
        g.set(i);
}

int main(int argc, char **argv) {
    FastBitset f(128);
    FastBitset g(128);
    initialize(f, g);

    printf("Testing set intersection.\n");
    printf("Version 1:\n");
    f.printBitset();
    g.printBitset();
    f.setIntersection_v1(g);
    f.printBitset();

#ifdef AVX2_ENABLED
    printf("\nVersion 2:\n");
    initialize(f, g);
    f.printBitset();
    g.printBitset();
    f.setIntersection_v2(g);
    f.printBitset();
#endif

    printf("\nTesting partial intersection.\n");
    printf("First half:\n");
    initialize(f, g);
    f.printBitset();
    g.printBitset();
    f.partial_intersection(g, 0, 64);
    f.printBitset();

    printf("\nMiddle half:\n");
    initialize(f, g);
    f.printBitset();
    g.printBitset();
    f.partial_intersection(g, 32, 64);
    f.printBitset();

    printf("\nFinal half:\n");
    initialize(f, g);
    f.printBitset();
    g.printBitset();
    f.partial_intersection(g, 64, 64);
    f.printBitset();

    printf("\nVector product:\n");
    unsigned int N = 8192;
    FastBitset m(N);
    FastBitset n(N);
    srand(time(NULL));
    for (unsigned int i = 0; i < N; i++) {
        if ((float)rand() / RAND_MAX > 0.5)
            m.set(i);
        if ((float)rand() / RAND_MAX > 0.5)
            n.set(i);
    }

    uint64_t c = m.partial_vecprod(n, 0, N);
    printf("Count (VPD): %" PRIu64 "\n", c);
    m.partial_intersection(n, 0, m.size());
    c = m.count_bits();
    printf("Count (COR): %" PRIu64 "\n", c);

#ifdef AVX512_ENABLED
    printf("\nVector product (AVX512):\n");
    m.reset();
    n.reset();
    for (unsigned int i = 0; i < N; i++) {
        if ((float)rand() / RAND_MAX > 0.5)
            m.set(i);
        if ((float)rand() / RAND_MAX > 0.5)
            n.set(i);
    }

    c = m.partial_vecprod_avx512(n, 0, N);
    printf("Count (VPD512): %" PRIu64 "\n", c);
    c = m.partial_vecprod(n, 0, N);
    printf("Count (VPD256): %" PRIu64 "\n", c);
#endif
}
