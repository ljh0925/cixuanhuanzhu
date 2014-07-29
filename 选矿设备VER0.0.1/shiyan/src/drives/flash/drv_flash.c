/******************************************************************************
			�������ķ�������޹�˾
			(Copyright 2005)	All rights riserved.

�ļ���	��drv_flash.c
��Ŀ���ƣ�X���߲����
���ܸ�Ҫ��cpuʹ��philips ��lpc2214 
			����ϵͳ uCOS-II
������	������
�������ڣ�2005.4.10


ȡ���汾��0.0.1
�޸���	������
������ڣ�2005.4.10
����˵������һ��
******************************************************************************/
#include "config.h"



extern OS_EVENT        *FLASH_Sem;


// static const INT8U FORMAT_END_SIGN[4]={0x55,0xaa,0x18,0xe7};
// static const INT8U END_SIGN[4]={0x18,0xe7,0x18,0xe7};

// extern void Delay_10us(INT16U _times);

//static volatile INT8U  Addr13_18 = 0xFF;					//FLASH�ߵ�ַA13-A18״̬�Ĵ���
extern volatile DEV_STAT  DevStat;				//�豸״̬
//volatile INT8U  OutPortStat;
//volatile DISP_BUF DispBuf;  //��ʾ�����0-7���ζ�Ӧ�����(������)�ģ�
//volatile INT8U FErrPage[ERR_MAP_SIZE];		//�������ͼ�����Ա�ʾ512*8 = 4096ҳ��״̬

INT8U PageBuf1[512];
//static INT8U PageBuf2[512];

/*****************************************************************
����ԭ�ͣ�Cal_Crc
 ��������������һ�����ݵ�CRCֵ������CCITT�㷨
 ����������
 �������ƣ�	����/�����	����		����
 data_to_cal_crc	����	INT8U const *		Ҫ����CRC������������ʼָ��
 len				 		����	INT16U				Ҫ����CRC���������ĳ���

 ��  ��  ֵ��uint�͵�CRCֵ(2�ֽ�)
 ��      ��	������
 ��      �ڣ�2004-02-04
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
*****************************************************************/
INT16U Cal_Crc(INT8U const *data_to_cal_crc, INT16U len)
{
	INT16U crc;
	INT8U i;

	ToggleWD();

	crc = 0xFFFF;			//CRC��ʼֵ

	while ( len-- != 0 )
	{
		ToggleWD();

		crc = crc ^ ((INT16U)*data_to_cal_crc++ << 8);

		for ( i = 0; i < 8; ++i )
			if ( crc & 0x8000 )
				crc = (crc << 1) ^ 0x1021;
			else
				crc	= crc << 1;
	}

	return(crc & 0xFFFF);
}

