@echo off

set RAMFILENAME=".\Debug\ram_all.bin"
::echo %RAMFILENAME%

cp ..\..\..\..\..\component\soc\realtek\8195a\misc\gcc_utility\target_NORMALB.axf ..\..\..\..\..\component\soc\realtek\8195a\misc\gcc_utility\target_NORMAL.axf

::===============================================================================
::get file size and translate to HEX
for %%A in (%RAMFILENAME%) do set fileSize=%%~zA
@call :toHex %fileSize% fileSizeHex

::echo %fileSize%
::echo %fileSizeHex%

echo set $RamFileSize = 0x%fileSizeHex% > .\Debug\fwsize.gdb

:: start GDB for flash write, argument 1 (%1) could be rather "openocd" or "jlink"
::echo %1
..\..\..\..\..\tools\arm-none-eabi-gcc\4_8-2014q3\bin\arm-none-eabi-gdb -x ..\..\..\..\..\component\soc\realtek\8195a\misc\gcc_utility\eclipse\rtl_eclipse_flash_write_%1.txt

exit

:toHex dec hex -- convert a decimal number to hexadecimal, i.e. -20 to FFFFFFEC or 26 to 0000001A
@echo off
SETLOCAL ENABLEDELAYEDEXPANSION
set /a dec=%~1
set "hex="
set "map=0123456789ABCDEF"
for /L %%N in (1,1,8) do (
    set /a "d=dec&15,dec>>=4"
    for %%D in (!d!) do set "hex=!map:~%%D,1!!hex!"
)

( ENDLOCAL & REM RETURN VALUES
    IF "%~2" NEQ "" (SET %~2=%hex%) ELSE ECHO.%hex%
)
EXIT /b
