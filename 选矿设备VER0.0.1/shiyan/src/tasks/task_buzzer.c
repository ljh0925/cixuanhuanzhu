/******************************************************************************
Copyright 2005 大连金德姆电子有限公司
All rights riserved.

文件名	：task_buzzer.c
模块名称：蜂鸣器任务程序
功能概要：蜂鸣器任务程序

取代版本：0.0.1
修改人	：许岩
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include 		"config.h"

//extern  OS_EVENT        *Beep_Mbox;
//extern  OS_EVENT        *Ack_Beep_Mbox;



///******************************************************************************
// 函数名称：Task_Beep
// 功能描述：蜂鸣器鸣叫任务,鸣叫时间通过邮箱来传递,要求最好>20ms
// 参数描述：
// 参数名称： 输入/输出？ 类型        描述
//
// 返  回  值：无
//
// 作      者 ：许岩
// 日      期：2005-04-11
// 修改历史：
//        日期        修改人      修改描述
//        ------      ---------   -------------
//******************************************************************************/
//void Task_Beep(void * data)
//{
//    INT16U ms;          //鸣叫时间
//    INT16U *rxmsg;
//    INT8U err;
//
//    data = data;
//
//    for(;;)
//    {
//        ToggleWD();
//
//  //      rxmsg = (INT16U *)OSMboxPend((OS_EVENT *)Beep_Mbox, 0, &err);
//    //    ms = *rxmsg;
//
//        SetBell(OFF);
//        OSTimeDlyHMSM(0, 1, 0, 0);
//        SetBell(OFF);
//    }
//}

