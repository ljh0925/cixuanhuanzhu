#define IP_GLOBALS
#include "Config.h"
#include "../include/cfg_net.h"

OS_EVENT  		*RecIcmpQFlag;
OS_EVENT  		*RecTcpQFlag;
OS_EVENT  		*RecUdpQFlag;

OS_EVENT  		*SendFlag; 	

Rec_Ptr	RECQ[MAX_REC_BUFF];

void *RecIcmpQ[Q_Max_Size];
void *RecUdpQ[Q_Max_Size];
void *RecTcpQ[Q_Max_Size];

/**********************************************************************
**º¯ÊýÔ­ÐÍ£º    void 	CreateIpHeadCrc()
**Èë¿Ú²ÎÊý:º	ÎÞ
**³ö¿Ú²ÎÊý:		ÎÞ
**·µ »Ø Öµ£º	crc £º  16Î»µÄÐ£ÑéºÍ        
**Ëµ    Ã÷£º	²úÉúIPÊ×²¿µÄÐ£ÑéºÍ 
************************************************************************/
uint16  CreateIpHeadCrc(uint8 * Ip) 
{
union  w  CrcTemp;
uint8  i;
CrcTemp.dwords = 0;
	for(i=0;i<10;i++)
	{
		CrcTemp.dwords = CrcTemp.dwords + ((uint32)Ip[2*i]<<8)+(uint32)Ip[2*i+1];
	}
	while(CrcTemp.words.high>0) 
	{
    		CrcTemp.dwords = (uint32)(CrcTemp.words.high + CrcTemp.words.low);
    }
	CrcTemp.words.low = 0xffff - CrcTemp.words.low;  //È¡·´
	return(CrcTemp.words.low);
}
/**********************************************************************
**º¯ÊýÔ­ÐÍ£º    unsigned char Send_Ip_Frame(struct _pkst *TxdData,unsigned char * de_ip,unsigned char PROTOCOL)
**Èë¿Ú²ÎÊý:º	*TxdData	     	: ·¢ËÍÊý¾Ý½á¹¹Ö¸Õë
**				* de_ip   	     	: IPµØÖ·Ö¸Õë
**				NextProtocal 		: ÏÂÒ»²ãÐ­Òé£¬ÀýÈçTCPÐ­Òé»òUDPÐ­Òé
**³ö¿Ú²ÎÊý:		unsigned char 
**·µ »Ø Öµ£º	1					:OK
				2					:FAIL
**Ëµ    Ã÷£º	·¢ËÍIP°ü
************************************************************************/
uint8 Send_Ip_Frame	(
				struct _pkst *TxdData,
				uint8 * de_ip,
				uint8 * so_ip,
				uint8 PROTOCOL
			) 
{
union ip_rc IpHead;
uint8 IpHeadUint8[20];
struct _pkst TxdIpData;
uint8 err,num;
static uint16 FrameIndex=0;
	for(num=0;num<MAX_NET_PORT;num++)
	{
		if(NetPort[num].My_Ip[0]==so_ip[0])
		if(NetPort[num].My_Ip[1]==so_ip[1])
		if(NetPort[num].My_Ip[2]==so_ip[2])
		if(NetPort[num].My_Ip[3]==so_ip[3])
			break;
	}
	if(num>=MAX_NET_PORT)
		return (0xff);
	IpHead.e_ip.VerandIphLen=0x45;			//IP°æ±¾ºÍÍ·³¤¶È
	IpHeadUint8[0]=0x45;
	
	IpHead.e_ip.ServerType=0x00;                     //·þÎñÀàÐÍÎª0
	IpHeadUint8[1]=0x00;//·þÎñÀàÐÍ
	
	IpHead.e_ip.TotalLen=(*TxdData).length+20;			//IPÊý¾Ý±¨×Ü³¤¶È
	IpHeadUint8[2]=(IpHead.e_ip.TotalLen&0xff00)>>8;//IPÊý¾Ý±¨×Ü³¤¶È¸ß×Ö½Ú
	IpHeadUint8[3]=IpHead.e_ip.TotalLen&0x00ff;//IPÊý¾Ý±¨×Ü³¤¶ÈµÍ×Ö½Ú
	
	IpHead.e_ip.FrameIndex=FrameIndex+1;
	IpHeadUint8[4]=(FrameIndex&0xff00)>>8;//IPÊý¾Ý±¨±êÊ¶¸ß×Ö½Ú
	IpHeadUint8[5]=FrameIndex&0x00ff;//IPÊý¾Ý±¨±êÊ¶µÍ×Ö½Ú
	FrameIndex++;
	
	IpHeadUint8[6]=0x40;//IPÊý¾Ý±¨±êÖ¾¼°·Ö¶ÎÆ«ÒÆÁ¿
	IpHeadUint8[7]=0x00;//IPÊý¾Ý±¨±êÖ¾¼°·Ö¶ÎÆ«ÒÆÁ¿
	
	IpHead.e_ip.ttl=0x80;       //128;				//TTL
	IpHeadUint8[8]=0x80;//ttl

	IpHead.e_ip.NextProtocal=PROTOCOL;		//ÏÂ²ãÐ­Òé
    	IpHeadUint8[9]=PROTOCOL;
	
    	IpHead.e_ip.Crc=0;
	IpHeadUint8[10]=0;//CRC h
	IpHeadUint8[11]=0;//CRC l
	
	IpHead.e_ip.SourceIp[0]=NetPort[num].My_Ip[0];	//Ìî³äÔ´IPµØÖ·
    	IpHead.e_ip.SourceIp[1]=NetPort[num].My_Ip[1];
	IpHead.e_ip.SourceIp[2]=NetPort[num].My_Ip[2];
	IpHead.e_ip.SourceIp[3]=NetPort[num].My_Ip[3];
	
	IpHeadUint8[12]=NetPort[num].My_Ip[0];	//Ìî³äÔ´IPµØÖ·
    	IpHeadUint8[13]=NetPort[num].My_Ip[1];
	IpHeadUint8[14]=NetPort[num].My_Ip[2];
	IpHeadUint8[15]=NetPort[num].My_Ip[3];

    	IpHead.e_ip.DestId[0]=de_ip[0];			//Ìî³äÄ¿µÄIPµØÖ·
    	IpHead.e_ip.DestId[1]=de_ip[1];
	IpHead.e_ip.DestId[2]=de_ip[2];
	IpHead.e_ip.DestId[3]=de_ip[3];

	IpHeadUint8[16]=de_ip[0];			//Ìî³äÄ¿µÄIPµØÖ·
    	IpHeadUint8[17]=de_ip[1];
	IpHeadUint8[18]=de_ip[2];
	IpHeadUint8[19]=de_ip[3];

    	IpHead.e_ip.Crc=CreateIpHeadCrc(IpHeadUint8);		//²úÉúIPÍ·µÄ¼ìÑéºÍ
    	IpHeadUint8[10]=(IpHead.e_ip.Crc&0xff00)>>8;//IPÊý¾Ý±¨×Ü³¤¶È¸ß×Ö½Ú
	IpHeadUint8[11]=IpHead.e_ip.Crc&0x00ff;//IPÊý¾Ý±¨×Ü³¤¶ÈµÍ×Ö½Ú	

	TxdIpData.STPTR=TxdData;
	TxdIpData.length=20;
	TxdIpData.DAPTR=IpHeadUint8;
	OSSemPend(SendFlag,10,&err);//»ñÈ¡·¢ËÍµÄÈ¨Á¦
	
	if(err==OS_NO_ERR)
	{
		if(Send_Ip_To_LLC(&TxdIpData,IpHead.e_ip.DestId,num))
		{
			OSSemPost(SendFlag);
			return(1);
		}
		else
		{
			OSSemPost(SendFlag);
			return(0);
		}
	}
	else
		return (0);

}

