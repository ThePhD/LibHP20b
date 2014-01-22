#ifndef LIBHP20BSTANDARD_H
#define LIBHP20BSTANDARD_H

#include "Architecture.h"
#include "StandardDef.h"
#include "Number.h"
#include "Decimal.h"

#ifndef ARM
#include <stdio.h>
#endif

typedef union {
	uint Packed;
	struct {
		byte IntegerPlaces;
		byte DecimalPlaces;
		byte DecimalPointIndex;
		byte Negative;
	};
} NumberInfo;

extern int DecimalToString(decimal value, char* out);
extern int NumToString(Number value, char* out);
extern int IntToString(int value, char* out);
extern int UIntToString(uint value, char* out);
extern int FloatToString(float value, char* out);

extern bool StringToDecimal(char* text, decimal* into);
extern bool StringToNum(char* text, Number* into);
extern bool StringToInt(char* text, int* into);
extern bool StringToUint(char* text, uint* into);
extern bool StringToFloat(char* text, float* into);

extern bool CharIsOperator (char c);

#endif /* LIBHP20BSTANDARD_H */
