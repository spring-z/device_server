#ifndef _DTL654_H_
#define _DTL654_H_


#define DTL654START		0x68
#define DTL654END		0x16



#define	DTL654START1_NOFIND			(-1)
#define	DTL654START2_NOFIND			(-2)
#define	NO_DTL654END_NOFIND			(-3)
#define	CHECK_ERROR					(-4)



typedef struct
{
	uint8_t DTL654ControlCode;
	uint8_t DTL654FrameAddr[6]; 
	uint8_t DTL654FrameDataLen;
	uint8_t* DTL654FrameData;
} DTL654Item_t;












#endif
