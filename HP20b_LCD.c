#include "AT91SAM7L128.h"
#include "HP20b_LCD.h"

char cbuffer[2];

// LCD configuration
#define HP20b_LCDSegment7Max 12
#define HP20b_LCDSegment7MaxIndex 11
#define HP20b_LCDSegment7FullMax 15
#define HP20b_LCDSegment7FullMaxIndex 14
#define HP20b_LCDCOMMON_NUMBER 9 << 0	 // 10 commons
#define HP20b_LCDSEGMENT_NUMBER	39 << 8	// 40 segments

// Graphic Font for every printable ASCII character above 32
const static byte MatrixFont[][7] = {
	{ 5, 0, 0, 0, 0, 0, 0 },
	{ 2, 1, 1, 1, 1, 0, 1 },
	{ 4, 5, 5, 5, 0, 0, 0 },
	{ 6, 0, 10, 31, 10, 31, 10 },
	{ 6, 4, 30, 5, 14, 20, 15 },
	{ 6, 0, 18, 8, 4, 2, 9 },
	{ 6, 6, 9, 6, 25, 9, 22 },
	{ 2, 1, 1, 1, 0, 0, 0 },
	{ 3, 2, 1, 1, 1, 1, 2 },
	{ 3, 1, 2, 2, 2, 2, 1 },
	{ 6, 0, 10, 4, 31, 4, 10 },
	{ 6, 0, 4, 4, 31, 4, 4 },
	{ 3, 0, 0, 3, 3, 2, 1 },
	{ 6, 0, 0, 0, 31, 0, 0 },
	{ 3, 0, 0, 0, 0, 3, 3 },
	{ 6, 0, 16, 8, 4, 2, 1 },
	{ 6, 14, 17, 25, 21, 19, 14 },
	{ 4, 2, 3, 2, 2, 2, 7 },
	{ 6, 14, 17, 16, 12, 2, 31 },
	{ 6, 14, 17, 12, 16, 17, 14 },
	{ 6, 12, 10, 9, 31, 8, 8 },
	{ 6, 31, 1, 15, 16, 16, 15 },
	{ 6, 14, 1, 15, 17, 17, 14 },
	{ 6, 31, 16, 8, 4, 4, 4 },
	{ 6, 14, 17, 14, 17, 17, 14 },
	{ 6, 14, 17, 17, 30, 16, 14 },
	{ 3, 0, 3, 3, 0, 3, 3 },
	{ 3, 0, 3, 3, 0, 3, 2 },
	{ 4, 8, 4, 2, 1, 2, 4 },
	{ 6, 0, 0, 31, 0, 31, 0 },
	{ 4, 1, 2, 4, 8, 4, 2 },
	{ 6, 14, 17, 8, 4, 0, 4 },
	{ 6, 14, 17, 21, 29, 1, 14 },
	{ 6, 14, 17, 17, 31, 17, 17 },
	{ 6, 15, 17, 15, 17, 17, 15 },
	{ 6, 14, 17, 1, 1, 17, 14 },
	{ 6, 15, 17, 17, 17, 17, 15 },
	{ 6, 31, 1, 15, 1, 1, 31 },
	{ 6, 31, 1, 15, 1, 1, 1 },
	{ 6, 14, 17, 1, 25, 17, 14 } ,
	{ 6, 17, 17, 17, 31, 17, 17 } ,
	{ 6, 31, 4, 4, 4, 4, 31 } ,
	{ 6, 16, 16, 16, 17, 17, 14 } ,
	{ 6, 17, 9, 7, 9, 17, 17 } ,
	{ 6, 1, 1, 1, 1, 1, 31 } ,
	{ 6, 17, 27, 21, 17, 17, 17 } ,
	{ 6, 17, 17, 19, 21, 25, 17 } ,
	{ 6, 14, 17, 17, 17, 17, 14 } ,
	{ 6, 15, 17, 17, 15, 1, 1 } ,
	{ 6, 14, 17, 17, 21, 9, 22 } ,
	{ 6, 15, 17, 17, 15, 17, 17 } ,
	{ 6, 14, 17, 6, 8, 17, 14 } ,
	{ 6, 31, 4, 4, 4, 4, 4 } ,
	{ 6, 17, 17, 17, 17, 17, 14 } ,
	{ 6, 17, 17, 17, 17, 10, 4 } ,
	{ 6, 17, 17, 17, 21, 27, 17 } ,
	{ 6, 17, 17, 10, 4, 10, 17 } ,
	{ 6, 17, 17, 10, 4, 4, 4 } ,
	{ 6, 31, 8, 4, 2, 1, 31 } ,
	{ 4, 7, 1, 1, 1, 1, 7 } ,
	{ 6, 0, 1, 2, 4, 8, 16 } ,
	{ 4, 7, 4, 4, 4, 4, 7 } ,
	{ 6, 4, 10, 17, 0, 0, 0 } ,
	{ 6, 0, 0, 0, 0, 0, 31 } ,
	{ 2, 1, 1, 2, 0, 0, 0 } ,
	{ 5, 0, 0, 7, 14, 9, 14 } ,
	{ 5, 1, 1, 7, 9, 9, 7 } ,
	{ 5, 0, 0, 6, 1, 1, 14 } ,
	{ 5, 8, 8, 14, 9, 9, 14 } ,
	{ 5, 0, 0, 6, 15, 1, 6 } ,
	{ 5, 6, 9, 1, 3, 1, 1 } ,
	{ 5, 0, 6, 9, 14, 8, 7 } ,
	{ 5, 1, 1, 7, 9, 9, 9 } ,
	{ 2, 0, 1, 0, 1, 1, 1 } ,
	{ 5, 8, 0, 12, 8, 9, 6 } ,
	{ 5, 1, 1, 9, 7, 9, 9 } ,
	{ 4, 3, 2, 2, 2, 2, 7 } ,
	{ 6, 0, 0, 15, 21, 17, 17 } ,
	{ 5, 0, 0, 7, 9, 9, 9 } ,
	{ 5, 0, 0, 6, 9, 9, 6 } ,
	{ 5, 0, 7, 9, 9, 7, 1 } ,
	{ 5, 0, 14, 9, 9, 14, 8 } ,
	{ 5, 0, 0, 13, 3, 1, 1 } ,
	{ 5, 0, 0, 14, 7, 8, 7 } ,
	{ 5, 1, 1, 7, 1, 9, 6 } ,
	{ 5, 0, 0, 9, 9, 9, 14 } ,
	{ 5, 0, 0, 9, 9, 5, 3 } ,
	{ 6, 0, 0, 17, 21, 21, 15 } ,
	{ 5, 0, 0, 9, 6, 6, 9 } ,
	{ 5, 0, 9, 9, 14, 8, 6 } ,
	{ 5, 0, 0, 15, 4, 2, 15 } ,
	{ 4, 6, 2, 1, 2, 6, 0 } ,
	{ 2, 1, 1, 1, 1, 1, 1 } ,
	{ 3, 3, 2, 4, 2, 3, 0 } ,
	{ 6, 0, 2, 21, 8, 0, 0 } ,
	{ 7, 21, 42, 21, 42, 21, 42 } ,
	{ 5, 0, 8, 4, 2, 5, 15 } ,
	{ 5, 15, 0, 9, 6, 6, 9 } ,
	{ 6, 31, 0, 9, 10, 4, 3 } ,
	{ 6, 28, 4, 4, 5, 6, 4 } ,
	{ 6, 8, 20, 20, 5, 5, 2 } ,
	{ 5, 15, 2, 4, 2, 1, 15 } ,
	{ 5, 3, 7, 15, 15, 7, 3 } ,
	{ 6, 1, 31, 10, 10, 10, 10 } ,
	{ 5, 2, 4, 14, 9, 9, 6 } ,
	{ 5, 8, 4, 2, 15, 0, 15 } ,
	{ 5, 1, 2, 4, 15, 0, 15 } ,
	{ 5, 0, 8, 15, 6, 15, 1 } ,
	{ 6, 0, 0, 22, 9, 9, 22 } ,
	{ 6, 0, 4, 8, 31, 8, 4 } ,
	{ 6, 0, 4, 2, 31, 2, 4 } ,
	{ 6, 0, 4, 4, 21, 14, 4 } ,
	{ 6, 0, 4, 14, 21, 4, 4 } ,
	{ 6, 0, 0, 18, 21, 8, 8 } ,
	{ 5, 2, 4, 14, 9, 9, 6 } ,
	{ 4, 0, 6, 1, 7, 1, 6 } ,
	{ 6, 0, 10, 21, 20, 16, 16 } ,
	{ 5, 0, 6, 9, 15, 9, 6 } ,
	{ 6, 0, 1, 2, 4, 10, 17 } ,
	{ 6, 12, 18, 18, 14, 2, 1 } ,
	{ 6, 0, 0, 30, 9, 9, 6 } ,
	{ 6, 0, 0, 30, 5, 20, 8 } ,
	{ 6, 0, 0, 9, 17, 21, 10 } ,
	{ 6, 0, 4, 10, 17, 31, 0 } ,
	{ 6, 31, 10, 10, 10, 10, 10 } ,
	{ 6, 14, 17, 17, 17, 10, 27 } ,
	{ 5, 0, 0, 14, 14, 14, 0 } ,
	{ 6, 0, 0, 10, 21, 21, 10 } ,
	{ 6, 28, 2, 15, 7, 2, 28 } ,
	{ 2, 2, 0, 2, 2, 2, 2 } ,
	{ 5, 4, 14, 5, 5, 14, 4 } ,
	{ 5, 4, 10, 2, 7, 2, 15 } ,
	{ 5, 9, 6, 9, 9, 6, 9 } ,
	{ 6, 17, 10, 31, 4, 31, 4 } ,
	{ 3, 0, 4, 4, 0, 4, 4 } ,
	{ 5, 4, 2, 6, 9, 6, 4 } ,
	{ 5, 10, 0, 0, 0, 0, 0 } ,
	{ 6, 17, 23, 19, 23, 17, 14 } ,
	{ 5, 8, 14, 9, 6, 15, 0 } ,
	{ 6, 0, 20, 10, 5, 10, 20 } ,
	{ 5, 0, 0, 0, 15, 8, 0 } ,
	{ 5, 0, 0, 0, 15, 0, 0 } ,
	{ 6, 17, 23, 23, 27, 17, 14 } ,
	{ 5, 15, 0, 0, 0, 0, 0 } ,
	{ 5, 14, 10, 14, 0, 0, 0 } ,
	{ 5, 0, 4, 14, 4, 14, 0 } ,
	{ 4, 7, 4, 7, 1, 7, 0 } ,
	{ 4, 4, 7, 4, 7, 0, 0 } ,
	{ 6, 8, 12, 9, 28, 0, 0 } ,
	{ 6, 0, 9, 9, 9, 23, 1 } ,
	{ 6, 30, 23, 23, 22, 20, 22 } ,
	{ 4, 0, 0, 6, 6, 0, 0 } ,
	{ 3, 0, 0, 0, 0, 4, 3 } ,
	{ 5, 4, 4, 14, 0, 0, 0 } ,
	{ 5, 9, 9, 6, 0, 15, 0 } ,
	{ 6, 0, 5, 10, 20, 10, 5 } ,
	{ 6, 9, 5, 2, 21, 28, 16 } ,
	{ 6, 9, 5, 26, 17, 8, 24 } ,
	{ 6, 18, 11, 6, 23, 28, 16 } ,
	{ 6, 4, 0, 4, 2, 1, 17 } ,
	{ 5, 4, 6, 9, 15, 9, 9 } ,
	{ 5, 2, 6, 9, 15, 9, 9 } ,
	{ 5, 9, 0, 6, 9, 15, 9 } ,
	{ 5, 5, 6, 9, 15, 9, 9 } ,
	{ 5, 0, 6, 9, 15, 9, 9 } ,
	{ 5, 9, 6, 9, 15, 9, 9 } ,
	{ 5, 10, 5, 5, 15, 5, 13 } ,
	{ 5, 6, 9, 1, 9, 6, 4 } ,
	{ 5, 4, 15, 1, 7, 1, 15 } ,
	{ 5, 4, 15, 1, 7, 1, 15 } ,
	{ 5, 10, 15, 1, 7, 1, 15 } ,
	{ 5, 0, 15, 1, 7, 1, 15 } ,
	{ 4, 2, 7, 2, 2, 2, 7 } ,
	{ 4, 2, 7, 2, 2, 2, 7 } ,
	{ 4, 5, 7, 2, 2, 2, 7 } ,
	{ 4, 0, 7, 2, 2, 2, 7 } ,
	{ 6, 14, 18, 18, 19, 18, 14 } ,
	{ 5, 5, 0, 9, 11, 13, 9 } ,
	{ 5, 4, 6, 9, 9, 9, 6 } ,
	{ 5, 4, 6, 9, 9, 9, 6 } ,
	{ 5, 9, 6, 9, 9, 9, 6 } ,
	{ 5, 5, 6, 9, 9, 9, 6 } ,
	{ 5, 0, 6, 9, 9, 9, 6 } ,
	{ 6, 0, 17, 10, 4, 10, 17 } ,
	{ 6, 14, 25, 21, 19, 14, 1 } ,
	{ 5, 2, 9, 9, 9, 9, 6 } ,
	{ 5, 4, 9, 9, 9, 9, 6 } ,
	{ 5, 9, 0, 9, 9, 9, 6 } ,
	{ 5, 0, 9, 9, 9, 9, 6 } ,
	{ 5, 4, 9, 9, 14, 8, 7 } ,
	{ 6, 2, 14, 18, 14, 2, 7 } ,
	{ 6, 6, 5, 15, 17, 15, 1 } ,
	{ 5, 4, 0, 7, 14, 9, 14 } ,
	{ 5, 4, 0, 7, 14, 9, 14 } ,
	{ 5, 9, 0, 7, 14, 9, 14 } ,
	{ 5, 5, 0, 7, 14, 9, 14 } ,
	{ 5, 9, 0, 7, 14, 9, 14 } ,
	{ 5, 5, 2, 7, 14, 9, 14 } ,
	{ 6, 0, 27, 20, 31, 5, 31 } ,
	{ 5, 0, 6, 1, 1, 14, 4 } ,
	{ 5, 4, 0, 6, 15, 1, 6 } ,
	{ 5, 2, 0, 6, 15, 1, 6 } ,
	{ 5, 10, 0, 6, 15, 1, 6 } ,
	{ 5, 10, 0, 6, 15, 1, 6 } ,
	{ 3, 2, 4, 0, 4, 4, 4 } ,
	{ 4, 8, 4, 0, 4, 4, 4 } ,
	{ 5, 4, 10, 0, 4, 4, 4 } ,
	{ 5, 0, 10, 0, 4, 4, 4 } ,
	{ 6, 28, 8, 14, 9, 9, 6 } ,
	{ 5, 7, 0, 7, 9, 9, 9 } ,
	{ 5, 4, 0, 6, 9, 9, 6 } ,
	{ 5, 2, 0, 6, 9, 9, 6 } ,
	{ 5, 10, 0, 6, 9, 9, 6 } ,
	{ 5, 5, 0, 6, 9, 9, 6 } ,
	{ 5, 10, 0, 6, 9, 9, 6 } ,
	{ 4, 0, 2, 0, 7, 0, 2 } ,
	{ 6, 0, 22, 9, 21, 18, 13 } ,
	{ 5, 4, 0, 9, 9, 9, 14 } ,
	{ 5, 2, 0, 9, 9, 9, 14 } ,
	{ 5, 9, 0, 9, 9, 9, 14 } ,
	{ 5, 9, 0, 9, 9, 9, 14 } ,
	{ 5, 2, 0, 9, 9, 14, 8 } ,
	{ 5, 1, 7, 9, 9, 7, 1 } ,
	{ 5, 9, 0, 9, 9, 14, 8 }
};

