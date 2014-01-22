#include "StandardDef.h"

const static ullong m1  = 0x5555555555555555ULL; //binary: 0101...
const static ullong m2  = 0x3333333333333333ULL; //binary: 00110011..
const static ullong m4  = 0x0f0f0f0f0f0f0f0fULL; //binary:  4 zeros,  4 ones ...
const static ullong h01 = 0x0101010101010101ULL; //the sum of 256 to the power of 0,1,2,3...

int Pow10 (int e) {
	int p = 1;
	while (e-- > 0)
		p *= 10;
	return p;
}

int BitCount (uint i) {
	i = i - ((i >> 1) & 0x55555555);
	i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
	return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

/*int LLBitCount (ullong l) {
	l -= (l >> 1) & m1;             //put count of each 2 bits into those 2 bits
	l = (l & m2) + ((l >> 2) & m2); //put count of each 4 bits into those 4 bits 
	l = (l + (l >> 4)) & m4;        //put count of each 8 bits into those 8 bits 
	return (l * h01) >> 56;  //returns left 8 bits of x + (x<<8) + (x<<16) + (x<<24) + ... 
}*/

int MostSignificantSet (uint k) {
	union {
		unsigned integer;
		float single;
	} uif;
	int n;

	k = k & ~(k >> 1);           /* Fix problem with rounding. */
	uif.single = (float)k;
	n = 158 - (uif.integer >> 23);
	n = (n & 31) + (n >> 6);     /* Fix problem with k = 0. */
	return n;
}

int LeastSignificantSet (int v) {
	static const int MultiplyDeBruijnBitPosition[32] = {
		0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8, 
		31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
	};
	return MultiplyDeBruijnBitPosition[((uint)((v & -v) * 0x077CB531U)) >> 27];
}

/*int LLMostSignificantSet (ullong k) {
	unsigned int r = 0;

	while (k >>= 1) {
		r++;
	}

	return r;
}*/

/*int LLLeastSignificantSet (llong v) {
	static const int MultiplyDebruijinBitPosition[64] = {
		0,  1,  2,  7,  3, 13,  8, 19,  4, 25, 14, 28,  9, 52, 20, 58,
		5, 17, 26, 56, 15, 38, 29, 40, 10, 49, 53, 31, 21, 34, 59, 42,
		63,  6, 12, 18, 24, 27, 51, 57, 16, 55, 37, 39, 48, 30, 33, 41,
		62, 11, 23, 50, 54, 36, 47, 32, 61, 22, 35, 46, 60, 45, 44, 43,
	};
	return MultiplyDebruijinBitPosition[((ullong)(v & -v) * 0x0218a392dd5fb34fULL) >> 58];
}*/

int LeadingZeros(uint value) {
	return 31 - MostSignificantSet(value);
}

/*int LLLeadingZeros(ullong value) {
	return 64 - LLMostSignificantSet(value);
}*/

int Log2 (uint value) {
	return MostSignificantSet(value);
}

/*int LLlog2 (ullong value) {
	return LLMostSignificantSet(value);
}*/

int Log10 (uint value) {
	return MostSignificantSet(value) / Log2(10);
}

/*int LLlog10 (ullong value) {
	return LLMostSignificantSet(value) / LLlog2(10);
}*/

void* memcpy(void* dst, const void* src, ulong size) {
	return lmemcpy(dst, src, size);
}

void* cmemcpy(void* dst, const void* src, ulong size) {
	char* pcDst = (char*)dst;
	char const* pcSrc = (char const*)src;

	while (--size) {
		*pcDst++ = *pcSrc++;
	}

	return dst;
}

void* imemcpy(void* dst, const void* src, ulong size) {
	int* piDst = (int*)dst;
	int const* plSrc = (int const *)src;
	char* pcDst = (char *)piDst;
	char const* pcSrc = (char const *)plSrc;

	if (!((uint)src & 0xFFFFFFFE) && !((uint)dst & 0xFFFFFFFE)) {
		while (size >= 2) {
			*piDst++ = *plSrc++;
			size -= 2;
		}
	}

	while (--size) {
		*pcDst++ = *pcSrc++;
	}

	return dst;
}

void* lmemcpy(void* dst, const void* src, ulong size) {
	// Using long-long (guaranteed 64-bits)
	// ensures that we maximize the amount of data copied
	// per move
	llong* plDst = (llong*)dst;
	llong const* plSrc = (llong const *)src;
	char* pcDst = (char *)plDst;
	char const* pcSrc = (char const *)plSrc;

	// Realign memory addresses
	if (!((uint)src & 0xFFFFFFFC) && !((uint)dst & 0xFFFFFFFC)) {
		while (size >= 4) {
			*plDst++ = *plSrc++;
			size -= 4;
		}
	}

	while (--size) {
		*pcDst++ = *pcSrc++;
	}

	return dst;
}



int raise (int signum) {
	return 0;
}

llong __aeabi_ldiv0 (llong return_value) {
	return 0LL;
}
