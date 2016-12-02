#ifndef _DTL645_H_
#define _DTL645_H_

#include <stdint.h>

#define DTL645START		0x68
#define DTL645END		0x16



#define	DTL645START1_NOFIND			(-1)
#define	DTL645START2_NOFIND			(-2)
#define	DTL645_LENGTH_ERROR			(-3)
#define	NO_DTL645END_NOFIND			(-4)
#define	CHECK_ERROR					(-5)



typedef struct
{
	uint8_t DTL645FrameAddr[6]; 
	uint8_t DTL645ControlCode;
	uint8_t DTL645FrameDataLen;
	uint8_t* DTL645FrameData;
} DTL645Item_t;



int EncodeDTL645Frame(uint8_t* frameOut, uint8_t frameLen, DTL645Item_t* DTL645Item);

/* 
 * @return: >=0 length of DTL645 frame
 *			<0  error
 */
int DecodeDTL645Frame(uint8_t* frameIn, uint8_t frameLen, DTL645Item_t* DTL645Item);





#endif
