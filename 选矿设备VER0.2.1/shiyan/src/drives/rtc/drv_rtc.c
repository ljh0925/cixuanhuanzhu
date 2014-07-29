/******************************************************************************
Copyright 2005 �����״�ͨ�������޹�˾
All rights riserved.

�ļ���	��drv_RTC.c
ģ�����ƣ�ʱ��оƬ��������
���ܸ�Ҫ��ʱ��оƬ��������

ȡ���汾��0.0.1
�޸���	������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include "config.h"

#define Rtc_USE_SEM


#ifdef Rtc_USE_SEM
extern OS_EVENT		*Rtc_Sem;
#endif


//OS_EVENT *RTCSem;           //����ʱ��оƬ�������ź���


/*****************************************************************
����ԭ�ͣ�WriteRX8025
������������ĳ����ֵд��rx��RX8025�ļĴ�����
����������
�������ƣ�	����/�����	����		����
	which_reg	����				INT8U		RX8025 �Ĵ�����ַ
	value		����				INT8U		Ҫд���ֵ

��  ��  ֵ��ok(0)-�ɹ� 
				notok(0xFF)��ʧ��
��      ��	������
��      �ڣ�2004-12-1
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U WriteRX8025(INT8U which_reg, INT8U value)
{
	INT8U i = 0;
	INT8U j = 0;
//  INT8U h = 0;
#ifdef Rtc_USE_SEM
	INT8U err = 0;
#endif

	ToggleWD();

#ifdef Rtc_USE_SEM
	OSSemPend(Rtc_Sem, 0, &err);
#endif
	OS_ENTER_CRITICAL();

	i = Rtc_Start();					//���Rtc_Start����������10��
	if ( i != ok )
	{
		j = 20;
		while ( j )
		{
			Rtc_Stop();
			i = Rtc_Start();
			if ( i != ok )
			{
				j--;
			}
			else
			{
				break;
			}
			if ( j == 0 )
			{
				Rtc_Stop();
				OS_EXIT_CRITICAL();
#ifdef Rtc_USE_SEM
				OSSemPost(Rtc_Sem);
#endif
				return notok;
			}
		}
	}

	i = Rtc_Send(RX8025_WRITE);	//����Ӳ����ַ�Լ�д�ź�
	if (i != ok)
	{
		Rtc_Stop();
		OS_EXIT_CRITICAL();
#ifdef Rtc_USE_SEM
		OSSemPost(Rtc_Sem);
#endif
		return notok;
	}

//  i = Rtc_Send((which_reg << 4) & 0xF0);  //���Ͳ����Ĵ�����ַ("0")
	i = Rtc_Send(which_reg);	//���Ͳ����Ĵ�����ַ("0")
	if (i != ok)
	{
		Rtc_Stop();
		OS_EXIT_CRITICAL();
#ifdef Rtc_USE_SEM
		OSSemPost(Rtc_Sem);
#endif
		return notok;
	}

	i = Rtc_Send(value);	//����Ӳ����ַ�Լ�д�ź�
	if (i != ok)
	{
		Rtc_Stop();
		OS_EXIT_CRITICAL();
#ifdef Rtc_USE_SEM
		OSSemPost(Rtc_Sem);
#endif
		return notok;
	}

	Rtc_Stop();

	OS_EXIT_CRITICAL();
#ifdef Rtc_USE_SEM
		OSSemPost(Rtc_Sem);
#endif
	return ok;
}




/////////////////////////////////////////////////////////
// ����Ϊ����Ҫ��д����
/////////////////////////////////////////////////////////


/*****************************************************************
����ԭ�ͣ�Get_Time
������������ȡʱ�䣬��ȡ���Σ�Ϊ�˷�ֹż��һ�β��ɹ�
����������
�������ƣ�	����/�����	����		����
*time_data	���   _TIME	7���ֽڳ������ݴ�ͷ��β˳�������� �� �� �� ʱ �� ��
��  ��  ֵ��ok(0)-�ɹ� 
				notok(0xFF)��ʧ��
��      ��	������
��      �ڣ�2005-04-12
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U Get_Time(_TIME *time_data)
{
	INT8U i = 0;
//  INT8U err;

	ToggleWD();

//  OSSemPend(RTCSem, 0, &err);

	i = Get_Time_Lowlvl(time_data);
	if ( i != ok )
	{
		i = Get_Time_Lowlvl(time_data);
		if ( i != ok )
		{
//          OSSemPost(RTCSem);
			return notok;
		}
	}

//  OSSemPost(RTCSem);

	return ok;
}

/*****************************************************************
����ԭ�ͣ�Get_Time_Lowlvl
������������ȡrx��RX8025	�ڵ�ʱ��
����������
�������ƣ�	����/�����	����		����
*time_data	���   _TIME	7���ֽڳ������ݴ�β��ͷ˳�������� �� �� �� ʱ �� ��
��  ��  ֵ��ok(0)-�ɹ� 
				notok(0xFF)��ʧ��
��      ��	������
��      �ڣ�2005-04-12
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U Get_Time_Lowlvl(_TIME *time_data)
{
	INT8U i = 0;
	INT8U start_reg = 0;		//�ӵ�0���Ĵ�����ʼ��ȡ
	INT8U buf[9];

	ToggleWD();

	i = Rtc_SendAndReceive(RX8025, start_reg, buf, 7);
	if (i != ok)
	{
		return notok;
	}

	time_data->second = buf[0];				//��
	time_data->minute = buf[1];				//��
	time_data->hour = buf[2];				//Сʱ
	time_data->day = buf[4];				//��
	time_data->month = buf[5];				//��
	time_data->year = buf[6];				//��
	time_data->century = 0x20;				//���Ͷ���Ϊ0x20

	return ok;
}

/*****************************************************************
����ԭ�ͣ�Modify_Time
�����������޸�ʱ�䣬�޸����Σ�Ϊ�˷�ֹż��һ�β��ɹ�
����������
�������ƣ�	����/�����	����		����
*time_data	����	_TIME	7���ֽڳ������ݴ�ͷ��β˳�������� �� �� �� ʱ �� ��
��  ��  ֵ��ok(0)-�ɹ� 
				notok(0xFF)��ʧ��
��      ��	������
��      �ڣ�2005-04-12
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U Modify_Time(_TIME const * const time_data)
{
	INT8U i;
//  INT8U err;

	ToggleWD();

//  OSSemPend(RTCSem, 0, &err);

	i = Modify_Time_Lowlvl(time_data);
	if ( i != ok )
	{
		i = Modify_Time_Lowlvl(time_data);
		if ( i != ok )
		{
//          OSSemPost(RTCSem);
			return notok;
		}
	}

//  OSSemPost(RTCSem);

	return ok;
}

/*****************************************************************
����ԭ�ͣ�Modify_Time_Lowlvl
�����������޸�rx-RX8025 �ڵ�ʱ��
����������
�������ƣ�	����/�����	����		����
*time_data	����	_TIME	7���ֽڳ������ݴ�β��ͷ˳����
											���� �� �� �� ʱ �� ��
��  ��  ֵ��ok(0)-�ɹ� 
				notok(0xFF)��ʧ��
��      ��	������
��      �ڣ�2003-05-12
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U Modify_Time_Lowlvl(_TIME const * const time_data)			//ָ��ָ������ǲ����޸ĵģ�ͬʱָ��Ҳ�ǲ����޸ĵ�
{
	INT8U i;

	ToggleWD();

	i = WriteRX8025(SEC_REG, time_data->second);				//��
	if ( i != ok )
		return notok;

	i = WriteRX8025(MIN_REG, time_data->minute);				//��
	if ( i != ok )
		return notok;

	i = WriteRX8025(HOUR_REG, time_data->hour);				//Сʱ
	if ( i != ok )
		return notok;

	i = WriteRX8025(DAY_REG, time_data->day);					//��
	if ( i != ok )
		return notok;

	i = WriteRX8025(MONTH_REG, time_data->month);				//��
	if ( i != ok )
		return notok;

	i = WriteRX8025(YEAR_REG, time_data->year);				//��
	if ( i != ok )
		return notok;

	return ok;
}

/*****************************************************************
����ԭ�ͣ�RX8025Init
������������rx��RX8025 ���г�ʼ��������Ϊ24 Сʱģʽ,��ʼ��ʱ����
����������
�������ƣ�	����/�����	����		����

��  ��  ֵ��ok(0)-�ɹ� 
				notok(0xFF)��ʧ��
��      ��	������
��      �ڣ�2005-04-12
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U RX8025Init(void)
{
	INT8U i;
	INT8U m;

	ToggleWD();

	for ( i=0; i<4; i++ )
	{
		m = WriteRX8025(CTRL1_REG, 0x20);		//24Сʱģʽ
		if ( m != ok )
			continue;

		m = WriteRX8025(CTRL2_REG, 0x20);		//2.1v��ѹ���
		if ( m != ok )
			continue;

		break;
	}

	if ( i >= 4 )
		return notok;
	else
		return ok;
}

/*****************************************************************
����ԭ�ͣ�CheckTimeFormat
�������������ʱ���ʽ�Ƿ�Ϸ�
����������
�������ƣ�	����/�����	����		����
time		����  			const _TIME *	7���ֽڳ������ݴ�ͷ��β˳�������� �� �� �� ʱ �� ��

��  ��  ֵ��ok(0)-�ɹ� 
				notok(0xFF)��ʧ��
��      ��	������
��      �ڣ�2003-05-12
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U CheckTimeFormat(const _TIME *time)
{   
	INT8U temp;

	ToggleWD();

	temp = time->month;
	if ( temp >= 0x0a )		  //�·�Ӧ��1��12֮��
	{
		if ( temp >= 0x10 && temp <= 0x12 )
			;
		else
			return notok;
	}

	temp = time->day;
	if ( temp > 0x31 )					 //��Ӧ��1-31֮��
		return notok;
	temp = temp&0x0F;
	if ( temp >0x09 )
		return notok;

	temp = time->hour;
	if ( temp > 0x23 )									 //СʱӦ��0-23֮��
		return notok;
	temp = temp & 0x0F;
	if ( temp >0x09 )
		return notok;

	temp = time->minute;
	if ( temp > 0x59 )									 //����Ӧ��0-59֮��
		return notok;
	temp = temp & 0x0F;
	if ( temp > 0x09 )
		return notok;

	temp = time->second;
	if ( temp > 0x59 )									  //��Ӧ��0-59֮��
		return notok;
	temp = temp & 0x0F;
	if ( temp > 0x09 )
		return notok;

	return ok;
}


