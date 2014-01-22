#include "Standard.h"

int NumToString (Number value, char* out)  {
	int i = 0, j = 0;
	byte halfmask, halfshift;
	Nybble* v = &value.MantissaLowest;

	char* p = out;

	if (NumIsOverflow(&value)) {
		*p++ = 'o';
		*p++ = 'F';
		*p++ = 'l';
		*p++ = 'o';
		*p++ = 'w';
		*p = '\0';
		return 6;
	}
	else if (NumIsUnderflow(&value)) {
		*p++ = 'u';
		*p++ = 'F';
		*p++ = 'l';
		*p++ = 'o';
		*p++ = 'w';
		*p = '\0';
		return 6;
	}
	else if (NumIsNan(&value)) {
		*p++ = 'N';
		*p++ = 'a';
		*p++ = 'N';
		*p = '\0';
		return 5;
	}
	else if (NumIsDiv0(&value)) {
		*p++ = 'D';
		*p++ = 'i';
		*p++ = 'v';
		*p++ = '/';
		*p++ = '0';
		*p = '\0';
		return 5;
	}

	if (NumIs0(&value)) {
		*p++ = '0';
		*p = 0;
		return 2;
	}

	if (NumIsNegative(&value)) {
		*p++ = '-';
		++j;
	}

	j = i = NumDigitCount(&value);
	if (i == value.Exponent) {
		*p++ = '0';
		*p++ = '.';
		j += 2;
	}

	for (--i, v += (i / 2), halfshift = (i & 1) * 4, halfmask = (0xF) << halfshift; i > -1; --i) {
		halfshift = (i & 1) * 4;
		*p++ = ((v->All & halfmask) >> halfshift) + 48;
		halfmask = ~halfmask;
		v -= (halfmask == 0xF0);
		if (i == value.Exponent && i != 0) {
			*p++ = '.';
			++j;
		}
	}

	*p = 0;
	 
	return j;
}

bool StringToNum (char* text, Number* into) {
	int i = NUMBERMAXDIGITS;
	byte halfmask = 0xF0;
	const char* p = text;
	Nybble* v = &into->MantissaHighest;

	NumZero(into);
	if (*p == '-') {
		NumNegate(into);
		p++;
	}
	if (!(*p >= '0' && *p <= '9') && *p != '.')
		return false;

	while (*p >= '0' && *p <= '9' && i-- > -1) {
		v->All = (v->All & ~ halfmask) | ((*p & 0xF) << ((halfmask == 0xF0) * 4));
		
		halfmask = ~halfmask;
		v -= (halfmask == 0xF0);
		++p;
	}

	if (*p == '.') {
		++p;
		while (*p >= '0' && *p <= '9' && i-- > -1) {
			v->All = (v->All & ~ halfmask) | ((*p & 0xF) << ((halfmask == 0xF0) * 4));

			halfmask = ~halfmask;
			v -= (halfmask == 0xF0);
			++into->Exponent;
			++p;
		}
	}

	NumSRightDigitShiftBy(into, i);
	NumNormalize(into);

	return true;
}

int DecimalToString (decimal value, char* out)  {
	/* How do we get the decimal to a human-readable string? */
	/* This is essentially how we're going to be able to display it */
	int i = 0, j = 0;
	char* p = out;

	if (DecimalIsOverflow(&value)) {
		*p++ = 'o';
		*p++ = 'F';
		*p++ = 'l';
		*p++ = 'o';
		*p++ = 'w';
		*p++ = '!';
		*p = '\0';
		return 6;
	}
	else if (DecimalIsNan(&value)) {
		*p++ = 'N';
		*p++ = 'A';
		*p++ = 'N';
		*p = '\0';
		return 3;
	}

	if (value.Mantissa == 0) {
		*p++ = '0';
		*p = 0;
		return 1;
	}
	if (value.Sign) {
		*p++ = '-';
		++i, ++j;
	}
	if (value.Exponent == 0) {
		for (;value.Mantissa != 0; ++i) {
			*p++ = (value.Mantissa % 10) + 48;
			value.Mantissa /= 10;
		}
	}
	else {
		for (;value.Exponent > 0; ++i, --value.Exponent) {
			*p++ = (value.Mantissa % 10) + 48;
			value.Mantissa /= 10;
		}
		*p++ = '.';
		++i;
		for (;value.Mantissa != 0; ++i, --value.Exponent) {
			*p++ = (value.Mantissa % 10) + 48;
			value.Mantissa /= 10;
		}
	}

	for (--i; j < i; ++j, --i) {
		out[i] ^= out[j];
		out[j] ^= out[i];
		out[i] ^= out[j];
	}

	*p = 0;

	return i + j + (out[0] == '-' ? 0 : 1);
}

bool StringToDecimal (char* text, decimal* into) {
	const char* p = text;
	char c = *p;
	
	into->Sign = 0;
	if (c == '-') {
		into->Sign = 1;
		c = *p++;
	}
	if (!(c >= '0' && c <= '9') && c != '.')
		return false;

	into->Mantissa = 0;
	into->Exponent = 0;
	while (c >= '0' && c <= '9') {
		into->Mantissa = ( into->Mantissa * 10 ) + ( c & 0xF);
		c = *p++;
	}
	if (c == '.') {
		c = *p++;
		while (c >= '0' && c <= '9') {
			into->Mantissa = ( into->Mantissa * 10 ) + ( c & 0xF );
			++into->Exponent;
			c = *p++;
		}
	}
	
	return true;
}

