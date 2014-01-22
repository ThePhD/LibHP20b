#include "Number.h"

Number value, bringdown, drhs;
HighNumber lhs, rhs, vhs, dlhs, dvhs, dbringdown;


int NybbleToBinary(Nybble* nib, bool high) {
	if (high) {
		return nib->Low;
	}
	else {
		return nib->High;
	}
}

int NybbleHighToBinary(Nybble* nib) {
	return nib->High;
}

int NybbleLowToBinary(Nybble* nib) {
	return nib->Low;
}

int NybbleAllToBinary(Nybble* nib) {
	return nib->Low + nib->High * 10;
}

void HNumAlias(HighNumber* hnum, Number* num) {
	int i = 0, hcount = 0, skew = 0, hhalfshift = 0, nhalfshift = 0;
	byte hhalfmask = 0xF, nhalfmask = 0xF;
	Nybble* n = &num->MantissaLowest;
	Nybble* h = &hnum->MantissaLowest;
	hcount = HNumDigitCount(hnum);
	
	if (HNumIsOverflow(hnum)) {
		NumOverflow(num);
		return;
	}
	else if (HNumIsUnderflow(hnum)) {
		NumUnderflow(num);
		return;
	}
	else if (hcount - hnum->Exponent > NUMBERMAXEXPONENT) {
		NumUnderflow(num);
		return;
	}
	else if (hcount - hnum->Exponent > NUMBERMAXDIGITS) {
		NumOverflow(num);
		return;
	}
	else if (HNumIsDiv0(hnum)) {
		NumDiv0(num);
		return;
	}
	else if (HNumIsNan(hnum)) {
		NumNan(num);
		return;
	}
	
	num->Sign = hnum->Sign;
	
	if (hcount > NUMBERMAXDIGITS) {
		// We will have to trim down some of the number
		skew = (hcount - NUMBERMAXDIGITS);
		num->Exponent = hnum->Exponent - skew;
		
		h += skew / 2;
		hhalfmask <<= ((skew & 0x1) * 4);
		
		for (i = 0; i < NUMBERMAXDIGITS - skew; ++i) {
			hhalfshift = (hhalfmask == 0xF0) * 4;
			nhalfshift = (nhalfmask == 0xF0) * 4;
			
			n->All = (n->All & ~nhalfmask) | (((h->All & hhalfmask) >> hhalfshift) << nhalfshift);
			
			hhalfmask = ~hhalfmask;
			nhalfmask = ~nhalfmask;
			h += hhalfmask == 0x0F;
			n += nhalfmask == 0x0F;
		}
	}
	else {
		num->Exponent = hnum->Exponent;
		for (i = 0; i < NUMBERMANTISSASIZE; ++i) {
			num->Mantissa[i].All = hnum->Mantissa[i].All;
		}
	}

	NumNormalize(num);
}

void HNumZero(HighNumber* num) {
	int i = 1;
	num->Sign = 0;
	num->Exponent = 0;
	num->Mantissa[0].All = 0;
	for (; i < HNUMBERMANTISSASIZE + 1; ++i) {
		num->Mantissa[i].All = 0;
	}
}

void HNum(HighNumber* hnum, Number* num) {
	int i = 0;
	hnum->Sign = num->Sign;
	hnum->Exponent = num->Exponent;
	for (; i < NUMBERMANTISSASIZE; ++i) {
		hnum->Mantissa[i].All = num->Mantissa[i].All;
	}
	for (; i < HNUMBERMANTISSASIZE; ++i) {
		hnum->Mantissa[i].All = 0;
	}
}

void HNumNegate(HighNumber* hnum) {
	hnum->Sign = ((hnum->Sign & NUMBERSIGNMASK) != NUMBERSIGNED) * NUMBERSIGNED;
}

void HNumOverflow(HighNumber* hnum) {
	hnum->Sign = (hnum->Sign & ~NUMBERSIGNALMASK) | NUMBERSIGNALOVERFLOW;
}

bool HNumIsOverflow(HighNumber* hnum) {
	return (hnum->Sign & NUMBERSIGNALMASK) == NUMBERSIGNALOVERFLOW;
}

void HNumUnderflow(HighNumber* hnum) {
	hnum->Sign = (hnum->Sign & ~NUMBERSIGNALMASK) | NUMBERSIGNALOVERFLOW;
}

bool HNumIsUnderflow(HighNumber* hnum) {
	return (hnum->Sign & NUMBERSIGNALMASK) == NUMBERSIGNALOVERFLOW;
}

void HNumNan(HighNumber* hnum) {
	hnum->Sign = (hnum->Sign & ~NUMBERSIGNALMASK) | NUMBERSIGNALNAN;
}

bool HNumIsNan(HighNumber* hnum) {
	return (hnum->Sign & NUMBERSIGNALMASK) == NUMBERSIGNALNAN;
}

void HNumDiv0(HighNumber* hnum) {
	hnum->Sign = (hnum->Sign & ~NUMBERSIGNALMASK) | NUMBERSIGNALDIV0;
}

bool HNumIsDiv0(HighNumber* hnum) {
	return (hnum->Sign & NUMBERSIGNALMASK) == NUMBERSIGNALDIV0;
}

