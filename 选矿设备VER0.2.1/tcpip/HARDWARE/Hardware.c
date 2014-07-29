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
* ���ƣ�WriteToNet()
* ���ܣ�������д��RTL8019AS
* ��ڲ�����	ADDR		д���ַ
WRITEDATA   д������
* ���ڲ�������
****************************************************************************/

void  WriteToNet(uint8 ADDR_16,uint16 WRITEDATA)
{
	
	(*((volatile unsigned short *) NET_BASE_ADDR+ADDR_16))=WRITEDATA;//0x83400000	 
	
}


/****************************************************************************
* ���ƣ�ReadFromNet()
* ���ܣ���RTL8019AS�����ݶ���
* ��ڲ�����	ADDR		������ַ

  * ���ڲ�����	READDATA   	��������
****************************************************************************/
uint16  ReadFromNet(uint8 ADDR_16)
{
	uint16 temp;
	temp=(*((volatile unsigned short *) NET_BASE_ADDR+ADDR_16));//0x83400000
	return (temp);
}
/**********************************************************************
**����ԭ�ͣ�    void  page(uchar pagenumber)
**��ڲ���:�	uchar	pagenumber: Ҫ�л���ҳ
**���ڲ���:		��
**�� �� ֵ��	��      
**˵    ����	ѡ��ҳ,��ѡ��0,1,2��ҳ,����ҳne000����оƬ���� 
************************************************************************/
void page(uint8 pagenumber)  
{
	uint8 temp;
	temp=ReadFromNet(0);//command register
	//temp=temp&0x3f;
	temp=temp&0x3B ;//ע��txpλ����Ҫ
	pagenumber=pagenumber <<6;
	temp=temp | pagenumber;
	WriteToNet(0,temp);
}

/**********************************************************************
**����ԭ�ͣ�    void 	SetMacID()
**��ڲ���:		*mac_ptr
**���ڲ���:		��
**�� �� ֵ��	��            
**˵    ����	����оƬ�����ַ,�����ַ�Ѿ��洢�ڳ���ռ��� 
************************************************************************/
void SetMacID(uint8 * mac_ptr)   
{
	//��MAC��ַд��MY����MAC����ID��
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
**����ԭ�ͣ�    void 		Send_Packet(struct _pkst *TxdData)
**��ڲ���:		struct _pkst *TxdData	:ָ��Ҫ�������ݵĽṹָ��
**              
**���ڲ���:		��
**�� �� ֵ��	��              
**˵    ����	�������ݰ�,��̫���ײ���������,���е����ݷ��Ͷ�Ҫͨ���ó���
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
	
	//i=0;//��¼�����Ƿ�С��60���ֽ�
	page(0);//�л�����0ҳ
	length=length+TxdData->length;
	ExPtr=TxdData->STPTR;
	while(ExPtr!=NULL)//�����Ҫ���͵����ݵ��ܳ���
	{	
		length=length+ExPtr->length;
		ExPtr=ExPtr->STPTR;
	}
	ii=length;
	Tx_Buff_Sel=Tx_Buff_Sel^1;//���ͻ��������л�
	if(Tx_Buff_Sel)
	{
		WriteToNet(0x09,0x40);			//���÷���ҳ��ַ
	}			
	else
	{
		WriteToNet(0x09,0x46);			//���÷���ҳ��ַ
    }
	
	WriteToNet(0x08,0x00); 			//д��RSAR0 DMA��ʼ��ַ��λread page address low
	WriteToNet(0x0b,ii>>8);			//д��RSCR1 DMA ��������λread count high
	WriteToNet(0x0a,ii&0x00ff);		//д��RSCR0 DMA ��������λread count low;
	WriteToNet(0,0x12);				//����DMAдwrite dma, page0
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
	//�������60
	
	/***************************************/
	//����Ϊ��ֹDMA����
	WriteToNet(0x0b,0x00); 
	WriteToNet(0x0a,0x00);
	WriteToNet(0x00,0x22);			//���������DMA����
	
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
		//������ݳ���<60�ֽ�,���ó���Ϊ60�ֽ�
		ii=60;
	}
	
	WriteToNet(0x06,ii>>8);	//high byte counter
	WriteToNet(0x05,ii&0x00ff);//low byte counter
	WriteToNet(0x07,0xff);
	WriteToNet(0x00,0x3e);       //to sendpacket;
	/***************************************/
	//�ط����ݵĴ���
	
	for(length=0;length<6;length++)	//����ط�6��
	{
		for(ii=0;ii<1000;ii++)
		{//���CR�Ĵ�����txpλ�Ƿ�Ϊ��,Ϊ1˵�����ڷ���,Ϊ0˵�������������
			if((ReadFromNet(0X00)&0x04)==0)  
			{ break; }
        }
		if((ReadFromNet(0X04)&0x01)!=0)//��ʾ���ͳɹ�,�жϷ���״̬�Ĵ���TSR�������Ƿ����
		{break;};
		WriteToNet(0x00,0x3e);       //to sendpacket;
	}
	
	/**************************************/
	//OS_EXIT_CRITICAL();
}

