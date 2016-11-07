#ifndef _LIBLOGER_H_
#define	_LIBLOGER_H_


#include <syslog.h>

#if 0	/*log to console*/
#define error(x) perror(x)
#define info(fmt...)  printf(fmt, __VA_ARGS__)
#endif

#if 1	/*log to syslog*/
#define error(x) perror(x)
#define info(fmt...)  syslog(LOG_INFO, fmt, __VA_ARGS__)
#endif

#if 0	/*log none*/
#define error(x) 
#define info(fmt...)  
#endif



#endif











