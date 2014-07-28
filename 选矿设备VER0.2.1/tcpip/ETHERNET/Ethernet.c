#define ETHERNET_GLOBALS
#include "config.h"
#include "../include/cfg_net.h"

uint8 My_Ip_1ID[4]=MY_IP_1ID;
uint8 My_Gateway_1ID[4]=MY_GATEWAY_1ID;
uint8 MY_IP_MARK_1ID[4]=IP_MARK_1ID;
uint8 MY_MAC_1ID[6]=My_Mac_1ID;//MACµÿ÷∑
#if (MAX_NET_PORT>=2)
uint8 My_Ip_2ID[4]=MY_IP_2ID;
uint8 My_Gateway_2ID[4]=MY_GATEWAY_2ID;
uint8 MY_IP_MARK_2ID[4]=IP_MARK_2ID;
uint8 MY_MAC_2ID[6]=My_Mac_2ID;//MACµÿ÷∑
#endif
/**********************************************************************
**∫Ø ˝‘≠–Õ£∫    	unsigned char Send_ethernet_Frame(struct _pkst *TxdData,unsigned char * de_mac,unsigned char PROTOCOL)
**»Îø⁄≤Œ ˝:∫		*TxdData 	: ∑¢ÀÕΩ·ππ÷∏’Î
**		* de_mac     	: MACµÿ÷∑÷∏’Î
**		NextProtocal 	: œ¬“ª≤„–≠“È£¨¿˝»ÁIP–≠“ÈªÚARP–≠“È(»Á£∫IP_PACKED°¢ARP_PACKED)
**≥ˆø⁄≤Œ ˝:	unsigned char 
**∑µ ªÿ ÷µ£∫	1
**Àµ    √˜£∫	Œ™IP°¢ARP ˝æ›∞¸≤˙…˙ETHERNET∞¸Õ∑£¨≤¢∑¢ÀÕ°£
************************************************************************/
uint8 Send_ethernet_Frame	(
					 struct _pkst *TxdData,//Ω·ππ÷∏’Î
					 uint8 * de_mac,			//∂‘∑ΩµƒMACµÿ÷∑÷∏’Î
					 uint8 PROTOCOL,			//IP–≠“ÈªÚARP–≠“È
					 uint8	num
				 )  
{ //»Áπ˚√ª”–MACµÿ÷∑£¨æÕ∑¢ÀÕ“ª∏ˆARP∞¸°£µ√µΩMACµÿ÷∑‘Ÿ∑¢°£
ipethernet ethernet_head; //ETHERNET¥¶¿Ìª∫¥Ê«¯
struct _pkst PKethernet;
	
	ethernet_head.DestMacId[0]=*de_mac;	//…Ë÷√∂‘∑ΩMAC
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
	ethernet_head.SourceMacId[0]=NetPort[num].My_Mac[0];//…Ë÷√±æª˙MACµÿ÷∑
	ethernet_head.SourceMacId[1]=NetPort[num].My_Mac[1];
	ethernet_head.SourceMacId[2]=NetPort[num].My_Mac[2];
	ethernet_head.SourceMacId[3]=NetPort[num].My_Mac[3];
	ethernet_head.SourceMacId[4]=NetPort[num].My_Mac[4];
	ethernet_head.SourceMacId[5]=NetPort[num].My_Mac[5];
#ifdef Little_End
	if(PROTOCOL==IP_PACKED)//»Áπ˚ «IP∞¸£¨æÕ…ËŒ™0X0800
		ethernet_head.NextProtocal=0X0008;
	else if(PROTOCOL==ARP_PACKED)//»Áπ˚ «ARP∞¸£¨æÕ…ËŒ™0X0806
		ethernet_head.NextProtocal=0X0608;//0X0806;
#endif
#ifdef Big_End
	if(PROTOCOL==IP_PACKED)//»Áπ˚ «IP∞¸£¨æÕ…ËŒ™0X0800
		ethernet_head.NextProtocal=0X0800;
	else if(PROTOCOL==ARP_PACKED)//»Áπ˚ «ARP∞¸£¨æÕ…ËŒ™0X0806
		ethernet_head.NextProtocal=0X0806;//0X0806;
#endif
	PKethernet.STPTR=TxdData;//÷∏œÚ«∞“ª∏ˆΩ·ππ ˝◊È
	PKethernet.length=14;			//ETHERNET±®Õ∑µƒ≥§∂»
	PKethernet.DAPTR=(uint8 EX_RAM *)&ethernet_head;//ETHERNET±®Õ∑µƒ÷∏’Î
	OS_ENTER_CRITICAL();//±£ª§
	switch(num)
	{
	case 0: 
		Send_Packet(&PKethernet);//∑¢ÀÕ∏√÷°
		break;
	case 1: 
		break;
	case 2: 
		break;
	default:
		break;
	}
	OS_EXIT_CRITICAL();//ª÷∏¥
	return(1);
}
/**********************************************************************
**∫Ø ˝‘≠–Õ£∫    uint8 Send_Ip_To_LLC(struct _pkst *TxdData,unsigned char * de_ip)
**»Îø⁄≤Œ ˝:∫	*TxdData	     	: ∑¢ÀÕΩ·ππ÷∏’Î
**				* de_ip   	     	: IPµÿ÷∑÷∏’Î
**≥ˆø⁄≤Œ ˝:		unsigned char 
**∑µ ªÿ ÷µ£∫	1					:OK
0					:FAIL
**Àµ    √˜£∫	Œ™IP ˝æ›∞¸µƒƒø±ÍIP≤È’“MACµÿ÷∑£¨≤¢∑¢ÀÕ°£
************************************************************************/
uint8 Send_Ip_To_LLC(struct _pkst *TxdData,uint8 * de_ip,uint8 num) 
{
	
	uint8 i;
	//»Áπ˚∏√∞¸‘⁄
	if((de_ip[0]&NetPort[num].My_Ip_Mark[0])==(NetPort[num].My_Ip[0]&NetPort[num].My_Ip_Mark[0]))
	if((de_ip[1]&NetPort[num].My_Ip_Mark[1])==(NetPort[num].My_Ip[1]&NetPort[num].My_Ip_Mark[1]))
	if((de_ip[2]&NetPort[num].My_Ip_Mark[2])==(NetPort[num].My_Ip[2]&NetPort[num].My_Ip_Mark[2]))
	if((de_ip[3]&NetPort[num].My_Ip_Mark[3])==(NetPort[num].My_Ip[3]&NetPort[num].My_Ip_Mark[3]))
	{
		i=0;//≤È’““ª¥ŒMAC±Ì°£
		do
		{	
			OS_ENTER_CRITICAL();//±£ª§
			if(NetPort[num].ARP_TERM[i].IP_NUM[2]==((IP_NUMBER*)de_ip)->IP[2])
			if(NetPort[num].ARP_TERM[i].IP_NUM[3]==((IP_NUMBER*)de_ip)->IP[3])//»Áπ˚ARP±Ìµƒ◊Ó∫Û¡Ω∏ˆ ˝œ‡µ»∂¯«“TTL>0±Ì æ£¨”–∂‘”¶µƒMAC
			if(NetPort[num].ARP_TERM[i].TTL>0)
			{
				NetPort[num].ARP_TERM[i].TTL=100;//∑¢ÀÕ
				Send_ethernet_Frame(TxdData,NetPort[num].ARP_TERM[i].MAC_NUM,IP_PACKED,num);
				OS_EXIT_CRITICAL();//±£ª§
				return(1);
			}
			OS_EXIT_CRITICAL();//±£ª§
			i++;
		}
		while(i<MAX_ARP_TERM);//»Áπ˚arp±Ì≤ÈÕÍ¡Àªπ√ª”–£¨æÕÕÀ≥ˆ
		Arp_Request(de_ip,num);//«Î«Û∂‘∑ΩMAC
		return(0);
	}
	Send_ethernet_Frame(TxdData,Gateway_MAC,IP_PACKED,num);
	return(1);
}

