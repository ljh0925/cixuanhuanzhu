/******************************************************************************
Copyright 2005 �������ķ�������޹�˾
All rights riserved.

�ļ���	��task_display_time.c
ģ�����ƣ���ʾʱ���������
���ܸ�Ҫ����ʾʱ���������

ȡ���汾��0.0.1
�޸���	������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include 		"config.h"
volatile INT8U DisplayTimeEN;
// extern OS_EVENT        *Beep_Mbox;
// extern OS_EVENT        *Ack_Beep_Mbox;
//volatile INT8U DisplayTimeEN;                                       //�Ƿ�����ˢ��ʱ�䣬=FALSE������=TRUE������ ����
//extern OS_EVENT        *Dis_Time_Dly_Mbox;



///******************************************************************************
// �������ƣ�Task_Display_Time
// ������������ʾʱ����������
// ����������
// �������ƣ� ����/����� ����        ����
//
// ��  ��  ֵ����
//
// ��      �� ������
// ��      �ڣ�2005-04-11
// �޸���ʷ��
//        ����        �޸���      �޸�����
//        ------      ---------   -------------
//******************************************************************************/
//void Task_Display_Time(void * data)
//{
//    INT8U sec;          //�ȴ�ʱ��
//    INT8U *rxmsg;
////     INT8U err;
//    INT8U i;
//
//    data = data;
//
//    for(;;)
//    {
//        ToggleWD();
//
//        rxmsg = (INT8U *)OSMboxAccept(Dis_Time_Dly_Mbox);       //��ѯ���䣬���Ƿ���Ҫ��ˢ��ʱ�����񱻹���
//        if (rxmsg != NULL)
//        {
//            sec = *rxmsg;
//            if (sec > 59)
//                sec = 59;
//            OSTimeDlyHMSM(0, 0, sec, 0);                //ˢ��ʱ�����񱻹�����Ӧʱ��
//        }
//        else
//        {
//            OS_ENTER_CRITICAL();
//            i = DisplayTimeEN;
//            OS_EXIT_CRITICAL();
//
//            if (i == FALSE)
//            {   //������ˢ��ʱ��
//            }
//            else
//            {   //����ˢ��ʱ��
//    //             rxmsg = (INT8U *)OSMboxAccept(Dis_Time_Dly_Mbox);
//    //             if (rxmsg != NULL)
//    //             {
//    //                 sec = *rxmsg;
//    //                 if (sec > 59)
//    //                     sec = 59;
//    //                 OSTimeDlyHMSM(0, 0, sec, 0);                //ˢ��ʱ�����񱻹�����Ӧʱ��
//    //             }
//                DisplayCurTime();
//            }
//
//            OSTimeDlyHMSM(0, 0, 1, 0);                          //ÿ1��ˢ��һ��ʱ��
//        }
//
//    }
//}

