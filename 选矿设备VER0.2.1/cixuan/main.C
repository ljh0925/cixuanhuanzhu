/*大连易达通电子技术有限公司
*********************************************************************************************************
*目前版本：ver0.2.1 修改人：刘及华 
* 
*修改内容:1、增加芯片加密程序 
*   	  2、增加GPRS通许部分
*   	  3、增加远程控制功能
*   	  4、增加开关报警功能
* 		  5、增加实时时钟
* 
*日期：2014年7月18日 
*日期：2014年6月30日 
* 
* 
*目前版本：ver0.0.2 修改人：刘及华 
*修改内容:增加测量铁粉数量的程序，在ADC测试的窗口可以查询目前的电压值 
*日期：2014年2月20日 
* 
*当前版本：ver0.0.1 作者：刘及华 
* 
*********************************************************************************************************
*/

#include "config.h"
#include "../include/cfg_net.h"

char * VERSION = "Ver 0.2.1";			//开机时显示的版本号
/*
*********************************************************************************************************
*                                              CONSTANTS
*********************************************************************************************************
*/


/*define task ID*/
#define         	TASK_START_ID				4                /* Application tasks IDs   */
#define         	TASK_SWITCH0_ID			    5
#define         	TASK_SWITCH1_ID			    6
#define         	TASK_SWITCH2_ID			    7
#define         	TASK_SWITCH3_ID			    8
#define         	TASK_ACTUATOR_ID			9
#define         	TASK_SWITCH5_ID			    10
#define         	TASK_SWITCH6_ID			    11
#define         	TASK_SWITCH7_ID			    12
#define             TASK_CTRL_AGC_ID			13
#define         	TASK_BEEP_ID				14
#define             TASK_COM0_ID				15
#define             TASK_COM1_ID                22
#define             TASK_READ_TLC2543_ID        16
#define         	TASK_WORK_LED_ID			17
#define         	TASK_BACK_LIGHT_CTRL_ID     18
#define				TASK_GENCTRL_ID				19
#define             TASK_NET_ID             	20
#define             TASK_LINK_ID       			21



// /*define task prio*/
#define         	TASK_START_PRIO				4                /* Application tasks priorities */
#define         	TASK_SWITCH0_PRIO		    5
#define         	TASK_SWITCH1_PRIO		    6
#define         	TASK_SWITCH2_PRIO		    7
#define         	TASK_SWITCH3_PRIO		    8
#define         	TASK_ACTUATOR_PRIO		    9
#define         	TASK_SWITCH5_PRIO		    10
#define         	TASK_SWITCH6_PRIO		    11
#define         	TASK_SWITCH7_PRIO		    12
#define             TASK_CTRL_AGC_PRIO			13
#define         	TASK_BEEP_PRIO				14
#define             TASK_COM0_PRIO				15
#define             TASK_COM1_PRIO				22
#define             TASK_READ_TLC2543_PRIO		16
#define         	TASK_WORK_LED_PRIO			17
#define         	TASK_BACK_LIGHT_CTRL_PRIO	18
#define				TASK_GENCTRL_PRIO			19
#define          	TASK_NET_PRIO          		20
#define          	TASK_LINK_PRIO      		21


/*define mux*/
OS_EVENT        		*I2c_Sem;
extern OS_EVENT        *gprs_sem;

OS_EVENT        *FLASH_Sem;
OS_EVENT        *Lcd_Sem;
OS_EVENT        *Rtc_Sem;
OS_EVENT        *Back_Light_Ctrl_Mbox;
OS_EVENT  		*RecPackedFlag; 		
OS_EVENT  		*RecBufOverFlowFlag;	
OS_EVENT  		*RecTcpPackedFlag;	
OS_EVENT  		*RecPingPackedFlag;	


