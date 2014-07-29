
/**************************************************************
�� File Name  : register.h
�� Originator :	Texas Instrument-Chongqing University DSPs Lab.
�� Description:	F240 Peripheral Registers + other useful definitions	
��**************************************************************/
/*--------------------------------------------------------------
�� Ƭ����Χ�Ĵ�������
��--------------------------------------------------------------*/
/*C2xx �ں˼Ĵ���
��~~~~~~~~~~~~~~~~~~~~*/
#define   IMR    (volatile unsigned int   *) 0x0004 
/*�ж����μĴ���*/
#define   GREG   (volatile unsigned int   *) 0x0005  
/*ȫ�ֱ�����λ�Ĵ���*/	
#define   IFR  (volatile unsigned int   *) 0x0006  
/*�жϱ�־�Ĵ���*/                                            

ioport   unsigned portFFFF; 	
#define  WSGR	  portFFFF
/*ϵͳģ��Ĵ���						
��~~~~~~~~~~~~~~~~~~~~~~~*/						

#define     SYSCR 	  (volatile unsigned int   *) 0x7018 
/* ϵͳ���ƺ�״̬�Ĵ���1*/
#define     SYSSR 	  (volatile unsigned int   *) 0x701A 
/* ϵͳ���ƺ�״̬�Ĵ���2*/
#define     SYSIVR   (volatile unsigned int   *) 0x701E 
/* ��Χ�ж������Ĵ���*/
/*���Ź�/ ʵʱ�ж�(RTI*)  ���໷ (PLL*)  �Ĵ���
��~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define    RTICNTR   (volatile unsigned int   *) 0x7021 
/*ʵʱ�жϼ����Ĵ���*/
#define     WDCNTR   (volatile unsigned int   *) 0x7023 
/*���Ź������Ĵ���*/
#define     WDKEY  (volatile unsigned int   *) 0x7025 	