uint8 Rec_Ethernet_Packed(uint8 * RecData,uint8 num)
{
uint8 i;
	
#ifdef Big_End
	if(((ipethernet*)RecData)->NextProtocal==0x0800)//ø…“‘ºı…Ÿ∂‘ ˝æ›ª∫≥Â»°µƒ¿˚”√
#endif
#ifdef Little_End
	if(((ipethernet*)RecData)->NextProtocal==0x0008)//ø…“‘ºı…Ÿ∂‘ ˝æ›ª∫≥Â»°µƒ¿˚”√
#endif
	{
		RecData=RecData+14;//÷∏œÚIP∞¸Õ∑
		//ºÏ≤È «∑Ò Ù”⁄±æIP∂Œ£¨∑Ò‘Ú≤ª±£¥ÊµΩARP±Ì÷÷–°£		
		if((((eip*)RecData)->SourceIp[0]&NetPort[num].My_Ip_Mark[0])==(NetPort[num].My_Ip[0]&NetPort[num].My_Ip_Mark[0]))
		if((((eip*)RecData)->SourceIp[1]&NetPort[num].My_Ip_Mark[1])==(NetPort[num].My_Ip[1]&NetPort[num].My_Ip_Mark[1]))
		if((((eip*)RecData)->SourceIp[2]&NetPort[num].My_Ip_Mark[2])==(NetPort[num].My_Ip[2]&NetPort[num].My_Ip_Mark[2]))
		if((((eip*)RecData)->SourceIp[3]&NetPort[num].My_Ip_Mark[3])==(NetPort[num].My_Ip[3]&NetPort[num].My_Ip_Mark[3]))
		{
			for(i=0;i<MAX_ARP_TERM;i++)
			{//Õ¨“ªÕ¯∂Œƒ⁄◊Ó∂‡65535Ã®Ω⁄µ„
				if(NetPort[num].ARP_TERM[i].IP_NUM[2]==((eip*)RecData)->SourceIp[2])
				if(NetPort[num].ARP_TERM[i].IP_NUM[3]==((eip*)RecData)->SourceIp[3])
				if(NetPort[num].ARP_TERM[i].TTL>0)//»Áπ˚“—æ≠±£¥Ê”–∂‘”¶µƒMACµÿ÷∑∂¯«“TTL”––ß
				{//À¢–¬TTL,¥¶¿ÌIP∞¸
					NetPort[num].ARP_TERM[i].TTL=100;
					IP_PROCESS(RecData,num);
					return(2);
				}
			}
			//ARP±Ì√ª”–∂‘”¶µƒMACµÿ÷∑
			for(i=0;i<MAX_ARP_TERM;i++)
			{
				if(NetPort[num].ARP_TERM[i].TTL==0)//»Áπ˚∏√ARP±ÌœÓŒﬁ–ß
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
					RecData=RecData+14;//±£¥Ê∆‰IP∫Õ∂‘”¶µƒMACµÿ÷∑
					IP_PROCESS(RecData,num);
					return(2);
				}
			}
			//MAC±Ì“Á≥ˆ£°£°
			IP_PROCESS(RecData,num);
			return(4);
		}//if((((eip*)TEMP)->
		//∑«±æµÿIP
		IP_PROCESS(RecData,num);
		return(3);
	}//if(((ipethernet*)
#ifdef Big_End
	else if(((ipethernet*)RecData)->NextProtocal==0x0806)//ø…“‘ºı…Ÿ∂‘ ˝æ›ª∫≥Â»°µƒ¿˚”√
#endif
#ifdef Little_End
	else if(((ipethernet*)RecData)->NextProtocal==0x0608)//ø…“‘ºı…Ÿ∂‘ ˝æ›ª∫≥Â»°µƒ¿˚”√
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