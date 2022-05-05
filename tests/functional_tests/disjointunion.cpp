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

using namespace fastmath;

void initialize(FastBitset &f, FastBitset &g) {
    f.reset();
    g.reset();
    for (int i = 0; i < f.size() >> 1; i += 2)
        f.set(i);
    for (int i = 1; i < g.size(); i += 2)
        g.set(i);
}

int main(int argc, char **argv) {
    FastBitset f(128);
    FastBitset g(128);
    initialize(f, g);

    printf("Testing set disjoint union.\n");
    printf("Version 1:\n");
    f.printBitset();
    g.printBitset();
    f.setDisjointUnion_v1(g);
    f.printBitset();

#ifdef AVX2_ENABLED
    printf("\nVersion 2:\n");
    initialize(f, g);
    f.printBitset();
    g.printBitset();
    f.setDisjointUnion_v2(g);
    f.printBitset();
#endif
}
