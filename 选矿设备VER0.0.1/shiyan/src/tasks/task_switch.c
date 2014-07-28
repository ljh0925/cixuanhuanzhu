/******************************************************************************
Copyright 2005 大连金德姆电子有限公司
All rights riserved.

文件名	：task_pushc.c
模块名称：推拉C型架任务程序
功能概要：推拉C型架任务程序

取代版本：0.0.1
修改人	：许岩
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include 		"config.h"


extern volatile DEV_STAT DevStat;


/******************************************************************************
 函数名称：Task_PushC
 功能描述：推拉C型架任务
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void Task_Switch0(void * data)
{
	INT16U cmd = 0;
	INT8U temp_Sec;
	INT16U temp_Mil;
	
	data = data;

	for( ;; )
	{
		ToggleWD();

		OS_ENTER_CRITICAL();
		cmd = DevStat.MOD_REG.reg[0x18];		//设置继电器闭合断开控制
		OS_EXIT_CRITICAL();

		if ( cmd == 0x5555 )		//开始工作
		{
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_delay[0]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_delay[0]%1000;
			   
			   if((temp_Sec * 1000 + temp_Mil) > 10)
			   {
				   IO2SET = (BIT30);
				   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);  
			   }

			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[0]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[0]%1000;
			   
			   if((temp_Sec * 1000 + temp_Mil) > 10)
			   {
				   IO2CLR = (BIT30);
				   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);  
			   }

			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[0]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[0]%1000;
			   
			   if((temp_Sec * 1000 + temp_Mil) > 10)
			   {
			
				   IO2SET = (BIT30);
				   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   }
			  
			       OSTimeDlyHMSM(0, 0, 0, 10);
		}
		else
		{
			OSTimeDlyHMSM(0, 0, 0, 10);			//200ms一次
		}
				
	}
}

void Task_Switch1(void * data)
{
	INT16U cmd = 0;

	INT8U temp_Sec;
	INT16U temp_Mil;
		data = data;
	for(;;)
	{
		ToggleWD();

		OS_ENTER_CRITICAL();
		cmd = DevStat.MOD_REG.reg[0x18];        //设置继电器闭合断开控制
		OS_EXIT_CRITICAL();

		if (cmd == 0x5555)      //前进到位
		{

				temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_delay[1]/1000;
				temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_delay[1]%1000;

				if((temp_Sec * 1000 + temp_Mil) > 10)
				{
					IO2SET = (BIT31);
					OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);  
				}

			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[1]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[1]%1000;
			   
			   if((temp_Sec * 1000 + temp_Mil) > 10)
			   {		 
				   IO2CLR = (BIT31);
				   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   }
			   
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[1]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[1]%1000;
			   
			   if((temp_Sec * 1000 + temp_Mil) > 10)
			   {
				   IO2SET = (BIT31);
				   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);  
			   }
			   OSTimeDlyHMSM(0, 0, 0, 10);

		}
		else
		{
			OSTimeDlyHMSM(0, 0, 0, 10);         //200ms一次
		}
				
	}
}

void Task_Switch2(void * data)
{
	INT16U cmd = 0;
	
	INT8U temp_Sec;
	INT16U temp_Mil;
	data = data;

	for(;;)
	{
		ToggleWD();

		OS_ENTER_CRITICAL();
		cmd = DevStat.MOD_REG.reg[0x18];        //设置继电器闭合断开控制
		OS_EXIT_CRITICAL();

		if (cmd == 0x5555)      //前进到位
		{
				temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_delay[2]/1000;
				temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_delay[2]%1000;

				if((temp_Sec * 1000 + temp_Mil) > 10)
				{
					IO1SET = BIT17;
					OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);  
				}

			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[2]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[2]%1000;
			   
			   if((temp_Sec * 1000 + temp_Mil) > 10)
			   {	   
				   IO1CLR = BIT17;
				   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   }
			   //SleepMs(2000);
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[2]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[2]%1000;
			   
			   if((temp_Sec * 1000 + temp_Mil) > 10)
			   {
					IO1SET = BIT17;
					OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   }
			   OSTimeDlyHMSM(0, 0, 0, 10);

		}
		else
		{
			OSTimeDlyHMSM(0, 0, 0, 10);         //200ms一次
		}
				
	}
}

void Task_Switch3(void * data)
{
	INT16U cmd = 0;
	
	INT8U temp_Sec;
	INT16U temp_Mil;
data = data;
	for(;;)
	{
		ToggleWD();

		OS_ENTER_CRITICAL();
		cmd = DevStat.MOD_REG.reg[0x18];        //设置继电器闭合断开控制
		OS_EXIT_CRITICAL();

		if (cmd == 0x5555)      //起始工作
		{
				temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_delay[3]/1000;
				temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_delay[3]%1000;

				if((temp_Sec * 1000 + temp_Mil) > 10)
				{
					IO0SET = BIT28;
					OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);  
				}

			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[3]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[3]%1000;
			   
			   if((temp_Sec * 1000 + temp_Mil) > 10)
			   {
				   IO0CLR = BIT28;
				   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   }
			   
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[3]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[3]%1000;
			   
			   if((temp_Sec * 1000 + temp_Mil) > 10)
			   {
				   IO0SET = BIT28;
				   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   }
			   OSTimeDlyHMSM(0, 0, 0, 10);
		}
		else
		{
			OSTimeDlyHMSM(0, 0, 0, 10);         //200ms一次
		}
				
	}
}

void Task_Switch4(void * data)
{
	INT16U cmd = 0;
	
	INT8U temp_Sec;
	INT16U temp_Mil;
data = data;
	for(;;)
	{
		ToggleWD();

		OS_ENTER_CRITICAL();
		cmd = DevStat.MOD_REG.reg[0x18];        //设置继电器闭合断开控制
		OS_EXIT_CRITICAL();

		if (cmd == 0x5555)      //前进到位
		{
				temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_delay[4]/1000;
				temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_delay[4]%1000;

				if((temp_Sec * 1000 + temp_Mil) > 10)
				{
					IO3SET = BIT29;
					OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);  
				}

			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[4]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[4]%1000;
			   
			   if((temp_Sec * 1000 + temp_Mil) > 10)
			   {
				   IO3CLR = BIT29;
				   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   }
			   
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[4]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[4]%1000;
			   
			   if((temp_Sec * 1000 + temp_Mil) > 10)
			   {
				   IO3SET = BIT29;
				   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   }
			   OSTimeDlyHMSM(0, 0, 0, 10);

		}
		else
		{
			OSTimeDlyHMSM(0, 0, 0, 10);         //200ms一次
		}
				
	}
}

void Task_Switch5(void * data)
{
	INT16U cmd = 0;
	
	INT8U temp_Sec;
	INT16U temp_Mil;
data = data;
	for(;;)
	{
		ToggleWD();

		OS_ENTER_CRITICAL();
		cmd = DevStat.MOD_REG.reg[0x18];        //设置继电器闭合断开控制
		OS_EXIT_CRITICAL();

		if (cmd == 0x5555)      //前进到位
		{
				temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_delay[5]/1000;
				temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_delay[5]%1000;

				if((temp_Sec * 1000 + temp_Mil) > 10)
				{
					IO3SET = BIT28;
					OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);  
				}

			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[5]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[5]%1000;
			   
			   if((temp_Sec * 1000 + temp_Mil) > 10)
			   {
				   IO3CLR = BIT28;
				   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   }
			  
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[5]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[5]%1000;
			   
			   if((temp_Sec * 1000 + temp_Mil) > 10)
			   {
				   IO3SET = BIT28;
				   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   }
			  OSTimeDlyHMSM(0, 0, 0, 10);
		}
		else
		{
			OSTimeDlyHMSM(0, 0, 0, 10);         //200ms一次
		}
				
	}
}

void Task_Switch6(void * data)
{
	INT16U cmd = 0;
	
	INT8U temp_Sec;
	INT16U temp_Mil;
data = data;
	for(;;)
	{
		ToggleWD();

		OS_ENTER_CRITICAL();
		cmd = DevStat.MOD_REG.reg[0x18];        //设置继电器闭合断开控制
		OS_EXIT_CRITICAL();

		if (cmd == 0x5555)      //前进到位
		{
				temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_delay[6]/1000;
				temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_delay[6]%1000;

				if((temp_Sec * 1000 + temp_Mil) > 10)
				{
					IO0SET = BIT29;
					OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);  
				}

			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[6]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[6]%1000;
			   
			   if((temp_Sec * 1000 + temp_Mil) > 10)
			   {
				   IO0CLR = BIT29;
				   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   }
			   
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[6]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[6]%1000;
			   
			   if((temp_Sec * 1000 + temp_Mil) > 10)
			   {
				   IO0SET = BIT29;
				   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   }
			   OSTimeDlyHMSM(0, 0, 0, 10);

		}
		else
		{
			OSTimeDlyHMSM(0, 0, 0, 10);         //200ms一次
		}
				
	}
}

void Task_Switch7(void * data)
{
	INT16U cmd = 0;
	
	INT8U temp_Sec;
	INT16U temp_Mil;
	data = data;
	for( ;; )
	{
		ToggleWD();

		OS_ENTER_CRITICAL();
		cmd = DevStat.MOD_REG.reg[0x18];        //设置继电器闭合断开控制
		OS_EXIT_CRITICAL();

		if (cmd == 0x5555)      //前进到位
		{
				temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_delay[7]/1000;
				temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_delay[7]%1000;

				if((temp_Sec * 1000 + temp_Mil) > 10)
				{
					IO0SET = BIT30;
					OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);  
				}

			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[7]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[7]%1000;
			   
			   if((temp_Sec * 1000 + temp_Mil) > 10)
			   {
				   IO0CLR = BIT30;
				   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   }
			   
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[7]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[7]%1000;
			   
			   if((temp_Sec * 1000 + temp_Mil) > 10)
			   {
				   IO0SET = BIT30;
				   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   }
			   
			   OSTimeDlyHMSM(0, 0, 0, 10);

		}
		else
		{
			OSTimeDlyHMSM(0, 0, 0, 10);         //200ms一次
		}

		
	}
}
