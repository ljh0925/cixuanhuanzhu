/******************************************************************************
Copyright 2005 ���������������޹�˾
All rights riserved.

�ļ���	��drv_display.c
ģ�����ƣ���ʾ��������
���ܸ�Ҫ����ʾ��������

ȡ���汾��0.0.1
�޸���	��������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include 		"config.h"
//#include "ziku.c"



// OS_EVENT *DispSem;			//����Һ�������ź�����display_init ����ʼ��
extern OS_EVENT        *Lcd_Sem;		//����Һ�������ź���

extern OS_EVENT        *Back_Light_Ctrl_Mbox;

extern volatile DEV_STAT DevStat;
extern const INT32S BACK_LIGHT_DELAY;



/******************************************************************************
�������ƣ�display_init
����������Һ������ʼ��
����������

��  ��  ֵ	����
��      ��	������
��      �ڣ�2005-04-08
�޸���ʷ��
����		�޸���		�޸�����
------		---------	---------
******************************************************************************/
void display_init(void)
{
	ToggleWD();

//	PINSEL2 = PINSEL2 & (~BIT4);       //P2.16~P2.22 ���ó�GPIO
//	PINSEL2 = PINSEL2 | (BIT5);       
	
	IO2DIR |= BIT17;			//P2.17 18 19 20 ��Ϊ���
	IO2DIR |= BIT18;
	IO2DIR |= BIT19;
	IO2DIR |= BIT20;

	//IO2DIR |= BIT21;			//P2.21 ��Ϊ���		���ϵĹ���ָʾ����

	SetLed(0, OFF);

//  SetBacklightON();
	SetBacklightOFF();

	Lcd_Init();
	Lcd_Cls();
}

/*****************************************************************
����ԭ�ͣ�PressKeyBkLtCtrl
��������������֮��ı�����ƴ���
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------

��  ��  ֵ����
��      �ߣ�����
��      �ڣ�2005-10-15
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
void PressKeyBkLtCtrl(void)
{
	ToggleWD();

//  if ( DevStat.back_light_mode == ALWAYS_OFF )
//  {
//      SetBacklightOFF();
//  }
//  else
//  {
		SetBacklightON();			//����������ֱ��⿪
//  }
	OSMboxPost(Back_Light_Ctrl_Mbox, (void *)&BACK_LIGHT_DELAY);				   //5��֮�ڲ��ر���
}

/*****************************************************************
����ԭ�ͣ�Write_Lcd_Ctrlreg
 ������������LCD�Ĵ���д����
 ����������
 �������ƣ�	����/�����	����		����
				data_to_write		����	INT8U		Ҫд������ݣ�һ���ֽ�
				
 ��  ��  ֵ����
 ��      ��	��������������
 �� 	 �ڣ�2004-03-01
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
*****************************************************************/
void Write_Lcd_Ctrlreg(INT8U data_to_write)
{
// 	INT8U i;
//	bit h;

	ToggleWD();

	OS_ENTER_CRITICAL();

// 	OutPortStat = (OutPortStat | 0x80);				//LCD CS SET HIGH
// 	EXTOUTPORT = OutPortStat;
	LCD_CS_SetHigh();				//LCD CS SET HIGH

// 	LCD_DATA = 1;				//����ͷ
	LCD_DI_SetHigh();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();

// 	LCD_DATA = 0;				//RW��RSȫд0
	LCD_DI_SetLow();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();

// 	LCD_DATA =  (bit)(data_to_write & 0x80);			//���͸���λ�ֽ�
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA = 0;				//��0
	LCD_DI_SetLow();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();

// 	LCD_DATA =  (bit)(data_to_write & 0x80);			//���͵���λ�ֽ�
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA = 0;				//��0
	LCD_DI_SetLow();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();

// 	LCD_DATA = 0;				//����β
	LCD_DI_SetLow();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();

// 	OutPortStat = (OutPortStat & 0x7F);			//LCD CS SET LOW
// 	EXTOUTPORT = OutPortStat;
	LCD_CS_SetLow();			//LCD CS SET LOW

	OS_EXIT_CRITICAL();

}

