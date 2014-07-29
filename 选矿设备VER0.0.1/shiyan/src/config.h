/****************************************Copyright (c)**************************************************
**                               ������������Ƭ����չ���޹�˾
**                                     ��    ��    ��
**                                        ��Ʒһ�� 
**
**                                 http://www.zlgmcu.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: config.h
**��   ��   ��: ������
**����޸�����: 2004��2��2��
**��        ��: �û������ļ�
**
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: ������
** ��  ��: 1.0
** �ա���: 2004��2��2��
** �衡��: ԭʼ�汾
**
**------------------------------------------------------------------------------------------------------
** �޸���:
** ��  ��:
** �ա���:
** �衡��:
**
**--------------��ǰ�汾�޶�------------------------------------------------------------------------------
** �޸���: 
** �ա���:
** �衡��:
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
//��һ������Ķ�
#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef unsigned char  uint8;                   /* �޷���8λ���ͱ���                        */
typedef signed   char  int8;                    /* �з���8λ���ͱ���                        */
typedef unsigned short uint16;                  /* �޷���16λ���ͱ���                       */
typedef signed   short int16;                   /* �з���16λ���ͱ���                       */
typedef unsigned int   uint32;                  /* �޷���32λ���ͱ���                       */
typedef signed   int   int32;                   /* �з���32λ���ͱ���                       */
typedef float          fp32;                    /* �����ȸ�������32λ���ȣ�                 */
typedef double         fp64;                    /* ˫���ȸ�������64λ���ȣ�                 */


/********************************/
/*      uC/OS-II���������      */
/********************************/

#define     USER_USING_MODE    0x10                    /*  �û�ģʽ,ARM����                  */
#define LOCAL_V_TYPE                                                     /*  ֻ����0x10,0x30,0x1f,0x3f֮һ       */
#include "Includes.h"

/********************************/
/*      ARM���������           */
/********************************/
//��һ������Ķ�

#include    "LPC2294.h"


/********************************/
/*     Ӧ�ó�������             */
/********************************/
//���¸�����Ҫ�Ķ�
#include    <stdio.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <setjmp.h>
#include    <rt_misc.h>
#include	<rt_fp.h>

//task head
#include    "main.h"
#include    "task_work_led.h"
#include    "task_net.h"
#include	"task_buzzer.h"
#include    "task_general_control.h"
#include	"menu_and_setting.h"
#include    "task_display_time.h"
#include	"task_back_light_ctrl.h"
#include    "task_com0.h"
//#include    "task_com1.h"
#include	"task_agc_output.h"
#include    "task_switch.h"
#include	"task_read_tlc2543.h"


//driver head 
#include    "drv_workled.h"
#include	"drv_delay.h"
#include	"drv_buzzer.h"
#include	"drv_display.h"
#include	"drv_flash.h"
#include	"drv_keyboard.h"
#include	"drv_i2c.h"
#include    "Uart_driver.h"
#include	"drv_ltc1448.h"
#include	"drv_tlc2543.h"
#include	"drv_dido.h"
//#include	"drv_ad5530.h"





/********************************/
/*     �����ӵ�����             */
/********************************/
/* ϵͳ����, Fosc��Fcclk��Fcco��Fpclk���붨��*/
#define Fosc            11059200                    //����Ƶ��,10MHz~25MHz��Ӧ����ʵ��һ��
//#define Fosc            12000000                    //����Ƶ��,10MHz~25MHz��Ӧ����ʵ��һ��
#define Fcclk           (Fosc * 4)                  //ϵͳƵ�ʣ�����ΪFosc��������(1~32)����<=60MHZ
#define Fcco            (Fcclk * 4)                 //CCOƵ�ʣ�����ΪFcclk��2��4��8��16������ΧΪ156MHz~320MHz
#define Fpclk           (Fcclk / 4) * 1             //VPBʱ��Ƶ�ʣ�ֻ��Ϊ(Fcclk / 4)��1 ~ 4��

#include    "target.h"              //��һ�䲻��ɾ��
#include    "..\..\Arm_Pc\pc.h"
/*define	pin	macro*/
#define	BIT0	0X00000001
#define	BIT1	0X00000002
#define	BIT2	0X00000004
#define	BIT3	0X00000008

#define	BIT4	0X00000010
#define	BIT5	0X00000020
#define	BIT6	0X00000040
#define	BIT7	0X00000080

#define	BIT8	0X00000100
#define	BIT9	0X00000200
#define	BIT10	0X00000400
#define	BIT11	0X00000800

#define	BIT12	0X00001000
#define	BIT13	0X00002000
#define	BIT14	0X00004000
#define	BIT15	0X00008000

#define	BIT16	0X00010000
#define	BIT17	0X00020000
#define	BIT18	0X00040000
#define	BIT19	0X00080000

#define	BIT20	0X00100000
#define	BIT21	0X00200000
#define	BIT22	0X00400000
#define	BIT23	0X00800000

#define	BIT24	0X01000000
#define	BIT25	0X02000000
#define	BIT26	0X04000000
#define	BIT27	0X08000000

#define	BIT28	0X10000000
#define	BIT29	0X20000000
#define	BIT30	0X40000000
#define	BIT31	0X80000000

#define BANK0   0x80000000
#define BANK1   0x81000000
#define BANK2   0x82000000
#define BANK3   0x83000000
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
