#include "cd_timer_if.h"

#define TIME_CYCLE_MS	(1000)

typedef struct {
	BOOL_T 	is_start;
    UINT_T	cd_time;
    UINT_T	run_time;
}CD_TIMER_CTL_S;

typedef struct {
    TIMER_ID cd_timer;
    UCHAR_T timer_cnt;
    UCHAR_T cycle_sec;
	CdTimeCB fun_cb;
	CD_TIMER_CTL_S* p_cd_timer_tbl;
}CD_TIMER_MANAGE_S;


STATIC VOID cd_timer_cb(IN UINT timerID,IN CONST PVOID pTimerArg)
{
	UCHAR_T i = 0;
	UINT_T remain_sec = 0;
    OPERATE_RET op_ret = OPRT_OK;
    CD_TIMER_MANAGE_S *mag_handle = (CD_TIMER_MANAGE_S*)pTimerArg;
    CD_TIMER_CTL_S *p_tbl = NULL;
    	
	if (NULL == mag_handle) {
		PR_ERR("OPRT_INVALID_PARM");
		return;
	}
	
	p_tbl = (CD_TIMER_CTL_S*)mag_handle->p_cd_timer_tbl;
	if (NULL == p_tbl) {
		PR_ERR("OPRT_INVALID_PARM");
		return;
	}
	
	for (i = 0;i < mag_handle->timer_cnt; i++) {
		if (TRUE == p_tbl[i].is_start) {
			p_tbl[i].run_time++;
			if (p_tbl[i].cd_time > p_tbl[i].run_time) {
				if (p_tbl[i].run_time % mag_handle->cycle_sec == 0) {
					remain_sec = p_tbl[i].cd_time - p_tbl[i].run_time;
					if (NULL != mag_handle->fun_cb) {
						mag_handle->fun_cb(i, remain_sec);
					}
				}
			} else {
				if (NULL != mag_handle->fun_cb) {
					mag_handle->fun_cb(i, 0);
				}
				p_tbl[i].is_start = FALSE;
			}
		}
	}
	
	op_ret = sys_start_timer(timerID, TIME_CYCLE_MS, TIMER_ONCE);
    if (OPRT_OK != op_ret) {
    	PR_DEBUG("sys_add_timer err:%d", op_ret);
	}
	
}

OPERATE_RET cd_time_init(INOUT CD_TIME_HANDLE* handle, IN UCHAR_T ch_num, IN UCHAR_T cycle_sec, IN CONST CdTimeCB fun_cb)
{
    OPERATE_RET op_ret = OPRT_OK;
    CD_TIMER_MANAGE_S *mag_handle = NULL;
    CD_TIMER_CTL_S *p_tbl = NULL;
    
	if ((NULL == fun_cb)\
	  ||(NULL == handle)\
	  ||(0 == ch_num)\
	  ||(0 == cycle_sec)) {
		PR_ERR("OPRT_INVALID_PARM");
		return OPRT_INVALID_PARM;
	}

	*handle = NULL;

	mag_handle = (CD_TIMER_MANAGE_S*)Malloc(SIZEOF(CD_TIMER_MANAGE_S));
	if (NULL == mag_handle) {
		PR_ERR("mag_handle is null");
		return OPRT_MALLOC_FAILED;
	}	
	memset((VOID*)mag_handle, 0 ,SIZEOF(CD_TIMER_MANAGE_S));

	p_tbl = (CD_TIMER_CTL_S*)mag_handle->p_cd_timer_tbl;
	p_tbl = (CD_TIMER_CTL_S*)Malloc(ch_num*SIZEOF(CD_TIMER_CTL_S));	
	if (NULL == p_tbl) {
		Free(mag_handle);
		PR_ERR("p_cd_timer_tbl is null");
		return OPRT_MALLOC_FAILED;
	}	
	memset((VOID*)p_tbl, 0, SIZEOF(CD_TIMER_CTL_S));
	
    mag_handle->timer_cnt = ch_num;
    mag_handle->cycle_sec = cycle_sec;
	mag_handle->fun_cb = fun_cb;
	mag_handle->p_cd_timer_tbl = p_tbl;

    op_ret = sys_add_timer(cd_timer_cb, (PVOID)mag_handle, &mag_handle->cd_timer);
    if (OPRT_OK != op_ret) {
    	PR_DEBUG("sys_add_timer err:%d", op_ret);
    	Free(mag_handle->p_cd_timer_tbl);
    	Free(mag_handle);
        return op_ret;
    } else {
        op_ret = sys_start_timer(mag_handle->cd_timer, TIME_CYCLE_MS, TIMER_ONCE);
        if (OPRT_OK != op_ret) {
			Free(mag_handle->p_cd_timer_tbl);
			Free(mag_handle);
	    	PR_DEBUG("sys_add_timer err:%d", op_ret);
	        return op_ret;
    	}
    }
    
	*handle =  (CD_TIME_HANDLE)mag_handle;
	return OPRT_OK;
}

