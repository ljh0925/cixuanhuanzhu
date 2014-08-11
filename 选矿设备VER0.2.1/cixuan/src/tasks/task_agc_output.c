/******************************************************************************
Copyright 2005 大连金德姆电子有限公司
All rights riserved.

文件名	：task_agc_output.c
模块名称：AGC输出任务程序
功能概要：AGC输出任务程序

取代版本：0.0.1
修改人	：许岩
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include 		"config.h"


extern volatile DEV_STAT DevStat;


/******************************************************************************
 函数名称：Task_Agc_Output
 功能描述：AGC输出任务
 参数描述：
 参数名称： 输入/输出？ 类型        描述

 返  回  值：无

 作      者 ：许岩
 日      期：2005-04-11
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
void Task_Agc_Output(void * data)
{
	INT16U rega = 0;
	INT16U regb = 0;

	ToggleWD();

	DevStat.MOD_REG.reg[0x0015] = 4000;
	DevStat.MOD_REG.reg[0x0016] = 4000;

	data = data;

	for(;;)
	{
		ToggleWD();

		OSTimeDlyHMSM(0, 0, 0, 30);            //30ms一次

		OS_ENTER_CRITICAL();
		rega = DevStat.MOD_REG.reg[0x0015];      //读使能AGC寄存器
		regb = DevStat.MOD_REG.reg[0x0016];      //读使能AGC寄存器
		OS_EXIT_CRITICAL();

		Ltc1448Output(rega,regb);					//AGC输出禁止，输出0V

	}
}

