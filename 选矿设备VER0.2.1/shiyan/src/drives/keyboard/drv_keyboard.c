/******************************************************************************
Copyright 2005 �������ķ�������޹�˾
All rights riserved.

�ļ���	��drv_display.c
ģ�����ƣ���ʾ�ͼ�����������
���ܸ�Ҫ����ʾ�ͼ�����������

ȡ���汾��0.0.1
�޸���	������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include 		"config.h"



const INT32S BACK_LIGHT_DELAY = 10;			//10��
extern OS_EVENT        *Back_Light_Ctrl_Mbox;

/******************************************************************************
�������ƣ�keyboard_init
�������������̿���оƬZLG7290��ʼ��
����������

��  ��  ֵ	����
��      ��	������
��      �ڣ�2005-04-08
�޸���ʷ��
����		�޸���		�޸�����
------		---------	---------
******************************************************************************/
void keyboard_init(void)
{
	ToggleWD();

	PINSEL1 = PINSEL1 & (~BIT18);
	PINSEL1 = PINSEL1 & (~BIT19);       //P0.25 ���ó�GPIO      IRQ

	PINSEL1 = PINSEL1 & (~BIT22);
	PINSEL1 = PINSEL1 & (~BIT23);       //P0.27 ���ó�GPIO      RESET

	IO0DIR &= (~BIT25);             //����
	IO0DIR |= BIT27;                //���

	RESET_7290();
}

/******************************************************************************
�������ƣ�RESET_7290() 
������������I/O��7290��λ
����������

��  ��  ֵ	����
��      ��	������
��      �ڣ�2005-10-10
�޸���ʷ��
����		�޸���		�޸�����
------		---------	---------
******************************************************************************/
void RESET_7290() 
{
	//ToggleWD();

	RESET_7290Low();
//  SleepMs(50);
	(void)OSTimeDlyHMSM(0, 0, 0, 50);

	RESET_7290High();
//  SleepMs(500);
	(void)OSTimeDlyHMSM(0, 0, 0, 500);
}

/******************************************************************************
�������ƣ�get_key
�����������õ�һ������������
������������

��  ��  ֵ	���ü�ֵ
��      ��	����̳
��      �ڣ�2005-04-08
�޸���ʷ��
����		�޸���		�޸�����
------		---------	---------
******************************************************************************/
INT8U get_key(void)
{
	INT8U temp[2];
//     INT8U key_number ,i=0;
//	INT8U  er;

	//ToggleWD();

	for ( ;; )				   //ֱ���õ�һ����Ч����Ϊֹ
	{
		//ToggleWD();

// 		OSSemPend(KeySem, 0, &er);		/***���޵ȴ���������***///��key��task��c����ֲ��

		for(;;)
		{
			if ( (IO0PIN & BIT25) == 0 )
			{
				break;
			}
			OSTimeDlyHMSM(0, 0, 0, 100);
		}

// 		temp[0] = 0;
// 		temp[1] = 1;
// 		I2cRead(ZLG7290ADDR, temp, temp + 1, 1, 2);
		temp[0] = 0xFF;
		(void)I2c_SendAndReceive(ZLG7290ADDR, SubKey, temp, 1);


		//////////////////////////////////////////////////////////////
		//ת����INT8U�ļ�ֵ
		/////////////////////////////////////////////////////////////
		switch ( temp[0] )
		{
			case KEY_0:
				temp[0]='0';
				break;

			case KEY_1:
				temp[0]='1';
				break;

			case KEY_2:
				temp[0]='2';
				break;

			case KEY_3:
				temp[0]='3';
				break;

			case KEY_4:
				temp[0]='4';
				break;

			case KEY_5:
				temp[0]='5';
				break;

			case KEY_6:
				temp[0]='6';
				break;

			case KEY_7:
				temp[0]='7';
				break;

			case KEY_8:
				temp[0]='8';
				break;

			case KEY_9:
				temp[0]='9';
				break;

// 			case KEY_POINT:
// 				temp[0]='.';
// 				break;
//
// 			case KEY_ADD:
// 				temp[0]='+';
// 				break;
//
// 			case KEY_SUB:
// 				temp[0]='-';
// 				break;
//
// 			case KEY_MUL:
// 				temp[0]='*';
// 				break;
//
// 			case KEY_EQUAL:
// 				temp[0]='=';
// 				break;

			case KEY_ENTER:
			case KEY_CANCEL:
// 			case KEY_MODE:
// 			case KEY_FUNC:
				break;

			default:
				temp[0]=NO_KEY;
				break;
		}

		if (temp[0] != NO_KEY)
		{
			PressKeyBkLtCtrl();
			Beep2(150);
		}

		return(temp[0]);
	}
}

