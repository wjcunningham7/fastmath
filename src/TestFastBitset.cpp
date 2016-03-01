#include "FastMath.h"

int main(int argc, char **argv)
{
	uint64_t nbits = 34;
	FastBitset fb(nbits);

	printf("Bitset Properties:\n");
	printf("-------------------------\n");
	printf(" > Bits Requested:\t%" PRIu64 "\n", nbits);
	printf(" > Bits Stored:\t\t%zd\n", fb.size());
	printf(" > Blocks Used:\t\t%" PRIu64 "\n", fb.getNumBlocks());
	printf(" > Block Size:\t\t%zd\n\n", fb.getBlockSize());
	fflush(stdout);

	printf("Initial Values:\t\t%s\n", fb.toString().c_str());
	uint64_t s1 = 3, s2 = 33, s3 = 5;
	fb.set(s1);
	printf("Bit %" PRIu64 " now set:\t\t%s\n", s1, fb.toString().c_str());
	fb.set(s2);
	printf("Bit %" PRIu64 " now set:\t\t%s\n", s2, fb.toString().c_str());
	fb.set(s3);
	printf("Bit %" PRIu64 " now set:\t\t%s\n", s3, fb.toString().c_str());
	uint64_t c = fb.count();
	printf("Number of bits set:\t%" PRIu64 "\n", c);
	fb.unset(s1);
	printf("Bit %" PRIu64 " now unset:\t%s\n\n", s1, fb.toString().c_str());

	FastBitset fb1(nbits);
	for (uint64_t i = 0; i < nbits / 2; i++)
		fb1.set(i);

	FastBitset fb2(nbits);
	fb2.set(3); fb2.set(5); fb2.set(7); fb2.set(11); fb2.set(13); fb2.set(17);
	fb2.set(19); fb2.set(23); fb2.set(29); fb2.set(31);

	printf("Bitset 1:\t\t%s\n", fb1.toString().c_str());
	printf("Bitset 2:\t\t%s\n", fb2.toString().c_str());
	fb1.setIntersection(fb2);
	printf("Intersection:\t\t%s\n", fb1.toString().c_str());
	fb2.setUnion(fb1);
	printf("Union:\t\t\t%s\n", fb2.toString().c_str());
}
