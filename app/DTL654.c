#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "DTL654.h"




int EncodeDTL654Frame(uint8_t* frameOut, uint8_t frameLen, DTL654Item_t* DTL654Item)
{
	int i;
	uint8_t checksum = 0;
	if(frameLen < (DTL654Item->DTL654FrameDataLen + 12))
		return -1;
	
	frameOut[0] = DTL654START;
	for(i=0;i<6;i++)
		frameOut[i+1] = DTL654Item->DTL654FrameAddr[i];

	frameOut[7] = DTL654START;
	frameOut[8] = DTL654Item->DTL654ControlCode;
	frameOut[9] = DTL654Item->DTL654FrameDataLen;
	for(i=0;i<DTL654Item->DTL654FrameDataLen;i++)
		frameOut[i+10] = DTL654Item->DTL654FrameData[i];
	
	for(i=0;i<(DTL654Item->DTL654FrameDataLen + 12 - 3);i++)
		checksum += frameOut[i];
	
	frameOut[DTL654Item->DTL654FrameDataLen + 12 -2] = checksum;
	frameOut[DTL654Item->DTL654FrameDataLen + 12 -1] =  DTL654END;
	
	return (DTL654Item->DTL654FrameDataLen + 12);
}




/* 
 * @return: >=0 length of DTL654 frame
 *			<0  error
 */
int DecodeDTL654Frame(uint8_t* frameIn, uint8_t frameLen, DTL654Item_t* DTL654Item)
{
	int i,j;
	int DTL654FrameLen;
	uint8_t checksum = 0;

	for(i=0;i<frameLen;i++)
	{
		if(frameIn[i] == DTL654START)
			break;
	}
	
	if(i == frameLen)
		return DTL654START1_NOFIND;
	
	/*headIndex = i*/
	if(frameIn[i+7] != DTL654START)
		return DTL654START2_NOFIND;
	
	DTL654FrameLen = 1/*start*/ + 6/*addr*/ + 1/*start*/ + 1/*C*/ + 1/*L*/+ frameIn[i+9]/*data*/ + 1/*CS*/ + 1/*END*/;
	
	if(DTL654FrameLen > frameLen)
		return DTL654_LENGTH_ERROR;
	
	if(frameIn[i + DTL654FrameLen - 1] != DTL654END)
		return NO_DTL654END_NOFIND;
	
	for(j=0;j<DTL654FrameLen-3;j++)
	{
		checksum += frameIn[i+j];
	}

	if(checksum != frameIn[i + DTL654FrameLen -2])
		return CHECK_ERROR;
	
	for(j=0;j<6;j++)
		DTL654Item->DTL654FrameAddr[j] = frameIn[i+1+j];
	
	DTL654Item->DTL654ControlCode = frameIn[i+8];
	
	DTL654Item->DTL654FrameDataLen = frameIn[i+9];

	for(j=0;j<DTL654Item->DTL654FrameDataLen;j++)
	{
		DTL654Item->DTL654FrameData[j] = frameIn[i+10+j];
	}
	
	return (i+DTL654FrameLen);
}





int main()
{
	uint8_t frame[100];
	int frameLen;
	DTL654Item_t dtl654Item;
	uint8_t dataBuf[3] = {1,2,3};
	int i;
	int itemLen;
	
	dtl654Item.DTL654FrameAddr[0] = 0x11;
	dtl654Item.DTL654FrameAddr[1] = 0x11;
	dtl654Item.DTL654FrameAddr[2] = 0x11;
	dtl654Item.DTL654FrameAddr[3] = 0x11;
	dtl654Item.DTL654FrameAddr[4] = 0x11;
	dtl654Item.DTL654FrameAddr[5] = 0x11;
	dtl654Item.DTL654ControlCode = 0xff;
	dtl654Item.DTL654FrameDataLen = 3;
	dtl654Item.DTL654FrameData = dataBuf;
	
	frameLen = EncodeDTL654Frame(frame, 100, &dtl654Item);
	
	for(i=0;i<frameLen;i++)
		printf("%x ",frame[i]);
		
	printf("\n");
	
	dtl654Item.DTL654FrameAddr[0] = 0;
	dtl654Item.DTL654FrameAddr[1] = 0;
	dtl654Item.DTL654FrameAddr[2] = 0;
	dtl654Item.DTL654FrameAddr[3] = 0;
	dtl654Item.DTL654FrameAddr[4] = 0;
	dtl654Item.DTL654FrameAddr[5] = 0;
	dtl654Item.DTL654ControlCode = 0;
	dtl654Item.DTL654FrameDataLen = 0;
	dtl654Item.DTL654FrameData = dataBuf;
	dtl654Item.DTL654FrameData[0] = 0;
	dtl654Item.DTL654FrameData[1] = 0;
	dtl654Item.DTL654FrameData[2] = 0;
	
	itemLen = DecodeDTL654Frame(frame,frameLen,&dtl654Item);
	
	
	for(i=0;i<6;i++)
		printf("%x ",dtl654Item.DTL654FrameAddr[i]);
	
	printf("%x ",dtl654Item.DTL654ControlCode);
	printf("%x ",dtl654Item.DTL654FrameDataLen);
	
	for(i=0;i<dtl654Item.DTL654FrameDataLen;i++)
		printf("%x ",dtl654Item.DTL654FrameData[i]);

	printf("\n");
}













