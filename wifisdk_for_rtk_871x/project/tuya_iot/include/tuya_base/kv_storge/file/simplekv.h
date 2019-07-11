#ifndef _SIMPLEKV_H_
#define _SIMPLEKV_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "adapter_platform.h"

#define MAX_KEY_LEN 999
#define MAX_DATA_LEN 9999


INT_T simplekv_init(IN CONST CHAR_T *pPath, IN CONST BYTE_T *p_encrypt_key, IN CONST UINT_T encrypt_key_len);

INT_T simplekv_deinit(void);

INT_T simplekv_write(const CHAR_T *pKey, const BYTE_T *pData, UINT_T dataLen);

INT_T simplekv_read(const CHAR_T *pKey, BYTE_T **ppData, UINT_T *pDataLen);

INT_T simplekv_delete(const CHAR_T *pKey);

INT_T simplekv_free_data(void *pData);

INT_T simplekv_fuzz_read(const CHAR_T *pKey, UINT_T *index, BYTE_T **ppData, UINT_T *pDataLen);

INT_T simplekv_fuzz_delete(const CHAR_T *pKey);

INT_T simplekv_format(void);

INT_T simplekv_show_status(void);

INT_T simplekv_factory(void);



#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /*_SIMPLEKV_H_*/
