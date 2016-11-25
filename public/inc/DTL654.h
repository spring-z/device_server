#ifndef _DTL654_H_
#define _DTL654_H_

#include <stdint.h>

#define DTL654START		0x68
#define DTL654END		0x16



#define	DTL654START1_NOFIND			(-1)
#define	DTL654START2_NOFIND			(-2)
#define	DTL654_LENGTH_ERROR			(-3)
#define	NO_DTL654END_NOFIND			(-4)
#define	CHECK_ERROR					(-5)



typedef struct
{
	uint8_t DTL654FrameAddr[6]; 
	uint8_t DTL654ControlCode;
	uint8_t DTL654FrameDataLen;
	uint8_t* DTL654FrameData;
} DTL654Item_t;



int EncodeDTL654Frame(uint8_t* frameOut, uint8_t frameLen, DTL654Item_t* DTL654Item);

/* 
 * @return: >=0 length of DTL654 frame
 *			<0  error
 */
int DecodeDTL654Frame(uint8_t* frameIn, uint8_t frameLen, DTL654Item_t* DTL654Item);





#endif
