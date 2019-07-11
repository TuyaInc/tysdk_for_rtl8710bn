/*
aes_inf.h
Copyright(C),2018-2020, 涂鸦科技 www.tuya.comm
*/
#ifndef _AES_INF_H
#define _AES_INF_H

#include "adapter_platform.h"
#include "tuya_cloud_error_code.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef VOID (*AES128_ECB_ENC_BUF)(IN CONST BYTE_T *input, IN CONST UINT_T length, IN CONST BYTE_T *key,OUT BYTE_T *output);
typedef VOID (*AES128_ECB_DEC_BUF)(IN CONST BYTE_T *input, IN CONST UINT_T length, IN CONST BYTE_T *key,OUT BYTE_T *output);

typedef VOID (*AES128_CBC_ENC_BUF)(IN CONST BYTE_T *input, IN CONST UINT_T length, IN CONST BYTE_T *key, IN BYTE_T *iv, OUT BYTE_T *output);
typedef VOID (*AES128_CBC_DEC_BUF)(IN CONST BYTE_T *input, IN CONST UINT_T length, IN CONST BYTE_T *key, IN BYTE_T *iv, OUT BYTE_T *output);

typedef struct {
    AES128_ECB_ENC_BUF ecb_enc_128;
    AES128_ECB_DEC_BUF ecb_dec_128;
    AES128_CBC_ENC_BUF cbc_enc_128;
    AES128_CBC_DEC_BUF cbc_dec_128;
}AES_METHOD_REG_S;

#define AES128_ENCRYPT_KEY_LEN 16


UINT_T aes_pkcs7padding_buffer(BYTE_T *p_buffer, UINT_T length);

OPERATE_RET aes_method_register(IN CONST AES_METHOD_REG_S *aes);

VOID aes_method_unregister(VOID);

OPERATE_RET aes192_cbc_encode(IN CONST BYTE_T *data,IN CONST UINT_T len,\
                            IN CONST BYTE_T *key,IN BYTE_T *iv,\
                            OUT BYTE_T **ec_data,OUT UINT_T *ec_len);
OPERATE_RET aes192_cbc_decode(IN CONST BYTE_T *data,IN CONST UINT_T len,\
                            IN CONST BYTE_T *key,IN BYTE_T *iv,\
                            OUT BYTE_T **dec_data,OUT UINT_T *dec_len);


OPERATE_RET aes128_ecb_encode(IN CONST BYTE_T *data,IN CONST UINT_T len,\
                              OUT BYTE_T **ec_data,OUT UINT_T *ec_len,\
                              IN CONST BYTE_T *key);
OPERATE_RET aes128_ecb_decode(IN CONST BYTE_T *data,IN CONST UINT_T len,\
                              OUT BYTE_T **dec_data,OUT UINT_T *dec_len,\
                              IN CONST BYTE_T *key);
OPERATE_RET aes128_cbc_encode(IN CONST BYTE_T *data,IN CONST UINT_T len,\
                              IN CONST BYTE_T *key,IN BYTE_T *iv,\
                              OUT BYTE_T **ec_data,OUT UINT_T *ec_len);
OPERATE_RET aes128_cbc_decode(IN CONST BYTE_T *data,IN CONST UINT_T len,\
                              IN CONST BYTE_T *key,IN BYTE_T *iv,\
                              OUT BYTE_T **dec_data,OUT UINT_T *dec_len);
OPERATE_RET aes_free_data(IN BYTE_T *data);
INT_T aes_get_actual_length(IN CONST BYTE_T *dec_data,IN CONST UINT_T dec_data_len);


OPERATE_RET aes192_cbc_encode_raw(IN CONST BYTE_T *data,IN CONST UINT_T len,\
                                  IN CONST BYTE_T *key,IN BYTE_T *iv,\
                                  OUT BYTE_T *ec_data);
OPERATE_RET aes192_cbc_decode_raw(IN CONST BYTE_T *data,IN CONST UINT_T len,\
                                  IN CONST BYTE_T *key,IN BYTE_T *iv,\
                                  OUT BYTE_T *dec_data);

OPERATE_RET aes256_cbc_encode_raw(IN CONST BYTE_T *data,IN CONST UINT_T len,\
                                  IN CONST BYTE_T *key,IN BYTE_T *iv,\
                                  OUT BYTE_T *ec_data);


OPERATE_RET aes128_ecb_encode_raw(IN CONST BYTE_T *data, IN CONST UINT_T len,\
                                  OUT BYTE_T *ec_data,IN CONST BYTE_T *key);
OPERATE_RET aes128_ecb_decode_raw(IN CONST BYTE_T *data, IN CONST UINT_T len,\
                                  OUT BYTE_T *dec_data,IN CONST BYTE_T *key);
OPERATE_RET aes128_cbc_encode_raw(IN CONST BYTE_T *data,IN CONST UINT_T len,\
                                  IN CONST BYTE_T *key,IN BYTE_T *iv,\
                                  OUT BYTE_T *ec_data);
OPERATE_RET aes128_cbc_decode_raw(IN CONST BYTE_T *data,IN CONST UINT_T len,\
                                  IN CONST BYTE_T *key,IN BYTE_T *iv,\
                                  OUT BYTE_T *dec_data);









#ifdef __cplusplus
}
#endif
#endif