/*****************************************************************
����ԭ�ͣ�Write_Lcd_Ram
 ������������LCD RAMд����
 ����������
 �������ƣ�	����/�����	����		����
				data_to_write		����	INT8U		Ҫд������ݣ�һ���ֽ�
				
 ��  ��  ֵ����
 ��      ��	��������������
 �� 	 �ڣ�2004-03-01
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
*****************************************************************/
void Write_Lcd_Ram(INT8U data_to_write)
{
// 	INT8U i;
//	bit h;

// 	i = data_to_write;

	ToggleWD();

	OS_ENTER_CRITICAL();
// 	DISCS = HIGH;
// 	OutPortStat = (OutPortStat | 0x80);				//LCD CS SET HIGH
// 	EXTOUTPORT = OutPortStat;
	LCD_CS_SetHigh();				//LCD CS SET HIGH

// 	LCD_DATA = 1;				//����ͷ
	LCD_DI_SetHigh();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();

// 	LCD_DATA = 0;				//RWд0
	LCD_DI_SetLow();
	Dis_Clock();
// 	LCD_DATA = 1;				//RSд1
	LCD_DI_SetHigh();
	Dis_Clock();
// 	LCD_DATA = 0;
	LCD_DI_SetLow();
	Dis_Clock();

// 	LCD_DATA =  (bit)(data_to_write & 0x80);			//���͸���λ�ֽ�
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA = 0;				//��0
	LCD_DI_SetLow();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();

// 	LCD_DATA =  (bit)(data_to_write & 0x80);			//���͵���λ�ֽ�
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA =  (bit)(data_to_write & 0x80);
	if ((data_to_write & 0x80) == 0)
	{
		LCD_DI_SetLow();
	}
	else
	{
		LCD_DI_SetHigh();
	}
	data_to_write = data_to_write << 1;
	Dis_Clock();
// 	LCD_DATA = 0;				//��0
	LCD_DI_SetLow();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();

// 	LCD_DATA = 0;				//����β
	LCD_DI_SetLow();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();
	Dis_Clock();

// 	DISCS = LOW;
// 	OutPortStat = (OutPortStat & 0x7F);			//LCD CS SET LOW
// 	EXTOUTPORT = OutPortStat;
	LCD_CS_SetLow();			//LCD CS SET LOW

	OS_EXIT_CRITICAL();

// 	delay_50us(4);
	Sleep10us(20);

}

