/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"
#include "main.h"

static USOC_TX_BD usocTXBD[USOC_TXBD_RING_SIZE];
static USOC_RX_BD usocRXBD[USOC_RXBD_RING_SIZE];
u8 HeaderPattern[58] = {0x59,0x05,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x88,0x01,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x01,0x10,0x00,0x06,0x00,0x3f,0x05,0x00,0x04,0x00,0x00,0x01,0x00}; 

void pkt_one_gen(u16 seqno, u16 pktlen, u8 seq)
{
	int i;
        u32 rxbd_sw_idx = USOC_RXBD_SWIDX_Get(USOC_REG);
        u8 payload[USOC_RX_BUFFER_SIZE];
        
        for( i = 0; i < 58; i++)
          payload[i] = HeaderPattern[i];
	if(seq == 1) {
		for( i = 58; i < pktlen; i++) {
			payload[i] = (u8) i;
		}
	} else {
		for( i = 58; i < pktlen; i++) {
			payload[i] = 0x3E;
		}
	}
        if(usocRXBD[rxbd_sw_idx].status == RXBD_STAT_INIT) {
          usocRXBD[rxbd_sw_idx].address = (u32)payload;
          usocRXBD[rxbd_sw_idx].pktSize = pktlen;
          usocRXBD[rxbd_sw_idx].seqNum = seqno;
          usocRXBD[rxbd_sw_idx].status = RXBD_STAT_RDY;
          rxbd_sw_idx++;
          rxbd_sw_idx = rxbd_sw_idx%USOC_RXBD_RING_SIZE;        
        }
        USOC_RXBD_SWIDX_Cfg(USOC_REG, rxbd_sw_idx);
}
void usoc_rxbd_irq_handle()
{
	USOC_RX_BD* pRxbd = NULL;
	int i;

	/* change rxbd status when hw done */
	for( i = 0; i < USOC_RXBD_RING_SIZE; i++) {
		pRxbd = &usocRXBD[i];
		
		if(pRxbd->status == RXBD_STAT_OK ){
			pRxbd->status = RXBD_STAT_INIT;
		}
	}
}

void usoc_txbd_irq_handle()
{
    USOC_TX_BD* pTxbd = NULL;
    u32 txbd_sw_idx = USOC_TXBD_SWIDX_Get(USOC_REG);
    int ret = SUCCESS;

    pTxbd = &usocTXBD[txbd_sw_idx];
    
    /* When TX has no data and RX is full quit loop */ 
    while(pTxbd->status == TXBD_STAT_OK) {
      DBG_8195A("tx bd seqno:%d addr:%x size:%d stat:%x\n", pTxbd->seqNum, pTxbd->address, pTxbd->pktSize, pTxbd->status);
            memset(pTxbd->address, 0, USOC_TX_BUFFER_SIZE);
            pTxbd->status = TXBD_STAT_RDY;		
            
            txbd_sw_idx++;
            txbd_sw_idx = txbd_sw_idx%USOC_TXBD_RING_SIZE;
            pTxbd = &usocTXBD[txbd_sw_idx];
    }
    /* Update idx */
    USOC_TXBD_SWIDX_Cfg(USOC_REG, txbd_sw_idx);
   // pkt_one_gen(1, USOC_RX_BUFFER_SIZE, 0);
}

void usoc_isr_handle(void *DATA)
{
	volatile u32 intr_stat = 0;
	intr_stat = USOC_INTGet(USOC_REG);

		if( intr_stat &  BIT_INTR_RX_PKT_OK) {
			DBG_8195A(">>>>>>RX_PKT_OK interrupt>>>>>>\n");		
			usoc_rxbd_irq_handle();
			
			/* Clear RX_PKT_OK */
			USOC_INTClr(USOC_REG, BIT_INTR_RX_PKT_OK);
		}

		if(intr_stat &  BIT_INTR_TX_PKT_OK) {
			DBG_8195A(">>>>>>TX_PKT_OK interrupt>>>>>>\n");
			usoc_txbd_irq_handle();
		
			/* Clear TX_PKT_OK */ 
			USOC_INTClr(USOC_REG, BIT_INTR_TX_PKT_OK);
		}

		if( intr_stat &  BIT_INTR_NO_TX_BD ) {
			USOC_INTClr(USOC_REG, BIT_INTR_NO_TX_BD);
			usoc_txbd_irq_handle();
		}
		
		if(intr_stat &  ~(BIT_INTR_TX_PKT_OK | BIT_INTR_RX_PKT_OK | BIT_INTR_NO_TX_BD) )  {
			DBG_8195A("Error!Interrupt!!!\n");
			DBG_8195A("Interrupt status : %x\n", intr_stat);	
		}

		/* Enable the interrupt */
		USOC_INTCfg(USOC_REG, USOC_INIT_INTR_MASK);
}

