#include "FastBitset.h"

int main(int argc, char **argv)
{
	FastBitset f(128);
	f.set(1);
	f.printBitset();
	printf("Size: %" PRIu64 "\n", f.size());
	printf("Number of Blocks: %" PRIu64 "\n", f.getNumBlocks());
	printf("Block Size: %" PRIu64 "\n", f.getBlockSize());

	printf("Any set? %s\n", f.any() ? "Yes" : "No");
	printf("Any in first block? %s\n", f.any_in_range(0, 64) ? "Yes" : "No");
	printf("Any in second block? %s\n", f.any_in_range(65, 64) ? "Yes" : "No");
}
