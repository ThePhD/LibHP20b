#include "AT91SAM7L128.h"
#include "HP20b_Keyboard.h"

#define HP20B_ONKEY_KEYBOARD_IOPIN AT91C_PC10_TWD
#define HP20B_ONKEY_KEYBOARD_ROW 6
#define HP20B_KEYBOARD_COLUMNS 0x7f
// The below is a combinated of the rest of the Pin Bit Indices shifted over and the ONKey IO Pin Value,
// Which is: 0x4fc00 | HP20B_ONKEY_KEYBOARD_IOPIN
#define HP20B_KEYBOARD_ROWS_PIN_BITS 0x4fe00 
/* Character codes returned by keyboard_key */
static char HP20b_KeyboardKeys[HP20B_KEYBOARD_NUMCOLUMNS][HP20B_KEYBOARD_NUMROWS] = {
	{ 0 }
};

// The IO status Pin Bits; the last one is reserved exclusively for ON/CE, which is a short/nonshorted test only of a single row
const static byte HP20b_KeyboardPinBit[] = { 11, 12, 13, 14, 15, 26, 10 };

byte hp20b_keycount = 0;
int hp20b_firstkey = -1;
int hp20b_lastkey = -1;

void HP20b_KeyboardInit () {
	if (HP20b.Keyboard)
		return;
#ifdef ARM
	// Initialize the keyboard: Columns are outputs, rows are inputs
	// The status pin bits are stored in defines
	AT91C_BASE_PMC->PMC_PCER = (uint) 1 << AT91C_ID_PIOC; // Turn on PIOC clock
	AT91C_BASE_PIOC->PIO_PER = HP20B_KEYBOARD_ROWS_PIN_BITS | HP20B_KEYBOARD_COLUMNS; // Enable control
	AT91C_BASE_PIOC->PIO_PPUDR = HP20B_KEYBOARD_COLUMNS; // Disable pullups on columns
	AT91C_BASE_PIOC->PIO_OER = HP20B_KEYBOARD_COLUMNS;	 // Make columns outputs
	AT91C_BASE_PIOC->PIO_PPUER = HP20B_KEYBOARD_ROWS_PIN_BITS;		// Enable pullups on rows
	AT91C_BASE_PIOC->PIO_ODR = HP20B_KEYBOARD_ROWS_PIN_BITS;			// Make rows inputs
	AT91C_BASE_PIOC->PIO_SODR = HP20B_KEYBOARD_COLUMNS;	// Drive all columns high
	
#else

#endif
	
	HP20b.Keyboard = true;
}

void HP20b_KeyboardQuit () {
	HP20b.Keyboard = false;
#ifdef ARM
	// Turn off PIOC clock
	AT91C_BASE_PMC->PMC_PCER = (uint)0 << AT91C_ID_PIOC;
	// Disable control on all pins
	AT91C_BASE_PIOC->PIO_PER = (uint)0; 
	// Drive all columns high
	AT91C_BASE_PIOC->PIO_SODR = HP20B_KEYBOARD_COLUMNS;
#else

#endif
}

void HP20b_KeyboardColumnHigh (int column) {
#ifdef ARM
	AT91C_BASE_PIOC->PIO_SODR = 1 << column;
#else

#endif
}

void HP20b_KeyboardColumnLow (int column) {
#ifdef ARM
	AT91C_BASE_PIOC->PIO_CODR = 1 << column;
#else

#endif
}

int HP20b_KeyboardRowRead (int row) {
#ifdef ARM
	return (AT91C_BASE_PIOC->PIO_PDSR) & (1 << HP20b_KeyboardPinBit[row]);
#else
	return true;
#endif
}

int HP20b_KeyDownCount () {
	return hp20b_keycount;
}

bool HP20b_AnyKeyDown () {
	return hp20b_keycount > 0;
}

int HP20b_FirstKey () {
	return hp20b_firstkey;
}

bool HP20b_LiveAnyKeyDown () {
	return HP20b_LiveFirstKey() != -1;
}

bool HP20b_FirstKeyIsUnique () {
	return hp20b_firstkey != -1 && hp20b_lastkey != hp20b_firstkey;
}

