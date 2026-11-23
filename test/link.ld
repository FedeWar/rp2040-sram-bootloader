MEMORY
{
	FLASH(RX)	: ORIGIN = 0x10000100, LENGTH = 2048k-256
	RAM(RWX)	: ORIGIN = 0x20000000, LENGTH = 264k
}

SECTIONS
{
	.text :
	{
		. = ORIGIN(RAM);
		KEEP(*(.main*));
		*(.text*)
		*(.ARM*)
	} > RAM AT > FLASH
}