/*****************************************************************
����ԭ�ͣ�Dis_Clock
�����������ṩLCD��SPI�ӿڵ�CLK
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------

��  ��  ֵ����
��      ��	������
��      �ڣ�2004-03-01
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
void Dis_Clock(void)
{
//  ToggleWD();

	LCD_CLK_SetLow();
 	Sleep10us(5);
//	Sleep10us(4);
	LCD_CLK_SetHigh();
}

/*****************************************************************
����ԭ�ͣ�Lcd_Init
�����������ṩLCD�ĳ�ʼ��
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------

��  ��  ֵ����
��      ��	��������������
��      �ڣ�2004-03-01
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
void Lcd_Init(void)
{
	ToggleWD();

	Write_Lcd_Ctrlreg(0x30);		//0011 X0XX B������8bit���ƺͻ���ָ�
	Sleep10us(30);
	Write_Lcd_Ctrlreg(0x30);		//0011 X0XX B������8bit���ƺͻ���ָ�
	Sleep10us(30);
	Write_Lcd_Ctrlreg(0x30);		//0011 X0XX B������8bit���ƺͻ���ָ�
	Sleep10us(30);
	Write_Lcd_Ctrlreg(0x30);		//0011 X0XX B������8bit���ƺͻ���ָ�
	Sleep10us(30);

//	Write_Lcd_Ctrlreg(0x0E);		//0001 1110 B������ʾ���α�ON
//	delay_50us(3);
	Write_Lcd_Ctrlreg(0x0C);		//0001 1100 B������ʾ���α�OFF
	Sleep10us(30);

	Write_Lcd_Ctrlreg(0x01);		//�����ʾ
	SleepMs(10);

	Write_Lcd_Ctrlreg(0x06);		//0000 0110 B���趨�α��ƶ�����Ϊ����
	Sleep10us(30);
}


/*****************************************************************
 ����ԭ�ͣ�Write_Lcd_Gdram_X
 ������������LCD GDRAM X��ַ�Ĵ���д����
 ����������
 �������ƣ�	����/�����	����		����
				data_to_write		����	INT8U		Ҫд������ݣ�һ���ֽ�
				
 ��  ��  ֵ����
 ��      ��	��������������
 �� 	 �ڣ�2004-04-05
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
*****************************************************************/
/*void Write_Lcd_Gdram_X(INT8U data_to_write)
{
	INT8U i;
//	bit h;

//	if (data_to_write >= 16)			// X���겻������15
//		return;
//
	i = data_to_write & 0x0F;		//����λ��0
	i = i | 0x80;					//��һλ��1

	Write_Lcd_Ctrlreg(i);
	delay_50us(4);

	return;
}
*/
/*****************************************************************
 ����ԭ�ͣ�Write_Lcd_Gdram_Y
 ������������LCD GDRAM Y��ַ�Ĵ���д����
 ����������
 �������ƣ�	����/�����	����		����
				data_to_write		����	INT8U		Ҫд������ݣ�һ���ֽ�
				
 ��  ��  ֵ����
 ��      ��	��������������
 �� 	 �ڣ�2004-04-05
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
*****************************************************************/
/*void Write_Lcd_Gdram_Y(INT8U data_to_write)
{
	INT8U i;
//	bit h;

//	if (data_to_write >= 16)			// X���겻������15
//		return;
//
	i = data_to_write & 0x3F;		//����λ��0
	i = i | 0x80;					//��һλ��1

	Write_Lcd_Ctrlreg(i);
	delay_50us(4);

	return;
}*/

