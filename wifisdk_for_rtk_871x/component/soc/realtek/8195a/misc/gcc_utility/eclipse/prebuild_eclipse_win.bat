:: application/Debug
@echo off

set bindir=.

:: unzip arm-none-eabi-gcc\4_8-2014q3.tar
set tooldir=..\..\..\..\..\..\component\soc\realtek\8195a\misc\gcc_utility\tools
set currentdir=%cd%

:CheckOS
if exist "%PROGRAMFILES(X86)%" (goto 64bit) else (goto 32bit)
:64bit
set os_prefix=x86_64
goto next
:32bit
set os_prefix=x86
:next

if not exist ..\..\..\..\..\..\tools\arm-none-eabi-gcc\4_8-2014q3\ (
	:: %tooldir%\%os_prefix%\gzip -dc ..\..\..\..\..\..\tools\arm-none-eabi-gcc\4.8.3-2014q1.tar.gz > ..\..\..\..\..\..\tools\arm-none-eabi-gcc\4.8.3-2014q1.tar
	cd ..\..\..\..\..\..\tools\arm-none-eabi-gcc
	..\..\component\soc\realtek\8195a\misc\gcc_utility\tools\%os_prefix%\tar -xf 4_8-2014q3.tar
	cd %currentdir%
)

set gccdir=..\..\..\..\..\..\tools\arm-none-eabi-gcc\4_8-2014q3\bin

::echo %tooldir%

:: Generate build_info.h
for /f "usebackq" %%i in (`hostname`) do set hostname=%%i

echo #define UTS_VERSION "%date:~0,10%-%time:~0,8%" > ..\..\..\..\inc\build_info.h
echo #define RTL8195AFW_COMPILE_TIME "%date:~0,10%-%time:~0,8%" >> ..\..\..\..\inc\build_info.h
echo #define RTL8195AFW_COMPILE_DATE "%date:~0,4%%date:~5,2%%date:~8,2%" >> ..\..\..\..\inc\build_info.h
echo #define RTL8195AFW_COMPILE_BY "%USERNAME%" >> ..\..\..\..\inc\build_info.h
echo #define RTL8195AFW_COMPILE_HOST "%hostname%" >> ..\..\..\..\inc\build_info.h
echo #define RTL8195AFW_COMPILE_DOMAIN >> ..\..\..\..\inc\build_info.h
echo #define RTL195AFW_COMPILER "GCC compiler" >> ..\..\..\..\inc\build_info.h

xcopy /Y ..\..\..\rlx8195A-symbol-v02-img2.ld .
xcopy /Y ..\..\..\export-rom_v02.txt .

::copy bootloader and convert from image to object file
xcopy /Y ..\..\..\..\..\..\component\soc\realtek\8195a\misc\bsp\image\ram_1.r.bin .
%gccdir%\arm-none-eabi-objcopy --rename-section .data=.loader.data,contents,alloc,load,readonly,data -I binary -O elf32-littlearm -B arm ram_1.r.bin ram_1.r.o 

exit
