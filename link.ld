
MEMORY
{
	FLASH(RX)	: ORIGIN = 0x10000000, LENGTH = 2048k
	RAM(RWX)	: ORIGIN = 0x20041f00, LENGTH = 256
}

SECTIONS
{
	.text :
	{
		. = ORIGIN(RAM);
		KEEP(*(.boot_entry*));
		KEEP(*(.boot_utils*));
		. = ORIGIN(RAM) + LENGTH(RAM) - 0x04;
		KEEP(*(.crc*));
	} > RAM AT > FLASH
}