const static char MatrixVerticalShift[]={ 2, 3, 4, 5, 6, 7, 8, 9, 0, 1};
/* Segment/common map for the seven-segment display area

	Segment names:
		aaa
	f			b
	f			b
		ggg
	e			c
	e			c
		ddd
				dp	 
				
				sc
				
				*/

			 
#define SC(str,c) ((uint8) (((str) - 6) | ((c-2) << 6)))
#define SEGMENT(x) (((x) & 0x3f) + 6)
#define COMMON(x) (((x) >> 6) + 2)
static byte const segcom[17][9] = {
	//	a			b			c			d			e			f			g			dp			sc
	// Number Digits [1 - 12]
	{SC( 7, 2),	SC( 8, 2), 	SC( 7, 4), 	SC( 6, 4), 	SC( 6, 3), 	SC( 6, 2), 	SC( 7, 3), 	SC( 8, 5), 	SC( 7, 5) },
	{SC( 9, 2),	SC(10, 3), 	SC(10, 4), 	SC( 9, 4), 	SC( 8, 4), 	SC( 8, 3), 	SC( 9, 3), 	SC(10, 5), 	SC( 9, 5) },
	{SC(11, 2),	SC(12, 2), 	SC(12, 3), 	SC(11, 5), 	SC(11, 4), 	SC(10, 2), 	SC(11, 3), 	SC(12, 4), 	SC(12, 5) },
	{SC(14, 2), 	SC(14, 3), 	SC(14, 4), 	SC(13, 5), 	SC(13, 4), 	SC(13, 2), 	SC(13, 3), 	SC(15, 4), 	SC(14, 5) },
	{SC(16, 2), 	SC(17, 2), 	SC(16, 4), 	SC(15, 5), 	SC(15, 3), 	SC(15, 2), 	SC(16, 3), 	SC(17, 5), 	SC(16, 5) },
	{SC(18, 2), 	SC(19, 3), 	SC(19, 4), 	SC(18, 4), 	SC(17, 4), 	SC(17, 3), 	SC(18, 3), 	SC(19, 5), 	SC(18, 5) },
	{SC(20, 2), 	SC(21, 2), 	SC(21, 3), 	SC(20, 5), 	SC(20, 4), 	SC(19, 2), 	SC(20, 3), 	SC(21, 4), 	SC(21, 5) },
	{SC(23, 2), 	SC(23, 3), 	SC(23, 4), 	SC(22, 5), 	SC(22, 4), 	SC(22, 2), 	SC(22, 3), 	SC(24, 4), 	SC(23, 5) },
	{SC(25, 2), 	SC(26, 2), 	SC(25, 4), 	SC(24, 5), 	SC(24, 3), 	SC(24, 2), 	SC(25, 3), 	SC(26, 5), 	SC(25, 5) },
	{SC(27, 2), 	SC(28, 3), 	SC(28, 4), 	SC(27, 4), 	SC(26, 4), 	SC(26, 3), 	SC(27, 3), 	SC(28, 5), 	SC(27, 5) },
	{SC(29, 2), 	SC(30, 2), 	SC(30, 3), 	SC(29, 5), 	SC(29, 4), 	SC(28, 2), 	SC(29, 3), 	SC(30, 4), 	SC(30, 5) },
	{SC(32, 2), 	SC(32, 3), 	SC(32, 4), 	SC(31, 5), 	SC(31, 4), 	SC(31, 2), 	SC(31, 3), 	SC(33, 5), 	SC(32, 5) },
	// Exponent digits [13 - 15]															 
	{SC(34, 2), 	SC(34, 3), 	SC(35, 5), 	SC(34, 5), 	SC(33, 4), 	SC(33, 2), 	SC(34, 4), 	SC(27, 5), 	SC(-1, -1) },
	{SC(36, 2), 	SC(36, 3), 	SC(36, 4), 	SC(36, 5), 	SC(35, 4), 	SC(35, 2), 	SC(35, 3), 	SC(30, 5), 	SC(-1, -1) },
	{SC(38, 3), 	SC(38, 4), 	SC(38, 5), 	SC(37, 5), 	SC(37, 4), 	SC(37, 2), 	SC(37, 3), 	SC(32, 5), 	SC(-1, -1) },
	// Number Negative Sign
	{SC(-1,-1),	SC(-1,-1), 	SC(-1,-1), 	SC(-1,-1), 	SC(-1,-1), 	SC(-1,-1), 	SC( 6, 5), 	SC(-1,-1), 	SC(-1,-1) },
	// Exponent Negative Sign
	{SC(-1,-1),	SC(-1,-1), 	SC(-1,-1), 	SC(-1,-1), 	SC(-1,-1), 	SC(-1,-1), 	SC(33, 3), 	SC(-1,-1), 	SC(-1,-1) },
};