void HNumSIncrement(HighNumber* hnum) {
	int i = 0, high = 0;
	Nybble* v = &hnum->MantissaLowest;
	byte halfmask = 0xF;
	i = hnum->Exponent;
	v += i / 2;
	high = (i & 0x1) * 4;
	halfmask = halfmask << high;

	do {
		if ((v->All & halfmask) == (9 << high)) {
			v->All = (v->All & ~halfmask);
			halfmask = ~halfmask;
			v += high / 4;
			high = ((i + 1) & 0x1) * 4;
			if ((v->All & halfmask) != (9 << high))
				v->All = (v->All & ~halfmask) | ((1 << high) + (v->All & halfmask));
		}
		else {
			v->All = (v->All & ~halfmask) | ((1 << high) + (v->All & halfmask));
			break;
		}
	}
	while ( ++i < HNUMBERMAXDIGITS && ((v->All & halfmask) == (9 << high)) );
}

void HNumSNinesComplement (HighNumber* hnum) {
	int i = HNUMBERMAXDIGITS, halfshift = 0;
	byte halfmask = 0xF0;
	Nybble* n = &hnum->MantissaHighest;

	while (!(n->All & halfmask) && --i > 0 ) {
		halfmask = ~halfmask;
		n -= halfmask == 0xF0;
	}

	for (; i > 0; --i) {
		halfshift = (halfmask == 0xF0) * 4;
		n->All = (n->All & ~halfmask) | ((9 - ((n->All & halfmask) >> halfshift)) << halfshift);
		halfmask = ~halfmask;
		n -= halfmask == 0xF0;
	}
}

void HNumSNinesComplementPad(HighNumber* hnum, int pad) {
	int i = HNUMBERMAXDIGITS, halfshift = 0;
	byte halfmask = 0xF0;
	Nybble* n = &hnum->MantissaHighest;

	if (pad < 0)
		pad = 0;
	else if (pad > HNUMBERMAXDIGITS)
		pad = HNUMBERMAXDIGITS;

	while (--i > pad && !(n->All & halfmask) ) {
		halfmask = ~halfmask;
		n -= halfmask == 0xF0;
	}

	halfmask = ~halfmask;
	n -= halfmask == 0xF0;
	for (; i > 0; --i) {
		halfshift = (halfmask == 0xF0) * 4;
		n->All = (n->All & ~halfmask) | ((9 - ((n->All & halfmask) >> halfshift)) << halfshift);
		halfmask = ~halfmask;
		n -= halfmask == 0xF0;
	}
}

void HNumSTensComplement (HighNumber* hnum) {
	HNumSNinesComplement(hnum);
	HNumSIncrement(hnum);
}

void HNumSTensComplementPad (HighNumber* hnum, int pad) {
	HNumSNinesComplementPad(hnum, pad);
	HNumSIncrement(hnum);
}

void HNumSLeftDigitShift(HighNumber* hnum) {
	int i = HNUMBERMANTISSASIZE - 1;
	Nybble* v = &hnum->MantissaHighest;

	for (; i > 0; --v, --i) {
		v->All <<= 4;
		v->Low = (v - 1)->High;
	}

	v->All <<= 4;

	hnum->MantissaLowest.Low = 0;
}

void HNumSRightDigitShift(HighNumber* hnum) {
	int i = 0;
	Nybble* v = &hnum->MantissaLowest;

	for (; i < HNUMBERMANTISSASIZE - 1; ++v, ++i) {
		v->All >>= 4;
		v->High = (v + 1)->Low;
	}

	v->All >>=4;

	hnum->MantissaHighest.High = 0;

}

int HNumDigitCount(HighNumber* hnum) {
	int count = HNUMBERMAXDIGITS;
	byte halfmask = 0xF0;
	Nybble* n = &hnum->MantissaHighest;
	while (!(n->All & halfmask) && count > hnum->Exponent && --count > -1) {
		halfmask = ~halfmask;
		n -= halfmask == 0xF0;
	}

	return count;
}

void HNumSAddDigit (HighNumber* hnum, int digit) {
	HNumSLeftDigitShift(hnum);
	hnum->MantissaLowest.Low = digit > 9 ? 9 : (digit < 0 ? 0 : digit & 0xF);
}

void NumZero(Number* num) {
	*num = Zero;
}

void NumFromNum(Number* num, Number* from) {
	int i = 0;
	num->Sign = from->Sign;
	num->Exponent = from->Exponent;
	for (; i < NUMBERMANTISSASIZE; ++i) {
		num->Mantissa[i].All = from->Mantissa[i].All;
	}
}

void Num (Number* num, int value, int decimalshift) {
	Nybble* n = (Nybble*)num;
	int halfshift = 0;
	NumZero(num);
	num->Sign = (num->Sign & ~NUMBERSIGNMASK) | ((value < 0) * NUMBERSIGNED);
	if (num->Sign) {
		value = -value;
	}
	if (decimalshift > NUMBERMAXDIGITS) {
		decimalshift = NUMBERMAXDIGITS;
	}
	while (value != 0) {
		n->All |= (value % 10) << halfshift;
		
		n += halfshift >> 2;
		if (halfshift) {
			halfshift = 0;
		}
		else
			halfshift = 4;
		
		value /= 10;
	}
	num->Exponent = decimalshift;
	NumNormalize(num);
}

