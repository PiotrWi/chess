#pragma once

#include <immintrin.h>
#include <cstdint>
/*
int avxPopcount(uint64_t* begin, uint64_t* end)
{
	int out;
	constexpr uint64_t s[] = {1, 2, 4, 8, 18, 32};
	constexpr uint64_t b[] = {0x55'55'55'55'55'55'55'55, 0x33'33'33'33'33'33'33'33, 0x0F'0F'0F'0F'0F'0F'0F'0F, 0x00'FF'00'FF'00'FF'00'FF, 0x00'00'FF'FF'00'00'FF'FF, 0x00'00'FF'FF'00'00'FF'FF, 0x00'00'00'00'FF'FF'FF'FF};
	constexpr uint64_t s1_tab[] = {s[0], s[0], s[0], s[0]};  
	constexpr uint64_t s2_tab[] = {s[1], s[1], s[1], s[1]};
	constexpr uint64_t s3_tab[] = {s[2], s[2], s[2], s[2]};
	constexpr uint64_t s4_tab[] = {s[3], s[3], s[3], s[3]};
	constexpr uint64_t s5_tab[] = {s[4], s[4], s[4], s[4]};
	constexpr uint64_t s6_tab[] = {s[5], s[5], s[5], s[5]};

	constexpr uint64_t b1_tab[] = {b[0], b[0], b[0], b[0]};  
	constexpr uint64_t b2_tab[] = {b[1], b[1], b[1], b[1]};
	constexpr uint64_t b3_tab[] = {b[2], b[2], b[2], b[2]};
	constexpr uint64_t b4_tab[] = {b[3], b[3], b[3], b[3]};
	constexpr uint64_t b5_tab[] = {b[4], b[4], b[4], b[4]};
	constexpr uint64_t b6_tab[] = {b[5], b[5], b[5], b[5]};

	__m256i s0 = _mm256_load_si256((__m256i*)s1_tab);
	__m256i s1 = _mm256_load_si256((__m256i*)s2_tab);
	__m256i s2 = _mm256_load_si256((__m256i*)s3_tab);
	__m256i s3 = _mm256_load_si256((__m256i*)s4_tab);
	__m256i s4 = _mm256_load_si256((__m256i*)s5_tab);
	__m256i s5 = _mm256_load_si256((__m256i*)s6_tab);

	__m256i b0 = _mm256_load_si256((__m256i*)b1_tab);
	__m256i b1 = _mm256_load_si256((__m256i*)b2_tab);
	__m256i b2 = _mm256_load_si256((__m256i*)b3_tab);
	__m256i b3 = _mm256_load_si256((__m256i*)b4_tab);
	__m256i b4 = _mm256_load_si256((__m256i*)b5_tab);
	__m256i b5 = _mm256_load_si256((__m256i*)b6_tab);

	uint64_t* it;
	uint64_t v_i[4];
	for (it = begin; it != end - 3; it +=4)
	{
		v_i[0] = *it;
		v_i[1] = *(it+1);
		v_i[2] = *(it+2);
		v_i[3] = *(it+3);

		__m256i v, c;
		v = _mm256_load_si256((__m256i*)v_i);
		// c = v - ((v >> 1) & b1);
		c = _mm256_sub_epi64(v, (_mm256_and_si256(_mm256_srli_epi64(v, 1), b0)));
		// c = ((c >> s1) & b1) + (c & B1)
		c = _mm256_add_epi64(_mm256_and_si256(_mm256_srli_epi64(v, s[1]), b1), _mm256_and_si256(c, b1));
	}

	return 0;
}*/