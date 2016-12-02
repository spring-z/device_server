#ifndef __TYPES_H_
#define __TYPES_H_

#include <stdint.h>
/*
typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int uint32_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;
*/
typedef char * string_t;
typedef unsigned char ubool_t;
typedef void (*pFunction)(void);

#ifndef NULL
#define NULL (0)
#endif

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE 	0
#endif

#ifndef SUCCESS
#define SUCCESS 0
#endif

#ifndef FAILURE
#define FAILURE 1
#endif

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

#define OS_ASSERT(condition)                                                \
	do                                                                      \
	{                                                                       \
		if ((condition) != TRUE)                                            \
		{                                                                   \
			printf("\nassert in %s %d\n", __FILE__, __LINE__);			    \
		}                                                                   \
	}                                                                       \
	while (0)

typedef union
{
    uint16_t us;
    uint8_t uc[2];
} var_s16_t;

typedef union
{
    uint32_t ul;
    uint8_t uc[4];
} var_s32_t;

typedef union
{
    float ft;
    uint8_t uc[4];
} var_f32_t;

typedef union
{
    double db;
    uint8_t uc[8];
} var_d64_t;


#endif // End for __XTSYS_TYPE_H_
