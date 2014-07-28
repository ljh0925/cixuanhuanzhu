/****************************************Copyright (c)**************************************************
**                               广州周立功单片机发展有限公司
**                                     研    究    所
**                                        产品一部 
**
**                                 http://www.zlgmcu.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: config.h
**创   建   人: 陈明计
**最后修改日期: 2004年2月2日
**描        述: 用户配置文件
**
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 陈明计
** 版  本: 1.0
** 日　期: 2004年2月2日
** 描　述: 原始版本
**
**------------------------------------------------------------------------------------------------------
** 修改人:
** 版  本:
** 日　期:
** 描　述:
**
**--------------当前版本修订------------------------------------------------------------------------------
** 修改人: 
** 日　期:
** 描　述:
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
//这一段无需改动
#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef unsigned char  uint8;                   /* 无符号8位整型变量                        */
typedef signed   char  int8;                    /* 有符号8位整型变量                        */
typedef unsigned short uint16;                  /* 无符号16位整型变量                       */
typedef signed   short int16;                   /* 有符号16位整型变量                       */
typedef unsigned int   uint32;                  /* 无符号32位整型变量                       */
typedef signed   int   int32;                   /* 有符号32位整型变量                       */
typedef float          fp32;                    /* 单精度浮点数（32位长度）                 */
typedef double         fp64;                    /* 双精度浮点数（64位长度）                 */


/********************************/
/*      uC/OS-II的特殊代码      */
/********************************/

#define     USER_USING_MODE    0x10                    /*  用户模式,ARM代码                  */
#define LOCAL_V_TYPE                                                     /*  只能是0x10,0x30,0x1f,0x3f之一       */
#include "Includes.h"

/********************************/
/*      ARM的特殊代码           */
/********************************/
//这一段无需改动

#include    "LPC2294.h"


/********************************/
/*     应用程序配置             */
/********************************/
//以下根据需要改动
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
/*     本例子的配置             */
/********************************/
/* 系统设置, Fosc、Fcclk、Fcco、Fpclk必须定义*/
#define Fosc            11059200                    //晶振频率,10MHz~25MHz，应当与实际一至
//#define Fosc            12000000                    //晶振频率,10MHz~25MHz，应当与实际一至
#define Fcclk           (Fosc * 4)                  //系统频率，必须为Fosc的整数倍(1~32)，且<=60MHZ
#define Fcco            (Fcclk * 4)                 //CCO频率，必须为Fcclk的2、4、8、16倍，范围为156MHz~320MHz
#define Fpclk           (Fcclk / 4) * 1             //VPB时钟频率，只能为(Fcclk / 4)的1 ~ 4倍

#include    "target.h"              //这一句不能删除
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
