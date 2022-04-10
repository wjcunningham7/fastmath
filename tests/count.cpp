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

void countAll(FastBitset &f)
{
	printf("Version 1: %" PRIu64 "\n", f.count_v1());
	printf("Version 2: %" PRIu64 "\n", f.count_v2());
	printf("Version 3: %" PRIu64 "\n", f.count_v3());
}

int main(int argc, char **argv)
{
	FastBitset f(128);
	for (int i = 0; i < f.size(); i += 2)
		f.set(i);

	printf("Testing bit counting.\n");
	f.printBitset();
	countAll(f);

	for (int i = f.size() * 3 / 4; i < f.size(); i++)
		f.unset(i);
	f.printBitset();
	countAll(f);

	for (int i = 0; i < f.size() >> 2; i++)
		f.set(i);
	f.printBitset();
	countAll(f);

	printf("\nTesting partial count.\n");
	f.reset();
	for (int i = 0; i < f.size() >> 1; i++)
		f.set(i);
	for (int i = f.size() >> 1; i < f.size(); i += 4)
		f.set(i);
	f.printBitset();
	printf("First half: %" PRIu64 "\n", f.partial_count(0, 64));
	printf("Middle half: %" PRIu64 "\n", f.partial_count(32, 64));
	printf("Final half: %" PRIu64 "\n", f.partial_count(64, 64));

	FastBitset g(550);
	g.set();

	printf("\nTesting set().\n");
	printf("Blocks used: %" PRIu64 "\n", g.getNumBlocks());
	printf("Bits set: %" PRIu64 "\n", g.count_bits());
}
