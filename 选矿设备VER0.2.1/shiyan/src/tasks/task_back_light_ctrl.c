/******************************************************************************
Copyright 2005 �������ķ�������޹�˾
All rights riserved.

�ļ���	��task_back_light_ctrl.c
ģ�����ƣ���������������
���ܸ�Ҫ����������������

ȡ���汾��0.0.1
�޸���	������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include 		"config.h"


extern OS_EVENT        *Back_Light_Ctrl_Mbox;
// extern OS_EVENT        *Dis_Time_Dly_Mbox;

// extern volatile INT8U DisplayTimeEN;                                        //�Ƿ�����ˢ��ʱ�䣬=FALSE������=TRUE������ ����

extern volatile DEV_STAT DevStat;

/******************************************************************************
 �������ƣ�Task_Back_Light_Ctrl
 ���������������������
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ������
 �� 	 �ڣ�2005-04-11
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void Task_Back_Light_Ctrl(void * data)
{
	INT32S s;			//���Ʋ�ˢ��ʱ���ˢ��ʹ�ܵ�ʱ��
	INT32S *rxmsg;
//     INT8U err;
//  BUS_TIME time;
//  INT8U i;
//  INT16S cur_time;
//  INT16S off_time;
//  INT16S on_time;

	//ToggleWD();

	data = data;

	s = 0;
	for ( ;; )
	{
		//ToggleWD();

		OSTimeDlyHMSM(0, 0, 1, 0);

		rxmsg = (INT32S *)OSMboxAccept(Back_Light_Ctrl_Mbox);		//��ѯ���䣬���Ƿ����µ��������
		if ( rxmsg != NULL )
		{
			s = *rxmsg;
		}

		s--;
		if (s <= 0)
		{	//�ùر�����
			s = 0;
//          if (DevStat.back_light_mode == ALWAYS_ON )
//          {
//              SetBacklightON();
//          }
//          else if (DevStat.back_light_mode == PERIOD_OF_TIME)      //��ʱ�ο��Ʊ���
//          {
//              i = Get_Time(&time);
//              if (i != ok)
//                  continue;
//              cur_time = (INT16S)( (time.hour >> 4) * 10 + (time.hour & 0x0F) ) * 60 + (INT16S)(time.minute >> 4) * 10 + (INT16S)(time.minute & 0x0F);
//              off_time = (INT16S)( (DevStat.back_light_off_time.hour >> 4) * 10 + (DevStat.back_light_off_time.hour & 0x0F) ) * 60
//                              + (INT16S)(DevStat.back_light_off_time.minute >> 4) *10 + (INT16S)(DevStat.back_light_off_time.minute & 0x0F);
//              on_time = (INT16S)( (DevStat.back_light_on_time.hour >> 4) * 10 + (DevStat.back_light_on_time.hour & 0x0F) ) * 60
//                              + (INT16S)(DevStat.back_light_on_time.minute >> 4) * 10 + (INT16S)(DevStat.back_light_on_time.minute & 0x0F);
//
//              if ( (cur_time < off_time) || (cur_time >= on_time) )
//              {
//                  SetBacklightON();
//              }
//              else
//              {
//                  SetBacklightOFF();
//              }
//          }
//          else
//          {
				SetBacklightOFF();           //����������ֱ����
//          }
		}
		else
		{	//���ֱ��⿪
//          if (DevStat.back_light_mode == ALWAYS_OFF )
//          {
//              SetBacklightOFF();
//          }
//          else
//          {
				SetBacklightON();			//����������ֱ��⿪
//          }
		}

	}
}

