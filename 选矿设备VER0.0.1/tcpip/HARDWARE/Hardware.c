#define HARDWARE_GLOBALS
#include "config.h"
#include "../include/cfg_net.h"


//#define   NET_RST   		0X00000040//0x20000000
//#define   NET_BASE_ADDR   	0X83400000//0x20000000
#define   NET_RST   		0X00000100//0x20000000
#define   NET_BASE_ADDR   	0X81400000//0x20000000
//#include "rtl8019as.h"
EX_RAM uint16 iic=0;

union  REC_BUFF_UNION	REC_BUFF[MAX_REC_BUFF];
/****************************************************************************
* Ãû³Æ£ºWriteToNet()
* ¹¦ÄÜ£º°ÑÊı¾İĞ´ÈëRTL8019AS
* Èë¿Ú²ÎÊı£º	ADDR		Ğ´ÈëµØÖ·
WRITEDATA   Ğ´ÈëÊı¾İ
* ³ö¿Ú²ÎÊı£ºÎŞ
****************************************************************************/

void  WriteToNet(uint8 ADDR_16,uint16 WRITEDATA)
{
	
	(*((volatile unsigned short *) NET_BASE_ADDR+ADDR_16))=WRITEDATA;//0x83400000	 
	
}


/****************************************************************************
* Ãû³Æ£ºReadFromNet()
* ¹¦ÄÜ£º´ÓRTL8019AS°ÑÊı¾İ¶Á³ö
* Èë¿Ú²ÎÊı£º	ADDR		¶Á³öµØÖ·

  * ³ö¿Ú²ÎÊı£º	READDATA   	¶Á³öÊı¾İ
****************************************************************************/
uint16  ReadFromNet(uint8 ADDR_16)
{
	uint16 temp;
	temp=(*((volatile unsigned short *) NET_BASE_ADDR+ADDR_16));//0x83400000
	return (temp);
}
/**********************************************************************
**º¯ÊıÔ­ĞÍ£º    void  page(uchar pagenumber)
**Èë¿Ú²ÎÊı:º	uchar	pagenumber: ÒªÇĞ»»µÄÒ³
**³ö¿Ú²ÎÊı:		ÎŞ
**·µ »Ø Öµ£º	ÎŞ      
**Ëµ    Ã÷£º	Ñ¡ÔñÒ³,¿ÉÑ¡Ôñ0,1,2ÈıÒ³,µÚËÄÒ³ne000¼æÈİĞ¾Æ¬±£Áô 
************************************************************************/
void page(uint8 pagenumber)  
{
	uint8 temp;
	temp=ReadFromNet(0);//command register
	//temp=temp&0x3f;
	temp=temp&0x3B ;//×¢ÒâtxpÎ»²»ÄÜÒª
	pagenumber=pagenumber <<6;
	temp=temp | pagenumber;
	WriteToNet(0,temp);
}