/******************************************************************************
�������ƣ�flash_init
����������flash��ʼ��
����������

��  ��  ֵ	����
��      ��	������
��      �ڣ�2005-04-08
�޸���ʷ��
����		�޸���		�޸�����
------		---------	---------
******************************************************************************/
void flash_init(void)
{
	ToggleWD();

//  PINSEL0 = PINSEL0 & (~BIT8);
//  PINSEL0 = PINSEL0 & (~BIT9);        //P0.4 ���ó�GPIO
//
//  PINSEL0 = PINSEL0 & (~BIT10);
//  PINSEL0 = PINSEL0 & (~BIT11);       //P0.5 ���ó�GPIO
//
//  PINSEL0 = PINSEL0 & (~BIT12);
//  PINSEL0 = PINSEL0 & (~BIT13);       //P0.6 ���ó�GPIO
//
//  PINSEL0 = PINSEL0 & (~BIT20);
//  PINSEL0 = PINSEL0 & (~BIT21);       //P0.10 ���ó�GPIO
//
//  PINSEL0 = PINSEL0 & (~BIT22);
//  PINSEL0 = PINSEL0 & (~BIT23);       //P0.11 ���ó�GPIO
//
//  PINSEL0 = PINSEL0 & (~BIT24);
//  PINSEL0 = PINSEL0 & (~BIT25);       //P0.12 ���ó�GPIO
//
//  IO0DIR |= BIT4;             //���
//  IO0DIR &= (~BIT5);              //����
//  IO0DIR |= BIT6;
//  IO0DIR |= BIT10;
//  IO0DIR |= BIT11;
//  IO0DIR |= BIT12;
//
//
//  PINSEL1 = PINSEL1 & (~BIT2);
//  PINSEL1 = PINSEL1 & (~BIT3);        //P0.17 ���ó�GPIO
//
//  PINSEL1 = PINSEL1 & (~BIT4);
//  PINSEL1 = PINSEL1 & (~BIT5);        //P0.18 ���ó�GPIO
//
//  PINSEL1 = PINSEL1 & (~BIT6);
//  PINSEL1 = PINSEL1 & (~BIT7);        //P0.19 ���ó�GPIO
//
//  PINSEL1 = PINSEL1 & (~BIT10);
//  PINSEL1 = PINSEL1 & (~BIT11);       //P0.21 ���ó�GPIO
//
//  PINSEL1 = PINSEL1 & (~BIT12);
//  PINSEL1 = PINSEL1 & (~BIT13);       //P0.22 ���ó�GPIO
//
//  PINSEL1 = PINSEL1 & (~BIT14);
//  PINSEL1 = PINSEL1 & (~BIT15);       //P0.23 ���ó�GPIO
//
//  IO0DIR |= BIT17;                //���
//  IO0DIR &= (~BIT18);             //����
//  IO0DIR |= BIT19;
//  IO0DIR |= BIT21;
//  IO0DIR |= BIT22;
//  IO0DIR |= BIT23;


	PINSEL0 = PINSEL0 & (~BIT20);
	PINSEL0 = PINSEL0 & (~BIT21);		//P0.10 ���ó�GPIO

	PINSEL0 = PINSEL0 & (~BIT22);
	PINSEL0 = PINSEL0 & (~BIT23);		//P0.11 ���ó�GPIO

	PINSEL0 = PINSEL0 & (~BIT24);
	PINSEL0 = PINSEL0 & (~BIT25);		//P0.12 ���ó�GPIO

	PINSEL0 = PINSEL0 & (~BIT26);
	PINSEL0 = PINSEL0 & (~BIT27);		//P0.13 ���ó�GPIO

	PINSEL0 = PINSEL0 & (~BIT30);
	PINSEL0 = PINSEL0 & (~BIT31);		//P0.15 ���ó�GPIO

	PINSEL1 = PINSEL1 & (~BIT0);
	PINSEL1 = PINSEL1 & (~BIT1);		//P0.16 ���ó�GPIO


	IO0DIR |= BIT10;
	IO0DIR |= BIT11;
	IO0DIR |= BIT12;
	IO0DIR |= BIT13;
	IO0DIR |= BIT15;                //���
	IO0DIR &= (~BIT16);             //����



	FlashReset(0);
	FlashDISENWP(0);				//���д����
//  FlashDISENWP(1);                //���д����

}

