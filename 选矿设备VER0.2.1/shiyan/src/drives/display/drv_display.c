/******************************************************************************
Copyright 2005 大连华科仪器有限公司
All rights riserved.

文件名	：drv_display.c
模块名称：显示驱动程序
功能概要：显示驱动程序

取代版本：0.0.1
修改人	：刘及华
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include 		"config.h"
//#include "ziku.c"



// OS_EVENT *DispSem;			//访问液晶互斥信号量在display_init 被初始化
extern OS_EVENT        *Lcd_Sem;		//访问液晶互斥信号量

extern OS_EVENT        *Back_Light_Ctrl_Mbox;

extern volatile DEV_STAT DevStat;
extern const INT32S BACK_LIGHT_DELAY;



/******************************************************************************
函数名称：display_init
功能描述：液晶屏初始化
参数描述：

返  回  值	：无
作      者	：许岩
日      期：2005-04-08
修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
void display_init(void)
{
	ToggleWD();

//	PINSEL2 = PINSEL2 & (~BIT4);       //P2.16~P2.22 配置成GPIO
//	PINSEL2 = PINSEL2 | (BIT5);       
	
	IO2DIR |= BIT17;			//P2.17 18 19 20 置为输出
	IO2DIR |= BIT18;
	IO2DIR |= BIT19;
	IO2DIR |= BIT20;

	//IO2DIR |= BIT21;			//P2.21 置为输出		板上的工作指示灯用

	SetLed(0, OFF);

//  SetBacklightON();
	SetBacklightOFF();

	Lcd_Init();
	Lcd_Cls();
}

/*****************************************************************
函数原型：PressKeyBkLtCtrl
功能描述：按键之后的背光控制处理
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：无
作      者：许岩
日      期：2005-10-15
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void PressKeyBkLtCtrl(void)
{
	ToggleWD();

//  if ( DevStat.back_light_mode == ALWAYS_OFF )
//  {
//      SetBacklightOFF();
//  }
//  else
//  {
		SetBacklightON();			//其他情况保持背光开
//  }
	OSMboxPost(Back_Light_Ctrl_Mbox, (void *)&BACK_LIGHT_DELAY);				   //5秒之内不关背光
}

/*****************************************************************
函数原型：Write_Lcd_Ctrlreg
 功能描述：向LCD寄存器写数据
 参数描述：
 参数名称：	输入/输出？	类型		描述
				data_to_write		输入	INT8U		要写入的数据，一个字节
				
 返  回  值：无
 作      者	：黄立国、许岩
 日 	 期：2004-03-01
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
*****************************************************************/
void Write_Lcd_Ctrlreg(INT8U data_to_write)
{
// 	INT8U i;
//	bit h;

	ToggleWD();

	OS_ENTER_CRITICAL();

// 	OutPortStat = (OutPortStat | 0x80);				//LCD CS SET HIGH
// 	EXTOUTPORT = OutPortStat;
	LCD_CS_SetHigh();				//LCD CS SET HIGH

// 	LCD_DATA = 1;				//发送头
	LCD_DI_SetHigh();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();

// 	LCD_DATA = 0;				//RW和RS全写0
	LCD_DI_SetLow();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();

// 	LCD_DATA =  (bit)(data_to_write & 0x80);			//发送高四位字节
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA = 0;				//补0
	LCD_DI_SetLow();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();

// 	LCD_DATA =  (bit)(data_to_write & 0x80);			//发送低四位字节
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA = 0;				//补0
	LCD_DI_SetLow();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();

// 	LCD_DATA = 0;				//发送尾
	LCD_DI_SetLow();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();

// 	OutPortStat = (OutPortStat & 0x7F);			//LCD CS SET LOW
// 	EXTOUTPORT = OutPortStat;
	LCD_CS_SetLow();			//LCD CS SET LOW

	OS_EXIT_CRITICAL();

}

/*****************************************************************
函数原型：Write_Lcd_Ram
 功能描述：向LCD RAM写数据
 参数描述：
 参数名称：	输入/输出？	类型		描述
				data_to_write		输入	INT8U		要写入的数据，一个字节
				
 返  回  值：无
 作      者	：黄立国、许岩
 日 	 期：2004-03-01
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
*****************************************************************/
void Write_Lcd_Ram(INT8U data_to_write)
{
// 	INT8U i;
//	bit h;

// 	i = data_to_write;

	ToggleWD();

	OS_ENTER_CRITICAL();
// 	DISCS = HIGH;
// 	OutPortStat = (OutPortStat | 0x80);				//LCD CS SET HIGH
// 	EXTOUTPORT = OutPortStat;
	LCD_CS_SetHigh();				//LCD CS SET HIGH

// 	LCD_DATA = 1;				//发送头
	LCD_DI_SetHigh();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();

// 	LCD_DATA = 0;				//RW写0
	LCD_DI_SetLow();
	Dis_Clock();
// 	LCD_DATA = 1;				//RS写1
	LCD_DI_SetHigh();
	Dis_Clock();
// 	LCD_DATA = 0;
	LCD_DI_SetLow();
	Dis_Clock();

// 	LCD_DATA =  (bit)(data_to_write & 0x80);			//发送高四位字节
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA = 0;				//补0
	LCD_DI_SetLow();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();

// 	LCD_DATA =  (bit)(data_to_write & 0x80);			//发送低四位字节
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA = 0;				//补0
	LCD_DI_SetLow();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();

// 	LCD_DATA = 0;				//发送尾
	LCD_DI_SetLow();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();

// 	DISCS = LOW;
// 	OutPortStat = (OutPortStat & 0x7F);			//LCD CS SET LOW
// 	EXTOUTPORT = OutPortStat;
	LCD_CS_SetLow();			//LCD CS SET LOW

	OS_EXIT_CRITICAL();

// 	delay_50us(4);
	Sleep10us(20);

}

