#ifndef LIBHP20BCORE_H
#define LIBHP20BCORE_H

#include "Standard.h"
#include "AT91SAM7L128.h"

typedef enum HP20bEntryMode {
	InfixDisplay,
	InfixEntry,
	PostfixDisplay,
	PostfixEntry,
} HP20bEntryModes;

typedef enum HP20bSubsystem {
	HP20b_WatchdogSystem = 0x01,
	HP20b_LCDSystem = 0x02,
	HP20b_KeyboardSystem = 0x04,
	HP20b_AllSystemsNoWatchdog = HP20b_LCDSystem | HP20b_KeyboardSystem,
	HP20b_AllSystems = HP20b_LCDSystem | HP20b_KeyboardSystem | HP20b_WatchdogSystem,
} HP20bSubsystems;

typedef enum HP20bAlignment {
	AlignLeft,
	AlignRight
} HP20bAlignments;

#define IndicatorSegCom(bit,seg) (bit << 4) + seg
typedef enum HP20bIndicator {
	BatteryIndicator = IndicatorSegCom(39,7),
	BigEqualsIndicator = IndicatorSegCom(39,1),
	SmallEqualsIndicator = IndicatorSegCom(39,8),
	BeginModeIndicator = IndicatorSegCom(39,6),
	MemoryStoreIndicator = IndicatorSegCom(39,5),
	RecallIndicator = IndicatorSegCom(39,4),
	RadiansIndicator = IndicatorSegCom(39,3),
	DegreesIndicator = IndicatorSegCom(39,2),
	PostfixIndicator = IndicatorSegCom(38,2),
	DownArrowIndicator = IndicatorSegCom(39,0),
	InputIndicator = IndicatorSegCom(39,9)
} HP20bIndicators;
#undef IndicatorSegCom

typedef enum HP20bKey {
	N, IYR, PV, PMT, FV, Amort,
	CshFl, IRR, NPV, Bond, Percent, RCL,
	Input, OpenParenthesis, CloseParenthesis, Negation, Backspace, BR5C2,
	Up, N7, N8, N9, Divide, BR5C3,
	Down, N4, N5, N6, Multiply, BR5C4,
	Shift, N1, N2, N3, Minus, BR5C5,
	OnClearSurrogate, BR1C6, N0, DecimalPoint, Equals, Plus,
	
	OnClear,

	xPYR, IConv, Beg, PYR, End, Depr,
	Data, Stats, BrkEv, Date, PercentCalc, STO,
	Memory, Mode, ShiftedBR2C2, EEX, Reset, ShiftedBR5C2,
	INS, SIN, COS, TAN, Math, ShiftedBR5C3,
	DEL, LN, eToThex, xSquared, Squareroot, ShiftedBR5C4,
	UnShift, Random, Factorial, yToThex, Inversex, ShiftedBR5C5,
	OffSurrogate, ShiftedBR1C6, nPr, nCr, ANS, Round,
	
	Off
	
} HP20bKeys;

#define NUMBERSTACKMAX 25
#define S7Size 15
#define MatrixSize 20

struct HP20bCalculator {
	uint CPUSpeed;
	union {
		uint Systems;
		struct {
			bool Watchdog:1;
			bool LCD:1;
			bool Keyboard:1;
		};
	};
	
	union {
		byte Status;
		union {
			struct {
				bool Entering:1;
				bool Postfix:1;
				bool AfterNum:1;
				bool Shifted:1;
				byte DelayedOperation:4;
			};
		};
	};

	union {
		byte Settings;
		struct {
			byte Contrast:4;
			bool AutoConsumeShift:1;
			bool S7AutoClear:1;
			bool S7Alignment:1;
			bool MatrixAlignment:1;
		};
	};
	int ResultStackIndex;
	int OpStackIndex;
	union {
		char Op;
		char OpStack[NUMBERSTACKMAX];
	};
	union {
		Number Result;
		Number ResultStack[NUMBERSTACKMAX];
	};
	char S7[S7Size];
	char Matrix[MatrixSize];
} HP20b;

#endif /* LIBHP20BCORE_H */

