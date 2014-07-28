/******************************************************************************
			大连金德姆电子有限公司
			(Copyright 2005)	All rights riserved.

文件名	：drv_flash.c
项目名称：X射线测厚仪
功能概要：cpu使用philips 的lpc2214 
			操作系统 uCOS-II
创建人	：许岩
创建日期：2005.4.10


取代版本：0.0.1
修改人	：许岩
完成日期：2005.4.10
升级说明：第一版
******************************************************************************/
#include "config.h"



extern OS_EVENT        *FLASH_Sem;


// static const INT8U FORMAT_END_SIGN[4]={0x55,0xaa,0x18,0xe7};
// static const INT8U END_SIGN[4]={0x18,0xe7,0x18,0xe7};

// extern void Delay_10us(INT16U _times);

//static volatile INT8U  Addr13_18 = 0xFF;					//FLASH高地址A13-A18状态寄存器
extern volatile DEV_STAT  DevStat;				//设备状态
//volatile INT8U  OutPortStat;
//volatile DISP_BUF DispBuf;  //显示缓存从0-7依次对应数码管(从左到右)的：
//volatile INT8U FErrPage[ERR_MAP_SIZE];		//坏块分配图，可以表示512*8 = 4096页的状态

INT8U PageBuf1[512];
//static INT8U PageBuf2[512];

/*****************************************************************
函数原型：Cal_Crc
 功能描述：计算一串数据的CRC值，采用CCITT算法
 参数描述：
 参数名称：	输入/输出？	类型		描述
 data_to_cal_crc	输入	INT8U const *		要进行CRC计算的数组的起始指针
 len				 		输入	INT16U				要进行CRC计算的数组的长度

 返  回  值：uint型的CRC值(2字节)
 作      者	：许岩
 日      期：2004-02-04
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
*****************************************************************/
INT16U Cal_Crc(INT8U const *data_to_cal_crc, INT16U len)
{
	INT16U crc;
	INT8U i;

	ToggleWD();

	crc = 0xFFFF;			//CRC初始值

	while ( len-- != 0 )
	{
		ToggleWD();

		crc = crc ^ ((INT16U)*data_to_cal_crc++ << 8);

		for ( i = 0; i < 8; ++i )
			if ( crc & 0x8000 )
				crc = (crc << 1) ^ 0x1021;
			else
				crc	= crc << 1;
	}

	return(crc & 0xFFFF);
}

