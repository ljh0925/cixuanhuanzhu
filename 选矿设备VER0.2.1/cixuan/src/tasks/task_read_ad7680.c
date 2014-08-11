/******************************************************************************
Copyright 2005 �������ķ�������޹�˾
All rights riserved.

�ļ���	��task_read_ad7680.c
ģ�����ƣ���ȡADֵ�������
���ܸ�Ҫ����ȡADֵ�������

ȡ���汾��0.0.1
�޸���	������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include 		"config.h"

extern volatile DEV_STAT DevStat;

//#define AD_DATA_NUM     80  //����ƽ������
//#define CALC_START      20
//#define CALC_NUM        40
//#define AD_DATA_NUM     70  //����ƽ������
//#define CALC_START      17
//#define CALC_NUM        36
#define AD_DATA_NUM     4  //����ƽ������
#define CALC_START      1
#define CALC_NUM        2

//#define AD_DATA_NUM     48  //����ƽ������
//#define CALC_START      8
//#define CALC_NUM        32
//#define AD_DATA_NUM     16  //����ƽ������
//#define CALC_START      4
//#define CALC_NUM        8

AD_LIST ad_data[AD_DATA_NUM];

PAD_LIST list_min;
PAD_LIST list_max;
PAD_LIST coming_data;

INT16U ad_buf[AD_QUEUE_LEN2];

/******************************************************************************
 �������ƣ�Task_Read_Ad7680
 ������������ȡADֵ����
 ����������
 �������ƣ� ����/����� ����        ����

 ��  ��  ֵ����

 ��      �� ������
 ��      �ڣ�2005-04-11
 �޸���ʷ��
		����        �޸���      �޸�����
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


		OSTimeDlyHMSM(0, 0, 0, 10);            //10msһ��
//      OSTimeDlyHMSM(0, 0, 0, 30);            //30msһ��

		for(j=0; j<AD_QUEUE_LEN2; j++)
		{
			ad_buf[j] = GetAd7680();
		}
		ad = average_ad2(ad_buf);        //ADƽ��ֵ���������

//      OS_ENTER_CRITICAL();
//      DevStat.MOD_REG.reg[0x0002] = ad;       //AD����ֵ
//      OS_EXIT_CRITICAL();
		OS_ENTER_CRITICAL();
		DevStat.ad_avg[DevStat.ad_avg_ptr] = ad;
		DevStat.ad_avg_ptr++;
		if ( DevStat.ad_avg_ptr >= AD_QUEUE_LEN )
			DevStat.ad_avg_ptr = 0;
		DevStat.MOD_REG.reg[0x0002] 
		= average_ad((INT16U *)&DevStat.ad_avg[0]);		//ADƽ��ֵ����ADֵ�Ĵ�����
		OS_EXIT_CRITICAL();

//      OS_ENTER_CRITICAL();
//      DevStat.ad_avg2[DevStat.ad_avg_ptr2] = ad;
//      DevStat.ad_avg_ptr2++;
//      if ( DevStat.ad_avg_ptr2 >= AD_QUEUE_LEN2 )
//          DevStat.ad_avg_ptr2 = 0;
//      ad = average_ad2((INT16U *)&DevStat.ad_avg2[0]);        //ADƽ��ֵ���������
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

		h_real = AVERAGE(h_real);				//���ƽ��ֵ

		OS_ENTER_CRITICAL();
		if (h_real >= 0.0005)
			h_real += 0.0005;		//��������
		else if (h_real <= -0.0005)
			h_real -= 0.0005;

		if ( h_real > 99.999 )
			h_real = 99.999;		//�����ʾ��ΧΪ99.999
		if ( h_real < -99.999 )
			h_real = -99.999;		//�����ʾ��ΧΪ99.999

		OS_ENTER_CRITICAL();
		DevStat.current_h = h_real;
		OS_EXIT_CRITICAL();
		
		temp_int32s = _dfix(h_real * 1000.0);
		if ( temp_int32s < 0 )
		{
			temp_int32u = (INT32U)abs(temp_int32s);
			DevStat.MOD_REG.reg[0x0003] = (INT16U)(temp_int32u / 1000);
			DevStat.MOD_REG.reg[0x0003] |= 0x8000;		//���λ��1
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
 �������ƣ�Init_AD_List
 ������������ʼ������
 ����������
 �������ƣ� ����/����� ����        ����

 ��  ��  ֵ����

 ��      �� ������
 ��      �ڣ�2005-04-11
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
void Init_AD_List(void)
{
	INT8U i;

	OS_ENTER_CRITICAL();

	//��ʼ�������γ���������
	//һ����Ϊ�˲��������ݣ�һ��Ϊ������
	ad_data[0].prev = &ad_data[AD_DATA_NUM-1];				//ѭ������
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
 �������ƣ�Add_To_List
 �����������������ݲ��뵽����
 ����������
 �������ƣ� ����/����� ����        ����
 val		����		AD_TYPE		������

 ��  ��  ֵ����

 ��      �� ������
 ��      �ڣ�2005-04-11
 �޸���ʷ��
		����        �޸���      �޸�����
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
	if ( val <= list_min->AD_Value )	//С����Сֵ
	{
		coming_data->_small = NULL;
		coming_data->big = list_min;
		list_min->_small = coming_data;
		list_min = coming_data;
	}
	else if ( val >= list_max->AD_Value ) //�������ֵ
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
 �������ƣ�Calc_Average
 ��������������ƽ��ֵ
 ����������
 �������ƣ� ����/����� ����        ����

 ��  ��  ֵ��ƽ��ֵ

 ��      �� ������
 ��      �ڣ�2005-04-11
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
AD_TYPE Calc_Average(void)
{
	INT8U i;
	PAD_LIST list_tmp = list_min;
	AD_TYPE fval = 0.0;

//  OS_ENTER_CRITICAL();

	for ( i=0; i<CALC_START; i++ ) //ȥ����Сֵ
	{
		list_tmp = list_tmp->big;
	}

	for ( i=0; i<CALC_NUM; i++ )   //�����м��ƽ��ֵ
	{
//      ToggleWD();
		fval += list_tmp->AD_Value;
		list_tmp = list_tmp->big;
	}

//  OS_EXIT_CRITICAL();

	return(fval / (AD_TYPE)CALC_NUM);
}

/******************************************************************************
 �������ƣ�average
 �������������ȶ��е�ƽ��ֵ
 ����������
 �������ƣ�	����/�����	����		����
 adc_data	����		AD_TYPE		���
				
 ��  ��  ֵ����ȵ�ƽ����
				   
 ��      ��	������
 ��      �ڣ�2005-11-02
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
AD_TYPE average(AD_TYPE adc_data)
{
//#define GAP 0.2                                         //���ֵ�������������Ϊ��Ծ
#define GAP 0.15											//���ֵ�������������Ϊ��Ծ

	INT8U i = 0;
	AD_TYPE result = 0.0;
	
	//ToggleWD();

	OS_ENTER_CRITICAL();

	if ( abs(adc_data - coming_data->prev->AD_Value) >= GAP )
	{
		if (adc_data > coming_data->prev->AD_Value)	//����
		{
			if ( coming_data->prev->AD_Value - coming_data->prev->prev->AD_Value >= GAP )   //��������ֵ��������ر���ж�Ϊ��Ծ
			{
				for ( i=0; i<AD_DATA_NUM; i++ )
				{
					Add_To_List(adc_data);
				}
				OS_EXIT_CRITICAL();
				return	adc_data;
			}
		}
		else	//�ݼ�
		{
			
			if ( coming_data->prev->prev->AD_Value - coming_data->prev->AD_Value >= GAP )   //��������ֵ��������ر���ж�Ϊ��Ծ
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
 �������ƣ�chabiao
 ������������������adֵ�����������
 �������ƣ�	����/�����	����		����
 v			����		INT16U		ad��������ֵ
 
 ��  ��  ֵ����ȣ���λmm
 
 ��      ��	������
 ��      �ڣ�2007-01-31
 �޸���ʷ��
		����		�޸���		�޸�����
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
		return 0.0;		//�����ֵ����
	}

	v1 = biao[(i - 1) * 2];
	h1 = biao[(i - 1) * 2 + 1];
	v2 = biao[i * 2];
	h2 = biao[i * 2 + 1];

	if ( !((AD_TYPE)v2 - (AD_TYPE)v1 > 0.0001 || (AD_TYPE)v2 - (AD_TYPE)v1 < -0.0001) )
	{
		OS_EXIT_CRITICAL();
		return 0.0;			//��ֹ����Ϊ0
	}

	a = ((AD_TYPE)h2 - (AD_TYPE)h1) / ((AD_TYPE)v2 - (AD_TYPE)v1);
	b = (AD_TYPE)h1 - a * (AD_TYPE)v1;

	result = (a * (AD_TYPE)v + b) / 1000.0;

	OS_EXIT_CRITICAL();
	return result;
}

/******************************************************************************
 �������ƣ�average_ad
 ������������AD���е�ƽ��ֵ
 ����������
 �������ƣ� ����/����� ����        ����
 adc_data   ����    	INT16U*        ADֵ����

 ��  ��  ֵ��AD��ƽ����

 ��      �� ������
 ��      �ڣ�2005-11-02
 �޸���ʷ��
		����        �޸���      �޸�����
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
 �������ƣ�average_ad2
 ������������AD���е�ƽ��ֵ
 ����������
 �������ƣ� ����/����� ����        ����
 adc_data   ����    	INT16U*        ADֵ����

 ��  ��  ֵ��AD��ƽ����

 ��      �� ������
 ��      �ڣ�2005-11-02
 �޸���ʷ��
		����        �޸���      �޸�����
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

