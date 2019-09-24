#include "fastbitset.h"

/////////////////////////////
//(C) Will Cunningham 2017 //
//         DK Lab          //
// Northeastern University //
/////////////////////////////

using namespace fastmath;

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

	printf("\nTesting flip().\n");
	FastBitset F(200);
	F.set(0);
	F.set(3);
	F.set(5);
	F.set(6);
	for (int i = 0; i < 256; i++)
		printf("%d", (int)F.read(i));
	printf("\n");
	F.flip();
	for (int i = 0; i < 256; i++)
		printf("%d", (int)F.read(i));
	printf("\n");

	printf("\nTesting flip(0).\n");
	F.flip(0);
	F.printBitset();
	printf("Testing flip(63).\n");
	F.flip(63);
	for (int i = 0; i < 64; i++)
		printf("%d", (int)F.read(i));
	printf("\n");
}
