#include "HP20b.h"

#define LAB1 1

int main() {
#ifdef LAB1 
	int integer = 0;
	Number num;

	HP20b_Init( HP20b_LCDSystem );
	StringToNum("0", &num);

	HP20b_LCDS7Integer(integer);
	
	for (;;) {
		HP20b_LCDS7Integer(integer);
		HP20b_Wait(480);
		++integer;
		HP20b_Wait(400);
		HP20b_LCDS7Num(num);
		NumSIncrement(&num);
	}

#elif LAB2 
	int key = -1;

	// Initialize both the LCD and the Keyboard System, and turn off the
	// Watchdog Timer
	// This is the same as HP20b_Init ( HP_20bLCDSystem | HP20b_KeyboardSystem )
	HP20b_Init( HP20b_AllSystemsNoWatchdog );
	
	// Display text on the LCD Matrix
	HP20b_LCDMatrix("PokeMe~");
	
	for (;;) {
		// Update the Whole Keyboard and Store Key Information
		HP20b_KeyboardUpdate();

		// Get the first available down key on the key array
		key = HP20b_FirstKey();
		if (HP20b_FirstKeyIsUnique()) {
			if (HP20b_KeyPressed(key)) {
				if (key == Shift) {
					HP20b_KeyNameShifted(key, HP20b.Shifted, HP20b.Matrix);
					HP20b.Shifted = !HP20b.Shifted;
				}
				else {
					HP20b_KeyNameShifted(key, HP20b.Shifted, HP20b.Matrix);
				}
				HP20b_LCDMatrix(HP20b.Matrix);
				HP20b_LCDS7Integer(HP20b_KeyCode(key));
			}
			else {
				HP20b_LCDMatrix("Again...~");
				HP20b_LCDS7Clear();
			}
		}
	}

#elif LAB3 
	int key = -1, keycode = 0;
	
	// Initialize both the LCD and the Keyboard System, and turn off the
	// Watchdog Timer
	// This is the same as HP20b_Init ( HP_20bLCDSystem | HP20b_KeyboardSystem )
	HP20b_Init( HP20b_AllSystemsNoWatchdog );
	HP20b.S7Alignment = AlignLeft;
	// Display text on the LCD Matrix
	HP20b_LCDMatrix("Make #s");
	HP20b_LCDS7Num(HP20b.Result);
	
	for (;;) {
		// Update the Whole Keyboard and Store Key Information
		HP20b_KeyboardUpdate();
		
		// Get the first available down key on the key array
		key = HP20b_FirstKey();
		if (HP20b_FirstKeyIsUnique()) {
			if (HP20b_KeyPressed(key)) {
				keycode = HP20b_KeyCode(key);
				if (keycode >= '0' && keycode <= '9') {
					if (!HP20b.Result.MantissaHighest.High) {
						NumSAddDigit(&HP20b.Result, keycode & 0xF, HP20b.AfterNum);
						HP20b_LCDMatrix("Number!");
					}
					else {
						HP20b_LCDMatrix("Full :(");
					}
				}
				else if (keycode == '.') {
					// We are now increasing the exponent as we go along
					if (!HP20b.AfterNum) {
						HP20b_LCDMatrix("Decimal!");
						HP20b.AfterNum = true;
					}
					else {
						HP20b_LCDMatrix("Meow.");
					}
				}
				else if (keycode == '\b') {
					// Erase the last number, and the decimal point if necessary
					NumSRightDigitShift(&HP20b.Result, HP20b.AfterNum);
					if (HP20b.Result.Exponent == 0) {
						HP20b.AfterNum = false;
					}
					if (NumIsNegative(&HP20b.Result) && NumIs0(&HP20b.Result)) {
						NumNegate(&HP20b.Result);
					}
					HP20b_LCDMatrix("B-space");
				}
				else if (keycode == '~') {
					NumNegate(&HP20b.Result);
				}
				else if (keycode == 'S') {
					HP20b_LCDMatrix("Ooh...");
					if (HP20b_LiveKeyDown(Inversex)) {
						HP20b_LCDMatrix("Inverse");
						NumInverseInto(&HP20b.Result, &HP20b.Result);	
					}
				}
				else if (keycode == 'O') {
					// Reset the number
					NumZero(&HP20b.Result);
					HP20b_LCDMatrix("Cleared!");
				}
				else {
					HP20b_LCDMatrix("Ignored!");
				}
				HP20b_LCDS7Num(HP20b.Result);
			}
		}
	}	

#elif LAB4 
	int key = -1, keycode = 0, lastkeycode = 0, count = 0;
	
	HP20b_Init( HP20b_AllSystemsNoWatchdog );
	HP20b_DisplayNum(&HP20b.Result);
	HP20b.Postfix = true;
	HP20b_LCDIndicatorSet(PostfixIndicator, true);
	HP20b_LCDIndicatorSet(DegreesIndicator, true);
	HP20b_LCDIndicatorSet(InputIndicator, true);
		
	for (;;) {
		// Update the Whole Keyboard and Store Key Information
		HP20b_KeyboardUpdate();
		
		// Get the first available down key on the key array
		if (HP20b_FirstKeyIsUnique()) {
			key = HP20b_FirstKey();
			if (HP20b_KeyPressed(key)) {
				keycode = HP20b_KeyCode(key);
				if (HP20b_LCDIndicatorIsOn(SmallEqualsIndicator)) {
					HP20b_LCDIndicatorSet(SmallEqualsIndicator, false);	
					HP20b_LCDIndicatorSet(InputIndicator, true);
				}
				if (HP20b.Postfix) {
					if (HP20b.Shifted) {
						switch (keycode) {
							case 'S':
								lastkeycode = keycode;
								break;
							case '(':
								if (HP20b_LCDIndicatorIsOn(PostfixIndicator)) {
									HP20b_LCDIndicatorSet(PostfixIndicator, false);
									HP20b.Postfix = false;
									HP20b_LCDMatrix("Infix Mode");
								}
								else {
									HP20b_LCDIndicatorSet(PostfixIndicator, true);
									HP20b.Postfix = true;
									HP20b_LCDMatrix("RPN Mode");
								}
								lastkeycode = keycode;
								break;
							case '6':
								HP20b_LCDMatrix("x ^ 2");
								NumMultiplyInto(HP20b_CurrentNum(), HP20b_CurrentNum(), HP20b_CurrentNum());
								HP20b_DisplayNum(HP20b_CurrentNum());
								lastkeycode = keycode;
								break;
							case '-':
								HP20b_LCDMatrix("1 / x");
								NumInverseInto(HP20b_CurrentNum(), HP20b_CurrentNum());
								HP20b_DisplayNum(HP20b_CurrentNum());
								lastkeycode = keycode;
								break;
							case '\b':
								// Reset
								HP20b_LCDMatrix("Reset");
								HP20b_DisplayNum(HP20b_Reset());
								lastkeycode = keycode;
								break;
							case 'O':
								// Turn off the calculator
							
								lastkeycode = keycode;
								break;
						}
						HP20b.Shifted = false;
						HP20b_LCDIndicatorSet(DownArrowIndicator, false);
					}
					else {
						switch (keycode) {
							case 'S':
								HP20b.Shifted = true;
								HP20b_LCDIndicatorSet(DownArrowIndicator, true);
								lastkeycode = keycode;
								break;
							case '0':
							case '1':
							case '2':
							case '3':
							case '4':
							case '5':
							case '6':
							case '7':
							case '8':
							case '9':
								HP20b_LCDMatrixClear();
								if (!HP20b_CurrentNum()->MantissaHighest.High) {
									NumSAddDigit(HP20b_CurrentNum(), keycode & 0xF, HP20b.AfterNum);
								}
								HP20b_DisplayNum(HP20b_CurrentNum());
								count = IntToString(HP20b.ResultStackIndex, HP20b.Matrix);
								*(HP20b.Matrix + count) = ' ';
								*(HP20b.Matrix + count + 1) = 'U';
								*(HP20b.Matrix + count + 2) = 's';
								*(HP20b.Matrix + count + 3) = 'e';
								*(HP20b.Matrix + count + 4) = 'd';
								*(HP20b.Matrix + count + 5) = '\0';
								HP20b_MatrixDisplay();
								
								lastkeycode = keycode;
								break;
							case '.':
								HP20b_LCDMatrixClear();
								// We are now increasing the exponent as we go along
								if (!HP20b.AfterNum) {
									HP20b.AfterNum = true;
								}
								lastkeycode = keycode;
								break;
							case '\b':
								NumSRightDigitShift(HP20b_CurrentNum(), HP20b.AfterNum);
								HP20b_DisplayNum(HP20b_CurrentNum());
								count = IntToString(HP20b.ResultStackIndex, HP20b.Matrix);
								*(HP20b.Matrix + count) = ' ';
								*(HP20b.Matrix + count + 1) = 'U';
								*(HP20b.Matrix + count + 2) = 's';
								*(HP20b.Matrix + count + 3) = 'e';
								*(HP20b.Matrix + count + 4) = 'd';
								*(HP20b.Matrix + count + 5) = '\0';
								HP20b_MatrixDisplay();

								lastkeycode = keycode;
								break;
							case '~':
								NumNegate(HP20b_CurrentNum());

								count = IntToString(HP20b.ResultStackIndex, HP20b.Matrix);
								*(HP20b.Matrix + count) = ' ';
								*(HP20b.Matrix + count + 1) = 'U';
								*(HP20b.Matrix + count + 2) = 's';
								*(HP20b.Matrix + count + 3) = 'e';
								*(HP20b.Matrix + count + 4) = 'd';
								*(HP20b.Matrix + count + 5) = '\0';
								HP20b_MatrixDisplay();
								
								lastkeycode = keycode;
								break;
							case '+':
								if (HP20b.ResultStackIndex > 0) {
									if (CharIsOperator(lastkeycode) && NumIs0(HP20b_CurrentNum()) && HP20b.ResultStackIndex > 1)
										--HP20b.ResultStackIndex;
									NumAddInto(&HP20b.ResultStack[HP20b.ResultStackIndex - 1],
										&HP20b.ResultStack[HP20b.ResultStackIndex],
										&HP20b.ResultStack[HP20b.ResultStackIndex - 1]);
									HP20b_ClearNum();
								}
								if (!HP20b_DisplayNum(HP20b_LastNum()) && HP20b.ResultStackIndex > 0) {
									--HP20b.ResultStackIndex;
								}
								lastkeycode = keycode;
								break;
							case '-':
								if (HP20b.ResultStackIndex > 0) {
									if (CharIsOperator(lastkeycode) && NumIs0(HP20b_CurrentNum()) && HP20b.ResultStackIndex > 1)
										--HP20b.ResultStackIndex;
									NumSubtractInto(&HP20b.ResultStack[HP20b.ResultStackIndex - 1],
										&HP20b.ResultStack[HP20b.ResultStackIndex],
										&HP20b.ResultStack[HP20b.ResultStackIndex - 1]);
									HP20b_ClearNum();
								}
								if (!HP20b_DisplayNum(HP20b_LastNum()) && HP20b.ResultStackIndex > 0) {
									--HP20b.ResultStackIndex;
								}
								lastkeycode = keycode;
								break;
							case '*':
								if (HP20b.ResultStackIndex > 0) {
									if (CharIsOperator(lastkeycode) && NumIs0(HP20b_CurrentNum()) && HP20b.ResultStackIndex > 1)
										--HP20b.ResultStackIndex;
									NumMultiplyInto(&HP20b.ResultStack[HP20b.ResultStackIndex - 1],
										&HP20b.ResultStack[HP20b.ResultStackIndex],
										&HP20b.ResultStack[HP20b.ResultStackIndex - 1]);
									HP20b_ClearNum();
								}
								if (!HP20b_DisplayNum(HP20b_LastNum()) && HP20b.ResultStackIndex > 0) {
									--HP20b.ResultStackIndex;
								}
								lastkeycode = keycode;
								break;
							case '/':
								if (HP20b.ResultStackIndex > 0) {
									if (CharIsOperator(lastkeycode) && NumIs0(HP20b_CurrentNum()) && HP20b.ResultStackIndex > 1)
										--HP20b.ResultStackIndex;
									NumDivideInto(&HP20b.ResultStack[HP20b.ResultStackIndex - 1],
										&HP20b.ResultStack[HP20b.ResultStackIndex],
										&HP20b.ResultStack[HP20b.ResultStackIndex - 1]);
									HP20b_ClearNum();
								}
								if (!HP20b_DisplayNum(HP20b_LastNum()) && HP20b.ResultStackIndex > 0) {
									--HP20b.ResultStackIndex;
								}
								lastkeycode = keycode;
								break;
							case '\r':
								++HP20b.ResultStackIndex;
								if (HP20b_DisplayNum(HP20b_LastNum())) {
									HP20b_LCDIndicatorSet(SmallEqualsIndicator, true);
									HP20b_LCDIndicatorSet(InputIndicator, false);
								}
								count = IntToString(HP20b.ResultStackIndex, HP20b.Matrix);
								*(HP20b.Matrix + count) = ' ';
								*(HP20b.Matrix + count + 1) = 'U';
								*(HP20b.Matrix + count + 2) = 's';
								*(HP20b.Matrix + count + 3) = 'e';
								*(HP20b.Matrix + count + 4) = 'd';
								*(HP20b.Matrix + count + 5) = '\0';
								HP20b_MatrixDisplay();
								lastkeycode = keycode;
								break;
							case '\n':
								if (HP20b.ResultStackIndex > 0) {
									HP20b_DisplayNum(HP20b_LastNum());
									count = IntToString(HP20b.ResultStackIndex, HP20b.Matrix);
									*(HP20b.Matrix + count) = ' ';
									*(HP20b.Matrix + count + 1) = 'U';
									*(HP20b.Matrix + count + 2) = 's';
									*(HP20b.Matrix + count + 3) = 'e';
									*(HP20b.Matrix + count + 4) = 'd';
									*(HP20b.Matrix + count + 5) = '\0';
									HP20b_MatrixDisplay();
								}
								lastkeycode = keycode;
								break;
							case '\v':
								HP20b_DisplayNum(HP20b_CurrentNum());
								count = IntToString(HP20b.ResultStackIndex, HP20b.Matrix);
								*(HP20b.Matrix + count) = ' ';
								*(HP20b.Matrix + count + 1) = 'U';
								*(HP20b.Matrix + count + 2) = 's';
								*(HP20b.Matrix + count + 3) = 'e';
								*(HP20b.Matrix + count + 4) = 'd';
								*(HP20b.Matrix + count + 5) = '\0';
								HP20b_MatrixDisplay();
								lastkeycode = keycode;
								break;
							case 'O':
								// Reset the number
								HP20b_ClearNum();
								HP20b_DisplayNum(HP20b_CurrentNum());
								count = IntToString(HP20b.ResultStackIndex, HP20b.Matrix);
								*(HP20b.Matrix + count) = ' ';
								*(HP20b.Matrix + count + 1) = 'U';
								*(HP20b.Matrix + count + 2) = 's';
								*(HP20b.Matrix + count + 3) = 'e';
								*(HP20b.Matrix + count + 4) = 'd';
								*(HP20b.Matrix + count + 5) = '\0';
								HP20b_MatrixDisplay();
								lastkeycode = keycode;
								break;
						}
					}
				}
				else {
					if (HP20b.Shifted) {
						switch (keycode) {
						case 'S':
							lastkeycode = keycode;
							break;
						case '(':
							HP20b_LCDS7Clear();
							if (HP20b_LCDIndicatorIsOn(PostfixIndicator)) {
								HP20b_LCDIndicatorSet(PostfixIndicator, false);
								HP20b.Postfix = false;
								HP20b_LCDMatrix("Infix !");
							}
							else {
								HP20b_LCDIndicatorSet(PostfixIndicator, true);
								HP20b.Postfix = true;
								HP20b_LCDMatrix("RPN !");
							}
							lastkeycode = keycode;
							break;
						case 'O':
							// Turn off the calculator

							lastkeycode = keycode;
							break;
						}
						HP20b.Shifted = false;
						HP20b_LCDIndicatorSet(DownArrowIndicator, false);
					}
					else {
						switch (keycode) {
						case 'S':
							HP20b.Shifted = true;
							HP20b_LCDIndicatorSet(DownArrowIndicator, true);
							lastkeycode = keycode;
							break;
						default:
							HP20b_LCDMatrix("Switch");
							HP20b_LCDS7("to RPN");
							break;
						}
					}
				}
			}
		}
	}	

#else 
	char display[15];
	Number test1, test2, test3;
	
	HP20b_Init( HP20b_AllSystemsNoWatchdog );
	HP20b.S7Alignment = AlignLeft;

	StringToNum("50", &test1);
	StringToNum("50", &test2);
	StringToNum("50", &test3);
	NumMultiplyInto(&test3, &test3, &test3);
	NumToString(test3, display);


	StringToNum(".025", &test1);
	StringToNum(".075", &test2);
	test3 = NumAdd(&test1, &test2);
	NumToString(test3, display);
	test3 = NumSubtract(&test1, &test2);
	NumToString(test3, display);
	test3 = NumMultiply(&test1, &test2);
	NumToString(test3, display);
	test3 = NumDivide(&test1, &test2);
	NumToString(test3, display);

	StringToNum("1", &test1);
	StringToNum("1", &test2);
	test3 = NumAdd(&test1, &test2);
	NumToString(test3, display);
	test3 = NumSubtract(&test1, &test2);
	NumToString(test3, display);
	test3 = NumMultiply(&test1, &test2);
	NumToString(test3, display);
	test3 = NumDivide(&test1, &test2);
	NumToString(test3, display);

	StringToNum("2", &test1);
	StringToNum("2", &test2);
	test3 = NumAdd(&test1, &test2);
	NumToString(test3, display);
	test3 = NumSubtract(&test1, &test2);
	NumToString(test3, display);
	test3 = NumMultiply(&test1, &test2);
	NumToString(test3, display);
	test3 = NumDivide(&test1, &test2);
	NumToString(test3, display);

	StringToNum("1", &test1);
	StringToNum("-1", &test2);
	test3 = NumAdd(&test1, &test2);
	NumToString(test3, display);
	test3 = NumSubtract(&test1, &test2);
	NumToString(test3, display);
	test3 = NumMultiply(&test1, &test2);
	NumToString(test3, display);
	test3 = NumDivide(&test1, &test2);
	NumToString(test3, display);

	StringToNum("0.1", &test1);
	StringToNum("-0.1", &test2);
	test3 = NumAdd(&test1, &test2);
	NumToString(test3, display);
	test3 = NumSubtract(&test1, &test2);
	NumToString(test3, display);
	test3 = NumMultiply(&test1, &test2);
	NumToString(test3, display);
	test3 = NumDivide(&test1, &test2);
	NumToString(test3, display);

	StringToNum("0.1", &test1);
	StringToNum("-1", &test2);
	test3 = NumAdd(&test1, &test2);
	NumToString(test3, display);
	test3 = NumSubtract(&test1, &test2);
	NumToString(test3, display);
	test3 = NumMultiply(&test1, &test2);
	NumToString(test3, display);
	test3 = NumDivide(&test1, &test2);
	NumToString(test3, display);

	StringToNum("5", &test1);
	StringToNum("3", &test2);
	test3 = NumAdd(&test1, &test2);
	NumToString(test3, display);
	test3 = NumSubtract(&test1, &test2);
	NumToString(test3, display);
	test3 = NumMultiply(&test1, &test2);
	NumToString(test3, display);
	test3 = NumDivide(&test1, &test2);
	NumToString(test3, display);

	StringToNum("5", &test1);
	StringToNum("-3", &test2);
	test3 = NumAdd(&test1, &test2);
	NumToString(test3, display);
	test3 = NumSubtract(&test1, &test2);
	NumToString(test3, display);
	test3 = NumMultiply(&test1, &test2);
	NumToString(test3, display);
	test3 = NumDivide(&test1, &test2);
	NumToString(test3, display);

	StringToNum("-0.1", &test1);
	StringToNum("1", &test2);
	test3 = NumAdd(&test1, &test2);
	NumToString(test3, display);
	test3 = NumSubtract(&test1, &test2);
	NumToString(test3, display);
	test3 = NumMultiply(&test1, &test2);
	NumToString(test3, display);
	test3 = NumDivide(&test1, &test2);
	NumToString(test3, display);

	StringToNum("-5.2", &test1);
	StringToNum("2.5", &test2);
	test3 = NumAdd(&test1, &test2);
	NumToString(test3, display);
	test3 = NumSubtract(&test1, &test2);
	NumToString(test3, display);
	test3 = NumMultiply(&test1, &test2);
	NumToString(test3, display);
	test3 = NumDivide(&test1, &test2);
	NumToString(test3, display);

	StringToNum("5604.234", &test1);
	StringToNum("0.2341", &test2);
	test3 = NumAdd(&test1, &test2);
	NumToString(test3, display);
	test3 = NumSubtract(&test1, &test2);
	NumToString(test3, display);
	test3 = NumMultiply(&test1, &test2);
	NumToString(test3, display);
	test3 = NumDivide(&test1, &test2);
	NumToString(test3, display);

	StringToNum("1", &test1);
	StringToNum("2", &test2);
	test3 = NumAdd(&test1, &test2);
	NumToString(test3, display);
	test3 = NumSubtract(&test1, &test2);
	NumToString(test3, display);
	test3 = NumMultiply(&test1, &test2);
	NumToString(test3, display);
	test3 = NumDivide(&test1, &test2);
	NumToString(test3, display);

	StringToNum("4", &test1);
	StringToNum("2.34", &test2);
	test3 = NumAdd(&test1, &test2);
	NumToString(test3, display);
	test3 = NumSubtract(&test1, &test2);
	NumToString(test3, display);
	test3 = NumMultiply(&test1, &test2);
	NumToString(test3, display);
	test3 = NumDivide(&test1, &test2);
	NumToString(test3, display);

	StringToNum("507.123", &test1);
	StringToNum("2", &test2);
	test3 = NumAdd(&test1, &test2);
	NumToString(test3, display);
	test3 = NumSubtract(&test1, &test2);
	NumToString(test3, display);
	test3 = NumMultiply(&test1, &test2);
	NumToString(test3, display);
	test3 = NumDivide(&test1, &test2);
	NumToString(test3, display);

	StringToNum("1", &test1);
	StringToNum("2", &test2);
	test3 = NumModulo(test1, test2);

	StringToNum("5", &test1);
	StringToNum("3", &test2);
	test3 = NumModulo(test1, test2);

	StringToNum("-1", &test1);
	StringToNum("2", &test2);
	test3 = NumModulo(test1, test2);

	StringToNum("1", &test1);
	StringToNum("-2", &test2);
	test3 = NumModulo(test1, test2);

	StringToNum("1.5", &test1);
	test3 = NumRound(test1);
	StringToNum("-2.5", &test2);
	test3 = NumRound(test2);

	StringToNum("54670.52", &test1);
	test3 = NumRound(test1);
	StringToNum("-2.36", &test2);
	test3 = NumRound(test2);


#endif
	
	// We don't need to quit; we're not concerned with turning the calculator on / off just yet.
	//HP20b_Quit();

	return 0;
}
