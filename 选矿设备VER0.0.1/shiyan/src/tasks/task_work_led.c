/******************************************************************************
Copyright 2005 大连金德姆电子有限公司
All rights riserved.

文件名	：task_work_led.c
模块名称：工作指示灯任务程序
功能概要：工作指示灯任务程序

取代版本：0.0.1
修改人	：许岩
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include 		"config.h"



/******************************************************************************
 函数名称：Task_Work_Led
 功能描述：工作指示灯任务
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void Task_Work_Led(void * data)
{
	data = data;

	for(;;)
	{
		ToggleWD();

		OSTimeDlyHMSM(0, 0, 1, 0);
		SetLed(0, ON);
		OSTimeDlyHMSM(0, 0, 1, 0);
		SetLed(0, OFF);

	}
}

