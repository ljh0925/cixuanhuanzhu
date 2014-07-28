
#define ICMP_GLOBALS
#include "config.h"
#include "../include/cfg_net.h"

/**********************************************************************
**º¯ÊýÔ­ÐÍ£º    void 	CreateIcmpCrc( )
**Èë¿Ú²ÎÊý:º	ÎÞ
**³ö¿Ú²ÎÊý:		ÎÞ
**·µ »Ø Öµ£º	crc £º  16Î»µÄÐ£ÑéºÍ        
**Ëµ    Ã÷£º	²úÉúICMPÊý¾Ý¶ÎµÄÐ£ÑéºÍ 
************************************************************************/
uint16  CreateIcmpCrc(union icmp_rc icmppk) 
{
uint8 i;
union  w  CrcTemp;
CrcTemp.dwords=0;
for(i=0;i<20;i++)				//ÓÃÓÚ²éÑ¯µÄICMP±¨ÎÄ¹²40×Ö½Ú
	{
	CrcTemp.dwords = CrcTemp.dwords + icmppk.words.wordbuf[i];
	}
while(CrcTemp.words.high>0) 
	{
    CrcTemp.dwords = (uint32)(CrcTemp.words.high+CrcTemp.words.low);
    }
CrcTemp.words.low = 0xffff - CrcTemp.words.low;
return(CrcTemp.words.low);
}
uint16  CreateIcmpCrc1(union icmp_rc icmppk) 
{
uint8 i;
union  w  CrcTemp;
CrcTemp.dwords=0;
for(i=0;i<18;i++)				//ÓÃÓÚ²éÑ¯µÄICMP±¨ÎÄ¹²40×Ö½Ú
	{
	CrcTemp.dwords = CrcTemp.dwords + icmppk.words.wordbuf[i];
	}
while(CrcTemp.words.high>0) 
	{
    CrcTemp.dwords = (uint32)(CrcTemp.words.high+CrcTemp.words.low);
    }
CrcTemp.words.low = 0xffff - CrcTemp.words.low;
return(CrcTemp.words.low);
}
/**********************************************************************
**º¯ÊýÔ­ÐÍ£º    void icmp_process(unsigned char  * ICMPSENDPTR)
**Èë¿Ú²ÎÊý:º	* ICMPSENDPTR	:	Êý¾ÝÖ¸Õë
**³ö¿Ú²ÎÊý:		ÎÞ
**·µ »Ø Öµ£º	ÎÞ    
**Ëµ    Ã÷£º	´¦ÀíICMP°ü 
************************************************************************/
void icmp_process(uint8  * ICMPSENDPTR,uint8 num)  
{
uint8 i,temp;
struct _pkst SendIcmpData;  
	if(((icmppro*)ICMPSENDPTR)->icmpf.e_icmp.type==0x08)//ÇëÇó£¡£¡
	{
		((icmppro*)ICMPSENDPTR)->icmpf.e_icmp.type=0;//»Ø¸´
		((icmppro*)ICMPSENDPTR)->ipf.e_ip.ttl--;
		for (i = 0; i < 4; i++)//IP·­×ª
    	{
        	temp = ((icmppro*)ICMPSENDPTR)->ipf.e_ip.SourceIp[i];
        	((icmppro*)ICMPSENDPTR)->ipf.e_ip.SourceIp[i] = ((icmppro*)ICMPSENDPTR)->ipf.e_ip.DestId[i];
        	((icmppro*)ICMPSENDPTR)->ipf.e_ip.DestId[i] = temp;
    	}
		((icmppro*)ICMPSENDPTR)->ipf.e_ip.Crc=0;
		OS_ENTER_CRITICAL();
		((icmppro*)ICMPSENDPTR)->ipf.e_ip.Crc=CreateIpHeadCrc(ICMPSENDPTR);//(((icmppro*)ICMPSENDPTR)->ipf);
#ifdef Little_End
		((icmppro*)ICMPSENDPTR)->ipf.e_ip.Crc=swap_int16(((icmppro*)ICMPSENDPTR)->ipf.e_ip.Crc);
#endif
		OS_EXIT_CRITICAL();
		((icmppro*)ICMPSENDPTR)->icmpf.e_icmp.Crc=0;
		((icmppro*)ICMPSENDPTR)->icmpf.e_icmp.Crc=CreateIcmpCrc(((icmppro*)ICMPSENDPTR)->icmpf);
		SendIcmpData.length=((icmppro*)ICMPSENDPTR)->ipf.e_ip.TotalLen;
#ifdef Little_End
		SendIcmpData.length=swap_int16(SendIcmpData.length);
#endif
		SendIcmpData.STPTR=NULL;
		SendIcmpData.DAPTR=ICMPSENDPTR;
		OSSemPend(SendFlag,5,&temp);
		if(temp==OS_NO_ERR)
		{
			Send_Ip_To_LLC(&SendIcmpData,((icmppro*)ICMPSENDPTR)->ipf.e_ip.DestId,num);
			OSSemPost(SendFlag);
		}
	}
	else if(((icmppro*)ICMPSENDPTR)->icmpf.e_icmp.type==0x00)//»Ø¸´
	{
		i=i;
	}

}
void Icmp_Send(uint8 type,uint8 option,uint8 *so_ip,uint8 *de_ip, uint8 * REC_FRAME )
{
struct _pkst SendPingData;
union icmp_rc IcmpPacked;
//static uint16 temp_findex=0x2345;
	OS_ENTER_CRITICAL();
	memcpy (&(IcmpPacked.e_icmp.icmpdata), REC_FRAME, 28);
	OS_EXIT_CRITICAL();
	
	IcmpPacked.e_icmp.type=type;
	IcmpPacked.e_icmp.option=option;
	IcmpPacked.e_icmp.Crc=0;
	IcmpPacked.e_icmp.id=0;
	IcmpPacked.e_icmp.seq=0;
	IcmpPacked.e_icmp.Crc=CreateIcmpCrc1(IcmpPacked);
	SendPingData.length=36;
	SendPingData.STPTR=NULL;
	SendPingData.DAPTR=( uint8 * )&IcmpPacked;
	Send_Ip_Frame	(
			&SendPingData,
			de_ip,
			so_ip,
			1
		);
}
void Ping_Precess(uint8 * de_ip,uint8 * so_ip) 
{
struct _pkst SendPingData;
union icmp_rc IcmpPacked;
static uint16 temp_findex=0x2345;
IcmpPacked.e_icmp.type=0x08;
IcmpPacked.e_icmp.option=0;
IcmpPacked.e_icmp.Crc=0;
#ifdef Big_End
IcmpPacked.e_icmp.id=0x0300;
#endif
#ifdef Little_End
IcmpPacked.e_icmp.id=0x0003;
#endif
IcmpPacked.e_icmp.seq=temp_findex++;
IcmpPacked.e_icmp.Crc=CreateIcmpCrc(IcmpPacked);
SendPingData.length=40;
SendPingData.STPTR=NULL;
SendPingData.DAPTR=( uint8 * )&IcmpPacked;
Send_Ip_Frame	(
			&SendPingData,
			de_ip,
			so_ip,
			1
		);
}