VOID cd_time_deinit(INOUT CD_TIME_HANDLE* handle)
{
	CD_TIMER_MANAGE_S *mag_handle = (CD_TIMER_MANAGE_S*)(*handle);
    CD_TIMER_CTL_S *p_tbl = NULL;
    
	if (NULL == mag_handle) {
		PR_ERR("OPRT_INVALID_PARM");
		return;
	}
	
	p_tbl = (CD_TIMER_CTL_S*)mag_handle->p_cd_timer_tbl;
	if (NULL != p_tbl) {
		Free(p_tbl);
		p_tbl = NULL;
	}
	
	Free(mag_handle);
	mag_handle = NULL;
}

OPERATE_RET cd_time_start(IN CONST CD_TIME_HANDLE handle, IN UCHAR_T ch_idx, IN UINT_T cd_time)
{	
	CD_TIMER_MANAGE_S *mag_handle = (CD_TIMER_MANAGE_S*)(handle);
    CD_TIMER_CTL_S *p_tbl = NULL;

	if (NULL == mag_handle) {
		PR_ERR("OPRT_INVALID_PARM");
		return OPRT_INVALID_PARM;
	}

	p_tbl = (CD_TIMER_CTL_S*)mag_handle->p_cd_timer_tbl;
	if (NULL == p_tbl) {
		PR_ERR("OPRT_INVALID_PARM");
		return OPRT_INVALID_PARM;
	}

	if ((ch_idx >= mag_handle->timer_cnt)
	||(cd_time == 0)) {
		PR_ERR("cd_time_start err:%d", OPRT_INVALID_PARM);
		return OPRT_INVALID_PARM;
	}
	
	p_tbl[ch_idx].cd_time = cd_time;
	p_tbl[ch_idx].run_time = 0;
	p_tbl[ch_idx].is_start = TRUE;
	return OPRT_OK;
}

OPERATE_RET  cd_time_stop(IN CONST CD_TIME_HANDLE handle, IN UCHAR_T ch_idx)
{
	CD_TIMER_MANAGE_S *mag_handle = (CD_TIMER_MANAGE_S*)(handle);
    CD_TIMER_CTL_S *p_tbl = NULL;

	if (NULL == mag_handle) {
		PR_ERR("OPRT_INVALID_PARM");
		return OPRT_INVALID_PARM;
	}

	p_tbl = (CD_TIMER_CTL_S*)mag_handle->p_cd_timer_tbl;
	if (NULL == p_tbl) {
		PR_ERR("OPRT_COM_ERROR");
		return OPRT_INVALID_PARM;
	}

	if (ch_idx >= mag_handle->timer_cnt) {
		PR_ERR("cd_time_stop err:%d", OPRT_INVALID_PARM);
		return OPRT_INVALID_PARM;
	}
	
	p_tbl[ch_idx].cd_time = 0;
	p_tbl[ch_idx].run_time = 0;
	p_tbl[ch_idx].is_start = FALSE;

	return OPRT_OK;
}

OPERATE_RET  cd_time_all_stop(IN CONST CD_TIME_HANDLE handle)
{
	UCHAR_T i = 0;
	CD_TIMER_MANAGE_S *mag_handle = (CD_TIMER_MANAGE_S*)(handle);
    CD_TIMER_CTL_S *p_tbl = NULL;

	if (NULL == mag_handle) {
		PR_ERR("OPRT_INVALID_PARM");
		return OPRT_INVALID_PARM;
	}

	p_tbl = (CD_TIMER_CTL_S*)mag_handle->p_cd_timer_tbl;
	if (NULL == p_tbl) {
		PR_ERR("OPRT_COM_ERROR");
		return OPRT_INVALID_PARM;
	}
	
	for (i = 0;i < mag_handle->timer_cnt;i++) {
		p_tbl[i].cd_time = 0;
		p_tbl[i].run_time = 0;
		p_tbl[i].is_start = FALSE;		
	}
	return OPRT_OK;
}

UINT_T  cd_time_get_remain_sec(IN CONST CD_TIME_HANDLE handle, IN UCHAR_T            ch_idx)
{
	UINT_T remain_sec = 0;
	CD_TIMER_MANAGE_S *mag_handle = (CD_TIMER_MANAGE_S*)(handle);
    CD_TIMER_CTL_S *p_tbl = NULL;

	if (NULL == mag_handle) {
		PR_ERR("OPRT_INVALID_PARM");
		return 0;
	}

	p_tbl = (CD_TIMER_CTL_S*)mag_handle->p_cd_timer_tbl;
	if (NULL == p_tbl) {
		PR_ERR("OPRT_INVALID_PARM");
		return 0;
	}
	
	if (ch_idx >= mag_handle->timer_cnt) {
		PR_ERR("cd_get_remain_sec err:%d,ch_idx:%d,timer_cnt:%d ", OPRT_INVALID_PARM, ch_idx, mag_handle->timer_cnt);
		return 0;
	}

	if (FALSE == p_tbl[ch_idx].is_start) {
		return 0;
	}
	
	if(p_tbl[ch_idx].cd_time > p_tbl[ch_idx].run_time){
		remain_sec = p_tbl[ch_idx].cd_time - p_tbl[ch_idx].run_time;
	}
	return remain_sec;
}



