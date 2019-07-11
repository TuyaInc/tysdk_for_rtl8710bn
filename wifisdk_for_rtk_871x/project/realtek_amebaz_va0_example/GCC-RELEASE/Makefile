all: ram_all

OS := $(shell uname)

.PHONY: toolchain
toolchain:
ifeq ($(OS),Linux)
	if [ ! -d ../../../tools/arm-none-eabi-gcc/4_8-2014q3 ] ; then tar -jxf ../../../tools/arm-none-eabi-gcc/gcc-arm-none-eabi-4_8-2014q3-20140805-linux.tar.bz2 -C ../../../tools/arm-none-eabi-gcc/ ; mv ../../../tools/arm-none-eabi-gcc/gcc-arm-none-eabi-4_8-2014q3 ../../../tools/arm-none-eabi-gcc/4_8-2014q3 ; fi
else
	if [ ! -d ../../../tools/arm-none-eabi-gcc/4_8-2014q3 ] ; then tar -xf ../../../tools/arm-none-eabi-gcc/4_8-2014q3.tar -C ../../../tools/arm-none-eabi-gcc/ ; fi
endif


.PHONY: ram_all
ram_all: toolchain
	@$(MAKE) -f application.mk 

.PHONY: mp
mp: toolchain
	@$(MAKE) -f application.mk mp MP=1

.PHONY: clean
clean:
	@$(MAKE) -f application.mk clean
	
.PHONY: flash debug ramdebug setup
setup:
	@$(MAKE) -f application.mk $(MAKECMDGOALS)

flash: toolchain
	@$(MAKE) -f application.mk flashburn
	
debug: toolchain
	@$(MAKE) -f application.mk debug	

ramdebug: toolchain
	@$(MAKE) -f application.mk ramdebug	