static ushort const Segment7Font[] = {
#define segs 0
#define a | 0x1
#define b | 0x2
#define c | 0x4
#define d | 0x8
#define e | 0x10
#define f | 0x20
#define g | 0x40
#define dp | 0x80
#define sc | 0x100
	segs					, // space
	segs b dp				, // !
	segs f b				, // "
	segs e f b c			, // #
	segs a f g c d dp		, // $
	segs b g e dp			, // %
	segs f g e c d dp		, // &
	segs f				, // '
	segs a f e d			, // (
	segs a b c d			, // )
	segs f b g e c dp		, // *
	segs f g e			, // +
	segs sc				, // ,
	segs g				, // -
	segs dp				, // .
	segs b g e			, // /
	segs a b c d e f		, // 0
	segs b c				, // 1
	segs a b g e d			, // 2
	segs a b g c d			, // 3
	segs f g b c			, // 4
	segs a f g c d			, // 5
	segs a f g e c d		, // 6
	segs a b c			, // 7
	segs a b c d e f g		, // 8
	segs a b f g c d		, // 9
	segs f e dp			, // :
	segs dp sc			, // ;
	segs b g c			, // <
	segs g d				, // =
	segs f g e			, // >
	segs a b g e dp		, // ?
	segs a b c d e f g dp	, // @
	segs a e f b c g		, // A
	segs f g e c d			, // b
	segs a f e d			, // C
	segs b g e c d			, // d
	segs a f g e d			, // E
	segs a f g e			, // F
	segs a f e d c g		, // g
	segs f e g b c			, // H
	segs e f				, // I
	segs b c d e			, // J
	segs f b g e c dp		, // K
	segs f e d			, // L
	segs e f a b c			, // m
	segs e g c			, // n
	segs a b c d e f		, // o
	segs f a b g e			, // P
	segs f a b g c dp		, // q
	segs g e				, // r
	segs a f g c d			, // S
	segs f g e d			, // t
	segs f e d c b			, // U
	segs e d c			, // v
	segs e d c a			, // w
	segs f b g e c dp		, // x
	segs f g b c d			, // y
	segs a b g e d dp		, // z
	segs a f e d dp		, // [
	segs f g c			, // backslash
	segs a b c d dp		, // ]
	segs f a b			, // ^
	segs d				, // _
	segs b				, // `
	segs a e f b c g		, // A
	segs f g e c d			, // b
	segs g e d			, // c
	segs b g e c d			, // d
	segs a f g e d			, // E
	segs a f g e			, // F
	segs a f b g c d		, // g
	segs f e g c			, // h
	segs c a				, // i
	segs b c d			, // j
	segs f b g e c dp		, // K
	segs f e				, // l
	segs e f a b c			, // m
	segs e g c			, // n
	segs e g c d			, // o
	segs f a b g e			, // P
	segs f a b g c dp		, // q
	segs g e				, // r
	segs a f g c d			, // S
	segs f g e d			, // t
	segs f e d c b			, // U
	segs e d c			, // v
	segs e d c a			, // w
	segs f b g e c dp		, // x
	segs f g b c d			, // y
	segs a b g e d dp		, // z
	segs b g c dp			, // {
	segs f e dp			, // |
	segs f e g dp			, // }
	segs a g				, // ~
	segs a b c d e f g dp	, // DEL
#undef a
#undef b
#undef c
#undef d
#undef e
#undef f
#undef g
#undef dp
};

