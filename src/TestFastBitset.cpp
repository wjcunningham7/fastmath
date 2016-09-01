#include "FastBitset.h"
#include "stopwatch.h"
#include <unordered_set>
#include <vector>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

typedef boost::mt19937 Engine;
typedef boost::uniform_real<double> UDistribution;
typedef boost::variate_generator< Engine &, UDistribution > UGenerator;

/*namespace std {
	template <>
	class hash<FastBitset>
	{
	public:
		size_t operator()(FastBitset const& fb) const
		{
			size_t seed = 0;
			for (uint64_t i = 0; i < fb.nb; i++)
				boost::hash_combine(seed, (size_t)fb.bits[i]);
			return seed;
		}
	};
};*/

int main(int argc, char **argv)
{
	uint64_t nbits = 47;
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

	FastBitset fb1(nbits);
	//for (uint64_t i = 0; i < nbits / 2; i++)
	//	fb1.set(i);
	for (uint64_t i = nbits / 2; i < nbits; i += 2)
		fb1.set(i);

	FastBitset fb2(nbits-10);
	fb2.set(3); fb2.set(5); fb2.set(7); fb2.set(11); fb2.set(13); fb2.set(17);
	fb2.set(19); fb2.set(23); fb2.set(29); fb2.set(31);

	printf("Bitset 1:\t\t%s\n", fb1.toString().c_str());
	printf("Bitset 2:\t\t%s\n", fb2.toString().c_str());
	fb1.setIntersection(fb2);
	printf("Intersection:\t\t%s\n", fb1.toString().c_str());
	//fb2.setUnion_v1(fb1);
	//printf("Union:\t\t\t%s\n", fb2.toString().c_str());

	bool equal = fb1 == fb1;
	printf("Equal: %d\n", (int)equal);

	std::unordered_set<FastBitset> fbset;
	fbset.insert(fb1);

	void *address = fb1.getAddress();

	//===========================================================//

	/*printf("\nTesting Partial Bitset Capabilities:\n");
	printf("------------------------------------\n");
	//FastBitset fb3(384);
	FastBitset fb3(640);
	for (uint64_t i = 0; i < 64; i += 2)
		fb3.set(i);
	for (uint64_t i = 64; i < 128; i += 4)
		fb3.set(i);
	for (uint64_t i = 128; i < 192; i += 8)
		fb3.set(i);
	for (uint64_t i = 192; i < 256; i += 16)
		fb3.set(i);
	for (uint64_t i = 256; i < 320; i += 2)
		fb3.set(i);
	for (uint64_t i = 320; i < 384; i += 4)
		fb3.set(i);
	printf("Bitset 3:\t\t%s\n", fb3.toString().c_str());
	
	FastBitset fb_clone(640);
	fb3.clone(fb_clone, 1, 8);
	for (uint64_t i = 256; i < 512; i += 8)
		fb_clone.set(i);
	printf("Clone of Bitset:\t%s\n", fb_clone.toString().c_str());

	printf("Count of the Clone: %" PRIu64 "\n", fb_clone.count_v3());

	unsigned int start = 0;
	unsigned int finish = 32;
	printf("Partial Count (%d to %d): %" PRIu64 "\n", start, finish-1, fb_clone.partial_count(start, finish - start));*/

	//===========================================================//

	/*printf("\nTesting Partial Intersection:\n");
	printf("-----------------------------\n");
	printf("Set 1:\n");
	for (int i = 0; i < 8; i++)
		printf("%s\n", fb_clone.toString(fb_clone.readBlock(i)).c_str());
	printf("\nSet 2:\n");
	for (int i = 0; i < 8; i++)
		printf("%s\n", fb3.toString(fb3.readBlock(i)).c_str());
	printf("\nIntersection:\n");
	fb_clone.partial_intersection(fb3, 48, 256);
	for (int i = 0; i < 8; i++)
		printf("%s\n", fb_clone.toString(fb_clone.readBlock(i)).c_str());
	//printf("Intersection:\t\t%s\n", fb_clone.toString().c_str());*/

	//===========================================================//

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

	//===========================================================//

	//Benchmark assignment, intersection, and count operations
	/*printf("\nBenchmarking Operations:\n");
	printf("------------------------\n");
	std::vector<FastBitset> adj;
	uint64_t vec_size = 2000;
	//uint64_t vec_size = 100;
	uint64_t cnt, tot = 0L;
	uint64_t i, j;

	Stopwatch s = Stopwatch();
	Stopwatch p1 = Stopwatch();
	Stopwatch p2 = Stopwatch();
	Stopwatch p3 = Stopwatch();*/

	/*srand(time(NULL));
	long seed = 49375439;
	Engine eng(seed);
	UDistribution udist(0.0, 1.0);
	UGenerator urng(eng, udist);
	float set_prob = 0.4;*/

	/*adj.reserve(vec_size);

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

	//===========================================================//

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

	//===========================================================//

	/*printf("\nTesting AVX2 Intrinsics:\n");
	printf("-----------------------\n");
	uint64_t data1[4] = { 1L, 2L, 1L, 1L };
	uint64_t data2[4] = { 2L, 2L, 1L, 3L };
	__m256i v1 = _mm256_load_si256((__m256i*)data1);
	__m256i v2 = _mm256_load_si256((__m256i*)data2);
	uint64_t data3[4];
	v1 = _mm256_and_si256(v1, v2);
	_mm256_store_si256((__m256i*)data3, v1);	
	for (int i = 0; i < 4; i++)
		printf("%" PRIu64 "\n", data3[i]);*/

	/*printf("\nBenchmarking AVX2 Intersection:\n");
	printf("-------------------------------\n");
	Stopwatch sw0 = Stopwatch();
	Stopwatch sw1 = Stopwatch();
	Stopwatch sw2 = Stopwatch();
	Stopwatch sw3 = Stopwatch();
	Stopwatch sw4 = Stopwatch();
	uint64_t cnt_size = 160000000;
	uint64_t samples = 25;
	set_prob = 0.3;
	uint64_t i, j;
	for (i = 0; i < samples; i++) {
		FastBitset fbb0(cnt_size);
		FastBitset fbb1(cnt_size);
		for (j = 0; j < cnt_size; j++) {
			if (urng() < set_prob)
				fbb0.set(i);
			if (urng() < set_prob)
				fbb1.set(i);
		}
		if (!(i % 5)) {
			stopwatchStart(&sw0);
			fbb0.setIntersectionS(fbb1);
			stopwatchStop(&sw0);
		} else if (i % 5 == 1) {
			stopwatchStart(&sw1);
			fbb0.setIntersectionS_v2(fbb1);
			stopwatchStop(&sw1);
		} else if (i % 5 == 2) {
			stopwatchStart(&sw2);
			fbb0.setIntersectionS_v3(fbb1);
			stopwatchStop(&sw2);
		} else if (i % 5 == 3) {
			stopwatchStart(&sw3);
			fbb0.setIntersectionS_v4(fbb1);
			stopwatchStop(&sw3);
		} else {
			stopwatchStart(&sw4);
			fbb0.setIntersectionS_v5(fbb1);
			stopwatchStop(&sw4);
		}
	}
	printf("Time using version 1: %5.6f sec\n", sw0.elapsedTime / samples * 5);
	printf("Time using version 2: %5.6f sec\n", sw1.elapsedTime / samples * 5);
	printf("Time using version 3: %5.6f sec\n", sw2.elapsedTime / samples * 5);
	printf("Time using version 4: %5.6f sec\n", sw3.elapsedTime / samples * 5);
	printf("Time using version 5: %5.6f sec\n", sw4.elapsedTime / samples * 5);*/

	//===========================================================//

	/*printf("\nBenchmarking Partial Count:\n");
	printf("---------------------------\n");

	Stopwatch s = Stopwatch();
	uint64_t size = 16384;
	uint64_t samples = 1;
	set_prob = 0.4;

	FastBitset work(size);
	for (uint64_t i = 0; i < samples; i++) {
		FastBitset fbpc0(size);
		FastBitset fbpc1(size);
		for (uint64_t j = 0; j < size; j++) {
			if (urng() < set_prob)
				fbpc0.set(j);
			if (urng() < set_prob)
				fbpc1.set(j);
		}

		stopwatchStart(&s);
		for (uint64_t j = 0; j < size; j++) {
			//printf("j: %" PRIu64 "\n", j);
			for (uint64_t k = 1; k <= size - j; k++) {
				//printf("k: %" PRIu64 "\n", k);
				fbpc0.clone(work);
				work.partial_intersection(fbpc1, j, k);
				work.partial_count(j, k);
			}
		}
		stopwatchStop(&s);
	}

	printf("Time for all operations: %5.6f sec\n", s.elapsedTime);*/
}
