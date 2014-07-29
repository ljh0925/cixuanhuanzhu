/******************************************************************************
Copyright 2005 �������ķ�������޹�˾
All rights riserved.

�ļ���	��task_pushc.c
ģ�����ƣ�����C�ͼ��������
���ܸ�Ҫ������C�ͼ��������

ȡ���汾��0.0.1
�޸���	������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include 		"config.h"


extern volatile DEV_STAT DevStat;


/******************************************************************************
 �������ƣ�Task_PushC
 ��������������C�ͼ�����
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ������
 �� 	 �ڣ�2005-04-11
 �޸���ʷ��
		����		�޸���		�޸�����
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
		cmd = DevStat.MOD_REG.reg[0x18];		//���ü̵����պ϶Ͽ�����
		OS_EXIT_CRITICAL();

		if ( cmd == 0x5555 )		//��ʼ����
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
			OSTimeDlyHMSM(0, 0, 0, 10);			//200msһ��
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
		cmd = DevStat.MOD_REG.reg[0x18];        //���ü̵����պ϶Ͽ�����
		OS_EXIT_CRITICAL();

		if (cmd == 0x5555)      //ǰ����λ
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
			OSTimeDlyHMSM(0, 0, 0, 10);         //200msһ��
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
		cmd = DevStat.MOD_REG.reg[0x18];        //���ü̵����պ϶Ͽ�����
		OS_EXIT_CRITICAL();

		if (cmd == 0x5555)      //ǰ����λ
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
			OSTimeDlyHMSM(0, 0, 0, 10);         //200msһ��
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
		cmd = DevStat.MOD_REG.reg[0x18];        //���ü̵����պ϶Ͽ�����
		OS_EXIT_CRITICAL();

		if (cmd == 0x5555)      //��ʼ����
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
			OSTimeDlyHMSM(0, 0, 0, 10);         //200msһ��
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
		cmd = DevStat.MOD_REG.reg[0x18];        //���ü̵����պ϶Ͽ�����
		OS_EXIT_CRITICAL();

		if (cmd == 0x5555)      //ǰ����λ
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
			OSTimeDlyHMSM(0, 0, 0, 10);         //200msһ��
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
		cmd = DevStat.MOD_REG.reg[0x18];        //���ü̵����պ϶Ͽ�����
		OS_EXIT_CRITICAL();

		if (cmd == 0x5555)      //ǰ����λ
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
			OSTimeDlyHMSM(0, 0, 0, 10);         //200msһ��
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
		cmd = DevStat.MOD_REG.reg[0x18];        //���ü̵����պ϶Ͽ�����
		OS_EXIT_CRITICAL();

		if (cmd == 0x5555)      //ǰ����λ
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
			OSTimeDlyHMSM(0, 0, 0, 10);         //200msһ��
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
		cmd = DevStat.MOD_REG.reg[0x18];        //���ü̵����պ϶Ͽ�����
		OS_EXIT_CRITICAL();

		if (cmd == 0x5555)      //ǰ����λ
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
			OSTimeDlyHMSM(0, 0, 0, 10);         //200msһ��
		}

		
	}
}