// Enable the LCD display: turn on its power supply and set various counters
void HP20b_LCDInit () {
#ifdef ARM
	int memst = 0;
	char* memstrt = 0;
	
	if (HP20b.Contrast == 0)
		HP20b.Contrast = 9;

	while (1) {
		// turn LCD on at selected contrast...
		AT91C_BASE_SUPC->SUPC_MR = (AT91C_BASE_SUPC->SUPC_MR & 0xFFFFFFC0)| (0xA5UL << 24) | AT91C_SUPC_LCDMODE_INTERNAL | (HP20b.Contrast & 0xF);
		if (AT91C_BASE_SUPC->SUPC_SR & AT91C_SUPC_LCDS) break;
		memstrt = (char*)AT91C_SLCDC_MEM;
		// Erase Screen if it was OFF before
		// We can't use memset, because <cstring> is apparently un-allowed, so
		// we'll just walk the data using a hard coded for loop and zero-out eight bits at a time.
		for (memst = 0; memst < (10 * 2 * 4) / 8; memst++, memstrt++)
			(*memstrt) = 0x0000; 
	}

	if (AT91C_BASE_SLCDC -> SLCDC_SR == AT91C_SLCDC_ENA) {
		HP20b.LCD = 1;
		return; // if LCD is already turned ON, quick exit...
	} 
	
	AT91C_BASE_SLCDC -> SLCDC_CR = AT91C_SLCDC_SWRST;
	AT91C_BASE_SLCDC -> SLCDC_MR = HP20b_LCDCOMMON_NUMBER | AT91C_SLCDC_BUFTIME_8_Tsclk | AT91C_SLCDC_BIAS_1_3 | HP20b_LCDSEGMENT_NUMBER;
	AT91C_BASE_SLCDC -> SLCDC_FRR = AT91C_SLCDC_PRESC_SCLK_16 | AT91C_SLCDC_DIV_2;
	AT91C_BASE_SLCDC -> SLCDC_CR = AT91C_SLCDC_LCDEN;
	// Check the status register continually until the LCD is enabled.
	while (AT91C_BASE_SLCDC -> SLCDC_SR != AT91C_SLCDC_ENA);
	
#else

#endif
	HP20b.LCD = 1;
}