int usoc_txbd_init(void)
{
	u32 i;
	
	/*init txbd */
	for( i=0 ; i<USOC_TXBD_RING_SIZE ; i++ )
	{	
		/* For convinient just 4 bytes align */
		usocTXBD[i].address = (u32)_rtw_zmalloc(USOC_TX_BUFFER_SIZE);
		if(!usocTXBD[i].address) {
			DBG_8195A("Malloc fail : TXBD[%d] \n",i);
			usocTXBD[i].status = TXBD_STAT_INIT;
			return -1;
		}
		else {
			/* set status to ready */
			usocTXBD[i].status = TXBD_STAT_RDY;
		}
	}
	
	return 0;
}

int usoc_rxbd_init(void)
{
	u32 i;
	
	/*init rxbd */
	for( i=0 ; i<USOC_RXBD_RING_SIZE ; i++ )
	{	
		/* For convinient just 4 bytes align */
		usocRXBD[i].address = (u32)_rtw_zmalloc(USOC_RX_BUFFER_SIZE);
		if(!usocRXBD[i].address) {
			DBG_8195A("Malloc fail : RXBD[%d] \n",i);
			usocRXBD[i].status = RXBD_STAT_INIT;
			return -1;
		}
		else {
			usocRXBD[i].status = RXBD_STAT_INIT;
		}
	}
	
	return 0;
}

void usb_device(void)
{
	int ret;
	USOC_InitTypeDef USOCInit_Struct;
  
	/* usoc init */
	RCC_PeriphClockCmd(APBPeriph_OTG, APBPeriph_OTG_CLOCK, DISABLE);
	USOC_POWER_On();
	
	/* Register the ISR */
	InterruptRegister((IRQ_FUN) usoc_isr_handle, USB_IRQ, NULL, 10);
	InterruptEn(USB_IRQ, 10);
		
	ret = usoc_txbd_init();
	if(ret != 0)
		goto fail;

	ret = usoc_rxbd_init();
	if(ret != 0)
		goto fail;

	/* disable usoc first */
	USOC_Cmd(USOC_REG, DISABLE);

	/* init SIE control register */
	USOC_MODE_Cfg(USOC_REG, USB_INIC_MODE);
	
	USOC_StructInit(&USOCInit_Struct);
	USOCInit_Struct.TXBD_BAR = (u32)usocTXBD; /* init txbd_bar */
	USOCInit_Struct.RXBD_BAR = (u32)usocRXBD; /* init rxbd_bar reg */
	USOC_Init(USOC_REG, &USOCInit_Struct);
	/* disable usoc mitigation function */
	USOC_MIT_Cfg(USOC_REG, DISABLE);
	
	/* Enable the interrupt */
	USOC_INTCfg(USOC_REG, USOC_INIT_INTR_MASK);
	HAL_WRITE8(SIE_REG_BASE, 0x5A, 1);
	HAL_WRITE8(SIE_REG_BASE, 0xF8, 0x80);
	/* enable usoc after init TXBD/RXBD */
	USOC_Cmd(USOC_REG, ENABLE);
	DelayUs(100);
        pkt_one_gen(1, USOC_RX_BUFFER_SIZE, 0);
fail:
	vTaskDelete(NULL);
}


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
	if(xTaskCreate( (TaskFunction_t)usb_device, "USB DEVICE DEMO", (2048/4), NULL, (tskIDLE_PRIORITY + 1), NULL)!= pdPASS) {
			DBG_8195A("Cannot create usb device demo task\n\r");
			goto end_demo;
	}

	vTaskStartScheduler();
end_demo:
	while(1);
}


