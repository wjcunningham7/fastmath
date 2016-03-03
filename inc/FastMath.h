#ifndef FAST_MATH_H_
#define FAST_MATH_H_

#include <algorithm>
#include <assert.h>
#include <cstring>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <new>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

/////////////////////////////
//(C) Will Cunningham 2014 //
//         DK Lab          //
// Northeastern University //
/////////////////////////////

//Used for Unit Testing
#define FM_DEBUG false

//Machine Epsilon
#define FM_TOL (1e-28)

//Useful Constants
#define HALF_PI  1.57079632679489661923
#define TWO_PI   6.28318530717958647692
#define SQRT_PI  1.77245385090551602729

//ACOS Chebyshev Series Coefficients
#define ACOS_C0  HALF_PI
#define ACOS_C1 -1.06305396909634217923
#define ACOS_C3  0.88385729242991187525
#define ACOS_C5 -4.69522239734719040073
#define ACOS_C7  7.39114112136511672686
#define ACOS_C9 -4.02406572163211910684

//ACOS Integration Series Coefficients
#define ACOS_I0   HALF_PI
#define ACOS_I1  -1.00000000000000000000
#define ACOS_I3  -0.16666666666666666666
#define ACOS_I5  -0.07500000000000000000
#define ACOS_I7  -0.04464285714285714285
#define ACOS_I9  -0.03038194444444444444
#define ACOS_I11 -0.02237215909090909090
#define ACOS_I13 -0.01735276442307692307
#define ACOS_I15 -0.01396484375000000000

//ATAN Chebyshev Series Coefficients
#define ATAN_C1  1.04538834220118418960
#define ATAN_C3 -0.39082098431982330905
#define ATAN_C5  0.17944049001227966481
#define ATAN_C7 -0.08419846479405229950
#define ATAN_C9  0.02041955547722351862

//ATAN Integration Series Coefficients (x < 0)
#define ATAN_H1   1.00000000000000000000
#define ATAN_H3  -0.33333333333333333333
#define ATAN_H5   0.20000000000000000000
#define ATAN_H7  -0.14285714285714285714
#define ATAN_H9   0.11111111111111111111
#define ATAN_H11 -0.09090909090909090909
#define ATAN_H13  0.07692307692307692307
#define ATAN_H15 -0.06666666666666666666

#define ATAN_I0   HALF_PI
#define ATAN_I1  -1.00000000000000000000
#define ATAN_I3   0.33333333333333333333
#define ATAN_I5  -0.20000000000000000000
#define ATAN_I7   0.14285714285714285714
#define ATAN_I9  -0.11111111111111111111
#define ATAN_I11  0.09090909090909090909
#define ATAN_I13 -0.07692307692307692307
#define ATAN_I15  0.06666666666666666666

#define ASINH_I1   1.00000000000000000000
#define ASINH_I3  -0.16666666666666666666
#define ASINH_I5   0.07500000000000000000
#define ASINH_I7  -0.04464285714285714285
#define ASINH_I9   0.03038194444444444444
#define ASINH_I11 -0.02237215909090909090
#define ASINH_I13  0.01735276442307692307
#define ASINH_I15 -0.01396484375000000000

#define ACOSH_I2  -0.25000000000000000000
#define ACOSH_I4  -0.09375000000000000000
#define ACOSH_I6  -0.10416666666666666666
#define ACOSH_I8  -0.06835937500000000000
#define ACOSH_I10 -0.04921875000000000000
#define ACOSH_I12 -0.03759765625000000000
#define ACOSH_I14 -0.02992466517857142857

//Method of Calculation
enum FastMethod {
	DEF = 0,
	STL = 0,	//STL from <math.h>
	FAST = 1,	//fastX from fastapprox.h
	FASTER = 2,	//fasterX from fastapprox.h
	BITWISE = 3,	//Bitwise operation
	CHEBYSHEV = 4,	//Chebyshev series approximation
	INTEGRATION = 5,//Integration of binomial approximation
	BOOST = 6,	//Boost library function
	EXACT = 7	//Exact solution
};

//Defines Number of Terms in Series for
//Selected Power Series Approximations
enum Precision {
	DEFAULT = 0,
	LOW_PRECISION = 0,
	HIGH_PRECISION = 1,
	VERY_HIGH_PRECISION = 2
};

//Type of transformation used for 2F1
struct HyperType {
	HyperType() : w(0.0), type(0) {}

	double w;
	int type;
};

//Power Functions
double POW2(const double x, const enum FastMethod fm);
double POW3(const double x, const enum FastMethod fm);
double POW(const double x, const double p, const enum FastMethod fm);
double SQRT(const double x, const enum FastMethod fm);

//Absolute Value, Natural Log, and Sign
double ABS(const double x, const enum FastMethod fm);
double LOG(const double x, const enum FastMethod fm);
double SGN(const double x, const enum FastMethod fm);

//Trigonometric Functions
double SIN(const double x, const enum FastMethod fm);
double COS(const double x, const enum FastMethod fm);
double TAN(const double x, const enum FastMethod fm);