// Enable the LCD display: turn on its power supply and set various counters
void HP20b_LCDInitContrast (int contrast) {
#ifdef ARM
	int memst = 0;
	char* memstrt = 0;
	HP20b.LCD = 0;
	HP20b.Contrast = contrast;
	
	while (1) {
		// turn LCD on at selected contrast...
		AT91C_BASE_SUPC->SUPC_MR = (AT91C_BASE_SUPC->SUPC_MR&0xFFFFFFC0)| (0xA5UL << 24) | AT91C_SUPC_LCDMODE_INTERNAL | (HP20b.Contrast & 0xf);
		if (AT91C_BASE_SUPC->SUPC_SR & AT91C_SUPC_LCDS) break;
		memstrt = (char*)AT91C_SLCDC_MEM;
		// Erase Screen if it was OFF before
		// We can't use memset, because <cstring> is apparently un-allowed, so
		// we'll just walk the data using a hard coded for loop and zero-out eight bits at a time.
		for (memst = 0; memst < (10 * 2 * 4) / 8; memst++, memstrt++)
			(*memstrt) = 0x0000;
	}

	if (AT91C_BASE_SLCDC -> SLCDC_SR == AT91C_SLCDC_ENA) {
		HP20b.LCD = 1;
		return; // if LCD is already turned ON, quick exit...
	}
	AT91C_BASE_SLCDC -> SLCDC_CR = AT91C_SLCDC_SWRST;
	AT91C_BASE_SLCDC -> SLCDC_MR = HP20b_LCDCOMMON_NUMBER | AT91C_SLCDC_BUFTIME_8_Tsclk | AT91C_SLCDC_BIAS_1_3 | HP20b_LCDSEGMENT_NUMBER;
	AT91C_BASE_SLCDC -> SLCDC_FRR = AT91C_SLCDC_PRESC_SCLK_16 | AT91C_SLCDC_DIV_2;
	AT91C_BASE_SLCDC -> SLCDC_CR = AT91C_SLCDC_LCDEN;
	//while (pSLCDC -> SLCDC_SR != AT91C_SLCDC_ENA); //TODO: put back...

#else

#endif

	HP20b.LCD = 1;
}

