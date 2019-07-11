taskkill /F /IM openocd.exe
openocd -f interface\cmsis-dap.cfg -f ..\..\..\component\soc\realtek\8711b\misc\gcc_utility\openocd\amebaz.cfg
