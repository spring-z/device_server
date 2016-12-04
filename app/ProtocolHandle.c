#include <stdio.h>
#include <stdlib.h>
#include "ProtocolHandle.h"
#include "types.h"
#include "LibMysql.h"
#include "LibLoger.h"


extern dbConnPool_t* g_mysqlConnPool;


static void HexToString(char *src, int len, char *dest);

/* 
 * @解析DTL645协议之上封装的应用层数据协议
 * @param DTL645数据内容
 * @return: void
 *
 */
void ProtocolHandle(DTL645Item_t* DTL645Item)
{
	char DTL645DataSting[1024];
	uint32_t uid = 0;
	dbConnNode_t* mysqlConn;
	
	HexToString((char*)DTL645Item->DTL645FrameData, DTL645Item->DTL645FrameDataLen, DTL645DataSting);
	uid += DTL645Item->DTL645FrameAddr[2];
	uid <<= 8;
	uid += DTL645Item->DTL645FrameAddr[3];
	uid <<= 8;
	uid += DTL645Item->DTL645FrameAddr[4];
	uid <<= 8;
	uid += DTL645Item->DTL645FrameAddr[5];

	mysqlConn = GetMysqlConnNode(g_mysqlConnPool);
	MysqlExcuteQuery(mysqlConn, \
		"insert into device_data_record(device_uid,device_group,device_type,server_time,device_data) \
			values(%ld,%d,%d,now(),%s);",\
			uid,
			DTL645Item->DTL645FrameAddr[0],
			DTL645Item->DTL645ControlCode,
			DTL645DataSting);
	ReleaseMysqlConnNode(mysqlConn);
	
	free(DTL645Item);
	info("user data decode done!\n");
	
}



static void HexToString(char *src, int len, char *dest)
{
    int i,j;
    //拼接
    for(i=0,j=0;j<len;i++,j++)
    {
        sprintf(dest+i,"%x",(*(src+j)>>4)&0x0f);
        sprintf(dest+(++i),"%x",*(src+j)&0x0f);
    }
    dest[2*len]='\0';    

}




