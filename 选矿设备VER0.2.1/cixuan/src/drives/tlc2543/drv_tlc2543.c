/******************************************************************************
Copyright 2005 大连金德姆电子有限公司
All rights riserved.

文件名	：drv_tlc2543.c
模块名称：AD芯片TLC2543驱动程序
功能概要：AD芯片TLC2543驱动程序

取代版本：0.0.1
修改人	：许岩
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include "config.h"


/******************************************************************************
 函数名称：Tlc2543Init
 功能描述： AD芯片TLC2543驱动初始化
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void Tlc2543Init(void)
{
	ToggleWD();

//  PINSEL0 = PINSEL0 & (~BIT8);
//  PINSEL0 = PINSEL0 & (~BIT9);		//P2.24 配置成GPIO		AD_CLK
//  PINSEL0 = PINSEL0 & (~BIT10);
//  PINSEL0 = PINSEL0 & (~BIT11);		//P2.28 配置成GPIO		AD_DOUT
//  PINSEL0 = PINSEL0 & (~BIT12);
//  PINSEL0 = PINSEL0 & (~BIT13);		//P2.25 配置成GPIO		AD_DIN
//  PINSEL0 = PINSEL0 & (~BIT14);
//  PINSEL0 = PINSEL0 & (~BIT15);		//P2.29 配置成GPIO		AD_CS
//  PINSEL1 = PINSEL1 & (~BIT2);
//  PINSEL1 = PINSEL1 & (~BIT3);		//P2.23 配置成GPIO		AD_EOC

//  PINSEL2 = PINSEL2 | (BIT4) | (BIT5);  //P2设置为通用GPIO

	IO2DIR |= BIT24;			//P2.24设置为输出
	IO2DIR |= BIT25;			//P2.25设置为输出
	IO2DIR |= BIT29;			//P2.29设置为输出
	IO2DIR &= (~BIT23);			//P2.23设置为输入
	IO2DIR &= (~BIT28);			//P2.28设置为输入

	AD_CS_SET_HIGH();
	AD_CLK_SET_LOW();
	AD_DIN_SET_LOW();
}

/******************************************************************************
 函数名称：SPI_CLK
 功能描述：SPI CLOCK一下
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void SPI_CLK(void)
{
//  ToggleWD();

	Tlc2543Delay();
	AD_CLK_SET_HIGH();
	Tlc2543Delay();
	AD_CLK_SET_LOW();
	Tlc2543Delay();
}

/******************************************************************************
 函数名称：Get2543Channel
 功能描述：取得AD芯片TLC2543某一路的值
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 channel	输入		INT8U		要采样的通道号
				
 返  回  值：AD采样值,0-4095
				   
 作 	 者 ：许岩
 日 	 期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT16U Get2543Channel(INT8U channel)
{
	INT8U i = 0;
	INT16U result = 0;
	INT16U send = 0;

	ToggleWD();

	OS_ENTER_CRITICAL();

	AD_CLK_SET_LOW();
	Tlc2543Delay();
	AD_CS_SET_LOW();
	Tlc2543Delay();

	send = (INT16U)channel << 4;

	for ( i=0; i<12; i++ )
	{
//      if ( ((send << i) & 0x80) != 0 )
		if ( ((send << i) & 0x0080) != 0 )
		{
			AD_DIN_SET_HIGH();
		}
		else
		{
			AD_DIN_SET_LOW();
		}
		SPI_CLK();
	}

	AD_CS_SET_HIGH();

	OS_EXIT_CRITICAL();

	for ( i=0; i<100; i++ )
	{
		Sleep10us(1);
		if ( (IO2PIN & BIT23) != 0 )
		{
			break;
		}
	}

	OS_ENTER_CRITICAL();

	AD_CS_SET_LOW();
	Tlc2543Delay();
	ToggleWD();
	if ( (IO2PIN & BIT28) == 0 )
	{
	}
	else
	{
		result |= 0x0001;
	}
//  result <<= 1;

	for ( i=0; i<11; i++ )
	{
		SPI_CLK();

		result <<= 1;
		if ( (IO2PIN & BIT28) == 0 )
		{
		}
		else
		{
			result |= 0x0001;
		}
	}

	SPI_CLK();

	AD_CS_SET_HIGH();

	OS_EXIT_CRITICAL();

	return result;
}

/******************************************************************************
 函数名称：Tlc2543Delay
 功能描述：SPI Delay
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void Tlc2543Delay(void)
{
//  ToggleWD();

	Sleep10us(1);
}



