/******************************************************************************
Copyright 2005 大连金德姆电子有限公司
All rights riserved.

文件名	：Task_actuator
模块名称：4-20ma控制信号输出任务程序

取代版本：0.0.1
修改人	：刘及华
完成日期：2014.04.06
升级说明：create

******************************************************************************/
#include 		"config.h"


extern volatile DEV_STAT DevStat;

/******************************************************************************
 函数名称：Task_actuator
 功能描述：开门报警指示
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2014-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void Task_actuator(void * data)
{
	INT16U cmd = 0;
	
	INT32S temp_int32s;
	float gap;
	data = data;

	for( ;; )
	{
		ToggleWD();

		OS_ENTER_CRITICAL();
		temp_int32s = ( DevStat.MOD_REG.reg[0x0010]  - DevStat.MOD_REG.reg[0x0011] );
		gap = (float)(temp_int32s / (DevStat.MOD_REG.reg[0x0010] + 1));
		OS_EXIT_CRITICAL();
		
		if ( gap <= 0.1 )		  //看是否为正
		{
			//开启阀门
			ad5410_control(0x01, 0);
		}
		if ( 0.1 < (gap) <= 0.25 )		  //看是否为正
		{
			//开启阀门
			ad5410_control(0x01, (INT16U)(1024));
		}
		if(0.25 <(gap) < 0.5)
		{
			//关闭阀门
			ad5410_control(0x01, 2048);
		}
		if(0.5 <= (gap) < 0.75)
		{
			//关闭阀门
			ad5410_control(0x01, 3072);
		}
		if(0.75 <= (gap))
		{
			//关闭阀门
			ad5410_control(0x01, 4095);
		}
		
		OSTimeDlyHMSM(0, 0, 1, 0);      //1s刷新一次
				
	}
}


