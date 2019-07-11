/***********************************************************
*  File: wf_nw_cfg.h 
*  Author: nzy
*  Date: 20170220
***********************************************************/
#ifndef _WF_NW_CFG_H
    #define _WF_NW_CFG_H

    #include "tuya_cloud_types.h"
    #include "wl_cfg_com.h"

#ifdef __cplusplus
	extern "C" {
#endif

#ifdef  __WF_NW_CFG_GLOBALS
    #define __WF_NW_CFG_EXT
#else
    #define __WF_NW_CFG_EXT extern
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
/***********************************************************
*  Function: wf_nw_cfg_start
*  Input: cb
*  Output: none 
*  Return: OPERATE_RET
***********************************************************/
__WF_NW_CFG_EXT \
OPERATE_RET wf_nw_cfg_start(NW_CFG_FIN_CALLBACK cb);

/***********************************************************
*  Function: wf_nw_cfg_stop
*  Input: none
*  Output: none 
*  Return: OPERATE_RET
***********************************************************/
__WF_NW_CFG_EXT \
VOID wf_nw_cfg_stop(VOID);


#ifdef __cplusplus
}
#endif
#endif