OS_STK          TASK_START_STK[TASK_STK_SIZE];
//OS_STK 			TASK_LINK_STACK[100];
//OS_STK 			TASK_NET_STACK[200];
//OS_STK          TASK_READ_AD7680_STK[400];
OS_STK          TASK_AGC_OUTPUT_STK[200];
//OS_STK          TASK_COM0_STK[TASK_STK_BIG_SIZE];
OS_STK          TASK_COM1_STK[TASK_STK_BIG_SIZE];
OS_STK          TASK_SWITCH0_STK[256];
OS_STK          TASK_SWITCH1_STK[256];
OS_STK          TASK_SWITCH2_STK[256];
//OS_STK          TASK_SWITCH3_STK[256];
//OS_STK          TASK_SWITCH4_STK[256];
//OS_STK          TASK_SWITCH5_STK[256];
//OS_STK          TASK_SWITCH6_STK[256];
//OS_STK          TASK_SWITCH7_STK[256];

OS_STK          TASK_READ_TLC2543_STK[TASK_STK_BIG_SIZE];
OS_STK          TASK_WORK_LED_STK[TASK_STK_TINY_SIZE];
OS_STK          TASK_BACK_LIGHT_CTRL_STK[32];
OS_STK   		TASK_ACTUATOR_STACK[TASK_STK_BIG_SIZE];
OS_STK          TASK_GENCTRL_STK[512];

 
/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

volatile DEV_STAT DevStat;
extern INT8U PageBuf1[512];
extern volatile INT8U DisplayTimeEN;
/*
*********************************************************************************************************
*                                              VARIABLES
*********************************************************************************************************
*/


//TASK_USER_DATA  TaskUserData[7];

//OS_EVENT       *MsgQueue;                             /* Message queue pointer                         */
//void           *MsgQueueTbl[20];                      /* Storage for messages                          */

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
/*********************************************************************************************************
** 函数名称: Start_beep
** 功能描述: 蜂鸣器鸣叫三声
** 输　      入: 无
** 输　      出: 无
** 全局变量: 无
** 调用模块: 
** 作　者: 黄力国
** 日　期: 2005年1月10日
********************************************************************************************************/
void Start_beep(void)
{
	ToggleWD();
	Beep(100);
}
/*****************************************************************
 函数原型：	System_Init()
 功能描述：器件类初始化
 参数描述：					 
 返回值：	无
 作  者：	刘及华
 日  期：	2004-09-17
 修改历史：
*****************************************************************/
void System_Init(void)
{
	INT8U   i;

	ToggleWD();

	SetBell(OFF);

	SleepMs(200);

	Start_beep();		/* 开机后蜂鸣器提示*/
	SleepMs(200);

	/* 全局变量初始化*/

	//DevStat初始化
	memset((void*)&DevStat, 0x00, sizeof(DevStat));
	ReadParam();

	DevStat.err_occured = FALSE;
	DevStat.err_code = 0x0000;
	DevStat.id = 0x10;
	DevStat.mode = AUTO;
	//DevStat.passwd = 0;
	memset((void*)&DevStat.passwd[0], 0x00, 6);
	DevStat.MOD_REG.reg[0x0018] = 0x00;	//禁止继电器动作

	i = query_key_nsnd();
	if ( i == KEY_CANCEL )	 //开机时按下CANCEL键，进入格式化状态
	{
		(void)Get_Passwd();
	}
	
	memset(PageBuf1, 0xFF, sizeof(PageBuf1));

	ReadDevState();

	if(DevStat.reset_times >= 254)
	{
		DevStat.reset_times = 0;
	}
	DevStat.reset_times++;
	
	SaveDevState();

	DisplayTimeEN = TRUE;		//允许刷新时间任务刷新时间

#ifdef QUICK_DEBUG
#else
	DisplayVersion(VERSION);
	DisplayID(DevStat.id);
	SleepMs(1000);
#endif

}


