#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "DTL645.h"




int EncodeDTL645Frame(uint8_t* frameOut, uint8_t frameLen, DTL645Item_t* DTL645Item)
{
	int i;
	uint8_t checksum = 0;
	if(frameLen < (DTL645Item->DTL645FrameDataLen + 12))
		return -1;
	
	frameOut[0] = DTL645START;
	for(i=0;i<6;i++)
		frameOut[i+1] = DTL645Item->DTL645FrameAddr[i];

	frameOut[7] = DTL645START;
	frameOut[8] = DTL645Item->DTL645ControlCode;
	frameOut[9] = DTL645Item->DTL645FrameDataLen;
	for(i=0;i<DTL645Item->DTL645FrameDataLen;i++)
		frameOut[i+10] = DTL645Item->DTL645FrameData[i];
	
	for(i=0;i<(DTL645Item->DTL645FrameDataLen + 12 - 3);i++)
		checksum += frameOut[i];
	
	frameOut[DTL645Item->DTL645FrameDataLen + 12 -2] = checksum;
	frameOut[DTL645Item->DTL645FrameDataLen + 12 -1] =  DTL645END;
	
	return (DTL645Item->DTL645FrameDataLen + 12);
}




/* 
 * @return: >=0 length of DTL645 frame
 *			<0  error
 */
int DecodeDTL645Frame(uint8_t* frameIn, uint8_t frameLen, DTL645Item_t* DTL645Item)
{
	int i,j;
	int DTL645FrameLen;
	uint8_t checksum = 0;

	for(i=0;i<frameLen;i++)
	{
		if(frameIn[i] == DTL645START)
			break;
	}
	
	if(i == frameLen)
		return DTL645START1_NOFIND;
	
	/*headIndex = i*/
	if(frameIn[i+7] != DTL645START)
		return DTL645START2_NOFIND;
	
	DTL645FrameLen = 1/*start*/ + 6/*addr*/ + 1/*start*/ + 1/*C*/ + 1/*L*/+ frameIn[i+9]/*data*/ + 1/*CS*/ + 1/*END*/;
	
	if(DTL645FrameLen > frameLen)
		return DTL645_LENGTH_ERROR;
	
	if(frameIn[i + DTL645FrameLen - 1] != DTL645END)
		return NO_DTL645END_NOFIND;
	
	for(j=0;j<DTL645FrameLen-3;j++)
	{
		checksum += frameIn[i+j];
	}

	if(checksum != frameIn[i + DTL645FrameLen -2])
		return CHECK_ERROR;
	
	for(j=0;j<6;j++)
		DTL645Item->DTL645FrameAddr[j] = frameIn[i+1+j];
	
	DTL645Item->DTL645ControlCode = frameIn[i+8];
	
	DTL645Item->DTL645FrameDataLen = frameIn[i+9];

	for(j=0;j<DTL645Item->DTL645FrameDataLen;j++)
	{
		DTL645Item->DTL645FrameData[j] = frameIn[i+10+j];
	}
	
	return (i+DTL645FrameLen);
}




/*
int main()
{
	uint8_t frame[100];
	int frameLen;
	DTL645Item_t DTL645Item;
	uint8_t dataBuf[3] = {1,2,3};
	int i;
	int itemLen;
	
	DTL645Item.DTL645FrameAddr[0] = 0x11;
	DTL645Item.DTL645FrameAddr[1] = 0x11;
	DTL645Item.DTL645FrameAddr[2] = 0x11;
	DTL645Item.DTL645FrameAddr[3] = 0x11;
	DTL645Item.DTL645FrameAddr[4] = 0x11;
	DTL645Item.DTL645FrameAddr[5] = 0x11;
	DTL645Item.DTL645ControlCode = 0xff;
	DTL645Item.DTL645FrameDataLen = 3;
	DTL645Item.DTL645FrameData = dataBuf;
	
	frameLen = EncodeDTL645Frame(frame, 100, &DTL645Item);
	
	for(i=0;i<frameLen;i++)
		printf("%x ",frame[i]);
		
	printf("\n");
	
	DTL645Item.DTL645FrameAddr[0] = 0;
	DTL645Item.DTL645FrameAddr[1] = 0;
	DTL645Item.DTL645FrameAddr[2] = 0;
	DTL645Item.DTL645FrameAddr[3] = 0;
	DTL645Item.DTL645FrameAddr[4] = 0;
	DTL645Item.DTL645FrameAddr[5] = 0;
	DTL645Item.DTL645ControlCode = 0;
	DTL645Item.DTL645FrameDataLen = 0;
	DTL645Item.DTL645FrameData = dataBuf;
	DTL645Item.DTL645FrameData[0] = 0;
	DTL645Item.DTL645FrameData[1] = 0;
	DTL645Item.DTL645FrameData[2] = 0;
	
	itemLen = DecodeDTL645Frame(frame,frameLen,&DTL645Item);
	
	
	for(i=0;i<6;i++)
		printf("%x ",DTL645Item.DTL645FrameAddr[i]);
	
	printf("%x ",DTL645Item.DTL645ControlCode);
	printf("%x ",DTL645Item.DTL645FrameDataLen);
	
	for(i=0;i<DTL645Item.DTL645FrameDataLen;i++)
		printf("%x ",DTL645Item.DTL645FrameData[i]);

	printf("\n");
}


*/










