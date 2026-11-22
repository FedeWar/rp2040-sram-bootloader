/*
 * boot.cpp
 *
 *  Created on: 22 nov 2026
 *      Author: feder
 */
// Project
#include "bootloader.h"
#include "load_flash.h"

BOOT_ENTRY void boot()
{
	flash_read_data(FLASH_START, (uint8_t*) FLASH_DEST, FLASH_SIZE);

	void (*main)(void) = (void (*)()) (FLASH_DEST | 1);
	main();

	__builtin_trap();
}
