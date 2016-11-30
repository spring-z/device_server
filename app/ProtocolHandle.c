#include <stdio.h>
#include "types.h"
#include "DTL645.h"
#include "LibMysql.h"




static void HexToString(char *src, int len, char *dest);


void ProtocolHandle(DTL654Item_t* DTL654Item)
{
	char DTL654DataSting[1024];
	uint32_t uid;
	dbConnNode_t* mysqlConn;
	
	HexToString(DTL654Item->DTL654FrameData, DTL654Item->DTL654FrameDataLen, DTL654DataSting);
	uid = DTL654Item->DTL654FrameAddr[2];
	uid <<= 8;
	uid = DTL654Item->DTL654FrameAddr[3];
	uid <<= 8;
	uid = DTL654Item->DTL654FrameAddr[4];
	uid <<= 8;
	uid = DTL654Item->DTL654FrameAddr[5];

	mysqlConn = GetMysqlConnNode(g_mysqlConnPool);
	MysqlExcuteQuery(mysqlConn, \
		"insert into ts_data(uid,group_id,type,server_time,data) \
			values('%ld',%d,%d,now(),%s);",\
			uid,
			DTL654Item->DTL654FrameAddr[0],
			DTL654Item->DTL654ControlCode,
			DTL654DataSting);
	
	
	free(DTL654Item->DTL654FrameData);
	
	
}



static void HexToString(char *src, int len, char *dest)
{
    int i,j;
    //Æ´½Ó
    for(i=0,j=0;j<len;i++,j++)
    {
        sprintf(dest+i,"%x",(*(src+j)>>4)&0x0f);
        sprintf(dest+(++i),"%x",*(src+j)&0x0f);
    }
    dest[2*len]='\0';    

}




