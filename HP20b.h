#ifndef LIBHP20B_H
#define LIBHP20B_H

#include "AT91SAM7L128.h"
#include "Core.h"
#include "HP20b_LCD.h"
#include "HP20b_Keyboard.h"

extern void HP20b_Init(int systems);
extern void HP20b_Quit(void);
extern void HP20b_QuitSubsystem(int system);
extern void HP20b_Wait(ulong milliseconds);
extern void HP20b_S7Diplay(void);
extern void HP20b_MatrixDisplay(void);
extern bool HP20b_DisplayNum(Number* num);
extern int HP20b_CPUSpeed(void);
extern Number* HP20b_CurrentNum(void);
extern Number* HP20b_LastNum(void);
extern Number* HP20b_Reset(void);
extern Number* HP20b_ClearNum(void);

#endif /* LIBHP20B_H */
