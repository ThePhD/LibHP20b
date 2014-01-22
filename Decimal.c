#include "Decimal.h"

void Decimal(decimal* dec, int value, int decimalshift) {
	(*dec).Sign = value < 0;
	if ((*dec).Sign) {
		value = -value;
	}
	if (value > DecimalMax) {
		value = DecimalMax;
	}
	if (decimalshift > DecimalExponentMax) {
		decimalshift = DecimalExponentMax;
	}
	
	(*dec).Mantissa = value;
	(*dec).Exponent = decimalshift;
}

void DecimalFromParts(decimal* dec, int integerpart, int decimalpart) {
	int dc, count = 0, rise = 1;
	if (decimalpart < 0) {
		decimalpart = -decimalpart;
	}
	while (decimalpart != 0 && decimalpart % 10 == 0) {
		decimalpart /= 10;
	}
	dc = decimalpart;
	
	(*dec).Sign = integerpart < 0;
	if (integerpart < 0) {
		integerpart = -integerpart;
	}

	while (dc > 0) {
		++count;
		rise *= 10;
		dc /= 10;
	}
	(*dec).Mantissa = integerpart * rise + decimalpart;
	(*dec).Exponent = count;
}

void DecimalCompact(decimal* dec) {
	while (!((*dec).Mantissa % 100) && (*dec).Exponent > 0) {
		(*dec).Mantissa /= 10;
		--(*dec).Exponent;
	}
}

void DecimalFromInt(decimal* dec, int value) {
	(*dec).Mantissa = value;
	(*dec).Exponent = 0;
	(*dec).Sign = value < 0;
}

void DecimalFromUInt(decimal* dec, uint value) {
	(*dec).Mantissa = value;
	(*dec).Exponent = 0;
	(*dec).Sign = 0;
}

void DecimalOverflow(decimal* dec) {
	(*dec).Mantissa = 0;
	(*dec).Exponent = 0x7F;
	(*dec).Sign = 0;
}

bool DecimalIsOverflow(decimal* dec) {
	return (*dec).Exponent == 0x7F;
}

void DecimalNan(decimal* dec) {
	(*dec).Mantissa = 0;
	(*dec).Exponent = 0x3F;
	(*dec).Sign = 0;
}

bool DecimalIsNan(decimal* dec) {
	return (*dec).Exponent == 0x3F;
}

decimal DecimalAdd (decimal left, decimal right) {
	// The place where the final value goes
	decimal value;

	/* Adding two decimals here! */
	if (left.Exponent == right.Exponent) {
		value.Exponent = left.Exponent;
		if (left.Sign == right.Sign) {
			value.Sign = left.Sign;
			value.Mantissa = left.Mantissa + right.Mantissa;
		}
		else {
			if (right.Sign) {
				value.Sign = left.Mantissa < right.Mantissa;
				if (value.Sign)
					value.Mantissa = right.Mantissa - left.Mantissa;
				else
					value.Mantissa = left.Mantissa - right.Mantissa;
			}
			else {
				value.Sign = left.Mantissa > right.Mantissa;
				if (value.Sign)
					value.Mantissa = left.Mantissa - right.Mantissa;
				else
					value.Mantissa = right.Mantissa - left.Mantissa;
			}
		}
	}
	else {
		if (left.Exponent > right.Exponent) {
			right.Mantissa = right.Mantissa * Pow10(left.Exponent - right.Exponent);
			right.Exponent = left.Exponent;	
		}
		else {
			left.Mantissa = left.Mantissa * Pow10(right.Exponent - left.Exponent);
			left.Exponent = right.Exponent;
		}
		
		value.Exponent = left.Exponent;
		if (left.Sign == right.Sign) {
			value.Sign = left.Sign;
			value.Mantissa = left.Mantissa + right.Mantissa;
		}
		else {
			if (right.Sign) {
				value.Sign = left.Mantissa < right.Mantissa;
				if (value.Sign)
					value.Mantissa = right.Mantissa - left.Mantissa;
				else
					value.Mantissa = left.Mantissa - right.Mantissa;
			}
			else {
				value.Sign = left.Mantissa > right.Mantissa;
				if (value.Sign)
					value.Mantissa = left.Mantissa - right.Mantissa;
				else
					value.Mantissa = right.Mantissa - left.Mantissa;
			}
		}
	}

	// 'return' - we're taking the value and passing it back to whoever called this function
	return value;
}

