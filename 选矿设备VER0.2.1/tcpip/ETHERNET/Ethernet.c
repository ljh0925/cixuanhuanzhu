#define ETHERNET_GLOBALS
#include "config.h"
#include "../include/cfg_net.h"

uint8 My_Ip_1ID[4]=MY_IP_1ID;
uint8 My_Gateway_1ID[4]=MY_GATEWAY_1ID;
uint8 MY_IP_MARK_1ID[4]=IP_MARK_1ID;
uint8 MY_MAC_1ID[6]=My_Mac_1ID;//MAC地址
#if (MAX_NET_PORT>=2)
uint8 My_Ip_2ID[4]=MY_IP_2ID;
uint8 My_Gateway_2ID[4]=MY_GATEWAY_2ID;
uint8 MY_IP_MARK_2ID[4]=IP_MARK_2ID;
uint8 MY_MAC_2ID[6]=My_Mac_2ID;//MAC地址
#endif
/**********************************************************************
**函数原型：    	unsigned char Send_ethernet_Frame(struct _pkst *TxdData,unsigned char * de_mac,unsigned char PROTOCOL)
**入口参数:�		*TxdData 	: 发送结构指针
**		* de_mac     	: MAC地址指针
**		NextProtocal 	: 下一层协议，例如IP协议或ARP协议(如：IP_PACKED、ARP_PACKED)
**出口参数:	unsigned char 
**返 回 值：	1
**说    明：	为IP、ARP数据包产生ETHERNET包头，并发送。
************************************************************************/
uint8 Send_ethernet_Frame	(
					 struct _pkst *TxdData,//结构指针
					 uint8 * de_mac,			//对方的MAC地址指针
					 uint8 PROTOCOL,			//IP协议或ARP协议
					 uint8	num
				 )  
{ //如果没有MAC地址，就发送一个ARP包。得到MAC地址再发。
ipethernet ethernet_head; //ETHERNET处理缓存区
struct _pkst PKethernet;
	
	ethernet_head.DestMacId[0]=*de_mac;	//设置对方MAC
	de_mac++;
	ethernet_head.DestMacId[1]=*de_mac;
	de_mac++;
	ethernet_head.DestMacId[2]=*de_mac;
	de_mac++;
	ethernet_head.DestMacId[3]=*de_mac;
	de_mac++;
	ethernet_head.DestMacId[4]=*de_mac;
	de_mac++;
	ethernet_head.DestMacId[5]=*de_mac;
	ethernet_head.SourceMacId[0]=NetPort[num].My_Mac[0];//设置本机MAC地址
	ethernet_head.SourceMacId[1]=NetPort[num].My_Mac[1];
	ethernet_head.SourceMacId[2]=NetPort[num].My_Mac[2];
	ethernet_head.SourceMacId[3]=NetPort[num].My_Mac[3];
	ethernet_head.SourceMacId[4]=NetPort[num].My_Mac[4];
	ethernet_head.SourceMacId[5]=NetPort[num].My_Mac[5];
#ifdef Little_End
	if(PROTOCOL==IP_PACKED)//如果是IP包，就设为0X0800
		ethernet_head.NextProtocal=0X0008;
	else if(PROTOCOL==ARP_PACKED)//如果是ARP包，就设为0X0806
		ethernet_head.NextProtocal=0X0608;//0X0806;
#endif
#ifdef Big_End
	if(PROTOCOL==IP_PACKED)//如果是IP包，就设为0X0800
		ethernet_head.NextProtocal=0X0800;
	else if(PROTOCOL==ARP_PACKED)//如果是ARP包，就设为0X0806
		ethernet_head.NextProtocal=0X0806;//0X0806;
#endif
	PKethernet.STPTR=TxdData;//指向前一个结构数组
	PKethernet.length=14;			//ETHERNET报头的长度
	PKethernet.DAPTR=(uint8 EX_RAM *)&ethernet_head;//ETHERNET报头的指针
	OS_ENTER_CRITICAL();//保护
	switch(num)
	{
	case 0: 
		Send_Packet(&PKethernet);//发送该帧
		break;
	case 1: 
		break;
	case 2: 
		break;
	default:
		break;
	}
	OS_EXIT_CRITICAL();//恢复
	return(1);
}
/**********************************************************************
**函数原型：    uint8 Send_Ip_To_LLC(struct _pkst *TxdData,unsigned char * de_ip)
**入口参数:�	*TxdData	     	: 发送结构指针
**				* de_ip   	     	: IP地址指针
**出口参数:		unsigned char 
**返 回 值：	1					:OK
0					:FAIL
**说    明：	为IP数据包的目标IP查找MAC地址，并发送。
************************************************************************/
uint8 Send_Ip_To_LLC(struct _pkst *TxdData,uint8 * de_ip,uint8 num) 
{
	
	uint8 i;
	//如果该包在
	if((de_ip[0]&NetPort[num].My_Ip_Mark[0])==(NetPort[num].My_Ip[0]&NetPort[num].My_Ip_Mark[0]))
	if((de_ip[1]&NetPort[num].My_Ip_Mark[1])==(NetPort[num].My_Ip[1]&NetPort[num].My_Ip_Mark[1]))
	if((de_ip[2]&NetPort[num].My_Ip_Mark[2])==(NetPort[num].My_Ip[2]&NetPort[num].My_Ip_Mark[2]))
	if((de_ip[3]&NetPort[num].My_Ip_Mark[3])==(NetPort[num].My_Ip[3]&NetPort[num].My_Ip_Mark[3]))
	{
		i=0;//查找一次MAC表。
		do
		{	
			OS_ENTER_CRITICAL();//保护
			if(NetPort[num].ARP_TERM[i].IP_NUM[2]==((IP_NUMBER*)de_ip)->IP[2])
			if(NetPort[num].ARP_TERM[i].IP_NUM[3]==((IP_NUMBER*)de_ip)->IP[3])//如果ARP表的最后两个数相等而且TTL>0表示，有对应的MAC
			if(NetPort[num].ARP_TERM[i].TTL>0)
			{
				NetPort[num].ARP_TERM[i].TTL=100;//发送
				Send_ethernet_Frame(TxdData,NetPort[num].ARP_TERM[i].MAC_NUM,IP_PACKED,num);
				OS_EXIT_CRITICAL();//保护
				return(1);
			}
			OS_EXIT_CRITICAL();//保护
			i++;
		}
		while(i<MAX_ARP_TERM);//如果arp表查完了还没有，就退出
		Arp_Request(de_ip,num);//请求对方MAC
		return(0);
	}
	Send_ethernet_Frame(TxdData,Gateway_MAC,IP_PACKED,num);
	return(1);
}

