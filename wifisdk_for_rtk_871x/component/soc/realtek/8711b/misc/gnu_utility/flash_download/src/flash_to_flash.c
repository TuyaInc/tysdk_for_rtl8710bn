#include "rtl8710b.h"
#include "build_info.h"


#define INH_PIN	_PA_0
#define CTRL_PIN	_PA_5

#define CHN_Y1	0
#define CHN_Y2	1

#define FLASHDATALEN		128*1024
BOOT_RAM_BSS_SECTION volatile u8*  FlashDataBuf;

/* channel : the parameter can be CHN_Y1 or CHN_Y2 */
BOOT_RAM_TEXT_SECTION
static VOID SPDT_Switch(u32 channel)
{
	GPIO_WriteBit(CTRL_PIN, channel);
}


BOOT_RAM_TEXT_SECTION
static VOID SPDT_Initial_Setup(VOID)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/* INH pin: enables or disables the switch. 0: Enable, 1: Disable */
	GPIO_InitStruct.GPIO_Pin = INH_PIN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(&GPIO_InitStruct);
	/* Enable the switch */
	GPIO_WriteBit(INH_PIN, 0);

	/* CTRL pin: controls the switch. 0: Y1 channel is on, 1: Y2 channel is on */
	GPIO_InitStruct.GPIO_Pin = CTRL_PIN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(&GPIO_InitStruct);
	/* Default on-channel is Y1 */
	SPDT_Switch(CHN_Y1);

}

BOOT_RAM_TEXT_SECTION
VOID BOOT_RAM_InitFlash(void)
{
	u32 Value32;

	/* disable cache first  */
	Cache_Enable(DISABLE); /* cahce enable becuase flash init will flush cahce */
	RCC_PeriphClockCmd(APBPeriph_FLASH, APBPeriph_FLASH_CLOCK, DISABLE);

	/* set 500MHz Div to gen spic clock 200MHz */
	FLASH_ClockDiv(FLASH_CLK_DIV2P5);
	RCC_PeriphClockCmd(APBPeriph_FLASH, APBPeriph_FLASH_CLOCK, ENABLE);
	
	PinCtrl(PERIPHERAL_SPI_FLASH, S0, ON);
	FLASH_StructInit(&flash_init_para);
	flash_init_para.FLASH_cur_cmd = FLASH_CMD_READ;
	flash_init_para.FLASH_baud_rate = 1;
	flash_init_para.FLASH_baud_boot = 1;
	FLASH_Init(SpicOneBitMode);

	DBG_8195A("Init flash baudrate: %d\n", flash_init_para.FLASH_baud_rate);

	/* enable cache before XIP */
	Cache_Enable(ENABLE);
	Cache_Flush();
}

BOOT_RAM_TEXT_SECTION
VOID BOOT_RAM_FlashtoFlash(VOID)
{
	u32 addr = 0;
	u32 image_size = 512; 		 //test size is 512KB
	u32 tail_size;
	u32 times = image_size / (FLASHDATALEN / 1024);
	int i = 0, j = 0;

	BOOT_RAM_InitFlash();
	DBG_8195A("Flash init done!\n");
	
	FlashDataBuf = __image2_entry_func__;
	
	DBG_8195A("Start copy data from source flash to destination flash...\n");
	SPDT_Initial_Setup();

	/* Erase Destination flash */
	SPDT_Switch(CHN_Y2);
	FLASH_Erase(EraseChip, 0);
	DBG_8195A("Erase Destination flash done!\n");

	for(; i < times; i++){
		/* SPDT switch to Source flash */
		SPDT_Switch(CHN_Y1);

		_memcpy(FlashDataBuf, SPI_FLASH_BASE + addr, FLASHDATALEN);
		
		/* SPDT switch to Destination flash */
		SPDT_Switch(CHN_Y2);

		for(j = 0; j < FLASHDATALEN; j += 8){
			FLASH_TxData12B(addr + j, 8, FlashDataBuf + j);
		}
		
		addr += FLASHDATALEN;
		DBG_8195A("128KB copied done!\n");

	}

	tail_size = image_size % (FLASHDATALEN / 1024);
	if(tail_size != 0){
		/* SPDT switch to Source flash */
		SPDT_Switch(CHN_Y1);

		_memcpy(FlashDataBuf, SPI_FLASH_BASE + addr, tail_size * 1024);
		
		/* SPDT switch to Destination flash */
		SPDT_Switch(CHN_Y2);

		for(j = 0; j < tail_size * 1024; j += 4){
			FLASH_TxData12B(addr + j, 4, FlashDataBuf + j);
		}

		addr += tail_size * 1024;
		DBG_8195A("%d KB done!\n", tail_size);
	}

	DBG_8195A("Flash to flash done!\n");
	while(1){}

}


IMAGE1_ENTRY_SECTION
RAM_FUNCTION_START_TABLE RamStartTable = {
	.RamStartFun = BOOT_RAM_FlashtoFlash,
	.RamWakeupFun = NULL, /* init in socps SOCPS_InitSYSIRQ */
	.RamPatchFun0 = BOOT_RAM_FlashtoFlash,
	.RamPatchFun1 = BOOT_RAM_FlashtoFlash,
	.RamPatchFun2 = BOOT_RAM_FlashtoFlash,
	.FlashStartFun = NULL
};