/* ���Ź� Key �Ĵ���*/
#define    RTICR   (volatile unsigned int   *) 0x7027 
/*ʵʱ�жϿ��ƼĴ���*/
#define     WDCR 	  (volatile unsigned int   *) 0x7029 
/* ���Ź����ƼĴ���*/ 
#define    CKCR0   (volatile unsigned int   *) 0x702B 
#define    CKCR1   (volatile unsigned int   *) 0x702D 
/*��Χ���нӿ�(SPI*) �Ĵ���
��~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define     SPICCR   (volatile unsigned int   *) 0x7040 
/* SPI���ÿ��ƼĴ���1*/
#define     SPICTL   (volatile unsigned int   *) 0x7041 
/* SPI ���п��ƼĴ���2*/
#define     SPISTS 	  (volatile unsigned int   *) 0x7042 
/* SPI ״̬�Ĵ���*/
#define     SPIBRR 	  (volatile unsigned int   *) 0x7044 
/* SPI �����ʿ��ƼĴ���*/
#define     SPIEMU 	  (volatile unsigned int   *) 0x7046 
/* SPI ���滺��Ĵ���*/
#define     SPIRXBUF	  (volatile unsigned int   *) 0x7047 
/* SPI �������뻺��Ĵ���*/
#define     SPITXBUF	  (volatile unsigned int   *) 0x7048 
/* SPI �����������Ĵ���*/
#define     SPIDAT 	  (volatile unsigned int   *) 0x7049 
/* SPI �������ݼĴ���*/
#define     SPIPC1 	  (volatile unsigned int   *) 0x704D 
/* SPI �˿ڿ��ƼĴ���1*/
#define     SPIPC2 	  (volatile unsigned int   *) 0x704E 
/* SPI�˿ڿ��ƼĴ���2*/
#define     SPIPRI 	  (volatile unsigned int   *) 0x704F 
/* SPI ���ȼ����ƼĴ���*/
/* ����ͨ�ŽӿڼĴ���
��~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define     SCICCR  (volatile unsigned int  *) 0x7050 
/* SCI ͨѶ���ƼĴ���*/
#define     SCICTL1 	  (volatile unsigned int   *) 0x7051 
/* SCI ���ƼĴ���1*/
#define     SCIHBAUD	  (volatile unsigned int   *) 0x7052 
/* SCI �����ʿ��ƼĴ�����λ*/
#define     SCILBAUD	  (volatile unsigned int   *) 0x7053 
/* SCI �����ʿ��ƼĴ�����λ*/
#define     SCICTL2 	  (volatile unsigned int   *) 0x7054 
/* SCI ���ƼĴ���2*/
#define     SCIRXST 	  (volatile unsigned int   *) 0x7055 
/* SCI ����״̬�Ĵ���*/
#define     SCIRXEMU	  (volatile unsigned int   *) 0x7056 
/* SCI EMU ���ݻ���Ĵ���*/
#define     SCIRXBUF  (volatile unsigned int   *) 0x7057 
/* SCI �������ݻ���Ĵ���*/
#define     SCITXBUF  (volatile unsigned int   *) 0x7059 
/* SCI�������ݻ���Ĵ���*/
#define    SCIPC2   (volatile unsigned int   *) 0x705E 
#define     SCIPRI   (volatile unsigned int   *) 0x705F 	
/* SCI ���ȼ����ƼĴ���*/
/* ��Χ�ж����üĴ���
��~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define     XINT1CR 	  (volatile unsigned int   *) 0x7070 
/* Int1 ���üĴ���*/
#define    NMICR      (volatile unsigned int   *) 0x7072 
#define     XINT2CR 	  (volatile unsigned int   *) 0x7078 	
/* Int2 ���üĴ���*/
#define    XINT3CR    (volatile unsigned int   *) 0x707A 
/* ���� I/O ���ƼĴ�����
��~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define     OCRA 	  (volatile unsigned int   *) 0x7090 	
/* I/O ���ÿ��ƼĴ���A*/ 
#define     OCRB 	  (volatile unsigned int   *) 0x7092 
/* I/O ���ÿ��ƼĴ���B*/

#define     PADATDIR     (volatile unsigned int   *) 0x7098 
/* I/O �˿� A ���ݺͷ�����ƼĴ���*/ 
#define     PBDATDIR	  (volatile unsigned int   *) 0x709A 
/* I/O �˿� B ���ݺͷ�����ƼĴ���*/
#define     PCDATDIR	  (volatile unsigned int   *) 0x709C 
/* I/O �˿� C ���ݺͷ�����ƼĴ���*/

/*ADC �Ĵ�������
��--------------------------------------------------------------*/
#define      ADCTRL1 	  (volatile unsigned int   *) 0x7032 
/* ADC���ƼĴ���1*/
#define      ADCTRL2 	  (volatile unsigned int   *) 0x7034 
/* ADC���ƼĴ���2*/ 
#define    ADCFIFO1   (volatile unsigned int   *) 0x7036 
#define    ADCFIFO2   (volatile unsigned int   *) 0x7038 