void NumFromParts (Number* num, int integerpart, int decimalpart) {
	int dc, count = 0, rise = 1, halfshift = 0;
	Nybble* n = (Nybble*)num;
	
	NumZero(num);
	if (decimalpart < 0) {
		decimalpart = -decimalpart;
	}
	while (decimalpart != 0 && decimalpart % 10 == 0) {
		decimalpart /= 10;
	}
	dc = decimalpart;
	
	num->Sign = (num->Sign & ~NUMBERSIGNMASK) | ((integerpart < 0) * NUMBERSIGNED);
	if (num->Sign) {
		integerpart = -integerpart;
	}

	dc = Log10(decimalpart);
	rise = Log10(integerpart);
	count = dc + rise;
	// Lose some digits from direct conversion; Floating Nybble Number cannot handle it
	if (count > NUMBERMAXDIGITS) {
		decimalpart /= Pow10(count - NUMBERMAXDIGITS);
		dc -= (count - NUMBERMAXDIGITS);
		count = NUMBERMAXDIGITS;
	}

	while (decimalpart != 0) {
		n->All |= (decimalpart % 10) << halfshift;

		n += halfshift >> 2;
		if (halfshift) {
			halfshift = 0;
		}
		else
			halfshift = 4;

		decimalpart /= 10;
	}

	while (integerpart != 0) {
		n->All |= (integerpart % 10) << halfshift;

		n += halfshift >> 2;
		if (halfshift) {
			halfshift = 0;
		}
		else
			halfshift = 4;
		
		integerpart /= 10;
	}
	num->Exponent = count;
	NumNormalize(num);
}

void NumFromInt(Number* num, int value) {
	Nybble* n = (Nybble*)num;
	
	NumZero(num);
	num->Sign = (value < 0) * 9;
	if (num->Sign) {
		value = -value;
	}
	while (value != 0) {
		n->Low = value % 10;
		n++->High = (value /= 10) % 10;
		value /= 10;
	}
	n->All = 0;
	num->Exponent = 0;
}

void NumFromUInt(Number* num, uint value) {
	Nybble* n = (Nybble*)num;
	num->Sign = 0;
	while (value != 0) {
		n->Low = value % 10;
		n++->High = (value /= 10) % 10;
		value /= 10;
	}
	n->All = NYBBLEUNUSED;
	num->Exponent = 0;
}

void NumOverflow(Number* num) {
	num->Sign = (num->Sign & ~NUMBERSIGNALMASK) | NUMBERSIGNALOVERFLOW;
}

bool NumIsOverflow(Number* num) {
	return (num->Sign & NUMBERSIGNALMASK) == NUMBERSIGNALOVERFLOW;
}

void NumUnderflow(Number* num) {
	num->Sign = (num->Sign & ~NUMBERSIGNALMASK) | NUMBERSIGNALUNDERFLOW;
}

bool NumIsUnderflow(Number* num) {
	return (num->Sign & NUMBERSIGNALMASK) == NUMBERSIGNALUNDERFLOW;
}

void NumNan(Number* num) {
	num->Sign = (num->Sign & ~NUMBERSIGNALMASK) | NUMBERSIGNALNAN;
}

bool NumIsNan(Number* num) {
	return (num->Sign & NUMBERSIGNALMASK) == NUMBERSIGNALNAN;
}

void NumDiv0(Number* num) {
	num->Sign = (num->Sign & ~NUMBERSIGNALMASK) | NUMBERSIGNALDIV0;
}

bool NumIsDiv0(Number* num) {
	return (num->Sign & NUMBERSIGNALMASK) == NUMBERSIGNALDIV0;
}

bool NumIs0 (Number* num) {
	int i = 0;
	for (; i < NUMBERMANTISSASIZE; i++) {
		if (num->Mantissa[i].All)
			return false;
	}
	return true;
}

int NumDigitCount(Number* num) {
	int count = NUMBERMAXDIGITS;
	byte halfmask = 0xF0;
	Nybble* n = &num->MantissaHighest;
	while (!(n->All & halfmask) && count > num->Exponent && --count > -1 ) {
		halfmask = ~halfmask;
		n -= halfmask == 0xF0;
	}
	return count;
}

Number* NumNegate(Number* num) {
	num->Sign = ((num->Sign & NUMBERSIGNMASK) != NUMBERSIGNED) * NUMBERSIGNED;
	return num;
}

bool NumIsNegative(Number* num) {
	return (num->Sign & NUMBERSIGNMASK) == NUMBERSIGNED;
}

Number* NumNormalize (Number* num) {
	while(num->MantissaLowest.Low == 0 && num->Exponent > 0) {
		NumSRightDigitShift(num, true);
	}
	return num;
}

Number NumIncrement (Number value) {
	return *NumSIncrement(&value);
}

Number* NumSIncrement(Number* num) {
	int i = 0, high = 0;
	Nybble* v = &num->MantissaLowest;
	byte halfmask = 0xF;
	
	if (NumIsNegative(num)) {
		return NumAddInto(num, (Number*)&One, num);
	}
	
	i = num->Exponent;
	v += i / 2;
	high = (i & 0x1) * 4;
	halfmask = halfmask << high;

	do {
		if ((v->All & halfmask) == (9 << high)) {
			v->All = (v->All & ~halfmask);
			halfmask = ~halfmask;
			v += high / 4;
			high = ((i + 1) & 0x1) * 4;
			if ((v->All & halfmask) != (9 << high))
				v->All = (v->All & ~halfmask) | ((1 << high) + (v->All & halfmask));
		}
		else {
			v->All = (v->All & ~halfmask) | ((1 << high) + (v->All & halfmask));
			break;
		}
	}
	while ( ++i < NUMBERMAXDIGITS && ((v->All & halfmask) == (9 << high)) );

	return num;
}

