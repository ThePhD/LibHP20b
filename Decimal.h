#ifndef LIBHP20BDECIMAL_H
#define LIBHP20BDECIMAL_H

#include "StandardDef.h"

#define DecimalMax INT_MAX
#define DecimalExponentMax 10U
#define DecimalMantissaBits 32

typedef union {
	byte Cells[5];
	struct {
		uint Mantissa;
		union {
			byte Info;
			struct {
				byte Exponent:7;
				bool Sign:1;
			};
		};
	};
} decimal;

extern void Decimal(decimal* dec, int value, int decimalshift);
extern void DecimalFromParts(decimal* dec, int integerpart, int decimalpart);
extern void DecimalFromInt(decimal* dec, int value);
extern void DecimalFromUInt(decimal* dec, uint value);
extern void DecimalCompact(decimal* dec);
extern void DecimalOverflow(decimal* dec);
extern void DecimalNan(decimal* dec);
extern bool DecimalIsOverflow(decimal* dec);
extern bool DecimalIsNan(decimal* dec);
extern decimal DecimalAdd (decimal left, decimal right);
extern decimal DecimalMultiply (decimal left, decimal right);
extern decimal DecimalDivide (decimal left, decimal right);
extern decimal DecimalSubtract (decimal left, decimal right);
extern decimal DecimaliAdd (decimal left, int right);
extern decimal DecimaliMultiply (decimal left, int right);
extern decimal DecimaliDivide(decimal left, int right);
extern decimal DecimaliSubtract(decimal left, int right);

#endif /* LIBHP20BDECIMAL_H */
