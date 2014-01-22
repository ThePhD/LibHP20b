#ifndef LIBHP20BKEYBOARD_H
#define LIBHP20BKEYBOARD_H

#include "Core.h"

/* Keyboard Layout

	 Rows and columns on HP 20b schematic are reversed from what you'd expect

	 Columns are outputs; rows are read

		Row0		Row1		Row2		Row3		Row4		Row5
		PC11		PC12		PC13		PC14		PC15		PC26

Col0	PC0	 N		I/YR		PV		PMT		FV		Amort
Col1	PC1	 CshFI	IRR		NPV		Bond		%		RCL
Col2	PC2	 INPUT	 (		)		+/-		<-
Col3	PC3	 UP		 7		8		9		/														 
Col4	PC4	 DOWN	 4		5		6		*																
Col5	PC5	 SHIFT	 1		2		3		-														 
Col6	PC6					0		.		=		+				 

	ON/CE is separate
 */

#define HP20B_KEYBOARD_NUMCOLUMNS 7
#define HP20B_KEYBOARD_NUMROWS 6

// Initialize the keyboard and set all columns high with pullups on the rows
extern void HP20b_KeyboardInit(void);
extern void HP20b_KeyboardQuit(void);

extern void HP20b_KeyboardColumnHigh(int column);
extern void HP20b_KeyboardColumnLow(int column);
extern int HP20b_KeyboardRowRead(int row);

extern void HP20b_KeyboardUpdate(void);

extern int HP20b_KeyDownCount(void);
extern bool HP20b_AnyKeyDown(void);
extern bool HP20b_KeyUp( int key );
extern bool HP20b_KeyDown( int key );
extern int HP20b_FirstKey(void);
extern bool HP20b_FirstKeyIsUnique(void);
extern bool HP20b_KeyPressed( int key );
extern bool HP20b_KeyReleased( int key );
extern bool HP20b_KeyHeld( int key );

extern bool HP20b_LiveAnyKeyDown(void);
extern bool HP20b_LiveKeyUp( int key );
extern bool HP20b_LiveKeyDown( int key );
extern int HP20b_LiveFirstKey( void );

extern int HP20b_KeyCode (int key);
extern void HP20b_KeyName( int key, char* out );
extern void HP20b_KeyNameShifted(int key, bool shift, char* out);

#endif /* LIBHP20BKEYBOARD_H */
