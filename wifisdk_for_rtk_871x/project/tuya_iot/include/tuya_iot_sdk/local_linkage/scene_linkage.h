/***********************************************************
*  File: scene_linkage.h
*  Author: nzy
*  Date: 20180723
***********************************************************/
#ifndef __SCENE_LINKAGE_H
    #define __SCENE_LINKAGE_H

    #include "tuya_cloud_types.h"
    #include "tuya_cloud_com_defs.h"
    #include "uni_hlist.h"
    #include "action.h"
    #include "condition.h"

#ifdef __cplusplus
	extern "C" {
#endif

#ifdef  __SCENE_LINKAGE_GLOBALS
    #define __SCENE_LINKAGE_EXT
#else
    #define __SCENE_LINKAGE_EXT extern
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
*  Function: scene_linkage_module_init
*  Input: none
*  Output: none
*  Return: OPERATE_RET
*  Note:
***********************************************************/
__SCENE_LINKAGE_EXT \
OPERATE_RET scene_linkage_module_init(VOID);

/***********************************************************
*  Function: scene_linkage_module_uninit
*  Input: none
*  Output: none
*  Return: none
*  Note:
***********************************************************/
__SCENE_LINKAGE_EXT \
VOID scene_linkage_module_uninit(VOID);

__SCENE_LINKAGE_EXT \
VOID scene_linkage_update_from_server(INT_T delaymS);


/***********************************************************
*  Function: dp_condition_detect
*  Input: id
*  Input: dp_cmd
*  Output: none 
*  Return: OPERATE_RET
***********************************************************/
// Note:
// id:means gwid,if(cid == NULL) then gwid is actual cid
// dp_cmd:{"cid":"xxxxxx",dps:{"1":2,"2":"111"}} or {"devid":"xxxxxx",dps:{"1":2,"2":"111"}}
__SCENE_LINKAGE_EXT \
OPERATE_RET dp_condition_detect(IN CHAR_T *id,IN CHAR_T *dp_cmd);

__SCENE_LINKAGE_EXT \
OPERATE_RET scene_linkage_parse_and_add_2_mag(IN CONST CHAR_T *sl_str);

#ifdef __cplusplus
}
#endif
#endif