/**********************************************************************
**º¯ÊıÔ­ĞÍ£º    void 	SetMacID()
**Èë¿Ú²ÎÊı:		*mac_ptr
**³ö¿Ú²ÎÊı:		ÎŞ
**·µ »Ø Öµ£º	ÎŞ            
**Ëµ    Ã÷£º	ÉèÖÃĞ¾Æ¬ÎïÀíµØÖ·,ÎïÀíµØÖ·ÒÑ¾­´æ´¢ÔÚ³ÌĞò¿Õ¼äÄÚ 
************************************************************************/
void SetMacID(uint8 * mac_ptr)   
{
	//°ÑMACµØÖ·Ğ´ÈëMY¡ª¡ªMAC¡ª¡ªIDÖĞ
	page(1);
	WriteToNet(1 , *mac_ptr);
	mac_ptr++;
	WriteToNet(2 , *mac_ptr);
	mac_ptr++;
	WriteToNet(3 , *mac_ptr);
	mac_ptr++;
	WriteToNet(4 , *mac_ptr);
	mac_ptr++;
	WriteToNet(5 , *mac_ptr);
	mac_ptr++;
	WriteToNet(6 , *mac_ptr);
	page(0);
}
/**********************************************************************
**º¯ÊıÔ­ĞÍ£º    void 		Send_Packet(struct _pkst *TxdData)
**Èë¿Ú²ÎÊı:		struct _pkst *TxdData	:Ö¸ÏòÒª·¢ËÍÊı¾İµÄ½á¹¹Ö¸Õë
**              
**³ö¿Ú²ÎÊı:		ÎŞ
**·µ »Ø Öµ£º	ÎŞ              
**Ëµ    Ã÷£º	·¢ËÍÊı¾İ°ü,ÒÔÌ«Íøµ×²ãÇı¶¯³ÌĞò,ËùÓĞµÄÊı¾İ·¢ËÍ¶¼ÒªÍ¨¹ı¸Ã³ÌĞò
************************************************************************/
void Send_Packet(struct _pkst *TxdData)//  
{
	static uint8 Tx_Buff_Sel=0;
	struct _pkst *ExPtr;//
	//uint16 i;
	uint8 *TEPTR;
	union send_temp{
		uint16 words;
		uint8	bytes[2];
				}send_buff;
	uint16 ii,length=0;
	
	//i=0;//¼ÇÂ¼·¢ËÍÊÇ·ñĞ¡ÓÚ60¸ö×Ö½Ú
	page(0);//ÇĞ»»ÖÁµÚ0Ò³
	length=length+TxdData->length;
	ExPtr=TxdData->STPTR;
	while(ExPtr!=NULL)//¼ÆËã³öÒª·¢ËÍµÄÊı¾İµÄ×Ü³¤¶È
	{	
		length=length+ExPtr->length;
		ExPtr=ExPtr->STPTR;
	}
	ii=length;
	Tx_Buff_Sel=Tx_Buff_Sel^1;//·¢ËÍ»º³åÇøµÄÇĞ»»
	if(Tx_Buff_Sel)
	{
		WriteToNet(0x09,0x40);			//ÉèÖÃ·¢ËÍÒ³µØÖ·
	}			
	else
	{
		WriteToNet(0x09,0x46);			//ÉèÖÃ·¢ËÍÒ³µØÖ·
    }
	
	WriteToNet(0x08,0x00); 			//Ğ´ÈëRSAR0 DMAÆğÊ¼µØÖ·µÍÎ»read page address low
	WriteToNet(0x0b,ii>>8);			//Ğ´ÈëRSCR1 DMA ¼ÆÊıÆ÷¸ßÎ»read count high
	WriteToNet(0x0a,ii&0x00ff);		//Ğ´ÈëRSCR0 DMA ¼ÆÊıÆ÷µÍÎ»read count low;
	WriteToNet(0,0x12);				//Æô¶¯DMAĞ´write dma, page0
	TEPTR=TxdData->DAPTR;
	for(ii=0;ii<(((TxdData->length)+1)/2);ii++)
	{
		send_buff.bytes[0]=*TEPTR;
		TEPTR++;
		send_buff.bytes[1]=*TEPTR;
		TEPTR++;
		WriteToNet(0x10,send_buff.words);
	}
	ExPtr=TxdData->STPTR;
	while(ExPtr!=NULL)
	{
		TEPTR=ExPtr->DAPTR;
		for(ii=0;ii<((ExPtr->length+1)/2);ii++)
		{
			send_buff.bytes[0]=*TEPTR;
			TEPTR++;
			send_buff.bytes[1]=*TEPTR;
			TEPTR++;
			WriteToNet(0x10,send_buff.words);	
		}
		ExPtr=ExPtr->STPTR;
	}
	//Èç¹ûÉÙÓÚ60
	
	/***************************************/
	//ÒÔÏÂÎªÖÕÖ¹DMA²Ù×÷
	WriteToNet(0x0b,0x00); 
	WriteToNet(0x0a,0x00);
	WriteToNet(0x00,0x22);			//½áÊø»ò·ÅÆúDMA²Ù×÷
	
	WriteToNet(0x07,0xff);
	if(Tx_Buff_Sel)
	{
		WriteToNet(0x04,0x40);   //txd packet start;
	}
	else
	{
		WriteToNet(0x04,0x46);  //txd packet start;
    }
	ii=length;
	if(length<60)
	{
		//Èç¹ûÊı¾İ³¤¶È<60×Ö½Ú,ÉèÖÃ³¤¶ÈÎª60×Ö½Ú
		ii=60;
	}
	
	WriteToNet(0x06,ii>>8);	//high byte counter
	WriteToNet(0x05,ii&0x00ff);//low byte counter
	WriteToNet(0x07,0xff);
	WriteToNet(0x00,0x3e);       //to sendpacket;
	/***************************************/
	//ÖØ·¢Êı¾İµÄ´¦Àí
	
	for(length=0;length<6;length++)	//×î¶àÖØ·¢6´Î
	{
		for(ii=0;ii<1000;ii++)
		{//¼ì²éCR¼Ä´æÆ÷µÄtxpÎ»ÊÇ·ñÎªµÍ,Îª1ËµÃ÷ÕıÔÚ·¢ËÍ,Îª0ËµÃ÷·¢Íê»ò³ö´í·ÅÆú
			if((ReadFromNet(0X00)&0x04)==0)  
			{ break; }
        }
		if((ReadFromNet(0X04)&0x01)!=0)//±íÊ¾·¢ËÍ³É¹¦,ÅĞ¶Ï·¢ËÍ×´Ì¬¼Ä´æÆ÷TSR£¬¾ö¶¨ÊÇ·ñ³ö´í
		{break;};
		WriteToNet(0x00,0x3e);       //to sendpacket;
	}
	
	/**************************************/
	//OS_EXIT_CRITICAL();
}

