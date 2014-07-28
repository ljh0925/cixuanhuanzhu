#define ARP_GLOBALS
#include "config.h"
#include "../include/cfg_net.h"
typedef struct    { /* arp±¨ÎÄµÄÄÚÈİ×Ü³¤28×Ö½Ú*/
                                       
                 uint16 	HardwareType;  	    /*ÒÔÌ«ÍøÎª0x0001*/
                 uint16 	ProtocalType;   	/*ip Îª0X0800*/
                 uint8 	HardwareLen;        /*=0X06*/
                 uint8 	ProtocalLen;        /*=0X04*/
                 uint16 	Operation;         	/*²Ù×÷  0X0001ÎªÇëÇó   0X0002ÎªÓ¦´ğ  */
                                           	/*0X0003Îª·´ÏòµØÖ·ÇëÇó 0X0004Îª·´ÏòµØÖ·Ó¦´ğ*/
                 uint8 	SourceMacId[6];   	/*Ô´Íø¿¨µØÖ·*/
                 uint8 	SourceIp[4];       	/*Ô´IPµØÖ·*/
                 uint8 	DestMacId[6];     	/*Ä¿µÄÍø¿¨µØÖ·*/
                 uint8 	DestId[4];         	/*Ä¿µÄIPµØÖ·*/
                } arp ;
uint8 ARP_REP_MAC[6]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};
uint8 Ping_Ip_Address[4];
uint8 Ping_IP_TTL;
//uint8 My_Ip_Address[4]=MY_IP;
//uint8 My_Gateway_Ip_Address[4]=MY_GATEWAY;
//uint8 MY_IP_MARK[4]=IP_MARK_SETTING;
//struct a_t ARP_TERM[MAX_ARP_TERM];