Number NumDecrement(Number value) {
	return NumSubtract(&value, (Number*)&One);
}

Number* NumSDecrement(Number* num) {
	return NumSubtractInto(num, (Number*)&One, num);
}

Number NumLeftDigitShift(Number value, bool exponent) {
	return *NumSLeftDigitShift(&value, exponent);
}

Number NumLeftDigitShiftBy(Number value, int by) {
	return *NumSLeftDigitShiftBy(&value, by);
}

Number* NumSLeftDigitShift(Number* num, bool exponent) {
	int i = NUMBERMANTISSASIZE - 1;
	Nybble* v = &num->MantissaHighest;

	for (; i > 0; --v, --i) {
		v->All <<= 4;
		v->Low = (v - 1)->High;
	}

	v->All <<= 4;
	if (exponent && num->Exponent < NUMBERMAXEXPONENT) {
		++num->Exponent;
	}

	num->MantissaLowest.Low = 0;

	return num;
}

Number* NumSLeftDigitShiftBy(Number* num, int by) {
	int i, b = 0;
	Nybble* v = &num->MantissaLowest;

	for (b = 0; b < by; ++b, v = &num->MantissaHighest) {

		for (i = NUMBERMANTISSASIZE - 1 - (b / 2); i > -1 ; --v, --i) {
			v->All <<= 4;
			v->Low = (v - 1)->High;
		}

		v->All <<= 4;
		v->Low = 0;

	}
	NumNormalize(num);
	return num;
}

Number NumRightDigitShift(Number value, bool exponent) {
	return *NumSRightDigitShift(&value, exponent);
}

Number NumRightDigitShiftBy(Number value, int by) {
	return *NumSRightDigitShiftBy(&value, by);
}

Number* NumSRightDigitShift(Number* num, bool exponent) {
	int i = 0;
	Nybble* v = &num->MantissaLowest;

	for (; i < NUMBERMANTISSASIZE - 1; ++v, ++i) {
		v->All >>= 4;
		v->High = (v + 1)->Low;
	}

	v->All >>=4;
	if (exponent && num->Exponent > 0) {
		--num->Exponent;
	}

	num->MantissaHighest.High = 0;

	return num;
}

Number* NumSRightDigitShiftBy(Number* num, int by) {
	int i, b = 0;
	Nybble* v = &num->MantissaLowest;

	for (b = 0; b < by; ++b, v = &num->MantissaLowest) {

		for (i = 0; i < NUMBERMANTISSASIZE - 1 - (b / 2); ++v, ++i) {
			v->All >>= 4;
			v->High = (v + 1)->Low;
		}

		v->All >>= 4;
		v->High = 0;

	}

	
	return num;
}

Number NumAddDigit (Number value, int digit, bool exponent) {
	return *NumSAddDigit(&value, digit, exponent);
}

Number* NumSAddDigit (Number* num, int digit, bool exponent) {
	NumSLeftDigitShift(num, exponent);
	num->MantissaLowest.Low = digit > 9 ? 9 : (digit < 0 ? 0 : digit & 0xF);
	return num;
}

Number* NumSNinesComplement (Number* num) {
	int pos = NUMBERMANTISSASIZE;
	while (!num->Cells[--pos].All);
	if (num->Cells[pos].High)
		num->Cells[pos].High = 9 - num->Cells[pos].High;
	num->Cells[pos].Low = 9 - num->Cells[pos].Low;
	--pos;

	for (; pos > -1; --pos) {
		num->Cells[pos].High = 9 - num->Cells[pos].High;
		num->Cells[pos].Low = 9 - num->Cells[pos].Low;
	}

	return num;
}

Number* NumSNinesComplementPad (Number* num, int pad) {
	int pos = NUMBERMANTISSASIZE;

	if (pad < 0)
		pad = 0;
	else if (pad > NUMBERMAXDIGITS)
		pad = NUMBERMAXDIGITS;

	while (!num->Cells[--pos].All && pos > (pad >> 1));
	if (num->Cells[pos].High || pos < (pad >> 1))
		num->Cells[pos].High = 9 - num->Cells[pos].High;
	num->Cells[pos].Low = 9 - num->Cells[pos].Low;
	--pos;

	for (; pos > -1; --pos) {
		num->Cells[pos].High = 9 - num->Cells[pos].High;
		num->Cells[pos].Low = 9 - num->Cells[pos].Low;
	}

	return num;
}

Number NumNinesComplement (Number value) {
	return *NumSNinesComplement(&value);
}

Number NumNinesComplementPad (Number value, int pad) {
	return *NumSNinesComplementPad(&value, pad);
}

Number NumTensComplement (Number value) {
	return *NumSIncrement(NumSNinesComplement(&value));
}

Number* NumSTensComplement (Number* num) {
	return NumSIncrement(NumSNinesComplement(num));
}

Number NumTensComplementPad (Number value, int pad) {
	return *NumSIncrement(NumSNinesComplementPad(&value, pad));
}

Number* NumSTensComplementPad (Number* num, int pad) {
	return NumSIncrement(NumSNinesComplementPad(num, pad));
}

