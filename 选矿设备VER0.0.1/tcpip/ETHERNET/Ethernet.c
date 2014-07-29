#define ETHERNET_GLOBALS
#include "config.h"
#include "../include/cfg_net.h"

uint8 My_Ip_1ID[4]=MY_IP_1ID;
uint8 My_Gateway_1ID[4]=MY_GATEWAY_1ID;
uint8 MY_IP_MARK_1ID[4]=IP_MARK_1ID;
uint8 MY_MAC_1ID[6]=My_Mac_1ID;//MAC��ַ
#if (MAX_NET_PORT>=2)
uint8 My_Ip_2ID[4]=MY_IP_2ID;
uint8 My_Gateway_2ID[4]=MY_GATEWAY_2ID;
uint8 MY_IP_MARK_2ID[4]=IP_MARK_2ID;
uint8 MY_MAC_2ID[6]=My_Mac_2ID;//MAC��ַ
#endif
/**********************************************************************
**����ԭ�ͣ�    	unsigned char Send_ethernet_Frame(struct _pkst *TxdData,unsigned char * de_mac,unsigned char PROTOCOL)
**��ڲ���:�		*TxdData 	: ���ͽṹָ��
**		* de_mac     	: MAC��ַָ��
**		NextProtocal 	: ��һ��Э�飬����IPЭ���ARPЭ��(�磺IP_PACKED��ARP_PACKED)
**���ڲ���:	unsigned char 
**�� �� ֵ��	1
**˵    ����	ΪIP��ARP���ݰ�����ETHERNET��ͷ�������͡�
************************************************************************/
uint8 Send_ethernet_Frame	(
					 struct _pkst *TxdData,//�ṹָ��
					 uint8 * de_mac,			//�Է���MAC��ַָ��
					 uint8 PROTOCOL,			//IPЭ���ARPЭ��
					 uint8	num
				 )  
{ //���û��MAC��ַ���ͷ���һ��ARP�����õ�MAC��ַ�ٷ���
ipethernet ethernet_head; //ETHERNET��������
struct _pkst PKethernet;
	
	ethernet_head.DestMacId[0]=*de_mac;	//���öԷ�MAC
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
	ethernet_head.SourceMacId[0]=NetPort[num].My_Mac[0];//���ñ���MAC��ַ
	ethernet_head.SourceMacId[1]=NetPort[num].My_Mac[1];
	ethernet_head.SourceMacId[2]=NetPort[num].My_Mac[2];
	ethernet_head.SourceMacId[3]=NetPort[num].My_Mac[3];
	ethernet_head.SourceMacId[4]=NetPort[num].My_Mac[4];
	ethernet_head.SourceMacId[5]=NetPort[num].My_Mac[5];
#ifdef Little_End
	if(PROTOCOL==IP_PACKED)//�����IP��������Ϊ0X0800
		ethernet_head.NextProtocal=0X0008;
	else if(PROTOCOL==ARP_PACKED)//�����ARP��������Ϊ0X0806
		ethernet_head.NextProtocal=0X0608;//0X0806;
#endif
#ifdef Big_End
	if(PROTOCOL==IP_PACKED)//�����IP��������Ϊ0X0800
		ethernet_head.NextProtocal=0X0800;
	else if(PROTOCOL==ARP_PACKED)//�����ARP��������Ϊ0X0806
		ethernet_head.NextProtocal=0X0806;//0X0806;
#endif
	PKethernet.STPTR=TxdData;//ָ��ǰһ���ṹ����
	PKethernet.length=14;			//ETHERNET��ͷ�ĳ���
	PKethernet.DAPTR=(uint8 EX_RAM *)&ethernet_head;//ETHERNET��ͷ��ָ��
	OS_ENTER_CRITICAL();//����
	switch(num)
	{
	case 0: 
		Send_Packet(&PKethernet);//���͸�֡
		break;
	case 1: 
		break;
	case 2: 
		break;
	default:
		break;
	}
	OS_EXIT_CRITICAL();//�ָ�
	return(1);
}
/**********************************************************************
**����ԭ�ͣ�    uint8 Send_Ip_To_LLC(struct _pkst *TxdData,unsigned char * de_ip)
**��ڲ���:�	*TxdData	     	: ���ͽṹָ��
**				* de_ip   	     	: IP��ַָ��
**���ڲ���:		unsigned char 
**�� �� ֵ��	1					:OK
0					:FAIL
**˵    ����	ΪIP���ݰ���Ŀ��IP����MAC��ַ�������͡�
************************************************************************/
uint8 Send_Ip_To_LLC(struct _pkst *TxdData,uint8 * de_ip,uint8 num) 
{
	
	uint8 i;
	//����ð���
	if((de_ip[0]&NetPort[num].My_Ip_Mark[0])==(NetPort[num].My_Ip[0]&NetPort[num].My_Ip_Mark[0]))
	if((de_ip[1]&NetPort[num].My_Ip_Mark[1])==(NetPort[num].My_Ip[1]&NetPort[num].My_Ip_Mark[1]))
	if((de_ip[2]&NetPort[num].My_Ip_Mark[2])==(NetPort[num].My_Ip[2]&NetPort[num].My_Ip_Mark[2]))
	if((de_ip[3]&NetPort[num].My_Ip_Mark[3])==(NetPort[num].My_Ip[3]&NetPort[num].My_Ip_Mark[3]))
	{
		i=0;//����һ��MAC��
		do
		{	
			OS_ENTER_CRITICAL();//����
			if(NetPort[num].ARP_TERM[i].IP_NUM[2]==((IP_NUMBER*)de_ip)->IP[2])
			if(NetPort[num].ARP_TERM[i].IP_NUM[3]==((IP_NUMBER*)de_ip)->IP[3])//���ARP��������������ȶ���TTL>0��ʾ���ж�Ӧ��MAC
			if(NetPort[num].ARP_TERM[i].TTL>0)
			{
				NetPort[num].ARP_TERM[i].TTL=100;//����
				Send_ethernet_Frame(TxdData,NetPort[num].ARP_TERM[i].MAC_NUM,IP_PACKED,num);
				OS_EXIT_CRITICAL();//����
				return(1);
			}
			OS_EXIT_CRITICAL();//����
			i++;
		}
		while(i<MAX_ARP_TERM);//���arp������˻�û�У����˳�
		Arp_Request(de_ip,num);//����Է�MAC
		return(0);
	}
	Send_ethernet_Frame(TxdData,Gateway_MAC,IP_PACKED,num);
	return(1);
}