/******************************************************************************
�������ƣ�query_key
�����������ò�ѯ��ʽ�õ�һ��������������
������������

��  ��  ֵ	���ü�ֵ
��      ��	������
��      �ڣ�2005-04-08
�޸���ʷ��
����		�޸���		�޸�����
------		---------	---------
******************************************************************************/
INT8U query_key(void)
{
	INT8U temp[2];
//     INT8U key_number ,i=0;
//     INT8U  er;
//	INT16U j;

	//ToggleWD();

// 	j = OSSemAccept(KeySem);		//��ѯһ���Ƿ��а���
// 	if ( j == 0 )
// 	{
// 		return NO_KEY;			//û�а���
// 	}
	if ( (IO0PIN & BIT25) == 0 )
	{
	}
	else
	{
		return NO_KEY;			//û�а���
	}


// 	temp[0] = 0;
// 	temp[1] = 1;
// 	I2cRead(ZLG7290ADDR, temp, temp + 1, 1, 2);
	temp[0] = 0xFF;
	(void)I2c_SendAndReceive(ZLG7290ADDR, SubKey, temp, 1);

	//////////////////////////////////////////////////////////////
	//ת����INT8U�ļ�ֵ
	/////////////////////////////////////////////////////////////
	switch ( temp[0] )
	{
		case KEY_0:
			temp[0]='0';
			break;

		case KEY_1:
			temp[0]='1';
			break;

		case KEY_2:
			temp[0]='2';
			break;

		case KEY_3:
			temp[0]='3';
			break;

		case KEY_4:
			temp[0]='4';
			break;

		case KEY_5:
			temp[0]='5';
			break;

		case KEY_6:
			temp[0]='6';
			break;

		case KEY_7:
			temp[0]='7';
			break;

		case KEY_8:
			temp[0]='8';
			break;

		case KEY_9:
			temp[0]='9';
			break;

// 		case KEY_POINT:
// 			temp[0]='.';
// 			break;
//
// 		case KEY_ADD:
// 			temp[0]='+';
// 			break;
//
// 		case KEY_SUB:
// 			temp[0]='-';
// 			break;
//
// 		case KEY_MUL:
// 			temp[0]='*';
// 			break;
//
// 		case KEY_EQUAL:
// 			temp[0]='=';
// 			break;

		case KEY_ENTER:
		case KEY_CANCEL:
// 		case KEY_MODE:
// 		case KEY_FUNC:
			break;

		default:
			temp[0]=NO_KEY;
			break;
	}
	if (temp[0] != NO_KEY)
	{
		PressKeyBkLtCtrl();
		Beep2(150);
	}

	return(temp[0]);
}


/******************************************************************************
�������ƣ�query_key_nsnd
�����������ò�ѯ��ʽ�õ�һ�������������ȣ���query_keyʡȥ��������
������������

��  ��  ֵ	���ü�ֵ
��      ��	������
��      �ڣ�2005-04-08
�޸���ʷ��
����		�޸���		�޸�����
------		---------	---------
******************************************************************************/
INT8U query_key_nsnd(void)
{
	INT8U temp[2];

	if ( (IO0PIN & BIT25) == 0 )
	{
	}
	else
	{
		return NO_KEY;			//û�а���
	}


// 	temp[0] = 0;
// 	temp[1] = 1;
// 	I2cRead(ZLG7290ADDR, temp, temp + 1, 1, 2);
	temp[0] = 0xFF;
	(void)I2c_SendAndReceive(ZLG7290ADDR, SubKey, temp, 1);

	//////////////////////////////////////////////////////////////
	//ת����INT8U�ļ�ֵ
	/////////////////////////////////////////////////////////////
	switch ( temp[0] )
	{
		case KEY_0:
			temp[0]='0';
			break;

		case KEY_1:
			temp[0]='1';
			break;

		case KEY_2:
			temp[0]='2';
			break;

		case KEY_3:
			temp[0]='3';
			break;

		case KEY_4:
			temp[0]='4';
			break;

		case KEY_5:
			temp[0]='5';
			break;

		case KEY_6:
			temp[0]='6';
			break;

		case KEY_7:
			temp[0]='7';
			break;

		case KEY_8:
			temp[0]='8';
			break;

		case KEY_9:
			temp[0]='9';
			break;

// 		case KEY_POINT:
// 			temp[0]='.';
// 			break;
//
// 		case KEY_ADD:
// 			temp[0]='+';
// 			break;
//
// 		case KEY_SUB:
// 			temp[0]='-';
// 			break;
//
// 		case KEY_MUL:
// 			temp[0]='*';
// 			break;
//
// 		case KEY_EQUAL:
// 			temp[0]='=';
// 			break;

		case KEY_ENTER:
		case KEY_CANCEL:
// 		case KEY_MODE:
// 		case KEY_FUNC:
			break;

		default:
			temp[0]=NO_KEY;
			break;
	}

	return(temp[0]);
}

