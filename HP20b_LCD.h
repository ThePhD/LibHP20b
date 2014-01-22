#ifndef LIBHP20BLCD_H
#define LIBHP20BLCD_H

#include "Core.h"

extern void HP20b_LCDInit(void);
extern void HP20b_LCDInitContrast(int contrast);
extern void HP20b_LCDQuit(void);

extern void HP20b_LCDS7Char(char letter, int column);
extern void HP20b_LCDS7CharAnd(char letter, int column, int decimalcomma);
extern void HP20b_LCDS7CharAligned(char letter);
extern void HP20b_LCDS7NegativeSign(bool on, bool exponent);

extern void HP20b_LCDS7Clear(void);
extern void HP20b_LCDS7Offset(const char* text, int offset);
extern void HP20b_LCDS7(const char* text);

extern void HP20b_LCDS7Integer(int integer);
extern void HP20b_LCDS7UInteger(uint uinteger);
extern void HP20b_LCDS7Float(float single);
extern void HP20b_LCDS7Decimal(decimal dec);
extern void HP20b_LCDS7Num(Number num);

extern void HP20b_LCDMatrixClear(void);
extern void HP20b_LCDMatrixGraphic(ullong* graphic);
extern void HP20b_LCDMatrixCharAligned(char c);
extern int HP20b_LCDMatrix(const char* text);
extern int HP20b_LCDMatrixOffset(const char* text, int offset);

extern void HP20b_LCDMatrixInteger(int integer);
extern void HP20b_LCDMatrixUInteger(uint uinteger);
extern void HP20b_LCDMatrixFloat(float single);
extern void HP20b_LCDMatrixDecimal(decimal dec);
extern void HP20b_LCDMatrixNum(Number num);

extern void HP20b_LCDBlink(int blink);
extern void HP20b_LCDClear(void);

extern void HP20b_LCDIndicatorSet(HP20bIndicators indicatorid, int on);
extern bool HP20b_LCDIndicatorIsOn(HP20bIndicators indicatorid);
extern bool HP20b_LCDIndicatorToggle(HP20bIndicators indicatorid);
extern void HP20b_LCDIndicatorClear();

#endif /* LIBHP20BLCD_H */