// disable the LCD drivers...
void HP20b_LCDQuit () {

#ifdef ARM

	AT91C_BASE_SLCDC->SLCDC_CR = AT91C_SLCDC_LCDDIS; // BDs
	AT91C_BASE_SUPC->SUPC_MR = (0xA5UL << 24) | (AT91C_BASE_SUPC->SUPC_MR & ~AT91C_SUPC_LCDMODE); // BDs
	while ((AT91C_BASE_SUPC->SUPC_SR & AT91C_SUPC_LCDS) == AT91C_SUPC_LCDS); // BDs

#else

#endif

	HP20b.LCD = 0;
}

void HP20b_LCDS7NegativeSign(bool on, bool exponentdigits) {
#ifdef ARM
	uint32 mask, i;
	uint8 segments, segment, common;
	AT91_REG* common_p;
	uint8 const* segment_column;
	
	segments = Segment7Font[ on ? '-' - ' ' : ' ' - ' '];
	segment_column = segcom[15 + (exponentdigits != 0)];
	
	common = COMMON(*segment_column);
	segment = SEGMENT(*segment_column);
	common_p = AT91C_SLCDC_MEM + (common * 2);
	mask = ((uint32)1) << (segment & 0x1f);

	for (i = 0 ; i < 9; i++) {
		common = COMMON(*segment_column);
		segment = SEGMENT(*segment_column);
		
		common_p = AT91C_SLCDC_MEM + (common * 2);
		mask = ((uint32)1) << (segment & 0x1f);
		if (segment & 0x20)
			common_p++;
		if (segments & 0x1)
			*common_p |= mask;
		else
			*common_p &= ~mask;
	
		++segment_column, segments >>= 1;
	}
	

	
	
#else

#endif
}

void HP20b_LCDS7Char(char c, int column) {
#ifdef ARM
	uint32 i, mask;
	uint8 segments, segment, common;
	AT91_REG* common_p;
	uint8 const* segment_column;

	if (column >= HP20b_LCDSegment7FullMax)
		return;

	if (c >= ' ' && c <= 127)
		segments = Segment7Font[c - ' '];
	else
		segments = 0;

	segment_column = segcom[column];
	for (i = 0 ; i < 9; i++) {
		common = COMMON(*segment_column);
		segment = SEGMENT(*segment_column);
		common_p = AT91C_SLCDC_MEM + (common * 2);
		mask = ((uint32)1) << (segment & 0x1f);
		if (segment & 0x20)
			common_p++;
		if (segments & 0x1)
			*common_p |= mask;
		else
			*common_p &= ~mask;
		++segment_column, segments >>= 1;
	}

#else

#endif

}

// Display an ASCII character in the given column on the 7-segment display
void HP20b_LCDS7CharAnd(char c, int column, int decimalcomma) {
#ifdef ARM
	uint32 i, mask;
	uint8 segments, segment, common;
	AT91_REG* common_p;
	uint8 const* segment_column;
	
	if (column >= HP20b_LCDSegment7FullMax)
		return;

	if (c >= ' ' && c <= 127)
		segments = Segment7Font[c - ' '];
	else
		segments = 0;

	if (decimalcomma == 1)
		segments |= Segment7Font['.' - ' '];
	else if (decimalcomma == 2)
		segments |= Segment7Font[',' - ' '];

	segment_column = segcom[column];
	for (i = 0 ; i < 9; i++) {
		common = COMMON(*segment_column);
		segment = SEGMENT(*segment_column);
		common_p = AT91C_SLCDC_MEM + (common * 2);
		mask = ((uint32)1) << (segment & 0x1f);
		if (segment & 0x20)
			common_p++;
		if (segments & 0x1)
			*common_p |= mask;
		else
			*common_p &= ~mask;
		++segment_column, segments >>= 1;
	}

#else

#endif

}