/******************************************************************************
函数名称：flash_init
功能描述：flash初始化
参数描述：

返  回  值	：无
作      者	：许岩
日      期：2005-04-08
修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
void flash_init(void)
{
	ToggleWD();

//  PINSEL0 = PINSEL0 & (~BIT8);
//  PINSEL0 = PINSEL0 & (~BIT9);        //P0.4 配置成GPIO
//
//  PINSEL0 = PINSEL0 & (~BIT10);
//  PINSEL0 = PINSEL0 & (~BIT11);       //P0.5 配置成GPIO
//
//  PINSEL0 = PINSEL0 & (~BIT12);
//  PINSEL0 = PINSEL0 & (~BIT13);       //P0.6 配置成GPIO
//
//  PINSEL0 = PINSEL0 & (~BIT20);
//  PINSEL0 = PINSEL0 & (~BIT21);       //P0.10 配置成GPIO
//
//  PINSEL0 = PINSEL0 & (~BIT22);
//  PINSEL0 = PINSEL0 & (~BIT23);       //P0.11 配置成GPIO
//
//  PINSEL0 = PINSEL0 & (~BIT24);
//  PINSEL0 = PINSEL0 & (~BIT25);       //P0.12 配置成GPIO
//
//  IO0DIR |= BIT4;             //输出
//  IO0DIR &= (~BIT5);              //输入
//  IO0DIR |= BIT6;
//  IO0DIR |= BIT10;
//  IO0DIR |= BIT11;
//  IO0DIR |= BIT12;
//
//
//  PINSEL1 = PINSEL1 & (~BIT2);
//  PINSEL1 = PINSEL1 & (~BIT3);        //P0.17 配置成GPIO
//
//  PINSEL1 = PINSEL1 & (~BIT4);
//  PINSEL1 = PINSEL1 & (~BIT5);        //P0.18 配置成GPIO
//
//  PINSEL1 = PINSEL1 & (~BIT6);
//  PINSEL1 = PINSEL1 & (~BIT7);        //P0.19 配置成GPIO
//
//  PINSEL1 = PINSEL1 & (~BIT10);
//  PINSEL1 = PINSEL1 & (~BIT11);       //P0.21 配置成GPIO
//
//  PINSEL1 = PINSEL1 & (~BIT12);
//  PINSEL1 = PINSEL1 & (~BIT13);       //P0.22 配置成GPIO
//
//  PINSEL1 = PINSEL1 & (~BIT14);
//  PINSEL1 = PINSEL1 & (~BIT15);       //P0.23 配置成GPIO
//
//  IO0DIR |= BIT17;                //输出
//  IO0DIR &= (~BIT18);             //输入
//  IO0DIR |= BIT19;
//  IO0DIR |= BIT21;
//  IO0DIR |= BIT22;
//  IO0DIR |= BIT23;


	PINSEL0 = PINSEL0 & (~BIT20);
	PINSEL0 = PINSEL0 & (~BIT21);		//P0.10 配置成GPIO

	PINSEL0 = PINSEL0 & (~BIT22);
	PINSEL0 = PINSEL0 & (~BIT23);		//P0.11 配置成GPIO

	PINSEL0 = PINSEL0 & (~BIT24);
	PINSEL0 = PINSEL0 & (~BIT25);		//P0.12 配置成GPIO

	PINSEL0 = PINSEL0 & (~BIT26);
	PINSEL0 = PINSEL0 & (~BIT27);		//P0.13 配置成GPIO

	PINSEL0 = PINSEL0 & (~BIT30);
	PINSEL0 = PINSEL0 & (~BIT31);		//P0.15 配置成GPIO

	PINSEL1 = PINSEL1 & (~BIT0);
	PINSEL1 = PINSEL1 & (~BIT1);		//P0.16 配置成GPIO


	IO0DIR |= BIT10;
	IO0DIR |= BIT11;
	IO0DIR |= BIT12;
	IO0DIR |= BIT13;
	IO0DIR |= BIT15;                //输出
	IO0DIR &= (~BIT16);             //输入



	FlashReset(0);
	FlashDISENWP(0);				//解除写保护
//  FlashDISENWP(1);                //解除写保护

}

/******************************************************************************
 函数名称：FlashReset
 功能描述：复位flash
 参数名称： 输入/输出？ 类型        描述
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

 返  回  值：无

 作      者 ：许岩
 日      期：2004-11-29
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
void FlashReset(INT8U which)
{
	ToggleWD();

	which = which;		//avoid warning
//  if ( which == 0 )
//  {
		FLASH_RST0_SetLow();
		SleepMs(1);
		FLASH_RST0_SetHigh();
//  }
//  else
//  {
//      FLASH_RST1_SetLow();
//      SleepMs(1);
//      FLASH_RST1_SetHigh();
//  }
}

/******************************************************************************
 函数名称：FlashENWP
 功能描述：始能flash的写保护
 参数名称： 输入/输出？ 类型        描述
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

 返  回  值：无

 作      者 ：许岩
 日      期：2004-11-29
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
void FlashENWP(INT8U which)
{
	ToggleWD();

	which = which;		//avoid warning
//  if ( which == 0 )
//  {
		FLASH_WP0_SetLow();
//  }
//  else
//  {
//      FLASH_WP1_SetLow();
//  }
}

/******************************************************************************
 函数名称：FlashDISENWP
 功能描述：去掉flash的写保护
 参数名称： 输入/输出？ 类型        描述
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

 返  回  值：无

 作      者 ：许岩
 日      期：2004-11-29
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
void FlashDISENWP(INT8U which)
{
	ToggleWD();

	which = which;		//avoid warning
//  if ( which == 0 )
//  {
		FLASH_WP0_SetHigh();
		Sleep10us(10);
//  }
//  else
//  {
//      FLASH_WP1_SetHigh();
//      Sleep10us(10);
//  }
}

/******************************************************************************
 函数名称：FLASH_SCK_SetLow
 功能描述：
 参数名称： 输入/输出？ 类型        描述
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

 返  回  值：无

 作      者 ：许岩
 日      期：2004-11-29
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
void FLASH_SCK_SetLow(INT8U which)
{
	ToggleWD();

	which = which;		//avoid warning
//  if ( which == 0 )
//  {
		FLASH_SCK0_SetLow();
//  }
//  else
//  {
//      FLASH_SCK1_SetLow();
//  }
}

/******************************************************************************
 函数名称：FLASH_SCK_SetHigh
 功能描述：
 参数名称： 输入/输出？ 类型        描述
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

 返  回  值：无

 作      者 ：许岩
 日      期：2004-11-29
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
void FLASH_SCK_SetHigh(INT8U which)
{
	ToggleWD();

	which = which;		//avoid warning
//  if ( which == 0 )
//  {
		FLASH_SCK0_SetHigh();
//  }
//  else
//  {
//      FLASH_SCK1_SetHigh();
//  }
}

/******************************************************************************
 函数名称：FLASH_SI_SetLow
 功能描述：
 参数名称： 输入/输出？ 类型        描述
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

 返  回  值：无

 作      者 ：许岩
 日      期：2004-11-29
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
void FLASH_SI_SetLow(INT8U which)
{
	ToggleWD();

	which = which;		//avoid warning
//  if ( which == 0 )
//  {
		FLASH_SI0_SetLow();
//  }
//  else
//  {
//      FLASH_SI1_SetLow();
//  }
}

/******************************************************************************
 函数名称：FLASH_SI_SetHigh
 功能描述：
 参数名称： 输入/输出？ 类型        描述
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

 返  回  值：无

 作      者 ：许岩
 日      期：2004-11-29
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
void FLASH_SI_SetHigh(INT8U which)
{
	ToggleWD();

	which = which;		//avoid warning
//  if ( which == 0 )
//  {
		FLASH_SI0_SetHigh();
//  }
//  else
//  {
//      FLASH_SI1_SetHigh();
//  }
}

/******************************************************************************
 函数名称：FLASH_RST_SetLow
 功能描述：
 参数名称： 输入/输出？ 类型        描述
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

 返  回  值：无

 作      者 ：许岩
 日      期：2004-11-29
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
void FLASH_RST_SetLow(INT8U which)
{
	ToggleWD();

	which = which;		//avoid warning
//  if ( which == 0 )
//  {
		FLASH_RST0_SetLow();
//  }
//  else
//  {
//      FLASH_RST1_SetLow();
//  }
}

/******************************************************************************
 函数名称：FLASH_RST_SetHigh
 功能描述：
 参数名称： 输入/输出？ 类型        描述
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

 返  回  值：无

 作      者 ：许岩
 日      期：2004-11-29
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
void FLASH_RST_SetHigh(INT8U which)
{
	ToggleWD();

	which = which;		//avoid warning
//  if ( which == 0 )
//  {
		FLASH_RST0_SetHigh();
//  }
//  else
//  {
//      FLASH_RST1_SetHigh();
//  }
}

/******************************************************************************
 函数名称：FLASH_CS_SetLow
 功能描述：
 参数名称： 输入/输出？ 类型        描述
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

 返  回  值：无

 作      者 ：许岩
 日      期：2004-11-29
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
void FLASH_CS_SetLow(INT8U which)
{
	ToggleWD();

	which = which;		//avoid warning
//  if ( which == 0 )
//  {
		FLASH_CS0_SetLow();
//  }
//  else
//  {
//      FLASH_CS1_SetLow();
//  }
}

/******************************************************************************
 函数名称：FLASH_CS_SetHigh
 功能描述：
 参数名称： 输入/输出？ 类型        描述
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

 返  回  值：无

 作      者 ：许岩
 日      期：2004-11-29
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
void FLASH_CS_SetHigh(INT8U which)
{
	ToggleWD();

	which = which;		//avoid warning
//  if ( which == 0 )
//  {
		FLASH_CS0_SetHigh();
//  }
//  else
//  {
//      FLASH_CS1_SetHigh();
//  }
}

/******************************************************************************
 函数名称：Send_SPI_8Bit
 功能描述：向spi总线发送1字节数据
 参数名称： 输入/输出？ 类型        描述
 value        输入        INT8U      要发送的数
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

 返  回  值：无

 作      者 ：许岩
 日      期：2004-11-29
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
void Send_SPI_8Bit (INT8U value, INT8U which)
{
// 	uchar	ValueNum;
//
// 	P2DIR|=FLASH_CLK;
// 	P2DIR|=FLASH_SI;
// 	for (ValueNum = 0; ValueNum < 8; ValueNum++)
// 	{
// 		P2OUT&=~FLASH_CLK;//CLK SET LOW
// 		if (Value&0x80)
// 			P2OUT|=FLASH_SI;	//此片注意的更，根椐口的位置不同
// 		else
// 			P2OUT&=~FLASH_SI;
// 		Value <<= 1;
// 		P2OUT|=FLASH_CLK;//CLK SET HIGH
// 	}
// 	P2OUT&=~FLASH_CLK;//CLK SET LOW


	INT8U i = 0;

	ToggleWD();

	OS_ENTER_CRITICAL();

	for ( i=0; i<8; i++ )
	{
		FLASH_SCK_SetLow(which);			//sck set low
		if ( value & 0x80 )
		{
			FLASH_SI_SetHigh(which);
		}
		else
		{
			FLASH_SI_SetLow(which);
		}
		value <<= 1;
		FLASH_SCK_SetHigh(which);			//sck set high
	}
	FLASH_SCK_SetLow(which);			//sck set low

	OS_EXIT_CRITICAL();
}

/******************************************************************************
 函数名称：Receive_SPI_8Bit
 功能描述：从spi总线接收1字节数据
 参数名称： 输入/输出？ 类型        描述
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

 返  回  值：读到的数

 作      者 ：许岩
 日      期：2004-11-29
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
INT8U Receive_SPI_8Bit(INT8U which)
{
// 	uchar	ValueNum, Value;
// 	P2DIR|=FLASH_CLK;
// 	P2DIR&=~FLASH_SO;
// 	for (ValueNum = 0; ValueNum < 8; ValueNum++)
// 	{
// 		P2OUT|=FLASH_CLK;//CLK SET HIGH
// 		Value <<= 1;
// 		if ((P2IN&FLASH_SO)== (0XFF&FLASH_SO))
// 			Value |= 0x01;
// 		P2OUT&=~FLASH_CLK;//CLK SET LOW
// 	}
// 	return (Value);


	INT8U i = 0;
	INT8U receive = 0;

	ToggleWD();

	OS_ENTER_CRITICAL();

	for ( i=0; i<8; i++ )
	{
		FLASH_SCK_SetHigh(which);			//sck set high
		receive <<= 1;
//      if ( which == 0 )
//      {
			if ( (IO0PIN & BIT16) == 0 )
			{
			}
			else
			{
				receive |= 0x01;
			}
//      }
//      else
//      {
//          if ( (IO0PIN & BIT18) == 0 )
//          {
//          }
//          else
//          {
//              receive |= 0x01;
//          }
//      }
		FLASH_SCK_SetLow(which);			//sck set low
	}

	OS_EXIT_CRITICAL();
	return receive;
}

/******************************************************************************
 函数名称：FlashReadStatus
 功能描述：读flash状态寄存器
 参数名称： 输入/输出？ 类型        描述
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

 返  回  值：读到的数

 作      者 ：许岩
 日      期：2004-11-29
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
INT8U FlashReadStatus(INT8U which)
{
// 	uchar	ValueStatus;
//
// 	// Choose The Chip
// 	P2DIR|=FLASH_CLK;
// 	P2OUT|=FLASH_CLK;//CLK SET HIGH
// 	P4DIR |=FLASH_CS;
// 	P4OUT|=FLASH_CS;	//FLASH_CS SET  HIGH
// 	P4OUT&=~FLASH_CS;	//FLASH_CS SET  LOW
// 	Send_SPI_8Bit(Status_Read);
// 	ValueStatus = Receive_SPI_8Bit();
//
// 	P4OUT|=FLASH_CS;	//FLASH_CS SET	HIGH
// 	P2OUT&=~FLASH_CLK;	//CLK SET LOW
//
// 	return(ValueStatus);

	INT8U status = 0;

	ToggleWD();

	FLASH_SCK_SetHigh(which);
	FLASH_CS_SetHigh(which);
	FLASH_CS_SetLow(which);

	Send_SPI_8Bit(READ_STATUSREAD, which);
	status = Receive_SPI_8Bit(which);

	FLASH_CS_SetHigh(which);
	FLASH_SCK_SetLow(which);

	return status;
}

/******************************************************************************
 函数名称：FlashBusy
 功能描述：看flash是否忙
 参数名称： 输入/输出？ 类型        描述
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

 返  回  值：TRUE = BUSY
				FALSE = DON'T BUSY

 作      者 ：许岩
 日      期：2004-11-29
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
INT8U FlashBusy(INT8U which)
{
	ToggleWD();

	if ( FlashReadStatus(which) < 0x80 )
		return TRUE;				//Busy
	else
		return FALSE;				//Don't Busy
}

/******************************************************************************
函数名称：FlashPageWrite512
功能描述：把数据(512 bytes)	写到Flash 中的某一页(先自擦除再写)
参数描述：
参数名称：	输入/输出？	类型		描述
page_address		  输入 				 INT16U 		页地址
*source_address		输出			const INT8U *		源数据(512 bytes)	存放地址		
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

作      者	：许岩
日      期：2004-09-02
修改历史：
日期		修改人		修改描述
------		---------	-------------
******************************************************************************/
void FlashPageWrite512(INT16U page_address, const INT8U *source_address, INT8U which)
{
	INT8U LOCAL_V_TYPE high_address;
	INT8U LOCAL_V_TYPE middle_address;
	INT8U LOCAL_V_TYPE low_address;      
	INT16U  LOCAL_V_TYPE i;
	INT8U err = 0;

	ToggleWD();

	high_address = (INT8U)(page_address >> 6) & 0x7F;			//取得高位地址		
	middle_address = (INT8U)(page_address << 2) & 0xFC;		  //取得中位地址		
	low_address = 0;							//取得低位地址		

	OSSemPend(FLASH_Sem, 0, &err);

	while ( FlashBusy(which) == TRUE )
	{
		ToggleWD();	// Check Flash Busy
	}

// 	FlashDISENWP(which);				//解除写保护
	FLASH_SCK_SetHigh(which);
	FLASH_CS_SetLow(which);

	Send_SPI_8Bit(MAIN_BUFFER1_WRITE, which);	//发送写命令		
	Send_SPI_8Bit(high_address, which);     
	Send_SPI_8Bit(middle_address, which);       
	Send_SPI_8Bit(low_address, which);      

	for ( i=0; i< USED_PAGE_SIZE; i++ )		  //*source_address指向的地址中数据发到flash中		
		Send_SPI_8Bit(*source_address++, which);

	FLASH_CS_SetHigh(which);
	FLASH_SCK_SetLow(which);
// 	FlashENWP(which);				//始能写保护

	OSSemPost(FLASH_Sem);
}       

