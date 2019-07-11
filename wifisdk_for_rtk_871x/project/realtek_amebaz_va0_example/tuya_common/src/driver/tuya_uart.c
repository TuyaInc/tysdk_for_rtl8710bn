/***********************************************************
*  File: tuya_uart.c
*  Author: nzy
*  Date: 20171106
***********************************************************/
#define __TUYA_UART_GLOBALS
#include "tuya_uart.h"
#include "objects.h"
#include "serial_api.h"
#include "uni_log.h"
#include "freertos_pmu.h"
#include "uni_semaphore.h"


/***********************************************************
*************************micro define***********************
***********************************************************/
#define UART_TX PA_23 //UART0 TX
#define UART_RX PA_18 //UART0 RX 

typedef struct {
    serial_t sobj;
    UINT_T buf_len;
    BYTE_T *buf;
    USHORT_T in;
    USHORT_T out;
    BOOL_T unblock;
    SEM_HANDLE uart_sem;
    BOOL_T has_sem_get;
    int count;
}TUYA_UART_S;

extern void set_uart_lp(int port,bool enable) ; 

/***********************************************************
*************************variable define********************
***********************************************************/
STATIC TUYA_UART_S ty_uart[TY_UART_NUM];

static u32 uart0_suspend(u32 expected_idle_time )
{
	return TRUE;
}

static u32 uart0_resume(u32 expected_idle_time)
{	
	NVIC_SetPriority(UART0_IRQ, 10);
	NVIC_EnableIRQ(UART0_IRQ);
	return TRUE;
}


/***********************************************************
*************************function define********************
***********************************************************/
STATIC VOID __uart_irq(UINT_T id, SerialIrq event);

/***********************************************************
*  Function: ty_uart_init 
*  Input: port badu bits parity stop
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET ty_uart_init(IN CONST TY_UART_PORT_E port,IN CONST TY_UART_BAUD_E badu,\
                               IN CONST TY_DATA_BIT_E bits,IN CONST TY_PARITY_E parity,\
                               IN CONST TY_STOPBITS_E stop,IN CONST UINT_T bufsz,IN CONST BOOL_T unblock)
{
    if((port >= TY_UART_NUM) || (bufsz == 0)) {
        return OPRT_INVALID_PARM;
    }
    
    if(ty_uart[port].buf == NULL) {
        memset(&ty_uart[port], 0, sizeof(TUYA_UART_S));
        ty_uart[port].buf = Malloc(bufsz);
        if(ty_uart[port].buf == NULL) {
            return OPRT_MALLOC_FAILED;
        }
        ty_uart[port].buf_len = bufsz;
        PR_DEBUG("uart buf size : %d",bufsz);
    }else {
        return OPRT_COM_ERROR;
    }
   
    if(!unblock) {
        OPERATE_RET op_ret = CreateAndInitSemaphore(&(ty_uart[port].uart_sem), 0, 10);
        if(OPRT_OK != op_ret)
        {
            PR_ERR("create uart semaphore failed");
            return op_ret;
        }

        set_uart_lp(port,TRUE);
    }else {
        set_uart_lp(port,FALSE);
    }
    
    serial_init(&ty_uart[port].sobj,UART_TX,UART_RX);
    serial_baud(&ty_uart[port].sobj,badu);

    INT_T data_bit = 0;
    if(bits == TYWL_5B) {
        data_bit = 5;
    }else if(bits == TYWL_6B) {
        data_bit = 6;
    }else if(bits == TYWL_7B) {
        data_bit = 7;
    }else {
        data_bit = 8;
    }
    serial_format(&ty_uart[port].sobj,data_bit,parity,stop);
    
    serial_irq_handler(&ty_uart[port].sobj, __uart_irq, (uint32_t)&ty_uart[port].sobj);
    serial_irq_set(&ty_uart[port].sobj, RxIrq, 1);

    if((!unblock) && tuya_get_lp_mode()) {
        pmu_register_sleep_callback(PMU_UART0_DEVICE, (PSM_HOOK_FUN)uart0_suspend, (void*)NULL, (PSM_HOOK_FUN)uart0_resume, (void*)NULL);
    }
    
    ty_uart[port].in = 0;
    ty_uart[port].out = 0;
    ty_uart[port].unblock = unblock;
    ty_uart[port].has_sem_get = TRUE;
    ty_uart[port].count = 0;

    PR_NOTICE("%d   %d",unblock,badu);

    return OPRT_OK;
}

/***********************************************************
*  Function: ty_uart_free 
*  Input:free uart
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET ty_uart_free(IN CONST TY_UART_PORT_E port)

{
    if(port >= TY_UART_NUM) {
       return OPRT_INVALID_PARM;
    }

    if((!ty_uart[port].unblock) && tuya_get_lp_mode()) {
        pmu_unregister_sleep_callback(PMU_UART0_DEVICE);
    }
    
    serial_free(&ty_uart[port].sobj);
    if(ty_uart[port].buf != NULL) {
        Free(ty_uart[port].buf);
        ty_uart[port].buf = NULL;
    }
    ty_uart[port].buf_len = 0;
    
    if(!ty_uart[port].unblock) {
        ReleaseSemaphore(ty_uart[port].uart_sem);
    }

    ty_uart[port].unblock = FALSE;
    
    

   return OPRT_OK;
}

/***********************************************************
*  Function: ty_uart_send_data 
*  Input: port data len
*  Output: none
*  Return: none
***********************************************************/
VOID ty_uart_send_data(IN CONST TY_UART_PORT_E port,IN CONST BYTE_T *data,IN CONST UINT_T len)
{
    if(port >= TY_UART_NUM) {
        return;
    }

    UINT_T i = 0;
    for(i = 0;i < len;i++) {
       serial_putc(&ty_uart[port].sobj, *(data+i));
    }
}

