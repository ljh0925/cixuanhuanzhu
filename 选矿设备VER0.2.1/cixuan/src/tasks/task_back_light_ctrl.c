/******************************************************************************
Copyright 2005 大连金德姆电子有限公司
All rights riserved.

文件名	：task_back_light_ctrl.c
模块名称：背光控制任务程序
功能概要：背光控制任务程序

取代版本：0.0.1
修改人	：许岩
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include 		"config.h"


extern OS_EVENT        *Back_Light_Ctrl_Mbox;
// extern OS_EVENT        *Dis_Time_Dly_Mbox;

// extern volatile INT8U DisplayTimeEN;                                        //是否允许刷新时间，=FALSE不允许，=TRUE或其他 允许

extern volatile DEV_STAT DevStat;

/******************************************************************************
 函数名称：Task_Back_Light_Ctrl
 功能描述：背光控制任务
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void Task_Back_Light_Ctrl(void * data)
{
	INT32S s;			//控制不刷新时间和刷卡使能的时间
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

		rxmsg = (INT32S *)OSMboxAccept(Back_Light_Ctrl_Mbox);		//查询邮箱，看是否有新的请求进来
		if ( rxmsg != NULL )
		{
			s = *rxmsg;
		}

		s--;
		if (s <= 0)
		{	//该关背光了
			s = 0;
//          if (DevStat.back_light_mode == ALWAYS_ON )
//          {
//              SetBacklightON();
//          }
//          else if (DevStat.back_light_mode == PERIOD_OF_TIME)      //按时段控制背光
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
				SetBacklightOFF();           //其他情况保持背光关
//          }
		}
		else
		{	//保持背光开
//          if (DevStat.back_light_mode == ALWAYS_OFF )
//          {
//              SetBacklightOFF();
//          }
//          else
//          {
				SetBacklightON();			//其他情况保持背光开
//          }
		}

	}
}

