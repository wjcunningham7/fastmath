#ifndef FAST_BITSET_H_
#define FAST_BITSET_H_

#include "FastMath.h"
#include <boost/functional/hash/hash.hpp>

/////////////////////////////
//(C) Will Cunningham 2016 //
//         DK Lab          //
// Northeastern University //
/////////////////////////////

//Note: Bits are addressed in little-endian format in this structure
//As a result, when the function toString() is used, a single binary word
//may appear to be printed backwards (least significant bit first)

//DO NOT CHANGE THESE
#define BlockType unsigned long
#define BlockTypeMPI MPI_UINT64_T
#define popcount(x) __builtin_popcountl(x)
#define BLOCK_SHIFT 6

static const unsigned int table_width = 8;
template <bool dummy_name = true>
struct count_table { static const unsigned char table[]; };

template <>
struct count_table<false> {};

template <bool b>
const unsigned char count_table<b>::table[] =
{
	0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

class FastBitset
{
public:
	//----------------------------//
	// Constructors, Destructors, //
	// and Other Class Utilities  //
	//----------------------------//

	//Default Constructor
	FastBitset()
	{
		n = 0; nb = 0; nr = 0;
		bits = NULL;
	}

	//Creation Constructor
	FastBitset(uint64_t _n)
	{
		createBitset(_n);
	}

	//Copy Constructor
	FastBitset(const FastBitset& other)
	{
		if (__builtin_expect(this != &other, 1L)) {
			createBitset(bits, other.n, other.nb);
			std::copy(other.bits, other.bits + other.nb, bits);
		}
	}

	//Destructor
	~FastBitset()
	{
		destroyBitset(bits);
	}

	//Overloaded Assignment Operator
	FastBitset& operator= (const FastBitset& other)
	{
		if (__builtin_expect(this != &other, 1L)) {
			BlockType *_bits = NULL;
			createBitset(_bits, other.n, other.nb);
			std::copy(other.bits, other.bits + other.nb, _bits);
			destroyBitset(bits);
			bits = _bits;
		}
		return *this;
	}

	//Equality Operator
	inline bool operator==(FastBitset const& other) const
	{
		if (nb != other.nb) return false;

		uint64_t same = 0;
		for (uint64_t i = 0; i < nb; i++)
			same ^= bits[i] ^ other.bits[i];
		return !(bool)same;
	}

	//-------------------//
	// Bitset Properties //
	//-------------------//

	//Returns the number of bits stored in 'bits'
	inline uint64_t size() const
	{
		return n;
	}

	//Returns the number of blocks (i.e. unsigned integers)
	//used to represent 'bits'
	inline uint64_t getNumBlocks() const
	{
		return nb;
	}

	//Returns the number of bits per block
	inline size_t getBlockSize() const
	{
		return block_size;
	}

	//Return address of bits (needed for MPI)
	inline void* getAddress() const
	{
		return (void*)bits;
	}

	//Return true if any bits are set
	inline bool any() const
	{
		uint64_t any_set = 0;
		for (uint64_t i = nb; i-- > 0; )
			any_set |= bits[i];
		return !!any_set;
	}

	//Return true if any bits are set within a particular range
	inline bool any_in_range(uint64_t offset, uint64_t length)
	{
		uint64_t block_idx = offset >> BLOCK_SHIFT;
		unsigned int idx0 = static_cast<unsigned int>(offset & (bits_per_block - 1));
		unsigned int idx1 = static_cast<unsigned int>((offset + length) & (bits_per_block - 1));
		BlockType lower_mask = idx0 ? ~get_bitmask(idx0) : (BlockType)(-1);
		BlockType upper_mask = idx1 ? get_bitmask(idx1) : (BlockType)(-1);

		uint64_t any_set[4] = { 0, 0, 0, 0 };
		uint64_t nmid, rem, max;

		if (length <= bits_per_block && (idx0 < idx1 || idx0 + idx1 == 0 || (idx0 > idx1 && idx1 == 0)))
			any_set[0] |= bits[block_idx] & lower_mask & upper_mask;
		else {
			nmid = (length - 1) >> BLOCK_SHIFT;
			if (idx0 < idx1 || idx0 == 0 || idx1 == 0) nmid--;
			rem = nmid & 3;
			max = nmid - rem;

			for (uint64_t i = 1; i <= max; i += 4) {
				any_set[0] |= bits[block_idx+i];
				any_set[1] |= bits[block_idx+i+1];
				any_set[2] |= bits[block_idx+i+2];
				any_set[3] |= bits[block_idx+i+3];
			}

			for (uint64_t i = 0; i < rem; i++)
				any_set[0] |= bits[block_idx + max + i + 1];

			any_set[0] |= (bits[block_idx] & lower_mask) | (bits[block_idx + nmid + 1] & upper_mask);
		}

		return !!(any_set[0] | any_set[1] | any_set[2] | any_set[3]);
	}

	//---------------------//
	// Reading and Writing //
	//---------------------//

	//Set the bit at location 'idx' to 1
	//NOTE: This operation is not thread-safe
	void set(uint64_t idx)
	{
		bits[idx >> BLOCK_SHIFT] |= (BlockType)1 << (idx & block_size_m);
	}

	//Set the bit at location 'idx' to 0
	//NOTE: This operation is not thread-safe
	void unset(uint64_t idx)
	{
		bits[idx >> BLOCK_SHIFT] &= ~((BlockType)1 << (idx & block_size_m));
	}

	//Read the bit at location 'idx'
	inline BlockType read(uint64_t idx) const
	{
		return (bits[idx >> BLOCK_SHIFT] >> (idx & block_size_m)) & (BlockType)1;
	}

	//Read the block at location 'idx'
	inline BlockType readBlock(uint64_t idx) const
	{
		return bits[idx];
	}

	//---------------//
	// Create Bitset //
	//---------------//

	inline void createBitset(uint64_t _n)
	{
		createBitset(bits, _n);
	}

	//--------------------//
	// Reset Bits to Zero //
	//--------------------//

	inline void reset()
	{
		memset(bits, 0, sizeof(BlockType) * nb);
	}

	//--------------------//
	// Cloning Operations //
	//--------------------//

	//Use this with fb as your workspace
	//to avoid unnecessary malloc and free operations
	//It is important fb be at least as large as this object
	inline void clone(FastBitset &fb)
	{
		fb.n = n;
		fb.nb = nb;
		fb.nr = nr;
		memcpy(fb.bits, bits, sizeof(BlockType) * nb);
	}

	//Clone only a subset of the bitset
	//NOTE: The offset and the length are for blocks, not bits!
	//It is important fb be at least as large as this object
	inline void clone(FastBitset &fb, const uint64_t offset, const uint64_t length)
	{
		fb.n = length * bits_per_block;
		fb.nb = length;
		fb.nr = fb.nb & 3;
		memcpy(fb.bits, bits + offset, sizeof(BlockType) * length);
	}

	//This copies a subset of bits, but does not
	//modify the parameters of 'fb' like the above method does
	inline void partial_clone(FastBitset &fb, const uint64_t offset, const uint64_t length)
	{
		uint64_t block_idx = offset >> BLOCK_SHIFT;
		unsigned int idx0 = static_cast<unsigned int>(offset & (bits_per_block - 1));
		unsigned int idx1 = static_cast<unsigned int>((offset + length) & (bits_per_block - 1));
		BlockType lower_mask = idx0 ? ~get_bitmask(idx0) : (BlockType)(-1);
		BlockType upper_mask = idx1 ? get_bitmask(idx1) : (BlockType)(-1);
		
		if (length <= bits_per_block && (idx0 < idx1 || idx0 + idx1 == 0 || (idx0 > idx1 && idx1 == 0)))
			fb.bits[block_idx] = (fb.bits[block_idx] & ~lower_mask) | (bits[block_idx] & lower_mask & upper_mask) | (fb.bits[block_idx] & ~upper_mask);
		else {
			uint64_t nmid = (length - 1) >> BLOCK_SHIFT;
			if (idx0 < idx1 || idx0 == 0 || idx1 == 0) nmid--;

			memcpy(fb.bits + block_idx + 1, bits + block_idx + 1, sizeof(BlockType) * nmid);
			fb.bits[block_idx] = (fb.bits[block_idx] & ~lower_mask) | (bits[block_idx] & lower_mask);
			fb.bits[block_idx+nmid+1] = (fb.bits[block_idx+nmid+1] & ~upper_mask) | (bits[block_idx+nmid+1] & upper_mask);
		}
	}

	//---------------------//
	// Counting Operations //
	//---------------------//

	//Computes the Hamming weight
	//Make sure to compile with the flag -mpopcnt to use this
	inline uint64_t count_v1() const
	{
		uint64_t num_set = 0;
		for (uint64_t i = nb; i-- > 0; )
			num_set += popcount(bits[i]);
		return num_set;
	}

	//Modified count based on Boost's algorithm
	inline uint64_t count_v2() const
	{
		return do_count(bits, nb);
	}

	//Modified count using assembly code (super fast)
	inline uint64_t count_v3() const
	{
		uint64_t cnt[4] = { 0, 0, 0, 0 };
		#ifdef AVX2_ENABLED
		uint64_t max = nb;
		#else
		uint64_t max = nb - nr;
		#endif

		for (uint64_t i = 0; i < max; i += 4) {
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
			: "r" (bits[i]), "r" (bits[i+1]), "r" (bits[i+2]), "r" (bits[i+3]));
		}

		#ifndef AVX2_ENABLED
		if (nr)
			*cnt += do_count(bits+max, nr);
		#endif

		return cnt[0] + cnt[1] + cnt[2] + cnt[3];
	}

	//Count a subset of bits
	//NOTE: The offset and the length are for bits, not blocks!
	inline uint64_t partial_count(uint64_t offset, uint64_t length)
	{
		uint64_t block_idx = offset >> BLOCK_SHIFT;
		//idx0 and idx1 lie between 0 and 63
		unsigned int idx0 = static_cast<unsigned int>(offset & (bits_per_block - 1));
		unsigned int idx1 = static_cast<unsigned int>((offset + length) & (bits_per_block - 1));
		BlockType lower_mask = idx0 ? ~get_bitmask(idx0) : (BlockType)(-1);
		BlockType upper_mask = idx1 ? get_bitmask(idx1) : (BlockType)(-1);

		/*printf("\noffset: %" PRIu64 "\n", offset);
		printf("length: %" PRIu64 "\n", length);
		printf("block_idx: %" PRIu64 "\n", block_idx);
		printf("idx0: %u\n", idx0);
		printf("idx1: %u\n", idx1);
		printf("lower_mask:\t\t%s\n", toString(lower_mask).c_str());
		printf("upper_mask:\t\t%s\n", toString(upper_mask).c_str());*/

		uint64_t cnt[4] = { 0, 0, 0, 0 };
		uint64_t nmid, rem, max;

		if (length <= bits_per_block && (idx0 < idx1 || idx0 + idx1 == 0 || (idx0 > idx1 && idx1 == 0)))
			cnt[0] += popcount(bits[block_idx] & lower_mask & upper_mask);
		else {
			nmid = (length - 1) >> BLOCK_SHIFT;
			if (idx0 < idx1 || idx0 == 0 || idx1 == 0) nmid--;
			rem = nmid & 3;
			max = nmid - rem;

			/*printf("\nnmid: %" PRIu64 "\n", nmid);
			printf("rem: %" PRIu64 "\n", rem);
			printf("max: %" PRIu64 "\n", max);*/
			
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
				: "r" (bits[block_idx+i]), "r" (bits[block_idx+i+1]), "r" (bits[block_idx+i+2]), "r" (bits[block_idx+i+3]));
			}

			if (rem)
				cnt[0] += do_count((bits + block_idx + max + 1), rem);

			//printf("\nCount of interior: %" PRIu64 "\n", cnt[0] + cnt[1] + cnt[2] + cnt[3]);

			cnt[0] += popcount(bits[block_idx] & lower_mask) + popcount(bits[block_idx + nmid + 1] & upper_mask);
		}

		return cnt[0] + cnt[1] + cnt[2] + cnt[3];
	}

	//------------------//
	// Set Intersection //
	//------------------//

	//Aliases to be used externally
	#ifdef AVX2_ENABLED
	#define setIntersection(fb) setIntersection_v2(fb)
	#else
	#define setIntersection(fb) setIntersection_v1(fb)
	#endif

	inline void setIntersection_v1(const FastBitset &fb)
	{
		for (uint64_t i = std::min(nb, fb.nb); i-- > 0; )
			bits[i] &= fb.bits[i];
	}

	//Use the general "G" function if you aren't sure
	//which bitset is larger - otherwise avoid the branch
	inline void setIntersectionG_v1(const FastBitset &fb)
	{
		setIntersection_v1(fb);
		if (nb > fb.nb)
			memset(bits+fb.nb, 0, sizeof(BlockType) * (nb-fb.nb));
	}

	#ifdef AVX2_ENABLED
	inline void setIntersection_v2(const FastBitset &fb)
	{
		asm volatile(
		"movq %2, %%rcx			\n"
		"forloop%=:			\n\t"
		"subq $4, %%rcx			\n\t"
		"vmovdqu (%0,%%rcx,8), %%ymm0	\n\t"
		"vmovdqu (%1,%%rcx,8), %%ymm1	\n\t"
		"vpand %%ymm0, %%ymm1, %%ymm0	\n\t"
		"vmovdqu %%ymm0, (%0,%%rcx,8)	\n\t"
		"cmpq $0, %%rcx			\n\t"
		"jne forloop%=			\n\t"
		: "+r" (bits)
		: "r" (fb.bits), "r" (std::min(nb, fb.nb))
		: "%rcx", "memory");
	}

	inline void setIntersectionG_v2(const FastBitset &fb)
	{
		setIntersection_v2(fb);
		if (nb > fb.nb)
			memset(bits+fb.nb, 0, sizeof(BlockType) * (nb-fb.nb));
	}
	#endif

	//Perform a subset intersection
	//This is equivalent to a full intersection with some bitmasks
	//NOTE: The offset and length refer to bit indices, not blocks
	inline void partial_intersection(const FastBitset &fb, uint64_t offset, uint64_t length)
	{
		uint64_t block_idx = offset >> BLOCK_SHIFT;
		unsigned int idx0 = static_cast<unsigned int>(offset & (bits_per_block - 1));
		unsigned int idx1 = static_cast<unsigned int>((offset + length) & (bits_per_block - 1));
		BlockType lower_mask = idx0 ? ~get_bitmask(idx0) : (BlockType)(-1);
		BlockType upper_mask = idx1 ? get_bitmask(idx1) : (BlockType)(-1);
		uint64_t nmid, nused;

		if (length <= bits_per_block && (idx0 < idx1 || idx0 + idx1 == 0 || (idx0 > idx1 && idx1 == 0))) {
			bits[block_idx] &= fb.bits[block_idx] & lower_mask & upper_mask;
			nused = 1;
		} else {
			nmid = (length - 1) >> BLOCK_SHIFT;
			if (idx0 < idx1 || idx0 == 0 || idx1 == 0) nmid--;

			#ifdef AVX2_ENABLED
			uint64_t rem = nmid & 3;
			uint64_t max = nmid - rem;

			if (max) {
				asm volatile(
				"movq %3, %%rax			\n\t"
				"movq %2, %%rcx			\n\t"
				"forloop%=:			\n\t"
				"vmovdqu (%0,%%rcx,8), %%ymm0	\n\t"
				"vmovdqu (%1,%%rcx,8), %%ymm1	\n\t"
				"vpand %%ymm0, %%ymm1, %%ymm0	\n\t"
				"vmovdqu %%ymm0, (%0,%%rcx,8)	\n\t"
				"addq $4, %%rcx			\n\t"
				"cmpq %%rax, %%rcx		\n\t"
				"jl forloop%=			\n\t"
				: "+r" (bits)
				: "r" (fb.bits), "r" (block_idx + 1), "r" (max + block_idx)
				: "%rax", "%rcx", "memory");
			}

			if (rem)
				for (uint64_t i = max + 1; i <= nmid; i++)
					bits[block_idx+i] &= fb.bits[block_idx+i];
			#else
			for (uint64_t i = 1; i <= nmid; i++)
				bits[block_idx+i] &= fb.bits[block_idx+i];
			#endif

			bits[block_idx] &= fb.bits[block_idx] & lower_mask;
			bits[block_idx+nmid+1] &= fb.bits[block_idx+nmid+1] & upper_mask;
			nused = nmid + 2;
		}

		uint64_t low = block_idx;
		uint64_t high = nb - low - nused;

		if (low)
			memset(bits, 0, sizeof(BlockType) * low);
		if (high)
			memset(bits + low + nused, 0, sizeof(BlockType) * high);
	}

	//-----------//
	// Set Union //
	//-----------//

	//Aliases to be used externally
	#ifdef AVX2_ENABLED
	#define setUnion(fb) setUnion_v2(fb)
	#else
	#define setUnion(fb) setUnion_v1(fb)
	#endif

	inline void setUnion_v1(const FastBitset &fb)
	{
		for (uint64_t i = std::min(nb, fb.nb); i-- > 0; )
			bits[i] |= fb.bits[i];
	}

	#ifdef AVX2_ENABLED
	inline void setUnion_v2(const FastBitset &fb)
	{
		asm volatile(
		"movq %2, %%rcx			\n"
		"forloop%=:			\n\t"
		"subq $4, %%rcx			\n\t"
		"vmovdqu (%0,%%rcx,8), %%ymm0	\n\t"
		"vmovdqu (%1,%%rcx,8), %%ymm1	\n\t"
		"vpor %%ymm0, %%ymm1, %%ymm0	\n\t"
		"vmovdqu %%ymm0, (%0,%%rcx,8)	\n\t"
		"cmpq $0, %%rcx			\n\t"
		"jne forloop%=			\n\t"
		: "+r" (bits)
		: "r" (fb.bits), "r" (std::min(nb, fb.nb))
		: "%rcx", "memory");
	}
	#endif

	//--------------------//
	// Set Disjoint Union //
	//--------------------//

	//Aliases to be used externally
	#ifdef AVX2_ENABLED
	#define setDisjointUnion(fb) setDisjointUnion_v2(fb)
	#else
	#define setDisjointUnion(fb) setDisjointUnion_v1(fb)
	#endif

	inline void setDisjointUnion_v1(const FastBitset &fb)
	{
		for (uint64_t i = std::min(nb, fb.nb); i-- > 0; )
			bits[i] ^= fb.bits[i];
	}

	#ifdef AVX2_ENABLED
	inline void setDisjointUnion_v2(const FastBitset &fb)
	{
		asm volatile(
		"movq %2, %%rcx			\n"
		"forloop%=:			\n\t"
		"subq $4, %%rcx			\n\t"
		"vmovdqu (%0,%%rcx,8), %%ymm0	\n\t"
		"vmovdqu (%1,%%rcx,8), %%ymm1	\n\t"
		"vpxor %%ymm0, %%ymm1, %%ymm0	\n\t"
		"vmovdqu %%ymm0, (%0,%%rcx,8)	\n\t"
		"cmpq $0, %%rcx			\n\t"
		"jne forloop%=			\n\t"
		: "+r" (bits)
		: "r" (fb.bits), "r" (std::min(nb, fb.nb))
		: "%rcx", "memory");
	}
	#endif

	//----------------//
	// Set Difference //
	//----------------//

	//Aliases to be used externally
	#ifdef AVX2_ENABLED
	#define setDifference(fb) setDifference_v2(fb)
	#else
	#define setDifference(fb) setDifference_v1(fb)
	#endif

	inline void setDifference_v1(const FastBitset &fb)
	{
		for (uint64_t i = std::min(nb, fb.nb); i-- > 0; )
			bits[i] &= (bits[i] ^ fb.bits[i]);
	}

	#ifdef AVX2_ENABLED
	inline void setDifference_v2(const FastBitset &fb)
	{
		asm volatile(
		"movq %2, %%rcx			\n"
		"forloop%=:			\n\t"
		"subq $4, %%rcx			\n\t"
		"vmovdqu (%0,%%rcx,8), %%ymm0	\n\t"
		"vmovdqu (%1,%%rcx,8), %%ymm1	\n\t"
		"vpxor %%ymm0, %%ymm1, %%ymm1	\n\t"
		"vpand %%ymm0, %%ymm1, %%ymm0	\n\t"		
		"vmovdqu %%ymm0, (%0,%%rcx,8)	\n\t"
		"cmpq $0, %%rcx			\n\t"
		"jne forloop%=			\n\t"
		: "+r" (bits)
		: "r" (fb.bits), "r" (std::min(nb, fb.nb))
		: "%rcx", "memory");
	}
	#endif

	//----------//
	// Printing //
	//----------//

	std::string toString() const
	{
		std::ostringstream s;
		for (uint64_t i = 0; i < n; i++)
			s << read(i);
		return s.str();
	}

	std::string toString(uint64_t v)
	{
		FastBitset cpy(64);
		memcpy(cpy.bits, &v, sizeof(uint64_t));
		return cpy.toString();
	}

	void printBitset() const
	{
		printf("%s\n", toString().c_str());
	}

	static const size_t bits_per_block = sizeof(BlockType) * CHAR_BIT;