Number NumRound(Number num) {
	int erase = 0;
	Nybble* n = &num.MantissaLowest;
	if (num.Exponent > 0) {
		n += (num.Exponent - 1) / 2;
		if (num.Exponent & 0x1) {
			if (n->Low > 4) {
				if (NumIsNegative(&num)) {
					NumSDecrement(&num);
				}
				else {
					NumSIncrement(&num);
				}
				n->Low = 0;
			}
		}
		else {
			if (n->High > 4) {
				if (NumIsNegative(&num)) {
					NumSDecrement(&num);
				}
				else {
					NumSIncrement(&num);
				}
				n->High = 0;
			}
		}
		n = &num.MantissaLowest;
		for (; erase < num.Exponent; ++erase) {
			n->All &= (n->All & ~(0xF << ((erase & 0x1) * 4)));
			n += (erase & 0x1);
		}
	}
	NumNormalize(&num);
	return num;
}

Number NumModulo(Number left, Number right) {
	int cmp = 0, dividendsign = 0;
	if (NumIsNegative(&left)) {
		NumNegate(&left);
		dividendsign = true;
	}
	if (NumIsNegative(&right)) {
		NumNegate(&right);
	}

	cmp = NumCompare(&left, &right);
	if (cmp == 0) {
		NumZero(&right);
		return right;
	}
	else if (cmp == -1) {
		if (dividendsign)
			NumNegate(&left);
		return left;
	}

	while (NumCompare(&left, &right) == 1) {
		left = NumSubtract(&left, &right);
	}

	if (dividendsign)
		NumNegate(&left);

	return left;
}

Number NumModuloCount(Number left, Number right, int* count) {
	int cmp = 0, dividendsign = 0;
	if (NumIsNegative(&left)) {
		NumNegate(&left);
		dividendsign = true;
	}
	if (NumIsNegative(&right)) {
		NumNegate(&right);
	}

	cmp = NumCompare(&left, &right);
	*count = 0;
	if (cmp == 0) {
		NumZero(&left);
		return left;
	}
	else if (cmp == -1) {
		return left;
	}

	while (NumCompare(&left, &right) == 1) {
		++*count;
		left = NumSubtract(&left, &right);
	}

	if (dividendsign)
		NumNegate(&left);

	return left;
}

Number* NumInverseInto(Number* num, Number* result) {
	return NumDivideInto((Number*)&One, num, result);
}

Number NumInverse(Number* num) {
	return *NumDivideInto((Number*)&One, num, &value);
}

Number NumAdd(Number* left, Number* right) {
	return *NumAddInto(left, right, &value);
}

Number NumSubtract(Number* left, Number* right) {
	return *NumSubtractInto(left, right, &value);
}

Number NumMultiply(Number* left, Number* right) {
	return *NumMultiplyInto(left, right, &value);
}

Number NumDivide(Number* left, Number* right) {
	return *NumDivideInto(left, right, &value);
}

Number* NumAddInto (Number* left, Number* right, Number* result) {
	int i, temp = 0, le, re, digitmax, signy;
	byte halfmask;
	Nybble* lbyte = &lhs.MantissaLowest;
	Nybble* rbyte = &rhs.MantissaLowest;
	Nybble* vbyte = &vhs.MantissaLowest;

	if (NumIs0(left)) {
		*result = *right;
		return result;
	}

	if (NumIs0(right)) {
		*result = *left;
		return result;
	}
	
	HNum(&lhs, left);
	HNum(&rhs, right);
	HNumZero(&vhs);

	le = HNumDigitCount(&lhs);
	re = HNumDigitCount(&rhs);
	digitmax = (le > re ? le : re);

	if (right->Sign == left->Sign && digitmax == NUMBERMAXDIGITS && (right->MantissaHighest.High + left->MantissaHighest.High) > 9) {
		NumOverflow(result);
		return result;
	}

	i = lhs.Exponent - rhs.Exponent;
	
	if (i < 0) {
		while (i++ != 0) {
			HNumSLeftDigitShift(&lhs);
			++lhs.Exponent;
			++digitmax;
		}
	}
	else {
		while (i-- != 0) {
			HNumSLeftDigitShift(&rhs);
			++rhs.Exponent;
			++digitmax;
		}
	}

	if (lhs.Sign == rhs.Sign) {
		vhs.Sign = lhs.Sign;
	}
	else {
		if (rhs.Sign) {
			// We need ten's complement of the right now to obtain signed BCD
			// So we can do addition regularly with the negative block
			HNumSNinesComplementPad(&rhs, digitmax);
			signy = left->Sign;
		}
		else {
			// Use the right as the subtrahend if the
			// left wants to be a jerk about it.
			HNumSNinesComplementPad(&lhs, digitmax);
			signy = right->Sign;
		}
	}

	for (i = 0, temp = 0, halfmask = 0x0F; i < HNUMBERMAXDIGITS; ++i) {
		// Shift down only if we're getting the high bits from the Nybble
		le = (halfmask == 0xF0) * 4;

		// Perform basic binary addition
		temp += ((lbyte->All & halfmask) >> le) + ((rbyte->All & halfmask) >> le);
		if (temp > 9) {
			temp += 6;
			vbyte->All = (vbyte->All & ~halfmask) | ((temp & 0xF) << le);
			temp = 1;
		}
		else {
			vbyte->All = (vbyte->All & ~halfmask) | (temp << le);
			temp = 0;
		}

		if (i >= digitmax) {
			break;
		}

		halfmask = ~halfmask;
		re = i & 0x01;
		lbyte += re;
		rbyte += re;
		vbyte += re;
	}

	if (temp > 0) {
		if (digitmax == HNUMBERMAXDIGITS) {
			NumOverflow(result);
			return result;
		}
		else {
			vbyte->All = (vbyte->All & ~halfmask) | ((temp & 0xF) << le);
		}
	}

	if (lhs.Sign != rhs.Sign)  {
		if ( ((vbyte->All & halfmask) >> le) == 1 ) {
			vbyte->All = (vbyte->All & ~halfmask);
			HNumSIncrement(&vhs);
		}
		else {
			vhs.Sign = (vhs.Sign & ~NUMBERSIGNMASK) | (signy == 0x0) * NUMBERSIGNED;
			HNumSNinesComplement(&vhs);
			if ((vhs.Sign & NUMBERSIGNMASK) == NUMBERSIGNED && HNumDigitCount(&vhs) == 0) {
				vhs.Sign &= ~NUMBERSIGNMASK;
			}
		}
	}

	vhs.Exponent = lhs.Exponent > rhs.Exponent ? lhs.Exponent : rhs.Exponent;

	HNumAlias(&vhs, result);
	NumNormalize(result);
	
	// 'return' - we're taking the value and passing it back to whoever called this function
	return result;
}