/**********************************************************************
**����ԭ�ͣ�    unsigned char * Rec_Packet()
**��ڲ���:�	��
**���ڲ���:		��������ָ��			unsigned char * 
**�� �� ֵ��	NULL          			û�������ݰ�
**              unsigned char *         ���յ������ݰ�
**˵    ����	��ѯ�Ƿ��������ݰ������ս�������
************************************************************************/
uint8 Rec_Packet() 
{
	static uint8 REC_BUFF_NUM=0;
	static uint8 bnry,curr;  //?�ɷ�ֻ���ֲ���
	static uint16 tmp[2];
	//static uint16 crt=0;
	uint16 * REC_BUFF_PTR_WORDS;
	uint8 * REC_BUFF_PTR_BYTES;
	uint8 i;
	uint16 ii,length;
	OS_ENTER_CRITICAL();
	
rea1:
	page(0);
	i=ReadFromNet(0X07);		//��ȡ�ж�״̬
	if((i&0x90)!=0)//�����λ����������³��Ի�
	{
		InitNic(0);
		OS_EXIT_CRITICAL();
		return(0);
	}
	bnry=ReadFromNet(0X03);		//bnry page have read ��ҳָ��
	page(1);
	curr=ReadFromNet(0X07);		//curr writepoint 8019дҳָ��
	page(0);
	if(curr==0)
	{
		OS_EXIT_CRITICAL();
		return(0);	//���Ĺ��̳���
	}
	bnry++;//bnry=bnry++;
	if(bnry>0x7f)
	{
		bnry=0x4c;
		//crt++;//ͳ���ڲ�16Kѭ���˶��ٴΣ�������29�κ����52��
	}
	
	if(bnry!=curr)	//��ʱ��ʾ���µ����ݰ��ڻ�������
   	{			//���κβ�������÷���page0
		if(REC_BUFF_NUM==MAX_REC_BUFF)//���ջ�����������
		{
			REC_BUFF_NUM=0;
		}
		REC_BUFF_PTR_WORDS=REC_BUFF[REC_BUFF_NUM].words;//�趨���ջ���������ʼ��ַ
		//=======================================
		
		WriteToNet(0x09,bnry);	//RSAR1д���ҳ��ַ�ĸ��ֽ�
		WriteToNet(0x08,0x00); //RSAR0д���ҳ��ַ�ĵ��ֽ�
		WriteToNet(0x0b,0x00);	//RSCR1д���ȡ�ֽڼ������ֽ�
		WriteToNet(0x0a,18);   //RSCR0д���ȡ�ֽڼ������ֽ�
		WriteToNet(0x00,0x0a); //����Remote DMA������
		//��ȡһ����ǰ4���ֽ�:4�ֽڵ�8019ͷ��
		for(i=0;i<2;i++)
		{
			*REC_BUFF_PTR_WORDS=ReadFromNet(0x10);
			tmp[i]=*REC_BUFF_PTR_WORDS;
			REC_BUFF_PTR_WORDS++;
		}
		//0:����״̬��1:��һ����ָ�룻2:������λ��3:������λ��
		//=======================================��ֹDMA����
		WriteToNet(0x0b,0x00);	//RSCR1д���ȡ�ֽڼ������ֽ�
		WriteToNet(0x0a,0x00);	//RSCR0д���ȡ�ֽڼ������ֽ�
		WriteToNet(0x00,0x22);	//���������DMA����
		//=======================================
		tmp[1]=tmp[1]-4;//ȥ��4���ֽڵ�CRC
		REC_BUFF[REC_BUFF_NUM].words[1]=tmp[1];	//�������Ľ��յ�֡�ĳ��ȱ��浽������
		//=====================���ϸ���������ʾ��������ݰ���Ч
		//0:����״̬��1:��һ����ָ�룻2:������λ��3:������λ��
		if(((tmp[0]&0x0001)==0)||((tmp[0]&0xff00)>0x7f00)
			||((tmp[0]&0xff00)<0x4c00)||(tmp[1]>0x0600))
		{//����״̬�������һ���ݰ�����ʼҳ��ַ�������յ����ݰ�����>1536�ֽ�
			page(1);
			curr=ReadFromNet(0X07); 	//page1��ȡCURR��ֵ
			page(0);		//�л���page0
			bnry = curr -1;	//��bnry�ָ�Ϊ��16K�еĿ��ಿ��
			if(bnry < 0x4c) 
			{
				bnry =0x7f;
			}
			WriteToNet(0x03,bnry); 	//��BNRY�ָ���ָ����һ֡write to bnry		
			WriteToNet(0x07,0xff);		//����жϱ�־
			//goto rea1;
			OS_EXIT_CRITICAL();
			return(0);
		}//end of if(((tmp[0]&0x0001)
		//=============================================
		else//��ʾ���ݰ�����õ�.��ȡʣ�µ�����
		{
			WriteToNet(0x09,bnry);   	//RSAR1д���ҳ��ַ�ĸ��ֽ�//read page address high
			WriteToNet(0x08,4);      	//RSAR0д���ҳ��ַ�ĵ��ֽ�//read page address low
			WriteToNet(0x0b,tmp[0]);  	//RSCR1д���ȡ�ֽڼ������ֽ�//read count high
			WriteToNet(0x0a,tmp[1]);	//RSCR0д���ȡ�ֽڼ������ֽ�//read count low;
			
			WriteToNet(0x00,0x0a); 	//����Remote DMA������
			//read dma
			
			length=tmp[1];//�����Ҫ��ȡ��֡�ĳ���
			
			for(ii=0;ii<((length+1)/2);ii++)
			{
				*REC_BUFF_PTR_WORDS=ReadFromNet(0x10);
				REC_BUFF_PTR_WORDS++;
			}
			//================��ֹDMA����
			WriteToNet(0x0b,0x00);			//RSCR1д���ȡ�ֽڼ������ֽ�//read count high   
			WriteToNet(0x0a,0x00);			//RSCR0д���ȡ�ֽڼ������ֽ�//read count low;
			WriteToNet(0x00,0x22);			//���������DMA����//���������DMA
			//============================
		}//end of else
		//=========================================
		bnry=(tmp[0]/256)-1;	//tmp[1]�ǵ�ǰ��Ч��CURR
		if(bnry<0x4c)
			bnry=0x7f;
		WriteToNet(0x03,bnry);   	//д����Ч��BNRY   //write to bnry
		WriteToNet(0x07,0xff);		//����жϱ�־	
		REC_BUFF_PTR_BYTES=REC_BUFF[REC_BUFF_NUM].bytes;
		REC_BUFF_PTR_BYTES=REC_BUFF_PTR_BYTES+4;//��ָ��ָ������ݰ�����ʼ
#ifdef Little_End
		if((((ipethernet*)REC_BUFF_PTR_BYTES)->NextProtocal==0x0008)//���Լ��ٶ����ݻ���ȡ������
			||(((ipethernet*)REC_BUFF_PTR_BYTES)->NextProtocal==0x0608))//��������������㹻�����ò�Ҫ
#endif
#ifdef Big_End
		if((((ipethernet*)REC_BUFF_PTR_BYTES)->NextProtocal==0x0800)//���Լ��ٶ����ݻ���ȡ������
			||(((ipethernet*)REC_BUFF_PTR_BYTES)->NextProtocal==0x0806))//��������������㹻�����ò�Ҫ
#endif
		{
			REC_BUFF_NUM++;
			OS_EXIT_CRITICAL();
			Rec_Ethernet_Packed(REC_BUFF_PTR_BYTES,0);
			OS_ENTER_CRITICAL();
			goto rea1;
			//����ֱ���˳���
		}
		else 
			goto rea1;
	}//end of if(bnry!=curr)
	OS_EXIT_CRITICAL();
	return(0);
}

