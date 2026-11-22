# rp2040-sram-bootloader
The RP2040 MCU uses a two-stage bootsequence: the first stage is executed by the internal bootrom and the second stage has to be provided in the flash program. This bootloader covers the second stage of the boot sequence.

## Prerequisites
1. the PICO SDK installed and set up.
2. the arm-none-eabi toolchain in PATH (from PICO SDK).
3. the elf2uf2 executable in PATH (from PICO SDK).
4. the crc32 executable in PATH (from [here](https://github.com/FedeWar/crc-mpeg2)).
5. the uf2merge executable in PATH (not needed to build the bootloader, only to generate a complete firmware) (from [here](https://github.com/FedeWar/uf2merge)).

## How to use it
First you have to build it, the result will be a 'boot.uf2' file. If everything is set up properly, the Makefile will take care of everything:
> make all

Unfortunatelly, the toolchain cannot check whether the CRC inserted in the firmware is correct. This last step has to be performed manually. If the previous command completed succesfully the following strings should be visible in the terminal. The exact numbers may vary.
> Int8 Little endian: 0x36 0xa7 0xc3 0xd0
> 
> Int32 Big endian: 0xd0c3a736

What is printed is the CRC computed on the UF2 file. The four numbers on the first line have to be copied in the crc.cpp file, then again:
> make all

You should see again printed the same numbers as before.

As a last step, you have to merge it with the target firmware file, which should be another UF2 file, using the following command.
> uf2merge -i boot.uf2 -i target.uf2 -o target_merged.uf2

## How it works
TODO

## How to customize it
TODO

## How to validate it
Whether you just completed the build as provided by this repo or you just customized the source code, this section will provide the necessary steps to determine whether the resulting firmware was assembled properly.

TODO
