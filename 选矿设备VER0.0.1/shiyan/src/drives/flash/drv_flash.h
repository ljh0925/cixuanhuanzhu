/******************************************************************************
		 	�������ķ�������޹�˾
			(Copyright 2005)	All rights riserved.

�ļ���	��taskmemory.h
��Ŀ���ƣ�С�����ѻ�
���ܸ�Ҫ��cpuʹ��philips ��lpc2214 
			����ϵͳ uCOS-II
������	����� (myron)
�������ڣ�2005.4.10
******************************************************************************/
#ifndef __DRV_FLASH_H__
#define __DRV_FLASH_H__


#define FLASH_OP_RETRY_TIMES	4				//flashд�����Դ���



#define  FLASH_RST0_SetLow()  		(IO0CLR = BIT10)
#define  FLASH_RST0_SetHigh()		 	(IO0SET = BIT10)

#define  FLASH_CS0_SetLow()  		(IO0CLR = BIT12)
#define  FLASH_CS0_SetHigh()		 	(IO0SET = BIT12)

#define  FLASH_WP0_SetLow()  		(IO0CLR = BIT11)
#define  FLASH_WP0_SetHigh()		 	(IO0SET = BIT11)

#define  FLASH_SCK0_SetLow()  		(IO0CLR = BIT13)
#define  FLASH_SCK0_SetHigh()		 	(IO0SET = BIT13)

#define  FLASH_SI0_SetLow()  		(IO0CLR = BIT15)
#define  FLASH_SI0_SetHigh()		 	(IO0SET = BIT15)



//#define  FLASH_SCK1_SetLow()        (IO0CLR = BIT17)
//#define  FLASH_SCK1_SetHigh()           (IO0SET = BIT17)
//
//#define  FLASH_SI1_SetLow()         (IO0CLR = BIT19)
//#define  FLASH_SI1_SetHigh()            (IO0SET = BIT19)
//
//#define  FLASH_RST1_SetLow()        (IO0CLR = BIT21)
//#define  FLASH_RST1_SetHigh()           (IO0SET = BIT21)
//
//#define  FLASH_CS1_SetLow()         (IO0CLR = BIT22)
//#define  FLASH_CS1_SetHigh()            (IO0SET = BIT22)
//
//#define  FLASH_WP1_SetLow()         (IO0CLR = BIT23)
//#define  FLASH_WP1_SetHigh()            (IO0SET = BIT23)





//flash  �����
#define		MAX_PAGE_SIZE	528		//flashҳ���ֵ;	ҳ��Ϊ0~527
#define		USED_PAGE_SIZE	512		//flashҳʵ��ʹ��ֵ;	ҳ��Ϊ0~511
// #define		MAXPAGENUM	4096		//һ������ҳ
#define		MAXPAGENUM	8192		//һ������ҳ
// #define		USED_PAGE_NUM	1024	//��¼��ռ�ö���ҳ


#define MAIN_FAT_FRT_STAT_PAGE		6
#define BAK_FAT_FRT_STAT_PAGE		7


// ��״̬�ּ�ҳ/�����
#define		READ_STATUSREAD			0xD7	// Status Register Read
#define		PAGE_ERASE				0x81	// Page Erase
#define		BLOCK_ERASE			0x50	// Block Erase
// Buffer   ��д����
#define		BUFFER1_READ			0xD4	// Read Data From Buffer 1
#define		BUFFER2_READ			0xD6	// Read Data From Buffer 2
#define		BUFFER1_WRITE		    	0x84	// Write Data To Buffer 1
#define		BUFFER2_WRITE			0x87	// Write Data To Buffer 2
// Flash ��Buffer   ���ݽ�������

#define		MAIN_COMPARE_BUFFER1	0x60 //������buffer1�Ƚ�
#define		MAIN_COMPARE_BUFFER2	0x61 //������buffer2�Ƚ�
#define		MAIN_TO_BUFFER1		0x53	// Main Memory Page Transfer Buffer 1
#define		MAIN_TO_BUFFER2		0x55	// Main Memory Page Transfer Buffer 2
#define		SELF_ERASE_BUFFER1_WRITE_MAIN		0x83	// ���Բ�����bufeer1д������
#define		SELF_ERASE_BUFFER2_WRITE_MAIN		0x86	// ���Բ�����bufeer2д������
#define		NO_ERASE_BUFFER1_WRITE_MAIN	    	0x88	// �����Բ�����bufeer1д������
#define		NO_ERASE_BUFFER2_WRITE_MAIN 		0x89	// �����Բ�����bufeer2д������
// ͨ��Buffer  ��Flash �������ݶ�д
#define		MAIN_BUFFER1_WRITE		0x82	// Main Memory Page Program Through Buffer 1(�Բ���)
#define		MAIN_BUFFER2_WRITE		0x85	// Main Memory Page Program Through Buffer 2(�Բ���)
#define		MAIN_PAGE_READ				0xD2	// ҳ��
#define		CONTINOUS_READ			0xE8	//��Ƭ���������Զ���ҳ��

// //����
// #define START_FLASH_PAGE 	0                              			//�洢��¼�õ���ʼҳ
// #define END_FLASH_PAGE 		(USED_PAGE_NUM - 1)                         //�洢��¼�õĽ���ҳ
// #define ERR_FLAG_PAGE_MAIN			4095									//�����־�Ĵ洢ҳ
// #define ERR_FLAG_PAGE_BAK		4087									//�����־�Ĵ洢ҳ�ı�����4095-8


/*****************************************************************
����ԭ�ͣ�Cal_Crc
 ��������������һ�����ݵ�CRCֵ������CCITT�㷨
 ����������
 �������ƣ�	����/�����	����		����
 data_to_cal_crc	����	INT8U const *		Ҫ����CRC������������ʼָ��
 len				 		����	INT16U				Ҫ����CRC���������ĳ���

 ��  ��  ֵ��uint�͵�CRCֵ(2�ֽ�)
 ��      ��	��������������
 ��      �ڣ�2004-02-04
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
*****************************************************************/
INT16U Cal_Crc(INT8U const *data_to_cal_crc, INT16U len);



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
void flash_init(void);



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
void FlashReset (INT8U which);



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
void FlashENWP(INT8U which);



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
void FlashDISENWP(INT8U which);



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
void Send_SPI_8Bit (INT8U value, INT8U which);



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
INT8U Receive_SPI_8Bit(INT8U which);



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
INT8U FlashReadStatus(INT8U which);



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
INT8U FlashBusy(INT8U which);



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
void FlashPageWrite512(INT16U page_address, const INT8U *source_address, INT8U which);



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
void FlashPageRead512(INT16U page_address,INT8U *source_address, INT8U which);



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
void WriteParam(void);



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
void ReadParam(void);



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
void FlashWriteWithoutErase(INT16U page_address, INT16U offset_in_page, const INT8U *source_address, INT16U len, INT8U which);



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
void FlashPageWrite256(INT16U page_address, const INT8U *source_address, INT8U which);



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
void FlashPageRead256(INT16U page_address,INT8U *source_address, INT8U which);








#endif
