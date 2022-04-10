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

int main(int argc, char **argv)
{
	FastBitset f(128);
	f.set(1);
	printf("Initial bitset:\n");
	f.printBitset();

	FastBitset g = f;
	printf("Testing assignment:\n");
	g.printBitset();

	f.reset();
	g.reset();
	if (f.any() || g.any()) printf("Reset failed.\n");
	else printf("Reset succeeded.\n");

	for (int i = 1; i < f.size(); i += 2)
		f.set(i);
	printf("\nTesting clone:\n");
	f.printBitset();
	f.clone(g);
	g.printBitset();

	if (f == g) printf("Equality operation succeeded.\n");
	else printf("Equality failed.\n");

	printf("\nTesting Partial Clone (1).\n");
	for (int i = 0; i < f.size() >> 1; i++)
		f.unset(i);
	f.printBitset();
	g.reset();
	f.clone(g, 1, 1);
	g.printBitset();

	printf("\nTesting Partial Clone (2).\n");
	for (int i = 0; i < f.size() >> 1; i++)
		f.set(i);
	f.printBitset();
	FastBitset h(f.size());
	f.partial_clone(h, 16, 64);
	h.printBitset();
}