uint8 Rec_Ethernet_Packed(uint8 * RecData,uint8 num)
{
uint8 i;
	
#ifdef Big_End
	if(((ipethernet*)RecData)->NextProtocal==0x0800)//可以减少对数据缓冲取的利用
#endif
#ifdef Little_End
	if(((ipethernet*)RecData)->NextProtocal==0x0008)//可以减少对数据缓冲取的利用
#endif
	{
		RecData=RecData+14;//指向IP包头
		//检查是否属于本IP段，否则不保存到ARP表种小�		
		if((((eip*)RecData)->SourceIp[0]&NetPort[num].My_Ip_Mark[0])==(NetPort[num].My_Ip[0]&NetPort[num].My_Ip_Mark[0]))
		if((((eip*)RecData)->SourceIp[1]&NetPort[num].My_Ip_Mark[1])==(NetPort[num].My_Ip[1]&NetPort[num].My_Ip_Mark[1]))
		if((((eip*)RecData)->SourceIp[2]&NetPort[num].My_Ip_Mark[2])==(NetPort[num].My_Ip[2]&NetPort[num].My_Ip_Mark[2]))
		if((((eip*)RecData)->SourceIp[3]&NetPort[num].My_Ip_Mark[3])==(NetPort[num].My_Ip[3]&NetPort[num].My_Ip_Mark[3]))
		{
			for(i=0;i<MAX_ARP_TERM;i++)
			{//同一网段内最多65535台节点
				if(NetPort[num].ARP_TERM[i].IP_NUM[2]==((eip*)RecData)->SourceIp[2])
				if(NetPort[num].ARP_TERM[i].IP_NUM[3]==((eip*)RecData)->SourceIp[3])
				if(NetPort[num].ARP_TERM[i].TTL>0)//如果已经保存有对应的MAC地址而且TTL有效
				{//刷新TTL,处理IP包
					NetPort[num].ARP_TERM[i].TTL=100;
					IP_PROCESS(RecData,num);
					return(2);
				}
			}
			//ARP表没有对应的MAC地址
			for(i=0;i<MAX_ARP_TERM;i++)
			{
				if(NetPort[num].ARP_TERM[i].TTL==0)//如果该ARP表项无效
				{
					NetPort[num].ARP_TERM[i].IP_NUM[0]=((eip*)RecData)->SourceIp[0];
					NetPort[num].ARP_TERM[i].IP_NUM[1]=((eip*)RecData)->SourceIp[1];
					NetPort[num].ARP_TERM[i].IP_NUM[2]=((eip*)RecData)->SourceIp[2];
					NetPort[num].ARP_TERM[i].IP_NUM[3]=((eip*)RecData)->SourceIp[3];
					RecData=RecData-14;
					NetPort[num].ARP_TERM[i].MAC_NUM[0]=((ipethernet*)RecData)->SourceMacId[0];
					NetPort[num].ARP_TERM[i].MAC_NUM[1]=((ipethernet*)RecData)->SourceMacId[1];
					NetPort[num].ARP_TERM[i].MAC_NUM[2]=((ipethernet*)RecData)->SourceMacId[2];
					NetPort[num].ARP_TERM[i].MAC_NUM[3]=((ipethernet*)RecData)->SourceMacId[3];
					NetPort[num].ARP_TERM[i].MAC_NUM[4]=((ipethernet*)RecData)->SourceMacId[4];
					NetPort[num].ARP_TERM[i].MAC_NUM[5]=((ipethernet*)RecData)->SourceMacId[5];
					NetPort[num].ARP_TERM[i].TTL=100;
					RecData=RecData+14;//保存其IP和对应的MAC地址
					IP_PROCESS(RecData,num);
					return(2);
				}
			}
			//MAC表溢出！！
			IP_PROCESS(RecData,num);
			return(4);
		}//if((((eip*)TEMP)->
		//非本地IP
		IP_PROCESS(RecData,num);
		return(3);
	}//if(((ipethernet*)
#ifdef Big_End
	else if(((ipethernet*)RecData)->NextProtocal==0x0806)//可以减少对数据缓冲取的利用
#endif
#ifdef Little_End
	else if(((ipethernet*)RecData)->NextProtocal==0x0608)//可以减少对数据缓冲取的利用
#endif
	{
		RecData=RecData+14;
		PROCESS_ARP_REC(RecData,num);
		return(1);
	}
	return(3);
}
void SetNetPort(void)
{
	uint8 i;
	for(i=0;i<4;i++)
	{
		NetPort[0].My_Ip[i]=My_Ip_1ID[i];
		NetPort[0].My_Gateway[i]=My_Gateway_1ID[i];
		NetPort[0].My_Ip_Mark[i]=MY_IP_MARK_1ID[i];
#if MAX_NET_PORT>=2
		NetPort[1].My_Ip[i]=My_Ip_2ID[i];
		NetPort[1].My_Gateway[i]=My_Gateway_2ID[i];
		NetPort[1].My_Ip_Mark[i]=MY_IP_MARK_2ID[i];
#endif
	}
	for(i=0;i<6;i++)
	{
		NetPort[0].My_Mac[i]=MY_MAC_1ID[i];
#if MAX_NET_PORT>=2
		NetPort[1].My_Mac[i]=MY_MAC_2ID[i];
#endif
	}
}