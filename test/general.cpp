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

	printf("\nTesting next_bit().\n");
	FastBitset g(f.size());
	f.reset();
	for (int i = 0; i < f.size(); i += 2)
		f.set(i);
	f.printBitset();
	for (int i = 0; i < f.getNumBlocks(); i++) {
		uint64_t block;
		while ((block = f.readBlock(i))) {
			uint64_t next = f.next_bit(i);
			g.set(next);
			f.unset(next);
		}
	}
	g.printBitset();

	g.clone(f);
	g.reset();
	while (f.any()) {
		uint64_t next = f.next_bit();
		g.set(next);
		f.unset(next);
	}
	g.printBitset();
}