/******************************************************************************
 �������ƣ�FlashReset
 ������������λflash
 �������ƣ� ����/����� ����        ����
 which        ����        INT8U      ��FLASH���Ǵ�FLASH��0 �� 1

 ��  ��  ֵ����

 ��      �� ������
 ��      �ڣ�2004-11-29
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
void FlashReset(INT8U which)
{
	ToggleWD();

	which = which;		//avoid warning
//  if ( which == 0 )
//  {
		FLASH_RST0_SetLow();
		SleepMs(1);
		FLASH_RST0_SetHigh();
//  }
//  else
//  {
//      FLASH_RST1_SetLow();
//      SleepMs(1);
//      FLASH_RST1_SetHigh();
//  }
}

/******************************************************************************
 �������ƣ�FlashENWP
 ����������ʼ��flash��д����
 �������ƣ� ����/����� ����        ����
 which        ����        INT8U      ��FLASH���Ǵ�FLASH��0 �� 1

 ��  ��  ֵ����

 ��      �� ������
 ��      �ڣ�2004-11-29
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
void FlashENWP(INT8U which)
{
	ToggleWD();

	which = which;		//avoid warning
//  if ( which == 0 )
//  {
		FLASH_WP0_SetLow();
//  }
//  else
//  {
//      FLASH_WP1_SetLow();
//  }
}

/******************************************************************************
 �������ƣ�FlashDISENWP
 ����������ȥ��flash��д����
 �������ƣ� ����/����� ����        ����
 which        ����        INT8U      ��FLASH���Ǵ�FLASH��0 �� 1

 ��  ��  ֵ����

 ��      �� ������
 ��      �ڣ�2004-11-29
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
void FlashDISENWP(INT8U which)
{
	ToggleWD();

	which = which;		//avoid warning
//  if ( which == 0 )
//  {
		FLASH_WP0_SetHigh();
		Sleep10us(10);
//  }
//  else
//  {
//      FLASH_WP1_SetHigh();
//      Sleep10us(10);
//  }
}

/******************************************************************************
 �������ƣ�FLASH_SCK_SetLow
 ����������
 �������ƣ� ����/����� ����        ����
 which        ����        INT8U      ��FLASH���Ǵ�FLASH��0 �� 1

 ��  ��  ֵ����

 ��      �� ������
 ��      �ڣ�2004-11-29
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
void FLASH_SCK_SetLow(INT8U which)
{
	ToggleWD();

	which = which;		//avoid warning
//  if ( which == 0 )
//  {
		FLASH_SCK0_SetLow();
//  }
//  else
//  {
//      FLASH_SCK1_SetLow();
//  }
}

/******************************************************************************
 �������ƣ�FLASH_SCK_SetHigh
 ����������
 �������ƣ� ����/����� ����        ����
 which        ����        INT8U      ��FLASH���Ǵ�FLASH��0 �� 1

 ��  ��  ֵ����

 ��      �� ������
 ��      �ڣ�2004-11-29
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
void FLASH_SCK_SetHigh(INT8U which)
{
	ToggleWD();

	which = which;		//avoid warning
//  if ( which == 0 )
//  {
		FLASH_SCK0_SetHigh();
//  }
//  else
//  {
//      FLASH_SCK1_SetHigh();
//  }
}

/******************************************************************************
 �������ƣ�FLASH_SI_SetLow
 ����������
 �������ƣ� ����/����� ����        ����
 which        ����        INT8U      ��FLASH���Ǵ�FLASH��0 �� 1

 ��  ��  ֵ����

 ��      �� ������
 ��      �ڣ�2004-11-29
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
void FLASH_SI_SetLow(INT8U which)
{
	ToggleWD();

	which = which;		//avoid warning
//  if ( which == 0 )
//  {
		FLASH_SI0_SetLow();
//  }
//  else
//  {
//      FLASH_SI1_SetLow();
//  }
}

/******************************************************************************
 �������ƣ�FLASH_SI_SetHigh
 ����������
 �������ƣ� ����/����� ����        ����
 which        ����        INT8U      ��FLASH���Ǵ�FLASH��0 �� 1

 ��  ��  ֵ����

 ��      �� ������
 ��      �ڣ�2004-11-29
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
void FLASH_SI_SetHigh(INT8U which)
{
	ToggleWD();

	which = which;		//avoid warning
//  if ( which == 0 )
//  {
		FLASH_SI0_SetHigh();
//  }
//  else
//  {
//      FLASH_SI1_SetHigh();
//  }
}

/******************************************************************************
 �������ƣ�FLASH_RST_SetLow
 ����������
 �������ƣ� ����/����� ����        ����
 which        ����        INT8U      ��FLASH���Ǵ�FLASH��0 �� 1

 ��  ��  ֵ����

 ��      �� ������
 ��      �ڣ�2004-11-29
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
void FLASH_RST_SetLow(INT8U which)
{
	ToggleWD();

	which = which;		//avoid warning
//  if ( which == 0 )
//  {
		FLASH_RST0_SetLow();
//  }
//  else
//  {
//      FLASH_RST1_SetLow();
//  }
}

/******************************************************************************
 �������ƣ�FLASH_RST_SetHigh
 ����������
 �������ƣ� ����/����� ����        ����
 which        ����        INT8U      ��FLASH���Ǵ�FLASH��0 �� 1

 ��  ��  ֵ����

 ��      �� ������
 ��      �ڣ�2004-11-29
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
void FLASH_RST_SetHigh(INT8U which)
{
	ToggleWD();

	which = which;		//avoid warning
//  if ( which == 0 )
//  {
		FLASH_RST0_SetHigh();
//  }
//  else
//  {
//      FLASH_RST1_SetHigh();
//  }
}

/******************************************************************************
 �������ƣ�FLASH_CS_SetLow
 ����������
 �������ƣ� ����/����� ����        ����
 which        ����        INT8U      ��FLASH���Ǵ�FLASH��0 �� 1

 ��  ��  ֵ����

 ��      �� ������
 ��      �ڣ�2004-11-29
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
void FLASH_CS_SetLow(INT8U which)
{
	ToggleWD();

	which = which;		//avoid warning
//  if ( which == 0 )
//  {
		FLASH_CS0_SetLow();
//  }
//  else
//  {
//      FLASH_CS1_SetLow();
//  }
}

/******************************************************************************
 �������ƣ�FLASH_CS_SetHigh
 ����������
 �������ƣ� ����/����� ����        ����
 which        ����        INT8U      ��FLASH���Ǵ�FLASH��0 �� 1

 ��  ��  ֵ����

 ��      �� ������
 ��      �ڣ�2004-11-29
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
void FLASH_CS_SetHigh(INT8U which)
{
	ToggleWD();

	which = which;		//avoid warning
//  if ( which == 0 )
//  {
		FLASH_CS0_SetHigh();
//  }
//  else
//  {
//      FLASH_CS1_SetHigh();
//  }
}

/******************************************************************************
 �������ƣ�Send_SPI_8Bit
 ������������spi���߷���1�ֽ�����
 �������ƣ� ����/����� ����        ����
 value        ����        INT8U      Ҫ���͵���
 which        ����        INT8U      ��FLASH���Ǵ�FLASH��0 �� 1

 ��  ��  ֵ����

 ��      �� ������
 ��      �ڣ�2004-11-29
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
void Send_SPI_8Bit (INT8U value, INT8U which)
{
// 	uchar	ValueNum;
//
// 	P2DIR|=FLASH_CLK;
// 	P2DIR|=FLASH_SI;
// 	for (ValueNum = 0; ValueNum < 8; ValueNum++)
// 	{
// 		P2OUT&=~FLASH_CLK;//CLK SET LOW
// 		if (Value&0x80)
// 			P2OUT|=FLASH_SI;	//��Ƭע��ĸ�����駿ڵ�λ�ò�ͬ
// 		else
// 			P2OUT&=~FLASH_SI;
// 		Value <<= 1;
// 		P2OUT|=FLASH_CLK;//CLK SET HIGH
// 	}
// 	P2OUT&=~FLASH_CLK;//CLK SET LOW


	INT8U i = 0;

	ToggleWD();

	OS_ENTER_CRITICAL();

	for ( i=0; i<8; i++ )
	{
		FLASH_SCK_SetLow(which);			//sck set low
		if ( value & 0x80 )
		{
			FLASH_SI_SetHigh(which);
		}
		else
		{
			FLASH_SI_SetLow(which);
		}
		value <<= 1;
		FLASH_SCK_SetHigh(which);			//sck set high
	}
	FLASH_SCK_SetLow(which);			//sck set low

	OS_EXIT_CRITICAL();
}

/******************************************************************************
 �������ƣ�Receive_SPI_8Bit
 ������������spi���߽���1�ֽ�����
 �������ƣ� ����/����� ����        ����
 which        ����        INT8U      ��FLASH���Ǵ�FLASH��0 �� 1

 ��  ��  ֵ����������

 ��      �� ������
 ��      �ڣ�2004-11-29
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
INT8U Receive_SPI_8Bit(INT8U which)
{
// 	uchar	ValueNum, Value;
// 	P2DIR|=FLASH_CLK;
// 	P2DIR&=~FLASH_SO;
// 	for (ValueNum = 0; ValueNum < 8; ValueNum++)
// 	{
// 		P2OUT|=FLASH_CLK;//CLK SET HIGH
// 		Value <<= 1;
// 		if ((P2IN&FLASH_SO)== (0XFF&FLASH_SO))
// 			Value |= 0x01;
// 		P2OUT&=~FLASH_CLK;//CLK SET LOW
// 	}
// 	return (Value);


	INT8U i = 0;
	INT8U receive = 0;

	ToggleWD();

	OS_ENTER_CRITICAL();

	for ( i=0; i<8; i++ )
	{
		FLASH_SCK_SetHigh(which);			//sck set high
		receive <<= 1;
//      if ( which == 0 )
//      {
			if ( (IO0PIN & BIT16) == 0 )
			{
			}
			else
			{
				receive |= 0x01;
			}
//      }
//      else
//      {
//          if ( (IO0PIN & BIT18) == 0 )
//          {
//          }
//          else
//          {
//              receive |= 0x01;
//          }
//      }
		FLASH_SCK_SetLow(which);			//sck set low
	}

	OS_EXIT_CRITICAL();
	return receive;
}

/******************************************************************************
 �������ƣ�FlashReadStatus
 ������������flash״̬�Ĵ���
 �������ƣ� ����/����� ����        ����
 which        ����        INT8U      ��FLASH���Ǵ�FLASH��0 �� 1

 ��  ��  ֵ����������

 ��      �� ������
 ��      �ڣ�2004-11-29
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
INT8U FlashReadStatus(INT8U which)
{
// 	uchar	ValueStatus;
//
// 	// Choose The Chip
// 	P2DIR|=FLASH_CLK;
// 	P2OUT|=FLASH_CLK;//CLK SET HIGH
// 	P4DIR |=FLASH_CS;
// 	P4OUT|=FLASH_CS;	//FLASH_CS SET  HIGH
// 	P4OUT&=~FLASH_CS;	//FLASH_CS SET  LOW
// 	Send_SPI_8Bit(Status_Read);
// 	ValueStatus = Receive_SPI_8Bit();
//
// 	P4OUT|=FLASH_CS;	//FLASH_CS SET	HIGH
// 	P2OUT&=~FLASH_CLK;	//CLK SET LOW
//
// 	return(ValueStatus);

	INT8U status = 0;

	ToggleWD();

	FLASH_SCK_SetHigh(which);
	FLASH_CS_SetHigh(which);
	FLASH_CS_SetLow(which);

	Send_SPI_8Bit(READ_STATUSREAD, which);
	status = Receive_SPI_8Bit(which);

	FLASH_CS_SetHigh(which);
	FLASH_SCK_SetLow(which);

	return status;
}

/******************************************************************************
 �������ƣ�FlashBusy
 ������������flash�Ƿ�æ
 �������ƣ� ����/����� ����        ����
 which        ����        INT8U      ��FLASH���Ǵ�FLASH��0 �� 1

 ��  ��  ֵ��TRUE = BUSY
				FALSE = DON'T BUSY

 ��      �� ������
 ��      �ڣ�2004-11-29
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
INT8U FlashBusy(INT8U which)
{
	ToggleWD();

	if ( FlashReadStatus(which) < 0x80 )
		return TRUE;				//Busy
	else
		return FALSE;				//Don't Busy
}

/******************************************************************************
�������ƣ�FlashPageWrite512
����������������(512 bytes)	д��Flash �е�ĳһҳ(���Բ�����д)
����������
�������ƣ�	����/�����	����		����
page_address		  ���� 				 INT16U 		ҳ��ַ
*source_address		���			const INT8U *		Դ����(512 bytes)	��ŵ�ַ		
 which        ����        INT8U      ��FLASH���Ǵ�FLASH��0 �� 1

��      ��	������
��      �ڣ�2004-09-02
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
******************************************************************************/
void FlashPageWrite512(INT16U page_address, const INT8U *source_address, INT8U which)
{
	INT8U LOCAL_V_TYPE high_address;
	INT8U LOCAL_V_TYPE middle_address;
	INT8U LOCAL_V_TYPE low_address;      
	INT16U  LOCAL_V_TYPE i;
	INT8U err = 0;

	ToggleWD();

	high_address = (INT8U)(page_address >> 6) & 0x7F;			//ȡ�ø�λ��ַ		
	middle_address = (INT8U)(page_address << 2) & 0xFC;		  //ȡ����λ��ַ		
	low_address = 0;							//ȡ�õ�λ��ַ		

	OSSemPend(FLASH_Sem, 0, &err);

	while ( FlashBusy(which) == TRUE )
	{
		ToggleWD();	// Check Flash Busy
	}

// 	FlashDISENWP(which);				//���д����
	FLASH_SCK_SetHigh(which);
	FLASH_CS_SetLow(which);

	Send_SPI_8Bit(MAIN_BUFFER1_WRITE, which);	//����д����		
	Send_SPI_8Bit(high_address, which);     
	Send_SPI_8Bit(middle_address, which);       
	Send_SPI_8Bit(low_address, which);      

	for ( i=0; i< USED_PAGE_SIZE; i++ )		  //*source_addressָ��ĵ�ַ�����ݷ���flash��		
		Send_SPI_8Bit(*source_address++, which);

	FLASH_CS_SetHigh(which);
	FLASH_SCK_SetLow(which);
// 	FlashENWP(which);				//ʼ��д����

	OSSemPost(FLASH_Sem);
}       

