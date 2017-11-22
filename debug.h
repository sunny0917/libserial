#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>

#define DEBUG
#define DEBUG_LEVEL		0

#define SY_ERROR	0
#define SY_WARN		1
#define SY_DEBUG	2
#define SY_INFO		3

#ifdef DEBUG
#define SYLOGI(fmt,args...)		do{if(DEBUG_LEVEL >= SY_INFO)	printf("[SY_INFO]:"fmt,##args);}while(0)
#define SYLOGW(fmt,args...)		do{if(DEBUG_LEVEL >= SY_WARN)	printf("[SY_WARN]:"fmt,##args);}while(0)
#define SYLOGD(fmt,args...)		do{if(DEBUG_LEVEL >= SY_DEBUG)	printf("[SY_DEBUG]:"fmt,##args);}while(0)
#define SYLOGE(fmt,args...)		do{if(DEBUG_LEVEL >= SY_ERROR)	printf("[SY_ERROR]:"fmt,##args);}while(0)
#else
#define SYLOGI(fmt,args...)		do{}while(0)
#define SYLOGW(fmt,args...)		do{}while(0)
#define SYLOGD(fmt,args...)		do{}while(0)
#define SYLOGE(fmt,args...)		do{}while(0)	
#endif

#endif