/**********************************************************************
**º¯ÊıÔ­ĞÍ£º    unsigned char * Rec_Packet()
**Èë¿Ú²ÎÊı:º	ÎŞ
**³ö¿Ú²ÎÊı:		·µ»ØÊı¾İÖ¸Õë			unsigned char * 
**·µ »Ø Öµ£º	NULL          			Ã»ÓĞĞÂÊı¾İ°ü
**              unsigned char *         ½ÓÊÕµ½ĞÂÊı¾İ°ü
**Ëµ    Ã÷£º	²éÑ¯ÊÇ·ñÓĞĞÂÊı¾İ°ü²¢½ÓÊÕ½ø»º³åÇø
************************************************************************/
uint8 Rec_Packet() 
{
	static uint8 REC_BUFF_NUM=0;
	static uint8 bnry,curr;  //?¿É·ñÖ»×ö¾Ö²¿£¿
	static uint16 tmp[2];
	//static uint16 crt=0;
	uint16 * REC_BUFF_PTR_WORDS;
	uint8 * REC_BUFF_PTR_BYTES;
	uint8 i;
	uint16 ii,length;
	OS_ENTER_CRITICAL();
	
rea1:
	page(0);
	i=ReadFromNet(0X07);		//¶ÁÈ¡ÖĞ¶Ï×´Ì¬
	if((i&0x90)!=0)//Èç¹û¸´Î»»òÒæ³ö¾ÍÖØĞÂ³õÊÔ»¯
	{
		InitNic(0);
		OS_EXIT_CRITICAL();
		return(0);
	}
	bnry=ReadFromNet(0X03);		//bnry page have read ¶ÁÒ³Ö¸Õë
	page(1);
	curr=ReadFromNet(0X07);		//curr writepoint 8019Ğ´Ò³Ö¸Õë
	page(0);
	if(curr==0)
	{
		OS_EXIT_CRITICAL();
		return(0);	//¶ÁµÄ¹ı³Ì³ö´í
	}
	bnry++;//bnry=bnry++;
	if(bnry>0x7f)
	{
		bnry=0x4c;
		//crt++;//Í³¼ÆÄÚ²¿16KÑ­»·ÁË¶àÉÙ´Î£¨·ÂÕæÆ÷29´Îºó³ö´í£©52¡¢
	}
	
	if(bnry!=curr)	//´ËÊ±±íÊ¾ÓĞĞÂµÄÊı¾İ°üÔÚ»º³åÇøÀï
   	{			//ÔÚÈÎºÎ²Ù×÷¶¼×îºÃ·µ»Øpage0
		if(REC_BUFF_NUM==MAX_REC_BUFF)//½ÓÊÕ»º³åÇøºÅÇåÁã
		{
			REC_BUFF_NUM=0;
		}
		REC_BUFF_PTR_WORDS=REC_BUFF[REC_BUFF_NUM].words;//Éè¶¨½ÓÊÕ»º³åÇøµÄÆğÊ¼µØÖ·
		//=======================================
		
		WriteToNet(0x09,bnry);	//RSAR1Ğ´Èë¶ÁÒ³µØÖ·µÄ¸ß×Ö½Ú
		WriteToNet(0x08,0x00); //RSAR0Ğ´Èë¶ÁÒ³µØÖ·µÄµÍ×Ö½Ú
		WriteToNet(0x0b,0x00);	//RSCR1Ğ´Èë¶ÁÈ¡×Ö½Ú¼ÆÊı¸ß×Ö½Ú
		WriteToNet(0x0a,18);   //RSCR0Ğ´Èë¶ÁÈ¡×Ö½Ú¼ÆÊı¸ß×Ö½Ú
		WriteToNet(0x00,0x0a); //Æô¶¯Remote DMA¶Á²Ù×÷
		//¶ÁÈ¡Ò»°üµÄÇ°4¸ö×Ö½Ú:4×Ö½ÚµÄ8019Í·²¿
		for(i=0;i<2;i++)
		{
			*REC_BUFF_PTR_WORDS=ReadFromNet(0x10);
			tmp[i]=*REC_BUFF_PTR_WORDS;
			REC_BUFF_PTR_WORDS++;
		}
		//0:½ÓÊÕ×´Ì¬£»1:ÏÂÒ»°üµÄÖ¸Õë£»2:±¾°üµÍÎ»£»3:±¾°ü¸ßÎ»£»
		//=======================================ÖĞÖ¹DMA²Ù×÷
		WriteToNet(0x0b,0x00);	//RSCR1Ğ´Èë¶ÁÈ¡×Ö½Ú¼ÆÊı¸ß×Ö½Ú
		WriteToNet(0x0a,0x00);	//RSCR0Ğ´Èë¶ÁÈ¡×Ö½Ú¼ÆÊı¸ß×Ö½Ú
		WriteToNet(0x00,0x22);	//½áÊø»ò·ÅÆúDMA²Ù×÷
		//=======================================
		tmp[1]=tmp[1]-4;//È¥µô4¸ö×Ö½ÚµÄCRC
		REC_BUFF[REC_BUFF_NUM].words[1]=tmp[1];	//°ÑÕæÕıµÄ½ÓÊÕµÄÖ¡µÄ³¤¶È±£´æµ½»º³åÇø
		//=====================ÒÔÉÏ¸÷²½²Ù×÷±íÊ¾¶ÁÈëµÄÊı¾İ°üÓĞĞ§
		//0:½ÓÊÕ×´Ì¬£»1:ÏÂÒ»°üµÄÖ¸Õë£»2:±¾°ü¸ßÎ»£»3:±¾°üµÍÎ»£»
		if(((tmp[0]&0x0001)==0)||((tmp[0]&0xff00)>0x7f00)
			||((tmp[0]&0xff00)<0x4c00)||(tmp[1]>0x0600))
		{//½ÓÊÕ×´Ì¬´íÎó»òÏÂÒ»Êı¾İ°üµÄÆğÊ¼Ò³µØÖ·´íÎó»ò½ÓÊÕµÄÊı¾İ°ü³¤¶È>1536×Ö½Ú
			page(1);
			curr=ReadFromNet(0X07); 	//page1¶ÁÈ¡CURRµÄÖµ
			page(0);		//ÇĞ»»»Øpage0
			bnry = curr -1;	//°Ñbnry»Ö¸´ÎªÏÂ16KÖĞµÄ¿ÕÓà²¿·Ö
			if(bnry < 0x4c) 
			{
				bnry =0x7f;
			}
			WriteToNet(0x03,bnry); 	//°ÑBNRY»Ö¸´µ½Ö¸ÏòÏÂÒ»Ö¡write to bnry		
			WriteToNet(0x07,0xff);		//Çå³ıÖĞ¶Ï±êÖ¾
			//goto rea1;
			OS_EXIT_CRITICAL();
			return(0);
		}//end of if(((tmp[0]&0x0001)
		//=============================================
		else//±íÊ¾Êı¾İ°üÊÇÍêºÃµÄ.¶ÁÈ¡Ê£ÏÂµÄÊı¾İ
		{
			WriteToNet(0x09,bnry);   	//RSAR1Ğ´Èë¶ÁÒ³µØÖ·µÄ¸ß×Ö½Ú//read page address high
			WriteToNet(0x08,4);      	//RSAR0Ğ´Èë¶ÁÒ³µØÖ·µÄµÍ×Ö½Ú//read page address low
			WriteToNet(0x0b,tmp[0]);  	//RSCR1Ğ´Èë¶ÁÈ¡×Ö½Ú¼ÆÊı¸ß×Ö½Ú//read count high
			WriteToNet(0x0a,tmp[1]);	//RSCR0Ğ´Èë¶ÁÈ¡×Ö½Ú¼ÆÊıµÍ×Ö½Ú//read count low;
			
			WriteToNet(0x00,0x0a); 	//Æô¶¯Remote DMA¶Á²Ù×÷
			//read dma
			
			length=tmp[1];//¼ÆËã³öÒª¶ÁÈ¡µÄÖ¡µÄ³¤¶È
			
			for(ii=0;ii<((length+1)/2);ii++)
			{
				*REC_BUFF_PTR_WORDS=ReadFromNet(0x10);
				REC_BUFF_PTR_WORDS++;
			}
			//================ÖÕÖ¹DMA²Ù×÷
			WriteToNet(0x0b,0x00);			//RSCR1Ğ´Èë¶ÁÈ¡×Ö½Ú¼ÆÊı¸ß×Ö½Ú//read count high   
			WriteToNet(0x0a,0x00);			//RSCR0Ğ´Èë¶ÁÈ¡×Ö½Ú¼ÆÊı¸ß×Ö½Ú//read count low;
			WriteToNet(0x00,0x22);			//½áÊø»ò·ÅÆúDMA²Ù×÷//½áÊø»ò·ÅÆúDMA
			//============================
		}//end of else
		//=========================================
		bnry=(tmp[0]/256)-1;	//tmp[1]ÊÇµ±Ç°ÓĞĞ§µÄCURR
		if(bnry<0x4c)
			bnry=0x7f;
		WriteToNet(0x03,bnry);   	//Ğ´ÈëÓĞĞ§µÄBNRY   //write to bnry
		WriteToNet(0x07,0xff);		//Çå³ıÖĞ¶Ï±êÖ¾	
		REC_BUFF_PTR_BYTES=REC_BUFF[REC_BUFF_NUM].bytes;
		REC_BUFF_PTR_BYTES=REC_BUFF_PTR_BYTES+4;//°ÑÖ¸Õë»Ö¸´µ½Êı¾İ°üµÄÆğÊ¼
#ifdef Little_End
		if((((ipethernet*)REC_BUFF_PTR_BYTES)->NextProtocal==0x0008)//¿ÉÒÔ¼õÉÙ¶ÔÊı¾İ»º³åÈ¡µÄÀûÓÃ
			||(((ipethernet*)REC_BUFF_PTR_BYTES)->NextProtocal==0x0608))//²»¹ıÈç¹û»º³åÇø×ã¹»´ó¾Í×îºÃ²»Òª
#endif
#ifdef Big_End
		if((((ipethernet*)REC_BUFF_PTR_BYTES)->NextProtocal==0x0800)//¿ÉÒÔ¼õÉÙ¶ÔÊı¾İ»º³åÈ¡µÄÀûÓÃ
			||(((ipethernet*)REC_BUFF_PTR_BYTES)->NextProtocal==0x0806))//²»¹ıÈç¹û»º³åÇø×ã¹»´ó¾Í×îºÃ²»Òª
#endif
		{
			REC_BUFF_NUM++;
			OS_EXIT_CRITICAL();
			Rec_Ethernet_Packed(REC_BUFF_PTR_BYTES,0);
			OS_ENTER_CRITICAL();
			goto rea1;
			//¿ÉÒÔÖ±½ÓÍË³ö¡£
		}
		else 
			goto rea1;
	}//end of if(bnry!=curr)
	OS_EXIT_CRITICAL();
	return(0);
}