uint8 IP_PROCESS(uint8 * RecData,uint8 num) 
{
static temp=0;
uint8  	iii;
uint16	PackedLength;
uint8 	TempIP[4];
#ifdef Little_End
uint16	Ltemp;
#endif
	TempIP[0]=((eip*)RecData)->SourceIp[0];
	TempIP[1]=((eip*)RecData)->SourceIp[1];
	TempIP[2]=((eip*)RecData)->SourceIp[2];
	TempIP[3]=((eip*)RecData)->SourceIp[3];//±£´æ¶Ô·½IP
	iii=((eip*)RecData)->VerandIphLen&0x0f;
	iii=iii*4;//È¡IP°üÍ·µÄ³¤¶È
#ifdef Big_End
	PackedLength=((eip*)RecData)->TotalLen;
#endif
#ifdef Little_End
	PackedLength=((eip*)RecData)->TotalLen;
	Ltemp=PackedLength&0x00ff;
	PackedLength=(PackedLength&0xff00)>>8;
	PackedLength=PackedLength+(Ltemp<<8);
#endif
	PackedLength=PackedLength-(uint16)iii;//¼ÆËãÓÐÐ§Êý¾ÝµÄ³¤¶È
	temp++;
	if(temp>=MAX_REC_BUFF)
		temp=1;
	switch(((eip*)RecData)->NextProtocal)
	{
		case 1:
			icmp_process(RecData,num);						  
			break;
		case 6:
			if(((eip*)RecData)->DestId[0]==NetPort[num].My_Ip[0])
			if(((eip*)RecData)->DestId[1]==NetPort[num].My_Ip[1])
			if(((eip*)RecData)->DestId[2]==NetPort[num].My_Ip[2])
			if(((eip*)RecData)->DestId[3]==NetPort[num].My_Ip[3])//Ä¿±êÎª±¾IP
			{
				RecData=RecData+iii;
				RECQ[temp].RecDataPtr=RecData;
				RECQ[temp].length=PackedLength;				//Òª´«ÊäipµØÖ·
				RECQ[temp].ip[0]=TempIP[0];
				RECQ[temp].ip[1]=TempIP[1];
				RECQ[temp].ip[2]=TempIP[2];
				RECQ[temp].ip[3]=TempIP[3];
				RECQ[temp].num=num;
				RECQ[temp].My_Ip[0]=NetPort[num].My_Ip[0];
				RECQ[temp].My_Ip[1]=NetPort[num].My_Ip[1];
				RECQ[temp].My_Ip[2]=NetPort[num].My_Ip[2];
				RECQ[temp].My_Ip[3]=NetPort[num].My_Ip[3];	
				OSQPost(RecTcpQFlag,(void *)&RECQ[temp]);
			}	
				//TCP
			break;
		case 17:
			//Òª´«ÊäipµØÖ·
			if(((eip*)RecData)->DestId[0]==NetPort[num].My_Ip[0])
			if(((eip*)RecData)->DestId[1]==NetPort[num].My_Ip[1])
			if(((eip*)RecData)->DestId[2]==NetPort[num].My_Ip[2])
			if(((eip*)RecData)->DestId[3]==NetPort[num].My_Ip[3])//Ä¿±êÎª±¾IP
			{
			RecData=RecData+iii;
			RECQ[temp].RecDataPtr=RecData;
			RECQ[temp].length=PackedLength;
			RECQ[temp].ip[0]=TempIP[0];
			RECQ[temp].ip[1]=TempIP[1];
			RECQ[temp].ip[2]=TempIP[2];
			RECQ[temp].ip[3]=TempIP[3];
			RECQ[temp].num=num;
			RECQ[temp].My_Ip[0]=NetPort[num].My_Ip[0];
			RECQ[temp].My_Ip[1]=NetPort[num].My_Ip[1];
			RECQ[temp].My_Ip[2]=NetPort[num].My_Ip[2];
			RECQ[temp].My_Ip[3]=NetPort[num].My_Ip[3];	
			OSQPost(RecUdpQFlag,(void *)&RECQ[temp]);
			}
			//UDP
			break;
		default : 
			break;
	}// end for switch
	return 1;
}