//===========================================================
/**********************************************************************
**º¯ÊıÔ­ĞÍ£º    unsigned char Arp_Answer(unsigned char EX_RAM * ARP_REC_PTR)
**Èë¿Ú²ÎÊı:		* ARP_REC_PTR	:½ÓÊÕµ½µÄARPÇëÇó°üµÄÖ¸Õë
**³ö¿Ú²ÎÊı:		unsigned char
**·µ »Ø Öµ£º	0		:±¾×ÓÍøÄÚµÄARP»º³åÇøÒÑ¾­ÓĞMAC
**				1		:Ä¿±êIP²»ÊÇ±¾»ú²»×÷»ØÓ¦
**				2		:±¾×ÓÍøÄÚµÄ£¬µ«ARP»º³åÇø»¹Ã»ÓĞMAC
**				3		:²»ÊÇ±¾×ÓÍøµÄARPÇëÇó
**				4		:ARP»º³åÇøÒÑ¾­Âú
**Ëµ    Ã÷£º	¶ÔARPÇëÇó±¨ÎÄµÄÓ¦´ğ:Ìî³ä±¾µØÎïÀíµØÖ·,½«ARP²Ù×÷¸ÄÎª»Ø´ğ.
************************************************************************/
uint8 Arp_Answer(uint8 * ARP_REC_PTR,uint8 num) 
{
struct _pkst TxdArp;
uint8 SEND_ARP_MAC[6];
uint8 i;
//Èç¹ûÄ¿±êIPµØÖ·ÊÇ±¾»úIP
if (((arp*)ARP_REC_PTR)->DestId[0]==NetPort[num].My_Ip[0])
if (((arp*)ARP_REC_PTR)->DestId[1]==NetPort[num].My_Ip[1])
if (((arp*)ARP_REC_PTR)->DestId[2]==NetPort[num].My_Ip[2])
if (((arp*)ARP_REC_PTR)->DestId[3]==NetPort[num].My_Ip[3])
	{										//±íÊ¾ÊÇÒª½âÎö±¾µØIPµÄÇëÇó
	for(i=0;i<4;i++)						//¸´ÖÆ¶Ô·½IPµØÖ·£¬Ìî³äÔ´µØÖ·
		{
        SEND_ARP_MAC[i]=((arp*)ARP_REC_PTR)->SourceIp[i];
		((arp*)ARP_REC_PTR)->SourceIp[i]=NetPort[num].My_Ip[i];
		((arp*)ARP_REC_PTR)->DestId[i]=SEND_ARP_MAC[i];
		}
	for(i=0;i<6;i++)
   		{									//¸´ÖÆ¶Ô·½ÎïÀíµØÖ·»òÍø¹ØµØÖ·
        SEND_ARP_MAC[i]=((arp*)ARP_REC_PTR)->SourceMacId[i];
		((arp*)ARP_REC_PTR)->SourceMacId[i]=NetPort[num].My_Mac[i];
		((arp*)ARP_REC_PTR)->DestMacId[i]=SEND_ARP_MAC[i];
    	}
#ifdef Big_End
	((arp*)ARP_REC_PTR)->Operation=0x0002;	//±íÃ÷Êı¾İÖ¡ÎªARPÓ¦´ğ
#endif
#ifdef Little_End
	((arp*)ARP_REC_PTR)->Operation=0x0200;	//±íÃ÷Êı¾İÖ¡ÎªARPÓ¦´ğ
#endif
	TxdArp.STPTR=NULL;
	TxdArp.length=0x60;
	TxdArp.DAPTR=ARP_REC_PTR;
	Send_ethernet_Frame(&TxdArp,SEND_ARP_MAC,ARP_PACKED,num);//·¢ËÍARPÓ¦´ğÖ¡
	//Èç¹û·¢ËÍ·½ÊôÓÚ±¾Íø¶Î
		if((((arp*)ARP_REC_PTR)->SourceIp[0]&NetPort[num].My_Ip_Mark[0])==(NetPort[num].My_Ip[0]&NetPort[num].My_Ip_Mark[0]))
		if((((arp*)ARP_REC_PTR)->SourceIp[1]&NetPort[num].My_Ip_Mark[1])==(NetPort[num].My_Ip[1]&NetPort[num].My_Ip_Mark[1]))
		if((((arp*)ARP_REC_PTR)->SourceIp[2]&NetPort[num].My_Ip_Mark[2])==(NetPort[num].My_Ip[2]&NetPort[num].My_Ip_Mark[2]))
		if((((arp*)ARP_REC_PTR)->SourceIp[3]&NetPort[num].My_Ip_Mark[3])==(NetPort[num].My_Ip[3]&NetPort[num].My_Ip_Mark[3]))
		{
			//²éÕÒÓĞ·ñÊôÓÚ¸ÃIPµÄ¶ÔÓ¦MAC±í
			for(i=0;i<MAX_ARP_TERM;i++)
			{
				if(NetPort[num].ARP_TERM[i].IP_NUM[2]==((arp*)ARP_REC_PTR)->SourceIp[2])
				if(NetPort[num].ARP_TERM[i].IP_NUM[3]==((arp*)ARP_REC_PTR)->SourceIp[3])
				if(NetPort[num].ARP_TERM[i].TTL>0)
				{//ÓĞÔòË¢ĞÂ
					NetPort[num].ARP_TERM[i].TTL=100;
					return(0);
				}
			}
			//²éÕÒÓĞ·ñ¿ÕµÄMAC±íÏî
			for(i=0;i<MAX_ARP_TERM;i++)
			{
				if(NetPort[num].ARP_TERM[i].TTL==0)
				{//ÓĞÔò±£´æ
					NetPort[num].ARP_TERM[i].IP_NUM[0]=((arp*)ARP_REC_PTR)->SourceIp[0];
					NetPort[num].ARP_TERM[i].IP_NUM[1]=((arp*)ARP_REC_PTR)->SourceIp[1];
					NetPort[num].ARP_TERM[i].IP_NUM[2]=((arp*)ARP_REC_PTR)->SourceIp[2];
					NetPort[num].ARP_TERM[i].IP_NUM[3]=((arp*)ARP_REC_PTR)->SourceIp[3];
					NetPort[num].ARP_TERM[i].MAC_NUM[0]=((arp*)ARP_REC_PTR)->SourceMacId[0];
					NetPort[num].ARP_TERM[i].MAC_NUM[1]=((arp*)ARP_REC_PTR)->SourceMacId[1];
					NetPort[num].ARP_TERM[i].MAC_NUM[2]=((arp*)ARP_REC_PTR)->SourceMacId[2];
					NetPort[num].ARP_TERM[i].MAC_NUM[3]=((arp*)ARP_REC_PTR)->SourceMacId[3];
					NetPort[num].ARP_TERM[i].MAC_NUM[4]=((arp*)ARP_REC_PTR)->SourceMacId[4];
					NetPort[num].ARP_TERM[i].MAC_NUM[5]=((arp*)ARP_REC_PTR)->SourceMacId[5];
					NetPort[num].ARP_TERM[i].TTL=100;
					return(2);
				}
			}//FOR
			//MAC±íÒÑ¾­Âú
			return(4);
		}//IF ARP
		//²»ÊôÓÚÍ¬Ò»Íø¶ÎµÄ
		return (3);
   	}
	//Ä¿±êIP²»ÊÇ±¾»ú
	return (1);
}
uint8 REC_ARP_REQ(uint8 * ARP_REC_REQ_PTR,uint8 num)  
{
uint8 i;
//======================================================================
if(((arp*)ARP_REC_REQ_PTR)->SourceIp[0]==NetPort[num].My_Gateway[0])
if(((arp*)ARP_REC_REQ_PTR)->SourceIp[1]==NetPort[num].My_Gateway[1])
if(((arp*)ARP_REC_REQ_PTR)->SourceIp[2]==NetPort[num].My_Gateway[2])
if(((arp*)ARP_REC_REQ_PTR)->SourceIp[3]==NetPort[num].My_Gateway[3])
	{  				//±íÊ¾ÊÇÍø¹Ø¶ÔARPÇëÇóµÄ»Ø´ğ.
   	for (i=0;i<6;i++)
		{
		NetPort[num].My_Gateway_Mac[i]=((arp*)ARP_REC_REQ_PTR)->SourceMacId[i];
		}
   	NetPort[num].Gateway_IP_TTL=100;	//±íÊ¾Íø¹ØµØÖ·ÒÑµÃµ½½âÎö
	//return(3);
	}
	//Èç¹û·¢ËÍ·½ÊôÓÚ±¾Íø¶Î
if((((arp*)ARP_REC_REQ_PTR)->SourceIp[0]&NetPort[num].My_Ip_Mark[0])==(NetPort[num].My_Ip[0]&NetPort[num].My_Ip_Mark[0]))
if((((arp*)ARP_REC_REQ_PTR)->SourceIp[1]&NetPort[num].My_Ip_Mark[1])==(NetPort[num].My_Ip[1]&NetPort[num].My_Ip_Mark[1]))
if((((arp*)ARP_REC_REQ_PTR)->SourceIp[2]&NetPort[num].My_Ip_Mark[2])==(NetPort[num].My_Ip[2]&NetPort[num].My_Ip_Mark[2]))
if((((arp*)ARP_REC_REQ_PTR)->SourceIp[3]&NetPort[num].My_Ip_Mark[3])==(NetPort[num].My_Ip[3]&NetPort[num].My_Ip_Mark[3]))
{//²éÕÒÓĞ·ñÊôÓÚ¸ÃIPµÄ¶ÔÓ¦MAC±í
	for(i=0;i<MAX_ARP_TERM;i++)
	{
		if(NetPort[num].ARP_TERM[i].IP_NUM[2]==((arp*)ARP_REC_REQ_PTR)->SourceIp[2])
		if(NetPort[num].ARP_TERM[i].IP_NUM[3]==((arp*)ARP_REC_REQ_PTR)->SourceIp[3])
		if(NetPort[num].ARP_TERM[i].TTL>0)
		{//ÓĞÔòË¢ĞÂ
			NetPort[num].ARP_TERM[i].TTL=100;
			return(0);
		}
	}
	//²éÕÒÓĞ·ñ¿ÕµÄMAC±íÏî
	for(i=0;i<MAX_ARP_TERM;i++)
	{
		if(NetPort[num].ARP_TERM[i].TTL==0)
		{//ÓĞÔò±£´æ
			NetPort[num].ARP_TERM[i].IP_NUM[0]=((arp*)ARP_REC_REQ_PTR)->SourceIp[0];
			NetPort[num].ARP_TERM[i].IP_NUM[1]=((arp*)ARP_REC_REQ_PTR)->SourceIp[1];
			NetPort[num].ARP_TERM[i].IP_NUM[2]=((arp*)ARP_REC_REQ_PTR)->SourceIp[2];
			NetPort[num].ARP_TERM[i].IP_NUM[3]=((arp*)ARP_REC_REQ_PTR)->SourceIp[3];
			NetPort[num].ARP_TERM[i].MAC_NUM[0]=((arp*)ARP_REC_REQ_PTR)->SourceMacId[0];
			NetPort[num].ARP_TERM[i].MAC_NUM[1]=((arp*)ARP_REC_REQ_PTR)->SourceMacId[1];
			NetPort[num].ARP_TERM[i].MAC_NUM[2]=((arp*)ARP_REC_REQ_PTR)->SourceMacId[2];
			NetPort[num].ARP_TERM[i].MAC_NUM[3]=((arp*)ARP_REC_REQ_PTR)->SourceMacId[3];
			NetPort[num].ARP_TERM[i].MAC_NUM[4]=((arp*)ARP_REC_REQ_PTR)->SourceMacId[4];
			NetPort[num].ARP_TERM[i].MAC_NUM[5]=((arp*)ARP_REC_REQ_PTR)->SourceMacId[5];
			NetPort[num].ARP_TERM[i].TTL=100;
			return(2);
		}
	}
	//MAC±íÒÑ¾­Âú
	return(4);
}
//¶Ô·½IP¼´²»ÊÇ±¾Íø¶ÎÒ²²»ÊÇGATEWAY
return(1);
}
//===========================================================================
//=======================================================================
/**********************************************************************
**º¯ÊıÔ­ĞÍ£º    void PROCESS_ARP_REC(unsigned char EX_RAM * ARP_PTR)
**Èë¿Ú²ÎÊı:		* ARP_PTR	:½ÓÊÕµ½µÄARP°üµÄÖ¸Õë
**³ö¿Ú²ÎÊı:		ÎŞ
**·µ »Ø Öµ£º	ÎŞ
**Ëµ    Ã÷£º	¶ÔARP°ü½øĞĞ·ÖÀà´¦Àí
************************************************************************/
void PROCESS_ARP_REC(uint8 * ARP_PTR,uint8 num)  
{
//	EX_RAM PKST ARP_PACKED;
#ifdef Big_End
if(((arp*)ARP_PTR)->Operation==0X0001)
#endif
#ifdef Little_End
if(((arp*)ARP_PTR)->Operation==0X0100)
#endif
	{
		Arp_Answer(ARP_PTR,num);
	}
#ifdef Big_End
else if(((arp*)ARP_PTR)->Operation==0X0002)
#endif
#ifdef Little_End
else if(((arp*)ARP_PTR)->Operation==0X0200)
#endif
	{
		REC_ARP_REQ(ARP_PTR,num);
	}
	//¿ÉÌí¼ÓREARP²Ù×÷¡£

}
/**********************************************************************
**º¯ÊıÔ­ĞÍ:º    void        Arp_Request(unsigned char * ip_address)
**Èë¿Ú²ÎÊı:º	ip_address: Òª½âÎöµÄIPµØÖ·
**³ö¿Ú²ÎÊı:		ÎŞ
**·µ »Ø Öµ:º	ÎŞ
**Ëµ    Ã÷:º	ÇëÇó¶ÔÖ¸¶¨µÄIPµØÖ·½øĞĞ½âÎö,»ñÈ¡ÆäÎïÀíµØÖ·
************************************************************************/
void Arp_Request(uint8 * ip_address,uint8 num)
{
struct _pkst TxdArpReq;
uint8 ARPREQ[46];
uint8 i;
for(i=0;i<6;i++)			  					//¸´ÖÆ¶Ô·½Íø¿¨µØÖ·»òÍø¹ØµØÖ·
	{											
	((arp*)ARPREQ)->SourceMacId[i]=NetPort[num].My_Mac[i];
	((arp*)ARPREQ)->DestMacId[i]=0x00;	//arp±¨ÎÄµÄÄ¿µÄÎïÀíµØÖ·ÌîÎª0,ÓÉarp»Ø´ğ±¨ÎÄ										//¸ºÔğÌî³ä
    }
for(i=0;i<4;i++)								
	{
	((arp*)ARPREQ)->SourceIp[i]=NetPort[num].My_Ip[i];//Ìî³äÔ´IPµØÖ·
	((arp*)ARPREQ)->DestId[i]=*ip_address;//Ìî³äÄ¿µÄIPµØÖ·
	ip_address++;
	}
#ifdef Big_End	
((arp*)ARPREQ)->HardwareType=0x0001;		//Ó²¼şÀàĞÍ£º0x0001,ÒÔÌ«ÍøÀàĞÍ
((arp*)ARPREQ)->ProtocalType=0x0800;		//Ğ­ÒéÀàĞÍ£º0x0800,¶ÔÓ¦IPv4
((arp*)ARPREQ)->Operation=0x0001;			//²Ù×÷ÀàĞÍ£ºARPÇëÇó
#endif

#ifdef Little_End
((arp*)ARPREQ)->HardwareType=0x0100;		//Ó²¼şÀàĞÍ£º0x0001,ÒÔÌ«ÍøÀàĞÍ
((arp*)ARPREQ)->ProtocalType=0x0008;		//Ğ­ÒéÀàĞÍ£º0x0800,¶ÔÓ¦IPv4
((arp*)ARPREQ)->Operation=0x0100;			//²Ù×÷ÀàĞÍ£ºARPÇëÇó
#endif	
((arp*)ARPREQ)->HardwareLen=0x06;			//Ó²¼ş³¤¶È£º¼´ÎïÀíµØÖ·³¤¶È£¬µ¥Î»×Ö½Ú
((arp*)ARPREQ)->ProtocalLen=0x04;			//Ğ­Òé³¤¶È£º¼´Âß¼­µØÖ·³¤¶È£¬µ¥Î»×Ö½Ú

/*******************************************************
 *×¢Òâ:	
 *arp±¨ÎÄ¶ÎµÄ³¤¶ÈÎª28×Ö½Ú,¶øÒÔÌ«ÍøÊı¾İ°üµÄ×îĞ¡µ¥ÔªÎª60×Ö½Ú,ËùÒÔÔÚ·¢ËÍarp±¨ÎÄÊ±ĞèÒª¶Ô
 *ÒÔÌ«Íø±¨ÎÄ¶Î½øĞĞÌî³ä,ÒÔÂú×ã×îĞ¡³¤¶ÈÒªÇó.
 *arp·Ö×éµÄ·â×°¸ñÊ½:
 *·¢ËÍ±¨ÎÄ:               |     ÒÔÌ«ÍøÊ×²¿(¹²14×Ö½Ú)    |    arpÇëÇó·Ö×é(28×Ö½Ú)  | Ìî³äÊı¾İ |       
 *½ÓÊÕ±¨ÎÄ: | RTL8019Ê×²¿ |     ÒÔÌ«ÍøÊ×²¿(¹²14×Ö½Ú)    |    arp»Ø´ğ·Ö×é(28×Ö½Ú)  | Ìî³äÊı¾İ |
 *          |--> 4×Ö½Ú <--|------->   14×Ö½Ú    <-------|-------> 28×Ö½Ú <--------|->18×Ö½Ú<-|                   
 *          |--------------------------->0x00¡«0x<2e<-----------------------------|                                 
 *Êµ¼ÊÉÏ4×Ö½ÚµÄ8019Ê×²¿ÔÚ·¢ËÍÊ±ÊÇ²»Æğ×÷ÓÃµÄ,ÕæÕı·¢ËÍµÄÊı¾İ´ÓÒÔÌ«ÍøÊ×²¿Æğ.ËùÒÔÔÚÌî³äÊı¾İÊ±,ÊÇ´Ó0x2e¿ªÊ¼µÄ.
 *ÒòÎª³ÌĞòÖĞ½«·¢ËÍÊı¾İ±¨Êı¾İ½á¹¹Óë½ÓÊÕÊı¾İ±¨Êı¾İ½á¹¹¶¨ÒåÓÚ¹«ÓÃÌåÖĞ,·½±ã²Ù×÷.¹ØÓÚÊı¾İ½á¹¹¶¨Òå
 *¿ÉÒÔ²Î¿´arp.h,ÒÔ¼°ÏàÓ¦µÄÎÄµµËµÃ÷.
 ******************************************************/
//for(i=28;i<29;i++)				//²»ĞèÒª46		//µ±Êı¾İ³¤¶È<60×Ö½ÚÊ±£¬ĞèÒª²¹×ã60×Ö½ÚÊı¾İ
//	{
	ARPREQ[28]=0x00;			//Ìî³äÊı¾İÎª0x00
//	}
//Æô¶¯·¢ËÍÊı¾İ,·¢ËÍµÄÊÇÒ»¸öarpÇëÇó.
TxdArpReq.STPTR=NULL;
TxdArpReq.length=46;
TxdArpReq.DAPTR=ARPREQ;
Send_ethernet_Frame(&TxdArpReq,ARP_REP_MAC,ARP_PACKED,num);
}
/**********************************************************************
**º¯ÊıÔ­ĞÍ:º    void	Initial_arp()
**Èë¿Ú²ÎÊı:º	ÎŞ
**³ö¿Ú²ÎÊı:		ÎŞ
**·µ »Ø Öµ:º	ÎŞ
**Ëµ    Ã÷:º	ARP³õÊ¼»¯º¯Êı
************************************************************************/
void	Initial_arp()  
{
uint8 i,num;
	Ping_IP_TTL=0;
	for(num=0;num<MAX_NET_PORT;num++)
	{
		NetPort[num].Gateway_IP_TTL=0;
		for(i=0;i<MAX_ARP_TERM;i++)
		{
			NetPort[num].ARP_TERM[i].TTL=0;
		}
		Arp_Request(NetPort[num].My_Gateway,num);
	}
}

