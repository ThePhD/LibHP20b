#include "HP20b.h"

void HP20b_Init (int systems) {
	int i;
	// The CPU Speed is measured by 16 ticks of the rising edge of the Slow Clock of the AT91,
	// Which is a 2 MhZ Clock. Thus, the actual CPUSpeed is a measure of the Slow Clock
	// multiplied by 16, which puts its speed in terms of the Slow Clock's Speed,
	// Multiplied by 2 to achieve the number of MhZ of the CPU Clock
	// This is only really useful for the "Wait" function (down below)
	HP20b_CPUSpeed();
	HP20b.Status = 1;
	//HP20b.Mode = InfixEntry;
	//HP20b.Entering = true;
	//HP20b.Postfix = false;
	//HP20b.AfterNum = false;
	//HP20b.Shifted = false;
	HP20b.Settings = 0xB9;
	// B = 0111 binary, which turns on AutoClear and sets LCd and Matrix alignment to right
	// 9 = 1001 binary, which is for the Contrast Setting on the calculator
	//HP20b.AutoConsumeShift = true;
	//HP20b.S7AutoClear = true;
	//HP20b.S7Alignment = AlignRight;
	//HP20b.MatrixAlignment = AlignRight;
	//HP20b.Contrast = 9;
	HP20b.Systems = 1U;
	HP20b.S7[0] = '\0';
	HP20b.Matrix[0] = '\0';
	
	HP20b.ResultStackIndex = 0;
	HP20b.OpStackIndex = 0;
	for (i = 0; i < NUMBERSTACKMAX; i++) {
		HP20b.OpStack[i] = 0;
		NumZero(&HP20b.ResultStack[i]);
	}
	
	if ((systems & HP20b_LCDSystem) == HP20b_LCDSystem) {
		HP20b_LCDInit();
		if (HP20b.LCD) {
			HP20b_LCDClear();
			HP20b_LCDIndicatorClear();
		}		
	}
	if ((systems & HP20b_KeyboardSystem) == HP20b_KeyboardSystem) {
		HP20b_KeyboardInit();
	}
	if ((systems & HP20b_WatchdogSystem) == 0) {
#ifdef ARM
		// Disable the Watchdog Timer for this session
		*AT91C_WDTC_WDMR = AT91C_WDTC_WDDIS;
#else
		
#endif
		HP20b.Watchdog = false;
	}
	else {
#ifdef ARM
		AT91C_BASE_WDTC->WDTC_WDCR= 0xA5000001;
#else

#endif
		HP20b.Watchdog = true;
	}

}

void HP20b_Quit (void) {
	if (HP20b.LCD) {
		HP20b_LCDQuit();
	}
	if (HP20b.Keyboard) {
		HP20b_KeyboardQuit();
	}
	if (HP20b.Watchdog) {
#ifdef ARM
		*AT91C_WDTC_WDMR = AT91C_WDTC_WDDIS;
#else

#endif	
		HP20b.Watchdog = false;
	}
	HP20b.Systems = 0;
}

void HP20b_QuitSubsystem (int systems) {
	if ((systems & HP20b_LCDSystem) == HP20b_LCDSystem && HP20b.LCD) {
		HP20b_LCDQuit();
	}
	if ((systems & HP20b_KeyboardSystem) == HP20b_KeyboardSystem && HP20b.Keyboard) {
		HP20b_KeyboardQuit();
	}
	if ((systems & HP20b_WatchdogSystem) == 1 && HP20b.Watchdog) {
#ifdef ARM
		*AT91C_WDTC_WDMR = AT91C_WDTC_WDDIS;
#else

#endif		
	}
}

void HP20b_S7Diplay (void) {
	if (HP20b.LCD)
		HP20b_LCDS7(HP20b.S7);
}

void HP20b_MatrixDisplay (void) {
	if (HP20b.LCD)
		HP20b_LCDMatrix(HP20b.Matrix);
}

bool HP20b_DisplayNum (Number* num) {
	NumToString(*num, HP20b.S7);
	if (HP20b.S7[0] == 'D' ||
	    HP20b.S7[0] == 'u' ||
	    HP20b.S7[0] == 'o' ||
	    HP20b.S7[0] == 'N' ) {
		HP20b_LCDS7("Error");
		HP20b_LCDMatrix(HP20b.S7);
		NumZero(num);
		return false;
	}
	else {
		HP20b_LCDS7(HP20b.S7);
		return true;
	}
}

void HP20b_Wait(ulong milliseconds) {
#ifdef ARM
	ulong ms = 0, max = milliseconds * (HP20b.CPUSpeed / 256);
	for (; ms < max;)
		++ms;
	// The reason the '++ms'
	// is in the for loop is to ensure the clock doesn't pin at max CPU like some computers do in empty loops
#else
	
#endif
}

int HP20b_CPUSpeed () {
#ifdef ARM
	HP20b.CPUSpeed = (int)( *((ushort*)AT91C_CKGR_MCFR) ) * 16 * 2;
#else 
	HP20b.CPUSpeed = 30000;
#endif
	return HP20b.CPUSpeed;
}

Number* HP20b_CurrentNum (void) {
	return &HP20b.ResultStack[HP20b.ResultStackIndex];
}

Number* HP20b_LastNum (void) {
	if (HP20b.ResultStackIndex > 0)
		return &HP20b.ResultStack[HP20b.ResultStackIndex - 1];
	else
		return &HP20b.Result;
}

Number* HP20b_Reset (void) {
	int i = HP20b.ResultStackIndex;
	
	for (; i > -1; --i) {
		NumZero(&HP20b.ResultStack[i]);
	}
	for (i = HP20b.OpStackIndex; i > -1; --i) {
		HP20b.OpStack[i] = 0;
	}

	HP20b.ResultStackIndex = 0;
	HP20b.OpStackIndex = 0;

	HP20b_ClearNum();
	
	if (HP20b.LCD) {
		HP20b_LCDIndicatorSet(PostfixIndicator, false);
		HP20b_LCDIndicatorSet(DegreesIndicator, true);
		HP20b_LCDIndicatorSet(InputIndicator, true);
	}

	return &HP20b.ResultStack[0];
}

Number* HP20b_ClearNum (void) {
	if (HP20b.LCD) {
		if (!HP20b.S7AutoClear)
			HP20b_LCDClear();
		HP20b_LCDS7("0");
	}
	
	NumZero(&HP20b.ResultStack[HP20b.ResultStackIndex]);
	HP20b.AfterNum = false;

	return &HP20b.ResultStack[HP20b.ResultStackIndex];
}
