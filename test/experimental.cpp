#include "FastBitset.h"

int main(int argc, char **argv)
{
	FastBitset f;
	srand(time(NULL));

	uint64_t bits[4] = { ~0ULL, ~0ULL, ~0ULL, ~0ULL };
	for (unsigned i = 0; i < 4; i++)
		bits[i] = ((uint64_t)rand() << 32) | rand();
	uint64_t bits1[4] = { 0, 0, 0, 0 };
	for (unsigned i = 0; i < 4; i++)
		bits1[i] = ((uint64_t)rand() << 32) | rand();

	printf("Initial bits:\n");
	for (unsigned i = 0; i < 4; i++)
		printf("%s ", f.toString(bits[i]).c_str());
	printf("\n"); fflush(stdout);

	//Shift each 128-bit range by 8 bits (1 byte)
	/*__m256i ymm0 = _mm256_loadu_si256((__m256i const*)bits);
	__m256i ymm1 = _mm256_slli_si256(ymm0, 1);
	_mm256_storeu_si256((__m256i*)bits, ymm1);*/

	//Shift lower 64 bits by 1 bit
	/*__m256i ymm0 = _mm256_loadu_si256((__m256i const*)bits);
	uint64_t mask[4] = { 1, 0, 0, 0 };
	__m256i ymm1 = _mm256_loadu_si256((__m256i const*)mask);
	__m256i ymm2 = _mm256_sllv_epi64(ymm0, ymm1);
	_mm256_storeu_si256((__m256i*)bits, ymm2);*/

	//Example of a partial vecprod
	//0. Load i, j into vector registers
	uint64_t i = 67, j = 200;
	__m256i ivec = _mm256_set1_epi64x(i);
	__m256i jvec = _mm256_set1_epi64x(j);

	//1. Load row0, row1, mask into ymm registers
	__m256i ymm0 = _mm256_loadu_si256((__m256i const*)bits);
	__m256i ymm1 = _mm256_loadu_si256((__m256i const*)bits1);
	uint64_t mask[4] = { 63, 127, 191, 255 };
	uint64_t mask2[4] = { 0, 64, 128, 192 };
	__m256i ymm2 = _mm256_loadu_si256((__m256i const*)mask);
	__m256i ymm22 = _mm256_loadu_si256((__m256i const*)mask2);
	//2. i-block = i/64 = i >> 6
	uint64_t iblock = i >> 6;
	uint64_t jblock = j >> 6;
	//3. i-shift = i%64 = i & 63
	uint64_t ishift = i & 63;
	uint64_t jshift = j & 63;
	//4. i-mask = { 0, 0, 0, 0 } with i-shift in element i-block
	__m256i ymm5 = _mm256_insert_epi64(_mm256_setzero_si256(), ishift, iblock);
	__m256i ymm6 = _mm256_insert_epi64(_mm256_setzero_si256(), jshift, jblock);
	//4.1 use cmpgt to set blocks lower than i-block to 1's
	__m256i ymm3 = _mm256_cmpgt_epi64(ivec, ymm2);
	__m256i ymm4 = _mm256_cmpgt_epi64(jvec, ymm22);
	//4.2 combine masks
	ymm3 = _mm256_or_si256(ymm3, ymm5);
	ymm4 = _mm256_or_si256(ymm4, ymm6);
	//5. Perform the shifts
	ymm0 = _mm256_srlv_epi64(ymm0, ymm3);
	ymm0 = _mm256_sllv_epi64(ymm0, ymm3);
	//ymm1 = _mm256_sllv_epi64(ymm1, ymm4);
	//ymm1 = _mm256_srlv_epi64(ymm1, ymm4);
	//6. Combine results
	//ymm0 = _mm256_or_si256(ymm0, ymm1);
	//7. Store results
	_mm256_storeu_si256((__m256i*)bits, ymm1);

	printf("Shifted bits:\n");
	for (unsigned i = 0; i < 4; i++)
		printf("%s ", f.toString(bits[i]).c_str());
	printf("\n"); fflush(stdout);
}
