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

int main(int argc, char **argv) {
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

    FastBitset workspace(F.size());
    printf("\nTesting shift left(2).\n");
    F.shift_left(workspace, 2);
    F.printBitset();

    printf("\nTesting shift right(2).\n");
    F.shift_right(workspace, 2);
    F.printBitset();

    printf("\nTesting swap_block.\n");

    Bitvector work;
    work.push_back(workspace);
    work.push_back(workspace);
    work.push_back(workspace);

    printf("Set 1:\n");
    f.printBitset();
    printf("Set 2:\n");
    g.printBitset();

    f.swap_block(g, 1);
    printf("Results:\n");
    f.printBitset();
    g.printBitset();

    printf("\nManually modifying bitsets:\n");
    for (int i = 0; i < 64; i += 3)
        f.set(i);
    for (int i = 64; i < 128; i += 4)
        g.set(i);
    printf("Set 1:\n");
    f.printBitset();
    printf("Set 2:\n");
    g.printBitset();

    /*printf("\nTesting swap_range (8, 60, 8)\n");
    printf(" > Should swap [8,15] in 'f' with [60,67] in 'g'\n");
    f.swap_range(g, 8, 60, 8, work[0], work[1], work[2]);
    printf("Results:\n");
    f.printBitset();
    g.printBitset();*/

    printf("\nTesting swap_range(48, 8, 80)\n");
    printf(" > Should swap [48,127] in 'f' with [8,87] in 'g'\n");
    f.swap_range_v2(g, 48, 8, 80, work[0], work[1], work[2]);
    printf("Results:\n");
    f.printBitset();
    g.printBitset();
}
