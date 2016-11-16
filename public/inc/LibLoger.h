#ifndef _LIBLOGER_H_
#define	_LIBLOGER_H_


#include <syslog.h>

#if 1	/*log to console*/
#define error(format,...) printf(format,##__VA_ARGS__)
#define info(format,...)  printf(format,##__VA_ARGS__)
#endif

#if 0	/*log to syslog*/ 
#define error(fmt,...) syslog(LOG_ERR,fmt,##__VA_ARGS__)
#define info(fmt,...)  syslog(LOG_INFO,fmt,##__VA_ARGS__)
#endif

#if 0	/*log none*/
#define error(x) 
#define info(x)  
#endif



#endif