/******************************************************************************
�������ƣ�FlashPageRead512
������������Flash �е�ĳһҳ
����������
�������ƣ�	����/�����	����		����
page_address		  ���� 				 INT16U 		ҳ��ַ
*source_address		���			INT8U *		������������(����Ϊ512)��ŵ�ַ		
 which        ����        INT8U      ��FLASH���Ǵ�FLASH��0 �� 1

��      ��	������
��      �ڣ�2004-09-02
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
******************************************************************************/
void FlashPageRead512(INT16U page_address,INT8U *source_address, INT8U which)
{
	INT16U  LOCAL_V_TYPE i;     
	INT8U LOCAL_V_TYPE high_address;
	INT8U LOCAL_V_TYPE middle_address;      
	INT8U err = 0;

	ToggleWD();

	high_address = (INT8U)(page_address >> 6) & 0x7F;			//ȡ�ø�λ��ַ		
	middle_address = (INT8U)(page_address << 2) & 0xFC;		  //ȡ����λ��ַ		

	OSSemPend(FLASH_Sem, 0, &err);

	while ( FlashBusy(which) == TRUE )
	{
		ToggleWD();	// Check Flash Busy
	}

// 	FlashDISENWP();				//���д����
	FLASH_SCK_SetHigh(which);
	FLASH_CS_SetLow(which);

	Send_SPI_8Bit(MAIN_PAGE_READ, which);	//���Ͷ�����		
	Send_SPI_8Bit(high_address, which);     
	Send_SPI_8Bit(middle_address, which);       
	Send_SPI_8Bit(0, which);        
	Send_SPI_8Bit(0, which);		//���Ϳ��ֽ�		
	Send_SPI_8Bit(0, which);		//���Ϳ��ֽ�		
	Send_SPI_8Bit(0, which);		//���Ϳ��ֽ�		
	Send_SPI_8Bit(0, which);		//���Ϳ��ֽ�		
	for ( i=0; i<USED_PAGE_SIZE; i++ ) //��ʼ�������ݷ���aim_data_addressָ��ĵ�ַ��		
		*source_address++ = Receive_SPI_8Bit(which);        

	FLASH_CS_SetHigh(which);
	FLASH_SCK_SetLow(which);
// 	FlashENWP();				//ʼ��д����

	OSSemPost(FLASH_Sem);
}  