/*****************************************************************
函数原型：Dis_Clock
功能描述：提供LCD的SPI接口的CLK
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：无
作      者	：许岩
日      期：2004-03-01
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void Dis_Clock(void)
{
//  ToggleWD();

	LCD_CLK_SetLow();
 	Sleep10us(5);
//	Sleep10us(4);
	LCD_CLK_SetHigh();
}

/*****************************************************************
函数原型：Lcd_Init
功能描述：提供LCD的初始化
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：无
作      者	：黄立国、许岩
日      期：2004-03-01
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void Lcd_Init(void)
{
	ToggleWD();

	Write_Lcd_Ctrlreg(0x30);		//0011 X0XX B，设置8bit控制和基本指令集
	Sleep10us(30);
	Write_Lcd_Ctrlreg(0x30);		//0011 X0XX B，设置8bit控制和基本指令集
	Sleep10us(30);
	Write_Lcd_Ctrlreg(0x30);		//0011 X0XX B，设置8bit控制和基本指令集
	Sleep10us(30);
	Write_Lcd_Ctrlreg(0x30);		//0011 X0XX B，设置8bit控制和基本指令集
	Sleep10us(30);

//	Write_Lcd_Ctrlreg(0x0E);		//0001 1110 B，开显示，游标ON
//	delay_50us(3);
	Write_Lcd_Ctrlreg(0x0C);		//0001 1100 B，开显示，游标OFF
	Sleep10us(30);

	Write_Lcd_Ctrlreg(0x01);		//清除显示
	SleepMs(10);

	Write_Lcd_Ctrlreg(0x06);		//0000 0110 B，设定游标移动方向为右移
	Sleep10us(30);
}


/*****************************************************************
 函数原型：Write_Lcd_Gdram_X
 功能描述：向LCD GDRAM X地址寄存器写数据
 参数描述：
 参数名称：	输入/输出？	类型		描述
				data_to_write		输入	INT8U		要写入的数据，一个字节
				
 返  回  值：无
 作      者	：黄立国、许岩
 日 	 期：2004-04-05
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
*****************************************************************/
/*void Write_Lcd_Gdram_X(INT8U data_to_write)
{
	INT8U i;
//	bit h;

//	if (data_to_write >= 16)			// X坐标不允许超过15
//		return;
//
	i = data_to_write & 0x0F;		//高四位置0
	i = i | 0x80;					//第一位置1

	Write_Lcd_Ctrlreg(i);
	delay_50us(4);

	return;
}
*/
/*****************************************************************
 函数原型：Write_Lcd_Gdram_Y
 功能描述：向LCD GDRAM Y地址寄存器写数据
 参数描述：
 参数名称：	输入/输出？	类型		描述
				data_to_write		输入	INT8U		要写入的数据，一个字节
				
 返  回  值：无
 作      者	：黄立国、许岩
 日 	 期：2004-04-05
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
*****************************************************************/
/*void Write_Lcd_Gdram_Y(INT8U data_to_write)
{
	INT8U i;
//	bit h;

//	if (data_to_write >= 16)			// X坐标不允许超过15
//		return;
//
	i = data_to_write & 0x3F;		//高两位置0
	i = i | 0x80;					//第一位置1

	Write_Lcd_Ctrlreg(i);
	delay_50us(4);

	return;
}*/