/*�¼������� (EV*) �¼������� A (EVA*)  �Ĵ���
��~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define     GPTCON      (volatile unsigned int   *) 0x7400 
/* ͨ�ö�ʱ�����ƼĴ���*/
#define     T1CNT 	  (volatile unsigned int   *)  0x7401 	
/* T1 ������*/
#define     T1CMPR 	  (volatile unsigned int   *) 0x7402 
/* T1 �Ƚ�ֵ�Ĵ���*/
#define     T1PR 	  (volatile unsigned int   *) 0x7403 	
/* T1 ���ڼĴ���*/
#define     T1CON 	  (volatile unsigned int   *) 0x7404 
/* T1 ���ƼĴ���*/
#define     T2CNT 	  (volatile unsigned int   *) 0x7405 	
/* T2 ������*/
#define     T2CMPR 	  (volatile unsigned int   *) 0x7406 
/* T2 �Ƚ�ֵ�Ĵ���*/
#define     T2PR 	  (volatile unsigned int   *) 0x7407 	
/* T2 ���ڼĴ���*/
#define     T2CON 	  (volatile unsigned int   *) 0x7408 
/* T2 ���ƼĴ���*/
#define    T3CNT   (volatile unsigned int   *) 0x7409 
#define    T3CMPR   (volatile unsigned int   *) 0x740A 
#define    T3PR   (volatile unsigned int   *) 0x740B 
#define    T3CON   (volatile unsigned int   *) 0x740C 
#define     COMCON 	  (volatile unsigned int   *) 0x7411 	
/* �ȽϿ��ƼĴ���*/
#define     ACTR 	  (volatile unsigned int   *) 0x7413 	
/* �Ƚ�����������ƼĴ���*/
#define    SACTR   (volatile unsigned int   *) 0x7414 
#define     DBTCON	  (volatile unsigned int   *) 0x7415 
/* �������ƼĴ���*/
#define    CMPR1 	  (volatile unsigned int   *) 0x7417 
/* �Ƚ�ֵ1*/
#define    CMPR2 	  (volatile unsigned int   *) 0x7418 
/* �Ƚ�ֵ2*/
#define    CMPR3 	  (volatile unsigned int   *) 0x7419 
/* �Ƚ�ֵ3*/
#define    SCMPR1   (volatile unsigned int   *) 0x741A 
#define    SCMPR2   (volatile unsigned int   *) 0x741B 
#define    SCMPR3   (volatile unsigned int   *) 0x741C 
#define    CAPCON 	  (volatile unsigned int   *) 0x7420 	
/* ��׽���ƼĴ���*/
#define    CAPFIFO	  (volatile unsigned int   *) 0x7422 	
/* ��׽ FIFO A ״̬�Ĵ���*/
#define    CAP1FIFO	  (volatile unsigned int   *) 0x7423 	
/* ��׽1 FIFO ����*/
#define    CAP2FIFO	  (volatile unsigned int   *) 0x7424 	
/* ��׽ 2 FIFO ����*/
#define    CAP3FIFO	  (volatile unsigned int   *) 0x7425 	
/* ��׽ 3 FIFO ����*/
#define    CAP4FIFO   (volatile unsigned int   *) 0x7426 