int IntToString (int value, char* out) {
	int i = 0, j = 0;
	char* p = out;
	if (value == 0) {
		*p++ = '0';
		*p = '\0';
		return 1;
	}
	else if (value < 0) {
		value = -value;
		*p++ = '-';
		++i, ++j;
	}
	for (; value != 0; ++i) {
		*p++ = (value % 10) + 48;
		value /= 10;
	}
	for (--i; j < i; ++j, --i) {
		out[i] ^= out[j];
		out[j] ^= out[i];
		out[i] ^= out[j];
	}
	*p = 0;
	return i + j + (out[0] == '-' ? 0 : 1);
}

int UIntToString (uint value, char* out) {
	int i = 0, j = 0;
	char* p = out;
	if (value == 0) {
		*p++ = '0';
		*p = 0;
		return 1;
	}
	for (; value != 0; ++i) {
		*p++ = (value % 10) + 48;
		value /= 10;
	}
	for (--i; j < i; ++j, --i) {
		out[i] ^= out[j];
		out[j] ^= out[i];
		out[i] ^= out[j];
	}
	*p = 0;
	return i + j + 1;
}

int FloatToString (float value, char* out) {
	typedef union {
		long L;
		float F;
	} LF_t;
	ullong mantissa, partfrac, safemask;
	int i = 0, j = 0, safeshift, partint;
	short exp2;
	LF_t x;
	NumberInfo desc;
	char* p, m, max;

	desc.Packed = 0;

	if (value == 0.0){
		out[0] = '0';
		out[1] = '.';
		out[2] = '0';
		out[3] = '\0';
		++desc.DecimalPlaces;
		++desc.DecimalPointIndex;
		++desc.IntegerPlaces;
		return desc.Packed;
	} 
	x.F = value;

	exp2 = (byte)(x.L >> 23) - 127;
	mantissa = (x.L & 0xFFFFFF) | 0x800000;
	partfrac = 0;
	partint = 0;

	safeshift = -(exp2 + 1);
	safemask = 0xFFFFFFFFFFFFFFFFULL >> (64 - 24 - safeshift);

	if (exp2 < -36) {
		out[0] = '0';
		out[1] = '.';
		out[2] = '0';
		out[3] = '\0';
		++desc.DecimalPlaces;
		++desc.DecimalPointIndex;
		++desc.IntegerPlaces;
		return desc.Packed;
	}
	else if (exp2 >= 64) {
		/* |f| >= 2^64 > ULONG_MAX */
		/* NaNs and +-INF are also handled here*/
		partint = INT_MAX;
	}
	else if (exp2 >= 23) {
		partint = (uint)(mantissa << (exp2 - 23));
	}
	else if (exp2 >= 0) {
		partint = (uint)(mantissa >> (23 - exp2));
		partfrac = (mantissa) & safemask;
	}
	else {
		partfrac = (mantissa & 0xFFFFFF);
	}

	p = out;

	if (x.L < 0) {
		*p++ = '-';
		desc.Negative = 1;
		++i;
		++j;
	}

	if (partint == 0) {
		*p++ = '0';
		++desc.IntegerPlaces;
	}
	else {
		for (; partint != 0; ++i) {
			*p++ = (partint % 10) + 48;
			partint /= 10;
			++desc.IntegerPlaces;
		}
		for (--i; j < i; ++j, --i) {
			out[i] ^= out[j];
			out[j] ^= out[i];
			out[i] ^= out[j];
		}
	}
	if (partfrac == 0) {
		*p++ = '.';
		*p++ = '0';
		desc.DecimalPointIndex = desc.IntegerPlaces + desc.Negative;
		++desc.DecimalPlaces;
	}
	else {
		max = ((sizeof(byte) * 8) - (p - out)) & 0x07;
		*p++ = '.';
		desc.DecimalPointIndex = desc.IntegerPlaces + desc.Negative;
		if (max == 0) {
			*p++ = '0';
			++desc.DecimalPlaces;
		}
		else {
			for (m = 0; m < max; ++m) {
				partfrac = (partfrac << 3) + (partfrac << 1);
				*p++ = (char)((partfrac >> (24 + safeshift))) + '0';

				partfrac &= safemask;
				++desc.DecimalPlaces;
			}
			for (--p; p[0] == '0' && p[-1] != '.'; --p);
			++p;
		}
	}
	*p = 0;
	return desc.Packed;
}

bool StringToFloat(char* text, float* into) {
	bool neg = false;
	int pow = 0;
	float num = 0.0f;
	char c = *text;
	const char* p = text;
	
	if (c == '-') {
		neg = true;
		c = *p++;
	}
	if (!(c >= '0' && c <= '9') && c != '.')
		return false;

	*into = 0.0f;
	while (c >= '0' && c <= '9') {
		*into = ( *into * 10.0f ) + ( c & 0xF );
		c = *p++;
	}
	if (c == '.') {
		c = *p++;
		while (c >= '0' && c <= '9') {
			num = ( num * 10.0f ) + ( c & 0xF );
			pow *= 10;
			c = *p++;
		}
		*into += num / pow;
	}

	if (neg)
		*into = -*into;
	
	return true;
}

bool StringToInt(char* text, int* into) {
	bool neg = false;
	char c = *text;
	const char* p = text;

	if (c == '-') {
		neg = true;
		c = *p++;
	}
	if (!(c >= '0' && c <= '9'))
		return false;

	*into = 0;
	while (c >= '0' && c <= '9') {
		*into = ( *into * 10 ) + ( c & 0xF );
		c = *p++;
	}

	if (neg)
		*into = -*into;

	return true;
}

bool StringToUint(char* text, uint* into) {
	char c = *text;
	const char* p = text;

	if (!(c >= '0' && c <= '9'))
		return false;

	*into = 0;
	while (c >= '0' && c <= '9') {
		*into = ( *into * 10 ) + ( c & 0xF );
		c = *p++;
	}

	return true;
}

bool CharIsOperator (char c) {
	return c == '+' || c == '-' || c == '/' || c == '*';
}