/******************************************************************************
 函数名称：RTCChk
 功能描述：读时钟，看是否能够读出
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：ok(0)-自检成功
				 notok(0xFF)-自检失败
				   
 作      者	：许岩
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT8U RTCChk(void)
{
	INT8U LOCAL_V_TYPE i = 0;
	_TIME LOCAL_V_TYPE ltime;

	ToggleWD();

	i = Get_Time(&ltime);
	if ( i != ok )
	{
		return notok;
	}

	return ok;
}

/******************************************************************************
 函数名称：TimeFormatChk
 功能描述：读时钟，看时钟格式是否正确
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：ok(0)-自检成功
				 notok(0xFF)-自检失败
				   
 作      者	：许岩
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT8U TimeFormatChk (void)
{
	INT8U LOCAL_V_TYPE i = 0;
	_TIME LOCAL_V_TYPE ltime;

	ToggleWD();

	i = Get_Time(&ltime);
	if ( i != ok )
	{
		return notok;
	}

	i = CheckTimeFormat(&ltime);
	return i;
}

/*****************************************************************
 函数原型：	System_Init()
 功能描述：器件类初始化
 参数描述：	
				 
 返回值：	无
 
 作  者：	大连现代/刘及华
 日  期：	2004-09-17
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
*****************************************************************/
INT8U System_Check(void)
{
	int i=0;

	ToggleWD();
	Lcd_Cls();
	lcddisp(1, 1, "系统自检中...");
	Display_Date_And_Time();
	SleepMs(500);


	///////////////////////////////////////////////////////////////////////
	//						real time clock read Check										
	///////////////////////////////////////////////////////////////////////
	i = RTCChk();
	if ( i != ok )
	{
		DevStat.err_occured = TRUE;
		DevStat.err_code += (INT16U)READ_CLOCK_ERROR;		  //发生读时钟芯片错误
	}

	///////////////////////////////////////////////////////////////////////
	//						time format Check	
	///////////////////////////////////////////////////////////////////////
	i = TimeFormatChk();
	if ( i != ok )
	{
		DevStat.err_occured = TRUE;
		DevStat.err_code += (INT16U)CLOCK_FORMAT_ERROR;		  //发生时钟格式错误
	}
	if ( DevStat.err_occured == TRUE )
		return notok;

	return ok;
}
/*********************************************************************************************************
** 函数名称: taskstart creates other task
** 功能描述: μCOS-II建立其它任务
** 输　入: 无
** 输　出: 无
** 全局变量: 无
** 调用模块: 
**
** 作　者: 黄力国
** 日　期: 2004年12月28日
**-------------------------------------------------------------------------------------------------------
** 修改人: 
** 日　期: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void  Create_Mux_Box(void)
{
	ToggleWD();

	/* I2c_bus uses*/
	I2c_Sem=OSSemCreate(1);
	FLASH_Sem = OSSemCreate(1);			/* 信号量用于互斥操作总线 */
	Lcd_Sem = OSSemCreate(1);
	gprs_sem = OSSemCreate(1);			/* 信号量用于互斥操作总线 */
	Rtc_Sem = OSSemCreate(1);
//  RecPackedFlag=		OSSemCreate(0);
//  RecBufOverFlowFlag=	OSSemCreate(0);
//  RecTcpPackedFlag=	OSSemCreate(0);
//  RecPingPackedFlag=	OSSemCreate(0);
//  SendFlag=		    OSSemCreate(1);
//
//  //RecIcmpQFlag=		OSQCreate(&RecIcmpQ[0],Q_Max_Size);
//  RecTcpQFlag=		OSQCreate(&RecTcpQ[0],Q_Max_Size);
//  RecUdpQFlag=		OSQCreate(&RecUdpQ[0],Q_Max_Size);
	Back_Light_Ctrl_Mbox = OSMboxCreate((void *)0);		//建立通讯时对时间显示和刷卡使能进行控制任务所需要的邮箱

}
/******************************************************************************
函数名称：DisplayVersion
功能描述：开机显示版本号
参数描述：
参数名称：	输入/输出类型		描述
version	输入		const char*		要显示的版本号，例如输入0x00 0x01 则显示0.0.1

返  回  值：无
				  
作      者	：刘及华
日      期：2004-09-02
修改历史：
	   日期		修改人		修改描述
	   ------		---------	-------------
******************************************************************************/
void DisplayVersion(const char * version)
{
	ToggleWD();

	Lcd_Cls();
#ifdef DEBUG
	lcddisp(1, 0, "测试");
#else
#endif
	lcddisp(1, 2, VERSION);

}