bool HP20b_LiveKeyUp (int key) {
	int row, col;
	if (key < 0) {
		return false;
	}
	else if (key == OnClear) {
		col = 6;
		row = 0;
		if (HP20b_KeyboardRowRead(col)) {
			return true;
		}
		return false;
	}
	else if (key > OnClear) {
		key -= OnClear + 1;
		row = key % HP20B_KEYBOARD_NUMROWS;
		col = (key - row) / (HP20B_KEYBOARD_NUMCOLUMNS - 1);
	}
	else {
		row = key % HP20B_KEYBOARD_NUMROWS;
		col = (key - row) / (HP20B_KEYBOARD_NUMCOLUMNS - 1);
	}
	
	HP20b_KeyboardColumnLow(col);
	if (HP20b_KeyboardRowRead(row)) {
		HP20b_KeyboardColumnHigh(col);
		return true;
	}

	return false;
}

bool HP20b_LiveKeyDown (int key) {
	int row, col;
	if (key < 0) {
		return false;
	}
	else if (key == OnClear) {
		col = 6;
		row = 0;
		if (HP20b_KeyboardRowRead(col)) {
			return false;
		}
		return true;
	}
	else if (key > OnClear) {
		key -= OnClear + 1;
		row = key % HP20B_KEYBOARD_NUMROWS;
		col = (key - row) / (HP20B_KEYBOARD_NUMCOLUMNS - 1);
	}
	else {
		row = key % HP20B_KEYBOARD_NUMROWS;
		col = (key - row) / (HP20B_KEYBOARD_NUMCOLUMNS - 1);
	}
	
	HP20b_KeyboardColumnLow(col);
	if (HP20b_KeyboardRowRead(row)) {
		HP20b_KeyboardColumnHigh(col);
		return false;
	}

	HP20b_KeyboardColumnHigh(col);
	return true;
}

int HP20b_LiveFirstKey () {
	int row, col;
	for (col = 0; col < HP20B_KEYBOARD_NUMCOLUMNS; col++) {
		HP20b_KeyboardColumnLow(col);
		for (row = 0; row < HP20B_KEYBOARD_NUMROWS; row++) {
			if (HP20b_KeyboardRowRead(row) == 0) {
				HP20b_KeyboardColumnHigh(col);
				return col * HP20B_KEYBOARD_NUMROWS + row;
			}
		}
		HP20b_KeyboardColumnHigh(col);
	}
	if (HP20b_KeyboardRowRead(6) == 0) {
		return OnClear;
	}
	return -1;
}

bool HP20b_KeyUp (int key) {
	char* keyboardkey = &HP20b_KeyboardKeys[0][0];
	if (key < 0) {
		return false;
	}
	else if (key > OnClear) {
		key = key % (OnClear + 1);
	}
	if (key == OnClear) {
		keyboardkey += OnClearSurrogate;
	}
	else {
		keyboardkey += key;
	}
	return (*keyboardkey & 0x1) == 0;
}

bool HP20b_KeyDown (int key) {
	char* keyboardkey = &HP20b_KeyboardKeys[0][0];
	if (key < 0) {
		return false;
	}
	else if (key > OnClear) {
		key = key % (OnClear + 1);
	}
	if (key == OnClear) {
		keyboardkey += OnClearSurrogate;
	}
	else {
		keyboardkey += key;
	}
	return (*keyboardkey & 0x1) != 0;
}

bool HP20b_KeyPressed (int key) {
	char* keyboardkey = &HP20b_KeyboardKeys[0][0];
	if (key < 0) {
		return false;
	}
	else if (key > OnClear) {
		key = key % (OnClear + 1);
	}
	if (key == OnClear) {
		keyboardkey += OnClearSurrogate;
	}
	else {
		keyboardkey += key;
	}
	return (*keyboardkey & 0x2) != 0;
}

bool HP20b_KeyReleased (int key) {
	char* keyboardkey = &HP20b_KeyboardKeys[0][0];
	if (key < 0) {
		return false;
	}
	else if (key > OnClear) {
		key = key % (OnClear + 1);
	}
	if (key == OnClear) {
		keyboardkey += OnClearSurrogate;
	}
	else {
		keyboardkey += key;
	}
	return (*keyboardkey & 0x4) != 0;
}

bool HP20b_KeyHeld (int key) {
	char* keyboardkey = &HP20b_KeyboardKeys[0][0];
	if (key < 0) {
		return false;
	}
	else if (key > OnClear) {
		key = key % (OnClear + 1);
	}
	if (key == OnClear) {
		keyboardkey += OnClearSurrogate;
	}
	else {
		keyboardkey += key;
	}
	return (*keyboardkey & 0x3) == 0x1;
}