private:
	inline void createBitset(BlockType *& _bits, uint64_t _n, uint64_t _nb)
	{
		try {
			n = _n;
			nb = _nb;
			nr = nb & 3;	//Equivalent to nb % 4
			_bits = (BlockType*)malloc(sizeof(BlockType) * nb);
			if (_bits == NULL)
				throw std::bad_alloc();
			memset(_bits, 0, sizeof(BlockType) * nb);
		} catch (std::bad_alloc) {
			fprintf(stderr, "Memory allocation failure in %s on line %d!\n", __FILE__, __LINE__);
			fflush(stderr);
			destroyBitset(_bits);
		}
	}

	inline void createBitset(BlockType *& _bits, uint64_t _n)
	{
		createBitset(_bits, _n, get_num_blocks(_n));
	}

	inline void destroyBitset(BlockType *& _bits)
	{
		n = 0; nb = 0; nr = 0;
		if (_bits != NULL) {
			free(_bits);
			_bits = NULL;
		}
	}

	#ifdef AVX2_ENABLED
	static const size_t block_size = 256;	//Enforces 32-byte alignment
	#else
	static const size_t block_size = bits_per_block;
	#endif
	static const size_t block_size_m = bits_per_block - 1;	//Used to speed up reads and writes
	BlockType *bits;	//The bitset array (bits in groups of block_size)
	uint64_t n;		//Number of bits (not including padding)
	uint64_t nb;		//Number of blocks
	uint64_t nr;		//Remainder variable

	//Return the number of unsigned integers necessary to store '_n' bits
	//Note if AVX2 is being used, extra blocks may be allocated to enforce 32-byte alignment
	//However, the size of BlockType is not equal to block_size in this case.
	inline uint64_t get_num_blocks(uint64_t _n)
	{
		#ifdef AVX2_ENABLED
		return ((_n + block_size - 1) >> (BLOCK_SHIFT + 2)) << 2;
		#else
		return (_n + block_size - 1) >> BLOCK_SHIFT;
		#endif
	}

	inline uint64_t do_count(BlockType *b, uint64_t num_blocks) const
	{
		uint64_t num = 0;
		while (num_blocks) {
			BlockType value = *b;
			while (value) {
				num += count_table<>::table[value & ((1u<<table_width) - 1)];
				value >>= table_width;
			}
			++b;
			--num_blocks;
		}
		return num;
	}

	//Returns a value with the last #'offset' bits set to 1
	//Note the value 'offset' must be less than the number of bits in BlockType
	BlockType get_bitmask(unsigned int offset) {
		return ((BlockType)1 << offset) - 1;
	}
};

//Add hashing function
namespace std
{
	template<>
	class hash<FastBitset>
	{
	public:
		size_t operator()(FastBitset const& fb) const
		{
			size_t seed = 0;
			for (uint64_t i = 0; i < fb.getNumBlocks(); i++)
				boost::hash_combine(seed, (size_t)fb.readBlock(i));
			return seed;
		}
	};
};

#endif