/******************************************************************************
函数名称：FlashPageRead512
功能描述：读Flash 中的某一页
参数描述：
参数名称：	输入/输出？	类型		描述
page_address		  输入 				 INT16U 		页地址
*source_address		输出			INT8U *		读出来的数据(长度为512)存放地址		
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

作      者	：许岩
日      期：2004-09-02
修改历史：
日期		修改人		修改描述
------		---------	-------------
******************************************************************************/
void FlashPageRead512(INT16U page_address,INT8U *source_address, INT8U which)
{
	INT16U  LOCAL_V_TYPE i;     
	INT8U LOCAL_V_TYPE high_address;
	INT8U LOCAL_V_TYPE middle_address;      
	INT8U err = 0;

	ToggleWD();

	high_address = (INT8U)(page_address >> 6) & 0x7F;			//取得高位地址		
	middle_address = (INT8U)(page_address << 2) & 0xFC;		  //取得中位地址		

	OSSemPend(FLASH_Sem, 0, &err);

	while ( FlashBusy(which) == TRUE )
	{
		ToggleWD();	// Check Flash Busy
	}

// 	FlashDISENWP();				//解除写保护
	FLASH_SCK_SetHigh(which);
	FLASH_CS_SetLow(which);

	Send_SPI_8Bit(MAIN_PAGE_READ, which);	//发送读命令		
	Send_SPI_8Bit(high_address, which);     
	Send_SPI_8Bit(middle_address, which);       
	Send_SPI_8Bit(0, which);        
	Send_SPI_8Bit(0, which);		//发送空字节		
	Send_SPI_8Bit(0, which);		//发送空字节		
	Send_SPI_8Bit(0, which);		//发送空字节		
	Send_SPI_8Bit(0, which);		//发送空字节		
	for ( i=0; i<USED_PAGE_SIZE; i++ ) //开始接收数据放在aim_data_address指向的地址中		
		*source_address++ = Receive_SPI_8Bit(which);        

	FLASH_CS_SetHigh(which);
	FLASH_SCK_SetLow(which);
// 	FlashENWP();				//始能写保护

	OSSemPost(FLASH_Sem);
}  