/******************************************************************************
 �������ƣ�WriteParam
 �����������������
 ����������
 �������ƣ�	����/�����	����		����
 
 ��  ��  ֵ����
				   
 ��      ��	������
 ��      �ڣ�2006-08-02
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void WriteParam(void)
{
// 	INT8U h = 0;
//	INT8U i = 0;
//	INT8U j = 0;
//  INT8U buf[512];
//  INT8U buf2[512];
//  INT8U buf[256];
//  INT8U buf2[256];

// 	INT16U crc = 0;				//add for debug

	ToggleWD();

	OS_ENTER_CRITICAL();

	memset(PageBuf1, 0xFF, sizeof(PageBuf1));
	memcpy(PageBuf1, (void *)&DevStat.MOD_REG.reg[0], 512);
	FlashPageWrite512(4, PageBuf1, 0);

//	memset(PageBuf1, 0xFF, sizeof(PageBuf1));
//	memcpy(PageBuf1, (void *)&DevStat.MOD_REG.reg[256], 512);
//	FlashPageWrite512(5, PageBuf1, 0);

	OS_EXIT_CRITICAL();

}

/******************************************************************************
 �������ƣ�ReadParam
 ������������ȡ����
 ����������
 �������ƣ�	����/�����	����		����
 
 ��  ��  ֵ����
				   
 ��      ��	������
 ��      �ڣ�2006-08-02
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void ReadParam(void)
{
// 	INT8U i = 0;
// 	INT8U j = 0;
//  INT8U buf[528];
//  INT8U buf[256];
//	INT16U crc = 0;

	ToggleWD();

	OS_ENTER_CRITICAL();

	FlashPageRead512(4, PageBuf1, 0);
	memcpy((void *)&DevStat.MOD_REG.reg[0], PageBuf1, 512);

//	FlashPageRead512(5, PageBuf1, 0);
//	memcpy((void *)&DevStat.MOD_REG.reg[256], PageBuf1, 512);

	OS_EXIT_CRITICAL();
}

/******************************************************************************
�������ƣ�FlashWriteWithoutErase
����������������д��Flash �е�ĳһҳ(���Բ���)
����������
�������ƣ�	����/�����	����		����
page_address		  ���� 				 INT16U 		ҳ��ַ
offset_in_page			����			INT16U			ҳ�ڵ�ַ
*source_address		���			const INT8U *		Դ����(512 bytes)	��ŵ�ַ		
len							����				INT16U			д�볤��
which        ����        INT8U      ��FLASH���Ǵ�FLASH��0 �� 1

��      ��	������
��      �ڣ�2004-09-02
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
******************************************************************************/
void FlashWriteWithoutErase(INT16U page_address, INT16U offset_in_page, const INT8U *source_address, INT16U len, INT8U which)
{
	INT8U LOCAL_V_TYPE high_address;
	INT8U LOCAL_V_TYPE middle_address;
	INT8U LOCAL_V_TYPE low_address;      
	INT16U  LOCAL_V_TYPE i;
	INT8U err = 0;

	ToggleWD();

	high_address = (INT8U)(page_address >> 6) & 0x7F;			//ȡ�ø�λ��ַ		
	middle_address = (INT8U)(page_address << 2) & 0xFC;		  //ȡ����λ��ַ		
	low_address = 0;							//ȡ�õ�λ��ַ		

	OSSemPend(FLASH_Sem, 0, &err);

	//////////////////////////////////////////////////
	//ҳ����->buffer1
	//////////////////////////////////////////////////
	while ( FlashBusy(which) == TRUE )
	{
		ToggleWD();	// Check Flash Busy
	}

// 	FlashDISENWP(which);				//���д����
	FLASH_SCK_SetHigh(which);
	FLASH_CS_SetLow(which);

	Send_SPI_8Bit(MAIN_TO_BUFFER1, which);	//����дҳ����->buffer1����
	Send_SPI_8Bit(high_address, which);     
	Send_SPI_8Bit(middle_address, which);       
	Send_SPI_8Bit(low_address, which);      

	FLASH_CS_SetHigh(which);
	FLASH_SCK_SetLow(which);
// 	FlashENWP(which);				//ʼ��д����

	SleepMs(2);


	//////////////////////////////////////////////////
	//дbuffer1
	//////////////////////////////////////////////////
	if ( offset_in_page > 512 )
		offset_in_page %= 512;
	if ( offset_in_page >= 256 )
		middle_address |= 1;
	low_address = offset_in_page % 256;							//ȡ�õ�λ��ַ		

	while ( FlashBusy(which) == TRUE )
	{
		ToggleWD();	// Check Flash Busy
	}

// 	FlashDISENWP(which);				//���д����
	FLASH_SCK_SetHigh(which);
	FLASH_CS_SetLow(which);

	Send_SPI_8Bit(BUFFER1_WRITE, which);	//����дbuffer1����
	Send_SPI_8Bit(high_address, which);     
	Send_SPI_8Bit(middle_address, which);       
	Send_SPI_8Bit(low_address, which);      

	for ( i=0; i< len; i++ )		  //*source_addressָ��ĵ�ַ�����ݷ���flash��
		Send_SPI_8Bit(*source_address++, which);

	FLASH_CS_SetHigh(which);
	FLASH_SCK_SetLow(which);
// 	FlashENWP(which);				//ʼ��д����


	//////////////////////////////////////////////////
	//buffer1->ҳ����
	//////////////////////////////////////////////////
	high_address = (INT8U)(page_address >> 6) & 0x7F;			//ȡ�ø�λ��ַ		
	middle_address = (INT8U)(page_address << 2) & 0xFC;		  //ȡ����λ��ַ		
	low_address = 0;							//ȡ�õ�λ��ַ		

	while ( FlashBusy(which) == TRUE )
	{
		ToggleWD();	// Check Flash Busy
	}

// 	FlashDISENWP(which);				//���д����
	FLASH_SCK_SetHigh(which);
	FLASH_CS_SetLow(which);

	Send_SPI_8Bit(NO_ERASE_BUFFER1_WRITE_MAIN, which);	//buffer1->ҳ����
	Send_SPI_8Bit(high_address, which);     
	Send_SPI_8Bit(middle_address, which);       
	Send_SPI_8Bit(low_address, which);      

	FLASH_CS_SetHigh(which);
	FLASH_SCK_SetLow(which);
// 	FlashENWP(which);				//ʼ��д����


	OSSemPost(FLASH_Sem);
}