/*****************************************************************
函数原型：DisplayID
功能描述：显示ID号
参数描述：无
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
返  回  值：无
作      者	：刘及华

日      期	：2005-5-23
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void DisplayID(INT32U id)
{
//     INT8U i = 0;
	INT8S buf[50];

	ToggleWD();

	lcddisp(2, 1, "ID号: ");

	buf[0] = ascii(id / 100000);
	buf[1] = ascii((id % 100000) / 10000);
	buf[2] = ascii((id % 10000) / 1000);
	buf[3] = ascii((id % 1000) / 100);
	buf[4] = ascii((id % 100) / 10);
	buf[5] = ascii(id % 10);
	buf[6] = '\0';
	lcddisp(2, 4, buf);
	return;
}
/******************************************************************************
 函数名称：InitImportantParameter
 功能描述：初始化关键参数
 参数描述：
 参数名称：	输入/输出？	类型		描述

 返  回  值：FLOK(0)-写入成功
				 FLNOTOK(0xFF)-前100页全废
				PRI	-	主FLASH失效
				SLV	-	从FLASH失效

 作      者	：刘及华
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT8U InitImportantParameter(void)
{
//	INT8U i = 0;

	ToggleWD();

	OS_ENTER_CRITICAL();
	(void)memset((void *)&DevStat, 0x00, sizeof(DevStat));			//参数全部初始化

	DevStat.err_occured = FALSE;
	DevStat.err_code = 0x0000;
	DevStat.id = 0x10;
	DevStat.mode = AUTO;
	//DevStat.passwd = 0;
	memset((void*)&DevStat.passwd[0], 0x00, 6);

//  DevStat.MOD_REG.reg[0x0009] = 1000;
//  DevStat.MOD_REG.reg[0x000B] = 1000;
	//测试表
//	DevStat.MOD_REG.reg[0x0010] = 0xFFFF;
//	DevStat.MOD_REG.reg[0x0011] = 0x0000;
//	DevStat.MOD_REG.reg[0x0012] = 0xCBD9;
//	DevStat.MOD_REG.reg[0x0013] = 0x0082;
//	DevStat.MOD_REG.reg[0x0014] = 0xA5EC;
//	DevStat.MOD_REG.reg[0x0015] = 0x0104;
//	DevStat.MOD_REG.reg[0x0016] = 0x8837;
//	DevStat.MOD_REG.reg[0x0017] = 0x0186;

	OS_EXIT_CRITICAL();

	WriteParam();

	memset(PageBuf1, 0xFF, sizeof(PageBuf1));
//  FlashPageRead512(10, PageBuf1, 0);
//  memcpy((void *)&DevStat.reset_times, (void *)PageBuf1, sizeof(DevStat.reset_times));
	DevStat.reset_times = 0;
	memcpy((void *)PageBuf1, (void *)&DevStat.reset_times, sizeof(DevStat.reset_times));
	FlashPageWrite512(10, PageBuf1, 0);

	return ok;
}

/******************************************************************************
 函数名称：Get_Passwd
 功能描述：验证密码进入设置POS号状态
 参数描述：
 参数名称：	输入/输出？	类型		描述

 返  回  值：ok(0)-验证成功
				 notok(0xFF)-验证失败

 作      者	：刘及华
 日      期：2005-05-20
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT8U Get_Passwd(void)
{
	INT8U i;
	INT8U key_seq[20];
	INT8S *password = (INT8S *)"22995757";
	INT8U buf[20];

	ToggleWD();

	Lcd_Cls();
	lcddisp(1, 1, "请输入密码: ");

	Beep(50);
	SleepMs(50);
	Beep(50);
	SleepMs(50);
	Beep(50);
	SleepMs(1000);

	for ( i=0; i<8; i++ )			   //取得8个按键输入
	{
		do				   			   //only accept 0-9, '+', '-'
		{
			key_seq[i] = get_key();
		}while ( !( (key_seq[i]>='0' && key_seq[i]<='9') ) );

		buf[strlen((void *)buf) - 1] = '\0';
		strcat((void *)buf, (void *)"*_");
// 		LCD_Display_Str(2, 0, buf, NORMAL);
		lcddisp(2, 1, buf);

//      OSTimeDlyHMSM(0, 0, 0, 150);
		OSTimeDlyHMSM(0, 0, 0, 100);
	}

	key_seq[i] = '\0';

	if ( strcmp((void *)key_seq, (void *)password) != 0 )	//密码错误
	{
		lcddisp(2, 2, "密码错误");
		OSTimeDlyHMSM(0, 0, 1, 0);
		return notok;
	}

	lcddisp(2, 2, "密码正确");
	lcddisp(3, 0, "执行格式化操作..");

	OS_ENTER_CRITICAL();

	InitImportantParameter();

	OS_EXIT_CRITICAL();

	OSTimeDlyHMSM(0, 0, 1, 0);		//为了显示能看清

	resetpos();

	return ok;
}
/*********************************************************************************************************
** 函数名称: TaskStart
** 功能描述: μCOS-II的第一个任务，通常由它初始化目标板和建立其它任务
** 输　入: 无
** 输　出: 无
** 全局变量: 无
** 调用模块: 
**
** 作　者: 陈明计
** 日　期: 2003年7月8日
**-------------------------------------------------------------------------------------------------------
** 修改人: 
** 日　期: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void Task_Start(void *pdata)  
{ /*在第1个任务中设置定时器0及中断*/
    //UBYTE iii;
    INT8U i;
    #if OS_CRITICAL_METHOD == 3              /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
	#endif
	
	
	ToggleWD();

	pdata = pdata;							/* 避免编译警告 */
	
	Create_Mux_Box();						/* 创建任务需要的信号量*/
	TargetInit();						    /* 目标板初始化 */
	System_Init();

	i = System_Check();							/* 系统自检*/
	if ( i != ok )		  //自检失败处理
	{
		//设备异常
		Beep(500);
	}

	TaskStart_CreateTasks();				/* start任务创建任务*/
                 
