:: application/Debug
@echo off

set bindir=.
set tooldir=..\..\..\..\..\..\component\soc\realtek\8195a\misc\iar_utility\common\tools
set libdir=..\..\..\..\..\..\component\soc\realtek\8195a\misc\bsp
set gccdir=..\..\..\..\..\..\tools\arm-none-eabi-gcc\4_8-2014q3\bin

::echo %tooldir%
::echo %libdir%

::copy bootloader and convert from image to object file
::copy ../../../../../component/soc/realtek/8195a/misc/bsp/image/ram_1.r.bin ram_1.r.bin
::%gccdir%/arm-none-eabi-objcopy --rename-section .data=.loader.data,contents,alloc,load,readonly,data -I binary -O elf32-littlearm -B arm ram_1.r.bin ram_1.r.o 

::del Debug/Exe/target.map Debug/Exe/application.asm *.bin
cmd /c "%gccdir%\arm-none-eabi-nm %bindir%/application.axf | %tooldir%\sort > %bindir%/application.nm.map"
cmd /c "%gccdir%\arm-none-eabi-objdump -d %bindir%/application.axf > %bindir%/application.asm"


for /f "delims=" %%i in ('cmd /c "%tooldir%\grep __ram_image2_text_start__ %bindir%/application.nm.map | %tooldir%\gawk '{print $1}'"') do set ram2_start=0x%%i
for /f "delims=" %%i in ('cmd /c "%tooldir%\grep __sdram_data_start__ %bindir%/application.nm.map | %tooldir%\gawk '{print $1}'"') do set ram3_start=0x%%i

for /f "delims=" %%i in ('cmd /c "%tooldir%\grep __ram_image2_text_end__ %bindir%/application.nm.map  | %tooldir%\gawk '{print $1}'"') do set ram2_end=0x%%i
for /f "delims=" %%i in ('cmd /c "%tooldir%\grep __sdram_data_end__ %bindir%/application.nm.map  | %tooldir%\gawk '{print $1}'"') do set ram3_end=0x%%i

::echo %ram1_start% > tmp.txt
echo %ram2_start%
echo %ram3_start%
::echo %ram1_end% >> tmp.txt
echo %ram2_end%
echo %ram3_end%


%gccdir%\arm-none-eabi-objcopy -j .image2.start.table -j .ram_image2.text -j .ram_image2.rodata -j .ram.data -Obinary %bindir%/application.axf %bindir%/ram_2.bin
if NOT %ram3_start% == %ram3_end% (
	%gccdir%\arm-none-eabi-objcopy -j .sdr_text -j .sdr_rodata -j .sdr_data -Obinary %bindir%/application.axf %bindir%/sdram.bin
)

%tooldir%\pick %ram2_start% %ram2_end% %bindir%\ram_2.bin %bindir%\ram_2.p.bin body+reset_offset+sig
if defined %ram3_start (
%tooldir%\pick %ram3_start% %ram3_end% %bindir%\sdram.bin %bindir%\ram_3.p.bin body+reset_offset
)

:: check ram_1.p.bin exist, copy default
if not exist %bindir%\ram_1.p.bin (
	copy %libdir%\image\ram_1.p.bin %bindir%\ram_1.p.bin
)

::padding ram_1.p.bin to 32K+4K+4K+4K, LOADER/RSVD/SYSTEM/CALIBRATION
%tooldir%\padding 44k 0xFF %bindir%\ram_1.p.bin

:: SDRAM case
if defined %ram3_start (
copy /b %bindir%\ram_1.p.bin+%bindir%\ram_2.p.bin+%bindir%\ram_3.p.bin %bindir%\ram_all.bin
copy /b %bindir%\ram_2.p.bin+%bindir%\ram_3.p.bin %bindir%\ota.bin
)

%tooldir%\checksum Debug\Exe\ota.bin

:: NO SDRAM case
if not defined %ram3_start (
copy /b %bindir%\ram_1.p.bin+%bindir%\ram_2.p.bin %bindir%\ram_all.bin
copy /b %bindir%\ram_2.p.bin %bindir%\ota.bin
)

del ram_1.r.*
del ram*.p.bin
del ram_2.bin
if exist sdram.bin (
	del sdram.bin
)

exit
