#ifndef FAST_BITSET_H_
#define FAST_BITSET_H_

#ifdef AVX2_ENABLED
#include <intrinsics/x86intrin.h>
#endif

//DO NOT CHANGE THESE
//If you do, modify bit shifts in 'set', 'unset', 'read', and 'get_block_size'
//and only use count_v1()
#define BlockType unsigned long
#define popcount(x) __builtin_popcountl(x)

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
	//Default Constructor
	FastBitset()
	{
		//printf("Calling the default constructor.\n");
		n = 0; nb = 0; nr = 0;
	}

	//Creation Constructor
	FastBitset(uint64_t _n)
	{
		//printf("Calling the creation constructor.\n");
		createBitset(bits, _n);
	}

	//Copy Constructor
	FastBitset(const FastBitset& other)
	{
		//printf("Calling the copy constructor.\n");
		if (__builtin_expect(this != &other, 1L)) {
			createBitset(bits, other.n, other.nb);
			std::copy(other.bits, other.bits + other.nb, bits);
		}
	}

	//Destructor
	~FastBitset()
	{
		//printf("Calling the destructor.\n");
		destroyBitset(bits);
	}

	//Overload assignment operator
	FastBitset& operator= (const FastBitset& other)
	{
		//printf("Calling the overloaded assignment operator.\n");
		if (__builtin_expect(this != &other, 1L)) {
			BlockType *_bits = NULL;
			createBitset(_bits, other.n, other.nb);
			std::copy(other.bits, other.bits + other.nb, _bits);
			destroyBitset(bits);
			bits = _bits;
		}
		return *this;
	}

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

	//Use this with fb as your workspace
	//to avoid unnecessary malloc and free operations
	inline void cloneBitset(FastBitset &fb)
	{
		fb.n = n;
		fb.nb = nb;
		fb.nr = nr;
		memcpy(fb.bits, bits, sizeof(BlockType) * nb);
	}

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
	//Uses a lookup table
	inline uint64_t count_v2() const
	{
		return do_count(bits, nb);
	}

	//Modified count using assembly code (super fast)
	inline uint64_t count_v3() const
	{
		uint64_t cnt[4] = { 0L, 0L, 0L, 0L };
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

	//Set the bit at location 'idx' to 1
	//NOTE: This operation is not thread-safe
	void set(uint64_t idx)
	{
		bits[idx >> 6] |= (BlockType)1 << (idx & block_size_m);
	}

	//Set the bit at location 'idx' to 0
	//NOTE: This operation is not thread-safe
	void unset(uint64_t idx)
	{
		bits[idx >> 6] &= ~((BlockType)1 << (idx & block_size_m));
	}

	//Read the bit at location 'idx'
	inline BlockType read(uint64_t idx) const
	{
		return (bits[idx >> 6] >> (idx & block_size_m)) & 1;
	}

	//Read the block at location 'idx'
	inline BlockType readBlock(uint64_t idx) const
	{
		return bits[idx];
	}

	//Effectively acts as an &= operator
	//In this case, this is smaller than fb (or the same size)
	inline void setIntersectionS(const FastBitset &fb)
	{
		for (uint64_t i = nb; i-- > 0; )
			bits[i] &= fb.bits[i];
	}

	//Uses AVX2 intrinsics
	inline void setIntersectionS_v2(const FastBitset &fb)
	{
		#ifdef AVX2_ENABLED
		for (uint64_t i = 0; i < nb; i += 4) {
			__m256i data0 = _mm256_loadu_si256((__m256i*)&bits[i]);
			__m256i data1 = _mm256_loadu_si256((__m256i*)&fb.bits[i]);
			data0 = _mm256_and_si256(data0, data1);
			_mm256_storeu_si256((__m256i*)&bits[i], data0);
		}
		#else
		setIntersectionS(fb);
		#endif
	}

	//The assembly implementation of version 2
	//This version is the fastest
	inline void setIntersectionS_v3(const FastBitset &fb)
	{
		#ifdef AVX2_ENABLED
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
		: "r" (fb.bits), "r" (nb)
		: "%rcx", "memory");
		#else
		setIntersectionS(fb);
		#endif
	}

	//In this case, this is larger than fb
	inline void setIntersectionL(const FastBitset fb)
	{
		for (uint64_t i = fb.nb; i-- > 0; )
			bits[i] &= fb.bits[i];
		memset(bits+fb.nb, 0, sizeof(BlockType) * (nb-fb.nb));
	}

	//Uses AVX2 intrinsics
	inline void setIntersectionL_v2(const FastBitset fb)
	{
		#ifdef AVX2_ENABLED
		for (uint64_t i = 0; i < fb.nb; i += 4) {
			__m256i data0 = _mm256_loadu_si256((__m256i*)&bits[i]);
			__m256i data1 = _mm256_loadu_si256((__m256i*)&fb.bits[i]);
			data0 = _mm256_and_si256(data0, data1);
			_mm256_storeu_si256((__m256i*)&bits[i], data0);
		}
		memset(bits+fb.nb, 0, sizeof(BlockType) * (nb-fb.nb));
		#else
		setIntersectionL(fb);
		#endif
	}

	//The assembly implementation
	inline void setIntersectionL_v3(const FastBitset &fb)
	{
		#ifdef AVX2_ENABLED
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
		: "r" (fb.bits), "r" (fb.nb)
		: "%rcx", "memory");
		memset(bits+fb.nb, 0, sizeof(BlockType) * (nb-fb.nb));
		#else
		setIntersectionL(fb);
		#endif
	}

	//The general function
	inline void setIntersection_v1(const FastBitset &fb)
	{
		if (nb <= fb.nb)
			setIntersectionS(fb);
		else
			setIntersectionL(fb);
	}

	//The most efficient version
	inline void setIntersection_v2(const FastBitset &fb)
	{
		if (nb <= fb.nb)
			setIntersectionS_v3(fb);
		else
			setIntersectionL_v3(fb);
	}

	//Effectively acts as an |= operator
	//In this case, this is smaller than fb (or the same size)
	inline void setUnionS(const FastBitset &fb)
	{
		for (uint64_t i = nb; i-- > 0; )
			bits[i] |= fb.bits[i];
	}

	//A more efficient version
	inline void setUnionS_v2(const FastBitset &fb)
	{
		#ifdef AVX2_ENABLED
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
		: "r" (fb.bits), "r" (nb)
		: "%rcx", "memory");
		#else
		setUnionS(fb);
		#endif
	}

	//In this case, this is larger than fb
	inline void setUnionL(const FastBitset &fb)
	{
		for (uint64_t i = fb.nb; i-- > 0; )
			bits[i] |= fb.bits[i];
	}

	//A more efficient version
	inline void setUnionL_v2(const FastBitset &fb)
	{
		#ifdef AVX2_ENABLED
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
		: "r" (fb.bits), "r" (fb.nb)
		: "%rcx", "memory");
		#else
		setUnionS(fb);
		#endif
	}

	//The general function
	inline void setUnion_v1(FastBitset &fb)
	{
		if (nb <= fb.nb)
			setUnionS(fb);
		else
			setUnionL(fb);
	}

	//The most efficient version
	inline void setUnion_v2(const FastBitset &fb)
	{
		if (nb <= fb.nb)
			setUnionS_v2(fb);
		else
			setUnionL_v2(fb);
	}

	std::string toString()
	{
		std::ostringstream s;
		for (uint64_t i = 0; i < n; i++)
			s << read(i);
		return s.str();
	}

	void printBitset()
	{
		printf("%s\n", toString().c_str());
	}

private:
	#ifdef AVX2_ENABLED
	static const size_t block_size = 256;	//Enforces 32-byte alignment
	#else
	static const size_t block_size = sizeof(BlockType) * CHAR_BIT;
	#endif
	static const size_t block_size_m = block_size - 1;	//Used to speed up reads and writes
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
		return ((_n + block_size - 1) >> 8) << 2;
		#else
		return (_n + block_size - 1) >> 6;
		#endif
	}

	//Based on Boost's algorithm for dynamic_bitset.hpp
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
};

#endif
