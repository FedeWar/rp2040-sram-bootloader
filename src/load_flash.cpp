/*
 * load_flash.cpp
 *
 *  Created on: 22 nov 2026
 *      Author: feder
 */
// Project
#include "bootloader.h"
#include "load_flash.h"
// PICO
#include <hardware/regs/io_qspi.h>
#include <hardware/structs/ssi.h>

/*
 * Copied from https://github.com/raspberrypi/pico-bootrom-rp2040/blob/master/bootrom/program_flash_generic.c
 */

BOOT_FUNC void flash_cs_force(outover_t over)
{
    io_rw_32 *reg = (io_rw_32 *) (IO_QSPI_BASE + IO_QSPI_GPIO_QSPI_SS_CTRL_OFFSET);
    *reg = (*reg & ~IO_QSPI_GPIO_QSPI_SS_CTRL_OUTOVER_BITS) | (over << IO_QSPI_GPIO_QSPI_SS_CTRL_OUTOVER_LSB);

    // Read to flush async bridge
    (void) *reg;
}

// Timing of this one is critical, so do not expose the symbol to debugger etc
BOOT_FUNC void __force_inline flash_put_cmd_addr(uint8_t cmd, uint32_t addr)
{
    flash_cs_force(OUTOVER_LOW);
    addr |= cmd << 24;
    for (int i = 0; i < 4; ++i) {
        ssi_hw->dr0 = addr >> 24;
        addr <<= 8;
    }
}

BOOT_FUNC int flash_was_aborted()
{
    return *(io_rw_32 *) (IO_QSPI_BASE + IO_QSPI_GPIO_QSPI_SD1_CTRL_OFFSET)
           & IO_QSPI_GPIO_QSPI_SD1_CTRL_INOVER_BITS;
}

BOOT_FUNC void __force_inline flash_put_get(const uint8_t *tx, uint8_t *rx, size_t count, size_t rx_skip) {
    // Make sure there is never more data in flight than the depth of the RX
    // FIFO. Otherwise, when we are interrupted for long periods, hardware
    // will overflow the RX FIFO.
    const uint max_in_flight = 16 - 2; // account for data internal to SSI
    size_t tx_count = count;
    size_t rx_count = count;
    while (tx_count || rx_skip || rx_count) {
        // NB order of reads, for pessimism rather than optimism
        uint32_t tx_level = ssi_hw->txflr;
        uint32_t rx_level = ssi_hw->rxflr;
        bool did_something = false; // Expect this to be folded into control flow, not register
        if (tx_count && tx_level + rx_level < max_in_flight) {
            ssi_hw->dr0 = (uint32_t) (tx ? *tx++ : 0);
            --tx_count;
            did_something = true;
        }
        if (rx_level) {
            uint8_t rxbyte = ssi_hw->dr0;
            did_something = true;
            if (rx_skip) {
                --rx_skip;
            } else {
                if (rx)
                    *rx++ = rxbyte;
                --rx_count;
            }
        }
        // APB load costs 4 cycles, so only do it on idle loops (our budget is 48 cyc/byte)
        if (!did_something && __builtin_expect(flash_was_aborted(), 0))
            break;
    }
    flash_cs_force(OUTOVER_HIGH);
}

BOOT_FUNC void flash_read_data(uint32_t addr, uint8_t *rx, size_t count)
{
    flash_put_cmd_addr(FLASHCMD_READ_DATA, addr);
    flash_put_get(NULL, rx, count, 4);
}