/******************************************************************************
�������ƣ�FlashPageWrite256
����������������(512 bytes)	д��Flash �е�ĳһҳ(���Բ�����д)
����������
�������ƣ�	����/�����	����		����
page_address		  ���� 				 INT16U 		ҳ��ַ
*source_address		���			const INT8U *		Դ����(512 bytes)	��ŵ�ַ		
 which        ����        INT8U      ��FLASH���Ǵ�FLASH��0 �� 1

��      ��	������
��      �ڣ�2004-09-02
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
******************************************************************************/
void FlashPageWrite256(INT16U page_address, const INT8U *source_address, INT8U which)
{
	INT8U LOCAL_V_TYPE high_address;
	INT8U LOCAL_V_TYPE middle_address;
	INT8U LOCAL_V_TYPE low_address;      
	INT16U  LOCAL_V_TYPE i;
	INT8U err = 0;

	ToggleWD();

	high_address = (INT8U)(page_address >> 6) & 0x7F;			//ȡ�ø�λ��ַ		
	middle_address = (INT8U)(page_address << 2) & 0xFC;		  //ȡ����λ��ַ		
	low_address = 0;							//ȡ�õ�λ��ַ		

	OSSemPend(FLASH_Sem, 0, &err);

	while ( FlashBusy(which) == TRUE )
	{
		ToggleWD();	// Check Flash Busy
	}

// 	FlashDISENWP(which);				//���д����
	FLASH_SCK_SetHigh(which);
	FLASH_CS_SetLow(which);

	Send_SPI_8Bit(MAIN_BUFFER1_WRITE, which);	//����д����		
	Send_SPI_8Bit(high_address, which);     
	Send_SPI_8Bit(middle_address, which);       
	Send_SPI_8Bit(low_address, which);      

	for ( i=0; i< 256; i++ )		  //*source_addressָ��ĵ�ַ�����ݷ���flash��		
		Send_SPI_8Bit(*source_address++, which);

	FLASH_CS_SetHigh(which);
	FLASH_SCK_SetLow(which);
// 	FlashENWP(which);				//ʼ��д����

	OSSemPost(FLASH_Sem);
}       