//Inverse Trigonometric Functions
double ACOS(const double x, const enum FastMethod fm, const enum Precision p);
double ATAN(const double x, const enum FastMethod fm, const enum Precision p);

//Hyperbolic Functions
double SINH(const double x, const enum FastMethod fm);
double COSH(const double x, const enum FastMethod fm);

//Inverse Hyperbolic Functions
double ASINH(const double x, const enum FastMethod fm, const enum Precision p);
double ACOSH(const double x, const enum FastMethod fm, const enum Precision p);

//Statistical Functions
double GAMMA(const double x, const enum FastMethod fm);
double LOGGAMMA(const double x, const enum FastMethod fm);
double GAMMA_RATIO(const double x, const double y);
double POCHHAMMER(const double x, const int j);
double POCHHAMMER_F(const double x, const int j);

//Gauss Hypergeometric Function
double _2F1_An(const double a, const double b, const double c, const int n);
double _2F1_An_F(const double a, const double b, const double c, const int n);
void _2F1(const double a, const double b, const double c, const double z, double * const sol, double * const err, int * const nterms, const bool check);
void _2F1_F(const double a, const double b, const double c, const double z, double * const sol, double * const err, int * const nterms);
int getNumTerms(const double &z, const double &err);

//Hypergeometric Transformation Type
HyperType getHyperType(const double &z);

//DO NOT CHANGE THESE
//If you do, modify bit shifts in 'set', 'unset', 'read', and 'get_block_size'
//and only use count_v1()
#define BlockType unsigned long
#define popcount(x) __builtin_popcountl(x)

static const unsigned int table_width = 8;
template <bool dummy_name = true>
struct count_table { static const unsigned char table[]; };

template <>
struct count_table<false> { /* no table */ };

template <bool b>
const unsigned char count_table<b>::table[] =
{
	// Automatically generated by GPTableGen.exe v.1.0
	//
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
	inline uint64_t count_v2() const
	{
		return do_count(bits, nb);
	}

	//Modified count using assembly code (super fast)
	inline uint64_t count_v3() const
	{
		uint64_t cnt[4];
		uint64_t diff = nb - nr;
		for (int i = 0; i < 4; i++)
			cnt[i] = 0L;
		for (uint64_t i = 0; i < diff; i += 4) {
			__asm__(
			"popcntq %4, %4	\t\n"
			"add %4, %0	\t\n"
			"popcntq %5, %5	\t\n"
			"add %5, %1	\t\n"
			"popcntq %6, %6	\t\n"
			"add %5, %2	\t\n"
			"popcntq %7, %7	\t\n"
			"add %7, %3	\t\n"
			: "+r" (cnt[0]), "+r" (cnt[1]), "+r" (cnt[2]), "+r" (cnt[3])
			: "r" (bits[i]), "r" (bits[i+1]), "r" (bits[i+2]), "r" (bits[i+3]));
		}
		if (nr)
			*cnt += do_count(bits+diff, nr);
		return cnt[0] + cnt[1] + cnt[2] + cnt[3];
	}

	//Set the bit at location 'idx' to 1
	void set(uint64_t idx)
	{
		bits[idx >> 6] |= (BlockType)1 << (idx & block_size_m);
	}

	//Set the bit at location 'idx' to 0
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

	//In this case, this is larger than fb
	inline void setIntersectionL(const FastBitset &fb)
	{
		for (uint64_t i = fb.nb; i-- > 0; )
			bits[i] &= fb.bits[i];
		memset(bits+fb.nb, 0, sizeof(BlockType) * (nb-fb.nb));
	}

	//The general function - use the others if you can
	inline void setIntersection(const FastBitset &fb)
	{
		if (nb < fb.nb)
			setIntersectionS(fb);
		else
			setIntersectionL(fb);
	}

	//Effectively acts as an |= operator
	//In this case, this is smaller than fb (or the same size)
	inline void setUnionS(const FastBitset &fb)
	{
		for (uint64_t i = nb; i-- > 0; )
			bits[i] |= fb.bits[i];
	}

	//In this case, this is larger than fb
	inline void setUnionL(const FastBitset &fb)
	{
		for (uint64_t i = fb.nb; i-- > 0; )
			bits[i] |= fb.bits[i];
	}

	//Again, use the other two if you are able
	inline void setUnion(const FastBitset &fb)
	{
		if (nb < fb.nb)
			setUnionS(fb);
		else
			setUnionL(fb);
	}

	std::string toString()
	{
		std::ostringstream s;
		for (uint64_t i = 0; i < n; i++)
			s << read(i);
		return s.str();
	}

private:
	static const size_t block_size = sizeof(BlockType) * CHAR_BIT;
	static const size_t block_size_m = block_size - 1;	//Used to speed up reads and writes
	BlockType *bits;	//The bitset array (bits in groups of block_size)
	uint64_t n;		//Number of bits (not including padding)
	uint64_t nb;		//Number of blocks
	uint64_t nr;		//Remainder variable, used for count()

	//Return the number of unsigned integers necessary to store '_n' bits
	inline uint64_t get_num_blocks(uint64_t _n)
	{
		return (_n + block_size - 1) >> 6;
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
};

#endif