/******************************************************************************
 函数名称：WriteParam
 功能描述：保存参数
 参数描述：
 参数名称：	输入/输出？	类型		描述
 
 返  回  值：无
				   
 作      者	：许岩
 日      期：2006-08-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void WriteParam(void)
{
// 	INT8U h = 0;
//	INT8U i = 0;
//	INT8U j = 0;
//  INT8U buf[512];
//  INT8U buf2[512];
//  INT8U buf[256];
//  INT8U buf2[256];

// 	INT16U crc = 0;				//add for debug

	ToggleWD();

	OS_ENTER_CRITICAL();

	memset(PageBuf1, 0xFF, sizeof(PageBuf1));
	memcpy(PageBuf1, (void *)&DevStat.MOD_REG.reg[0], 512);
	FlashPageWrite512(4, PageBuf1, 0);

//	memset(PageBuf1, 0xFF, sizeof(PageBuf1));
//	memcpy(PageBuf1, (void *)&DevStat.MOD_REG.reg[256], 512);
//	FlashPageWrite512(5, PageBuf1, 0);

	OS_EXIT_CRITICAL();

}

/******************************************************************************
 函数名称：ReadParam
 功能描述：读取参数
 参数描述：
 参数名称：	输入/输出？	类型		描述
 
 返  回  值：无
				   
 作      者	：许岩
 日      期：2006-08-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void ReadParam(void)
{
// 	INT8U i = 0;
// 	INT8U j = 0;
//  INT8U buf[528];
//  INT8U buf[256];
//	INT16U crc = 0;

	ToggleWD();

	OS_ENTER_CRITICAL();

	FlashPageRead512(4, PageBuf1, 0);
	memcpy((void *)&DevStat.MOD_REG.reg[0], PageBuf1, 512);

//	FlashPageRead512(5, PageBuf1, 0);
//	memcpy((void *)&DevStat.MOD_REG.reg[256], PageBuf1, 512);

	OS_EXIT_CRITICAL();
}

/******************************************************************************
函数名称：FlashWriteWithoutErase
功能描述：把数据写到Flash 中的某一页(无自擦除)
参数描述：
参数名称：	输入/输出？	类型		描述
page_address		  输入 				 INT16U 		页地址
offset_in_page			输入			INT16U			页内地址
*source_address		输出			const INT8U *		源数据(512 bytes)	存放地址		
len							输入				INT16U			写入长度
which        输入        INT8U      主FLASH还是从FLASH，0 或 1

作      者	：许岩
日      期：2004-09-02
修改历史：
日期		修改人		修改描述
------		---------	-------------
******************************************************************************/
void FlashWriteWithoutErase(INT16U page_address, INT16U offset_in_page, const INT8U *source_address, INT16U len, INT8U which)
{
	INT8U LOCAL_V_TYPE high_address;
	INT8U LOCAL_V_TYPE middle_address;
	INT8U LOCAL_V_TYPE low_address;      
	INT16U  LOCAL_V_TYPE i;
	INT8U err = 0;

	ToggleWD();

	high_address = (INT8U)(page_address >> 6) & 0x7F;			//取得高位地址		
	middle_address = (INT8U)(page_address << 2) & 0xFC;		  //取得中位地址		
	low_address = 0;							//取得低位地址		

	OSSemPend(FLASH_Sem, 0, &err);

	//////////////////////////////////////////////////
	//页数据->buffer1
	//////////////////////////////////////////////////
	while ( FlashBusy(which) == TRUE )
	{
		ToggleWD();	// Check Flash Busy
	}

// 	FlashDISENWP(which);				//解除写保护
	FLASH_SCK_SetHigh(which);
	FLASH_CS_SetLow(which);

	Send_SPI_8Bit(MAIN_TO_BUFFER1, which);	//发送写页数据->buffer1命令
	Send_SPI_8Bit(high_address, which);     
	Send_SPI_8Bit(middle_address, which);       
	Send_SPI_8Bit(low_address, which);      

	FLASH_CS_SetHigh(which);
	FLASH_SCK_SetLow(which);
// 	FlashENWP(which);				//始能写保护

	SleepMs(2);


	//////////////////////////////////////////////////
	//写buffer1
	//////////////////////////////////////////////////
	if ( offset_in_page > 512 )
		offset_in_page %= 512;
	if ( offset_in_page >= 256 )
		middle_address |= 1;
	low_address = offset_in_page % 256;							//取得低位地址		

	while ( FlashBusy(which) == TRUE )
	{
		ToggleWD();	// Check Flash Busy
	}

// 	FlashDISENWP(which);				//解除写保护
	FLASH_SCK_SetHigh(which);
	FLASH_CS_SetLow(which);

	Send_SPI_8Bit(BUFFER1_WRITE, which);	//发送写buffer1命令
	Send_SPI_8Bit(high_address, which);     
	Send_SPI_8Bit(middle_address, which);       
	Send_SPI_8Bit(low_address, which);      

	for ( i=0; i< len; i++ )		  //*source_address指向的地址中数据发到flash中
		Send_SPI_8Bit(*source_address++, which);

	FLASH_CS_SetHigh(which);
	FLASH_SCK_SetLow(which);
// 	FlashENWP(which);				//始能写保护


	//////////////////////////////////////////////////
	//buffer1->页数据
	//////////////////////////////////////////////////
	high_address = (INT8U)(page_address >> 6) & 0x7F;			//取得高位地址		
	middle_address = (INT8U)(page_address << 2) & 0xFC;		  //取得中位地址		
	low_address = 0;							//取得低位地址		

	while ( FlashBusy(which) == TRUE )
	{
		ToggleWD();	// Check Flash Busy
	}

// 	FlashDISENWP(which);				//解除写保护
	FLASH_SCK_SetHigh(which);
	FLASH_CS_SetLow(which);

	Send_SPI_8Bit(NO_ERASE_BUFFER1_WRITE_MAIN, which);	//buffer1->页数据
	Send_SPI_8Bit(high_address, which);     
	Send_SPI_8Bit(middle_address, which);       
	Send_SPI_8Bit(low_address, which);      

	FLASH_CS_SetHigh(which);
	FLASH_SCK_SetLow(which);
// 	FlashENWP(which);				//始能写保护


	OSSemPost(FLASH_Sem);
}

