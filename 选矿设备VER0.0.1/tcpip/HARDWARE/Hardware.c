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
* 名称：WriteToNet()
* 功能：把数据写入RTL8019AS
* 入口参数：	ADDR		写入地址
WRITEDATA   写入数据
* 出口参数：无
****************************************************************************/

void  WriteToNet(uint8 ADDR_16,uint16 WRITEDATA)
{
	
	(*((volatile unsigned short *) NET_BASE_ADDR+ADDR_16))=WRITEDATA;//0x83400000	 
	
}


/****************************************************************************
* 名称：ReadFromNet()
* 功能：从RTL8019AS把数据读出
* 入口参数：	ADDR		读出地址

  * 出口参数：	READDATA   	读出数据
****************************************************************************/
uint16  ReadFromNet(uint8 ADDR_16)
{
	uint16 temp;
	temp=(*((volatile unsigned short *) NET_BASE_ADDR+ADDR_16));//0x83400000
	return (temp);
}
/**********************************************************************
**函数原型：    void  page(uchar pagenumber)
**入口参数:�	uchar	pagenumber: 要切换的页
**出口参数:		无
**返 回 值：	无      
**说    明：	选择页,可选择0,1,2三页,第四页ne000兼容芯片保留 
************************************************************************/
void page(uint8 pagenumber)  
{
	uint8 temp;
	temp=ReadFromNet(0);//command register
	//temp=temp&0x3f;
	temp=temp&0x3B ;//注意txp位不能要
	pagenumber=pagenumber <<6;
	temp=temp | pagenumber;
	WriteToNet(0,temp);
}

/**********************************************************************
**函数原型：    void 	SetMacID()
**入口参数:		*mac_ptr
**出口参数:		无
**返 回 值：	无            
**说    明：	设置芯片物理地址,物理地址已经存储在程序空间内 
************************************************************************/
void SetMacID(uint8 * mac_ptr)   
{
	//把MAC地址写入MY——MAC——ID中
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
**函数原型：    void 		Send_Packet(struct _pkst *TxdData)
**入口参数:		struct _pkst *TxdData	:指向要发送数据的结构指针
**              
**出口参数:		无
**返 回 值：	无              
**说    明：	发送数据包,以太网底层驱动程序,所有的数据发送都要通过该程序
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
	
	//i=0;//记录发送是否小于60个字节
	page(0);//切换至第0页
	length=length+TxdData->length;
	ExPtr=TxdData->STPTR;
	while(ExPtr!=NULL)//计算出要发送的数据的总长度
	{	
		length=length+ExPtr->length;
		ExPtr=ExPtr->STPTR;
	}
	ii=length;
	Tx_Buff_Sel=Tx_Buff_Sel^1;//发送缓冲区的切换
	if(Tx_Buff_Sel)
	{
		WriteToNet(0x09,0x40);			//设置发送页地址
	}			
	else
	{
		WriteToNet(0x09,0x46);			//设置发送页地址
    }
	
	WriteToNet(0x08,0x00); 			//写入RSAR0 DMA起始地址低位read page address low
	WriteToNet(0x0b,ii>>8);			//写入RSCR1 DMA 计数器高位read count high
	WriteToNet(0x0a,ii&0x00ff);		//写入RSCR0 DMA 计数器低位read count low;
	WriteToNet(0,0x12);				//启动DMA写write dma, page0
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
	//如果少于60
	
	/***************************************/
	//以下为终止DMA操作
	WriteToNet(0x0b,0x00); 
	WriteToNet(0x0a,0x00);
	WriteToNet(0x00,0x22);			//结束或放弃DMA操作
	
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
		//如果数据长度<60字节,设置长度为60字节
		ii=60;
	}
	
	WriteToNet(0x06,ii>>8);	//high byte counter
	WriteToNet(0x05,ii&0x00ff);//low byte counter
	WriteToNet(0x07,0xff);
	WriteToNet(0x00,0x3e);       //to sendpacket;
	/***************************************/
	//重发数据的处理
	
	for(length=0;length<6;length++)	//最多重发6次
	{
		for(ii=0;ii<1000;ii++)
		{//检查CR寄存器的txp位是否为低,为1说明正在发送,为0说明发完或出错放弃
			if((ReadFromNet(0X00)&0x04)==0)  
			{ break; }
        }
		if((ReadFromNet(0X04)&0x01)!=0)//表示发送成功,判断发送状态寄存器TSR，决定是否出错
		{break;};
		WriteToNet(0x00,0x3e);       //to sendpacket;
	}
	
	/**************************************/
	//OS_EXIT_CRITICAL();
}

