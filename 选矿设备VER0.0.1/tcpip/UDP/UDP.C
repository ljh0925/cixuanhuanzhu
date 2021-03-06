#define UDP_GLOBALS
#include "config.h"
#include "../include/cfg_net.h"

/**********************************************************************
**函数原型:	uint16 CheckSumUdp(uint8 *check,uint16 length)
**入口参数:�		check	:	被校验数据起始地址
		length	:	被校验数据长度，单位字
**出口参数:	无
**返 回 值:	~((sum)&0xffff))		       
**说    明:	计算校验和 
************************************************************************/
//=================================================================================
uint16 CheckSumUdp(uint8 *check,uint16 length)
{
uint32 sum=0;
uint16 i;
	for (i=0;i<(length)/2;i++)
	{
		sum = sum + ((uint32)check[2*i]<<8)+(uint32)check[2*i+1];
	}
	if(length&0x0001)//长度为奇数个时，要进行该操作
	{
		sum = sum + ((uint32)check[2*i]<<8);
	}
	sum = (sum&0xffff) + ((sum>>16)&0xffff);//高16位和低16位相加
	if(sum & 0xffff0000)
	{//表示有进位
		sum++;
	}
	return ( (uint16)(~((sum)&0xffff))  );
}

uint8 Udp_Process(Rec_Ptr * Udp_Rec) 
{
static uint8	UDP_REC_BUFF_NUM=0;
uint8	i,j;
	OS_ENTER_CRITICAL();
	memcpy (&(UdpRecBuff[UDP_REC_BUFF_NUM]), (*Udp_Rec).RecDataPtr, (*Udp_Rec).length);
	OS_EXIT_CRITICAL();
#ifdef Little_End
	UdpRecBuff[UDP_REC_BUFF_NUM].DestPort=swap_int16(UdpRecBuff[UDP_REC_BUFF_NUM].DestPort);
#endif
	i=0;
	do
	{
		if(UdpStatus[i].My_Port==UdpRecBuff[UDP_REC_BUFF_NUM].DestPort)//有可能组播
		{
			break;
		}
		i++;
	}
	while(i<MAX_UDP_LINKS);
	if(i>=MAX_UDP_LINKS)
	{
		Icmp_Send(3,3,((Rec_Ptr*)Udp_Rec)->My_Ip,((Rec_Ptr*)Udp_Rec)->ip, (*Udp_Rec).RecDataPtr-20 );
		return 0;
	}
	OS_ENTER_CRITICAL();
	j=0;
	do
	{
		if(UdpStatus[i].info[j].num==UdpMaxRec)
			break;
		j++;
	}
	while(j<UdpMaxRec);
	if(j>=UdpMaxRec)
	{
		OS_EXIT_CRITICAL();
		return (1);
	}
	UdpStatus[i].info[j].De_Port=UdpRecBuff[UDP_REC_BUFF_NUM].SourcePort;
#ifdef Little_End
	UdpStatus[i].info[j].De_Port=swap_int16(UdpStatus[i].info[j].De_Port);
#endif
	UdpStatus[i].info[j].De_Ip[0]=(*Udp_Rec).ip[0];
	UdpStatus[i].info[j].De_Ip[1]=(*Udp_Rec).ip[1];
	UdpStatus[i].info[j].De_Ip[2]=(*Udp_Rec).ip[2];
	UdpStatus[i].info[j].De_Ip[3]=(*Udp_Rec).ip[3];
	UdpStatus[i].info[j].num=UDP_REC_BUFF_NUM;
	UdpStatus[i].info[j].len=(*Udp_Rec).length;
	OS_EXIT_CRITICAL();
	UDP_REC_BUFF_NUM++;
	if(UDP_REC_BUFF_NUM>=UdpMaxRec)
		UDP_REC_BUFF_NUM=0;
	OSSemPost(UdpStatus[i].UdpSemRec);
	return (2);
}

void Udp_Initial(void)
{	
uint8 i,j;
	for(i=0;i<MAX_UDP_LINKS;i++)
	{
		UdpStatus[i].My_Port=0;
		UdpStatus[i].My_Ip[0]=0xff;
		UdpStatus[i].My_Ip[1]=0xff;
		UdpStatus[i].My_Ip[2]=0xff;
		UdpStatus[i].My_Ip[3]=0xff;
		UdpStatus[i].Rec_Sta=0;
		UdpStatus[i].UdpSemRec=OSSemCreate(0);
		for(j=0;j<UdpMaxRec;j++)
		{
			UdpStatus[i].info[j].De_Port=0;
			UdpStatus[i].info[j].num=UdpMaxRec;
			UdpStatus[i].info[j].len=0;
		}
	}
}