uint8 Rec_Ethernet_Packed(uint8 * RecData,uint8 num)
{
uint8 i;
	
#ifdef Big_End
	if(((ipethernet*)RecData)->NextProtocal==0x0800)//���Լ��ٶ����ݻ���ȡ������
#endif
#ifdef Little_End
	if(((ipethernet*)RecData)->NextProtocal==0x0008)//���Լ��ٶ����ݻ���ȡ������
#endif
	{
		RecData=RecData+14;//ָ��IP��ͷ
		//����Ƿ����ڱ�IP�Σ����򲻱��浽ARP����С�		
		if((((eip*)RecData)->SourceIp[0]&NetPort[num].My_Ip_Mark[0])==(NetPort[num].My_Ip[0]&NetPort[num].My_Ip_Mark[0]))
		if((((eip*)RecData)->SourceIp[1]&NetPort[num].My_Ip_Mark[1])==(NetPort[num].My_Ip[1]&NetPort[num].My_Ip_Mark[1]))
		if((((eip*)RecData)->SourceIp[2]&NetPort[num].My_Ip_Mark[2])==(NetPort[num].My_Ip[2]&NetPort[num].My_Ip_Mark[2]))
		if((((eip*)RecData)->SourceIp[3]&NetPort[num].My_Ip_Mark[3])==(NetPort[num].My_Ip[3]&NetPort[num].My_Ip_Mark[3]))
		{
			for(i=0;i<MAX_ARP_TERM;i++)
			{//ͬһ���������65535̨�ڵ�
				if(NetPort[num].ARP_TERM[i].IP_NUM[2]==((eip*)RecData)->SourceIp[2])
				if(NetPort[num].ARP_TERM[i].IP_NUM[3]==((eip*)RecData)->SourceIp[3])
				if(NetPort[num].ARP_TERM[i].TTL>0)//����Ѿ������ж�Ӧ��MAC��ַ����TTL��Ч
				{//ˢ��TTL,����IP��
					NetPort[num].ARP_TERM[i].TTL=100;
					IP_PROCESS(RecData,num);
					return(2);
				}
			}
			//ARP��û�ж�Ӧ��MAC��ַ
			for(i=0;i<MAX_ARP_TERM;i++)
			{
				if(NetPort[num].ARP_TERM[i].TTL==0)//�����ARP������Ч
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
					RecData=RecData+14;//������IP�Ͷ�Ӧ��MAC��ַ
					IP_PROCESS(RecData,num);
					return(2);
				}
			}
			//MAC���������
			IP_PROCESS(RecData,num);
			return(4);
		}//if((((eip*)TEMP)->
		//�Ǳ���IP
		IP_PROCESS(RecData,num);
		return(3);
	}//if(((ipethernet*)
#ifdef Big_End
	else if(((ipethernet*)RecData)->NextProtocal==0x0806)//���Լ��ٶ����ݻ���ȡ������
#endif
#ifdef Little_End
	else if(((ipethernet*)RecData)->NextProtocal==0x0608)//���Լ��ٶ����ݻ���ȡ������
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