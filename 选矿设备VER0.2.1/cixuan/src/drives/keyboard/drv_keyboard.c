/******************************************************************************
Copyright 2005 大连金德姆电子有限公司
All rights riserved.

文件名	：drv_display.c
模块名称：显示和键盘驱动程序
功能概要：显示和键盘驱动程序

取代版本：0.0.1
修改人	：许岩
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include 		"config.h"



const INT32S BACK_LIGHT_DELAY = 10;			//10秒
extern OS_EVENT        *Back_Light_Ctrl_Mbox;

/******************************************************************************
函数名称：keyboard_init
功能描述：键盘控制芯片ZLG7290初始化
参数描述：

返  回  值	：无
作      者	：许岩
日      期：2005-04-08
修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
void keyboard_init(void)
{
	ToggleWD();

	PINSEL1 = PINSEL1 & (~BIT18);
	PINSEL1 = PINSEL1 & (~BIT19);       //P0.25 配置成GPIO      IRQ

	PINSEL1 = PINSEL1 & (~BIT22);
	PINSEL1 = PINSEL1 & (~BIT23);       //P0.27 配置成GPIO      RESET

	IO0DIR &= (~BIT25);             //输入
	IO0DIR |= BIT27;                //输出

	RESET_7290();
}

/******************************************************************************
函数名称：RESET_7290() 
功能描述：用I/O和7290复位
参数描述：

返  回  值	：无
作      者	：许岩
日      期：2005-10-10
修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
void RESET_7290() 
{
	//ToggleWD();

	RESET_7290Low();
//  SleepMs(50);
	(void)OSTimeDlyHMSM(0, 0, 0, 50);

	RESET_7290High();
//  SleepMs(500);
	(void)OSTimeDlyHMSM(0, 0, 0, 500);
}

/******************************************************************************
函数名称：get_key
功能描述：得到一个按键，死等
参数描述：无

返  回  值	：该键值
作      者	：张坛
日      期：2005-04-08
修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
INT8U get_key(void)
{
	INT8U temp[2];
//     INT8U key_number ,i=0;
//	INT8U  er;

	//ToggleWD();

	for ( ;; )				   //直到得到一个有效按键为止
	{
		//ToggleWD();

// 		OSSemPend(KeySem, 0, &er);		/***无限等待接收数据***///从key—task。c中移植的

		for(;;)
		{
			if ( (IO0PIN & BIT25) == 0 )
			{
				break;
			}
			OSTimeDlyHMSM(0, 0, 0, 100);
		}

// 		temp[0] = 0;
// 		temp[1] = 1;
// 		I2cRead(ZLG7290ADDR, temp, temp + 1, 1, 2);
		temp[0] = 0xFF;
		(void)I2c_SendAndReceive(ZLG7290ADDR, SubKey, temp, 1);


		//////////////////////////////////////////////////////////////
		//转换成INT8U的键值
		/////////////////////////////////////////////////////////////
		switch ( temp[0] )
		{
			case KEY_0:
				temp[0]='0';
				break;

			case KEY_1:
				temp[0]='1';
				break;

			case KEY_2:
				temp[0]='2';
				break;

			case KEY_3:
				temp[0]='3';
				break;

			case KEY_4:
				temp[0]='4';
				break;

			case KEY_5:
				temp[0]='5';
				break;

			case KEY_6:
				temp[0]='6';
				break;

			case KEY_7:
				temp[0]='7';
				break;

			case KEY_8:
				temp[0]='8';
				break;

			case KEY_9:
				temp[0]='9';
				break;

// 			case KEY_POINT:
// 				temp[0]='.';
// 				break;
//
// 			case KEY_ADD:
// 				temp[0]='+';
// 				break;
//
// 			case KEY_SUB:
// 				temp[0]='-';
// 				break;
//
// 			case KEY_MUL:
// 				temp[0]='*';
// 				break;
//
// 			case KEY_EQUAL:
// 				temp[0]='=';
// 				break;

			case KEY_ENTER:
			case KEY_CANCEL:
// 			case KEY_MODE:
// 			case KEY_FUNC:
				break;

			default:
				temp[0]=NO_KEY;
				break;
		}

		if (temp[0] != NO_KEY)
		{
			PressKeyBkLtCtrl();
			Beep2(150);
		}

		return(temp[0]);
	}
}

