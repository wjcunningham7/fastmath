inline uint64_t partial_vecprod(const FastBitset &fb0, const FastBitset &fb1, uint64_t offset, uint64_t length)
{
	uint64_t block_idx = offset >> BLOCK_SHIFT;
	unsigned int idx0 = static_cast<unsigned int>(offset & block_size_m);
	unsigned int idx1 = static_cast<unsigned int>((offset + length) & block_size_m);
	BlockType lower_mask = masks2[idx0];
	BlockType upper_mask = masks[idx1];
	uint64_t cnt[4] = { 0, 0, 0, 0 };
	uint64_t nmid, rem, max;

	if (length <= bits_per_block && (idx0 < idx1 || idx0 + idx1 == 0 || (idx0 > idx1 && idx1 == 0))) {
		return popcount(fb0.bits[block_idx] & fb1.bits[block_idx] & lower_mask & upper_mask);
	} else {
		nmid = (length - 1) >> BLOCK_SHIFT;
		if (idx0 < idx1 || idx0 == 0 || idx1 == 0) nmid--;
		rem = nmid & 3;
		max = nmid - rem;

		#ifdef AVX2_ENABLED
		if (max) {
			asm volatile(
			"movq %4, %%rax			\n\t"
			"movq %3, %%rcx			\n\t"
			"forloop%=:			\n\t"
			"vmovdqu (%1,%%rcx,8), %%ymm0	\n\t"
			"vmovdqu (%2,%%rcx,8), %%ymm1	\n\t"
			"vpand %%ymm0, %%ymm1, %%ymm0	\n\t"

			"popcntq %%ymm0, %%ymm2		\n\t"
			"vpsrlq %%ymm0, $64		\n\t"
			"popcntq %%ymm0, %%ymm3		\n\t"
			"vpaddq %%ymm2, %%ymm3, %%ymm3	\n\t"
			"vpsrlq %%ymm0, $64		\n\t"

			"addq $4, %%rcx			\n\t"
			"cmpq %%rax, %%rcx		\n\t"
			"jl forloop%=			\n\t"
			: "+r" (cnt[0])
			: "r" (fb0.bits), "r" (fb1.bits), "r" (block_idx + 1), "r" (max + block_idx)
			: "%rax", "%rcx", "memory");
		}
		#else
		for (uint64_t i = 1; i <= max; i += 4) {
			asm volatile(
			"popcntq %4, %4	\n\t"
			"addq %4, %0	\n\t"
			"popcntq %5, %5	\n\t"
			"addq %5, %1	\n\t"
			"popcntq %6, %6	\n\t"
			"addq %6, %2	\n\t"
			"popcntq %7, %7	\n\t"
			"addq %7, %3	\n\t"
			: "+r" (cnt[0]), "+r" (cnt[1]), "+r" (cnt[2]), "+r" (cnt[3])
			: "r" (fb0.bits[block_idx+i] & fb1.bits[block_idx+i]), "r" (fb0.bits[block_idx+i+1] & fb1.bits[block_idx+i+1]),
			  "r" (fb0.bits[block_idx+i+2] & fb1.bits[block_idx+i+2]), "r" (fb0.bits[block_idx+i+3] & fb1.bits[block_idx+i+3]));
		}
		#endif

		if (rem)
			for (uint64_t i = max + 1; i <= nmid; i++)
				cnt[0] += popcount(fb0.bits[block_idx+i] & fb1.bits[block_idx+i]);

		cnt[1] += popcount(fb0.bits[block_idx] & fb1.bits[block_idx] & lower_mask) + popcount(fb0.bits[block_idx + nmid + 1] & fb1.bits[block_idx + nmid + 1] & upper_mask);
	}

	return cnt[0] + cnt[1] + cnt[2] + cnt[3];
}
