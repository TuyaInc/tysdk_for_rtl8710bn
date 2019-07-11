#include <string.h>
#include <stdlib.h>
#include <string.h>  

//////////////////8个变换表格///////////////////////////////
//初始置换表
static unsigned char DesIp[]=
{ 
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17, 9,  1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
};


//逆初始值置换表
static unsigned char DesIp_1[] =
{
	40, 8,  48, 16, 56, 24, 64, 32,
	39, 7,  47, 15, 55, 23, 63, 31,
	38, 6,  46, 14, 54, 22, 62, 30,
	37, 5,  45, 13, 53, 21, 61, 29,
	36, 4,  44, 12, 52, 20, 60, 28,
	35, 3,  43, 11, 51, 19, 59, 27,
	34, 2,  42, 10, 50, 18, 58, 26,
	33, 1,  41, 9,  49, 17, 57, 25
};


//S盒
static unsigned char DesS[8][4][16] = 
{
	{
		{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
		{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
		{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
		{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
	},
	
	{
		{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
		{3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
		{0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
		{13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
	},
	
	{
		{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
		{13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
		{13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
		{1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
	},
	
	{
		{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
		{13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
		{10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
		{3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
	},
	
	{
		{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
		{14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
		{4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
		{11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
	},
	
	{
		{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
		{10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
		{9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
		{4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
	},
	
	{
		{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
		{13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
		{1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
		{6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
	},
	
	{
		{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
		{1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
		{7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
		{2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
	}
};


//32位扩展到48位（与密钥进行异或）
static unsigned char DesE[] =
{
	32, 1, 2, 3, 4, 5,
	4, 5, 6, 7, 8, 9,
	8, 9, 10, 11, 12,13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32, 1
};

static unsigned char DesP[] = 
{
	16, 7, 20, 21,
	29, 12, 28, 17,
	1, 15, 23, 26,
	5, 18, 31, 10,
	2, 8, 24,  14,
	32, 27, 3, 9,
	19, 13, 30, 6,
	22, 11, 4, 25
};

//缩小选择变换表 将64位密钥转换为56位密钥
static unsigned char DesPc_1[] =
{
	57, 49, 41, 33, 25, 17, 9,
	1, 58, 50, 42, 34, 26, 18,
	10, 2, 59, 51, 43, 35, 27,
	19, 11, 3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	7, 62, 54, 46, 38, 30, 22,
	14, 6, 61, 53, 45, 37, 29,
	21, 13, 5, 28, 20, 12, 4
};

//缩小选择变换表2 将56位密钥转换成48位密钥
static unsigned char DesPc_2[] =
{
	14, 17, 11, 24, 1, 5,
	3, 28, 15, 6, 21, 10,
	23, 19, 12, 4, 26, 8,
	16, 7, 27, 20, 13, 2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};

//16次循环左移选择表 （密钥）
static unsigned char DesRots[] = 
{
	1, 1, 2, 2, 
	2, 2, 2, 2, 
	1, 2, 2, 2, 
	2, 2, 2, 1, 
	0
};
//***********************函数声明****************************************************
//
static void movram(unsigned char* source,unsigned char* target,unsigned char length);
static void doxor(unsigned char* sourceaddr,unsigned char* targetaddr,unsigned char length);
static void setbit(unsigned char* dataddr,unsigned char pos,unsigned char b0);
static unsigned char getbit(unsigned char* dataddr,unsigned char pos);
static void selectbits(unsigned char* source,unsigned char* table,unsigned char* target,unsigned char count);
static void shlc(unsigned char* data_p);
static void shrc(unsigned char* data_p);
static void strans(unsigned char* index,unsigned char* target);

//********************开始处理********************************************************

//字节复制，相当于memcpy， 与调用系统的函数有什么差异？？
//source -  源地址
//target -  目的地址
//length -  复制长度
static void movram(unsigned char* source,unsigned char* target,unsigned char length)
{
	unsigned char i;
	
	for(i = 0;i < length;i++)/*复制*/
	{
		target[i] = source[i];
	}
}

//两个数组进行异或
//source -  数组1，且结果存放地址
//target -  数组2
//length -  参与异或字节长度
static void doxor(unsigned char* sourceaddr,unsigned char* targetaddr,unsigned char length)
{
	unsigned char i;
	
	for (i = 0;i < length;i++)/*按字节异或*/
	{
		sourceaddr[i] ^= targetaddr[i];
	}
	return;
}

//设置某位的值
//dataddr - 变换数据后的地址
//pos - 某位i的位置
//b0 - 此位i的值
//例：变换后第1位的值 为原数据第58位的值。 bo是第58位的值。byte_count是变换后数据此位所在的字节;bit_count是所在字节的第几位
static void setbit(unsigned char* dataddr,unsigned char pos,unsigned char b0)	
{
	unsigned char byte_count;
	unsigned char bit_count;
	unsigned char temp;

	temp = 1;
	byte_count = (pos - 1) / 8;
	bit_count = 7 - ((pos - 1) % 8);
	temp <<= bit_count;
	
	if(b0)
	{
		dataddr[byte_count] |= temp;
	}
	else
	{
		temp = ~temp;
		dataddr[byte_count] &= temp;
	}
	return;
}


//获取某位的值
//source - 变换前数据的存放地址
//pos - 某位i通过查变换表，在原数据的原位置
//返回变换后的某位值（1或0）
//例：置位变换表第0位是原数据的第58位，原数据的第58位是原数据第7个字节的第6位。
//这里byte_count就是计算此58位在原数据所在的字节；bit_count计算此位在此字节的第几位。
//此函数得出的是变换后第0位(原58位)的值是1还是0
static unsigned char getbit(unsigned char* dataddr,unsigned char pos)	
{
	unsigned char byte_count;
	unsigned char bit_count;
	unsigned char temp;

	temp = 1;
	byte_count = (pos - 1) / 8;
	bit_count = 7 - ((pos - 1) % 8);
	temp <<= bit_count;
	if(dataddr[byte_count] & temp)
	{
		return(1);
	}
	else 
	{
		return(0);
	}
}

//数据变换
//source - 变换数据源地址
//table - 变换所采用的变换表格(变换算法)
//target - 变换后目的地址
//count - 变换数据长度
static void selectbits(unsigned char* source,unsigned char* table,unsigned char* target,unsigned char count)
{
	unsigned char i;
	
	for(i = 0;i < count;i++)
	{
		setbit(target,i + 1,getbit(source,table[i])); 
	}
	return;
}

//循环左移1位 
//功能：56位密钥分成2半，循环左移1位，然后再合并成56位密钥
//data_p - 需移位的数据地址，结果也存放在此
static void shlc(unsigned char* data_p)
{
	unsigned char i,b0;
	
	b0 = getbit(data_p,1); //获得data_p第1位的值
	
	for(i = 0;i < 7;i++)
	{
		data_p[i] <<= 1;	//将7个字节都左移一位
		
		if(i != 6)
		{
			setbit(&data_p[i],8,getbit(&data_p[i + 1],1));//将前1字节的第1位（实际是第7位）移到该字节的第8位（实际第0位）
		}
	}
	
	setbit(data_p,56,getbit(data_p,28));//将原数据的第28位移到第56位？？实际上将原29位移到现在的第56位
	setbit(data_p,28,b0);			 //将原来第1位放在现在的第56位
	return;
}

//循环右移1位 
//功能：56位密钥分成2半，循环右移1位，然后再合并成56位密钥
//data_p - 需移位的数据地址，结果也存放在此
static void shrc(unsigned char* data_p)
{
	unsigned char b0;
	int i;
	
	b0 = getbit(data_p,56);
	
	for(i = 6;i >= 0;i--)
	{
		data_p[i] >>= 1;
		
		if(i != 0)
		{
			setbit(&data_p[i],1,getbit(&data_p[i - 1],8)); 
		}
	}
	
	setbit(data_p,1,getbit(data_p,29));
	setbit(data_p,29,b0);
	return;
}

//S盒变换 : 将48位转换为32位
//index - 原数据存放地址 6字节48位
//target - 转换后存放地址
//原理：6字节48位分为8组，每组6位，每组作为S盒(也有8组)的每组输入。b1b2b3b4b5b6; 行=b1b6 列 = b2b3b4b5; 得到的值为4位。
/* The problem is about yielded in this function */
static void strans(unsigned char* index,unsigned char* target)
{
	unsigned char row,line,t,i,j;
	
	for(i = 0;i < 4;i++)
	{
		row = line = t = 0;
		setbit(&line,7,getbit(index,i * 12 + 1));//获取源数据地址的第1位和第6位的值放置在行变量的第7位第8位（即b1b6）
		setbit(&line,8,getbit(index,i * 12 + 6));
		
		for(j = 2;j < 6;j++)//获取源数据地址的第2位到第5位的值放置在列变量的第5位~第8位（即b5b4b3b2）
		{
			setbit(&row,3 + j,getbit(index,i * 12 + j));
		}
		
		t = DesS[i * 2][line][row];//获取S盒值，此结果低4位有效
		t <<= 4;
		line = row = 0; 
		setbit(&line,7,getbit(index,i * 12 + 7));
		setbit(&line,8,getbit(index,i * 12 + 12));
		
		for(j = 2;j < 6;j++)
		{
			setbit(&row,3 + j,getbit(index,i * 12 + 6 + j));
		}
		
		t |= DesS[i * 2 + 1][line][row];
		target[i] = t;			
	}
	return;
}

//DES加密解密
//data_p:	输入的数据地址
//key_p : 密钥地址
//type :类型 (1- 加密； 0 - 解密；)
void DES(unsigned char *data_p,unsigned char* key_p,int type)
{
	unsigned char tempbuf[12];//12字节，96位，存放暂时数据
	unsigned char key[7];
	unsigned char i;
	unsigned char j;
	unsigned char count;
	void (*f)(unsigned char* data_p);//空的函数指针，可以传递函数名参数
	
	selectbits(data_p,DesIp,tempbuf,64);/*IP置换，结果存放在tempbuf中*/
	movram(tempbuf,data_p,8);		//将8字节置换结果放在data_p原缓冲内。
	selectbits(key_p,DesPc_1,key,56);	/*将64位密钥进行缩小变换1得到56位密钥*/
	
	for(i = 0;i < 16;i ++)//16次迭代运算
	{
		selectbits(data_p + 4,DesE,tempbuf,48);/*将右半部分32位数据扩展成48位*/
		
		if(type ==1)		//加密
		{
			f = shlc;
			count = i;
		}
		else				//解密
		{
			count = 16 - i;	
			f = shrc;
		}
		
		for(j = 0;j < DesRots[count];j++)/*56位子密钥移位*/
		{
			f(key);
		}
		
		selectbits(key,DesPc_2,tempbuf + 6,48);/*将56位子密钥缩小变换2为48字节*/
		doxor(tempbuf,tempbuf + 6,6);			//子密钥与右半部分异或
		strans(tempbuf,tempbuf + 6);			//S盒变换， 48位转换为32位
		selectbits(tempbuf + 6,DesP,tempbuf,32);//P置换
		doxor(tempbuf,data_p,4);				//左右异或
		
		if(i < 15)
		{
			movram(data_p + 4,data_p,4);		//左右交换
			movram(tempbuf,data_p + 4,4);
		}
	}
	
	movram(tempbuf,data_p,4);
	selectbits(data_p,DesIp_1,tempbuf,64);	//IP逆置换
	movram(tempbuf,data_p,8);			//复制到data_p中
	return;
}