#define    EVIMRA	  (volatile unsigned int   *) 0x742C 	
/* EVA �ж����μĴ��� A*/
#define    EVIMRB	  (volatile unsigned int   *) 0x742D 	
/* EVA�ж����μĴ��� B*/
#define    EVIMRC	  (volatile unsigned int   *) 0x742E 	
/* EVA �ж����μĴ��� C*/
#define     EVIFRA	  (volatile unsigned int   *) 0x742F 	
/* EVA�жϱ�־�Ĵ��� A*/
#define     EVIFRB	  (volatile unsigned int   *) 0x7430 	
/* EVA �жϱ�־�Ĵ��� B*/
#define     EVIFRC	  (volatile unsigned int   *) 0x7431 	
/* EVA�жϱ�־�Ĵ��� C*/
#define    EVIVRA   (volatile unsigned int   *) 0x7432 
#define    EVIVRB   (volatile unsigned int   *) 0x7433 
#define    EVIVRC   (volatile unsigned int   *) 0x7434 
/*FLASH MODULE REGISTERS(MAPPED INTO PROGRAM SPACE*) 

-----------------------------------------------------------------------------*/
#define    SEG_CTR   (volatile unsigned int   *) 0x0000 
/*FLASH SEGMENT CONTROL REGISTER*/
#define    WADRS   (volatile unsigned int   *) 0x0002 
/*FLASH WRITE ADDRESS REGISTER*/
#define    WDATA   (volatile unsigned int   *) 0x0003 
/*FLASH WRITE DATA REGISTER*
�������Ķ���
��-----------------------------------------------------------------------------
�����ݿռ�
��~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define		B2_SADDR	0x0060	/*�� B2 �Ŀ�ʼ��ַ*/
#define		B2_EADDR	0x007F	/*�� B2 �Ľ�����ַ*/
#define		B0_SADDR	0x0200	/*�� B0 �Ŀ�ʼ��ַ*/
#define		B0_EADDR	0x02FF	/*�� B0 �Ľ�����ַ*/
#define		B1_SADDR	0x0300	/*�� B1 �Ŀ�ʼ��ַ*/
#define		B1_EADDR	0x03FF	/*�� B1 �Ľ�����ַ*/
#define		SARAM		0x0800	/*SARAM �Ŀ�ʼ��ַ(0800h-0FFFh*) */
#define		EXTDATA		0x8000	/*�ⲿ�����ݿռ�Ŀ�ʼ��ַ*/
/*�����õ�������ҳ
��~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define		DP_B2		0	/*ҳ 0  ���ݿռ�*/
#define		DP_B01		4	/*ҳ 4  B0(200H/80H*) */	
#define		DP_B02		5	/*ҳ 5  B0(280H/80H*) */	
#define		DP_B11		6	/*ҳ 6  B1(300H/80H*) */	
#define		DP_B12		7	/*ҳ 7  B1(380H/80H*) */
#define		DP_SARAM1    16	/*ҳ 16 SARAM(800H/80H*) */
#define		DP_PF1	     224     /*ҳ1 ����֡�ļ� (7000h/80h*) (0XE0*) */
#define		DP_PF2	     225    /*ҳ 2 ����֡�ļ� (7080h/80h*) (0XE1*) */
#define		DP_PF3	     226    /*ҳ 3 ����֡�ļ� (7100h/80h*) (0XE2*) */

#define		DP_EV	     232	/*EVA �Ĵ����ļ� (7400h/80h*) (0XE8*) */

/*λ����ָ���λ����(BIT*) 
��~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define		BIT15		0	/*λ���� 15*/
#define		BIT14		1	/*λ����14*/
#define		BIT13		2	/*λ����13*/
#define		BIT12		3	/*λ����12*/
#define		BIT11		4	/*λ����11*/
#define		BIT10		5	/*λ����10*/
#define		BIT9		6	/*λ����9*/
#define		BIT8		7	/*λ����8*/
#define		BIT7		8	/*λ����7*/
#define		BIT6		9	/*λ����6*/
#define		BIT5		10	/*λ����5*/
#define		BIT4		11	/*λ����4*/
#define		BIT3		12	/*λ����3*/
#define		BIT2		13	/*λ����2*/
#define		BIT1		14	/*λ����1*/
#define		BIT0		15	/*λ����0*/
/*��SBIT0��SBIT1������λ
��~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define		B15_MSK		0x8000	/*λ����15*/
#define		B14_MSK		0x4000	/*λ����14*/
#define		B13_MSK		0x2000	/*λ����13*/
#define		B12_MSK		0x1000	/*λ����12*/
#define		B11_MSK		0x0800	/*λ����11*/
#define		B10_MSK		0x0400	/*λ����10*/
#define		B9_MSK		0x0200	/*λ����9*/
#define		B8_MSK		0x0100	/*λ����8*/
#define		B7_MSK		0x0080	/*λ����7*/
#define		B6_MSK		0x0040	/*λ����6*/
#define		B5_MSK		0x0020	/*λ����5*/
#define		B4_MSK		0x0010	/*λ����4*/
#define		B3_MSK		0x0008	/*λ����3*/
#define		B2_MSK		0x0004	/*λ����2*/
#define		B1_MSK		0x0002	/*λ����1*/
#define		B0_MSK		0x0001	/*λ����0*/
/*���Ź���ʱ����λ�ַ��� 
��~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define		wd_rst_1	0x0055
#define		wd_rst_2	0x00AA
/*����Ϊ�û����ж���ı���
��~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