//	SetNetPort();
//	InitNic(0);			//RTL8019芯片初始化,在global.c中定义
//	Initial_arp();
//	Tcp_Initial();
//	Udp_Initial();
	
//  	while(1)
//	{ 
//		OSTimeDly(4);
//		do
//		{
//		    iii=Rec_Packet();
//		}
//		while(iii!=0);
//	}


	OSTaskSuspend(OS_PRIO_SELF);
	for ( ;; )
	{
		resetpos();
	}

}


/*********************************************************************************************************
** 函数名称: taskstart creates other task
** 功能描述: μCOS-II建立其它任务
** 输　入: 无
** 输　出: 无
** 全局变量: 无
** 调用模块: 
**
** 作　者: 黄力国
** 日　期: 2004年12月28日
**-------------------------------------------------------------------------------------------------------
** 修改人: 
** 日　期: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void TaskStart_CreateTasks(void)
{
//	INT8U i = 0;

	ToggleWD();


	/*task 0*/
    //建立串口1接收GPRS处理任务
	OSTaskCreateExt(Task_Com0,
					(void *)0,
					&TASK_COM0_STK[TASK_STK_BIG_SIZE - 1],
					TASK_COM0_PRIO,
					TASK_COM0_ID,
					&TASK_COM0_STK[0],
					TASK_STK_BIG_SIZE,
					(void *)0,
					OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);


    /*task 1*/
    //建立串口1接收GPRS处理任务
	OSTaskCreateExt(Task_Com1,
					(void *)0,
					&TASK_COM1_STK[TASK_STK_BIG_SIZE - 1],
					TASK_COM1_PRIO,
					TASK_COM1_ID,
					&TASK_COM1_STK[0],
					TASK_STK_BIG_SIZE,
					(void *)0,
					OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

    /*task 2*/
    //建立AGC输出任务
	OSTaskCreateExt(Task_Agc_Output,
				  (void *)0,
				  &TASK_AGC_OUTPUT_STK[200 - 1],
				  TASK_CTRL_AGC_PRIO,
				  TASK_CTRL_AGC_ID,
				  &TASK_AGC_OUTPUT_STK[0],
				  200,
				  (void *)0,
				  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);


    /*task 6*/
    //建立读取DO任务
	OSTaskCreateExt(Task_Switch0,
				  (void *)0,
				  &TASK_SWITCH0_STK[256 - 1],
				  TASK_SWITCH0_PRIO,
				  TASK_SWITCH0_ID,
				  &TASK_SWITCH0_STK[0],
				  256,
				  (void *)0,
				  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	OSTaskCreateExt(Task_Switch1,
				  (void *)0,
				  &TASK_SWITCH1_STK[256 - 1],
				  TASK_SWITCH1_PRIO,
				  TASK_SWITCH1_ID,
				  &TASK_SWITCH1_STK[0],
				  256,
				  (void *)0,
				  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	OSTaskCreateExt(Task_Switch2,
				  (void *)0,
				  &TASK_SWITCH2_STK[256 - 1],
				  TASK_SWITCH2_PRIO,
				  TASK_SWITCH2_ID,
				  &TASK_SWITCH2_STK[0],
				  256,
				  (void *)0,
				  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

    /*task 7*/
    //建立读取AD值任务
 	OSTaskCreateExt(Task_Read_TLC2543,
				 (void *)0,
				 &TASK_READ_TLC2543_STK[TASK_STK_SMALL_SIZE - 1],
				 TASK_READ_TLC2543_PRIO,
				 TASK_READ_TLC2543_ID,
				 &TASK_READ_TLC2543_STK[0],
				 TASK_STK_SMALL_SIZE,
				 (void *)0,
				 OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

//	OSTaskCreateExt(Task_Link,
//					(void *)0,
//					&TASK_LINK_STACK[99],
//					TASK_LINK_PRIO,
//					TASK_LINK_ID,
//					&TASK_LINK_STACK[0],
//					100,
//					(void *)0,//&TaskUserData[TASK_4_ID]
//					OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
//	OSTaskCreateExt(Task_Net,
//					(void *)0,
//					&TASK_NET_STACK[199],
//					TASK_NET_PRIO,
//					TASK_NET_ID,
//					&TASK_NET_STACK[0],
//					200,
//					(void *)0,//&TaskUserData[TASK_4_ID]
//					OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);


	OSTaskCreateExt(TaskGeneralCtrl,
					(void *)0,
					&TASK_GENCTRL_STK[511],
					TASK_GENCTRL_PRIO,
					TASK_GENCTRL_ID,
					&TASK_GENCTRL_STK[0],
					512,
					(void *)0,//&TaskUserData[TASK_4_ID]
					OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	OSTaskCreateExt(Task_Work_Led,
					(void *)0,
					&TASK_WORK_LED_STK[32 - 1],
					TASK_WORK_LED_PRIO,
					TASK_WORK_LED_ID,
					&TASK_WORK_LED_STK[0],
					32,
					(void *)0,
					OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	OSTaskCreateExt(Task_Back_Light_Ctrl,
					(void *)0,
					&TASK_BACK_LIGHT_CTRL_STK[31],
					TASK_BACK_LIGHT_CTRL_PRIO,
					TASK_BACK_LIGHT_CTRL_ID,
					&TASK_BACK_LIGHT_CTRL_STK[0],
					32,
					(void *)0,
					OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	//com1_shell任务
	OSTaskCreateExt(Task_actuator,
					(void *)0,
					&TASK_ACTUATOR_STACK[TASK_STK_BIG_SIZE-1],
					TASK_ACTUATOR_PRIO,
					TASK_ACTUATOR_ID,
					&TASK_ACTUATOR_STACK[0],
					TASK_STK_BIG_SIZE,
					(void *)0,
					OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

}


/*$PAGE*/
/*
*********************************************************************************************************
*                                                  MAIN
*********************************************************************************************************
*/
int main (void)
{   
	
    OSInit();                                   /* Initialize uC/OS-II                      */
    OSTaskCreateExt(Task_Start,
                    (void *)0,
                    &TASK_START_STK[127],
                    TASK_START_PRIO,
                    TASK_START_ID,
                    &TASK_START_STK[0],
                    300,
                    (void *)0,//&TaskUserData[TASK_4_ID]
                    0);
        
//	RecPackedFlag=		OSSemCreate(0);
//  RecBufOverFlowFlag=	OSSemCreate(0);
//  RecTcpPackedFlag=	OSSemCreate(0);
//  RecPingPackedFlag=	OSSemCreate(0);
//  SendFlag=		    OSSemCreate(1);
//
//  RecIcmpQFlag=		OSQCreate(&RecIcmpQ[0],Q_Max_Size);
//  RecTcpQFlag=		OSQCreate(&RecTcpQ[0],Q_Max_Size);
//  RecUdpQFlag=		OSQCreate(&RecUdpQ[0],Q_Max_Size);

    OSStart();                                  /* Start multitasking                       */
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                               STARTUP TASK
*********************************************************************************************************
*/
        void Timer1Init(void)
{
//  OS_ENTER_CRITICAL();
//  	T1IR = 0xffffffff;
//  	T1TC = 0;
//  	T1TCR = 0x01;
//  	T1MCR = 0x03;
//  	T1MR0 = 0xffffff;//(Fpclk / OS_TICKS_PER_SEC);
//  OS_EXIT_CRITICAL();

	T1IR = 0xffffffff;
	T1TC = 0;
	T1TCR = 0x01;
	T1MCR = 0x03;
	T1MR0 = (Fpclk / OS_TICKS_PER_SEC);
}
    
//        void Timer1(void)
//{
//	static 	uint8 	 m_10sec=0;
//	static 	uint8 	 p_sec=0;
//	static 	uint8	 p_min=0;
//	uint8 num;
//    OS_ENTER_CRITICAL();
//    	T1IR = 0x01;
////    T0MR0 += (Fpclk / OS_TICKS_PER_SEC);
//	m_10sec++;
//	if(m_10sec==100)
//	{
//		p_sec++;
//		m_10sec=0;
//		if(Ping_IP_TTL>0)
//			Ping_IP_TTL--;
//		for(num=0;num<MAX_NET_PORT;num++)
//		{
//			for(p_min=0;p_min<MAX_ARP_TERM;p_min++)
//			{
//				if(NetPort[num].ARP_TERM[p_min].TTL>0)
//					NetPort[num].ARP_TERM[p_min].TTL--;
//			}
//			if(Gateway_IP_TTL>0)
//				NetPort[num].Gateway_IP_TTL--;
//		}
//		if(p_sec==60)
//		{
//			p_min++;
//			p_sec=0;
//			if(p_min==60)
//			{
//				p_min=0;
//			}
//		}
//	}
//    	VICVectAddr = 0;            // 通知中断控制器中断结束
//    	OS_EXIT_CRITICAL();
//    //OSTimeTick();
//}