/******************************************************************************
函数名称：query_key
功能描述：用查询方式得到一个按键，不死等
参数描述：无

返  回  值	：该键值
作      者	：许岩
日      期：2005-04-08
修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
INT8U query_key(void)
{
	INT8U temp[2];
//     INT8U key_number ,i=0;
//     INT8U  er;
//	INT16U j;

	//ToggleWD();

// 	j = OSSemAccept(KeySem);		//查询一下是否有按键
// 	if ( j == 0 )
// 	{
// 		return NO_KEY;			//没有按键
// 	}
	if ( (IO0PIN & BIT25) == 0 )
	{
	}
	else
	{
		return NO_KEY;			//没有按键
	}


// 	temp[0] = 0;
// 	temp[1] = 1;
// 	I2cRead(ZLG7290ADDR, temp, temp + 1, 1, 2);
	temp[0] = 0xFF;
	(void)I2c_SendAndReceive(ZLG7290ADDR, SubKey, temp, 1);

	//////////////////////////////////////////////////////////////
	//转换成INT8U的键值
	/////////////////////////////////////////////////////////////
	switch ( temp[0] )
	{
		case KEY_0:
			temp[0]='0';
			break;

		case KEY_1:
			temp[0]='1';
			break;

		case KEY_2:
			temp[0]='2';
			break;

		case KEY_3:
			temp[0]='3';
			break;

		case KEY_4:
			temp[0]='4';
			break;

		case KEY_5:
			temp[0]='5';
			break;

		case KEY_6:
			temp[0]='6';
			break;

		case KEY_7:
			temp[0]='7';
			break;

		case KEY_8:
			temp[0]='8';
			break;

		case KEY_9:
			temp[0]='9';
			break;

// 		case KEY_POINT:
// 			temp[0]='.';
// 			break;
//
// 		case KEY_ADD:
// 			temp[0]='+';
// 			break;
//
// 		case KEY_SUB:
// 			temp[0]='-';
// 			break;
//
// 		case KEY_MUL:
// 			temp[0]='*';
// 			break;
//
// 		case KEY_EQUAL:
// 			temp[0]='=';
// 			break;

		case KEY_ENTER:
		case KEY_CANCEL:
// 		case KEY_MODE:
// 		case KEY_FUNC:
			break;

		default:
			temp[0]=NO_KEY;
			break;
	}
	if (temp[0] != NO_KEY)
	{
		PressKeyBkLtCtrl();
		Beep2(150);
	}

	return(temp[0]);
}


/******************************************************************************
函数名称：query_key_nsnd
功能描述：用查询方式得到一个按键，不死等，比query_key省去语音部分
参数描述：无

返  回  值	：该键值
作      者	：许岩
日      期：2005-04-08
修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
INT8U query_key_nsnd(void)
{
	INT8U temp[2];

	if ( (IO0PIN & BIT25) == 0 )
	{
	}
	else
	{
		return NO_KEY;			//没有按键
	}


// 	temp[0] = 0;
// 	temp[1] = 1;
// 	I2cRead(ZLG7290ADDR, temp, temp + 1, 1, 2);
	temp[0] = 0xFF;
	(void)I2c_SendAndReceive(ZLG7290ADDR, SubKey, temp, 1);

	//////////////////////////////////////////////////////////////
	//转换成INT8U的键值
	/////////////////////////////////////////////////////////////
	switch ( temp[0] )
	{
		case KEY_0:
			temp[0]='0';
			break;

		case KEY_1:
			temp[0]='1';
			break;

		case KEY_2:
			temp[0]='2';
			break;

		case KEY_3:
			temp[0]='3';
			break;

		case KEY_4:
			temp[0]='4';
			break;

		case KEY_5:
			temp[0]='5';
			break;

		case KEY_6:
			temp[0]='6';
			break;

		case KEY_7:
			temp[0]='7';
			break;

		case KEY_8:
			temp[0]='8';
			break;

		case KEY_9:
			temp[0]='9';
			break;

// 		case KEY_POINT:
// 			temp[0]='.';
// 			break;
//
// 		case KEY_ADD:
// 			temp[0]='+';
// 			break;
//
// 		case KEY_SUB:
// 			temp[0]='-';
// 			break;
//
// 		case KEY_MUL:
// 			temp[0]='*';
// 			break;
//
// 		case KEY_EQUAL:
// 			temp[0]='=';
// 			break;

		case KEY_ENTER:
		case KEY_CANCEL:
// 		case KEY_MODE:
// 		case KEY_FUNC:
			break;

		default:
			temp[0]=NO_KEY;
			break;
	}

	return(temp[0]);
}

