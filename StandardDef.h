#ifndef LIBHP20BSTANDARDDEF_H
#define LIBHP20BSTANDARDDEF_H

#ifndef ARM
#include <stdio.h>
#endif

typedef char sbool;
typedef unsigned char bool;
typedef signed char schar;
typedef unsigned char uchar;
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef volatile unsigned int Register;
typedef unsigned long ulong;
typedef long long llong;
typedef unsigned long long ullong;

const static bool false = 0;
const static bool true = 1;

#define INT_MIN -2147483647
#define INT_MAX 2147483647
#define UINT_MAX 4294967295U
#define UINT_MIN 0U
#define LLONG_MIN -9223372036854775807LL
#define LLONG_MAX 9223372036854775807LL
#define ULLONG_MIN 0ULL
#define ULLONG_MAX 0xFFFFFFFFFFFFFFFFULL

typedef enum {
	EmptyStack = 0,
	FullStack = EmptyStack,
	SuccessStack = 1,
} StackCode;


extern int Pow10 (int e);
extern int BitCount(uint value);
extern int LLBitCount(ullong value);
extern int Log2(uint value);
//extern int LLlog2(ullong value);
extern int Log10(uint value);
//extern int LLlog10(ullong value);
extern int MostSignificantSet(uint value);
extern int LeastSignificantSet(int value);
extern int LLMostSignificantSet(ullong value);
extern int LLLeastSignificantSet(llong value);
extern int LeadingZeros (uint value);
extern int LLLeadingZeros (ullong value);

extern void* memcpy(void* dst, const void* src, ulong size);
extern void* cmemcpy(void* dst, const void* src, ulong size);
extern void* imemcpy(void* dst, const void* src, ulong size);
extern void* lmemcpy(void* dst, const void* src, ulong size);

int raise (int signum);
llong __aeabi_ldiv0 (llong l);

#endif /* LIBHP20BSTANDARDDEF_H */