STATIC UINT __ty_uart_read_data_size(IN CONST TY_UART_PORT_E port)
{
    UINT_T remain_buf_size = 0;

    if(ty_uart[port].in >= ty_uart[port].out) {
        remain_buf_size = ty_uart[port].in-ty_uart[port].out;
    }else {
        remain_buf_size = ty_uart[port].in + ty_uart[port].buf_len - ty_uart[port].out;
    }

    return remain_buf_size;
}


STATIC VOID __uart_irq(UINT_T id, SerialIrq event)
{
    serial_t *sobj = (void*)id;
    INT_T rc = 0;
    
    INT_T i = 0;
    for(i = 0;i < TY_UART_NUM;i++) {
        if(&ty_uart[i].sobj == sobj) {
            break;
        }
    }
    
    if(event == RxIrq) {
        rc = serial_getc(sobj);

        if(__ty_uart_read_data_size(i) < ty_uart[i].buf_len - 1) {
            ty_uart[i].buf[ty_uart[i].in++] = rc;
            if(ty_uart[i].in >= ty_uart[i].buf_len){
                ty_uart[i].in = 0;
            }
            
            if(!ty_uart[i].unblock) {
                if(ty_uart[i].has_sem_get) {
                    ty_uart[i].has_sem_get = FALSE;
                    PostSemaphore(ty_uart[i].uart_sem);
                    ty_uart[i].count++;
                }
            }
            
        }
    }
}


/***********************************************************
*  Function: ty_uart_send_data 
*  Input: len->data buf len
*  Output: buf->read data buf
*  Return: actual read data size
***********************************************************/
UINT_T ty_uart_read_data(IN CONST TY_UART_PORT_E port,OUT BYTE_T *buf,IN CONST UINT_T len)
{
    if(NULL == buf || 0 == len) {
        return 0;
    }
    
    if(!ty_uart[port].unblock) {
        OPERATE_RET op_ret = WaitSemaphore(ty_uart[port].uart_sem);
        if(OPRT_OK != op_ret) {
            PR_ERR("WaitSemaphore failed");
            return op_ret;
        }
        
        //PR_DEBUG_RAW("\r\n xx %d\r\n",ty_uart[port].count);
        ty_uart[port].count = 0;
        ty_uart[port].has_sem_get = TRUE;
    }

    UINT_T actual_size = 0;
    UINT_T cur_num = __ty_uart_read_data_size(port);
    if(cur_num > ty_uart[port].buf_len - 1) {
        PR_DEBUG("uart fifo is full! buf_zize:%d  len:%d",cur_num,len);
    }

    if(len > cur_num) {
        actual_size = cur_num;
    }else {
        actual_size = len;
    }
    //PR_NOTICE("uart_num = %d", cur_num);
    UINT_T i = 0;
    for(i = 0;i < actual_size;i++) {
        *buf++ = ty_uart[port].buf[ty_uart[port].out++];
        if(ty_uart[port].out >= ty_uart[port].buf_len) {
            ty_uart[port].out = 0;
        }
    }

    return actual_size;
}



