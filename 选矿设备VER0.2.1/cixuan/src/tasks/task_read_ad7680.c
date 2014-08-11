/******************************************************************************
Copyright 2005 大连金德姆电子有限公司
All rights riserved.

文件名	：task_read_ad7680.c
模块名称：读取AD值任务程序
功能概要：读取AD值任务程序

取代版本：0.0.1
修改人	：许岩
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include 		"config.h"

extern volatile DEV_STAT DevStat;

//#define AD_DATA_NUM     80  //滑动平均个数
//#define CALC_START      20
//#define CALC_NUM        40
//#define AD_DATA_NUM     70  //滑动平均个数
//#define CALC_START      17
//#define CALC_NUM        36
#define AD_DATA_NUM     4  //滑动平均个数
#define CALC_START      1
#define CALC_NUM        2

//#define AD_DATA_NUM     48  //滑动平均个数
//#define CALC_START      8
//#define CALC_NUM        32
//#define AD_DATA_NUM     16  //滑动平均个数
//#define CALC_START      4
//#define CALC_NUM        8

AD_LIST ad_data[AD_DATA_NUM];

PAD_LIST list_min;
PAD_LIST list_max;
PAD_LIST coming_data;

INT16U ad_buf[AD_QUEUE_LEN2];

/******************************************************************************
 函数名称：Task_Read_Ad7680
 功能描述：读取AD值任务
 参数描述：
 参数名称： 输入/输出？ 类型        描述

 返  回  值：无

 作      者 ：许岩
 日      期：2005-04-11
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
void Task_Read_Ad7680(void * data)
{
	INT16U ad = 0;
	INT16U reg= 0;
	AD_TYPE h = 0.0;
	AD_TYPE b = 0.0;
	AD_TYPE k = 0.0;
	AD_TYPE h_real = 0.0;
	INT32S temp_int32s = 0;
	INT32U temp_int32u = 0;
	INT16U j = 0;

#ifdef STK_DEBUG
		OS_STK_DATA stk_data;
		INT8U err = 0;
		INT8U i = 0;
#endif

	//ToggleWD();

	data = data;

	Init_AD_List();

	for ( ;; )
	{
//      ToggleWD();


		OSTimeDlyHMSM(0, 0, 0, 10);            //10ms一次
//      OSTimeDlyHMSM(0, 0, 0, 30);            //30ms一次

		for(j=0; j<AD_QUEUE_LEN2; j++)
		{
			ad_buf[j] = GetAd7680();
		}
		ad = average_ad2(ad_buf);        //AD平均值，给查表用

//      OS_ENTER_CRITICAL();
//      DevStat.MOD_REG.reg[0x0002] = ad;       //AD采样值
//      OS_EXIT_CRITICAL();
		OS_ENTER_CRITICAL();
		DevStat.ad_avg[DevStat.ad_avg_ptr] = ad;
		DevStat.ad_avg_ptr++;
		if ( DevStat.ad_avg_ptr >= AD_QUEUE_LEN )
			DevStat.ad_avg_ptr = 0;
		DevStat.MOD_REG.reg[0x0002] 
		= average_ad((INT16U *)&DevStat.ad_avg[0]);		//AD平均值，给AD值寄存器用
		OS_EXIT_CRITICAL();

//      OS_ENTER_CRITICAL();
//      DevStat.ad_avg2[DevStat.ad_avg_ptr2] = ad;
//      DevStat.ad_avg_ptr2++;
//      if ( DevStat.ad_avg_ptr2 >= AD_QUEUE_LEN2 )
//          DevStat.ad_avg_ptr2 = 0;
//      ad = average_ad2((INT16U *)&DevStat.ad_avg2[0]);        //AD平均值，给查表用
//      OS_EXIT_CRITICAL();

		h = chabiao(ad);

		OS_ENTER_CRITICAL();
		reg = DevStat.MOD_REG.reg[0x0008];
		OS_EXIT_CRITICAL();
		if ( (reg & 0x8000) == 0 )
		{
			b = (AD_TYPE)reg / 1000.0;
		}
		else
		{
			b = (0.0 - (AD_TYPE)(reg & (~0x8000))) / 1000.0;
		}
		OS_ENTER_CRITICAL();
		reg = DevStat.MOD_REG.reg[0x0009];
		OS_EXIT_CRITICAL();
		k = (AD_TYPE)reg / 1000.0;

		h_real = (h + b) * k;

		h_real = AVERAGE(h_real);				//算出平均值

		OS_ENTER_CRITICAL();
		if (h_real >= 0.0005)
			h_real += 0.0005;		//四舍五入
		else if (h_real <= -0.0005)
			h_real -= 0.0005;

		if ( h_real > 99.999 )
			h_real = 99.999;		//最大显示范围为99.999
		if ( h_real < -99.999 )
			h_real = -99.999;		//最大显示范围为99.999

		OS_ENTER_CRITICAL();
		DevStat.current_h = h_real;
		OS_EXIT_CRITICAL();
		
		temp_int32s = _dfix(h_real * 1000.0);
		if ( temp_int32s < 0 )
		{
			temp_int32u = (INT32U)abs(temp_int32s);
			DevStat.MOD_REG.reg[0x0003] = (INT16U)(temp_int32u / 1000);
			DevStat.MOD_REG.reg[0x0003] |= 0x8000;		//最高位置1
			DevStat.MOD_REG.reg[0x0004] = (INT16U)(temp_int32u % 1000);
		}
		else
		{
			temp_int32u = (INT32U)temp_int32s;
			DevStat.MOD_REG.reg[0x0003] = (INT16U)(temp_int32u / 1000);
			DevStat.MOD_REG.reg[0x0004] = (INT16U)(temp_int32u % 1000);
		}
		OS_EXIT_CRITICAL();
	}
}

/******************************************************************************
 函数名称：Init_AD_List
 功能描述：初始化链表
 参数描述：
 参数名称： 输入/输出？ 类型        描述

 返  回  值：无

 作      者 ：许岩
 日      期：2005-04-11
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
void Init_AD_List(void)
{
	INT8U i;

	OS_ENTER_CRITICAL();

	//初始化链表，形成两个链，
	//一个是为了插入新数据，一个为了排序
	ad_data[0].prev = &ad_data[AD_DATA_NUM-1];				//循环链表
	ad_data[0].next = &ad_data[1];
	ad_data[0].big = &ad_data[1];
	ad_data[0]._small = NULL;
	ad_data[0].AD_Value = 0;

	for ( i=1; i<AD_DATA_NUM-1; i++ )
	{
		ad_data[i].prev = &ad_data[i-1];
		ad_data[i].next = &ad_data[i+1];
		ad_data[i].big = &ad_data[i+1];
		ad_data[i]._small = &ad_data[i-1];
		ad_data[i].AD_Value = 0;
	}

	ad_data[AD_DATA_NUM-1].prev = &ad_data[AD_DATA_NUM-2];
	ad_data[AD_DATA_NUM-1].next = &ad_data[0];
	ad_data[AD_DATA_NUM-1].big = NULL;
	ad_data[AD_DATA_NUM-1]._small = &ad_data[AD_DATA_NUM-2];
	ad_data[AD_DATA_NUM-1].AD_Value = 0;

	list_min = &ad_data[0];
	list_max = &ad_data[AD_DATA_NUM-1];
	coming_data = &ad_data[0];

	OS_EXIT_CRITICAL();
}

/******************************************************************************
 函数名称：Add_To_List
 功能描述：将新数据插入到链表
 参数描述：
 参数名称： 输入/输出？ 类型        描述
 val		输入		AD_TYPE		新数据

 返  回  值：无

 作      者 ：许岩
 日      期：2005-04-11
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
void Add_To_List(AD_TYPE val)
{
//  OS_ENTER_CRITICAL();

	coming_data->AD_Value = val;

	//remove from the list
	if ( coming_data->_small == NULL )	//min value
	{
		coming_data->big->_small = NULL;
		list_min = coming_data->big;
	}
	else if ( coming_data->big == NULL )	//max value
	{
		coming_data->_small->big = NULL;
		list_max = coming_data->_small;
	}
	else
	{
		coming_data->big->_small = coming_data->_small;
		coming_data->_small->big = coming_data->big;
	}

	//insert to list
	if ( val <= list_min->AD_Value )	//小于最小值
	{
		coming_data->_small = NULL;
		coming_data->big = list_min;
		list_min->_small = coming_data;
		list_min = coming_data;
	}
	else if ( val >= list_max->AD_Value ) //大于最大值
	{
		coming_data->_small = list_max;
		coming_data->big = NULL;
		list_max->big = coming_data;
		list_max = coming_data;
	}
	else
	{
		PAD_LIST list_tmp;
		for ( list_tmp = list_min->big; list_tmp != NULL; list_tmp = list_tmp->big )
		{
			if ( val <= list_tmp->AD_Value )   //insert before
			{
				coming_data->_small = list_tmp->_small;
				coming_data->big = list_tmp;
				list_tmp->_small->big = coming_data;
				list_tmp->_small = coming_data;
				break;
			}
		}
	}
	coming_data = coming_data->next;

//  OS_EXIT_CRITICAL();
}

/******************************************************************************
 函数名称：Calc_Average
 功能描述：计算平均值
 参数描述：
 参数名称： 输入/输出？ 类型        描述

 返  回  值：平均值

 作      者 ：许岩
 日      期：2005-04-11
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
AD_TYPE Calc_Average(void)
{
	INT8U i;
	PAD_LIST list_tmp = list_min;
	AD_TYPE fval = 0.0;

//  OS_ENTER_CRITICAL();

	for ( i=0; i<CALC_START; i++ ) //去掉最小值
	{
		list_tmp = list_tmp->big;
	}

	for ( i=0; i<CALC_NUM; i++ )   //计算中间的平均值
	{
//      ToggleWD();
		fval += list_tmp->AD_Value;
		list_tmp = list_tmp->big;
	}

//  OS_EXIT_CRITICAL();

	return(fval / (AD_TYPE)CALC_NUM);
}

/******************************************************************************
 函数名称：average
 功能描述：算厚度队列的平均值
 参数描述：
 参数名称：	输入/输出？	类型		描述
 adc_data	输入		AD_TYPE		厚度
				
 返  回  值：厚度的平均数
				   
 作      者	：许岩
 日      期：2005-11-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
AD_TYPE average(AD_TYPE adc_data)
{
//#define GAP 0.2                                         //厚度值，如果超过此则为阶跃
#define GAP 0.15											//厚度值，如果超过此则为阶跃

	INT8U i = 0;
	AD_TYPE result = 0.0;
	
	//ToggleWD();

	OS_ENTER_CRITICAL();

	if ( abs(adc_data - coming_data->prev->AD_Value) >= GAP )
	{
		if (adc_data > coming_data->prev->AD_Value)	//递增
		{
			if ( coming_data->prev->AD_Value - coming_data->prev->prev->AD_Value >= GAP )   //连续两个值都是相差特别大，判断为阶跃
			{
				for ( i=0; i<AD_DATA_NUM; i++ )
				{
					Add_To_List(adc_data);
				}
				OS_EXIT_CRITICAL();
				return	adc_data;
			}
		}
		else	//递减
		{
			
			if ( coming_data->prev->prev->AD_Value - coming_data->prev->AD_Value >= GAP )   //连续两个值都是相差特别大，判断为阶跃
			{
				for ( i=0; i<AD_DATA_NUM; i++ )
				{
					Add_To_List(adc_data);
				}
				OS_EXIT_CRITICAL();
				return	adc_data;
			}
		}
	}

	Add_To_List(adc_data);
	result = Calc_Average();
	OS_EXIT_CRITICAL();

	return result;
}

/******************************************************************************
 函数名称：chabiao
 功能描述：根据输入ad值，查表算出厚度
 参数名称：	输入/输出？	类型		描述
 v			输入		INT16U		ad读出的数值
 
 返  回  值：厚度，单位mm
 
 作      者	：许岩
 日      期：2007-01-31
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
AD_TYPE chabiao(INT16U v)
{
	INT16U i = 0;

	INT16U *biao = (INT16U *)&DevStat.MOD_REG.reg[0x0010];
	INT16U v1 = 0;
	INT16U v2 = 0;
	INT16U h1 = 0;
	INT16U h2 = 0;
	AD_TYPE a = 0.0;
	AD_TYPE b = 0.0;
	AD_TYPE result = 0.0;

	//ToggleWD();

	OS_ENTER_CRITICAL();

//  for ( i=0; i<128 / 2; i++ )
	for ( i=0; i<HOU_DU_BIAO_LEN / 2; i++ )
	{
//      ToggleWD();
//      if ( v > biao[i * 2] )
		if ( v >= biao[i * 2] )
		{
			break;
		}
	}
	if ( v == biao[0] )
		i = 1;

	if ( i == 0 )
	{
		OS_EXIT_CRITICAL();
		return 0.0;		//比最大值还大
	}

	v1 = biao[(i - 1) * 2];
	h1 = biao[(i - 1) * 2 + 1];
	v2 = biao[i * 2];
	h2 = biao[i * 2 + 1];

	if ( !((AD_TYPE)v2 - (AD_TYPE)v1 > 0.0001 || (AD_TYPE)v2 - (AD_TYPE)v1 < -0.0001) )
	{
		OS_EXIT_CRITICAL();
		return 0.0;			//防止除数为0
	}

	a = ((AD_TYPE)h2 - (AD_TYPE)h1) / ((AD_TYPE)v2 - (AD_TYPE)v1);
	b = (AD_TYPE)h1 - a * (AD_TYPE)v1;

	result = (a * (AD_TYPE)v + b) / 1000.0;

	OS_EXIT_CRITICAL();
	return result;
}

/******************************************************************************
 函数名称：average_ad
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
INT16U average_ad(INT16U* adc_data)
{
	INT32U sum = 0;
	INT8U i = 0;

	ToggleWD();

	for ( i=0; i<AD_QUEUE_LEN; i++ )
	{
//      ToggleWD();
		sum += (INT32U)adc_data[i];
	}

	return(INT16U)(sum / AD_QUEUE_LEN);
}

/******************************************************************************
 函数名称：average_ad2
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
INT16U average_ad2(INT16U* adc_data)
{
	INT32U sum = 0;
	INT16U i = 0;

	ToggleWD();

	for ( i=0; i<AD_QUEUE_LEN2; i++ )
	{
//      ToggleWD();
		sum += (INT32U)adc_data[i];
	}

	return(INT16U)(sum / AD_QUEUE_LEN2);
}

