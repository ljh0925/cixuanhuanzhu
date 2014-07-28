/******************************************************************************
Copyright 2005 大连金德姆电子有限公司
All rights riserved.

文件名	：task_general_control.c
模块名称：总控制任务程序
功能概要：总控制任务程序

取代版本：0.0.1
修改人	：许岩
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include 		"config.h"


// extern volatile CARD_INFO          CardInfo;				   //卡片信息，交易记录由此变量生成
// extern volatile ERROR_CARD_INFO    ErrCardInfo;	   //出错卡状态

//OS_STK          TaskStartStk[TASK_STK_TCPIP_SIZE * 4];
//OS_STK          TASK_GENCTRL_STK[TASK_STK_TCPIP_SIZE * 10];

//OS_STK          TaskStartStk[TASK_STK_TCPIP_SIZE];
//OS_STK          TASK_GENCTRL_STK[TASK_STK_TCPIP_SIZE];

extern volatile DEV_STAT DevStat;
//extern OS_EVENT        *Beep_Mbox;
//extern OS_EVENT        *Dis_Time_Dly_Mbox;
//extern OS_EVENT        *ICCard_Mbox;

extern volatile INT8U DisplayTimeEN;										//是否允许刷新时间，=FALSE不允许，=TRUE或其他 允许

//extern const  INT8U       ReadPOSNumber[5];                         //取POS号
//extern const  INT8U       SELECT_SAM_ADF1[7];       //选1001目录

//TRANS_STK trans_stk;
//TRANS_STK* ptrans_stk = &trans_stk;
//CAL_STK cal_stk;
//CAL_STK* pcal_stk = &cal_stk;


extern struct       MENU*       curMenu;		//当前菜单
extern struct       MENU        mu[MENU_MAX_ITEM];



extern char * VERSION;			 //开机时显示的版本号


/******************************************************************************
 函数名称：TaskGeneralCtrl
 功能描述：总控制任务
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2005-04-07
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void TaskGeneralCtrl(void * data)
{
	INT8U key = NO_KEY;
	INT8U mode = AUTO;
	INT8U buf[17] = {0};
//	INT16U reg = 0;
//  FP64 temp_fp64 = 0.0;
//	INT32S temp_int32s = 0;

	ToggleWD();

	data = data;		//avoid warning

	//初始化
	InitMenu();
	Lcd_Cls();
	SetBacklightON();

	for ( ;; )
	{
//      ToggleWD();

		OSTimeDlyHMSM(0, 0, 0, 50);

//      if ( DevStat.err_occured == TRUE )
//      {   //如果有设备出错，将显示错误代码，不允许刷卡
//          DisplayErr(DevStat.err_code);
//          continue;
//      }

		OS_ENTER_CRITICAL();
		mode = DevStat.mode;
		OS_EXIT_CRITICAL();
		
		if ( mode == AUTO )
		{
//          strcpy((void *)buf, "自动   ");
			sprintf((void *)buf, "自动%3d", DevStat.reset_times);
		}
		else
		{
//          strcpy((void *)buf, "手动   ");
			sprintf((void *)buf, "手动%3d", DevStat.reset_times);
		}
		strcat((void *)buf, VERSION);
		lcddisp(0, 0, buf);
      	lcddisp(1, 0, "  磁选环柱设备  ");
    	lcddisp(2, 0, "    控制单元    ");
	    lcddisp(3, 0, "  菜单--确认键  ");

//        OS_ENTER_CRITICAL();
//        reg = DevStat.MOD_REG.reg[0x000D];
//        OS_EXIT_CRITICAL();
//        sprintf((void *)buf, "目标厚度:%+7.3f", (FP64)reg / 1000.0);
//        lcddisp(1, 0, buf);
//
//        OS_ENTER_CRITICAL();
////    reg = DevStat.MOD_REG.reg[0x0003];
//        if ( (DevStat.MOD_REG.reg[0x0003] & 0x8000) == 0 )        //看是否为正
//        {
//            temp_int32s = (INT32S)DevStat.MOD_REG.reg[0x0003] * 1000L
//                          + (INT32S)DevStat.MOD_REG.reg[0x0004];
//        }
//        else
//        {
//            temp_int32s = 0 - ((INT32S)(DevStat.MOD_REG.reg[0x0003] & (~0x8000)) * 1000L)
//                          - (INT32S)DevStat.MOD_REG.reg[0x0004];
//        }
//        sprintf((void *)buf, "测量厚度:%+7.3f", _fflt(temp_int32s) / 1000.0);
//        OS_EXIT_CRITICAL();
//        lcddisp(2, 0, buf);
//
////      OS_ENTER_CRITICAL();
////      reg = DevStat.MOD_REG.reg[0x0005];
////      OS_EXIT_CRITICAL();
////      sprintf((void *)buf, " AGC输出:%+6.2fV", (FP64)reg / 1000.0);
////      lcddisp(3, 0, buf);
//
//        OS_ENTER_CRITICAL();
//        reg = DevStat.MOD_REG.reg[0x0005];
//        if ( (reg & 0x8000) == 0 )        //看是否为正
//        {
//            temp_int32s = reg;
//        }
//        else
//        {
//            temp_int32s = 0 - (reg & (~0x8000));
//        }
//        sprintf((void *)buf, "AGC 输出:%+6.2fV", _fflt(temp_int32s) / 100.0);
//        OS_EXIT_CRITICAL();
//        lcddisp(3, 0, buf);


		key = delay_and_wait_key_ms(100, 0, 0);
		switch ( key )
		{
			case KEY_ENTER:					//菜单
				SetupProcess();
				break;

			default:
				break;
		}

	}
}


///*****************************************************************
//函数原型：DisplayErr
//功能描述：显示错误代码
//参数描述：
//参数名称：  输入/输出？ 类型        描述
//err_code    输入            INT16U      错误代码, 错误代码, DevStat.err_code
//
//返  回  值：无
//
//作      者  ：许岩
//日      期：2005-05-19
//修改历史：
//日期        修改人      修改描述
//------      ---------   -------------
//*****************************************************************/
//void DisplayErr(INT16U err_code)
//{
////     INT8U i = 0;
//    static INT8U display_time_dly = 2;
//
//    ToggleWD();
//
//    lcddisp(0, 0, "                ");
//    lcddisp(1, 0, "    系统故障!   ");
//
//    if ( (err_code & READ_CLOCK_ERROR) != 0 )               //读取时钟错误
//    {
//        lcddisp(2, 0, "  时钟芯片故障  ");
//        OSMboxPost(Dis_Time_Dly_Mbox, (void *)&display_time_dly);                   //2秒之内不允许刷新时间任务进行时间刷新
//        SleepMs(1000);
//    }
//
//    if ( (err_code & CLOCK_FORMAT_ERROR) != 0 )             //时钟格式错误
//    {
//        lcddisp(2, 0, "  时钟格式错误  ");
//        OSMboxPost(Dis_Time_Dly_Mbox, (void *)&display_time_dly);                   //2秒之内不允许刷新时间任务进行时间刷新
//        SleepMs(1000);
//    }
//
//    if ( (err_code & RC500_ERROR) != 0 )                //配置读写头错误
//    {
//        lcddisp(2, 0, "  读写头故障    ");
//        OSMboxPost(Dis_Time_Dly_Mbox, (void *)&display_time_dly);                   //2秒之内不允许刷新时间任务进行时间刷新
//        SleepMs(1000);
//    }
//
//    if ( (err_code & FLASH_ERROR) != 0 )                //FLASH错误
//    {
//        lcddisp(2, 0, "  存储芯片故障  ");
//        OSMboxPost(Dis_Time_Dly_Mbox, (void *)&display_time_dly);                   //2秒之内不允许刷新时间任务进行时间刷新
//        SleepMs(1000);
//    }
//
//    if ( (err_code & PARAMETER_ERROR) != 0 )                //参数错误
//    {
//        lcddisp(2, 0, "  读取参数错误  ");
//        OSMboxPost(Dis_Time_Dly_Mbox, (void *)&display_time_dly);                   //2秒之内不允许刷新时间任务进行时间刷新
//        SleepMs(1000);
//    }
//
//    if ( (err_code & ST16C554_ERROR) != 0 )             //16C554错误
//    {
//        lcddisp(2, 0, "  16C554故障");
//        OSMboxPost(Dis_Time_Dly_Mbox, (void *)&display_time_dly);                   //2秒之内不允许刷新时间任务进行时间刷新
//        SleepMs(1000);
//    }
//
//    if ( (err_code & GPRS_ERROR) != 0 )             //GPRS模块故障
//    {
//        lcddisp(2, 0, "  GPRS模块故障");
//        OSMboxPost(Dis_Time_Dly_Mbox, (void *)&display_time_dly);                   //2秒之内不允许刷新时间任务进行时间刷新
//        SleepMs(1000);
//    }
//
//}