void HP20b_LCDS7Offset(const char* c, int offset) {
	int column, count = 0;
	char ch = 0, comdec = 0, comcount = 0, lastcount = 0;
	
	if (HP20b.S7AutoClear)
		HP20b_LCDS7Clear();
	
	if (*c == '-') {
		HP20b_LCDS7NegativeSign(true, false);
		++c;
	}

	if (HP20b.S7Alignment) {
		while (*c) {
			if (*c != '.' && *c != ',') {
				++count;
			}
			else {
				if (lastcount == count)
					count++;
				else
					++comcount;
				lastcount = count;
			}
			++c;
		}
		c -= count + comcount;
		comcount = false;
		count = HP20b_LCDSegment7Max - count;

		for (column = offset; column < HP20b_LCDSegment7Max && (ch = *c++); ++column) {
			comdec = *(c);
			if (comdec == '.' && !comcount) {
				HP20b_LCDS7CharAnd(ch, count + column, 1);
				++c;
				comcount = true;
			}
			else if (comdec == ',' && !comcount) {
				HP20b_LCDS7CharAnd(ch, count + column, 2);
				++c;
				comcount = true;
			}
			else {
				HP20b_LCDS7Char(ch, count + column);
				comcount = false;
			}
		}
	}
	else {
		for (column = offset; column < HP20b_LCDSegment7Max && (ch = *c++); ++column) {
			comdec = *(c);
			if (comdec == '.' && !comcount) {
				HP20b_LCDS7CharAnd(ch, column, 1);
				++c;
				comcount = true;
			}
			else if (comdec == ',' && !comcount) {
				HP20b_LCDS7CharAnd(ch, column, 2);
				++c;
				comcount = true;
			}
			else {
				HP20b_LCDS7Char(ch, column);
				comcount = false;
			}
		}
	}		
}

void HP20b_LCDS7(const char* text) {
	HP20b_LCDS7Offset(text, 0);
}

void HP20b_LCDS7CharAligned(char c) {
	HP20b_LCDS7Char(c, HP20b.S7Alignment ? HP20b_LCDSegment7MaxIndex : 0);
}

void HP20b_LCDS7Integer (int integer) {
	IntToString(integer, HP20b.S7);
	HP20b_LCDS7(HP20b.S7);
}

void HP20b_LCDS7UInteger (uint uinteger) {
	UIntToString(uinteger, HP20b.S7);
	HP20b_LCDS7(HP20b.S7);
}

void HP20b_LCDS7Float (float single) {
	FloatToString(single, HP20b.S7);
	HP20b_LCDS7(HP20b.S7);
}

void HP20b_LCDS7Num (Number num) {
	NumToString(num, HP20b.S7);
	HP20b_LCDS7(HP20b.S7);
}

void HP20b_LCDS7Decimal (decimal dec) {
	DecimalToString(dec, HP20b.S7);
	HP20b_LCDS7(HP20b.S7);
}

void HP20b_LCDS7Clear () {
	int column;
	for (column = 0; column < HP20b_LCDSegment7FullMax; ++column)
		HP20b_LCDS7Char(' ', column);
	HP20b_LCDS7NegativeSign(false, false);
	HP20b_LCDS7NegativeSign(false, true);
}

void HP20b_LCDMatrixClear () {
	HP20b_LCDMatrixOffset("      ", 0);
}

void HP20b_LCDClear () {
	HP20b_LCDMatrixClear();
	HP20b_LCDS7Clear();
}

void HP20b_LCDMatrixGraphic(ullong* graphic) {
#ifdef ARM
	uint c = 0;
	int j = 0, i = 0;
	ullong* lcdmemory = 0;

	lcdmemory = (ullong*)AT91C_SLCDC_MEM;
	// These slots of the Matrix LCD are written horizontally
	// AND vertically in the registers

	// The Horizontal Part is 33 spaces long, hence the 33-bit mask 0x7FFFFFFFC0
	// Which is 
	// 111111111111111111111111111111111000000
	// Thus, we shift the graphics data over by 6 bits, which we will use
	// later on for the vertical registers
	lcdmemory[6] = (lcdmemory[6] & ~0x7fffffffc0LL) | ((graphic[0] << 6) & 0x7fffffffc0LL);
	lcdmemory[7] = (lcdmemory[7] & ~0x7fffffffc0LL) | ((graphic[1] << 6) & 0x7fffffffc0LL);
	lcdmemory[8] = (lcdmemory[8] & ~0x7fffffffc0LL) | ((graphic[2] << 6) & 0x7fffffffc0LL);
	lcdmemory[9] = (lcdmemory[9] & ~0x7fffffffc0LL) | ((graphic[3] << 6) & 0x7fffffffc0LL);
	lcdmemory[0] = (lcdmemory[0] & ~0x7fffffffc0LL) | ((graphic[4] << 6) & 0x7fffffffc0LL);
	lcdmemory[1] = (lcdmemory[1] & ~0x7fffffffc0LL) | ((graphic[5] << 6) & 0x7fffffffc0LL);
	
	// Because these registers are vertical, we can account for the last 10
	// columns in the matrix (10 Vertical + 33 Horizontal = 43 Columns),
	for (j = 9; j >= 0; j--) {
		c = 0;
		// We store the lower six bits of the graphic in c
		// for use later.
		for (i = 0; i < 6; i++) { 
			c = ( c << 1 ); 
			if ( ( graphic[i] & ( 1LL << (j + 33) ) ) != 0 ) 
				c++; 
		}
		// Here, we use the VerticalShift to get our data in the
		// right position from the start of the AT91 Registry Slot
		// We bitwise-and by the negation of 0x3f,
		// that is we preserve the graphical data from
		// the horizontals perfectly (while simultaneously
		// condemning all bits in position < 5) and then we take the original
		// graphical data, stored in "c", and bitwise OR it to
		// get our final register data.
		lcdmemory[(int)MatrixVerticalShift[j]] =
			(lcdmemory[(int)MatrixVerticalShift[j]] & ~(0x3fLL)) | (ullong)c;
	}

#else
	
#endif
}