/**********************************************************************
**����ԭ��:     void 	InitNic()
**��ڲ���:�	��
**���ڲ���:		��
**�� �� ֵ:�	��              
**˵    ��:�	��оƬ�Ĺ����Ĵ�����������,�����Ĵ������÷��ɲο��ĵ���
**				��оƬ�������ֲ�
************************************************************************/
void InitNic(uint8 num)  
{
	
	uint8  i;
	uint8  j;
	
	//PINSEL0 = 0x00000005;		// �������йܽ�����GPIO
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
	
	  1.Reg00����Ĵ���: CR,����Ĵ���,��ַƫ����00H,Ϊһ���ֽ�
	  λ:  7   6   5   4   3   2   1   0 
	  ����: PS1 PS0 RD2 RD1 RD0 TXP STA STP 
	  //============================================
	  2.
	  RD2,RD1,RD0: ��3��λ����Ҫִ�еĹ��ܡ�
	  0   0   1 : �������ڴ�
	  0   1   0 : д�����ڴ�
	  0   1   1 : �����������ݰ�
	  1   *   * : ��ɻ����DMA�Ķ�д����
	  //============================================
	  3.TXPλ��1ʱ�������ݰ��������Զ�����
	  //============================================
	  4.
	  STA,STP:	������λ�������������ֹͣ����
	  1   0 		��������
	  0   1 		ֹͣ����
	  //============================================
	********************************************************/
	WriteToNet(0x00,0x21);   //ʹоƬ����ֹͣģʽ,��ʱ���мĴ�������
	
	for(i=0;i<200;i++)
	{
		for(j=0;j<200;j++);
	} //��ʱ10����,ȷ��оƬ����ֹͣģʽ
	page(0);
	
	WriteToNet(0x0a,0x00);   //��rbcr0
	WriteToNet(0x0b,0x00);   //��rbcr1
	WriteToNet(0x0c, 0xe0);  //RCR,����ģʽ,���������ݰ�
	WriteToNet(0x0d, 0xe2);  //TCR,loop backģʽ
	page(0);
	WriteToNet(0x01,0x4c);		//PSTART=0X4C
	WriteToNet(0x02,0x80);		//PSTOP=0X80
	WriteToNet(0x03,0x4c);	  //bnry=0x4c;
	page(0);
	WriteToNet(0x04,0x40);  //TPSR,������ʼҳ�Ĵ���
	WriteToNet(0x07,0xff);  //��������жϱ�־λ���ж�״̬�Ĵ���
	WriteToNet(0x0f,0x11);  //�ж����μĴ�����0����ֹ�ж�
	
	WriteToNet(0x0e,0xcb);  // �������üĴ�����8λdma��ʽ
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
	WriteToNet(0x00,0x22);  //��ʱ��оƬ��ʼ����
	SetMacID(NetPort[num].My_Mac);  //��оƬ�����ַд�뵽MAR�Ĵ���
	
	page(0);
	WriteToNet(0x0c,0xcc);  //��оƬ���ó�����ģʽ,���ⲿ��������
	WriteToNet(0x0d,0xe0);
	WriteToNet(0x00,0x22);  //����оƬ��ʼ����
	WriteToNet(0x07,0xff);  //��������жϱ�־λ
				
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
**����ԭ��:     void 	int_ex1() interrupt 2
**��ڲ���:�	��
**���ڲ���:		��
**�� �� ֵ:�	��              
**˵    ��:�	RTL8019AS�жϷ������
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
