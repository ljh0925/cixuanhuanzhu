/******************************************************************************
Copyright 2005 大连易达通电子有限公司
All rights riserved.

文件名	：drv_RTC.c
模块名称：时钟芯片驱动程序
功能概要：时钟芯片驱动程序

取代版本：0.0.1
修改人	：许岩
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include "config.h"

#define Rtc_USE_SEM


#ifdef Rtc_USE_SEM
extern OS_EVENT		*Rtc_Sem;
#endif


//OS_EVENT *RTCSem;           //访问时钟芯片二进制信号量


/*****************************************************************
函数原型：WriteRX8025
功能描述：把某个数值写到rx－RX8025的寄存器中
参数描述：
参数名称：	输入/输出？	类型		描述
	which_reg	输入				INT8U		RX8025 寄存器地址
	value		输入				INT8U		要写入的值

返  回  值：ok(0)-成功 
				notok(0xFF)－失败
作      者	：许岩
日      期：2004-12-1
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U WriteRX8025(INT8U which_reg, INT8U value)
{
	INT8U i = 0;
	INT8U j = 0;
//  INT8U h = 0;
#ifdef Rtc_USE_SEM
	INT8U err = 0;
#endif

	ToggleWD();

#ifdef Rtc_USE_SEM
	OSSemPend(Rtc_Sem, 0, &err);
#endif
	OS_ENTER_CRITICAL();

	i = Rtc_Start();					//如果Rtc_Start不过，重试10次
	if ( i != ok )
	{
		j = 20;
		while ( j )
		{
			Rtc_Stop();
			i = Rtc_Start();
			if ( i != ok )
			{
				j--;
			}
			else
			{
				break;
			}
			if ( j == 0 )
			{
				Rtc_Stop();
				OS_EXIT_CRITICAL();
#ifdef Rtc_USE_SEM
				OSSemPost(Rtc_Sem);
#endif
				return notok;
			}
		}
	}

	i = Rtc_Send(RX8025_WRITE);	//发送硬件地址以及写信号
	if (i != ok)
	{
		Rtc_Stop();
		OS_EXIT_CRITICAL();
#ifdef Rtc_USE_SEM
		OSSemPost(Rtc_Sem);
#endif
		return notok;
	}

//  i = Rtc_Send((which_reg << 4) & 0xF0);  //发送操作寄存器地址("0")
	i = Rtc_Send(which_reg);	//发送操作寄存器地址("0")
	if (i != ok)
	{
		Rtc_Stop();
		OS_EXIT_CRITICAL();
#ifdef Rtc_USE_SEM
		OSSemPost(Rtc_Sem);
#endif
		return notok;
	}

	i = Rtc_Send(value);	//发送硬件地址以及写信号
	if (i != ok)
	{
		Rtc_Stop();
		OS_EXIT_CRITICAL();
#ifdef Rtc_USE_SEM
		OSSemPost(Rtc_Sem);
#endif
		return notok;
	}

	Rtc_Stop();

	OS_EXIT_CRITICAL();
#ifdef Rtc_USE_SEM
		OSSemPost(Rtc_Sem);
#endif
	return ok;
}




/////////////////////////////////////////////////////////
// 以下为不需要改写部分
/////////////////////////////////////////////////////////


/*****************************************************************
函数原型：Get_Time
功能描述：读取时间，读取两次，为了防止偶尔一次不成功
参数描述：
参数名称：	输入/输出？	类型		描述
*time_data	输出   _TIME	7个字节长，数据从头到尾顺序是世纪 年 月 日 时 分 秒
返  回  值：ok(0)-成功 
				notok(0xFF)－失败
作      者	：许岩
日      期：2005-04-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U Get_Time(_TIME *time_data)
{
	INT8U i = 0;
//  INT8U err;

	ToggleWD();

//  OSSemPend(RTCSem, 0, &err);

	i = Get_Time_Lowlvl(time_data);
	if ( i != ok )
	{
		i = Get_Time_Lowlvl(time_data);
		if ( i != ok )
		{
//          OSSemPost(RTCSem);
			return notok;
		}
	}

//  OSSemPost(RTCSem);

	return ok;
}

/*****************************************************************
函数原型：Get_Time_Lowlvl
功能描述：读取rx－RX8025	内的时间
参数描述：
参数名称：	输入/输出？	类型		描述
*time_data	输出   _TIME	7个字节长，数据从尾到头顺序是世纪 年 月 日 时 分 秒
返  回  值：ok(0)-成功 
				notok(0xFF)－失败
作      者	：许岩
日      期：2005-04-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U Get_Time_Lowlvl(_TIME *time_data)
{
	INT8U i = 0;
	INT8U start_reg = 0;		//从第0个寄存器开始读取
	INT8U buf[9];

	ToggleWD();

	i = Rtc_SendAndReceive(RX8025, start_reg, buf, 7);
	if (i != ok)
	{
		return notok;
	}

	time_data->second = buf[0];				//秒
	time_data->minute = buf[1];				//分
	time_data->hour = buf[2];				//小时
	time_data->day = buf[4];				//日
	time_data->month = buf[5];				//月
	time_data->year = buf[6];				//年
	time_data->century = 0x20;				//世纪定死为0x20

	return ok;
}

/*****************************************************************
函数原型：Modify_Time
功能描述：修改时间，修改两次，为了防止偶尔一次不成功
参数描述：
参数名称：	输入/输出？	类型		描述
*time_data	输入	_TIME	7个字节长，数据从头到尾顺序是世纪 年 月 日 时 分 秒
返  回  值：ok(0)-成功 
				notok(0xFF)－失败
作      者	：许岩
日      期：2005-04-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U Modify_Time(_TIME const * const time_data)
{
	INT8U i;
//  INT8U err;

	ToggleWD();

//  OSSemPend(RTCSem, 0, &err);

	i = Modify_Time_Lowlvl(time_data);
	if ( i != ok )
	{
		i = Modify_Time_Lowlvl(time_data);
		if ( i != ok )
		{
//          OSSemPost(RTCSem);
			return notok;
		}
	}

//  OSSemPost(RTCSem);

	return ok;
}

/*****************************************************************
函数原型：Modify_Time_Lowlvl
功能描述：修改rx-RX8025 内的时间
参数描述：
参数名称：	输入/输出？	类型		描述
*time_data	输入	_TIME	7个字节长，数据从尾到头顺序是
											世纪 年 月 日 时 分 秒
返  回  值：ok(0)-成功 
				notok(0xFF)－失败
作      者	：许岩
日      期：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U Modify_Time_Lowlvl(_TIME const * const time_data)			//指针指向的数是不可修改的，同时指针也是不可修改的
{
	INT8U i;

	ToggleWD();

	i = WriteRX8025(SEC_REG, time_data->second);				//秒
	if ( i != ok )
		return notok;

	i = WriteRX8025(MIN_REG, time_data->minute);				//分
	if ( i != ok )
		return notok;

	i = WriteRX8025(HOUR_REG, time_data->hour);				//小时
	if ( i != ok )
		return notok;

	i = WriteRX8025(DAY_REG, time_data->day);					//日
	if ( i != ok )
		return notok;

	i = WriteRX8025(MONTH_REG, time_data->month);				//月
	if ( i != ok )
		return notok;

	i = WriteRX8025(YEAR_REG, time_data->year);				//年
	if ( i != ok )
		return notok;

	return ok;
}

/*****************************************************************
函数原型：RX8025Init
功能描述：对rx－RX8025 进行初始化，设置为24 小时模式,初始化时调用
参数描述：
参数名称：	输入/输出？	类型		描述

返  回  值：ok(0)-成功 
				notok(0xFF)－失败
作      者	：许岩
日      期：2005-04-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U RX8025Init(void)
{
	INT8U i;
	INT8U m;

	ToggleWD();

	for ( i=0; i<4; i++ )
	{
		m = WriteRX8025(CTRL1_REG, 0x20);		//24小时模式
		if ( m != ok )
			continue;

		m = WriteRX8025(CTRL2_REG, 0x20);		//2.1v电压检测
		if ( m != ok )
			continue;

		break;
	}

	if ( i >= 4 )
		return notok;
	else
		return ok;
}

/*****************************************************************
函数原型：CheckTimeFormat
功能描述：检查时间格式是否合法
参数描述：
参数名称：	输入/输出？	类型		描述
time		输入  			const _TIME *	7个字节长，数据从头到尾顺序是世纪 年 月 日 时 分 秒

返  回  值：ok(0)-成功 
				notok(0xFF)－失败
作      者	：许岩
日      期：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U CheckTimeFormat(const _TIME *time)
{   
	INT8U temp;

	ToggleWD();

	temp = time->month;
	if ( temp >= 0x0a )		  //月份应在1－12之间
	{
		if ( temp >= 0x10 && temp <= 0x12 )
			;
		else
			return notok;
	}

	temp = time->day;
	if ( temp > 0x31 )					 //天应在1-31之间
		return notok;
	temp = temp&0x0F;
	if ( temp >0x09 )
		return notok;

	temp = time->hour;
	if ( temp > 0x23 )									 //小时应在0-23之间
		return notok;
	temp = temp & 0x0F;
	if ( temp >0x09 )
		return notok;

	temp = time->minute;
	if ( temp > 0x59 )									 //分钟应在0-59之间
		return notok;
	temp = temp & 0x0F;
	if ( temp > 0x09 )
		return notok;

	temp = time->second;
	if ( temp > 0x59 )									  //秒应在0-59之间
		return notok;
	temp = temp & 0x0F;
	if ( temp > 0x09 )
		return notok;

	return ok;
}