int HP20b_LCDMatrixOffset(const char* text, int offset) {
	const byte* a;
	const char* ptext = text;
	int /*linecount= 0, */s = 0, rowcount;
	ullong matrix[6] = {0, 0, 0, 0, 0, 0};
	
	for (;;) {
		s = MatrixFont[ ( (*ptext) < 32 ? 32 : ( *ptext) - 32) ][0];
		if (offset - s < 0)
			break;
		
		if (*ptext != 0) {
			ptext++;
		}
		/*else if (loop) {
			ptext = text;
			++linecount;
		}*/
		else {
			HP20b_LCDMatrixGraphic(matrix);
			return 0;
		} 
		offset -= s;
		if (offset == 0)
			break;
	}

	offset = -offset;
	while (offset <= 43) {
		a = &MatrixFont[ *ptext < 32 ? 0 : (*ptext) - 32 ][1];
		if (offset < 0)
			for (rowcount = 0; rowcount < 6; rowcount++)
				matrix[rowcount] |= ((ullong)(*a++)) >> (-offset);
		else
			for (rowcount = 0; rowcount < 6; rowcount++)
				matrix[rowcount] |= ((ullong)(*a++)) << offset;
		offset += a[-7];
		/*if (loop) {
			if (*ptext== 0)
				ptext = msg;
			else ptext++;
		}
		else {*/
			if (*ptext == 0)
				break;
			ptext++;
			if (*ptext == 0)
				break;
		//}
	}
	HP20b_LCDMatrixGraphic(matrix);
	/*if (loop)
		return linecount;
	else */if (*ptext == 0)
		return offset - 1;
	else
		return 44;
}

int HP20b_LCDMatrix (const char* text) {
	return HP20b_LCDMatrixOffset(text, 0);
}

void HP20b_LCDMatrixCharAligned(char c) {
	cbuffer[0] = c;
	cbuffer[1] = '\0';
	HP20b_LCDMatrixOffset(cbuffer, 0);
}

void HP20b_LCDMatrixInteger (int integer) {
	IntToString(integer, HP20b.Matrix);
	HP20b_LCDMatrix(HP20b.Matrix);
}

void HP20b_LCDMatrixUInteger (uint uinteger) {
	UIntToString(uinteger, HP20b.Matrix);
	HP20b_LCDMatrix(HP20b.Matrix);
}

void HP20b_LCDMatrixFloat (float single) {
	FloatToString(single, HP20b.Matrix);
	HP20b_LCDMatrix(HP20b.Matrix);
}

void HP20b_LCDMatrixNum (Number num) {
	NumToString(num, HP20b.Matrix);
	HP20b_LCDMatrix(HP20b.Matrix);
}

void HP20b_LCDMatrixDecimal (decimal dec) {
	DecimalToString(dec, HP20b.Matrix);
	HP20b_LCDMatrix(HP20b.Matrix);
}

void HP20b_LCDIndicatorSet( HP20bIndicators indicatorid, int on) {
#ifdef ARM
	ullong* lcdmemory = (ullong*)(AT91C_SLCDC_MEM) + (indicatorid & 15);
	ullong mask = 0x1LL << ( indicatorid >> 4 );
	if (on)
		*lcdmemory |= mask;
	else
		*lcdmemory &= ~mask;

#else

#endif
}

bool HP20b_LCDIndicatorIsOn ( HP20bIndicators indicatorid ) {
#ifdef ARM
	ullong* lcdmemory = (ullong*)(AT91C_SLCDC_MEM) + (indicatorid & 15);
	return (uint)( ( *lcdmemory >> ( indicatorid >> 4) ) & 1) != 0;
#else
	return false;
#endif
}

bool HP20b_LCDIndicatorToggle ( HP20bIndicators indicatorid ) {
#ifdef ARM
	ullong* lcdmemory = (ullong*)(AT91C_SLCDC_MEM) + (indicatorid & 15);
	ullong mask = 0x1LL << ( indicatorid >> 4);
	bool on = !(HP20b_LCDIndicatorIsOn(indicatorid));
	if (on)
		*lcdmemory |= mask;
	else
		*lcdmemory &= ~mask;

	return on;
#else
	return false;
#endif
}

void HP20b_LCDIndicatorClear () {
	HP20b_LCDIndicatorSet(SmallEqualsIndicator, false);
	HP20b_LCDIndicatorSet(BigEqualsIndicator, false);
	HP20b_LCDIndicatorSet(PostfixIndicator, false);
	HP20b_LCDIndicatorSet(BeginModeIndicator, false);
	HP20b_LCDIndicatorSet(MemoryStoreIndicator, false);
	HP20b_LCDIndicatorSet(RadiansIndicator, false);
	HP20b_LCDIndicatorSet(DegreesIndicator, false);
	HP20b_LCDIndicatorSet(RecallIndicator, false);
	HP20b_LCDIndicatorSet(InputIndicator, false);
	HP20b_LCDIndicatorSet(BatteryIndicator, false);
	HP20b_LCDIndicatorSet(DownArrowIndicator, false);
}
