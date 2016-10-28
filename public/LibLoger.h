#ifndef _LIBLOGER_H_
#define	_LIBLOGER_H_


#include <syslog.h>

#if 1	/*log to console*/
#define error(x) perror(x)
#define info(unsigned char *fmt,args...)  printf(fmt, args)
#endif

#if 0	/*log to syslog*/
#define error(x) perror(x)
#define info(unsigned char *fmt,args...)  syslog(LOG_INFO, fmt, args)
#endif

#if 0	/*log none*/
#define error(x) 
#define info(unsigned char *fmt,args...)  
#endif



#endif











