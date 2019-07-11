/***********************************************************
*  File: condition.h
*  Author: nzy
*  Date: 20180718
***********************************************************/
#ifndef __CONDITION_H
    #define __CONDITION_H

    #include "tuya_cloud_types.h"
    #include "tuya_cloud_com_defs.h"
    #include "hashmap.h"
    #include "uni_slist.h"
    #include "scene_linkage.h"
    #include "mem_pool.h"
    #include "ty_cJSON.h"

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef  __CONDITION_GLOBALS
    #define __CONDITION_EXT
#else
    #define __CONDITION_EXT extern
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/
// normal operator
typedef BYTE_T OPERATOR_T;
#define OP_GT 1 // greater than >
#define OP_GE 2 // greater and equal >=
#define OP_EQ 3 // equal ==
#define OP_LT 4 // less than <
#define OP_LE 5 // less and equal <=

// condition trigger strategy 
typedef BYTE_T COND_TRIG_STRGY_T;
#define CTS_EDGE 1 // not trigger repeatly
#define CTS_REPEAT 2 // trigger repeatly

// condition trigger type
typedef BYTE_T COND_TRIG_TP_T;
#define CTT_DP_EXPRESSION 1 // 
#define CTT_DP 2 // trigger strategy only support repeatly
#define CTT_TIMER 3 

/* dp expression */
typedef BYTE_T COND_DP_TP_T;
#define CDT_BOOL PROP_BOOL // bool
#define CDT_VALUE PROP_VALUE // value bitmap
#define CDT_STR PROP_STR // enum string

typedef union {
    BOOL_T bl;
    INT_T val; // notice:bitmap dp maxlen <= 31
    CHAR_T *str;
}CDT_VAL_U;

typedef struct {
    CHAR_T id[DEV_ID_LEN+1]; // device id
    BYTE_T dpid;
    OPERATOR_T oper;
    COND_DP_TP_T tp;
    CDT_VAL_U fixed_val;
    CDT_VAL_U tmp_val;
}CTT_DP_EXP_S;
/* dp expression */

// only dp trigger
typedef struct {
    CHAR_T id[DEV_ID_LEN+1]; // device id
    BYTE_T dpid;
}CTT_DP_S;

/* timer trigger */
typedef BYTE_T LOOP_T; // bit6:Sunday  bit5:Mon  bit4:Tues  bit3:wed bit2:Thur bit1:Fri bit0:Sat
#define LP_SUN 1 << 6
#define LP_MON 1 << 5
#define LP_TUES 1 << 4
#define LP_WED 1 << 3
#define LP_THUR 1 << 2
#define LP_FRI 1 << 1
#define LP_SAT 1 << 0

typedef struct {
    SHORT_T year;
    BYTE_T month;
    BYTE_T day;
}TT_DATE_S;

typedef struct {
    BYTE_T hour;
    BYTE_T minute;
}TT_TIME_S;

typedef struct {
    LOOP_T lp;
    TT_DATE_S date; // if(lp == 0) then valid
    TT_TIME_S time;
}CTT_TIMER_S;
/* timer trigger */

// condition trigger type union
typedef union {
    CTT_DP_EXP_S dp_exp;
    CTT_DP_S dp;
    CTT_TIMER_S timer;
}COND_TRIGGER_U;

// operator between conditions
typedef BYTE_T COND_OPERATOR_T;
#define COP_AND 1 // && "operation prior than or"
#define COP_OR 2 // ||
#define COP_BRACKET 3 // () priorest
#define COP_INVALID 0xff // means invalid operator

typedef BYTE_T COND_TYPE_T;
#define CT_SINGLE 1 // c1
#define CT_MIX 2 // c1 && c2 && (xxx)
#define CT_BRK 3 // (c1 && c2 || c3)

typedef struct s_exep_or {
    UINT_T cond_index; // 0xffffffff invalid
    SLIST_HEAD node;
    SLIST_HEAD brk_list;
}EXEP_OR_S;

