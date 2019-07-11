#ifndef _DES_H
#define _DES_H

///////////////////////////////////////////////////////////////
//des加密函数
// data_p: 需加密的数据
// key_p : 加密密钥
// type : 1 - 加密； 0 - 解密
extern void DES(unsigned char *data_p,unsigned char* key_p,int type);

#endif