decimal DecimalSubtract (decimal left, decimal right) {
	// The place where the final value goes
	decimal value;

	/* Subtracting two decimals here! */
	if (left.Exponent == right.Exponent) {

		value.Exponent = left.Exponent;
		value.Sign = !right.Sign;
		if (left.Sign != right.Sign) {
			value.Mantissa = left.Mantissa + right.Mantissa;
		}
		else {
			if (left.Mantissa < right.Mantissa)
				value.Mantissa = right.Mantissa - left.Mantissa;
			else	{
				value.Sign = !value.Sign;
				value.Mantissa = left.Mantissa - right.Mantissa;
			}
		}

	}
	else {
		if (left.Exponent > right.Exponent) {
			right.Mantissa = right.Mantissa * Pow10(left.Exponent - right.Exponent);
			right.Exponent = left.Exponent;	
		}
		else {
			left.Mantissa = left.Mantissa * Pow10(right.Exponent - left.Exponent);
			left.Exponent = right.Exponent;
		}

		value.Exponent = left.Exponent;
		value.Sign = !right.Sign;
		if (left.Sign != right.Sign) {
			value.Mantissa = left.Mantissa + right.Mantissa;
		}
		else {
			if (left.Mantissa < right.Mantissa)
				value.Mantissa = right.Mantissa - left.Mantissa;
			else	{
				value.Sign = !value.Sign;
				value.Mantissa = left.Mantissa - right.Mantissa;
			}
		}

	}
	// 'return' - we're taking the value and passing it back to whoever called this function
	return value;
}

decimal DecimalMultiply (decimal left, decimal right) {
	// The place where the final value goes
	decimal value;
	int lm, rm;
	lm = left.Mantissa / Pow10(left.Exponent);
	rm = right.Mantissa / Pow10(right.Exponent);
	if (Log2(lm) + Log2(rm) > DecimalMantissaBits ) {
		DecimalOverflow(&value);
		return value;
	}

	/* Multiplying two decimals here! */
	value.Exponent = left.Exponent + right.Exponent;
	value.Sign = left.Sign != right.Sign;

	value.Mantissa = left.Mantissa * right.Mantissa;

	DecimalCompact(&value);

	// 'return' - we're taking the value and passing it back to whoever called this function
	return value;
}

decimal DecimalDivide (decimal left, decimal right) {
	// The place where the final value goes
	int precisionplus = 0;
	decimal value;

	/* Dividing two decimals here! */
	value.Sign = left.Sign != right.Sign;

	
	if (left.Mantissa > right.Mantissa) {
		if (left.Mantissa % 10 != 0) {
			precisionplus += left.Exponent;
		}
		precisionplus += Log10(left.Mantissa) - Log10(right.Mantissa);
	}
	else {
		if (right.Mantissa % 10 != 0) {
			precisionplus += right.Exponent;
		}
		precisionplus += Log10(right.Mantissa) - Log10(left.Mantissa);	
	}
	value.Exponent = precisionplus;

	if (left.Exponent < right.Exponent)
		precisionplus += right.Exponent - left.Exponent;
	else
		precisionplus += left.Exponent - right.Exponent;

	value.Mantissa = (left.Mantissa * Pow10(precisionplus)) / (right.Mantissa);
	
	DecimalCompact(&value);

	// 'return' - we're taking the value and passing it back to whoever called this function
	return value;
}