Number* NumSubtractInto (Number* left, Number* right, Number* result) {
	int i, temp = 0, le, re, digitmax, signy;
	byte halfmask;
	Nybble* lbyte = &lhs.MantissaLowest;
	Nybble* rbyte = &rhs.MantissaLowest;
	Nybble* vbyte = &vhs.MantissaLowest;

	if (NumIs0(left)) {
		*result = *right;
		return result;
	}
	
	if (NumIs0(right)) {
		*result = *left;
		return result;
	}

	HNum(&lhs, left);
	HNum(&rhs, right);
	HNumZero(&vhs);

	le = HNumDigitCount(&lhs);
	re = HNumDigitCount(&rhs);
	digitmax = (le > re ? le : re);

	if (right->Sign == left->Sign && digitmax == NUMBERMAXDIGITS && (right->MantissaHighest.High + left->MantissaHighest.High) > 9) {
		NumOverflow(result);
		return result;
	}

	i = lhs.Exponent - rhs.Exponent;

	if (i < 0) {
		while (i++ != 0) {
			HNumSLeftDigitShift(&lhs);
			++lhs.Exponent;
			++digitmax;
		}
	}
	else {
		while (i-- != 0) {
			HNumSLeftDigitShift(&rhs);
			++rhs.Exponent;
			++digitmax;
		}
	}

	if (lhs.Sign != rhs.Sign) {
		vhs.Sign = lhs.Sign;
	}
	else {
		if (lhs.Sign) {
			// We need ten's complement of the right now to obtain signed BCD
			// So we can do addition regularly with the negative block
			HNumSNinesComplementPad(&lhs, digitmax);
			signy = right->Sign;
		}
		else {
			// Use the right as the subtrahend if the
			// left wants to be a jerk about it.
			HNumSNinesComplementPad(&rhs, digitmax);
			signy = left->Sign;
		}
	}

	for (i = 0, temp = 0, halfmask = 0x0F; i < HNUMBERMAXDIGITS; ++i) {
		// Shift down only if we're getting the high bits from the Nybble
		le = (halfmask == 0xF0) * 4;

		// Perform basic binary addition
		temp += ((lbyte->All & halfmask) >> le) + ((rbyte->All & halfmask) >> le);
		if (temp > 9) {
			temp += 6;
			vbyte->All = (vbyte->All & ~halfmask) | ((temp & 0xF) << le);
			temp = 1;
		}
		else {
			vbyte->All = (vbyte->All & ~halfmask) | (temp << le);
			temp = 0;
		}

		if (i >= digitmax) {
			break;
		}

		halfmask = ~halfmask;
		re = i & 0x01;
		lbyte += re;
		rbyte += re;
		vbyte += re;
	}

	if (temp > 0) {
		if (digitmax == HNUMBERMAXDIGITS) {
			NumOverflow(result);
			return result;
		}
		else {
			vbyte->All = (vbyte->All & ~halfmask) | ((temp & 0xF) << le);
		}
	}

	if (lhs.Sign == rhs.Sign)  {
		if ( ((vbyte->All & halfmask) >> le) == 1 ) {
			vbyte->All = (vbyte->All & ~halfmask);
			HNumSIncrement(&vhs);
		}
		else {
			vhs.Sign = (vhs.Sign & ~NUMBERSIGNMASK) | (signy == 0x0) * NUMBERSIGNED;
			HNumSNinesComplement(&vhs);
			if ((vhs.Sign & NUMBERSIGNMASK) == NUMBERSIGNED && HNumDigitCount(&vhs) == 0) {
				vhs.Sign &= ~NUMBERSIGNMASK;
			}
		}
	}

	vhs.Exponent = lhs.Exponent > rhs.Exponent ? lhs.Exponent : rhs.Exponent;

	HNumAlias(&vhs, result);
	NumNormalize(result);

	// 'return' - we're taking the value and passing it back to whoever called this function
	return result;
}