/**********************************************************************
**º¯ÊıÔ­ĞÍ:     void 	InitNic()
**Èë¿Ú²ÎÊı:º	ÎŞ
**³ö¿Ú²ÎÊı:		ÎŞ
**·µ »Ø Öµ:º	ÎŞ              
**Ëµ    Ã÷:º	¶ÔĞ¾Æ¬µÄ¹¤×÷¼Ä´æÆ÷½øĞĞÉèÖÃ,¸÷¸ö¼Ä´æÆ÷µÄÓÃ·¨¿É²Î¿¼ÎÄµµºÍ
**				ÂçĞ¾Æ¬µÄÊı¾İÊÖ²á
************************************************************************/
void InitNic(uint8 num)  
{
	
	uint8  i;
	uint8  j;
	
	//PINSEL0 = 0x00000005;		// ÉèÖÃËùÓĞ¹Ü½ÅÁ¬½ÓGPIO
	//PINSEL1 = 0x00000000;
	//IODIR=IODIR|(NET_DATA+NET_CS+NET_WR+NET_ALE+NET_RD+NET_RST);
	//IOSET=NET_DATA+NET_CS+NET_WR+NET_ALE+NET_RD+NET_RST;
	IODIR=IODIR|NET_RST;
	IOCLR=NET_RST;
	for(i=0;i<200;i++)
	{
		for(j=0;j<200;j++);
	}
	IOSET=NET_RST;
	for(i=0;i<200;i++)
	{
		for(j=0;j<200;j++);
	}
	IOCLR=NET_RST;
	for(i=0;i<200;i++)
	{
		for(j=0;j<200;j++);
	}	
	WriteToNet(0x1f,0x00); 	
	for(i=0;i<200;i++)
	{
		for(j=0;j<200;j++);
	}
	
	/**********************************************************************
	
	  1.Reg00ÃüÁî¼Ä´æÆ÷: CR,ÃüÁî¼Ä´æÆ÷,µØÖ·Æ«ÒÆÁ¿00H,ÎªÒ»¸ö×Ö½Ú
	  Î»:  7   6   5   4   3   2   1   0 
	  Ãû×Ö: PS1 PS0 RD2 RD1 RD0 TXP STA STP 
	  //============================================
	  2.
	  RD2,RD1,RD0: Õâ3¸öÎ»´ú±íÒªÖ´ĞĞµÄ¹¦ÄÜ¡£
	  0   0   1 : ¶ÁÍø¿¨ÄÚ´æ
	  0   1   0 : Ğ´Íø¿¨ÄÚ´æ
	  0   1   1 : ·¢ËÍÍø¿¨Êı¾İ°ü
	  1   *   * : Íê³É»ò½áÊøDMAµÄ¶ÁĞ´²Ù×÷
	  //============================================
	  3.TXPÎ»ÖÃ1Ê±·¢ËÍÊı¾İ°ü£¬·¢Íê×Ô¶¯ÇåÁã
	  //============================================
	  4.
	  STA,STP:	ÕâÁ½¸öÎ»ÓÃÀ´Æô¶¯ÃüÁî»òÍ£Ö¹ÃüÁî
	  1   0 		Æô¶¯ÃüÁî
	  0   1 		Í£Ö¹ÃüÁî
	  //============================================
	********************************************************/
	WriteToNet(0x00,0x21);   //Ê¹Ğ¾Æ¬´¦ÓÚÍ£Ö¹Ä£Ê½,ÕâÊ±½øĞĞ¼Ä´æÆ÷ÉèÖÃ
	
	for(i=0;i<200;i++)
	{
		for(j=0;j<200;j++);
	} //ÑÓÊ±10ºÁÃë,È·±£Ğ¾Æ¬½øÈëÍ£Ö¹Ä£Ê½
	page(0);
	
	WriteToNet(0x0a,0x00);   //Çårbcr0
	WriteToNet(0x0b,0x00);   //Çårbcr1
	WriteToNet(0x0c, 0xe0);  //RCR,¼àÊÓÄ£Ê½,²»½ÓÊÕÊı¾İ°ü
	WriteToNet(0x0d, 0xe2);  //TCR,loop backÄ£Ê½
	page(0);
	WriteToNet(0x01,0x4c);		//PSTART=0X4C
	WriteToNet(0x02,0x80);		//PSTOP=0X80
	WriteToNet(0x03,0x4c);	  //bnry=0x4c;
	page(0);
	WriteToNet(0x04,0x40);  //TPSR,·¢ËÍÆğÊ¼Ò³¼Ä´æÆ÷
	WriteToNet(0x07,0xff);  //Çå³ıËùÓĞÖĞ¶Ï±êÖ¾Î»£¬ÖĞ¶Ï×´Ì¬¼Ä´æÆ÷
	WriteToNet(0x0f,0x11);  //ÖĞ¶ÏÆÁ±Î¼Ä´æÆ÷Çå0£¬½ûÖ¹ÖĞ¶Ï
	
	WriteToNet(0x0e,0xcb);  // Êı¾İÅäÖÃ¼Ä´æÆ÷£¬8Î»dma·½Ê½
	page(1);
	WriteToNet(0x07,0x4d);  //curr=0x4d
	WriteToNet(0x08,0x00);
	WriteToNet(0x09,0x00);
	WriteToNet(0x0a,0x00);
	WriteToNet(0x0b,0x00);
	WriteToNet(0x0c,0x00);
	WriteToNet(0x0d,0x00);
	WriteToNet(0x0e,0x00);
	WriteToNet(0x0f,0x00);
	WriteToNet(0x00,0x22);  //ÕâÊ±ÈÃĞ¾Æ¬¿ªÊ¼¹¤×÷
	SetMacID(NetPort[num].My_Mac);  //½«Ğ¾Æ¬ÎïÀíµØÖ·Ğ´Èëµ½MAR¼Ä´æÆ÷
	
	page(0);
	WriteToNet(0x0c,0xcc);  //½«Ğ¾Æ¬ÉèÖÃ³ÉÕı³£Ä£Ê½,¸úÍâ²¿ÍøÂçÁ¬½Ó
	WriteToNet(0x0d,0xe0);
	WriteToNet(0x00,0x22);  //Æô¶¯Ğ¾Æ¬¿ªÊ¼¹¤×÷
	WriteToNet(0x07,0xff);  //Çå³ıËùÓĞÖĞ¶Ï±êÖ¾Î»
				
}
uint16 swap_int16(uint16 temp)
{uint16 temp1;
temp1=(temp&0xff00)>>8;
temp=(temp&0x00ff)<<8;
return(temp+temp1);
}
uint32 swap_int32(uint32 temp)
{
	union   w  temp232,temp132;
	temp232.dwords=temp;
	temp132.bytes.byte0=temp232.bytes.byte3;
	temp132.bytes.byte1=temp232.bytes.byte2;
	temp132.bytes.byte2=temp232.bytes.byte1;
	temp132.bytes.byte3=temp232.bytes.byte0;
	return(temp132.dwords);
}

