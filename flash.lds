OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(resetHandler)

MEMORY
{
		sram (W!RX) :			 ORIGIN = 0x200000, LENGTH = 0x1000
		backupsram (W!RX) : ORIGIN = 0x300000, LENGTH = 0x800
		flash (RX) :				ORIGIN = 0x100000, LENGTH = 0x20000
}

SECTIONS {
 
	 .text : {
		 *(.vectors)
		 *(.text)
		 *(.note.gnu.build-id)
		 *(.rodata)
	 } > flash

	 .bss (NOLOAD) : {
		 *(.bss)
	 } > sram

	 PROVIDE (_stack = 0x200800); /* C stack */
	 PROVIDE (_irqstack = 0x201000); /* Interrupt stack */
}