Number* NumMultiplyInto (Number* left, Number* right, Number* result) {
	// The place where the final value goes
	int i, j, innercarry = 0, multemp = 0, vmultemp = 0, innermultemp = 0, remtemp = 0, ldc, rdc, le = 0, re = 0, digitmax = 0;
	byte halfmask, innerhalfmask, vhalfmask;
	Nybble* lbyte = &lhs.MantissaLowest;
	Nybble* rbyte = &rhs.MantissaLowest;
	Nybble* vbyte = &vhs.MantissaLowest;

	if (NumIs0(left)) {
		*result = *left;
		return result;
	}
	else if (NumIs0(right)) {
		*result = *right;
		return result;
	}

	HNum(&lhs, left);
	HNum(&rhs, right);
	HNumZero(&vhs);

	ldc = HNumDigitCount(&lhs);
	rdc = HNumDigitCount(&rhs);
	digitmax = ldc + rdc;

	vhs.Exponent = lhs.Exponent + rhs.Exponent;

	if (digitmax - vhs.Exponent > NUMBERMAXDIGITS) {
		NumOverflow(result);
		return result;
	}

	vhs.Sign = (vhs.Sign & ~ NUMBERSIGNMASK) | (((left->Sign & NUMBERSIGNMASK) != (right->Sign & NUMBERSIGNMASK)) * NUMBERSIGNED);

	for (i = 0, re = 1, j = 0, halfmask = 0xF; i < ldc; ++i) {
		// Shift down only if we're getting the high bits from the Nybble
		le = (halfmask == 0xF0) * 4;
		multemp = ((lbyte->All & halfmask) >> le);
		vbyte += (i / 2);

		if (multemp != 0) {
			for (j = 0, innercarry = 0, innerhalfmask = 0xF,
				vhalfmask = (0xF << ((i & 0x1) * 4));
				j < rdc || innercarry != 0; j++) {
				le = (innerhalfmask == 0xF0) * 4;
				re = (vhalfmask == 0xF0) * 4;
				// Perform basic binary multiplication
				innermultemp = multemp
					* ((rbyte->All & innerhalfmask) >> le)
					+ innercarry;
				innercarry = innermultemp / 10;
				remtemp = innermultemp - (innercarry * 10);

				vmultemp = (remtemp + ((vbyte->All & vhalfmask) >> re));
				if (vmultemp > 9) {
					innercarry += vmultemp / 10;
					remtemp = vmultemp - ((vmultemp / 10) * 10);
					vbyte->All = (vbyte->All & ~vhalfmask) | (remtemp << re);
				}
				else {
					vbyte->All = (vbyte->All & ~vhalfmask) | (vmultemp << re);
				}
				
				innerhalfmask = ~innerhalfmask;
				vhalfmask = ~vhalfmask;
				rbyte += (j & 0x01);
				vbyte += (vhalfmask == 0xF);
			}
		}

		if (innercarry != 0) {
			vbyte->All = (vbyte->All & ~innerhalfmask) | ((innercarry << le) + (vbyte->All & ~innerhalfmask));
		}

		halfmask = ~halfmask;
		vbyte = &vhs.MantissaLowest;
		rbyte = &rhs.MantissaLowest;
		lbyte += (i & 0x01);
	}

	HNumAlias(&vhs, result);
	NumNormalize(result);

	// 'return' - we're taking the value and passing it back to whoever called this function
	return result;
}

