
#define UDP_GLOBALS
#include "..\main\config.h"

EX_RAM udp_send Udp_Packed;



/**********************************************************************
**函数原型:	    unsigned int CheckSum(unsigned int xdata *check,unsigned int length)
**入口参数:�	check	:	被校验数据起始地址
				length	:	被校验数据长度，单位字
**出口参数:		无
**返 回 值:		~((sum)&0xffff))		       
**说    明:		计算校验和 
************************************************************************/
//=================================================================================
uint16 CheckSumUdp(uint16 EX_RAM *check1,uint16 EX_RAM *check2,uint16 length) USE_XDATA keilreentrant 
{
EX_RAM uint32 sum=0;
EX_RAM uint16 i;
for (i=0;i<4;i++)
	{
	sum += *check1++;
	}
for (i=0;i<(length/2);i++)
	{
	sum += *check2++;
	}
if(length&0x01)//长度为奇数个时，要进行该操作
	{
	sum = sum + ((*check2)&0xff00);
	}
sum = (sum&0xffff) + ((sum>>16)&0xffff);//高16位和低16位相加
if(sum & 0xffff0000)
	{//表示有进位
	sum++;
	}
return ( (uint16)(~((sum)&0xffff))  );
}

void Udp_Send_Packed(udp_socket UDPSEDN ) USE_XDATA keilreentrant //udp_send Udp_Temp)
{
EX_RAM struct _pkst TxdData1,TxdData2;
udp_send Udp_Packed;
Udp_Packed.UdpDHead.rev=0;
Udp_Packed.UdpDHead.NextProtocal=17;
Udp_Packed.UdpDHead.TotalLen=8+UDPSEDN.length;//伪头＋头＋数据
Udp_Packed.UdpDHead.SourceIp[0]=My_Ip_Address[0];
Udp_Packed.UdpDHead.SourceIp[1]=My_Ip_Address[1];
Udp_Packed.UdpDHead.SourceIp[2]=My_Ip_Address[2];
Udp_Packed.UdpDHead.SourceIp[3]=My_Ip_Address[3];
Udp_Packed.UdpDHead.DestId[0]=UDPSEDN.DestIp[0];
Udp_Packed.UdpDHead.DestId[1]=UDPSEDN.DestIp[1];
Udp_Packed.UdpDHead.DestId[2]=UDPSEDN.DestIp[2];
Udp_Packed.UdpDHead.DestId[3]=UDPSEDN.DestIp[3];
Udp_Packed.Udp.DestPort=UDPSEDN.DestPort;
Udp_Packed.Udp.SourcePort=UDPSEDN.SourcePort;
Udp_Packed.Udp.length=Udp_Packed.UdpDHead.TotalLen;//头＋数据
Udp_Packed.Udp.Crc=0;

Udp_Packed.Udp.Crc=CheckSumUdp((uint16 EX_RAM *)&Udp_Packed,(uint16 EX_RAM *)UDPSEDN.data_ptr,UDPSEDN.length);
TxdData1.STPTR=&TxdData2;
TxdData1.length=8;
TxdData1.DAPTR=(uint8 EX_RAM *)&Udp_Packed.Udp;
TxdData2.STPTR=NULL;
TxdData2.length=UDPSEDN.length;
TxdData2.DAPTR=UDPSEDN.data_ptr;
Send_Ip_Frame	(
					&TxdData1,
					Udp_Packed.UdpDHead.DestId,
					17
				);
//return 1 ;
}

uint8* Udp_Process(uint8 EX_RAM * Udp_Rec) USE_XDATA keilreentrant 
{
uint8 udpdata[10];
struct  udp_data Udp_Packed;
//udp_send Udp_Packed;
udp_socket UDPSEDN;
	OS_ENTER_CRITICAL();//保护
	memcpy (&(Udp_Packed), ((Rec_Ptr*)Udp_Rec)->RecDataPtr, 8);//((Rec_Ptr*)Udp_Rec)->length);
	OS_EXIT_CRITICAL();//保护

//	Udp_Packed.UdpDHead.DestId[0]=((Rec_Ptr*)Udp_Rec)->ip[0];
//	Udp_Packed.UdpDHead.DestId[1]=((Rec_Ptr*)Udp_Rec)->ip[1];
//	Udp_Packed.UdpDHead.DestId[2]=((Rec_Ptr*)Udp_Rec)->ip[2];
//	Udp_Packed.UdpDHead.DestId[3]=((Rec_Ptr*)Udp_Rec)->ip[3];
	UDPSEDN.DestIp[0]=((Rec_Ptr*)Udp_Rec)->ip[0];
	UDPSEDN.DestIp[1]=((Rec_Ptr*)Udp_Rec)->ip[1];
	UDPSEDN.DestIp[2]=((Rec_Ptr*)Udp_Rec)->ip[2];
	UDPSEDN.DestIp[3]=((Rec_Ptr*)Udp_Rec)->ip[3];

	switch(Udp_Packed.SourcePort)
	{
		case	1025   :
			udpdata[0]=0x31;
			udpdata[1]=0x30;
			udpdata[2]=0x32;
			udpdata[3]=0x35;
			UDPSEDN.SourcePort=Udp_Packed.DestPort;
			UDPSEDN.DestPort=Udp_Packed.SourcePort;
			UDPSEDN.length=4;
			UDPSEDN.data_ptr=udpdata;
			Udp_Send_Packed(UDPSEDN);
			break;
		case	1026   :
			udpdata[0]=0x31;
			udpdata[1]=0x30;
			udpdata[2]=0x32;
			udpdata[3]=0x36;
			UDPSEDN.SourcePort=Udp_Packed.DestPort;
			UDPSEDN.DestPort=Udp_Packed.SourcePort;
			UDPSEDN.length=4;
			UDPSEDN.data_ptr=udpdata;
			Udp_Send_Packed(UDPSEDN);
			break;
  		case	137   :
			udpdata[0]=0x31;
			udpdata[1]=0x30;
			udpdata[2]=0x32;
			udpdata[3]=0x37;
			UDPSEDN.SourcePort=Udp_Packed.DestPort;
			UDPSEDN.DestPort=Udp_Packed.SourcePort;
			UDPSEDN.length=4;
			UDPSEDN.data_ptr=udpdata;
			Udp_Send_Packed(UDPSEDN);
			break;
		case	1028   :
			udpdata[0]=0x31;
			udpdata[1]=0x30;
			udpdata[2]=0x32;
			udpdata[3]=0x38;
			UDPSEDN.SourcePort=Udp_Packed.DestPort;
			UDPSEDN.DestPort=Udp_Packed.SourcePort;
			UDPSEDN.length=4;
			UDPSEDN.data_ptr=udpdata;
			Udp_Send_Packed(UDPSEDN);
			break;
		default : 
			break;
	}
	return (((Rec_Ptr*)Udp_Rec)->RecDataPtr);
}