/******************************************************************************
函数名称：FlashPageWrite256
功能描述：把数据(512 bytes)	写到Flash 中的某一页(先自擦除再写)
参数描述：
参数名称：	输入/输出？	类型		描述
page_address		  输入 				 INT16U 		页地址
*source_address		输出			const INT8U *		源数据(512 bytes)	存放地址		
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

作      者	：许岩
日      期：2004-09-02
修改历史：
日期		修改人		修改描述
------		---------	-------------
******************************************************************************/
void FlashPageWrite256(INT16U page_address, const INT8U *source_address, INT8U which)
{
	INT8U LOCAL_V_TYPE high_address;
	INT8U LOCAL_V_TYPE middle_address;
	INT8U LOCAL_V_TYPE low_address;      
	INT16U  LOCAL_V_TYPE i;
	INT8U err = 0;

	ToggleWD();

	high_address = (INT8U)(page_address >> 6) & 0x7F;			//取得高位地址		
	middle_address = (INT8U)(page_address << 2) & 0xFC;		  //取得中位地址		
	low_address = 0;							//取得低位地址		

	OSSemPend(FLASH_Sem, 0, &err);

	while ( FlashBusy(which) == TRUE )
	{
		ToggleWD();	// Check Flash Busy
	}

// 	FlashDISENWP(which);				//解除写保护
	FLASH_SCK_SetHigh(which);
	FLASH_CS_SetLow(which);

	Send_SPI_8Bit(MAIN_BUFFER1_WRITE, which);	//发送写命令		
	Send_SPI_8Bit(high_address, which);     
	Send_SPI_8Bit(middle_address, which);       
	Send_SPI_8Bit(low_address, which);      

	for ( i=0; i< 256; i++ )		  //*source_address指向的地址中数据发到flash中		
		Send_SPI_8Bit(*source_address++, which);

	FLASH_CS_SetHigh(which);
	FLASH_SCK_SetLow(which);
// 	FlashENWP(which);				//始能写保护

	OSSemPost(FLASH_Sem);
}       