uint16 Char2ToInt16(uint8 * temp)
{
	uint16 temp16;
	temp16=((uint16)(*temp))<<8;
	temp++;
	temp16=temp16+(uint16)(*temp);
	return(temp16);
}

uint32 Char4ToInt32(uint8 * temp)
{
	uint32 temp32;
	temp32=((uint32)(*temp))<<24;
	temp++;
	temp32=temp32+(((uint32)(*temp))<<16);
	temp++;
	temp32=temp32+(((uint32)(*temp))<<8);
	temp++;
	temp32=temp32+(uint32)(*temp);
	return(temp32);
}
/**********************************************************************
**º¯ÊıÔ­ĞÍ:     void 	int_ex1() interrupt 2
**Èë¿Ú²ÎÊı:º	ÎŞ
**³ö¿Ú²ÎÊı:		ÎŞ
**·µ »Ø Öµ:º	ÎŞ              
**Ëµ    Ã÷:º	RTL8019ASÖĞ¶Ï·şÎñ³ÌĞò
************************************************************************/
/*
void int_ex1() interrupt 2
{
//	OS_INT_ENTER();

  EX_RAM uint8 i,TEMP;
  EX1=0;
  iic++;
  TEMP=Reg00;
  page(0);
  i=Reg07;
  
	if((i&&0x10)==0x10)
	{
	OSSemIntPost(RecBufOverFlowFlag);
	}
	else if((i&&0x01)==0x01)
	OSSemIntPost(RecPackedFlag);
	Reg07=0XFF;
	Reg00=TEMP;
	OSIntExit();
	}
	uint8 rec_data()
	{
	uint8 i,TEMP;
	TEMP=Reg00;
	page(0);
	i=Reg07;
	Reg07=0XFF;
	Reg00=TEMP;
	if((i&&0x01)==0x01)
	return (0);
	return(1);
	
	  }
*/
