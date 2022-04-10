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

#include "fastbitset.h"

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

    printf("Testing set difference.\n");
    printf("Version 1:\n");
    f.printBitset();
    g.printBitset();
    f.setDifference_v1(g);
    f.printBitset();

#ifdef AVX2_ENABLED
    printf("\nVersion 2:\n");
    initialize(f, g);
    f.printBitset();
    g.printBitset();
    f.setDifference_v2(g);
    f.printBitset();
#endif
}
