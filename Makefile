SRC=boot.cpp crc.cpp load_flash.cpp
EXE=boot.elf
UF2=boot.uf2
BUILDDIR=build
CFLAGS=-mcpu=cortex-m0plus -march=armv6-m -mthumb -mlittle-endian -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Werror -Wall -std=gnu++11 -fabi-version=0 -fno-exceptions -c
LFLAGS=-mcpu=cortex-m0plus -march=armv6-m -mthumb -mlittle-endian -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Werror -Wall -T "link.ld" -nostdlib -Xlinker --gc-sections -Xlinker --print-gc-sections -Wl,-Map,"$(BUILDDIR)/boot.map" --specs=nosys.specs
PICOSDK=C:\Program Files\Raspberry Pi\Pico SDK v1.5.1
INCLUDES=-I"$(PICOSDK)\gcc-arm-none-eabi\arm-none-eabi\include\c++\10.3.1" -I"$(PICOSDK)\pico-sdk\src\common\pico_base\include" -I"$(PICOSDK)\pico-sdk\src\rp2_common\pico_platform\include" -I"$(PICOSDK)\pico-sdk\src\rp2_common\hardware_base\include" -I"$(PICOSDK)\pico-sdk\src\rp2040\hardware_regs\include" -I"$(PICOSDK)\pico-sdk\src\rp2040\hardware_structs\include"

vpath %.cpp src

OBJS=$(addprefix $(BUILDDIR)/,$(SRC:.cpp=.o))

all: folder $(OBJS)
	arm-none-eabi-g++ $(LFLAGS) -o $(BUILDDIR)/$(EXE) $(OBJS)
	elf2uf2 $(BUILDDIR)/$(EXE) $(BUILDDIR)/$(UF2)
	crc32 -i $(BUILDDIR)/$(UF2) -s 32 -l 252

folder:
	-mkdir $(BUILDDIR)

$(BUILDDIR)/%.o: %.cpp
	arm-none-eabi-g++ $(CFLAGS) $(INCLUDES) -o $@ $<

clean:
	-rmdir /s /q build