void HP20b_KeyboardUpdate () {
	int col = 0, row = 0;
	hp20b_keycount = 0;
	hp20b_lastkey = hp20b_firstkey;
	hp20b_firstkey = -1;
	for (col = 0 ; col < HP20B_KEYBOARD_NUMCOLUMNS; col++) {
		HP20b_KeyboardColumnLow(col);
		for (row = 0 ; row < HP20B_KEYBOARD_NUMROWS; row++) {
			if (HP20b_KeyboardRowRead(row) == 0) {
				if (hp20b_firstkey == -1)
					hp20b_firstkey = col * HP20B_KEYBOARD_NUMROWS + row;
				++hp20b_keycount;
				// If it wasn't already down, then it's just been pressed down
				// Thus, the key is "Pressed" (State: Fresh Down)
				if ((HP20b_KeyboardKeys[col][row] & 0x1) == 0)
					HP20b_KeyboardKeys[col][row] |= 0x2;
				else
					HP20b_KeyboardKeys[col][row] &= ~(0x2);

				// Turn off the Release Bit
				HP20b_KeyboardKeys[col][row] &= ~(0x4);

				// Turn on the Down Bit (State: Down)
				HP20b_KeyboardKeys[col][row] |= 0x1;
			}
			else {
				// If it was already down, then it's up
				// Thus, the key is "Released" (State: Fresh Up)
				if ((HP20b_KeyboardKeys[col][row] & 0x1) != 0)
					HP20b_KeyboardKeys[col][row] |= 0x4;
				else
					HP20b_KeyboardKeys[col][row] &= ~(0x4);

				// Turn off the Pressed Bit
				HP20b_KeyboardKeys[col][row] &= ~(0x2);

				// Turn off the Down Bit (State: Up)
				HP20b_KeyboardKeys[col][row] &= ~(0x1);
			}
		}
		HP20b_KeyboardColumnHigh(col);
	}

	/* Now for the ON / CE Key */
	if (HP20b_KeyboardRowRead(6) == 0) {
		if (hp20b_firstkey == -1)
			hp20b_firstkey = OnClear;
		++hp20b_keycount;
		// If it wasn't already down, then it's just been pressed down
		// Thus, the key is "Pressed" (State: Fresh Down)
		if ((HP20b_KeyboardKeys[6][0] & 0x1) == 0)
			HP20b_KeyboardKeys[6][0] |= 0x2;
		else
			HP20b_KeyboardKeys[6][0] &= ~(0x2);

		// Turn off the Release Bit
		HP20b_KeyboardKeys[6][0] &= ~(0x4);

		// Turn on the Down Bit (State: Down)
		HP20b_KeyboardKeys[6][0] |= 0x1;
	}
	else {
		// If it was already down, then it's up
		// Thus, the key is "Released" (State: Fresh Up)
		if ((HP20b_KeyboardKeys[6][0] & 0x1) != 0)
			HP20b_KeyboardKeys[6][0] |= 0x4;
		else
			HP20b_KeyboardKeys[6][0] &= ~(0x4);

		// Turn off the Pressed Bit
		HP20b_KeyboardKeys[6][0] &= ~(0x2);

		// Turn off the Down Bit (State: Up)
		HP20b_KeyboardKeys[6][0] &= ~(0x1);
	}
}

int HP20b_KeyCode (int key) {
	if (key > OnClear) {
		key = key & (OnClear + 1);
	}
	switch (key) {
		case 17:
		case 23:
		case 29:
		case 35:
		case 41:
			return 0;
		case 0:
			return 'N';
		case 1:
			return 'I';
		case 2:
			return 'P';
		case 3:
			return 'M';
		case 4:
			return 'F';
		case 5:
			return 'A';
		case 6:
			return 'C';
		case 7:
			return 'R';
		case 8:
			return 'V';
		case 9:
			return 'B';
		case 10:
			return '%';
		case 11:
			return 'L';
		case 12:
			return '\r';
		case 13:
			return '(';
		case 14:
			return ')';
		case 15:
			return '~';
		case 16:
			return '\b';
		case 18:
			return '\v';
		case 19:
			return '7';
		case 20:
			return '8';
		case 21:
			return '9';
		case 22:
			return '/';
		case 24:
			return '\n';
		case 25:
			return '4';
		case 26:
			return '5';
		case 27:
			return '6';
		case 28:
			return '*';
		case 30:
			return 'S';
		case 31:
			return '1';
		case 32:
			return '2';
		case 33:
			return '3';
		case 34:
			return '-';
		case 37:
			return '0';
		case 38:
			return '.';
		case 39:
			return '=';
		case 40:
			return '+';
		case 36:
		case 42:
			return 'O';
		default:
			return 0;
	}
}