/*****************************************************************
����ԭ�ͣ�Lcd_Clean_Line
����������LCD���ָ����(ȫдΪ�ո�)
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------

��  ��  ֵ����
��      ��	��������������
��      �ڣ�2004-03-01
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
void Lcd_Clean_Line(INT8U line_to_clean)
{
	INT8U i;
	INT8U line_code = 0;

	ToggleWD();

	if ( line_to_clean > 3 )
		line_to_clean = 0;

	switch ( line_to_clean )
	{
		case 0:				//��1��
			line_code = 0x80;
			break;

		case 1:				//��2��
			line_code = 0x90;
			break;

		case 2:				//��3��
			line_code = 0x88;
			break;

		case 3:				//��4��
			line_code = 0x98;
			break;

		default:
			line_code = 0x98;	//��4��
	}
	Write_Lcd_Ctrlreg(line_code);		//AC��ַдΪҪ����еĿ�ͷ
// 	delay_50us(4);
	Sleep10us(20);

	for ( i=0; i<16; i++ )
	{
		Write_Lcd_Ram(0x20);				//�ո�
// 		delay_50us(4);
// 		Sleep10us(20);
	}
}

/*****************************************************************
����ԭ�ͣ�Lcd_Clean_Line
����������LCD������ƶ���ĳ���꣬����Ϊ(0, 0)������Ϊ(3, 7)
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------

��  ��  ֵ����
��      ��	��������������
��      �ڣ�2004-03-01
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
void Lcd_Goto(INT8U x, INT8U y)
{
	INT8U line_code;

	ToggleWD();

	//�������Ϸ���
	if ( x > 3 )
		x = 3;
	if ( y > 7 )
		y = 7;

	switch ( x )
	{
		case 0:				//��1��
			line_code = 0x80;
			break;

		case 1:				//��2��
			line_code = 0x90;
			break;

		case 2:				//��3��
			line_code = 0x88;
			break;

		case 3:				//��4��
			line_code = 0x98;
			break;

		default:
			line_code = 0x80;	//��4��
	}
	Write_Lcd_Ctrlreg(line_code + y);		//AC��ַдΪҪ����еĿ�ͷ
// 	delay_50us(4);
	Sleep10us(20);
}

/*****************************************************************
����ԭ�ͣ�Lcd_Set_Backlight
��������������LCD����Ŀ���
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------
backlight_status	����	INT8U	ON-�����⣬OFF-�ر���

��  ��  ֵ����
��      ��	������
��      �ڣ�2004-04-01
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
void Lcd_Set_Backlight(INT8U backlight_status)
{
	ToggleWD();

	if ( backlight_status == ON )		  //������
	{
// 		OutPortStat2 = (OutPortStat2 & 0xFE);			//���⿪
// 		EXTOUTPORT2 = OutPortStat2;
		SetBacklightON();
	}
	else						//�ر���
	{
// 		OutPortStat2 = (OutPortStat2 | 0x01);			//�����
// 		EXTOUTPORT2 = OutPortStat2;
		SetBacklightOFF();
	}
}

// /*****************************************************************
// ����ԭ�ͣ�Lcd_Display_Cursor
// ����������LCD��ʾ�α꣬����Ϊ(1, 1)������Ϊ(4, 8)
// ����������
// �������ƣ�	����/�����	����		����
// -----------		-----------	------	   	-------
//
// ��  ��  ֵ����
// ��      ��	�� ����
// ��      �ڣ�2004-03-01
// �޸���ʷ��
// ����		�޸���		�޸�����
// ------		---------	-------------
// *****************************************************************/
// void Lcd_Display_Cursor(INT8U x, INT8U y)
// {
// 	Lcd_Goto(x, y);
//
// 	Write_Lcd_Ctrlreg(0x0E);		//0001 1110 B������ʾ���α�ON
// 	delay_50us(4);
// }
//
// /*****************************************************************
// ����ԭ�ͣ�Lcd_Clean_Cursor
// ����������LCD�ر��α���ʾ
// ����������
// �������ƣ�	����/�����	����		����
// -----------		-----------	------	   	-------
//
// ��  ��  ֵ����
// ��      ��	�� ����
// ��      �ڣ�2004-03-01
// �޸���ʷ��
// ����		�޸���		�޸�����
// ------		---------	-------------
// *****************************************************************/
// void Lcd_Clean_Cursor(void)
// {
// 	Write_Lcd_Ctrlreg(0x0C);		//0001 1100 B������ʾ���α�OFF
// 	delay_50us(4);
// }

/*****************************************************************
����ԭ�ͣ�Lcd_Cls
����������LCD����ȫдΪ�ո�)
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------

��  ��  ֵ����
��      ��	�� ����
��      �ڣ�2004-03-01
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
void Lcd_Cls(void)
{
	ToggleWD();

	Lcd_Clean_Line(0);
	Lcd_Clean_Line(1);
	Lcd_Clean_Line(2);
	Lcd_Clean_Line(3);
}

/*****************************************************************
����ԭ�ͣ�printlcd
������������LCD ��ʾ
����������
�������ƣ�	����/�����	����		����
	s			����			const void*			��ʾ���ݵ��׵�ַ

��  ��  ֵ����
��      ��	������
�� 	 �ڣ�2005-08-19
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
void printlcd(const void *s)
{
	INT8U i = 0;
	INT8S *p = (INT8S *)s;

// 	while ( p[i] != '\0' )
	while ( (p[i] != '\0') && (i < 16) )
	{
        ToggleWD();
		Write_Lcd_Ram(p[i]);
		i++; 
	}
}

/*****************************************************************
����ԭ�ͣ�lcddisp
����������LCD������ƶ���ĳ���꣬����Ϊ(0, 0)������Ϊ(3, 7)��Ȼ����ʾ�ַ���
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------

��  ��  ֵ����
��      ��	������
��      �ڣ�2004-03-01
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
void lcddisp(INT8U x, INT8U y, void *s)
{
	INT8U err = 0;

	ToggleWD();

	OSSemPend(Lcd_Sem, 0, &err);

	Lcd_Goto(x, y);
	printlcd(s);

	OSSemPost(Lcd_Sem);
}