Number* NumDivideInto (Number* left, Number* right, Number* result) {
	// The place where the final value goes
	int i, rci = 0, vi = 0, cmp = 1, le = 0, re = 0;
	bool leftlim = false;
	byte halfmask, vhalfmask;
	Nybble* lbyte = &dlhs.MantissaLowest;
	Nybble* rbyte = &drhs.MantissaLowest;
	Nybble* vbyte = &dvhs.MantissaHighest;

	HNumZero(&dvhs);
	HNum(&dlhs, left);
	drhs = *right;
	NumZero(&bringdown);

	dvhs.Sign = (dvhs.Sign & ~NUMBERSIGNMASK) | (((left->Sign & NUMBERSIGNMASK) != (right->Sign & NUMBERSIGNMASK)) * NUMBERSIGNED);

	if (NumIs0(left)) {
		*result = *left;
		return result;
	}
	else if (NumIs0(right)) {
		NumDiv0(result);
		return result;
	}
	if (NumIsNegative(&drhs)) {
		NumNegate(&drhs);
	}
	if (NumIsNegative(left)) {
		HNumNegate(&dlhs);
	}

	if (left->Exponent + right->Exponent > NUMBERMAXDIGITS) {
		NumUnderflow(result);
		return result;
	}

	le = NumDigitCount(left);
	lbyte += (le - 1) / 2;
	re = NumDigitCount(right);
	rbyte += (re - 1) / 2;

	i = drhs.Exponent;
	while (i-- != 0) {
		--drhs.Exponent;
		if (dlhs.Exponent > 0) {
			--dlhs.Exponent;
		}
		else {
			//NumSLeftDigitShift(&drhs, false);
		}
	}

	for (i = 0, halfmask = (0x0F << (((le & 0x1) == 0) * 4)); i < re && !leftlim; ++i) {
		vi = halfmask == 0xF0;
		
		dvhs.Exponent += i > le;
		NumSAddDigit(&bringdown, (lbyte->All & halfmask) >> vi * 4, false);
		
		halfmask = ~halfmask;
		lbyte -= !vi;
		if (lbyte - &dlhs.MantissaLowest < 0) {
			leftlim = true;
		}
	}

	vi = 0;
	vhalfmask = 0xF0;
	while (vi < NUMBERMAXDIGITS) {
		// Compare until the carry is bigger than the piece
		cmp = 0;
		while (NumCompare(&drhs, &bringdown) == 1 && !bringdown.MantissaHighest.High) {
			if (cmp > 0) {
				if (dvhs.MantissaHighest.High) {
					vbyte->All = (vbyte->All & ~vhalfmask) | (0 << ((vhalfmask == 0xF0) * 4));
					vhalfmask = ~vhalfmask;
					++vi;
					vbyte -= (vhalfmask == 0xF0);
					if (vi >= NUMBERMAXDIGITS) {
						break;
					}
				}
			}
			rci = (halfmask == 0xF0) * 4;
			if (leftlim) {
				NumSLeftDigitShift(&bringdown, false);
				++dvhs.Exponent;
			}
			else {
				NumSAddDigit(&bringdown, (lbyte->All & halfmask) >> rci, false);
				lbyte -= !rci;
				if (lbyte - &dlhs.MantissaLowest < 0)
					leftlim = true;
			}	
			halfmask = ~halfmask;
			++cmp;
		}
		if (vi >= NUMBERMAXDIGITS) {
			break;
		}
		// Get right amount of divides for the current rhs vs. bringdown
		cmp = 0;
		// In this case, repeated subtraction. Essentially, Modulus with a counter
		while (NumCompare(&drhs, &bringdown) != 1) {
			++cmp;
			NumSubtractInto(&bringdown, &drhs, &bringdown);
		}
		
		if (cmp > 9) {
			vbyte->All = (vbyte->All & ~vhalfmask) | ((cmp - 10) << ((vhalfmask == 0xF0) * 4));
			cmp = 1;
		}
		else {
			vbyte->All = (vbyte->All & ~vhalfmask) | (cmp << ((vhalfmask == 0xF0) * 4));
			cmp = 0;
		}
		vhalfmask = ~vhalfmask;
		++vi;
		vbyte -= (vhalfmask == 0xF0);
		if (leftlim && NumIs0(&bringdown))
			break;
	}

	if (cmp > 0) {
		if (vi < NUMBERMAXDIGITS) {
			vbyte->All = (vbyte->All & ~vhalfmask) | (cmp << ((vhalfmask == 0xF0) * 4));
		}
	}

	while (!dvhs.MantissaLowest.Low)
		HNumSRightDigitShift(&dvhs);

	dvhs.Exponent += (left->Exponent - right->Exponent);

	HNumAlias(&dvhs, result);

	// 'return' - we're taking the value and passing it back to whoever called this function
	return result;
}

int NumCompare(Number* left, Number* right) {
	int skew = 0, maxcomp = -1, lskew = 0, rskew = 0, bastardize = 1;
	Nybble* lbyte = &left->MantissaHighest;
	Nybble* rbyte = &right->MantissaHighest;
	byte lhalfmask = 0xF0, rhalfmask = 0xF0;
	
	skew = -(left->Sign == NUMBERSIGNED) + (right->Sign == NUMBERSIGNED);
	if (skew) {
		return skew;
	}
	else if (left->Sign) {
		bastardize = -1;
	}

	skew = left->Exponent - right->Exponent;
	if (skew < 0) {
		skew = -skew;
		rskew = 0;
		if (skew == NUMBERMAXDIGITS - 1) {
			return -1 * bastardize;
		}
		lbyte -= skew;
		lhalfmask >>= ((skew & 0x01) * 4);
	}
	else {
		if (skew == NUMBERMAXDIGITS - 1) {
			return 1 * bastardize;
		}
		lskew = skew;
		rbyte -= skew;
		rhalfmask >>= (skew & 0x01) * 4;
	}

	for (maxcomp += skew, skew = NUMBERMAXDIGITS - 1; skew > maxcomp;
		--skew, rhalfmask = ~rhalfmask, lhalfmask = ~lhalfmask,
		rbyte -= (rhalfmask == 0xF0), lbyte -= (lhalfmask == 0xF0)) {
		if (!lbyte->All && !rbyte->All) {
			continue;
		}
		if ( ((lbyte->All & lhalfmask) >> ((lhalfmask == 0xF0) * 4))
			> ((rbyte->All & rhalfmask) >> ((rhalfmask == 0xF0) * 4)) ) {
				return 1 * bastardize;
		}
		if ( ((lbyte->All & lhalfmask) >> ((lhalfmask == 0xF0) * 4))
			< ((rbyte->All & rhalfmask) >> ((rhalfmask == 0xF0) * 4)) ) {
				return -1 * bastardize;
		}
	}

	if (lskew > 0) {
		lhalfmask = ~lhalfmask;
		lbyte -= (lhalfmask == 0xF0);
		for (; lskew > -1; --lskew) {
			if (lbyte->All & lhalfmask)
				return 1 * bastardize;
			lhalfmask = ~lhalfmask;
			lbyte -= (lhalfmask == 0xF0);
		}
	}
	else if (rskew > 0) {
		rhalfmask = ~rhalfmask;
		rbyte -= (rhalfmask == 0xF0);
		for (; rskew > -1; --rskew) {
			if (rbyte->All & rhalfmask)
				return -1 * bastardize;
			rhalfmask = ~rhalfmask;
			rbyte -= (rhalfmask == 0xF0);
		}
	}

	return 0;
}
