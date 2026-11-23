/*
 * bootloader.h
 *
 *  Created on: 22 nov 2026
 *      Author: feder
 */
#pragma once
// Project
// C++
// ARM

#define BOOT_FUNC extern "C" __attribute__((section(".boot_utils")))
#define BOOT_ENTRY extern "C" __attribute__((section(".boot_entry"), noreturn))

#define FLASH_SIZE 1024
#define FLASH_DEST 0x20000000L
#define FLASH_START 0x00000100L
