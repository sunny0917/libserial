#include <stdio.h>  
#include <string.h>  
#include <sys/types.h>  
#include <errno.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <termios.h>  
#include <stdlib.h>

#include "serial.h"
#include "debug.h"

#define SERIAL_PORTING_DEBUG

int uart_setopt(int sfd,int NBits, unsigned char NEvent, int NSpeed, int NStop)
 {  
	struct termios newtio;  
	struct termios oldtio;  

	if(tcgetattr(sfd,&oldtio) != 0)  
	{  
		SYLOGE("SetupSerial 1\n");  
		return FAIL;  
	}  

	bzero(&newtio,sizeof(newtio));  
	newtio.c_cflag |= CLOCAL |CREAD;  
	newtio.c_cflag &= ~CSIZE;  

	switch(NBits)  
	{  
		case 7:  
			newtio.c_cflag |= CS7;  
			break;  
		case 8:  
			newtio.c_cflag |= CS8;  
			break;      
	}  

	switch(NEvent)  
	{  
		case 'O':  
			newtio.c_cflag |= PARENB;  
			newtio.c_cflag |= PARODD;  
			newtio.c_iflag |= (INPCK | ISTRIP);  
			break;  
		case 'E':  
			newtio.c_iflag |= (INPCK |ISTRIP);  
			newtio.c_cflag |= PARENB;  
			newtio.c_cflag &= ~PARODD;  
			break;  
		case 'N':  
			newtio.c_cflag &= ~PARENB;  
			break;  
	}  

	switch(NSpeed)  
	{  
		case 2400:  
			cfsetispeed(&newtio,B2400);  
			cfsetospeed(&newtio,B2400);  
			break;  
		case 4800:  
			cfsetispeed(&newtio,B4800);  
			cfsetospeed(&newtio,B4800);  
			break;  
		case 9600:  
			cfsetispeed(&newtio,B9600);  
			cfsetospeed(&newtio,B9600);  
			break; 
		case 19200:  
			cfsetispeed(&newtio,B19200);  
			cfsetospeed(&newtio,B19200);  
			break;  		
		case 115200:  
			cfsetispeed(&newtio,B115200);  
			cfsetospeed(&newtio,B115200);  
			break;  
		case 460800:  
			cfsetispeed(&newtio,B460800);  
			cfsetospeed(&newtio,B460800);  
			break;            
		default:  
			cfsetispeed(&newtio,B9600);  
			cfsetospeed(&newtio,B9600);  
			break;  
	}  

	if(NStop == 1)
	{  
		newtio.c_cflag &= ~CSTOPB;  
	}  
	else if(NStop ==2)
	{  
		newtio.c_cflag |= CSTOPB;  
	}  
	newtio.c_cc[VTIME] = 0;  
	newtio.c_cc[VMIN] = 0;  

	tcflush(sfd,TCIOFLUSH);  
	if((tcsetattr(sfd,TCSANOW,&newtio)) != 0)  
	{  
		SYLOGE("com set error\n");  
		return FAIL;  
	}  
	//SERIAL_DEBUG(IRIS_PF_INFO,"set done!\n");  
	return SUCCSSE;  
}  

int uart_devinit(int* sfd,const char* name,int NBits, char NEvent,int NSpeed,int NStop)
{
	int i;
	*sfd= open(name,O_RDWR | O_NOCTTY | O_NDELAY);
	if(*sfd == INVALL_DEV)
	{
		SYLOGE("open tty fail\n");
		return FAIL;
	}
	fcntl(INVALL_DEV, F_SETFL, 0);//
	if(uart_setopt(*sfd,NBits,NEvent,NSpeed,NStop) < 0)  
	{
		SYLOGE("set_opt error\n");  
		return FAIL;  
	}  
	return SUCCSSE;
}


