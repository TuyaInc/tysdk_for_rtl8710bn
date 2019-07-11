#include <string.h>
#include <stdlib.h>
#include <string.h>  

//////////////////8���任���///////////////////////////////
//��ʼ�û���
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


//���ʼֵ�û���
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


//S��
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


//32λ��չ��48λ������Կ�������
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

//��Сѡ��任�� ��64λ��Կת��Ϊ56λ��Կ
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

//��Сѡ��任��2 ��56λ��Կת����48λ��Կ
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

//16��ѭ������ѡ��� ����Կ��
static unsigned char DesRots[] = 
{
	1, 1, 2, 2, 
	2, 2, 2, 2, 
	1, 2, 2, 2, 
	2, 2, 2, 1, 
	0
};
//***********************��������****************************************************
//
static void movram(unsigned char* source,unsigned char* target,unsigned char length);
static void doxor(unsigned char* sourceaddr,unsigned char* targetaddr,unsigned char length);
static void setbit(unsigned char* dataddr,unsigned char pos,unsigned char b0);
static unsigned char getbit(unsigned char* dataddr,unsigned char pos);
static void selectbits(unsigned char* source,unsigned char* table,unsigned char* target,unsigned char count);
static void shlc(unsigned char* data_p);
static void shrc(unsigned char* data_p);
static void strans(unsigned char* index,unsigned char* target);

//********************��ʼ����********************************************************

//�ֽڸ��ƣ��൱��memcpy�� �����ϵͳ�ĺ�����ʲô���죿��
//source -  Դ��ַ
//target -  Ŀ�ĵ�ַ
//length -  ���Ƴ���
static void movram(unsigned char* source,unsigned char* target,unsigned char length)
{
	unsigned char i;
	
	for(i = 0;i < length;i++)/*����*/
	{
		target[i] = source[i];
	}
}

//��������������
//source -  ����1���ҽ����ŵ�ַ
//target -  ����2
//length -  ��������ֽڳ���
static void doxor(unsigned char* sourceaddr,unsigned char* targetaddr,unsigned char length)
{
	unsigned char i;
	
	for (i = 0;i < length;i++)/*���ֽ����*/
	{
		sourceaddr[i] ^= targetaddr[i];
	}
	return;
}

//����ĳλ��ֵ
//dataddr - �任���ݺ�ĵ�ַ
//pos - ĳλi��λ��
//b0 - ��λi��ֵ
//�����任���1λ��ֵ Ϊԭ���ݵ�58λ��ֵ�� bo�ǵ�58λ��ֵ��byte_count�Ǳ任�����ݴ�λ���ڵ��ֽ�;bit_count�������ֽڵĵڼ�λ
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


//��ȡĳλ��ֵ
//source - �任ǰ���ݵĴ�ŵ�ַ
//pos - ĳλiͨ����任����ԭ���ݵ�ԭλ��
//���ر任���ĳλֵ��1��0��
//������λ�任���0λ��ԭ���ݵĵ�58λ��ԭ���ݵĵ�58λ��ԭ���ݵ�7���ֽڵĵ�6λ��
//����byte_count���Ǽ����58λ��ԭ�������ڵ��ֽڣ�bit_count�����λ�ڴ��ֽڵĵڼ�λ��
//�˺����ó����Ǳ任���0λ(ԭ58λ)��ֵ��1����0
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

//���ݱ任
//source - �任����Դ��ַ
//table - �任�����õı任���(�任�㷨)
//target - �任��Ŀ�ĵ�ַ
//count - �任���ݳ���
static void selectbits(unsigned char* source,unsigned char* table,unsigned char* target,unsigned char count)
{
	unsigned char i;
	
	for(i = 0;i < count;i++)
	{
		setbit(target,i + 1,getbit(source,table[i])); 
	}
	return;
}

//ѭ������1λ 
//���ܣ�56λ��Կ�ֳ�2�룬ѭ������1λ��Ȼ���ٺϲ���56λ��Կ
//data_p - ����λ�����ݵ�ַ�����Ҳ����ڴ�
static void shlc(unsigned char* data_p)
{
	unsigned char i,b0;
	
	b0 = getbit(data_p,1); //���data_p��1λ��ֵ
	
	for(i = 0;i < 7;i++)
	{
		data_p[i] <<= 1;	//��7���ֽڶ�����һλ
		
		if(i != 6)
		{
			setbit(&data_p[i],8,getbit(&data_p[i + 1],1));//��ǰ1�ֽڵĵ�1λ��ʵ���ǵ�7λ���Ƶ����ֽڵĵ�8λ��ʵ�ʵ�0λ��
		}
	}
	
	setbit(data_p,56,getbit(data_p,28));//��ԭ���ݵĵ�28λ�Ƶ���56λ����ʵ���Ͻ�ԭ29λ�Ƶ����ڵĵ�56λ
	setbit(data_p,28,b0);			 //��ԭ����1λ�������ڵĵ�56λ
	return;
}

