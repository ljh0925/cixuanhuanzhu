
#define UDP_GLOBALS
#include "..\main\config.h"

EX_RAM udp_send Udp_Packed;



/**********************************************************************
**º¯ÊýÔ­ÐÍ:	    unsigned int CheckSum(unsigned int xdata *check,unsigned int length)
**Èë¿Ú²ÎÊý:º	check	:	±»Ð£ÑéÊý¾ÝÆðÊ¼µØÖ·
				length	:	±»Ð£ÑéÊý¾Ý³¤¶È£¬µ¥Î»×Ö
**³ö¿Ú²ÎÊý:		ÎÞ
**·µ »Ø Öµ:		~((sum)&0xffff))		       
**Ëµ    Ã÷:		¼ÆËãÐ£ÑéºÍ 
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
if(length&0x01)//³¤¶ÈÎªÆæÊý¸öÊ±£¬Òª½øÐÐ¸Ã²Ù×÷
	{
	sum = sum + ((*check2)&0xff00);
	}
sum = (sum&0xffff) + ((sum>>16)&0xffff);//¸ß16Î»ºÍµÍ16Î»Ïà¼Ó
if(sum & 0xffff0000)
	{//±íÊ¾ÓÐ½øÎ»
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
Udp_Packed.UdpDHead.TotalLen=8+UDPSEDN.length;//Î±Í·£«Í·£«Êý¾Ý
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
Udp_Packed.Udp.length=Udp_Packed.UdpDHead.TotalLen;//Í·£«Êý¾Ý
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
	OS_ENTER_CRITICAL();//±£»¤
	memcpy (&(Udp_Packed), ((Rec_Ptr*)Udp_Rec)->RecDataPtr, 8);//((Rec_Ptr*)Udp_Rec)->length);
	OS_EXIT_CRITICAL();//±£»¤

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


