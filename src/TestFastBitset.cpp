#include "FastMath.h"
#include "stopwatch.h"
#include <vector>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
//#include <immintrin.h>
#include <x86intrin.h>
#include <avx2intrin.h>

typedef boost::mt19937 Engine;
typedef boost::uniform_real<double> UDistribution;
typedef boost::variate_generator< Engine &, UDistribution > UGenerator;

int main(int argc, char **argv)
{
	uint64_t nbits = 64*2;
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
	fb.set(nbits-1);
	printf("Bit %" PRIu64 " now set:\t\t%s\n", nbits-1, fb.toString().c_str());
	uint64_t c = fb.count_v3();
	printf("Number of bits set:\t%" PRIu64 "\n", c);
	fb.unset(s1);
	printf("Bit %" PRIu64 " now unset:\t%s\n\n", s1, fb.toString().c_str());

	/*FastBitset fb1(nbits);
	for (uint64_t i = 0; i < nbits / 2; i++)
		fb1.set(i);

	FastBitset fb2(nbits);
	fb2.set(3); fb2.set(5); fb2.set(7); fb2.set(11); fb2.set(13); fb2.set(17);
	fb2.set(19); fb2.set(23); fb2.set(29); fb2.set(31);

	printf("Bitset 1:\t\t%s\n", fb1.toString().c_str());
	printf("Bitset 2:\t\t%s\n", fb2.toString().c_str());
	fb1.setIntersectionS(fb2);
	printf("Intersection:\t\t%s\n", fb1.toString().c_str());
	fb2.setUnion(fb1);
	printf("Union:\t\t\t%s\n\n", fb2.toString().c_str());*/

	/*printf("Testing Vector Capabilities:\n");	//Enable print statements in constructors
	printf("----------------------------\n");
	std::vector<FastBitset> vec;
	uint64_t size = 1;
	vec.reserve(size);

	//FastBitset f(1);
	uint64_t i;
	for (i = 0; i < size; i++) {
		FastBitset f(i+1);
		vec.push_back(f);
		
		//FastBitset g = f;
	}

	printf("Now I'm here.\n");
	vec[0].set(0);*/

	//Benchmark assignment, intersection, and count operations
	/*printf("Benchmarking Operations:\n");
	printf("------------------------\n");
	std::vector<FastBitset> adj;
	uint64_t vec_size = 2000;
	//uint64_t vec_size = 100;
	uint64_t cnt, tot = 0L;
	uint64_t i, j;

	Stopwatch s = Stopwatch();
	Stopwatch p1 = Stopwatch();
	Stopwatch p2 = Stopwatch();
	Stopwatch p3 = Stopwatch();

	srand(time(NULL));
	Engine eng((long)time(NULL));
	UDistribution udist(0.0, 1.0);
	UGenerator urng(eng, udist);
	float set_prob = 0.4;

	adj.reserve(vec_size);

	for (i = 0; i < vec_size; i++) {
		uint64_t length = i;
		//uint64_t length = 64 * 4 * (i + 1) * 10;
		FastBitset f(length);
		//Randomly set bits
		for (j = 0; j < length; j++)
			if (urng() < set_prob)
				f.set(j);
		adj.push_back(f);
	}

	stopwatchStart(&s);
	//Test the intersection of all pairs
	for (i = 0; i < vec_size * vec_size; i++) {
		int idx1 = static_cast<int>(i / vec_size);
		int idx2 = static_cast<int>(i % vec_size);
		stopwatchStart(&p1);
		FastBitset g = adj[idx2];
		stopwatchStop(&p1);
		if (idx2 > idx1) {
			stopwatchStart(&p2);
			g.setIntersectionL(adj[idx1]);
			stopwatchStop(&p2);
		} else {
			stopwatchStart(&p2);
			g.setIntersectionS(adj[idx1]);
			stopwatchStop(&p2);
		}
		stopwatchStart(&p3);
		cnt = g.count_v3();
		stopwatchStop(&p3);
		if (cnt) tot++;
	}
	stopwatchStop(&s);
	float tot_time = p1.elapsedTime + p2.elapsedTime + p3.elapsedTime;
	printf("tot: %" PRIu64 "\n", tot);
	printf("Execution Time: %5.6f sec\n", s.elapsedTime);
	printf("Operation: [Assignment]\n");
	printf("\tPartial Execution Time: %5.6f sec\n", p1.elapsedTime);
	printf("\tPercent Used:           %5.6f%%\n", p1.elapsedTime / tot_time);
	printf("Operation: [Intersection]\n");
	printf("\tPartial Execution Time: %5.6f sec\n", p2.elapsedTime);
	printf("\tPercent Used:           %5.6f%%\n", p2.elapsedTime / tot_time);
	printf("Operation: [Count]\n");
	printf("\tPartial Execution Time: %5.6f sec\n", p3.elapsedTime);
	printf("\tPercent Used:           %5.6f%%\n", p3.elapsedTime / tot_time);
	fflush(stdout);*/

	//Test count speed
	/*printf("\nBenchmarking count_v3():\n");
	printf("------------------------\n");
	Stopwatch s = Stopwatch();
	stopwatchReset(&s);
	uint64_t cnt_size = 8000000;
	uint64_t samples = 10;
	set_prob = 0.5;
	uint64_t i, j;
	for (i = 0; i < samples; i++) {
		FastBitset fbb(cnt_size);
		for (j = 0; j < cnt_size; j++)
			if (urng() < set_prob)
				fbb.set(i);
		stopwatchStart(&s);
		fbb.count_v3();
		stopwatchStop(&s);
	}
	printf("Time for all operations: %5.6f sec\n", s.elapsedTime);*/

	printf("\nTesting AVX Intrinsics:\n");
	printf("-----------------------\n");
	uint64_t data1[4] = { 1L, 2L, 1L, 1L };
	uint64_t data2[4] = { 2L, 2L, 1L, 1L };
	__m256i v1 = _mm256_load_si256((__m256i*)&data1[0]);
	__m256i v2 = _mm256_load_si256((__m256i*)&data2[0]);
	uint64_t data3[4];
	__m256i v3 = _mm256_add_epi64(v1, v2);
	//__m256i v3 = (__m256i)__builtin_ia32_andsi256((__v4di)v1, (__v4di)v2);
	_mm256_store_si256((__m256i*)&data3[0], v3);	
	for (int i = 0; i < 4; i++)
		printf("%" PRIu64 "\n", data3[i]);
	
	/*uint64_t data1[2] = { 1L, 1L };
	uint64_t data2[2] = { 2L, 1L };
	__m128i v1 = _mm_load_si128((__m128i const*)&data1[0]);
	__m128i v2 = _mm_load_si128((__m128i const*)&data2[0]);
	//__m128i v3 = _mm_add_epi64(v1, v2);
	__m128i v3 = _mm_and_si128(v1, v2);
	uint64_t data3[2];
	_mm_store_si128((__m128i*)&data3[0], v3);
	printf("%" PRIu64 "\t%" PRIu64 "\n", data3[0], data3[1]);*/
}