/**********************************************************************
**函数原型：    unsigned char * Rec_Packet()
**入口参数:�	无
**出口参数:		返回数据指针			unsigned char * 
**返 回 值：	NULL          			没有新数据包
**              unsigned char *         接收到新数据包
**说    明：	查询是否有新数据包并接收进缓冲区
************************************************************************/
uint8 Rec_Packet() 
{
	static uint8 REC_BUFF_NUM=0;
	static uint8 bnry,curr;  //?可否只做局部？
	static uint16 tmp[2];
	//static uint16 crt=0;
	uint16 * REC_BUFF_PTR_WORDS;
	uint8 * REC_BUFF_PTR_BYTES;
	uint8 i;
	uint16 ii,length;
	OS_ENTER_CRITICAL();
	
rea1:
	page(0);
	i=ReadFromNet(0X07);		//读取中断状态
	if((i&0x90)!=0)//如果复位或益出就重新初试化
	{
		InitNic(0);
		OS_EXIT_CRITICAL();
		return(0);
	}
	bnry=ReadFromNet(0X03);		//bnry page have read 读页指针
	page(1);
	curr=ReadFromNet(0X07);		//curr writepoint 8019写页指针
	page(0);
	if(curr==0)
	{
		OS_EXIT_CRITICAL();
		return(0);	//读的过程出错
	}
	bnry++;//bnry=bnry++;
	if(bnry>0x7f)
	{
		bnry=0x4c;
		//crt++;//统计内部16K循环了多少次（仿真器29次后出错）52、
	}
	
	if(bnry!=curr)	//此时表示有新的数据包在缓冲区里
   	{			//在任何操作都最好返回page0
		if(REC_BUFF_NUM==MAX_REC_BUFF)//接收缓冲区号清零
		{
			REC_BUFF_NUM=0;
		}
		REC_BUFF_PTR_WORDS=REC_BUFF[REC_BUFF_NUM].words;//设定接收缓冲区的起始地址
		//=======================================
		
		WriteToNet(0x09,bnry);	//RSAR1写入读页地址的高字节
		WriteToNet(0x08,0x00); //RSAR0写入读页地址的低字节
		WriteToNet(0x0b,0x00);	//RSCR1写入读取字节计数高字节
		WriteToNet(0x0a,18);   //RSCR0写入读取字节计数高字节
		WriteToNet(0x00,0x0a); //启动Remote DMA读操作
		//读取一包的前4个字节:4字节的8019头部
		for(i=0;i<2;i++)
		{
			*REC_BUFF_PTR_WORDS=ReadFromNet(0x10);
			tmp[i]=*REC_BUFF_PTR_WORDS;
			REC_BUFF_PTR_WORDS++;
		}
		//0:接收状态；1:下一包的指针；2:本包低位；3:本包高位；
		//=======================================中止DMA操作
		WriteToNet(0x0b,0x00);	//RSCR1写入读取字节计数高字节
		WriteToNet(0x0a,0x00);	//RSCR0写入读取字节计数高字节
		WriteToNet(0x00,0x22);	//结束或放弃DMA操作
		//=======================================
		tmp[1]=tmp[1]-4;//去掉4个字节的CRC
		REC_BUFF[REC_BUFF_NUM].words[1]=tmp[1];	//把真正的接收的帧的长度保存到缓冲区
		//=====================以上各步操作表示读入的数据包有效
		//0:接收状态；1:下一包的指针；2:本包高位；3:本包低位；
		if(((tmp[0]&0x0001)==0)||((tmp[0]&0xff00)>0x7f00)
			||((tmp[0]&0xff00)<0x4c00)||(tmp[1]>0x0600))
		{//接收状态错误或下一数据包的起始页地址错误或接收的数据包长度>1536字节
			page(1);
			curr=ReadFromNet(0X07); 	//page1读取CURR的值
			page(0);		//切换回page0
			bnry = curr -1;	//把bnry恢复为下16K中的空余部分
			if(bnry < 0x4c) 
			{
				bnry =0x7f;
			}
			WriteToNet(0x03,bnry); 	//把BNRY恢复到指向下一帧write to bnry		
			WriteToNet(0x07,0xff);		//清除中断标志
			//goto rea1;
			OS_EXIT_CRITICAL();
			return(0);
		}//end of if(((tmp[0]&0x0001)
		//=============================================
		else//表示数据包是完好的.读取剩下的数据
		{
			WriteToNet(0x09,bnry);   	//RSAR1写入读页地址的高字节//read page address high
			WriteToNet(0x08,4);      	//RSAR0写入读页地址的低字节//read page address low
			WriteToNet(0x0b,tmp[0]);  	//RSCR1写入读取字节计数高字节//read count high
			WriteToNet(0x0a,tmp[1]);	//RSCR0写入读取字节计数低字节//read count low;
			
			WriteToNet(0x00,0x0a); 	//启动Remote DMA读操作
			//read dma
			
			length=tmp[1];//计算出要读取的帧的长度
			
			for(ii=0;ii<((length+1)/2);ii++)
			{
				*REC_BUFF_PTR_WORDS=ReadFromNet(0x10);
				REC_BUFF_PTR_WORDS++;
			}
			//================终止DMA操作
			WriteToNet(0x0b,0x00);			//RSCR1写入读取字节计数高字节//read count high   
			WriteToNet(0x0a,0x00);			//RSCR0写入读取字节计数高字节//read count low;
			WriteToNet(0x00,0x22);			//结束或放弃DMA操作//结束或放弃DMA
			//============================
		}//end of else
		//=========================================
		bnry=(tmp[0]/256)-1;	//tmp[1]是当前有效的CURR
		if(bnry<0x4c)
			bnry=0x7f;
		WriteToNet(0x03,bnry);   	//写入有效的BNRY   //write to bnry
		WriteToNet(0x07,0xff);		//清除中断标志	
		REC_BUFF_PTR_BYTES=REC_BUFF[REC_BUFF_NUM].bytes;
		REC_BUFF_PTR_BYTES=REC_BUFF_PTR_BYTES+4;//把指针恢复到数据包的起始
#ifdef Little_End
		if((((ipethernet*)REC_BUFF_PTR_BYTES)->NextProtocal==0x0008)//可以减少对数据缓冲取的利用
			||(((ipethernet*)REC_BUFF_PTR_BYTES)->NextProtocal==0x0608))//不过如果缓冲区足够大就最好不要
#endif
#ifdef Big_End
		if((((ipethernet*)REC_BUFF_PTR_BYTES)->NextProtocal==0x0800)//可以减少对数据缓冲取的利用
			||(((ipethernet*)REC_BUFF_PTR_BYTES)->NextProtocal==0x0806))//不过如果缓冲区足够大就最好不要
#endif
		{
			REC_BUFF_NUM++;
			OS_EXIT_CRITICAL();
			Rec_Ethernet_Packed(REC_BUFF_PTR_BYTES,0);
			OS_ENTER_CRITICAL();
			goto rea1;
			//可以直接退出。
		}
		else 
			goto rea1;
	}//end of if(bnry!=curr)
	OS_EXIT_CRITICAL();
	return(0);
}