/******************************************************************************
�������ƣ�FlashPageRead256
������������Flash �е�ĳһҳ
����������
�������ƣ�	����/�����	����		����
page_address		  ���� 				 INT16U 		ҳ��ַ
*source_address		���			INT8U *		������������(����Ϊ512)��ŵ�ַ		
 which        ����        INT8U      ��FLASH���Ǵ�FLASH��0 �� 1

��      ��	������
��      �ڣ�2004-09-02
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
******************************************************************************/
void FlashPageRead256(INT16U page_address,INT8U *source_address, INT8U which)
{
	INT16U  LOCAL_V_TYPE i;     
	INT8U LOCAL_V_TYPE high_address;
	INT8U LOCAL_V_TYPE middle_address;      
	INT8U err = 0;

	ToggleWD();

	high_address = (INT8U)(page_address >> 6) & 0x7F;			//ȡ�ø�λ��ַ		
	middle_address = (INT8U)(page_address << 2) & 0xFC;		  //ȡ����λ��ַ		

	OSSemPend(FLASH_Sem, 0, &err);

	while ( FlashBusy(which) == TRUE )
	{
		ToggleWD();	// Check Flash Busy
	}

// 	FlashDISENWP();				//���д����
	FLASH_SCK_SetHigh(which);
	FLASH_CS_SetLow(which);

	Send_SPI_8Bit(MAIN_PAGE_READ, which);	//���Ͷ�����		
	Send_SPI_8Bit(high_address, which);     
	Send_SPI_8Bit(middle_address, which);       
	Send_SPI_8Bit(0, which);        
	Send_SPI_8Bit(0, which);		//���Ϳ��ֽ�		
	Send_SPI_8Bit(0, which);		//���Ϳ��ֽ�		
	Send_SPI_8Bit(0, which);		//���Ϳ��ֽ�		
	Send_SPI_8Bit(0, which);		//���Ϳ��ֽ�		
	for ( i=0; i<256; i++ ) //��ʼ�������ݷ���aim_data_addressָ��ĵ�ַ��		
		*source_address++ = Receive_SPI_8Bit(which);        

	FLASH_CS_SetHigh(which);
	FLASH_SCK_SetLow(which);
// 	FlashENWP();				//ʼ��д����

	OSSemPost(FLASH_Sem);
}  

