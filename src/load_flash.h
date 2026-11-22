/*
 * load_flash.h
 *
 *  Created on: 22 nov 2026
 *      Author: feder
 */
#pragma once
// C++
#include <cstdint>
#include <cstddef>

#define FLASHCMD_READ_DATA 0x3

typedef enum {
    OUTOVER_NORMAL = 0,
    OUTOVER_INVERT,
    OUTOVER_LOW,
    OUTOVER_HIGH
} outover_t;

BOOT_FUNC void flash_read_data(uint32_t addr, uint8_t *rx, size_t count);