/******************************************************************************
函数名称：FlashPageRead256
功能描述：读Flash 中的某一页
参数描述：
参数名称：	输入/输出？	类型		描述
page_address		  输入 				 INT16U 		页地址
*source_address		输出			INT8U *		读出来的数据(长度为512)存放地址		
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

作      者	：许岩
日      期：2004-09-02
修改历史：
日期		修改人		修改描述
------		---------	-------------
******************************************************************************/
void FlashPageRead256(INT16U page_address,INT8U *source_address, INT8U which)
{
	INT16U  LOCAL_V_TYPE i;     
	INT8U LOCAL_V_TYPE high_address;
	INT8U LOCAL_V_TYPE middle_address;      
	INT8U err = 0;

	ToggleWD();

	high_address = (INT8U)(page_address >> 6) & 0x7F;			//取得高位地址		
	middle_address = (INT8U)(page_address << 2) & 0xFC;		  //取得中位地址		

	OSSemPend(FLASH_Sem, 0, &err);

	while ( FlashBusy(which) == TRUE )
	{
		ToggleWD();	// Check Flash Busy
	}

// 	FlashDISENWP();				//解除写保护
	FLASH_SCK_SetHigh(which);
	FLASH_CS_SetLow(which);

	Send_SPI_8Bit(MAIN_PAGE_READ, which);	//发送读命令		
	Send_SPI_8Bit(high_address, which);     
	Send_SPI_8Bit(middle_address, which);       
	Send_SPI_8Bit(0, which);        
	Send_SPI_8Bit(0, which);		//发送空字节		
	Send_SPI_8Bit(0, which);		//发送空字节		
	Send_SPI_8Bit(0, which);		//发送空字节		
	Send_SPI_8Bit(0, which);		//发送空字节		
	for ( i=0; i<256; i++ ) //开始接收数据放在aim_data_address指向的地址中		
		*source_address++ = Receive_SPI_8Bit(which);        

	FLASH_CS_SetHigh(which);
	FLASH_SCK_SetLow(which);
// 	FlashENWP();				//始能写保护

	OSSemPost(FLASH_Sem);
}  

