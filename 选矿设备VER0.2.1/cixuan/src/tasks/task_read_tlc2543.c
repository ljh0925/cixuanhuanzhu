/******************************************************************************
Copyright 2005 大连金德姆电子有限公司
All rights riserved.

文件名	：task_read_tlc2543.c
模块名称：读取AD值任务程序
功能概要：读取AD值任务程序

取代版本：0.0.1
修改人	：许岩
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include 		"config.h"


extern volatile DEV_STAT DevStat;


/******************************************************************************
 函数名称：Task_Read_TLC2543
 功能描述：读取AD值任务
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void Task_Read_TLC2543(void * data)
{
	INT8U i = 0;
	INT16U ad = 0;
	FP32 temp = 0.0;		//摄氏温度

	ToggleWD();

	data = data;

	for( ;; )
	{
		ToggleWD();

		for ( i=0; i<1; i++ )
		{
			ad = Get2543Channel(i);
			OS_ENTER_CRITICAL();
			DevStat.ad_avg[i][DevStat.ad_avg_ptr] = ad; 
			OS_EXIT_CRITICAL();
		}
		OS_ENTER_CRITICAL();
		DevStat.ad_avg_ptr++;
		if ( DevStat.ad_avg_ptr >= AD_QUEUE_LEN )
			DevStat.ad_avg_ptr = 0;
		OS_EXIT_CRITICAL();

		OS_ENTER_CRITICAL();
		for ( i=0; i<1; i++ )
		{
			DevStat.MOD_REG.reg[0x0010 + i] 
			= AVERAGE((INT16U *)&DevStat.ad_avg[i][0]);
		}
		OS_EXIT_CRITICAL();

		//算出铁粉含量对应的电压值
		OS_ENTER_CRITICAL();

		for(i=0;i<1;i++)
		{
			temp = (((FP64)DevStat.MOD_REG.reg[0x0010 + i] * 5.0) / 4095.0);
			DevStat.MOD_REG.reg[0x0010 + i] = (INT16S)(temp * 1000.0);
		}

		OS_EXIT_CRITICAL();

		OSTimeDlyHMSM(0, 0, 0, 100);			//100ms一次
	}
}

/******************************************************************************
 函数名称：average
 功能描述：算AD队列的平均值
 参数描述：
 参数名称： 输入/输出？ 类型        描述
 adc_data   输入    	INT16U*        AD值队列

 返  回  值：AD的平均数

 作      者 ：许岩
 日      期：2005-11-02
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
INT16U AVERAGE(INT16U* adc_data)
{
	INT32U sum = 0;
	INT8U i = 0;

	ToggleWD();

	for ( i=0; i<AD_QUEUE_LEN; i++ )
	{
		sum += (INT32U)adc_data[i];
	}

	return(INT16U)(sum / AD_QUEUE_LEN);
}