/**********************************************************************
**函数原型:     void 	InitNic()
**入口参数:�	无
**出口参数:		无
**返 回 值:�	无              
**说    明:�	对芯片的工作寄存器进行设置,各个寄存器的用法可参考文档和
**				络芯片的数据手册
************************************************************************/
void InitNic(uint8 num)  
{
	
	uint8  i;
	uint8  j;
	
	//PINSEL0 = 0x00000005;		// 设置所有管脚连接GPIO
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
	
	  1.Reg00命令寄存器: CR,命令寄存器,地址偏移量00H,为一个字节
	  位:  7   6   5   4   3   2   1   0 
	  名字: PS1 PS0 RD2 RD1 RD0 TXP STA STP 
	  //============================================
	  2.
	  RD2,RD1,RD0: 这3个位代表要执行的功能。
	  0   0   1 : 读网卡内存
	  0   1   0 : 写网卡内存
	  0   1   1 : 发送网卡数据包
	  1   *   * : 完成或结束DMA的读写操作
	  //============================================
	  3.TXP位置1时发送数据包，发完自动清零
	  //============================================
	  4.
	  STA,STP:	这两个位用来启动命令或停止命令
	  1   0 		启动命令
	  0   1 		停止命令
	  //============================================
	********************************************************/
	WriteToNet(0x00,0x21);   //使芯片处于停止模式,这时进行寄存器设置
	
	for(i=0;i<200;i++)
	{
		for(j=0;j<200;j++);
	} //延时10毫秒,确保芯片进入停止模式
	page(0);
	
	WriteToNet(0x0a,0x00);   //清rbcr0
	WriteToNet(0x0b,0x00);   //清rbcr1
	WriteToNet(0x0c, 0xe0);  //RCR,监视模式,不接收数据包
	WriteToNet(0x0d, 0xe2);  //TCR,loop back模式
	page(0);
	WriteToNet(0x01,0x4c);		//PSTART=0X4C
	WriteToNet(0x02,0x80);		//PSTOP=0X80
	WriteToNet(0x03,0x4c);	  //bnry=0x4c;
	page(0);
	WriteToNet(0x04,0x40);  //TPSR,发送起始页寄存器
	WriteToNet(0x07,0xff);  //清除所有中断标志位，中断状态寄存器
	WriteToNet(0x0f,0x11);  //中断屏蔽寄存器清0，禁止中断
	
	WriteToNet(0x0e,0xcb);  // 数据配置寄存器，8位dma方式
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
	WriteToNet(0x00,0x22);  //这时让芯片开始工作
	SetMacID(NetPort[num].My_Mac);  //将芯片物理地址写入到MAR寄存器
	
	page(0);
	WriteToNet(0x0c,0xcc);  //将芯片设置成正常模式,跟外部网络连接
	WriteToNet(0x0d,0xe0);
	WriteToNet(0x00,0x22);  //启动芯片开始工作
	WriteToNet(0x07,0xff);  //清除所有中断标志位
				
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
**函数原型:     void 	int_ex1() interrupt 2
**入口参数:�	无
**出口参数:		无
**返 回 值:�	无              
**说    明:�	RTL8019AS中断服务程序
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