/*****************************************************************
函数原型：Lcd_Clean_Line
功能描述：LCD清除指定行(全写为空格)
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：无
作      者	：黄立国、许岩
日      期：2004-03-01
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void Lcd_Clean_Line(INT8U line_to_clean)
{
	INT8U i;
	INT8U line_code = 0;

	ToggleWD();

	if ( line_to_clean > 3 )
		line_to_clean = 0;

	switch ( line_to_clean )
	{
		case 0:				//第1行
			line_code = 0x80;
			break;

		case 1:				//第2行
			line_code = 0x90;
			break;

		case 2:				//第3行
			line_code = 0x88;
			break;

		case 3:				//第4行
			line_code = 0x98;
			break;

		default:
			line_code = 0x98;	//第4行
	}
	Write_Lcd_Ctrlreg(line_code);		//AC地址写为要清除行的开头
// 	delay_50us(4);
	Sleep10us(20);

	for ( i=0; i<16; i++ )
	{
		Write_Lcd_Ram(0x20);				//空格
// 		delay_50us(4);
// 		Sleep10us(20);
	}
}

/*****************************************************************
函数原型：Lcd_Clean_Line
功能描述：LCD将光标移动到某坐标，左上为(0, 0)，右下为(3, 7)
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：无
作      者	：黄立国、许岩
日      期：2004-03-01
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void Lcd_Goto(INT8U x, INT8U y)
{
	INT8U line_code;

	ToggleWD();

	//检查输入合法性
	if ( x > 3 )
		x = 3;
	if ( y > 7 )
		y = 7;

	switch ( x )
	{
		case 0:				//第1行
			line_code = 0x80;
			break;

		case 1:				//第2行
			line_code = 0x90;
			break;

		case 2:				//第3行
			line_code = 0x88;
			break;

		case 3:				//第4行
			line_code = 0x98;
			break;

		default:
			line_code = 0x80;	//第4行
	}
	Write_Lcd_Ctrlreg(line_code + y);		//AC地址写为要清除行的开头
// 	delay_50us(4);
	Sleep10us(20);
}

/*****************************************************************
函数原型：Lcd_Set_Backlight
功能描述：控制LCD背光的开关
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
backlight_status	输入	INT8U	ON-开背光，OFF-关背光

返  回  值：无
作      者	：许岩
日      期：2004-04-01
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void Lcd_Set_Backlight(INT8U backlight_status)
{
	ToggleWD();

	if ( backlight_status == ON )		  //开背光
	{
// 		OutPortStat2 = (OutPortStat2 & 0xFE);			//背光开
// 		EXTOUTPORT2 = OutPortStat2;
		SetBacklightON();
	}
	else						//关背光
	{
// 		OutPortStat2 = (OutPortStat2 | 0x01);			//背光关
// 		EXTOUTPORT2 = OutPortStat2;
		SetBacklightOFF();
	}
}

// /*****************************************************************
// 函数原型：Lcd_Display_Cursor
// 功能描述：LCD显示游标，左上为(1, 1)，右下为(4, 8)
// 参数描述：
// 参数名称：	输入/输出？	类型		描述
// -----------		-----------	------	   	-------
//
// 返  回  值：无
// 作      者	： 许岩
// 日      期：2004-03-01
// 修改历史：
// 日期		修改人		修改描述
// ------		---------	-------------
// *****************************************************************/
// void Lcd_Display_Cursor(INT8U x, INT8U y)
// {
// 	Lcd_Goto(x, y);
//
// 	Write_Lcd_Ctrlreg(0x0E);		//0001 1110 B，开显示，游标ON
// 	delay_50us(4);
// }
//
// /*****************************************************************
// 函数原型：Lcd_Clean_Cursor
// 功能描述：LCD关闭游标显示
// 参数描述：
// 参数名称：	输入/输出？	类型		描述
// -----------		-----------	------	   	-------
//
// 返  回  值：无
// 作      者	： 许岩
// 日      期：2004-03-01
// 修改历史：
// 日期		修改人		修改描述
// ------		---------	-------------
// *****************************************************************/
// void Lcd_Clean_Cursor(void)
// {
// 	Write_Lcd_Ctrlreg(0x0C);		//0001 1100 B，开显示，游标OFF
// 	delay_50us(4);
// }

/*****************************************************************
函数原型：Lcd_Cls
功能描述：LCD清屏全写为空格)
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：无
作      者	： 许岩
日      期：2004-03-01
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void Lcd_Cls(void)
{
	ToggleWD();

	Lcd_Clean_Line(0);
	Lcd_Clean_Line(1);
	Lcd_Clean_Line(2);
	Lcd_Clean_Line(3);
}

/*****************************************************************
函数原型：printlcd
功能描述：让LCD 显示
参数描述：
参数名称：	输入/输出？	类型		描述
	s			输入			const void*			显示内容的首地址

返  回  值：无
作      者	：许岩
日 	 期：2005-08-19
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void printlcd(const void *s)
{
	INT8U i = 0;
	INT8S *p = (INT8S *)s;

// 	while ( p[i] != '\0' )
	while ( (p[i] != '\0') && (i < 16) )
	{
        ToggleWD();
		Write_Lcd_Ram(p[i]);
		i++; 
	}
}

/*****************************************************************
函数原型：lcddisp
功能描述：LCD将光标移动到某坐标，左上为(0, 0)，右下为(3, 7)，然后显示字符串
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：无
作      者	：许岩
日      期：2004-03-01
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void lcddisp(INT8U x, INT8U y, void *s)
{
	INT8U err = 0;

	ToggleWD();

	OSSemPend(Lcd_Sem, 0, &err);

	Lcd_Goto(x, y);
	printlcd(s);

	OSSemPost(Lcd_Sem);
}