void HP20b_KeyNameShifted(int key, bool shift, char* out) {
	HP20b_KeyName(key + (shift != 0) * 43, out);
}


void HP20b_KeyName( int key, char* out ) {
	char* p = out;
	switch (key) {
	case 17:
	case 23:
	case 29:
	case 35:
	case 41:
		break;
	case 0:
		*p++ = 'N';
		break;
	case 1:
		*p++ = 'I';
		*p++ = '/';
		*p++ = 'Y';
		*p++ = 'R';
		break;
	case 2:
		*p++ = 'P';
		*p++ = 'V';
		break;
	case 3:
		*p++ = 'P';
		*p++ = 'M';
		*p++ = 'T';
		break;
	case 4:
		*p++ = 'F';
		*p++ = 'V';
		break;
	case 5:
		*p++ = 'A';
		*p++ = 'm';
		*p++ = 'o';
		*p++ = 'r';
		*p++ = 't';
		break;
	case 6:
		*p++ = 'C';
		*p++ = 's';
		*p++ = 'h';
		*p++ = 'F';
		*p++ = 'l';
		break;
	case 7:
		*p++ = 'I';
		*p++ = 'R';
		*p++ = 'R';
		break;
	case 8:
		*p++ = 'N';
		*p++ = 'P';
		*p++ = 'V';
		break;
	case 9:
		*p++ = 'B';
		*p++ = 'o';
		*p++ = 'n';
		*p++ = 'd';
		break;
	case 10:
		*p++ = '%';
		break;
	case 11:
		*p++ = 'R';
		*p++ = 'C';
		*p++ = 'L';
		break;
	case 12:
		*p++ = 'I';
		*p++ = 'n';
		*p++ = 'p';
		*p++ = 'u';
		*p++ = 't';
		break;
	case 13:
		*p++ = '(';
		break;
	case 14:
		*p++ = ')';
		break;
	case 15:
		*p++ = '+';
		*p++ = '/';
		*p++ = '-';
		break;
	case 16:
		*p++ = '<';
		*p++ = '-';
		break;
	case 18:
		*p++ = 'U';
		*p++ = 'P';
		break;
	case 19:
		*p++ = '7';
		break;
	case 20:
		*p++ = '8';
		break;
	case 21:
		*p++ = '9';
		break;
	case 22:
		*p++ = '/';
		break;
	case 24:
		*p++ = 'D';
		*p++ = 'o';
		*p++ = 'w';
		*p++ = 'n';
		break;
	case 25:
		*p++ = '4';
		break;
	case 26:
		*p++ = '5';
		break;
	case 27:
		*p++ = '6';
		break;
	case 28:
		*p++ = '*';
		break;
	case 30:
		*p++ = 'S';
		*p++ = 'h';
		*p++ = 'i';
		*p++ = 'f';
		*p++ = 't';
		break;
	case 31:
		*p++ = '1';
		break;
	case 32:
		*p++ = '2';
		break;
	case 33:
		*p++ = '3';
		break;
	case 34:
		*p++ = '-';
		break;
	case 36:
		*p++ = 'O';
		*p++ = 'n';
		*p++ = '/';
		*p++ = 'C';
		*p++ = 'l';
		*p++ = 'e';
		*p++ = 'a';
		*p++ = 'r';
		break;
	case 37:
		*p++ = '0';
		break;
	case 38:
		*p++ = '.';
		break;
	case 39:
		*p++ = '=';
		break;
	case 40:
		*p++ = '+';
		break;
	case 42:
		*p++ = 'O';
		*p++ = 'n';
		*p++ = '/';
		*p++ = 'C';
		*p++ = 'l';
		*p++ = 'e';
		*p++ = 'a';
		*p++ = 'r';
	break;
	case 14 + 43:
	case 17 + 43:
	case 23 + 43:
	case 29 + 43:
	case 35 + 43:
	case 41 + 43:
		break;
	case 0 + 43:
		*p++ = 'x';
		*p++ = 'P';
		*p++ = '/';
		*p++ = 'Y';
		*p++ = 'R';
		break;
	case 1 + 43:
		*p++ = 'I';
		*p++ = 'C';
		*p++ = 'o';
		*p++ = 'n';
		*p++ = 'v';
		break;
	case 2 + 43:
		*p++ = 'B';
		*p++ = 'e';
		*p++ = 'g';
		break;
	case 3 + 43:
		*p++ = 'P';
		*p++ = '/';
		*p++ = 'Y';
		*p++ = 'R';
		break;
	case 4 + 43:
		*p++ = 'E';
		*p++ = 'n';
		*p++ = 'd';
		break;
	case 5 + 43:
		*p++ = 'D';
		*p++ = 'e';
		*p++ = 'p';
		*p++ = 'r';
		break;
	case 6 + 43:
		*p++ = 'D';
		*p++ = 'a';
		*p++ = 't';
		*p++ = 'a';
		break;
	case 7 + 43:
		*p++ = 'S';
		*p++ = 't';
		*p++ = 'a';
		*p++ = 't';
		*p++ = 's';
		break;
	case 8 + 43:
		*p++ = 'B';
		*p++ = 'r';
		*p++ = 'k';
		*p++ = 'E';
		*p++ = 'v';
		break;
	case 9 + 43:
		*p++ = 'D';
		*p++ = 'a';
		*p++ = 't';
		*p++ = 'e';
		break;
	case 10 + 43:
		*p++ = '%';
		*p++ = 'c';
		*p++ = 'a';
		*p++ = 'l';
		*p++ = 'c';
		break;
	case 11 + 43:
		*p++ = 'S';
		*p++ = 'T';
		*p++ = 'O';
		break;
	case 12 + 43:
		*p++ = 'M';
		*p++ = 'e';
		*p++ = 'm';
		*p++ = 'o';
		*p++ = 'r';
		*p++ = 'y';
		break;
	case 13 + 43:
		*p++ = 'M';
		*p++ = 'o';
		*p++ = 'd';
		*p++ = 'e';
		break;
	case 15 + 43:
		*p++ = 'E';
		*p++ = 'E';
		*p++ = 'X';
		break;
	case 16 + 43:
		*p++ = 'R';
		*p++ = 'e';
		*p++ = 's';
		*p++ = 'e';
		*p++ = 't';
		break;
	case 18 + 43:
		*p++ = 'I';
		*p++ = 'n';
		*p++ = 's';
		break;
	case 19 + 43:
		*p++ = 'S';
		*p++ = 'i';
		*p++ = 'n';
		break;
	case 20 + 43:
		*p++ = 'C';
		*p++ = 'o';
		*p++ = 's';
		break;
	case 21 + 43:
		*p++ = 'T';
		*p++ = 'a';
		*p++ = 'n';
		break;
	case 22 + 43:
		*p++ = 'M';
		*p++ = 'a';
		*p++ = 't';
		*p++ = 'h';
		break;
	case 24 + 43:
		*p++ = 'D';
		*p++ = 'e';
		*p++ = 'l';
		break;
	case 25 + 43:
		*p++ = 'L';
		*p++ = 'n';
		break;
	case 26 + 43:
		*p++ = 'e';
		*p++ = '^';
		*p++ = 'x';
		break;
	case 27 + 43:
		*p++ = 'x';
		*p++ = '^';
		*p++ = '2';
		break;
	case 28 + 43:
		*p++ = 'S';
		*p++ = 'q';
		*p++ = 'r';
		*p++ = 't';
		break;
	case 30 + 43:
		*p++ = 'U';
		*p++ = 'n';
		*p++ = 'S';
		*p++ = 'h';
		*p++ = 'i';
		*p++ = 'f';
		*p++ = 't';
		break;
	case 31 + 43:
		*p++ = 'R';
		*p++ = 'a';
		*p++ = 'n';
		*p++ = 'd';
		*p++ = 'o';
		*p++ = 'm';
		break;
	case 32 + 43:
		*p++ = '!';
		break;
	case 33 + 43:
		*p++ = 'y';
		*p++ = '^';
		*p++ = 'x';
		break;
	case 34 + 43:
		*p++ = '1';
		*p++ = '/';
		*p++ = 'x';
		break;
	case 36 + 43:
		*p++ = 'O';
		*p++ = 'f';
		*p++ = 'f';
		break;
	case 37 + 43:
		*p++ = 'n';
		*p++ = 'P';
		*p++ = 'r';
		break;
	case 38 + 43:
		*p++ = 'n';
		*p++ = 'C';
		*p++ = 'r';
		break;
	case 39 + 43:
		*p++ = 'A';
		*p++ = 'N';
		*p++ = 'S';
		break;
	case 40 + 43:
		*p++ = 'R';
		*p++ = 'N';
		*p++ = 'D';
		break;
	case 42 + 43:
		*p++ = 'O';
		*p++ = 'f';
		*p++ = 'f';
		break;
	default:
		break;
	}
	*p = '\0';
}