// /*****************************************************************
// 函数原型：calc
// 功能描述：键盘操作和显示
// 参数描述：
// 参数名称：	输入/输出？	类型		描述
// price			输出		INT32U*		操作人员输入的价格，单位分
//
// 返  回  值：KEY_ENTER = 有价格输入
// 				KEY_CANCEL = 退出
// 				KEY_MODE = 输入了模式键
// 				KEY_FUNC = 输入了功能键
//
// 作      者	：许岩
// 日      期：2005-10-21
// 修改历史：
// 日期		修改人		修改描述
// ------		---------	-------------
// *****************************************************************/
// INT8U calc(INT32U* price)
// {
// 	INT8U i;
// 	INT8U key;
// 	FP64 sum;										//总金额，单位元
// 	char str[MAX_INPUT_LENGTH + 3];
// 	char translated[MAX_INPUT_LENGTH * 2];			//由str整理成的后缀表达式
// //     char *pstr = str;
// //     char* init = "";
// 	INT8U len = 0;
// 	INT8U point_num = 0;					//小数点的个数，用于防止出现12.34.56的情况
// 	INT8U cancel_times = 0;					//按过几次cancel键
// 	INT8U have_operator = FALSE;
// //     INT8U first_meet_dot = FALSE;
// //     INT8U first_meet_operator = FALSE;
// 	char * endptr;
//
//
// 	for ( ;; )
// 	{
// 		ToggleWD();
// 		key = get_key();
//
// 		switch ( key )
// 		{
// 			case '0':
// 			case '1':
// 			case '2':
// 			case '3':
// 			case '4':
// 			case '5':
// 			case '6':
// 			case '7':
// 			case '8':
// 			case '9':
// 				cancel_times = 0;
// 				if ( len > MAX_INPUT_LENGTH )
// 				{
// 					BeepInputErr();
// 					break;
// 				}
// 				if ( len >= 4 )
// 				{
// 					if ( isdigit(str[len - 1]) != 0 && isdigit(str[len - 2]) !=0 && str[len - 3] == '.' )
// 					{	//小数点后只能输入两位数
// 						BeepInputErr();
// 						break;
// 					}
// 				}
//
// 				if ( len != 0 )
// 				{
// 					if ( len == 1 && str[len - 1] == '0' && key == '0' )
// 					{		//开头输入两个0
// 						BeepInputErr();
// 						break;
// 					}
// 					if ( str[len - 1] == '0' && ( (str[len - 2] == '+') || (str[len - 2] == '-') ||(str[len - 2] == '*') ) && key == '0' )
// 					{	//符号后连续输入两个0
// 						BeepInputErr();
// 						break;
// 					}
// 				}
//
// 				str[len] = key;
// 				len++;
// 				str[len] = '\0';
// 				if ( len <= 16 )
// 				{
// 					LCD_Display_Str(2, 16 - len, str, NORMAL);
// 				}
// 				else
// 				{
// 					LCD_Display_Str(2, 0, str + len - 16, NORMAL);
// 				}
// 				break;
//
// 			case '.':
// 				cancel_times = 0;
// 				if ( (str[len - 1] == '.') || (len > MAX_INPUT_LENGTH) )
// 				{
// 					BeepInputErr();
// 					break;
// 				}
// 				if ( len != 0 )
// 				{
// 					point_num = 0;
// 					for ( i=0; i<len; i++ )
// 					{
// 						if ( (str[len - 1 - i] == '+') || (str[len - 1 - i] == '-') || (str[len - 1 - i] == '*') )
// 							break;
// 						if ( str[len - 1 - i] == '.' )
// 							point_num++;
// 					}
// 					if ( point_num >= 1 )
// 					{	//已经有了一个小数点了
// 						BeepInputErr();
// 						break;
// 					}
// 				}
// 				if ( (len == 0) || (str[len - 1] == '+') || (str[len - 1] == '-') || (str[len - 1] == '*') )
// 				{
// 					str[len] = '0';
// 					len++;
// 				}
// 				str[len] = '.';
// 				len++;
// 				str[len] = '\0';
// 				if ( len <= 16 )
// 				{
// 					LCD_Display_Str(2, 16 - len, str, NORMAL);
// 				}
// 				else
// 				{
// 					LCD_Display_Str(2, 0, str + len - 16, NORMAL);
// 				}
// 				break;
//
// 			case '+':
// 			case '-':
// 			case '*':
// 				cancel_times = 0;
// 				if ( (len == 0) || (str[len - 1] == '+') || (str[len - 1] == '-') || (str[len - 1] == '*')
// 					 || (str[len - 1] == '.') || (len > MAX_INPUT_LENGTH) )
// 				{
// 					BeepInputErr();
// 					break;
// 				}
// 				str[len] = key;
// 				len++;
// 				str[len] = '\0';
// 				if ( len <= 16 )
// 				{
// 					LCD_Display_Str(2, 16 - len, str, NORMAL);
// 				}
// 				else
// 				{
// 					LCD_Display_Str(2, 0, str + len - 16, NORMAL);
// 				}
// 				break;
//
// 			case '=':
// 				cancel_times = 0;
// 				if ( len == 0 )
// 					break;
// 				if ( (str[len - 1] == '+') || (str[len - 1] == '-') || (str[len - 1] == '*') || (str[len - 1] == '.') )
// 				{
// 					BeepInputErr();
// 					break;
// 				}
//
// 				for ( i=0; i<len; i++ )					   //查找输入序列中是否含有运算符
// 				{
// 					if ( str[i] == '*' || str[i] == '+' || str[i] == '-' )
// 					{
// 						have_operator = TRUE;
// 						break;
// 					}
// 				}
// 				if ( i >= len )
// 					have_operator = FALSE;
//
// 				if ( have_operator == TRUE )
// 				{
// 					trans(str, translated);			//由算术表达式整理成后缀表达式
// 					sum = compvalue(translated);		//计算结果
// 					if ( sum >= 0.0 )
// 					{
// 						(void)sprintf((void *)str, "%.2f", sum);
// 						len = strlen(str);
// 						if ( len <= 16 )
// 						{
// 							LCD_Display_Str(2, 0, "                ", NORMAL);
// 							LCD_Display_Str(2, 16 - len, str, NORMAL);
// 						}
// 						else
// 						{
// 							LCD_Display_Str(2, 0, str + len - 16, NORMAL);
// 						}
// 					}
// 					else
// 					{
// 						LCD_Display_Str(2, 0, "                ", NORMAL);
// 						LCD_Display_Str(2, 0, "结果小于0!  非法", NORMAL);
// 						BeepInputErr();
// 						OSTimeDlyHMSM(0, 0, 1, 0);
// 						return KEY_CANCEL;
// 					}
//
// 				}
// 				else
// 					;		//do nothing
// 				break;
//
// 			case KEY_ENTER:		//基本同=键
// 				if ( len == 0 )
// 					break;
// 				if ( (str[len - 1] == '+') || (str[len - 1] == '-') || (str[len - 1] == '*') || (str[len - 1] == '.') )
// 				{
// 					BeepInputErr();
// 					break;
// 				}
//
// 				for ( i=0; i<len; i++ )					   //查找输入序列中是否含有运算符
// 				{
// 					if ( str[i] == '*' || str[i] == '+' || str[i] == '-' )
// 					{
// 						have_operator = TRUE;
// 						break;
// 					}
// 				}
// 				if ( i >= len )
// 					have_operator = FALSE;
//
// 				if ( have_operator == TRUE )
// 				{
// 					trans(str, translated);			//由算术表达式整理成后缀表达式
// 					sum = compvalue(translated);		//计算结果
// 					if ( sum == 0.0 )
// 					{
// 						(void)sprintf((void *)str, "%.2f", sum);
// 						len = strlen(str);
// 						if ( len <= 16 )
// 						{
// 							LCD_Display_Str(2, 0, "                ", NORMAL);
// 							LCD_Display_Str(2, 16 - len, str, NORMAL);
// 						}
// 						else
// 						{
// 							LCD_Display_Str(2, 0, str + len - 16, NORMAL);
// 						}
// 						BeepInputErr();
// 						break;			//0元将不被接受，继续输入
// 					}
// 					else if ( sum > 0.0 )
// 					{
// 						(void)sprintf((void *)str, "%.2f", sum);
// 						len = strlen(str);
// 						if ( len <= 16 )
// 						{
// 							LCD_Display_Str(2, 0, "                ", NORMAL);
// 							LCD_Display_Str(2, 16 - len, str, NORMAL);
// 						}
// 						else
// 						{
// 							LCD_Display_Str(2, 0, str + len - 16, NORMAL);
// 						}
// 						*price = (INT32U)(sum * 100.0);
// 						return KEY_ENTER;
// 					}
// 					else
// 					{
// 						LCD_Display_Str(2, 0, "                ", NORMAL);
// 						LCD_Display_Str(2, 0, "结果小于0!  非法", NORMAL);
// 						BeepInputErr();
// 						OSTimeDlyHMSM(0, 0, 1, 0);
// 						return KEY_CANCEL;
// 					}
//
// 				}
// 				else
// 				{		//没有符号，输入即消费金额
// 					sum = strtod(str, &endptr);
// 					if ( sum == 0.0 )
// 					{
// 						(void)sprintf((void *)str, "%.2f", sum);
// 						len = strlen(str);
// 						if ( len <= 16 )
// 						{
// 							LCD_Display_Str(2, 0, "                ", NORMAL);
// 							LCD_Display_Str(2, 16 - len, str, NORMAL);
// 						}
// 						else
// 						{
// 							LCD_Display_Str(2, 0, str + len - 16, NORMAL);
// 						}
// 						BeepInputErr();
// 						break;			//0元将不被接受，继续输入
// 					}
// 					else if ( sum > 0.0 )
// 					{
// 						(void)sprintf((void *)str, "%.2f", sum);
// 						len = strlen(str);
// 						if ( len <= 16 )
// 						{
// 							LCD_Display_Str(2, 0, "                ", NORMAL);
// 							LCD_Display_Str(2, 16 - len, str, NORMAL);
// 						}
// 						else
// 						{
// 							LCD_Display_Str(2, 0, str + len - 16, NORMAL);
// 						}
// 						*price = (INT32U)(sum * 100.0);
// 						return KEY_ENTER;
// 					}
// 					else
// 					{
// 						LCD_Display_Str(2, 0, "                ", NORMAL);
// 						LCD_Display_Str(2, 0, "结果小于0!  非法", NORMAL);
// 						BeepInputErr();
// 						OSTimeDlyHMSM(0, 0, 1, 0);
// 						return KEY_CANCEL;
// 					}
// 				}
// 				break;
//
// 			case KEY_CANCEL:
// 				if ( cancel_times == 0 )
// 				{	//第一次按cancel键
// 					if ( len == 0 )
// 						return KEY_CANCEL;
// 					cancel_times++;
// 					len--;
// 					str[len] = '\0';
// 					if ( len <= 16 )
// 					{
// 						LCD_Display_Str(2, 0, "                ", NORMAL);
// 						LCD_Display_Str(2, 16 - len, str, NORMAL);
// 					}
// 					else
// 					{
// 						LCD_Display_Str(2, 0, str + len - 16, NORMAL);
// 					}
//
// 				}
// 				else
// 				{	//连按两次cancel键
// 					return KEY_CANCEL;
// 				}
// 				break;
//
// 			case KEY_MODE:
// 				return KEY_MODE;
//
// 			case KEY_FUNC:
// 				return KEY_FUNC;
//
// 			default:
// 				break;
// 		}
//
// 	}
// }
//
// void init_trans_stk(TRANS_STK *s)
// {
// 	s->top = -1;
// }
//
// void push_trans_stk(TRANS_STK *s, char ch)
// {
// 	if ( s->top >= MAX_INPUT_LENGTH )		  //上溢
// 		return;
// 	s->top++;
// 	s->stack[s->top] = ch;
// }
//
// void pop_trans_stk(TRANS_STK *s)
// {
// 	if ( s->top <= -1 )		  //下溢
// 		return;
// 	s->top--;
// }
//
// char gettop_trans_stk(TRANS_STK *s)
// {
// 	if ( s->top <= -1 )		  //栈空
// 		return NULL;
// 	return(s->stack[s->top]);
// }
//
// INT8U empty_trans_stk(TRANS_STK *s)
// {
// 	if ( s->top <= -1 )
// 		return TRUE;
// 	else
// 		return FALSE;
// }
//
// /*****************************************************************
// 函数原型：trans
// 功能描述：将算术表达式转换为后缀表达式
// 参数描述：
// 参数名称：	输入/输出？	类型		描述
// str				输入		char*		需要转换的算术表达式
// ret				输出		char*		转换后的后缀表达式
//
// 返  回  值：无
// 作      者	：许岩
// 日      期：2005-10-21
// 修改历史：
// 日期		修改人		修改描述
// ------		---------	-------------
// *****************************************************************/
// void trans(char* str, char* ret)
// {
// 	INT8U p_str = 0;
// 	INT8U p_ret = 0;
// //     INT16S top = -1;            //堆栈指针，使用堆栈时先+1，然后写入数据
// //     char stk[MAX_INPUT_LENGTH + 3];     //符号使用的堆栈
// 	char ch;
//
// 	init_trans_stk(ptrans_stk);
//
// 	while ( str[p_str] != NULL )
// 	{
// 		ToggleWD();
//
// 		switch ( str[p_str] )
// 		{
// 			case '+':
// 			case '-':
// 				ret[p_ret++] = '#';			//标志着一个数的结束
// 				while ( empty_trans_stk(ptrans_stk) == FALSE )
// 				{
// 					ret[p_ret++] = gettop_trans_stk(ptrans_stk);			//以前的符号都出栈
// 					pop_trans_stk(ptrans_stk);
// 				}
// 				push_trans_stk(ptrans_stk, str[p_str]);			//刚输入的符号入栈
// 				p_str++;
// 				break;
//
// 			case '*':
// 				ret[p_ret++] = '#';			//标志着一个数的结束
// 				while ( ((ch = gettop_trans_stk(ptrans_stk)) == '*') && (empty_trans_stk(ptrans_stk) == FALSE) )
// 				{
// 					ret[p_ret++] = ch;			//栈里的*和/都出栈
// 					pop_trans_stk(ptrans_stk);
// 				}
// 				push_trans_stk(ptrans_stk, str[p_str]);			//刚输入的符号入栈
// 				p_str++;
// 				break;
//
// 			case '0':
// 			case '1':
// 			case '2':
// 			case '3':
// 			case '4':
// 			case '5':
// 			case '6':
// 			case '7':
// 			case '8':
// 			case '9':
// 			case '.':
// 				ret[p_ret++] = str[p_str++];
// 				break;
//
// 			default:
// 				//遇到未知字符，跳过
// 				p_str++;
// 				break;
// 		}
// 	}
// 	if ( isdigit(ret[p_ret - 1]) != 0 )
// 	{	//以数字为结尾
// 		ret[p_ret++] = '#';
// 	}
//
// 	while ( empty_trans_stk(ptrans_stk) == FALSE )
// 	{
// 		ret[p_ret++] = gettop_trans_stk(ptrans_stk);			//以前的符号都出栈
// 		pop_trans_stk(ptrans_stk);
// 	}
//
// 	if ( isdigit(ret[p_ret - 1]) != 0 )
// 	{	//以数字为结尾
// 		ret[p_ret++] = '#';
// 	}
// 	ret[p_ret] = '\0';
//
// }
//
//
// void init_cal_stk(CAL_STK *s)
// {
// 	s->top = -1;
// }
//
// void push_cal_stk(CAL_STK *s, FP64 ch)
// {
// 	if ( s->top >= MAX_INPUT_LENGTH )		  //上溢
// 		return;
// 	s->top++;
// 	s->stack[s->top] = ch;
// }
//
// void pop_cal_stk(CAL_STK *s)
// {
// 	if ( s->top <= -1 )		  //下溢
// 		return;
// 	s->top--;
// }
//
// FP64 gettop_cal_stk(CAL_STK *s)
// {
// 	if ( s->top <= -1 )		  //栈空
// 		return NULL;
// 	return(s->stack[s->top]);
// }
//
// INT8U empty_cal_stk(CAL_STK *s)
// {
// 	if ( s->top <= -1 )
// 		return TRUE;
// 	else
// 		return FALSE;
// }
//
// /*****************************************************************
// 函数原型：compvalue
// 功能描述：输入后缀表达式，计算
// 参数描述：
// 参数名称：	输入/输出？	类型		描述
// trans			输入		char*		后缀表达式
//
// 返  回  值：计算后得到的结果
// 作      者	：许岩
// 日      期：2005-10-21
// 修改历史：
// 日期		修改人		修改描述
// ------		---------	-------------
// *****************************************************************/
// FP64 compvalue(char * trans)
// {
// 	FP64 sum;
// 	FP64 left;
// 	FP64 right;
// 	char ch;
// 	char ch2;
// 	INT8U ptrans = 0;
// 	char num[MAX_INPUT_LENGTH];
// 	INT8U i;
// 	char* endptr;
//
// //     char temp_buf[30];
//
// 	ch = trans[ptrans++];
//
//
// 	init_cal_stk(pcal_stk);
//
// 	while ( ch != NULL )
// 	{
// 		ToggleWD();
//
// 		switch ( ch )
// 		{
// 			case '+':
// 				if ( empty_cal_stk(pcal_stk) != TRUE )
// 				{
// 					right = gettop_cal_stk(pcal_stk);
// 					pop_cal_stk(pcal_stk);
// 					if ( empty_cal_stk(pcal_stk) != TRUE )
// 					{
// 						left = gettop_cal_stk(pcal_stk);
// 						pop_cal_stk(pcal_stk);
// 						sum = left + right;
//
// //                         (void)sprintf((void *)temp_buf, "%.2f", left);
// //                         (void)sprintf((void *)temp_buf, "%.2f", right);
// //                         (void)sprintf((void *)temp_buf, "%.2f", sum);
//
// 						push_cal_stk(pcal_stk, sum);
// 					}
// 					else
// 						return 0.0;				//输入的后缀表达式有问题
// 				}
// 				else
// 					return 0.0;				//输入的后缀表达式有问题
//
// 				break;
//
// 			case '-':
// 				if ( empty_cal_stk(pcal_stk) != TRUE )
// 				{
// 					right = gettop_cal_stk(pcal_stk);
// 					pop_cal_stk(pcal_stk);
// 					if ( empty_cal_stk(pcal_stk) != TRUE )
// 					{
// 						left = gettop_cal_stk(pcal_stk);
// 						pop_cal_stk(pcal_stk);
// 						sum = left - right;
// 						push_cal_stk(pcal_stk, sum);
// 					}
// 					else
// 						return 0.0;				//输入的后缀表达式有问题
// 				}
// 				else
// 					return 0.0;				//输入的后缀表达式有问题
//
// 				break;
//
// 			case '*':
// 				if ( empty_cal_stk(pcal_stk) != TRUE )
// 				{
// 					right = gettop_cal_stk(pcal_stk);
// 					pop_cal_stk(pcal_stk);
// 					if ( empty_cal_stk(pcal_stk) != TRUE )
// 					{
// 						left = gettop_cal_stk(pcal_stk);
// 						pop_cal_stk(pcal_stk);
// 						sum = left * right;
//
// //                         (void)sprintf((void *)temp_buf, "%.2f", left);
// //                         (void)sprintf((void *)temp_buf, "%.2f", right);
// //                         (void)sprintf((void *)temp_buf, "%.2f", sum);
//
// 						push_cal_stk(pcal_stk, sum);
// 					}
// 					else
// 						return 0.0;				//输入的后缀表达式有问题
// 				}
// 				else
// 					return 0.0;				//输入的后缀表达式有问题
//
// 				break;
//
// 			case '0':
// 			case '1':
// 			case '2':
// 			case '3':
// 			case '4':
// 			case '5':
// 			case '6':
// 			case '7':
// 			case '8':
// 			case '9':
// 			case '.':
// 				i = 0;
// 				ch2 = ch;
// 				do
// 				{
// 					num[i++] = ch2;
// 				}
// 				while ( (ch2 = trans[ptrans++]) != '#' );
// 				num[i] = '\0';
//
// 				sum = strtod(num, &endptr);
//
// //                 (void)sprintf((void *)temp_buf, "%.2f", sum);
//
// 				push_cal_stk(pcal_stk, sum);
// 				break;
//
// 			default:
// 				//遇到未知字符，跳过
// 				ptrans++;
// 				break;
// 		}
//
// 		ch = trans[ptrans++];
//
// 	}
//
// 	if ( empty_cal_stk(pcal_stk) != TRUE )
// 	{
// 		sum = gettop_cal_stk(pcal_stk);
// 		return sum;
// 	}
// 	else
// 		return 0.0;
//
// }