int uart_devread(int sfd,unsigned char* readb,int maxlen, int timeout)
{
	int nread = 0;
	int time = 0;
	int len = 0;
	int i=0;

	struct timeval start,stop,diff;
	int gettimes = 0;
	gettimeofday(&start,0);			
	if(sfd == INVALL_DEV)
	{
		SYLOGE("tty fail\n");
		return FAIL;
	}
	while(1)
	{
		gettimeofday(&stop,0);
		tim_subtract(&diff,&start,&stop);
		if(diff.tv_sec*1000000 + diff.tv_usec > 1000000 * timeout)
		{
			return TIMEOUT;
		}
		nread = read(sfd,readb+len,maxlen-len);
		if(nread<0)
		{
			SYLOGE("device is error\n");
			return FAIL;
		}
		else
		{
			if(nread == (maxlen-len))
			{
				return SUCCSSE;	
			}
			else if(nread == 0)
			{
				usleep(20000);
				continue;
			}
			else
			{
				len += nread;
				//ALOGE("get %d left:%d:%d\n",len,maxlen-len,nread);
				continue;
			}
		}
	}
	return FAIL;	
}

int uart_devread_ms(int sfd,unsigned char* readb,int maxlen, int timeout)
{
	int nread = 0;
	int time = 0;
	int len = 0;
	int i=0;

	struct timeval start,stop,diff;
	int gettimes = 0;
	gettimeofday(&start,0);			
	if(sfd == INVALL_DEV)
	{
		SYLOGE("tty fail\n");
		return FAIL;
	}
	while(1)
	{
		gettimeofday(&stop,0);
		tim_subtract(&diff,&start,&stop);
		if(diff.tv_sec*1000000 + diff.tv_usec > 1000 * timeout)
		{
			return TIMEOUT;
		}
		nread = read(sfd,readb+len,maxlen-len);
		if(nread<0)
		{
			SYLOGE("device is error\n");
			return FAIL;
		}
		else
		{
			if(nread == (maxlen-len))
			{
				return SUCCSSE;	
			}
			else if(nread == 0)
			{
				usleep(20000);
				continue;
			}
			else
			{
				len += nread;
				//ALOGE("get %d left:%d:%d\n",len,maxlen-len,nread);
				continue;
			}
		}
	}
	return FAIL;	
}

int uart_devclose(int sfd)
{
	if(sfd == INVALL_DEV)
	{
		SYLOGE("DevClose a invall fd\n");
		return FAIL;
	}
	close(sfd);
	sfd = INVALL_DEV;
	return SUCCSSE;
}

int uart_devwrite(int sfd,unsigned char* writeb,int maxlen)
{
	int sendlen = 0;
	int len = 0;
	
	if(sfd == INVALL_DEV)
	{
		SYLOGE("DevWrite a invall fd\n");
		return FAIL;
	}
	//tcflush(sfd,TCOFLUSH);
	while(1)
	{
		sendlen = write(sfd,writeb+len,maxlen-len);
		len += sendlen;
		if(len == maxlen){
			break;
		}
	}
	return SUCCSSE;
}
   
int tim_subtract(struct timeval *result, struct timeval *x, struct timeval *y)
{
	if ( x->tv_sec > y->tv_sec )
	{
		return	-1;
	}
	if ((x->tv_sec==y->tv_sec) && (x->tv_usec>y->tv_usec))
	{
		return	-1;
	}
	result->tv_sec = ( y->tv_sec-x->tv_sec );
	result->tv_usec = ( y->tv_usec-x->tv_usec );
	if (result->tv_usec<0)
	{
		result->tv_sec--;
		result->tv_usec += 1000000;
	}
	return	0;
}

void time_udelay(U32 us)
{	
	struct timeval start,stop,diff;
	int gettimes = 0;
	gettimeofday(&start,0);
	while(1)
	{
		gettimeofday(&stop,0);
		tim_subtract(&diff,&start,&stop);
		if(diff.tv_usec >= us)
		{
			return;
		}
	}
	return;	
}
