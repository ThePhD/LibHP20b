#ifndef LIBHP20BNUMBER_H
#define LIBHP20BNUMBER_H

#include "StandardDef.h"

typedef union {
	byte All;
	struct {
		byte Low:4;
		byte High:4;
	};
} Nybble;

#define NYBBLEUNUSED 0xCC
#define NYBBLEPARTUNUSED 0xC
#define NUMBERMANTISSASIZE 6
#define NUMBERMAXDIGITS NUMBERMANTISSASIZE * 2
#define NUMBERMAXEXPONENT NUMBERMAXDIGITS
#define NUMBERSIGNALMASK 0xE
#define NUMBERSIGNMASK 0x1
#define NUMBERSIGNALOVERFLOW 0x2
#define NUMBERSIGNALUNDERFLOW 0x4
#define NUMBERSIGNALDIV0 0x6
#define NUMBERSIGNALNAN 0x8
#define NUMBERSIGNED 0x01
#define HNUMBERMANTISSASIZE NUMBERMANTISSASIZE * 2
#define HNUMBERMAXDIGITS HNUMBERMANTISSASIZE * 2
#define HNUMBERMAXEXPONENT HNUMBERMAXDIGITS

typedef union {
	Nybble Cells[(NUMBERMANTISSASIZE * 2) + 2];
	struct {
		union {
			Nybble Mantissa[NUMBERMANTISSASIZE * 2];
			struct {
				Nybble MantissaLowest;
				Nybble MantissaPad[(NUMBERMANTISSASIZE * 2) - 2];
				Nybble MantissaHighest;
			};
		};
		union {
			Nybble Info;
			struct {
				byte Exponent;
			};
		};
		union {
			Nybble HInfo;
			struct {
				byte Sign:4;
			};
		};
	};
} HighNumber;


typedef union {
#if NUMBERMAXEXPONENT > 16
	Nybble Cells[NUMBERMANTISSASIZE + 2];
#else
	Nybble Cells[NUMBERMANTISSASIZE + 1];
#endif
	struct {
		union {
			Nybble Mantissa[NUMBERMANTISSASIZE];
			struct {
				Nybble MantissaLowest;
				Nybble MantissaPad[NUMBERMANTISSASIZE - 2];
				Nybble MantissaHighest;
			};
		};
#if NUMBERMAXEXPONENT > 16
		union {
			Nybble Info[2];
			struct {
				byte Exponent;
				byte Sign:4;
				byte:4;
			};
		};
#else
		union {
			Nybble Info;
			struct {
				byte Exponent:4;
				byte Sign:4;
			};
		};
#endif
	};
} Number;

const static Number One = { { { 1 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 } } };
const static Number Zero = { { { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 } } };
const static Number NegativeOne = { { { 1 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 | (NUMBERSIGNED << 4) } } };

extern void HNumZero(HighNumber* hnum);
extern void HNum(HighNumber* hnum, Number* num);
extern void HNumAlias(HighNumber* hnum, Number* num);
extern void HNumNegate(HighNumber* hnum);
extern void HNumOverflow(HighNumber* hnum);
extern void HNumUnderflow(HighNumber* hnum);
extern void HNumNan(HighNumber* hnum);
extern void HNumDiv0(HighNumber* hnum);
extern bool HNumIsOverflow(HighNumber* hnum);
extern bool HNumIsUnderflow(HighNumber* hnum);
extern bool HNumIsNan(HighNumber* hnum);
extern bool HNumIsDiv0(HighNumber* hnum);
extern int HNumDigitCount(HighNumber* hnum);
extern void HNumSIncrement(HighNumber* hnum);
extern void HNumSNinesComplement(HighNumber* hnum);
extern void HNumSTensComplement(HighNumber* hnum);
extern void HNumSNinesComplementPad(HighNumber* hnum, int pad);
extern void HNumSTensComplementPad(HighNumber* hnum, int pad);
extern void HNumSLeftDigitShift(HighNumber* hnum);
extern void HNumSRightDigitShift(HighNumber* hnum);
extern void HNumSAddDigit(HighNumber* hnum, int digit);

extern int NybbleToBinary(Nybble* nib, bool leftright);
extern int NybbleHighToBinary(Nybble* nib);
extern int NybbleLowToBinary(Nybble* nib);
extern int NybbleAllToBinary(Nybble* nib);

extern void Num(Number* num, int value, int decimalshift);
extern void NumZero(Number* num);
extern void NumFromNum(Number* num, Number* from);
extern void NumFromParts(Number* num, int integerpart, int decimalpart);
extern void NumFromInt(Number* num, int value);
extern void NumFromUInt(Number* num, uint value);
extern void NumOverflow(Number* num);
extern void NumUnderflow(Number* num);
extern void NumNan(Number* num);
extern void NumDiv0(Number* num);
extern bool NumIsOverflow(Number* num);
extern bool NumIsUnderflow(Number* num);
extern bool NumIsNan(Number* num);
extern bool NumIsDiv0(Number* num);
extern bool NumIs0(Number* num);
extern bool NumIsNegative(Number* num);
extern int NumDigitCount(Number* num);

extern Number* NumNegate(Number* num);
extern Number* NumNormalize(Number* num);
extern Number NumIncrement(Number value);
extern Number NumDecrement(Number value);
extern Number NumNinesComplement(Number value);
extern Number NumTensComplement(Number value);
extern Number NumNinesComplementPad(Number value, int pad);
extern Number NumTensComplementPad(Number value, int pad);
extern Number NumLeftDigitShift(Number value, bool exponent);
extern Number NumRightDigitShift(Number value, bool exponent);
extern Number NumLeftDigitShiftBy(Number value, int by);
extern Number NumRightDigitShiftBy(Number value, int by);
extern Number NumAddDigit(Number value, int digit, bool exponent);
extern Number NumRound(Number num);

extern Number* NumSIncrement(Number* num);
extern Number* NumSDecrement(Number* num);
extern Number* NumSNinesComplement(Number* num);
extern Number* NumSTensComplement(Number* num);
extern Number* NumSNinesComplementPad(Number* value, int pad);
extern Number* NumSTensComplementPad(Number* value, int pad);
extern Number* NumSLeftDigitShift(Number* num, bool exponent);
extern Number* NumSRightDigitShift(Number* num, bool exponent);
extern Number* NumSLeftDigitShiftBy(Number* num, int by);
extern Number* NumSRightDigitShiftBy(Number* num, int by);
extern Number* NumSAddDigit(Number* num, int digit, bool exponent);
extern Number* NumSRound(Number* num);

extern Number NumModulo (Number left, Number right);
extern Number NumModuloCount (Number left, Number right, int* count);
extern Number NumInverse(Number* num);
extern Number* NumInverseInto(Number* num, Number* result);

extern Number NumAdd (Number* left, Number* right);
extern Number NumMultiply (Number* left, Number* right);
extern Number NumSubtract (Number* left, Number* right);
extern Number NumDivide (Number* left, Number* right);

extern Number* NumAddInto (Number* left, Number* right, Number* result);
extern Number* NumMultiplyInto (Number* left, Number* right, Number* result);
extern Number* NumSubtractInto (Number* left, Number* right, Number* result);
extern Number* NumDivideInto (Number* left, Number* right, Number* result);

extern int NumCompare(Number* left, Number* right);

#endif /* LIBHP20BNUMBER_H */
