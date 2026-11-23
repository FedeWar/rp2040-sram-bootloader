SRC=main.cpp
EXE=blinky.elf

BOOT_UF2=..\build\boot.uf2
ARMINC=C:\Program Files\Raspberry Pi\Pico SDK v1.5.1\gcc-arm-none-eabi\arm-none-eabi\include
PICOSDK=C:\Program Files\Raspberry Pi\Pico SDK v1.5.1\pico-sdk\src

CFLAGS=-mcpu=cortex-m0plus -march=armv6-m -mthumb -mlittle-endian -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Werror -Wall -std=gnu++11 -fabi-version=0 -fno-exceptions -c
LFLAGS=-N -mcpu=cortex-m0plus -march=armv6-m -mthumb -mlittle-endian -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Werror -Wall -T "link.ld" -nostdlib -Xlinker --gc-sections -Xlinker --print-gc-sections -Wl,-Map,"$(BUILDDIR)/$(EXE:.elf=.map)" --specs=nosys.specs
INCLUDES=-I"$(ARMINC)\c++\10.3.1" -I"$(PICOSDK)\common\pico_base\include" -I"$(PICOSDK)\rp2_common\pico_platform\include" -I"$(PICOSDK)\rp2_common\hardware_base\include" -I"$(PICOSDK)\rp2040\hardware_regs\include" -I"$(PICOSDK)\rp2040\hardware_structs\include"

BUILDDIR=build
OBJS=$(addprefix $(BUILDDIR)/,$(SRC:.cpp=.o))
UF2=$(EXE:.elf=.uf2)
vpath %.cpp src

all: folder $(OBJS)
	arm-none-eabi-g++ $(LFLAGS) -o $(BUILDDIR)/$(EXE) $(OBJS)
	elf2uf2 $(BUILDDIR)/$(EXE) $(BUILDDIR)/temp.uf2
	uf2merge -i "$(BOOT_UF2)" -i "$(BUILDDIR)/temp.uf2" -o "$(BUILDDIR)/$(UF2)"

folder:
	-mkdir $(BUILDDIR)

$(BUILDDIR)/%.o: %.cpp
	arm-none-eabi-g++ $(CFLAGS) $(INCLUDES) -o $@ $<

clean:
	-rmdir /s /q $(BUILDDIR)

dump:
	arm-none-eabi-objdump -hSD $(BUILDDIR)/$(EXE) > $(BUILDDIR)/$(EXE:.elf=.objdump)
