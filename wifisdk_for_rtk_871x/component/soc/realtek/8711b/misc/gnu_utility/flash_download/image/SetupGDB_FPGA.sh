#!/bin/sh

#===============================================================================
CURRENT_UTILITY_DIR=$(pwd)
FLASHDLIMGDIR=$1
IMAGE_TARGET_FOLDER=$CURRENT_UTILITY_DIR/image
echo "..."
echo $CURRENT_UTILITY_DIR
echo $IMAGE_TARGET_FOLDER
GDBSCPTFILE="$FLASHDLIMGDIR/rtl_gdb_flash_write.txt" 

#===============================================================================
#set image2 code size
FLASHFILENAME="$IMAGE_TARGET_FOLDER/image2_all_ota1.bin" 
FLASH_FILE_SIZE=$(stat -c %s $FLASHFILENAME)
sed -i "s#^set \$FlashFileSize =.*#set \$FlashFileSize =$FLASH_FILE_SIZE#g"  $GDBSCPTFILE

#set boot code size
FLASHFILENAME="$IMAGE_TARGET_FOLDER/boot_all.bin" 
FLASH_FILE_SIZE=$(stat -c %s $FLASHFILENAME)
sed -i "s#^set \$XIPBootSize =.*#set \$XIPBootSize =$FLASH_FILE_SIZE#g"  $GDBSCPTFILE

#set system data size
FLASHFILENAME="$IMAGE_TARGET_FOLDER/system.bin" 
FLASH_FILE_SIZE=$(stat -c %s $FLASHFILENAME)
sed -i "s#^set \$SysDataSize =.*#set \$SysDataSize =$FLASH_FILE_SIZE#g"  $GDBSCPTFILE

#OTA2 size
FLASHFILENAME="$IMAGE_TARGET_FOLDER/image2_all_ota2.bin" 
FLASH_FILE_SIZE=$(stat -c %s $FLASHFILENAME)
sed -i "s#^set \$OTA2Size =.*#set \$OTA2Size =$FLASH_FILE_SIZE#g"  $GDBSCPTFILE
