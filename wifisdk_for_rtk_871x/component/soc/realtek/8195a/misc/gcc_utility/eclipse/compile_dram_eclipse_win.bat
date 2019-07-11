:: application/Debug
@echo off

set bindir=.
set gccdir=..\..\..\..\..\..\tools\arm-none-eabi-gcc\4_8-2014q3\bin

::e.g. arm-none-eabi-gcc -c -mcpu=cortex-m3 -mthumb -std=gnu99 -DGCC_ARMCM3 -DM3 -DCONFIG_PLATFORM_8195A -DF_CPU=166000000L -I..\..\..\..\inc -I..\..\..\..\..\..\component\soc\realtek\common\bsp -I..\..\..\..\..\..\component\os\freertos -I..\..\..\..\..\..\component\os\freertos\freertos_v8.1.2\Source\include -I..\..\..\..\..\..\component\os\freertos\freertos_v8.1.2\Source\portable\GCC\ARM_CM3 -I..\..\..\..\..\..\component\os\os_dep\include -I..\..\..\..\..\..\component\soc\realtek\8195a\misc\driver -I..\..\..\..\..\..\component\common\api\network\include -I..\..\..\..\..\..\component\common\api -I..\..\..\..\..\..\component\common\api\platform -I..\..\..\..\..\..\component\common\api\wifi -I..\..\..\..\..\..\component\common\api\wifi\rtw_wpa_supplicant\src -I..\..\..\..\..\..\component\common\application -I..\..\..\..\..\..\component\common\media\framework -I..\..\..\..\..\..\component\common\example -I..\..\..\..\..\..\component\common\example\wlan_fast_connect -I..\..\..\..\..\..\component\common\mbed\api -I..\..\..\..\..\..\component\common\mbed\hal -I..\..\..\..\..\..\component\common\mbed\hal_ext -I..\..\..\..\..\..\component\common\mbed\targets\hal\rtl8195a -I..\..\..\..\..\..\component\common\network -I..\..\..\..\..\..\component\common\network\lwip\lwip_v1.4.1\port\realtek\freertos -I..\..\..\..\..\..\component\common\network\lwip\lwip_v1.4.1\src\include -I..\..\..\..\..\..\component\common\network\lwip\lwip_v1.4.1\src\include\lwip -I..\..\..\..\..\..\component\common\network\lwip\lwip_v1.4.1\src\include\ipv4 -I..\..\..\..\..\..\component\common\network\lwip\lwip_v1.4.1\port\realtek -I..\..\..\..\..\..\component\common\test -I..\..\..\..\..\..\component\soc\realtek\8195a\cmsis -I..\..\..\..\..\..\component\soc\realtek\8195a\cmsis\device -I..\..\..\..\..\..\component\soc\realtek\8195a\fwlib -I..\..\..\..\..\..\component\soc\realtek\8195a\fwlib\rtl8195a -I..\..\..\..\..\..\component\soc\realtek\8195a\misc\rtl_std_lib\include -I..\..\..\..\..\..\component\common\drivers\wlan\realtek\include -I..\..\..\..\..\..\component\common\drivers\wlan\realtek\src\osdep -I..\..\..\..\..\..\component\soc\realtek\8195a\fwlib\ram_lib\wlan\realtek\wlan_ram_map\rom -I..\..\..\..\..\..\component\common\network\ssl\polarssl-1.3.8\include -I..\..\..\..\..\..\component\common\network\ssl\ssl_ram_map\rom -I..\..\..\..\..\..\component\common\utilities -I..\..\..\..\..\..\component\soc\realtek\8195a\misc\rtl_std_lib\include -I..\..\..\..\..\..\component\soc\realtek\8195a\fwlib\ram_lib\usb_otg\include -I..\..\..\..\..\..\component\common\video\v4l2\inc -I..\..\..\..\..\..\component\common\media\codec -I..\..\..\..\..\..\component\common\drivers\usb_class\host\uvc\inc -I..\..\..\..\..\..\component\common\drivers\usb_class\device -I..\..\..\..\..\..\component\common\drivers\usb_class\device\class -I..\..\..\..\..\..\component\common\file_system\fatfs -I..\..\..\..\..\..\component\common\file_system\fatfs\r0.10c\include -I..\..\..\..\..\..\component\common\drivers\sdio\realtek\sdio_host\inc -I..\..\..\..\..\..\component\common\audio -I..\..\..\..\..\..\component\common\drivers\i2s -I..\..\..\..\..\..\component\common\application\apple\WACServer\External\Curve25519 -I..\..\..\..\..\..\component\common\application\apple\WACServer\External\GladmanAES -I..\..\..\..\..\..\component\common\application\google -I..\..\..\..\..\..\component\common\application\xmodem -O2 -g -Wno-pointer-sign -fno-common -fmessage-length=0 -ffunction-sections -fdata-sections -fomit-frame-pointer -fno-short-enums -fsigned-char -MMD -MP -MFSDRAM/polarssl/aes.d -MTSDRAM/polarssl/aes.o -o SDRAM/polarssl/aes.o D:/test/sdk-ameba1-v3.5b_beta_v2/component/common/network/ssl/polarssl-1.3.8/library/aes.c

REM get last two argument which is the output object file
call :GetLastTwoArg %*
::echo %lastTwoArg%

:: check argument count
::set argC=0
::for %%x in (%*) do Set /A argC+=1
::echo %argC%

:: gcc compile, %1 might be gcc or arm-none-eabi-gcc
set arg1=%1
if not "%arg1:arm-none-eabi-=%" == "%arg1%" (
	::echo is arm-none-eabi-gcc
	%gccdir%\%*
) else (
	::echo is gcc
	%gccdir%\arm-none-eabi-%*
)

:: objcopy append .sdram section info
%gccdir%\arm-none-eabi-objcopy --prefix-alloc-sections .sdram %lastTwoArg%

REM This subroutine gets the last command-line argument by using SHIFT
:GetLastTwoArg
set "lastTwoArg=%~1"
shift
if not "%~2"=="" goto GetLastTwoArg
goto :eof

exit