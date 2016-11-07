#ifndef _LIBLOGER_H_
#define	_LIBLOGER_H_


#include <syslog.h>

#if 1	/*log to console*/
#define error(x) perror(x)
#define info(x)  printf(x)
#endif

#if 0	/*log to syslog*/ 
#define error(x) perror(x)
#define info(x)  syslog(LOG_INFO, x)
#endif

#if 0	/*log none*/
#define error(x) 
#define info(x)  
#endif



#endif











