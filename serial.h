#ifndef _SERIAL_H_
#define _SERIAL_H_
#include <stdio.h>  
#include <string.h>  
#include <sys/types.h>  
#include <errno.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <termios.h>  
#include <stdlib.h> 

#define SUCCSSE		0
#define FAIL			-1
#define WRONG			-2
#define TIMEOUT			-3

typedef unsigned long      TMO;
typedef unsigned int		OSAL_ID;
typedef int            	ER;
typedef unsigned int      	U32;
typedef unsigned short     U16;
typedef unsigned char      U8;

#define INVALL_DEV		-1

/*
 *功能：设置串口
 *输入：
 *	sfd		:文件描述符
 *	NBits	:数据位
 *	NEvent	:校验位
 *	NSpeed	:波特率
 *	NStop	:停止位
 *返回:
 *	-1	:设置失败
 *	0	:设置成功
 */
int uart_setopt(int sfd,int NBits, unsigned char NEvent, int NSpeed, int NStop);

/*
 *功能：初始化串口
 *输入：
 *	sfd		:返回文件描述符
 *	name	:串口节点
 *	NBits	:数据位
 *	NEvent	:校验位
 *	NSpeed	:波特率
 *	NStop	:停止位
 *返回:
 *	-1	:初始化失败
 *	0	:初始化成功
 */
int uart_devinit(int* sfd,const char* name,int NBits, char NEvent,int NSpeed,int NStop);

/*
 *功能：读取串口数据
 *输入：
 *	sfd		:文件描述符
 *	readb	:读取到的数据
 *	maxlen	:期望读maxlen个字节
 *	timeout	:超时(秒级别)
 *返回:
 *	-3	:超时
 *	-1	:读取失败
 *	0	:读取成功
 */
int uart_devread(int sfd,unsigned char* readb,int maxlen, int timeout);

/*
 *功能：读取串口数据
 *输入：
 *	sfd		:文件描述符
 *	readb	:读取到的数据
 *	maxlen	:期望读maxlen个字节
 *	timeout	:超时(毫秒级别)
 *返回:
 *	-3	:超时
 *	-1	:读取失败
 *	0	:读取成功
 */
int uart_devread_ms(int sfd,unsigned char* readb,int maxlen, int timeout);

/*
 *功能：释放串口
 *输入：
 *	sfd		:文件描述符
 *返回:
 *	-1	:失败
 *	0	:成功
 */
int uart_devclose(int sfd);

/*
 *功能：写串口数据
 *输入：
 *	sfd		:文件描述符
 *	writeb	:要写的数据
 *	maxlen	:要写的数据长度
 *返回:
 *	-1	:写失败
 *	0	:写成功
 */
int uart_devwrite(int sfd,unsigned char* writeb,int maxlen);

/*
 *功能：延时
 *输入：
 *	us		:延时us微妙
 */
void time_udelay(U32 us);

#endif