typedef struct s_exep_and {
    UINT_T cond_index; // 0xffffffff invalid
    SLIST_HEAD node;
    SLIST_HEAD brk_list;
}EXEP_AND_S;

typedef struct s_exep_bracket {
    SLIST_HEAD node;
    SLIST_HEAD or_list;
    SLIST_HEAD and_list;
}EXEP_BRACKET_S;

typedef struct s_exep {
    SLIST_HEAD or_list; // c1
    SLIST_HEAD and_list; // c1 && c2 && (xxx)
}EXEP_S;

typedef BYTE_T COND_STAT_T;
#define CS_INVALID          1
#define CS_READY_EXECUTE    2 // condition match,could execute actions
#define CS_EXECUTED         3

// condition description
struct s_cond_set;
typedef struct {
    COND_TRIG_STRGY_T strategy; // trigger strategy
    COND_TRIG_TP_T trig_tp;
    COND_TRIGGER_U trig;

    // condition status
    COND_STAT_T cond_stat; // condition status
    struct s_cond_set *cond_set; // condition set who belongs to 
}CONDITION_S;

/* condition set */
// conditions set match type
typedef BYTE_T COND_MATCH_TP_T;
#define CMT_OR 1 // any condition
#define CMT_AND 2 // all condition
#define CMT_USER_DEF 3 // user defined

// time condition attribute
typedef struct {
    BOOL_T enable;
    LOOP_T loop;
    TIME_S s_timestamp; // if(lp == 0) then valid
    TIME_S e_timestamp; // if(lp == 0) then valid
    TT_TIME_S s_time; // if(lp != 0) then valid
    TT_TIME_S e_time; // if(lp != 0) then valid
}TIME_COND_S;

// condition set
typedef struct s_cond_set {
    struct s_scene_linkage *scene_linkage;
    COND_MATCH_TP_T match_tp;
    EXEP_S *s_exep; // if(CMT_USER_DEF == match_tp) then s_exep not null
    TIME_COND_S time_cond;
    BOOL_T has_timer;
    BYTE_T cond_num;
    CONDITION_S condition[0];
}COND_SET_S;

/* condition set */

#define AND_STR '&'
#define OR_STR '|'
#define L_BRACKET_STR '('
#define R_BRACKET_STR ')'
#define KEY_WORD 'c'
#define KEY_WORD2 'C'

#define MAX_STR_DEF_TBL_SIZE 3
typedef struct {
    UINT_T size;
    CHAR_T *str[0];
}SEP_STR_TBL_S;

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
/***********************************************************
*  Function: scene_linkage_module_init
*  Input: match_type == COND_MATCH_TP_T
*  Input: when(match_type == COND_MATCH_TP_T) then 
*           exep ==”c1 && (c2 || c3)”
*  Input: root == conditions array,for example:[{c1},{c2}]
*  Output: cond_set 
*  Return: OPERATE_RET
***********************************************************/
__CONDITION_EXT \
OPERATE_RET conditions_parse(IN CONST UINT_T match_type,IN CHAR_T *exep,\
                             IN ty_cJSON *root,OUT COND_SET_S **cond_set);

OPERATE_RET conditions_parse_timeCond(IN ty_cJSON *root,INOUT COND_SET_S *p_cond_set);

/***********************************************************
*  Function: free_cond_set
*  Input: cond_set
*  Output: none 
*  Return: none
***********************************************************/
__CONDITION_EXT \
VOID free_cond_set(INOUT COND_SET_S *cond_set);

/***********************************************************
*  Function: cond_set_detect
*  Input: cond_set
*  Output: execute
*  Return: OPERATE_RET
***********************************************************/
__CONDITION_EXT \
OPERATE_RET cond_set_detect(INOUT COND_SET_S *cond_set,OUT BOOL_T *execute);


__CONDITION_EXT \
VOID conditions_dump_print(COND_SET_S *p_cond_set);

#ifdef __cplusplus
}
#endif
#endif