//ѭ������1λ 
//���ܣ�56λ��Կ�ֳ�2�룬ѭ������1λ��Ȼ���ٺϲ���56λ��Կ
//data_p - ����λ�����ݵ�ַ�����Ҳ����ڴ�
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

//S�б任 : ��48λת��Ϊ32λ
//index - ԭ���ݴ�ŵ�ַ 6�ֽ�48λ
//target - ת�����ŵ�ַ
//ԭ��6�ֽ�48λ��Ϊ8�飬ÿ��6λ��ÿ����ΪS��(Ҳ��8��)��ÿ�����롣b1b2b3b4b5b6; ��=b1b6 �� = b2b3b4b5; �õ���ֵΪ4λ��
/* The problem is about yielded in this function */
static void strans(unsigned char* index,unsigned char* target)
{
	unsigned char row,line,t,i,j;
	
	for(i = 0;i < 4;i++)
	{
		row = line = t = 0;
		setbit(&line,7,getbit(index,i * 12 + 1));//��ȡԴ���ݵ�ַ�ĵ�1λ�͵�6λ��ֵ�������б����ĵ�7λ��8λ����b1b6��
		setbit(&line,8,getbit(index,i * 12 + 6));
		
		for(j = 2;j < 6;j++)//��ȡԴ���ݵ�ַ�ĵ�2λ����5λ��ֵ�������б����ĵ�5λ~��8λ����b5b4b3b2��
		{
			setbit(&row,3 + j,getbit(index,i * 12 + j));
		}
		
		t = DesS[i * 2][line][row];//��ȡS��ֵ���˽����4λ��Ч
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

//DES���ܽ���
//data_p:	��������ݵ�ַ
//key_p : ��Կ��ַ
//type :���� (1- ���ܣ� 0 - ���ܣ�)
void DES(unsigned char *data_p,unsigned char* key_p,int type)
{
	unsigned char tempbuf[12];//12�ֽڣ�96λ�������ʱ����
	unsigned char key[7];
	unsigned char i;
	unsigned char j;
	unsigned char count;
	void (*f)(unsigned char* data_p);//�յĺ���ָ�룬���Դ��ݺ���������
	
	selectbits(data_p,DesIp,tempbuf,64);/*IP�û�����������tempbuf��*/
	movram(tempbuf,data_p,8);		//��8�ֽ��û��������data_pԭ�����ڡ�
	selectbits(key_p,DesPc_1,key,56);	/*��64λ��Կ������С�任1�õ�56λ��Կ*/
	
	for(i = 0;i < 16;i ++)//16�ε�������
	{
		selectbits(data_p + 4,DesE,tempbuf,48);/*���Ұ벿��32λ������չ��48λ*/
		
		if(type ==1)		//����
		{
			f = shlc;
			count = i;
		}
		else				//����
		{
			count = 16 - i;	
			f = shrc;
		}
		
		for(j = 0;j < DesRots[count];j++)/*56λ����Կ��λ*/
		{
			f(key);
		}
		
		selectbits(key,DesPc_2,tempbuf + 6,48);/*��56λ����Կ��С�任2Ϊ48�ֽ�*/
		doxor(tempbuf,tempbuf + 6,6);			//����Կ���Ұ벿�����
		strans(tempbuf,tempbuf + 6);			//S�б任�� 48λת��Ϊ32λ
		selectbits(tempbuf + 6,DesP,tempbuf,32);//P�û�
		doxor(tempbuf,data_p,4);				//�������
		
		if(i < 15)
		{
			movram(data_p + 4,data_p,4);		//���ҽ���
			movram(tempbuf,data_p + 4,4);
		}
	}
	
	movram(tempbuf,data_p,4);
	selectbits(data_p,DesIp_1,tempbuf,64);	//IP���û�
	movram(tempbuf,data_p,8);			//���Ƶ�data_p��
	return;
}

