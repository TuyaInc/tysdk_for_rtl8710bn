#!/bin/sh
#BUILD_TYPE=8710_1M
BUILD_TYPE=8710_2M

if [ -z "$1" ];then
        echo "please input the app bin name(no suffix \".bin\")!!!"
        exit 1
else
        APP_BIN_NAME=$1
fi

if [ -z "$2" ];then
        echo "please input the app sw version(for example:the format is "1.1.1")!!!"
        exit 1
else
        USER_SW_VER=$2
fi
#mkdir tuya_user/$APP_BIN_NAME/output/$USER_SW_VER
# $3 compile parameter command as user set,for example clean and so on.
USER_DEF_CMD=$3


echo ""
echo "start..."
echo ""
set -e

if [ "$USER_DEF_CMD" = "package" ];then
	echo "do package"
	#ota1
	make APP_BIN_NAME=$APP_BIN_NAME USER_SW_VER=$USER_SW_VER BUILD_TYPE=$BUILD_TYPE -C ./GCC-RELEASE
	echo "1111111111111111111111111111111111111111111111111111111111111111111"
	#ota2
	make APP_BIN_NAME=$APP_BIN_NAME USER_SW_VER=$USER_SW_VER BUILD_TYPE=$BUILD_TYPE ota_idx=2 -C ./GCC-RELEASE
	echo "2222222222222222222222222222222222222222222222222222222222222222222"
	#mp
	make APP_BIN_NAME=$APP_BIN_NAME USER_SW_VER=$USER_SW_VER BUILD_TYPE=$BUILD_TYPE mp -C ./GCC-RELEASE
	echo "3333333333333333333333333333333333333333333333333333333333333333333"
	#do package
	#BIN_PATH=tuya_user/$APP_BIN_NAME/output/$USER_SW_VER
	BIN_QIO=tuya_user/$APP_BIN_NAME/output/$USER_SW_VER/${APP_BIN_NAME}_QIO_$USER_SW_VER.bin
	BIN_DOUT=tuya_user/$APP_BIN_NAME/output/$USER_SW_VER/${APP_BIN_NAME}_DOUT_$USER_SW_VER.bin
	BIN2=GCC-RELEASE/boot_all.bin
	BIN4=tuya_user/$APP_BIN_NAME/output/$USER_SW_VER/mp_${APP_BIN_NAME}_\(1\)_$USER_SW_VER.bin
	BIN5=tuya_user/$APP_BIN_NAME/output/$USER_SW_VER/${APP_BIN_NAME}_\(2\)_$USER_SW_VER.bin
	
	./package_2M $BIN_QIO $BIN2 GCC-RELEASE/system_qio_2M.bin $BIN4 $BIN5
	echo "package_2M QIO file success"
	
	./package_2M $BIN_DOUT $BIN2 GCC-RELEASE/system_dout_2M.bin $BIN4 $BIN5
	echo "package DOUT file success"
	chmod -R 777 tuya_user/$APP_BIN_NAME/output/$USER_SW_VER/
	
	
else
	make APP_BIN_NAME=$APP_BIN_NAME USER_SW_VER=$USER_SW_VER $USER_DEF_CMD BUILD_TYPE=$BUILD_TYPE -C ./GCC-RELEASE
	if [ -z "$3" ];then
			make APP_BIN_NAME=$APP_BIN_NAME USER_SW_VER=$USER_SW_VER BUILD_TYPE=$BUILD_TYPE ota_idx=2 -C ./GCC-RELEASE
	fi
	echo ""
	echo "***************************************COMPILE SUCCESS!*******************************************"
fi


