/******************************************************************************
Copyright 2005 大连金德姆电子有限公司
All rights riserved.

文件名	：task_display_time.c
模块名称：显示时间任务程序
功能概要：显示时间任务程序

取代版本：0.0.1
修改人	：许岩
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include 		"config.h"
volatile INT8U DisplayTimeEN;
// extern OS_EVENT        *Beep_Mbox;
// extern OS_EVENT        *Ack_Beep_Mbox;
//volatile INT8U DisplayTimeEN;                                       //是否允许刷新时间，=FALSE不允许，=TRUE或其他 允许
//extern OS_EVENT        *Dis_Time_Dly_Mbox;



///******************************************************************************
// 函数名称：Task_Display_Time
// 功能描述：显示时间鸣叫任务
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
//void Task_Display_Time(void * data)
//{
//    INT8U sec;          //等待时间
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
//        rxmsg = (INT8U *)OSMboxAccept(Dis_Time_Dly_Mbox);       //查询邮箱，看是否有要求刷新时间任务被挂起
//        if (rxmsg != NULL)
//        {
//            sec = *rxmsg;
//            if (sec > 59)
//                sec = 59;
//            OSTimeDlyHMSM(0, 0, sec, 0);                //刷新时间任务被挂起相应时间
//        }
//        else
//        {
//            OS_ENTER_CRITICAL();
//            i = DisplayTimeEN;
//            OS_EXIT_CRITICAL();
//
//            if (i == FALSE)
//            {   //不允许刷新时间
//            }
//            else
//            {   //允许刷新时间
//    //             rxmsg = (INT8U *)OSMboxAccept(Dis_Time_Dly_Mbox);
//    //             if (rxmsg != NULL)
//    //             {
//    //                 sec = *rxmsg;
//    //                 if (sec > 59)
//    //                     sec = 59;
//    //                 OSTimeDlyHMSM(0, 0, sec, 0);                //刷新时间任务被挂起相应时间
//    //             }
//                DisplayCurTime();
//            }
//
//            OSTimeDlyHMSM(0, 0, 1, 0);                          //每1秒刷新一次时间
//        }
//
//    }
//}

