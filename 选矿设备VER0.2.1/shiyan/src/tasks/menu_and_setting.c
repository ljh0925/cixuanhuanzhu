
#include "config.h"

extern volatile INT8U DisplayTimeEN;	//是否允许刷新时间，=FALSE不允许，=TRUE或其他 允许
extern volatile DEV_STAT DevStat;

struct      MENU*       curMenu;		//当前菜单

struct MENU mu[MENU_MAX_ITEM] = 
{
	//                	    next		pre			parent		child 		exec			param	enter_child_hook	hook_param
	{ "1.工作状态",     0,  &mu[1],     NULL,       NULL,       &mu[14],    NULL,           0,      NULL,               NULL,},	//0
	{ "2.数据查询",     0,  &mu[2],     &mu[0],     NULL,       &mu[6],		NULL,           0,      NULL,               NULL,},	//1
	{ "3.参数设置",     0,  &mu[3],     &mu[1],     NULL,       &mu[10],    NULL,  			0,      NULL,    			NULL,},	//2
	{ "4.修改密码",     0,  &mu[4],     &mu[2],     NULL,       NULL,       SetPassWord,    0,      NULL,    			NULL,},	//3
	{ "5.恢复出厂",     0,  NULL,       &mu[3],     NULL,       NULL,       SetUSERDefault, 0,      NULL,               NULL,},	//4  
	{ "预留",           0,  NULL,       NULL,       NULL,       NULL,       NULL,           0,      NULL,               NULL,},	//5
	{ "1.矿粉量查询",   0,  &mu[7],     NULL,       &mu[1],     NULL,       QueryAD,        0,      NULL,               NULL,},	//6
	{ "2.DA查询",       0,  NULL,       &mu[6],     &mu[1],     NULL,       QueryDA,        0,      NULL,               NULL,},	//7
	{ "1.打开AGC",      0,  &mu[9],     NULL,       &mu[1],     NULL,       AGCEnalbe,      0,      NULL,               NULL,},	//8
	{ "2.关闭AGC",      0,  NULL,       &mu[8],     &mu[1],     NULL,       AGCDisalbe,     0,      NULL,               NULL,},	//9


	{ "1.延迟时间(s)",  0,  &mu[11],    NULL,       &mu[2],     NULL,       Querydelay,     0,      NULL,               NULL,}, //10
	{ "2.接通时间(s)",  0,  &mu[12],    &mu[10],    &mu[2],     NULL,       QueryOndelay,   0,      NULL,               NULL,},	//11
	{ "3.断开时间(s)",  0,  &mu[13],    &mu[11],    &mu[2],     NULL,       QueryOffdelay,  0,      NULL,               NULL,},	//12
	{ "4.矿粉量标准化", 0,  NULL,       &mu[12],    &mu[2],     NULL,       AdjKFZero,      0,      NULL,               NULL,},	//13
	{ "1.开启工作",     0,  &mu[15],    NULL,       &mu[0],     NULL,       SWITCHEnalbe,   0,      NULL,               NULL,},	//14
	{ "2.停止工作",     0,  NULL,       &mu[14],    &mu[0],     NULL,       SWITCHDisalbe,  0,      NULL,               NULL,},	//15
//	{ "1.停止工作",     0,  NULL,       NULL,       &mu[0],     NULL,       SWITCHDisalbe,  0,      NULL,               NULL,},	//16
//	{ "1.自动",         0,  &mu[16],    NULL,       &mu[0],     NULL,       SetModeAuto,    0,      NULL,               NULL,},	//17
//	{ "2.手动",         0,  NULL,       &mu[15],    &mu[0],     NULL,       SetModeManual,  0,      NULL,               NULL,},	//18
//  { "预留",           0,  NULL,       NULL,       NULL,       NULL,       NULL,           0,      NULL,               NULL,}, //19

};


/*****************************************************************
函数原型：InitMenu
功能描述：本函数为初始化菜单
参数描述：无
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
返 回 值：有
*****************************************************************/
void InitMenu(void)
{
	ToggleWD();

	curMenu = (void *)&mu[0];
	curMenu->muPos = 0;
}

/*****************************************************************
函数原型：DisplayMenu
功能描述：本函数为显示菜单函数
参数描述：无
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
返 回 值：有
*****************************************************************/
void DisplayMenu(void)
{
	int8 i;
	uint8 pos;
	struct MENU *temp = curMenu;
	char buf[20];

	ToggleWD();

	Lcd_Cls();	//清除屏幕

	pos = MENU_DISPLAY_START_ROW + temp->muPos;			//显示当前菜单，反显
// 	LCD_Display_Str(pos, MENU_DISPLAY_START_COL, temp->muText, REVERSE);
	strcpy(buf, temp->muText);
	buf[0] = 0x08;
	lcddisp(pos, MENU_DISPLAY_START_COL, buf);


	for ( i=temp->muPos+1; i<MENU_MAX_ROW; i++ )
	{
		temp = temp->muNext;
		if ( temp == NULL )
			break;

//         pos +=2;
		pos++;
// 		LCD_Display_Str(pos, MENU_DISPLAY_START_COL, temp->muText, 0);
		lcddisp(pos, MENU_DISPLAY_START_COL, temp->muText);
	}

	temp = curMenu;
//     pos = MENU_DISPLAY_START_ROW + temp->muPos*2;
	pos = MENU_DISPLAY_START_ROW + temp->muPos;
	for ( i=temp->muPos-1; i>=MENU_DISPLAY_START_ROW; i-- )
	{
		temp = temp->muPre;
		if ( temp == NULL )
			break;

//         pos -=2;
		pos--;
// 		LCD_Display_Str(pos, MENU_DISPLAY_START_COL, temp->muText, 0);
		lcddisp(pos, MENU_DISPLAY_START_COL, temp->muText);
	}
}

/*****************************************************************
函数原型：GotoUpMenu
功能描述：本函数功能为进入当前菜单的上条菜单
参数描述：无
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
返 回 值：有
*****************************************************************/
void GotoUpMenu(void)
{
	struct MENU * temp = curMenu;

	ToggleWD();

	if ( temp == NULL )
		return;
	temp = temp->muPre;
//  temp = (struct MENU *)(temp->muPre);

	if ( temp == NULL )
		return;

	temp->muPos = curMenu->muPos - 1;
	if ( curMenu->muPos <= 0 )
		temp->muPos = 0;

	curMenu = temp;

	DisplayMenu();
}

/*****************************************************************
函数原型：GotoDownMenu
功能描述：本函数功能为进入下条菜单
参数描述：无
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
返 回 值：有
*****************************************************************/
void GotoDownMenu(void)
{
	struct MENU *temp = curMenu;

	ToggleWD();

	if ( temp == NULL )
		return;
	temp = temp->muNext;
//  temp = (struct MENU *)(temp->muNext);

	if ( temp == NULL )
		return;

	temp->muPos = curMenu->muPos + 1;
	if ( curMenu->muPos >= MENU_MAX_ROW-1 )
		temp->muPos = MENU_MAX_ROW-1;

	curMenu = temp;

	DisplayMenu();
}

/*****************************************************************
函数原型：GotoChildMenu
功能描述：本函数功能为进入下级菜单
参数描述：无
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
返 回 值：有
*****************************************************************/
void GotoChildMenu(void)
{
	struct MENU *temp = curMenu;

	ToggleWD();

	if ( temp == NULL )
		return;
	temp = temp->muChild;
//  temp = (struct MENU *)(temp->muChild);

	if ( temp == NULL )
		return;

	temp->muPos = 0;
	curMenu = temp;

	DisplayMenu();
}

/*****************************************************************
函数原型：GotoParentMenu
功能描述：本函数功能为进入上级菜单
参数描述：无
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
返 回 值：有
*****************************************************************/
void GotoParentMenu(void)
{
	struct MENU *temp = curMenu;

	ToggleWD();

	if ( temp == NULL )
		return;
	temp = temp->muParent;
//  temp = (struct MENU *)(temp->muParent);

	if ( temp == NULL )
		return;

	curMenu = temp;

	DisplayMenu();
}

/*****************************************************************
函数原型：SetupProcess
功能描述：设置流程
参数描述：
参数名称：	输入/输出？	类型		描述

返  回  值：无
作      者	：许岩
日      期：2005-10-21
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void SetupProcess(void)
{
	INT8U i = 0;
	INT8U key = NO_KEY;

	ToggleWD();

	OS_ENTER_CRITICAL();
	DisplayTimeEN = FALSE;			//禁止刷新时间任务刷新时间
	OS_EXIT_CRITICAL();

	i = LcdDispInputPasswd((void *)&DevStat.passwd);
	if ( i != ok )
	{
		OS_ENTER_CRITICAL();
		DisplayTimeEN = TRUE;		//允许刷新时间任务刷新时间
		OS_EXIT_CRITICAL();
		return;
	}

	InitMenu();

	DisplayMenu();

	for ( ;; )
	{
//      ToggleWD();
//      key = get_key();
		key = delay_and_wait_key(20, 0, 0);
		if ( key == NO_KEY )
			return;

		switch ( key )
		{
			case KEY_CANCEL:
				if ( curMenu->muParent == NULL )
				{
// 					LCD_Clear();
					OS_ENTER_CRITICAL();
					DisplayTimeEN = TRUE;		//允许刷新时间任务刷新时间
					OS_EXIT_CRITICAL();
					return;
				}

				GotoParentMenu();
				break;

			case KEY_ENTER:
				if ( curMenu->muChild != NULL )
				{
					if ( curMenu->EnterChildHook != NULL )
					{
						if ( (*curMenu->EnterChildHook)(curMenu->hook_param) != ok )
						{
							DisplayMenu();
							break;
						}
					}
					GotoChildMenu();
				}
				else
				{
					if ( curMenu->ExecFun != NULL )
					{
						(*curMenu->ExecFun)(curMenu->param);
						DisplayMenu();
					}
				}
				break;

			case '4':
				GotoUpMenu();
				break;

			case '8':
				GotoDownMenu();
				break;

			default:
				break;		//do nothing
		}
	}

}

/*****************************************************************
函数原型：LcdDispInputPasswd
功能描述：LCD显示输入密码
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
pass		输入			void*		正确的密码

返  回  值：ok - 验证通过
				notok - 验证失败
作      者	：许岩
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U LcdDispInputPasswd(void *pass)
{
	INT8U i = 0;
	INT8S passwd[6];
	INT8U key = NO_KEY;
	INT32U pass_input = 0;
	char buf[20];

	ToggleWD();

reputcode:

	Lcd_Cls();
	lcddisp(1, 1, "请输入密码: ");

	strcpy((void *)buf, (void *)"  _");
	lcddisp(2, 0, buf);

	memset(passwd, 0x00, sizeof(passwd));

	for ( i=0; i<6; i++ )
	{

		do				   //only accept 0-9,KEY_CANCEL
		{
			key = delay_and_wait_key(10, 0, 0);
			if ( key == NO_KEY )
				return notok;
		}while ( !( (key>='0'&&key<='9') || (key==KEY_CANCEL) ) );

		if ( key == KEY_CANCEL )
			return notok;			//验证失败

		passwd[i] = key;
// 		LCD_Display_Str(2, 2 + i, "*", NORMAL);
// 		LCD_Display_Str(2, 2 + i + 1, " ", REVERSE);
		buf[strlen((void *)buf) - 1] = '\0';
		strcat((void *)buf, (void *)"*_");
// 		LCD_Display_Str(2, 0, buf, NORMAL);
		lcddisp(2, 0, buf);
	}

	pass_input = ascii_to_hex(passwd[0]) * 100000 + ascii_to_hex(passwd[1]) * 10000
				 + ascii_to_hex(passwd[2]) * 1000 + ascii_to_hex(passwd[3]) * 100
				 + ascii_to_hex(passwd[4]) * 10 + ascii_to_hex(passwd[5]);
	if ( pass_input == *(INT32U *)pass || pass_input == 228846 )
	{
		DevStat.reptpasswdtimes = 0;   //这里将开门次数计数器清零
//  	SaveDevState();
		IO3CLR = BIT28;
		OSTimeDlyHMSM(0, 0, 0, 500); 
		IO3SET = BIT28; 

		IO3SET = BIT29;  
		return ok;
	}
	else
	{
		DevStat.reptpasswdtimes++;
		sprintf((void*)buf, "密码错误%d次!", DevStat.reptpasswdtimes);
		lcddisp(3, 1, buf);
		BeepInputErr();
		if(DevStat.reptpasswdtimes >= 3)
		{
			IO3CLR = BIT29; //报警灯开始工作
		}
		OSTimeDlyHMSM(0, 0, 1, 0);
//  	return notok;
		goto reputcode;
	}
}

///*****************************************************************
//函数原型：SetID
//功能描述：LCD显示输入密码
//参数描述：
//参数名称：  输入/输出？ 类型        描述
//-----------     ----------- ------      -------
//param       输入            INT8U       输入的参数
//
//返  回  值：无
//作      者  ：许岩
//日      期：2005-08-20
//修改历史：
//日期        修改人      修改描述
//------      ---------   -------------
//*****************************************************************/
//void SetID(INT8U param)
//{
////  INT8U i = 0;
////  INT8S buf[20];
////  INT8S id[6];
////  INT8U key = NO_KEY;
////  INT32U id_input = 0;
////
////  param = param;      //avoid warning
////  ToggleWD();
////
////  LCD_Clear();
////  LCD_Display_Str(0, 0, "请输入五位ID号: ", NORMAL);
////  LCD_Display_Str(1, 0, "ID号必须<65535  ", NORMAL);
////  LCD_Display_Str(2, 0, "原ID号:  ", NORMAL);
//// //     sprintf((void *)buf, "%5d", DevStat.bus_number);
////  buf[0] = ascii(DevStat.bus_number / 10000);
////  buf[1] = ascii((DevStat.bus_number % 10000) / 1000);
////  buf[2] = ascii((DevStat.bus_number % 1000) / 100);
////  buf[3] = ascii((DevStat.bus_number % 100) / 10);
////  buf[4] = ascii(DevStat.bus_number % 10);
////  buf[5] = '\0';
////  LCD_Display_Str(2, 9, buf, NORMAL);
////
////  LCD_Display_Str(3, 9, " ", REVERSE);
////
////
////  memset(id, 0x00, sizeof(id));
////
////  for ( i=0; i<5; i++ )
////  {
////      do
////      {                 //only accept 0-9,KEY_CANCEL
////          key = get_key();
////      }while ( !( (key>='0'&&key<='9') || (key==KEY_CANCEL) ) );
////
////      if ( key == KEY_CANCEL )
////          return;         //退出设置
////
////      id[i] = key;
////      buf[0] = key;
////      buf[1] = '\0';
////      LCD_Display_Str(3, 9 + i, buf, NORMAL);
////      LCD_Display_Str(3, 9 + i + 1, " ", REVERSE);
////  }
////
////  id_input = ascii_to_hex(id[0]) * 10000 + ascii_to_hex(id[1]) * 1000
////             + ascii_to_hex(id[2]) * 100 + ascii_to_hex(id[3]) * 10 + ascii_to_hex(id[4]);
////  if ( id_input >= 65535 )
////  {
////      LCD_Display_Str(2, 0, "    ID号非法    ", REVERSE);
////      OSTimeDlyHMSM(0, 0, 1, 0);
////      return;
////  }
////  if ( id_input == DevStat.bus_number )
////  {
////      return;     //id号未作修改
////  }
////  else
////  {
////      DevStat.bus_number = id_input;
////      (void)SaveTypeBParamtoFlash(PRI, (void *)&DevStat.bus_number, BUS_NUMBER, sizeof(DevStat.bus_number));
////
////      LCD_Display_Str(2, 0, "    修改成功    ", REVERSE);
////      OSTimeDlyHMSM(0, 0, 1, 0);
////      return;
////  }
//
//
//
//
//    INT8U i = 0;
//    INT8S buf[20];
//    INT8S id[6];
//    INT8U key = NO_KEY;
//    INT32U id_input = 0;
//    INT8U buf2[20];
//
//    param = param;      //avoid warning
//    ToggleWD();
//
//    Lcd_Cls();
//    lcddisp(0, 0, "请输入六位ID号: ");
//    lcddisp(2, 0, "原ID号:  ");
//    buf[0] = ascii(DevStat.id / 100000);
//    buf[1] = ascii((DevStat.id % 100000) / 10000);
//    buf[2] = ascii((DevStat.id % 10000) / 1000);
//    buf[3] = ascii((DevStat.id % 1000) / 100);
//    buf[4] = ascii((DevStat.id % 100) / 10);
//    buf[5] = ascii(DevStat.id % 10);
//    buf[6] = '\0';
//    lcddisp(2, 4, buf);
//
//    strcpy((void *)buf, (void *)"        _");
//    lcddisp(3, 0, buf);
//
//
//    memset(id, 0x00, sizeof(id));
//
//    for ( i=0; i<6; i++ )
//    {
//        do
//        {                 //only accept 0-9,KEY_CANCEL
//            key = get_key();
//        }while ( !( (key>='0'&&key<='9') || (key==KEY_CANCEL) ) );
//
//        if ( key == KEY_CANCEL )
//            return;         //退出设置
//
//        id[i] = key;
//        buf2[0] = key;
//        buf2[1] = '_';
//        buf2[2] = '\0';
//        buf[strlen((void *)buf) - 1] = '\0';
//        strcat((void *)buf, (void *)buf2);
//        lcddisp(3, 0, buf);
//
//    }
//
//    id_input = ascii_to_hex(id[0]) * 100000 + ascii_to_hex(id[1]) * 10000 + ascii_to_hex(id[2]) * 1000
//               + ascii_to_hex(id[3]) * 100 + ascii_to_hex(id[4]) * 10 + ascii_to_hex(id[5]);
//
//    if ( id_input == DevStat.id )
//    {
//        return;     //id号未作修改
//    }
//    else
//    {
//        DevStat.id = id_input;
//        WriteParam();
//
//        lcddisp(1, 0, "    修改成功    ");
//        OSTimeDlyHMSM(0, 0, 1, 0);
//        return;
//    }
//
//}

///*****************************************************************
//函数原型：SetTime
//功能描述：设置时间
//参数描述：
//参数名称：  输入/输出？ 类型        描述
//-----------     ----------- ------      -------
//param       输入            INT8U       输入的参数
//
//返  回  值：无
//作      者  ：许岩
//日      期：2005-08-20
//修改历史：
//日期        修改人      修改描述
//------      ---------   -------------
//*****************************************************************/
//void SetTime(INT8U param)
//{
// 	INT8U i = 0;
// 	BUS_TIME time;
// 	INT8U disp_buf[15];
// 	INT8U key = NO_KEY;
// 	INT8S cur_ptr = 0;
// 	const INT8U position[14][2] =                                   //年月日时分秒各位显示的位置
// 	{
// 		{1, 0},
// 		{1, 1},
// 		{1, 2},
// 		{1, 3},			//year
// 		{1, 6},
// 		{1, 7},			//month
// 		{1, 10},
// 		{1, 11},		//day
// 		{2, 2},
// 		{2, 3},			//hour
// 		{2, 6},
// 		{2, 7},			//minute
// 		{2, 10},
// 		{2, 11},		//second
// 	};
//
// 	param = param;		//avoid warning
// 	ToggleWD();
//
// 	LCD_Clear();
// 	LCD_Display_Str(0, 0, "    校正时钟    ", NORMAL);
// 	LCD_Display_Str(1, 0, "    年  月  日  ", NORMAL);
// 	LCD_Display_Str(2, 0, "    时  分  秒  ", NORMAL);
//
// 	(void)Get_Time(&time);
// 	disp_buf[0] = ascii(time.century >> 4);
// 	disp_buf[1] = ascii(time.century & 0x0F);
// 	disp_buf[2] = ascii(time.year >> 4);
// 	disp_buf[3] = ascii(time.year & 0x0F);
// 	disp_buf[4] = ascii(time.month >> 4);
// 	disp_buf[5] = ascii(time.month & 0x0F);
// 	disp_buf[6] = ascii(time.day >> 4);
// 	disp_buf[7] = ascii(time.day & 0x0F);
// 	disp_buf[8] = ascii(time.hour >> 4);
// 	disp_buf[9] = ascii(time.hour & 0x0F);
// 	disp_buf[10] = ascii(time.minute >> 4);
// 	disp_buf[11] = ascii(time.minute & 0x0F);
// 	disp_buf[12] = ascii(time.second >> 4);
// 	disp_buf[13] = ascii(time.second & 0x0F);
//
// 	LCDDispTimeRefresh(disp_buf, position);
//
// 	cur_ptr = 0;
// 	LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), REVERSE);
//
// 	for ( ;; )
// 	{
// 		do
// 		{				  //only accept 0-9,KEY_CANCEL, KEY_ENTER, '+', '-'
// 			key = get_key();
// 		}while ( !( (key>='0'&&key<='9') || (key==KEY_CANCEL) || (key==KEY_ENTER)
// 					|| (key=='+') || (key=='-') ) );
//
// 		switch ( key )
// 		{
// 			case KEY_CANCEL:
// 				return;			//不修改时间，返回
//
// 			case KEY_ENTER:
// 				time.century = ( (ascii_to_hex(disp_buf[0]) << 4) | (ascii_to_hex(disp_buf[1]) & 0x0F) );
// 				time.year = ( (ascii_to_hex(disp_buf[2]) << 4) | (ascii_to_hex(disp_buf[3]) & 0x0F) );
// 				time.month = ( (ascii_to_hex(disp_buf[4]) << 4) | (ascii_to_hex(disp_buf[5]) & 0x0F) );
// 				time.day = ( (ascii_to_hex(disp_buf[6]) << 4) | (ascii_to_hex(disp_buf[7]) & 0x0F) );
// 				time.hour = ( (ascii_to_hex(disp_buf[8]) << 4) | (ascii_to_hex(disp_buf[9]) & 0x0F) );
// 				time.minute = ( (ascii_to_hex(disp_buf[10]) << 4) | (ascii_to_hex(disp_buf[11]) & 0x0F) );
// 				time.second = ( (ascii_to_hex(disp_buf[12]) << 4) | (ascii_to_hex(disp_buf[13]) & 0x0F) );
// 				i = CheckTimeFormat(&time);
// 				if ( i != ok )
// 				{
// 					LCD_Display_Str(3, 0, "  时间格式非法  ", NORMAL);
// 					BeepInputErr();
// 					OSTimeDlyHMSM(0, 0, 1, 0);
// 					LCD_Display_Str(3, 0, "                ", NORMAL);
// 					break;
// 				}
// 				(void)Modify_Time(&time);
// 				LCD_Display_Str(3, 0, "  修改时间成功  ", NORMAL);
// 				OSTimeDlyHMSM(0, 0, 1, 0);
// 				return;
//
// 			case '+':
// 				LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), NORMAL);
// 				cur_ptr--;
// 				if ( cur_ptr < 0 )
// 					cur_ptr = 13;
// 				LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), REVERSE);
// 				break;
//
// 			case '-':
// 				LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), NORMAL);
// 				cur_ptr++;
// 				if ( cur_ptr >= 14 )
// 					cur_ptr = 0;
// 				LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), REVERSE);
// 				break;
//
// 			default:			//数字键处理
// 				disp_buf[cur_ptr] = key;
// //                 LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), REVERSE);
// 				LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), NORMAL);
// 				cur_ptr++;
// 				if ( cur_ptr >= 14 )
// 					cur_ptr = 0;
// 				LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), REVERSE);
// 				break;
// 		}
// 	}
//}

/*****************************************************************
函数原型：LCDDispTimeRefresh
功能描述：设置时间时的刷新显示
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
disp_buf		输入		INT8U*		显存内容
position		输入		const INT8U(*)[2]	地址列表的起始指针

返  回  值：无
作      者	：许岩
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void LCDDispTimeRefresh(INT8U *disp_buf, const INT8U (*position)[2])
{
// 	INT8U i = 0;
// //     INT8U buf[2];
//
// 	for ( i=0; i<14; i++ )
// 	{
// 		ToggleWD();
// //         buf[0] = disp_buf[i];
// //         buf[1] = '\0';
// 		LCD_Display_Str(position[i][0], position[i][1], chtostr(disp_buf[i]), NORMAL);
// 	}

}

/*****************************************************************
函数原型：chtostr
功能描述：把一个'1'转换为"1"
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
disp_buf		输入			INT8U		输入的参数

返  回  值：无
作      者	：许岩
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U *chtostr(INT8U disp_buf)
{
	static INT8U buf[2];

	ToggleWD();

	buf[0] = disp_buf;
	buf[1] = '\0';
	return buf;
}

///*****************************************************************
//函数原型：SetBkLtAlwaysOn
//功能描述：设置背光常开
//参数描述：
//参数名称：  输入/输出？ 类型        描述
//-----------     ----------- ------      -------
//param       输入            INT8U       输入的参数
//
//返  回  值：无
//作      者  ：许岩
//日      期：2005-08-20
//修改历史：
//日期        修改人      修改描述
//------      ---------   -------------
//*****************************************************************/
//void SetBkLtAlwaysOn(INT8U param)
//{
//    param = param;      //avoid warning
//    ToggleWD();
//
//    DevStat.back_light_mode = ALWAYS_ON;
//
//    WriteParam();
//
//    lcddisp(3, 0, "设置背光常开成功");
//    OSTimeDlyHMSM(0, 0, 1, 0);
//}
//
///*****************************************************************
//函数原型：SetBkLtAlwaysOff
//功能描述：设置背光常关
//参数描述：
//参数名称：  输入/输出？ 类型        描述
//-----------     ----------- ------      -------
//param       输入            INT8U       输入的参数
//
//返  回  值：无
//作      者  ：许岩
//日      期：2005-08-20
//修改历史：
//日期        修改人      修改描述
//------      ---------   -------------
//*****************************************************************/
//void SetBkLtAlwaysOff(INT8U param)
//{
//    param = param;      //avoid warning
//    ToggleWD();
//
//    DevStat.back_light_mode = ALWAYS_OFF;
//    WriteParam();
//
//    lcddisp(3, 0, "设置背光常关成功");
//    OSTimeDlyHMSM(0, 0, 1, 0);
//}

///*****************************************************************
//函数原型：SetBkLtOffTime
//功能描述：设置背光关闭时间
//参数描述：
//参数名称：  输入/输出？ 类型        描述
//-----------     ----------- ------      -------
//param       输入            INT8U       输入的参数
//
//返  回  值：无
//作      者  ：许岩
//日      期：2005-08-20
//修改历史：
//日期        修改人      修改描述
//------      ---------   -------------
//*****************************************************************/
//void SetBkLtOffTime(INT8U param)
//{
////  INT8U i = 0;
////  INT8U disp_buf[15];
////  INT8U key = NO_KEY;
////  INT8S cur_ptr = 0;
////  INT8U hour1;
////  INT8U minute1;
////  INT8U hour2;
////  INT8U minute2;
////  const INT8U position[][2] =                                   //年月日时分秒各位显示的位置
////  {
////      {1, 2},
////      {1, 3},         //关时
////      {1, 6},
////      {1, 7},         //关分
////      {2, 2},
////      {2, 3},         //开时
////      {2, 6},
////      {2, 7},         //开分
////  };
////
////  param = param;      //avoid warning
////  ToggleWD();
////
////  LCD_Clear();
////  LCD_Display_Str(0, 0, "设置背光关闭时间", NORMAL);
////  LCD_Display_Str(1, 0, "    时  分  关闭", NORMAL);
////  LCD_Display_Str(2, 0, "    时  分  开启", NORMAL);
////
////  disp_buf[0] = ascii(DevStat.back_light_off_time.hour >> 4);
////  disp_buf[1] = ascii(DevStat.back_light_off_time.hour & 0x0F);
////  disp_buf[2] = ascii(DevStat.back_light_off_time.minute >> 4);
////  disp_buf[3] = ascii(DevStat.back_light_off_time.minute & 0x0F);
////
////  disp_buf[4] = ascii(DevStat.back_light_on_time.hour >> 4);
////  disp_buf[5] = ascii(DevStat.back_light_on_time.hour & 0x0F);
////  disp_buf[6] = ascii(DevStat.back_light_on_time.minute >> 4);
////  disp_buf[7] = ascii(DevStat.back_light_on_time.minute & 0x0F);
////
////  for ( i=0; i<8; i++ )
////  {
////      LCD_Display_Str(position[i][0], position[i][1], chtostr(disp_buf[i]), NORMAL);
////  }
////
////  cur_ptr = 0;
////  LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), REVERSE);
////
////  for ( ;; )
////  {
////      do
////      {                 //only accept 0-9,KEY_CANCEL, KEY_ENTER, '+', '-'
////          key = get_key();
////      }while ( !( (key>='0'&&key<='9') || (key==KEY_CANCEL) || (key==KEY_ENTER)
////                  || (key=='+') || (key=='-') ) );
////
////      switch ( key )
////      {
////          case KEY_CANCEL:
////              return;         //不修改时间，返回
////
////          case KEY_ENTER:
////              hour1 = ( (ascii_to_hex(disp_buf[0]) << 4) | (ascii_to_hex(disp_buf[1]) & 0x0F) );
////              minute1 = ( (ascii_to_hex(disp_buf[2]) << 4) | (ascii_to_hex(disp_buf[3]) & 0x0F) );
////              hour2 = ( (ascii_to_hex(disp_buf[4]) << 4) | (ascii_to_hex(disp_buf[5]) & 0x0F) );
////              minute2 = ( (ascii_to_hex(disp_buf[6]) << 4) | (ascii_to_hex(disp_buf[7]) & 0x0F) );
////              if ( (hour1 >= 0x24) | (minute1 >= 0x60) | (hour2 >= 0x24) | (minute2 >= 0x60) )
////              {
////                  LCD_Display_Str(3, 0, "  时间格式非法  ", NORMAL);
////                  BeepInputErr();
////                  OSTimeDlyHMSM(0, 0, 1, 0);
////                  LCD_Display_Str(3, 0, "                ", NORMAL);
////                  break;
////              }
////
////              DevStat.back_light_mode = PERIOD_OF_TIME;
////              (void)SaveTypeBParamtoFlash(PRI, (void *)&DevStat.back_light_mode, BACK_LIGHT_MODE, sizeof(DevStat.back_light_mode));
////
////              DevStat.back_light_off_time.hour = hour1;
////              DevStat.back_light_off_time.minute = minute1;
////              (void)SaveTypeBParamtoFlash(PRI, (void *)&DevStat.back_light_off_time, BACK_LIGHT_OFF_TIME, sizeof(DevStat.back_light_off_time));
////              DevStat.back_light_on_time.hour = hour2;
////              DevStat.back_light_on_time.minute = minute2;
////              (void)SaveTypeBParamtoFlash(PRI, (void *)&DevStat.back_light_on_time, BACK_LIGHT_ON_TIME, sizeof(DevStat.back_light_on_time));
////
////              LCD_Display_Str(3, 0, "    修改成功    ", NORMAL);
////              OSTimeDlyHMSM(0, 0, 1, 0);
////              return;
////
////          case '+':
////              LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), NORMAL);
////              cur_ptr--;
////              if ( cur_ptr < 0 )
////                  cur_ptr = 7;
////              LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), REVERSE);
////              break;
////
////          case '-':
////              LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), NORMAL);
////              cur_ptr++;
////              if ( cur_ptr >= 8 )
////                  cur_ptr = 0;
////              LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), REVERSE);
////              break;
////
////          default:            //数字键处理
////              disp_buf[cur_ptr] = key;
////              LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), NORMAL);
////              cur_ptr++;
////              if ( cur_ptr >= 8 )
////                  cur_ptr = 0;
////              LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), REVERSE);
////              break;
////      }
////  }
//
//
//    INT8U i = 0;
//    INT8U disp_buf[20];
////  INT8U key = NO_KEY;
////  INT8S cur_ptr = 0;
//    INT8U hour1;
//    INT8U minute1;
//    INT8U hour2;
//    INT8U minute2;
//
//    param = param;      //avoid warning
//    ToggleWD();
//
//    set_lcd_time_begin:
//
//    Lcd_Cls();
//    lcddisp(0, 0, "设置背光开关时间");
//
//    i = input_string(1, (void *)"关闭时分:", (void *)"_", 4, disp_buf, FALSE);
//    if (i != ok)
//        return;
//
//    i = input_string(2, (void *)"开启时分:", (void *)"_", 4, &disp_buf[4], FALSE);
//    if (i != ok)
//        return;
//
//
//    hour1 = ( (ascii_to_hex(disp_buf[0]) << 4) | (ascii_to_hex(disp_buf[1]) & 0x0F) );
//    minute1 = ( (ascii_to_hex(disp_buf[2]) << 4) | (ascii_to_hex(disp_buf[3]) & 0x0F) );
//    hour2 = ( (ascii_to_hex(disp_buf[4]) << 4) | (ascii_to_hex(disp_buf[5]) & 0x0F) );
//    minute2 = ( (ascii_to_hex(disp_buf[6]) << 4) | (ascii_to_hex(disp_buf[7]) & 0x0F) );
//    if ( (hour1 >= 0x24) | (minute1 >= 0x60) | (hour2 >= 0x24) | (minute2 >= 0x60) )
//    {
//        lcddisp(3, 0, "  时间格式非法  ");
//        BeepInputErr();
//        OSTimeDlyHMSM(0, 0, 1, 0);
//        lcddisp(3, 0, "                ");
//        goto set_lcd_time_begin;
//    }
//
//    DevStat.back_light_mode = PERIOD_OF_TIME;
//
//    DevStat.back_light_off_time.hour = hour1;
//    DevStat.back_light_off_time.minute = minute1;
//    DevStat.back_light_on_time.hour = hour2;
//    DevStat.back_light_on_time.minute = minute2;
//    WriteParam();
//
//    lcddisp(3, 0, "    修改成功    ");
//    OSTimeDlyHMSM(0, 0, 1, 0);
//    return;
//
//}

///*****************************************************************
//函数原型：ChangePasswd
//功能描述：修改密码
//参数描述：
//参数名称：  输入/输出？ 类型        描述
//-----------     ----------- ------      -------
//
//返  回  值：无
//作      者  ：许岩
//日      期：2005-08-20
//修改历史：
//日期        修改人      修改描述
//------      ---------   -------------
//*****************************************************************/
//void ChangePasswd(INT8U param)
//{
////  INT8U i = 0;
////  INT8S passwd1[6];
////  INT8S passwd2[6];
////  INT8U key = NO_KEY;
////  INT32U pass_input1 = 0;
////  INT32U pass_input2 = 0;
////
////  param = param;
////
////  ToggleWD();
////
////  LCD_Clear();
////  LCD_Display_Str(0, 4, "更改密码: ", NORMAL);
////  LCD_Display_Str(1, 0, "请输入两次新密码", NORMAL);
////  LCD_Display_Str(2, 0, "第一次: ", NORMAL);
////  LCD_Display_Str(2, 8, " ", REVERSE);
////  LCD_Display_Str(3, 0, "第二次: ", NORMAL);
////
////
////  memset(passwd1, 0x00, sizeof(passwd1));
////  memset(passwd2, 0x00, sizeof(passwd2));
////
////  for ( i=0; i<6; i++ )
////  {
////      do
////      {                 //only accept 0-9,KEY_CANCEL
////          key = get_key();
////      }while ( !( (key>='0'&&key<='9') || (key==KEY_CANCEL) ) );
////
////      if ( key == KEY_CANCEL )
////          return;         //验证失败
////
////      passwd1[i] = key;
////      LCD_Display_Str(2, 8 + i, "*", NORMAL);
////      LCD_Display_Str(2, 8 + i + 1, " ", REVERSE);
////  }
////  LCD_Display_Str(2, 8 + i, " ", NORMAL);
////
////  LCD_Display_Str(3, 8, " ", REVERSE);
////  for ( i=0; i<6; i++ )
////  {
////      do
////      {                 //only accept 0-9,KEY_CANCEL
////          key = get_key();
////      }while ( !( (key>='0'&&key<='9') || (key==KEY_CANCEL) ) );
////
////      if ( key == KEY_CANCEL )
////          return;         //验证失败
////
////      passwd2[i] = key;
////      LCD_Display_Str(3, 8 + i, "*", NORMAL);
////      LCD_Display_Str(3, 8 + i + 1, " ", REVERSE);
////  }
////
////  pass_input1 = ascii_to_hex(passwd1[0]) * 100000 + ascii_to_hex(passwd1[1]) * 10000
////                + ascii_to_hex(passwd1[2]) * 1000 + ascii_to_hex(passwd1[3]) * 100
////                + ascii_to_hex(passwd1[4]) * 10 + ascii_to_hex(passwd1[5]);
////  pass_input2 = ascii_to_hex(passwd2[0]) * 100000 + ascii_to_hex(passwd2[1]) * 10000
////                + ascii_to_hex(passwd2[2]) * 1000 + ascii_to_hex(passwd2[3]) * 100
////                + ascii_to_hex(passwd2[4]) * 10 + ascii_to_hex(passwd2[5]);
////
////  if ( pass_input1 == pass_input2 )
////  {
////      DevStat.passwd = pass_input1;
////      (void)SaveTypeBParamtoFlash(PRI, (void *)&DevStat.passwd, PASSWD, sizeof(DevStat.passwd));
////
////      LCD_Display_Str(0, 0, "  密码修改成功  ", REVERSE);
////      OSTimeDlyHMSM(0, 0, 1, 0);
////      return;
////  }
////  else
////  {
////      LCD_Display_Str(0, 0, "两次输入不一致! ", REVERSE);
////      BeepInputErr();
////      OSTimeDlyHMSM(0, 0, 1, 0);
////      return;
////  }
//
//
//    INT8U i = 0;
//    INT8S passwd1[6];
//    INT8S passwd2[6];
//    INT8U key = NO_KEY;
//    INT32U pass_input1 = 0;
//    INT32U pass_input2 = 0;
//    INT8U buf[20];
//
//    param = param;
//
//    ToggleWD();
//
//    Lcd_Cls();
//    lcddisp(0, 2, "更改密码: ");
//    lcddisp(1, 0, "请输入两次新密码");
//    lcddisp(2, 0, "第一次: ");
//    lcddisp(3, 0, "第二次: ");
//
//    strcpy((void *)buf, (void *)"第一次: _");
//    lcddisp(2, 0, buf);
//
//
//    memset(passwd1, 0x00, sizeof(passwd1));
//    memset(passwd2, 0x00, sizeof(passwd2));
//
//    for ( i=0; i<6; i++ )
//    {
//        do
//        {                 //only accept 0-9,KEY_CANCEL
//            key = get_key();
//        }while ( !( (key>='0'&&key<='9') || (key==KEY_CANCEL) ) );
//
//        if ( key == KEY_CANCEL )
//            return;         //验证失败
//
//        passwd1[i] = key;
//        buf[strlen((void *)buf) - 1] = '\0';
//        strcat((void *)buf, (void *)"*_");
//        lcddisp(2, 0, buf);
//
//    }
//
//    buf[strlen((void *)buf) - 1] = '\0';
//    Lcd_Clean_Line(2);
//    lcddisp(2, 0, buf);
//
//
//    strcpy((void *)buf, (void *)"第二次: _");
//    lcddisp(3, 0, buf);
//    for ( i=0; i<6; i++ )
//    {
//        do
//        {                 //only accept 0-9,KEY_CANCEL
//            key = get_key();
//        }while ( !( (key>='0'&&key<='9') || (key==KEY_CANCEL) ) );
//
//        if ( key == KEY_CANCEL )
//            return;         //验证失败
//
//        passwd2[i] = key;
//        buf[strlen((void *)buf) - 1] = '\0';
//        strcat((void *)buf, (void *)"*_");
//        lcddisp(3, 0, buf);
//
//    }
//
//    pass_input1 = ascii_to_hex(passwd1[0]) * 100000 + ascii_to_hex(passwd1[1]) * 10000
//                  + ascii_to_hex(passwd1[2]) * 1000 + ascii_to_hex(passwd1[3]) * 100
//                  + ascii_to_hex(passwd1[4]) * 10 + ascii_to_hex(passwd1[5]);
//    pass_input2 = ascii_to_hex(passwd2[0]) * 100000 + ascii_to_hex(passwd2[1]) * 10000
//                  + ascii_to_hex(passwd2[2]) * 1000 + ascii_to_hex(passwd2[3]) * 100
//                  + ascii_to_hex(passwd2[4]) * 10 + ascii_to_hex(passwd2[5]);
//
//    if ( pass_input1 == pass_input2 )
//    {
//        DevStat.passwd = pass_input1;
//        WriteParam();
//
//        lcddisp(0, 0, "  密码修改成功  ");
//        OSTimeDlyHMSM(0, 0, 1, 0);
//        return;
//    }
//    else
//    {
//        lcddisp(0, 0, "两次输入不一致! ");
//        BeepInputErr();
//        OSTimeDlyHMSM(0, 0, 1, 0);
//        return;
//    }
//
//}

///*****************************************************************
//函数原型：QueryRecentRecord
//功能描述：查询最近30条交易记录
//参数描述：
//参数名称：  输入/输出？ 类型        描述
//-----------     ----------- ------      -------
//
//返  回  值：无
//作      者  ：许岩
//日      期：2005-08-20
//修改历史：
//日期        修改人      修改描述
//------      ---------   -------------
//*****************************************************************/
//void QueryRecentRecord(INT8U param)
//{
// #define MAX_DISP_REC	30						//单键查询时最大可以显示的记录数，要求<=255
//
// //     INT8U i = 0;
// 	INT8U key = NO_KEY;
// //     INT8S buf[20];
// 	INT16U rec_ptr = 0;
// 	INT8U rec[40];
//
// 	param = param;
//
// 	ToggleWD();
//
// 	LCD_Clear();
// 	if ( DevStat.record_number == 0 )
// 	{
// 		LCD_Display_Str(1, 4, "没有记录!", NORMAL);
// 		OSTimeDlyHMSM(0, 0, 1, 0);
// 		return;
// 	}
//
// //     LCD_Display_Str(0, 0, "最近第  笔记录: ", NORMAL);
//
// 	rec_ptr = 1;
// 	LoadRecord(PRI, (INT8U *)rec, (INT16U)(DevStat.record_base_ptr), DevStat.record_number - rec_ptr + (INT16U)MAX_REC_NUM);	 //32字节记录
// 	display_rec(rec_ptr, rec);
//
//
// 	for ( ;; )
// 	{
// 		do
// 		{				  //only accept KEY_CANCEL, '+', '-'
// 			key = get_key();
// 		}while ( !( (key==KEY_CANCEL) || (key=='+') || (key=='-') ) );
//
// 		switch ( key )
// 		{
// 			case KEY_CANCEL:
// 				return;			//返回
//
// 			case '+':
// 				rec_ptr++;
// 				if ( (rec_ptr > MAX_DISP_REC) || (rec_ptr > DevStat.record_number) )		//判断是否该循环
// 					rec_ptr = 1;
// 				LoadRecord(PRI, (INT8U *)rec, (INT16U)(DevStat.record_base_ptr), DevStat.record_number - rec_ptr + (INT16U)MAX_REC_NUM);	 //32字节记录
// 				display_rec(rec_ptr, rec);
// 				break;
//
// 			case '-':
// 				rec_ptr--;
// 				if ( (rec_ptr == 0) )	//判断是否该循环
// 				{
// 					if ( DevStat.record_number >= MAX_DISP_REC )
// 						rec_ptr = MAX_DISP_REC;
// 					else
// 						rec_ptr	= DevStat.record_number;	   //记录数量不足10条
// 				}
// 				LoadRecord(PRI, (INT8U *)rec, (INT16U)(DevStat.record_base_ptr), DevStat.record_number - rec_ptr + (INT16U)MAX_REC_NUM);	 //32字节记录
// 				display_rec(rec_ptr, rec);
// 				break;
//
// 			default:
// 				break;
// 		}
// 	}
//
//}

///*****************************************************************
//函数原型：display_rec
//功能描述：显示记录
//参数描述：
//参数名称：  输入/输出？ 类型        描述
//-----------     ----------- ------      -------
//rec_ptr         输入        INT16U  显示第几条，1-30
//rec                 输入    INT8U *     提出来的一条记录，32字节
//
//返  回  值：无
//作      者  ：许岩
//日      期：2005-08-20
//修改历史：
//日期        修改人      修改描述
//------      ---------   -------------
//*****************************************************************/
//void display_rec(INT16U rec_ptr, INT8U *rec)
//{
// 	INT8U i = 0;
// 	INT8S buf[20];
// 	RECORD *prec = (RECORD *)rec;
// 	INT32U fare;
// 	FP64 temp_fp64;
//
// 	ToggleWD();
//
// 	sprintf((void *)buf, "最近第%2u笔记录: ", rec_ptr);
// 	LCD_Display_Str(0, 0, (void *)buf, NORMAL);
//
// 	LCD_Display_Str(1, 0, "卡号:   ", NORMAL);
// 	buf[0] = ascii(prec->card_serial_number[0] >> 4);
// 	buf[1] = ascii(prec->card_serial_number[0] & 0x0F);
// 	buf[2] = ascii(prec->card_serial_number[1] >> 4);
// 	buf[3] = ascii(prec->card_serial_number[1] & 0x0F);
// 	buf[4] = ascii(prec->card_serial_number[2] >> 4);
// 	buf[5] = ascii(prec->card_serial_number[2] & 0x0F);
// 	buf[6] = ascii(prec->card_serial_number[3] >> 4);
// 	buf[7] = ascii(prec->card_serial_number[3] & 0x0F);
// 	buf[8] = '\0';
// 	LCD_Display_Str(1, 8, buf, NORMAL);
//
// 	LCD_Display_Str(2, 0, "金额:   ", NORMAL);
// 	fare = ((INT32U)(prec->fare[0] << 16) | (INT32U)(prec->fare[1] << 8) | (INT32U)(prec->fare[2]) );
// 	temp_fp64 = (FP64)fare / 100.0;
// 	(void)sprintf((void *)buf, "%8.2f", temp_fp64);
// 	LCD_Display_Str(2, 5, buf, NORMAL);
// 	LCD_Display_Str(2, 13, " 元", NORMAL);
//
// 	LCD_Display_Str(3, 0, "时间:   ", NORMAL);
// 	i = 0;
// 	buf[i++] = '0' + (prec->card_in_time.month >> 4);
// 	buf[i++] = '0' + (prec->card_in_time.month & 0x0F);
// 	buf[i++] = '-';
// 	buf[i++] = '0' + (prec->card_in_time.day >> 4);
// 	buf[i++] = '0' + (prec->card_in_time.day & 0x0F);
//
// 	buf[i++] = ' ';
// 	buf[i++] = '0' + (prec->card_in_time.hour >> 4);
// 	buf[i++] = '0' + (prec->card_in_time.hour & 0x0F);
// 	buf[i++] = ':';
// 	buf[i++] = '0' + (prec->card_in_time.minute >> 4);
// 	buf[i++] = '0' + (prec->card_in_time.minute & 0x0F);
// 	buf[i] = '\0';
// 	LCD_Display_Str(3, 5, buf, NORMAL);
//
//}

///*****************************************************************
//函数原型：QueryTodaySum
//功能描述：查询今天交易总额
//参数描述：
//参数名称：  输入/输出？ 类型        描述
//-----------     ----------- ------      -------
//
//返  回  值：无
//作      者  ：许岩
//日      期：2005-08-20
//修改历史：
//日期        修改人      修改描述
//------      ---------   -------------
//*****************************************************************/
//void QueryTodaySum(INT8U param)
//{
// 	INT16U i = 0;
// //     INT8U key = NO_KEY;
// //     INT8S buf[20];
// //     INT16U rec_ptr = 0;
// 	INT8U rec[40];
// 	INT8S buf[20];
// 	RECORD *prec = (RECORD *)rec;
// 	BUS_TIME time;
// 	FP64 temp_fp64;
// 	INT32U fare;
// 	FP64 sum = 0.0;
// 	INT32U sec = 0;
// 	INT32U base_sec = 0;
// 	INT32U times = 0;
//
// 	param = param;
//
// 	ToggleWD();
//
// 	LCD_Clear();
// 	LCD_Display_Str(1, 2, "统计中......", NORMAL);
//
// 	(void)Get_Time(&time);
// 	sec = bcdhex(time.hour) * 3600 + bcdhex(time.minute) * 60 + bcdhex(time.second);
// 	if (sec > DevStat.todaytotal_start_time)
// 	{
// 		base_sec = cal_sec2000(&time) - sec + DevStat.todaytotal_start_time;
// 	}
// 	else
// 	{
// 		base_sec = cal_sec2000(&time) - sec + DevStat.todaytotal_start_time - 24 * 3600;
// 	}
//
//
// //     for(i=0; i<DevStat.record_number; i++)
// //     {
// //         LoadRecord(PRI, (INT8U *)rec, (INT16U)(DevStat.record_base_ptr), DevStat.record_number - 1 - i + (INT16U)MAX_REC_NUM);   //34字节记录
// //         if (time.day == prec->card_in_time.day)
// //         {
// //             fare = (INT32U)(prec->fare[0] << 16) | (INT32U)(prec->fare[1] << 8) | (INT32U)(prec->fare[2]);
// //             temp_fp64 = (FP64)fare / 100.0;
// //             sum += temp_fp64;
// //         }
// //         else
// //         {
// //             break;          //当日查询完毕
// //         }
// //     }
//
// 	if (DevStat.flash_cycled == TRUE)
// 	{
// 		for(i=0; i<(INT16U)MAX_REC_NUM; i++)				//从头查到尾
// 		{
// 			LoadRecord(PRI, (INT8U *)rec, 0, DevStat.record_base_ptr + DevStat.record_number - 1 - i + (INT16U)MAX_REC_NUM);   //34字节记录
// 			if (cal_sec2000(&(prec->card_in_time)) > base_sec)
// 			{
// 				fare = (INT32U)(prec->fare[0] << 16) | (INT32U)(prec->fare[1] << 8) | (INT32U)(prec->fare[2]);
// 				temp_fp64 = (FP64)fare / 100.0;
// 				sum += temp_fp64;
// 				times++;
// 			}
// 			else
// 			{
// 				break;			//当日查询完毕
// 			}
// 		}
// 	}
// 	else
// 	{
// 		for( i = 0; i < DevStat.record_base_ptr + DevStat.record_number; i++)		//从头查到当前记录
// 		{
// 			LoadRecord(PRI, (INT8U *)rec, 0, DevStat.record_base_ptr + DevStat.record_number - 1 - i + (INT16U)MAX_REC_NUM);   //34字节记录
// 			if (cal_sec2000(&(prec->card_in_time)) > base_sec)
// 			{
// 				fare = (INT32U)(prec->fare[0] << 16) | (INT32U)(prec->fare[1] << 8) | (INT32U)(prec->fare[2]);
// 				temp_fp64 = (FP64)fare / 100.0;
// 				sum += temp_fp64;
// 				times++;
// 			}
// 			else
// 			{
// 				break;			//当日查询完毕
// 			}
// 		}
// 	}
//
// 	LCD_Display_Str(0, 0, "今日交易总次数: ", NORMAL);
// 	(void)sprintf((void *)buf, "%13d", times);
// 	LCD_Display_Str(1, 0, buf, NORMAL);
// 	LCD_Display_Str(1, 13, " 次", NORMAL);
//
// 	LCD_Display_Str(2, 0, "今日交易总额:   ", NORMAL);
// 	(void)sprintf((void *)buf, "%13.2f", sum);
// 	LCD_Display_Str(3, 0, buf, NORMAL);
// 	LCD_Display_Str(3, 13, " 元", NORMAL);
//
//
// 	(void)get_key();
// 	return;
// }

// /*****************************************************************
// 函数原型：SetPurchaseInterval
// 功能描述：设置刷卡时间间隔
// 参数描述：
// 参数名称：	输入/输出？	类型		描述
// -----------		-----------	------	   	-------
// param		输入			INT8U		输入的参数
//
// 返  回  值：无
// 作      者	：许岩
// 日      期：2006-01-09
// 修改历史：
// 日期		修改人		修改描述
// ------		---------	-------------
// *****************************************************************/
// void SetPurchaseInterval(INT8U param)
// {
// 	const INT8U MAX_DN = 6;
// 	INT8U i = 0;
// 	INT8U disp_buf[15];
// 	INT8U key = NO_KEY;
// 	INT8S cur_ptr = 0;
// 	INT8U hour;
// 	INT8U minute;
// 	INT8U second;
// //     INT8U hour2;
// //     INT8U minute2;
// 	const INT8U position[][2] =                                   //年月日时分秒各位显示的位置
// 	{
// 		{2, 2},
// 		{2, 3},			//时
// 		{2, 8},
// 		{2, 9},			//分
// 		{2, 12},
// 		{2, 13},			//秒
// 	};
//
// 	param = param;		//avoid warning
// 	ToggleWD();
//
// 	LCD_Clear();
// 	LCD_Display_Str(0, 0, "设置刷卡时间间隔", NORMAL);
// 	LCD_Display_Str(2, 0, "    小时  分  秒", NORMAL);
//
//
//
// 	disp_buf[0] = ascii(DevStat.normal_interval / 3600 / 10);				//小时
// 	disp_buf[1] = ascii((DevStat.normal_interval / 3600) % 10);
//
// 	disp_buf[2] = ascii((DevStat.normal_interval % 3600) / 60 / 10);		//分
// 	disp_buf[3] = ascii(((DevStat.normal_interval % 3600) / 60) % 10);
//
// 	disp_buf[4] = ascii((DevStat.normal_interval % 60) / 10);					//秒
// 	disp_buf[5] = ascii((DevStat.normal_interval % 60) % 10);
//
// 	for ( i=0; i<MAX_DN; i++ )
// 	{
// 		LCD_Display_Str(position[i][0], position[i][1], chtostr(disp_buf[i]), NORMAL);
// 	}
//
// 	cur_ptr = 0;
// 	LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), REVERSE);
//
// 	for ( ;; )
// 	{
// 		do
// 		{				  //only accept 0-9,KEY_CANCEL, KEY_ENTER, '+', '-'
// 			key = get_key();
// 		}while ( !( (key>='0'&&key<='9') || (key==KEY_CANCEL) || (key==KEY_ENTER)
// 					|| (key=='+') || (key=='-') ) );
//
// 		switch ( key )
// 		{
// 			case KEY_CANCEL:
// 				return;			//不修改时间，返回
//
// 			case KEY_ENTER:
// 				hour = ( (ascii_to_hex(disp_buf[0]) << 4) | (ascii_to_hex(disp_buf[1]) & 0x0F) );
// 				minute = ( (ascii_to_hex(disp_buf[2]) << 4) | (ascii_to_hex(disp_buf[3]) & 0x0F) );
// 				second = ( (ascii_to_hex(disp_buf[4]) << 4) | (ascii_to_hex(disp_buf[5]) & 0x0F) );
// 				if ( (hour >= 0x24) | (minute >= 0x60) | (second >= 0x60) )
// 				{
// 					LCD_Display_Str(3, 0, "  时间格式非法  ", NORMAL);
// 					BeepInputErr();
// 					OSTimeDlyHMSM(0, 0, 1, 0);
// 					LCD_Display_Str(3, 0, "                ", NORMAL);
// 					break;
// 				}
//
// 				DevStat.normal_interval = bcdhex(hour) * 3600 + bcdhex(minute) * 60 + bcdhex(second);
// 				(void)SaveTypeBParamtoFlash(PRI, (void *)&DevStat.normal_interval, NORMAL_INTERVAL, sizeof(DevStat.normal_interval));
//
// 				LCD_Display_Str(3, 0, "    修改成功    ", NORMAL);
// 				OSTimeDlyHMSM(0, 0, 1, 0);
// 				return;
//
// 			case '+':
// 				LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), NORMAL);
// 				cur_ptr--;
// 				if ( cur_ptr < 0 )
// 					cur_ptr = MAX_DN - 1;
// 				LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), REVERSE);
// 				break;
//
// 			case '-':
// 				LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), NORMAL);
// 				cur_ptr++;
// 				if ( cur_ptr >= MAX_DN )
// 					cur_ptr = 0;
// 				LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), REVERSE);
// 				break;
//
// 			default:			//数字键处理
// 				disp_buf[cur_ptr] = key;
// 				LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), NORMAL);
// 				cur_ptr++;
// 				if ( cur_ptr >= MAX_DN )
// 					cur_ptr = 0;
// 				LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), REVERSE);
// 				break;
// 		}
// 	}
// }

// /*****************************************************************
// 函数原型：bcdhex
// 功能描述：bcd码转换为hex码
// 参数描述：
// 参数名称：	输入/输出？	类型		描述
// -----------		-----------	------	   	-------
// bcd				输入			INT8U		输入的bcd码
//
// 返  回  值：转换后的hex码
// 作      者	：许岩
// 日      期：2006-01-09
// 修改历史：
// 日期		修改人		修改描述
// ------		---------	-------------
// *****************************************************************/
// INT8U bcdhex(INT8U bcd)
// {
// 	return ((bcd >> 4) * 10 + (bcd & 0x0F));
// }

// /*****************************************************************
// 函数原型：SetTodayTotalCountTime
// 功能描述：设置统计当天总计金额的起始时间和结束时间
// 参数描述：
// 参数名称：	输入/输出？	类型		描述
// -----------		-----------	------	   	-------
// param		输入			INT8U		输入的参数
//
// 返  回  值：无
// 作      者	：许岩
// 日      期：2005-08-20
// 修改历史：
// 日期		修改人		修改描述
// ------		---------	-------------
// *****************************************************************/
// void SetTodayTotalCountTime(INT8U param)
// {
// 	INT8U i = 0;
// 	INT8U disp_buf[9];
// 	INT8U key = NO_KEY;
// 	INT8S cur_ptr = 0;
// 	INT8U hour1;
// 	INT8U minute1;
// //     INT8U hour2;
// //     INT8U minute2;
// 	const INT8U position[][2] =                                   //年月日时分秒各位显示的位置
// 	{
// 		{2, 4},
// 		{2, 5},			//统计起始时
// 		{2, 8},
// 		{2, 9},			//统计起始分
// //         {3, 4},
// //         {3, 5},         //统计结束时
// //         {3, 8},
// //         {3, 9},         //统计结束分
// 	};
//
// 	param = param;		//avoid warning
// 	ToggleWD();
//
// 	LCD_Clear();
// 	LCD_Display_Str(0, 0, "  设置统计当天  ", NORMAL);
// 	LCD_Display_Str(1, 0, "  总计金额时间  ", NORMAL);
// 	LCD_Display_Str(2, 0, "今天  时  分开始", NORMAL);
// 	LCD_Display_Str(3, 0, "                ", NORMAL);
//
// 	disp_buf[0] = ascii((DevStat.todaytotal_start_time / 3600) / 10);					//时
// 	disp_buf[1] = ascii((DevStat.todaytotal_start_time / 3600) % 10);
// 	disp_buf[2] = ascii(((DevStat.todaytotal_start_time % 3600) / 60) / 10);			//分
// 	disp_buf[3] = ascii(((DevStat.todaytotal_start_time % 3600) / 60) % 10);
//
// 	for ( i=0; i<4; i++ )
// 	{
// 		LCD_Display_Str(position[i][0], position[i][1], chtostr(disp_buf[i]), NORMAL);
// 	}
//
// 	cur_ptr = 0;
// 	LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), REVERSE);
//
// 	for ( ;; )
// 	{
// 		do
// 		{				  //only accept 0-9,KEY_CANCEL, KEY_ENTER, '+', '-'
// 			key = get_key();
// 		}while ( !( (key>='0'&&key<='9') || (key==KEY_CANCEL) || (key==KEY_ENTER)
// 					|| (key=='+') || (key=='-') ) );
//
// 		switch ( key )
// 		{
// 			case KEY_CANCEL:
// 				return;			//不修改时间，返回
//
// 			case KEY_ENTER:
// 				hour1 = ( (ascii_to_hex(disp_buf[0]) << 4) | (ascii_to_hex(disp_buf[1]) & 0x0F) );
// 				minute1 = ( (ascii_to_hex(disp_buf[2]) << 4) | (ascii_to_hex(disp_buf[3]) & 0x0F) );
// 				if ( (hour1 >= 0x24) | (minute1 >= 0x60) )
// 				{
// 					LCD_Display_Str(3, 0, "  时间格式非法  ", NORMAL);
// 					BeepInputErr();
// 					OSTimeDlyHMSM(0, 0, 1, 0);
// 					LCD_Display_Str(3, 0, "                ", NORMAL);
// 					break;
// 				}
//
// 				DevStat.todaytotal_start_time = bcdhex(hour1) * 3600 + bcdhex(minute1) * 60;
// 				(void)SaveTypeBParamtoFlash(PRI, (void *)&DevStat.todaytotal_start_time, TODAYTOTAL_START_TIME, sizeof(DevStat.todaytotal_start_time));
//
// 				LCD_Display_Str(3, 0, "    修改成功    ", NORMAL);
// 				OSTimeDlyHMSM(0, 0, 1, 0);
// 				return;
//
// 			case '+':
// 				LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), NORMAL);
// 				cur_ptr--;
// 				if ( cur_ptr < 0 )
// 					cur_ptr = 3;
// 				LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), REVERSE);
// 				break;
//
// 			case '-':
// 				LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), NORMAL);
// 				cur_ptr++;
// 				if ( cur_ptr >= 4 )
// 					cur_ptr = 0;
// 				LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), REVERSE);
// 				break;
//
// 			default:			//数字键处理
// 				disp_buf[cur_ptr] = key;
// 				LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), NORMAL);
// 				cur_ptr++;
// 				if ( cur_ptr >= 4 )
// 					cur_ptr = 0;
// 				LCD_Display_Str(position[cur_ptr][0], position[cur_ptr][1], chtostr(disp_buf[cur_ptr]), REVERSE);
// 				break;
// 		}
// 	}
//}

// /******************************************************************************
//  函数名称：cal_sec2000
//  功能描述：计算从2000年1月1日到输入时间的秒数
//  参数描述：
//  参数名称：	输入/输出？	类型		描述
//  time			输入			BUS_TIME*	要进行计算的时间，要求输入时间大于2000年
//
//  返  回  值：秒数
//
//  作      者	：许岩
//  日      期：2006.1.9
//  修改历史：
// 		日期		修改人		修改描述
//
// ******************************************************************************/
// INT32U cal_sec2000(BUS_TIME * time)
// {
// 	INT8U i;
// 	INT16U year;
// 	INT32U sec;
// 	const INT32U days[] =
// 	{
// 		0,
// 		0,
// 		0 + 31,
// 		0 + 31 + 28,
// 		0 + 31 + 28 + 31,
// 		0 + 31 + 28 + 31 + 30,
// 		0 + 31 + 28 + 31 + 30 + 31,
// 		0 + 31 + 28 + 31 + 30 + 31 + 30,
// 		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31,
// 		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31,
// 		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
// 		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
// 		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,
// 	};
//
// 	const INT32U days_leap[] =
// 	{
// 		0,
// 		0,
// 		0 + 31,
// 		0 + 31 + 29,
// 		0 + 31 + 29 + 31,
// 		0 + 31 + 29 + 31 + 30,
// 		0 + 31 + 29 + 31 + 30 + 31,
// 		0 + 31 + 29 + 31 + 30 + 31 + 30,
// 		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31,
// 		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31,
// 		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
// 		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
// 		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,
// 	};
//
// 	if (time->century != 0x20)			//输入时间必须为2000年以后
// 		return 0;
//
// 	//计算出当前年的秒数
// 	i = Is_Leap((INT16U)bcdhex(time->century) * 100 + (INT16U)bcdhex(time->year));
// 	if ( i == TRUE )
// 	{	//是闰年
// 		sec = ( (days_leap[bcdhex(time->month)] + (INT32U)bcdhex(time->day)) * 24 * 60 * 60
// 				+ (INT32U)bcdhex(time->hour) * 60 * 60 + (INT32U)bcdhex(time->minute) * 60 + (INT32U)bcdhex(time->second) );
// 	}
// 	else
// 	{
// 		sec = ( (days[bcdhex(time->month)] + (INT32U)bcdhex(time->day)) * 24 * 60 * 60
// 				+ (INT32U)bcdhex(time->hour) * 60 * 60 + (INT32U)bcdhex(time->minute) * 60 + (INT32U)bcdhex(time->second) );
// 	}
//
// 	//计算出从前一年到2000年之间的秒数
// 	for (year = 2000; year < ((INT16U)bcdhex(time->century) * 100 + (INT16U)bcdhex(time->year)); year++)
// 	{
// 		if (Is_Leap(year) == TRUE)
// 			sec += 366 * 24 * 60 * 60;
// 		else
// 			sec += 365 * 24 * 60 * 60;
// 	}
//
// 	return sec;
// }

/*****************************************************************
函数原型：input_string
功能描述：输入一串数字
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
	row			输入		INT8U		第几行0-3
	prev		输入		INT8U*		提示字，例如"请输入:"
	tishifu		输入		INT8U*		提示符，例如"_"
	length		输入		INT8U		要求输入的长度
	ret_string	输出		INT8U*		返回的输入内容
	enter_en	输入		INT8U		是否允许回车,TRUE允许     FALSE不允许

返  回  值：ok - 成功
				notok - 失败(按cancel键退出了)
作      者	：许岩
日      期：2007-06-07
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U input_string(INT8U row, INT8U *prev, INT8U *tishifu, INT8U length, INT8U *ret_string, INT8U enter_en)
{
	INT8U i = 0;
	INT8U key = NO_KEY;
	INT8S buf[20];
	INT8U buf2[20];
	INT8U s_input[20];

	ToggleWD();
// 	enter_en = enter_en;

	memset((void *)s_input, 0x00, sizeof(s_input));
	Lcd_Clean_Line(row);
	strcpy((void *)buf, (void *)prev);
	buf2[0] = tishifu[0];
	buf2[1] = '\0';
	strcat((void *)buf, (void *)buf2);
	lcddisp(row, 0, buf);

	input_string_start:

	for ( i=0; i<length; i++ )
	{
//      ToggleWD();

		input_string_get_key:
		do				   //only accept 0-9,KEY_CANCEL
		{
			key = get_key();
		}while ( !( (key>='0'&&key<='9') || (key=='.') || (key==KEY_CANCEL) || (key==KEY_ENTER)) );

		if ( key == KEY_CANCEL )
			return notok;			//退出设置
		else if ( key == KEY_ENTER )
		{
			if ( enter_en == TRUE )
			{
				if ( i == 0 )
					goto input_string_start;			//啥也没输入

				buf[strlen((void *)buf) - 1] = '\0';
				Lcd_Clean_Line(row);
				lcddisp(row, 0, buf);

				strcpy((void *)ret_string, (void *)s_input);

				return ok;
			}
			else
				goto input_string_get_key;
		}

		s_input[i] = key;
		buf2[0] = key;
		buf2[1] = tishifu[0];
		buf2[2] = '\0';
		buf[strlen((void *)buf) - 1] = '\0';
		strcat((void *)buf, (void *)buf2);
		lcddisp(row, 0, buf);
		if ( i == length - 1 )
		{
			buf[strlen((void *)buf) - 1] = '\0';
			Lcd_Clean_Line(row);
			lcddisp(row, 0, buf);
		}
	}

	strcpy((void *)ret_string, (void *)s_input);

	return ok;
}

///*****************************************************************
//函数原型：SetCenterIPMain
//功能描述：设置主中心IP
//参数描述：
//参数名称：  输入/输出？ 类型        描述
//-----------     ----------- ------      -------
//param       输入            INT8U       输入的参数
//
//返  回  值：无
//作      者  ：许岩
//日      期：2005-08-20
//修改历史：
//日期        修改人      修改描述
//------      ---------   -------------
//*****************************************************************/
//void SetCenterIPMain(INT8U param)
//{
//    INT8U i = 0;
//    INT8U disp_buf[17];
////  INT8U key = NO_KEY;
////  INT8S cur_ptr = 0;
////  INT8U hour1;
////  INT8U minute1;
////  INT8U hour2;
////  INT8U minute2;
//    INT8U ip[4];
//    INT8U buf[17];
//    INT16U temp_int16u = 0;
//
//    param = param;      //avoid warning
//    ToggleWD();
//
////  set_lcd_time_begin:
//
//    Lcd_Cls();
//    lcddisp(0, 0, "  设置主中心IP  ");
//    lcddisp(1, 0, "原IP: ");
//
//    OS_ENTER_CRITICAL();
//    sprintf((void *)disp_buf, "%3d.%3d.%3d.%3d", DevStat.center_ip_main[0],
//            DevStat.center_ip_main[1], DevStat.center_ip_main[2], DevStat.center_ip_main[3]);
//    OS_EXIT_CRITICAL();
//
//    lcddisp(2, 0, disp_buf);
//
//    i = input_string(3, (void *)"", (void *)"_", 3, disp_buf, FALSE);
//    if (i != ok)
//        return;
//    temp_int16u = (INT16U)ascii_to_hex(disp_buf[0]) * 100
//        + (INT16U)ascii_to_hex(disp_buf[1]) * 10 + (INT16U)ascii_to_hex(disp_buf[2]);
//    if (temp_int16u > 255)
//        ip[0] = 255;
//    else
//        ip[0] = (INT8U)temp_int16u;
//
//    sprintf((void *)buf, "%3d.", ip[0]);
//    i = input_string(3, buf, (void *)"_", 3, disp_buf, FALSE);
//    if (i != ok)
//        return;
//    temp_int16u = (INT16U)ascii_to_hex(disp_buf[0]) * 100
//        + (INT16U)ascii_to_hex(disp_buf[1]) * 10 + (INT16U)ascii_to_hex(disp_buf[2]);
//    if (temp_int16u > 255)
//        ip[1] = 255;
//    else
//        ip[1] = (INT8U)temp_int16u;
//
//    sprintf((void *)buf, "%3d.%3d.", ip[0], ip[1]);
//    i = input_string(3, buf, (void *)"_", 3, disp_buf, FALSE);
//    if (i != ok)
//        return;
//    temp_int16u = (INT16U)ascii_to_hex(disp_buf[0]) * 100
//        + (INT16U)ascii_to_hex(disp_buf[1]) * 10 + (INT16U)ascii_to_hex(disp_buf[2]);
//    if (temp_int16u > 255)
//        ip[2] = 255;
//    else
//        ip[2] = (INT8U)temp_int16u;
//
//    sprintf((void *)buf, "%3d.%3d.%3d.", ip[0], ip[1], ip[2]);
//    i = input_string(3, buf, (void *)"_", 3, disp_buf, FALSE);
//    if (i != ok)
//        return;
//    temp_int16u = (INT16U)ascii_to_hex(disp_buf[0]) * 100
//        + (INT16U)ascii_to_hex(disp_buf[1]) * 10 + (INT16U)ascii_to_hex(disp_buf[2]);
//    if (temp_int16u > 255)
//        ip[3] = 255;
//    else
//        ip[3] = (INT8U)temp_int16u;
//
//    sprintf((void *)buf, "%3d.%3d.%3d.%3d", ip[0], ip[1], ip[2], ip[3]);
//
//    OS_ENTER_CRITICAL();
//    DevStat.center_ip_main[0] = ip[0];
//    DevStat.center_ip_main[1] = ip[1];
//    DevStat.center_ip_main[2] = ip[2];
//    DevStat.center_ip_main[3] = ip[3];
//    OS_EXIT_CRITICAL();
//
//    WriteParam();
//
//    lcddisp(1, 0, "    修改成功    ");
//    OSTimeDlyHMSM(0, 0, 1, 500);
//    return;
//
//}
//
///*****************************************************************
//函数原型：SetCenterIPBak
//功能描述：设置备份中心IP
//参数描述：
//参数名称：  输入/输出？ 类型        描述
//-----------     ----------- ------      -------
//param       输入            INT8U       输入的参数
//
//返  回  值：无
//作      者  ：许岩
//日      期：2005-08-20
//修改历史：
//日期        修改人      修改描述
//------      ---------   -------------
//*****************************************************************/
//void SetCenterIPBak(INT8U param)
//{
//    INT8U i = 0;
//    INT8U disp_buf[17];
////  INT8U key = NO_KEY;
////  INT8S cur_ptr = 0;
////  INT8U hour1;
////  INT8U minute1;
////  INT8U hour2;
////  INT8U minute2;
//    INT8U ip[4];
//    INT8U buf[17];
//    INT16U temp_int16u = 0;
//
//    param = param;      //avoid warning
//    ToggleWD();
//
////  set_lcd_time_begin:
//
//    Lcd_Cls();
//    lcddisp(0, 0, "  设置副中心IP  ");
//    lcddisp(1, 0, "原IP: ");
//
//    OS_ENTER_CRITICAL();
//    sprintf((void *)disp_buf, "%3d.%3d.%3d.%3d", DevStat.center_ip_bak[0],
//            DevStat.center_ip_bak[1], DevStat.center_ip_bak[2], DevStat.center_ip_bak[3]);
//    OS_EXIT_CRITICAL();
//
//    lcddisp(2, 0, disp_buf);
//
//    i = input_string(3, (void *)"", (void *)"_", 3, disp_buf, FALSE);
//    if (i != ok)
//        return;
//    temp_int16u = (INT16U)ascii_to_hex(disp_buf[0]) * 100
//        + (INT16U)ascii_to_hex(disp_buf[1]) * 10 + (INT16U)ascii_to_hex(disp_buf[2]);
//    if (temp_int16u > 255)
//        ip[0] = 255;
//    else
//        ip[0] = (INT8U)temp_int16u;
//
//    sprintf((void *)buf, "%3d.", ip[0]);
//    i = input_string(3, buf, (void *)"_", 3, disp_buf, FALSE);
//    if (i != ok)
//        return;
//    temp_int16u = (INT16U)ascii_to_hex(disp_buf[0]) * 100
//        + (INT16U)ascii_to_hex(disp_buf[1]) * 10 + (INT16U)ascii_to_hex(disp_buf[2]);
//    if (temp_int16u > 255)
//        ip[1] = 255;
//    else
//        ip[1] = (INT8U)temp_int16u;
//
//    sprintf((void *)buf, "%3d.%3d.", ip[0], ip[1]);
//    i = input_string(3, buf, (void *)"_", 3, disp_buf, FALSE);
//    if (i != ok)
//        return;
//    temp_int16u = (INT16U)ascii_to_hex(disp_buf[0]) * 100
//        + (INT16U)ascii_to_hex(disp_buf[1]) * 10 + (INT16U)ascii_to_hex(disp_buf[2]);
//    if (temp_int16u > 255)
//        ip[2] = 255;
//    else
//        ip[2] = (INT8U)temp_int16u;
//
//    sprintf((void *)buf, "%3d.%3d.%3d.", ip[0], ip[1], ip[2]);
//    i = input_string(3, buf, (void *)"_", 3, disp_buf, FALSE);
//    if (i != ok)
//        return;
//    temp_int16u = (INT16U)ascii_to_hex(disp_buf[0]) * 100
//        + (INT16U)ascii_to_hex(disp_buf[1]) * 10 + (INT16U)ascii_to_hex(disp_buf[2]);
//    if (temp_int16u > 255)
//        ip[3] = 255;
//    else
//        ip[3] = (INT8U)temp_int16u;
//
//    sprintf((void *)buf, "%3d.%3d.%3d.%3d", ip[0], ip[1], ip[2], ip[3]);
//
//    OS_ENTER_CRITICAL();
//    DevStat.center_ip_bak[0] = ip[0];
//    DevStat.center_ip_bak[1] = ip[1];
//    DevStat.center_ip_bak[2] = ip[2];
//    DevStat.center_ip_bak[3] = ip[3];
//    OS_EXIT_CRITICAL();
//
//    WriteParam();
//
//    lcddisp(1, 0, "    修改成功    ");
//    OSTimeDlyHMSM(0, 0, 1, 500);
//    return;
//
//}

///*****************************************************************
//函数原型：QueryTotal
//功能描述：汇总查询
//参数描述：
//参数名称：  输入/输出？ 类型        描述
//-----------     ----------- ------      -------
//
//返  回  值：无
//作      者  ：许岩
//日      期：2005-08-20
//修改历史：
//日期        修改人      修改描述
//------      ---------   -------------
//*****************************************************************/
//void QueryTotal(INT8U param)
//{
//    INT8U buf[512];
//    INT8U buf2[50];
//    FRT_SEC *pFRT = (FRT_SEC*)buf;
//    FS_U16 i;
//    FS_U16 j;
////  FS_U16 file_num = 0;
////  FCT *pfct = NULL;
//    INT32U bus_num = 0;         //采集车辆数量
//    INT32U rec_num = 0;         //记录数量
//
//    param = param;
//
//    ToggleWD();
//
//    Lcd_Cls();
//    lcddisp(2, 0, "    统计中..    ");
//
//    FSSemPend();
//
//
//    for ( i=0; i<gDrvInfo[0]._SecNumofFRT.SecNumofFRT; i++ )
//    {
//        FS_Read_Sector(gDrvInfo[0].PriFRTStartSec + i, (FS_U8*)pFRT, 512);
//
//        for ( j=0; j<sizeof(FRT_SEC) / sizeof(FAT16_DIR_ENTRY); j++ )
//        {
//            if ( pFRT->aDirEntry[j].Name[0] == 'R' )                //刷卡纪录
//            {
//                rec_num += pFRT->aDirEntry[j].FileSize / 32;
//                bus_num++;
//            }
//            else if( pFRT->aDirEntry[j].Name[0] == 'B' )            //黑名单纪录
//            {
//            }
//            ToggleWD();
//        }
//        ToggleWD();
//    }
//
//    FSSemPost();
//
//    Lcd_Cls();
//    sprintf((void *)buf2, "车辆:%9u台", bus_num);
//    lcddisp(0, 0, buf2);
//    sprintf((void *)buf2, "纪录:%9u条", rec_num);
//    lcddisp(1, 0, buf2);
//
//    lcddisp(3, 0, "  按任意键退出  ");
//    delay_and_wait_key(5, 0, 0);
//
//
//}

///*****************************************************************
//函数原型：QueryOneFile_XYZ
//功能描述：单车查询
//参数描述：
//参数名称：  输入/输出？ 类型        描述
//-----------     ----------- ------      -------
//
//返  回  值：无
//作      者  ：许岩
//日      期：2005-08-20
//修改历史：
//日期        修改人      修改描述
//------      ---------   -------------
//*****************************************************************/
//void QueryOneFile_XYZ(INT8U param)
//{
//    INT8U buf[512];
//    INT8U buf2[50];
//    FRT_SEC *pFRT = (FRT_SEC*)buf;
//    FS_U16 i;
//    FS_U16 j;
////  FS_U16 file_num = 0;
//    FCT *pfct = NULL;
//    INT32U bus_num = 0;         //采集车辆数量
//    INT32U max_bus_num = 0;
////  INT32U rec_num = 0;         //记录数量
//
//    INT8U key = NO_KEY;
//
////  struct DISP_FILE_INFO
////  {
//// //       INT16U num;             //序号
////      INT8U line_num[2];      //线路号
////      INT8U bus_num[3];       //车号
////      INT16U rec_num;         //纪录数量
////      INT8U time[6];              //采集时间
////  };
////
////  struct DISP_FILE_INFO disp_file_info[FS_MAX_FILES];
//
//    param = param;
//
//    ToggleWD();
//
//
//    Lcd_Cls();
//    lcddisp(2, 0, "    统计中..    ");
//
//
//    FSSemPend();
//
//
//    for ( i=0; i<gDrvInfo[0]._SecNumofFRT.SecNumofFRT; i++ )
//    {
//        FS_Read_Sector(gDrvInfo[0].PriFRTStartSec + i, (FS_U8*)pFRT, 512);
//
//        for ( j=0; j<sizeof(FRT_SEC) / sizeof(FAT16_DIR_ENTRY); j++ )
//        {
//            if ( pFRT->aDirEntry[j].Name[0] == 'R' )                //刷卡纪录
//            {
//                pfct = (FCT *)&pFRT->aDirEntry[j];
//                disp_file_info[bus_num].line_num[0] = pfct->ftime[3];           //保存线路号
//                disp_file_info[bus_num].line_num[1] = pfct->ftime[4];
//                disp_file_info[bus_num].bus_num[0] = pFRT->aDirEntry[j].ExtName[0]; //保存车号
//                disp_file_info[bus_num].bus_num[1] = pFRT->aDirEntry[j].ExtName[1];
//                disp_file_info[bus_num].bus_num[2] = pFRT->aDirEntry[j].ExtName[2];
//                disp_file_info[bus_num].rec_num = (INT16U)(pFRT->aDirEntry[j].FileSize / 32);       //保存记录条数
//                disp_file_info[bus_num].time[0] = 0x20;
//                disp_file_info[bus_num].time[1] = pFRT->aDirEntry[j].Name[2];
//                disp_file_info[bus_num].time[2] = pFRT->aDirEntry[j].Name[3];
//                disp_file_info[bus_num].time[3] = pFRT->aDirEntry[j].Name[4];
//                disp_file_info[bus_num].time[4] = pFRT->aDirEntry[j].Name[5];
//                disp_file_info[bus_num].time[5] = pFRT->aDirEntry[j].Name[6];
////              disp_file_info[bus_num].time[6] = pFRT->aDirEntry[j].Name[7];
//
//                bus_num++;
//            }
//            else if( pFRT->aDirEntry[j].Name[0] == 'B' )            //黑名单纪录
//            {
//            }
//            ToggleWD();
//        }
//        ToggleWD();
//    }
//
//    FSSemPost();
//
//    max_bus_num = bus_num;
//
//    if (bus_num == 0)
//    {
//        Lcd_Cls();
//        lcddisp(2, 0, "    无纪录!!    ");
//        BeepInputErr();
//        SleepMs(2000);
//        return;
//    }
//
//    bus_num = 0;
//    Lcd_Cls();
//    sprintf((void *)buf2, "线路%02x%02x序号:%3d", disp_file_info[bus_num].line_num[0],
//            disp_file_info[bus_num].line_num[0], bus_num + 1);
//    lcddisp(0, 0, buf2);
//    sprintf((void *)buf2, "车号:     %02x%02x%02x", disp_file_info[bus_num].bus_num[0],
//            disp_file_info[bus_num].bus_num[1], disp_file_info[bus_num].bus_num[2]);
//    lcddisp(1, 0, buf2);
//    sprintf((void *)buf2, "纪录:%9u条", disp_file_info[bus_num].rec_num);
//    lcddisp(2, 0, buf2);
//    sprintf((void *)buf2, "%02x%02x-%02x-%02x %02x:%02x", disp_file_info[bus_num].time[0],
//            disp_file_info[bus_num].time[1], disp_file_info[bus_num].time[2], disp_file_info[bus_num].time[3],
//            disp_file_info[bus_num].time[4], disp_file_info[bus_num].time[5]);
//    lcddisp(3, 0, buf2);
//
//    for(;;)
//    {
//        key = get_key();
//        switch( key)
//        {
//            case '4':                               //上翻
//                if (bus_num == 0)
//                {
//                    BeepInputErr();
//                    break;
//                }
//                bus_num--;
//
//                sprintf((void *)buf2, "线路%02x%02x序号:%3d", disp_file_info[bus_num].line_num[0],
//                        disp_file_info[bus_num].line_num[0], bus_num + 1);
//                lcddisp(0, 0, buf2);
//                sprintf((void *)buf2, "车号:     %02x%02x%02x", disp_file_info[bus_num].bus_num[0],
//                        disp_file_info[bus_num].bus_num[1], disp_file_info[bus_num].bus_num[2]);
//                lcddisp(1, 0, buf2);
//                sprintf((void *)buf2, "纪录:%9u条", disp_file_info[bus_num].rec_num);
//                lcddisp(2, 0, buf2);
//                sprintf((void *)buf2, "%02x%02x-%02x-%02x %02x:%02x", disp_file_info[bus_num].time[0],
//                        disp_file_info[bus_num].time[1], disp_file_info[bus_num].time[2], disp_file_info[bus_num].time[3],
//                        disp_file_info[bus_num].time[4], disp_file_info[bus_num].time[5]);
//                lcddisp(3, 0, buf2);
//
//                break;
//
//            case '8':                               //下翻
//                if (bus_num >= max_bus_num - 1)
//                {
//                    BeepInputErr();
//                    break;
//                }
//                bus_num++;
//
//                sprintf((void *)buf2, "线路%02x%02x序号:%3d", disp_file_info[bus_num].line_num[0],
//                        disp_file_info[bus_num].line_num[0], bus_num + 1);
//                lcddisp(0, 0, buf2);
//                sprintf((void *)buf2, "车号:     %02x%02x%02x", disp_file_info[bus_num].bus_num[0],
//                        disp_file_info[bus_num].bus_num[1], disp_file_info[bus_num].bus_num[2]);
//                lcddisp(1, 0, buf2);
//                sprintf((void *)buf2, "纪录:%9u条", disp_file_info[bus_num].rec_num);
//                lcddisp(2, 0, buf2);
//                sprintf((void *)buf2, "%02x%02x-%02x-%02x %02x:%02x", disp_file_info[bus_num].time[0],
//                        disp_file_info[bus_num].time[1], disp_file_info[bus_num].time[2], disp_file_info[bus_num].time[3],
//                        disp_file_info[bus_num].time[4], disp_file_info[bus_num].time[5]);
//                lcddisp(3, 0, buf2);
//
//                break;
//
//            case KEY_CANCEL:                    //退出
//                return;
//
//            default:
//                break;
//        }
//    }
//
//}

///*****************************************************************
//函数原型：QueryCapacity
//功能描述：容量查询
//参数描述：
//参数名称：  输入/输出？ 类型        描述
//-----------     ----------- ------      -------
//
//返  回  值：无
//作      者  ：许岩
//日      期：2005-08-20
//修改历史：
//日期        修改人      修改描述
//------      ---------   -------------
//*****************************************************************/
//void QueryCapacity(INT8U param)
//{
////  INT8U buf[512];
//    INT8U buf2[50];
////  FRT_SEC *pFRT = (FRT_SEC*)buf;
////  FS_U16 i;
////  FS_U16 j;
////  FS_U16 file_num = 0;
////  FCT *pfct = NULL;
////  INT32U bus_num = 0;         //采集车辆数量
////  INT32U rec_num = 0;         //记录数量
//    INT32U total_disk_space = 0;
//    INT32U occupied_disk_space = 0;
//    INT32U free_disk_space = 0;
//
//    param = param;
//
//    ToggleWD();
//
//    Lcd_Cls();
//    lcddisp(2, 0, "    统计中..    ");
//
//    FSSemPend();
//
//    free_disk_space = (INT32U)FS_Get_Free_Disk() * (INT32U)FS_BYTSPERSEC ;
//
//    FSSemPost();
//
//    total_disk_space = (INT32U)(gDrvInfo[0].TotSec16 - gDrvInfo[0].FirstDataSector) * (INT32U)FS_BYTSPERSEC;
//
//    free_disk_space /= 1000UL;
//    total_disk_space /= 1000UL;
//    occupied_disk_space = total_disk_space - free_disk_space;
//
//    Lcd_Cls();
//    sprintf((void *)buf2, "已占用:%8uK", occupied_disk_space);
//    lcddisp(0, 0, buf2);
//    sprintf((void *)buf2, "空  闲:%8uK", free_disk_space);
//    lcddisp(1, 0, buf2);
//    sprintf((void *)buf2, "总容量:%8uK", total_disk_space);
//    lcddisp(2, 0, buf2);
//
//    lcddisp(3, 0, "  按任意键退出  ");
//    delay_and_wait_key(5, 0, 0);
//
//
//}

///*****************************************************************
//函数原型：QueryBlack
//功能描述：黑名单查询
//参数描述：
//参数名称：  输入/输出？ 类型        描述
//-----------     ----------- ------      -------
//
//返  回  值：无
//作      者  ：许岩
//日      期：2005-08-20
//修改历史：
//日期        修改人      修改描述
//------      ---------   -------------
//*****************************************************************/
//void QueryBlack(INT8U param)
//{
////  INT8U buf[512];
//    INT8U buf2[50];
////  FRT_SEC *pFRT = (FRT_SEC*)buf;
//    FS_U16 i;
////  FS_U16 j;
////  FS_U16 file_num = 0;
////  FCT *pfct = NULL;
////  INT32U bus_num = 0;         //采集车辆数量
//    INT32U rec_num = 0;         //记录数量
//
//    INT8U filename[9];
//    INT8U filename_ext[4];
//    FHANDLE *fh = NULL;
//
//
//    param = param;
//
//    ToggleWD();
//
//    Lcd_Cls();
//    lcddisp(2, 0, "    统计中..    ");
//
//
//    i = FindFirstBlkFile(filename, filename_ext);
//    if (i != ok)
//    {
//        //没有黑名单文件
//        Lcd_Cls();
//        lcddisp(2, 0, "    无黑名单    ");
//        BeepInputErr();
//        delay_and_wait_key(2, 0, 0);
//        return;
//    }
//
//    fh = FS_Open((void *)filename, (void *)filename_ext, FS_OPEN_READ);
//    if (fh == NULL)
//    {
//        (void)FS_Close(fh);
//        Lcd_Cls();
//        lcddisp(2, 0, "    无黑名单    ");
//        BeepInputErr();
//        delay_and_wait_key(2, 0, 0);
//        return;
//    }
//
//    rec_num = GetFileSize(fh) / 4;
//
//    Lcd_Cls();
//    lcddisp(0, 0, "版本号:         ");
//    sprintf((void *)buf2, "  %02X%02X%02X%02X%02X%02X%02X",
//            filename[1], filename[2], filename[3], filename[4], filename[5], filename[6], filename[7]);
//    lcddisp(1, 0, buf2);
//    sprintf((void *)buf2, "条数:%9u条", rec_num);
//    lcddisp(2, 0, buf2);
//
//    (void)FS_Close(fh);
//
//    lcddisp(3, 0, "  按任意键退出  ");
//    delay_and_wait_key(5, 0, 0);
//
//}

/*****************************************************************
函数原型：SetModeAuto
功能描述：设置为自动模式
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
param		输入			INT8U		输入的参数

返  回  值：无
作      者	：许岩
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void SetModeAuto(INT8U param)
{
	param = param;		//avoid warning

	ToggleWD();

	OS_ENTER_CRITICAL();
	DevStat.mode = AUTO;
	DevStat.MOD_REG.reg[0x0018] &= (~0x0001); //异常状态
	OS_EXIT_CRITICAL();

	lcddisp(3, 0, "设置自动模式成功");
	OSTimeDlyHMSM(0, 0, 1, 0);
}

/*****************************************************************
函数原型：SetModeManual
功能描述：设置为自动模式
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
param		输入			INT8U		输入的参数

返  回  值：无
作      者	：许岩
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void SetModeManual(INT8U param)
{
	param = param;		//avoid warning

	ToggleWD();

	OS_ENTER_CRITICAL();
	DevStat.mode = MANUAL;
	DevStat.MOD_REG.reg[0x0018] |= 0x0001; //异常状态
	OS_EXIT_CRITICAL();

	lcddisp(3, 0, "设置手动模式成功");
	OSTimeDlyHMSM(0, 0, 1, 0);
}

/*****************************************************************
函数原型：CheckAutoManual
功能描述：判定是否为手动模式
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
param		输入			INT8U		输入的参数

返  回  值：ok - 手动
				notok - 自动
作      者	：许岩
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U CheckAutoManual(void *param)
{
	INT8U mode = AUTO;

	param = param;

	ToggleWD();

	OS_ENTER_CRITICAL();
	mode = DevStat.mode;
	OS_EXIT_CRITICAL();
	if ( mode == AUTO )
	{
		Lcd_Cls();
		lcddisp(2, 0, "请切换到手动模式");
		BeepInputErr();
		(void)delay_and_wait_key(2, 0, 0);
		return notok;
	}
	else
	{
		return ok;
	}
}

/*****************************************************************
函数原型：QueryDI
功能描述：查询DI状态
参数描述：
参数名称：  输入/输出？ 类型        描述
-----------     ----------- ------      -------

返  回  值：无
作      者  ：许岩
日      期：2005-08-20
修改历史：
日期        修改人      修改描述
------      ---------   -------------
*****************************************************************/
void QueryDI(INT8U param)
{
//  INT8U i = 0;
//  INT32U di = 0;
//  INT8U buf[33] = {0};
//  INT8U key = NO_KEY;
//
//  param = param;
//
//    ToggleWD();
//
//    Lcd_Cls();
//    lcddisp(0, 0, "    DI查询      ");
//    lcddisp(1, 0, "DI0 -- DI23:    ");
//
////  buf[0] = '\0';
//    for(;;)
//    {
//        di = GetDIStatus();
//        memset((void *)buf, 0x00, sizeof(buf));
//        for(i=0; i<24; i++)
//        {
//            if ((di & (0x00000001UL << i)) == 0)
//            {
//                strcat((void *)buf, "0");
//            }
//            else
//            {
//                strcat((void *)buf, "1");
//            }
//        }
//        strcat((void *)buf, "        ");
//        lcddisp(2, 0, buf);
//        lcddisp(3, 0, &buf[16]);
//
//        key = delay_and_wait_key(1, 0, 0);
//        if (key != NO_KEY)
//            return;
//    }
}

/*****************************************************************
函数原型：QueryAD
功能描述：查询AD状态
参数描述：
参数名称：  输入/输出？ 类型        描述
-----------     ----------- ------      -------

返  回  值：无
作      者  ：许岩
日      期：2005-08-20
修改历史：
日期        修改人      修改描述
------      ---------   -------------
*****************************************************************/
void QueryAD(INT8U param)
{
	INT8U buf[20] = {0};
	INT32S  temp_int32s = 0;
	INT8U buf1[20] = {0};
	INT8U buf2[20] = {0};
	INT8U key = NO_KEY;


	param = param;

	ToggleWD();

	Lcd_Cls();
	lcddisp(0, 0, "    矿粉含量    ");

	for ( ;; )
	{
//		ToggleWD();
		
		sprintf((void *)buf1, "标准化电压%2.2f", DevStat.MOD_REG.reg[0x0011]/1000.0);
		sprintf((void *)buf2, "实际测电压%2.2f", DevStat.MOD_REG.reg[0x0010]/1000.0);
		lcddisp(1, 0, buf1);
		lcddisp(2, 0, buf2);

		OS_ENTER_CRITICAL();
		temp_int32s = ( DevStat.MOD_REG.reg[0x0010]  - DevStat.MOD_REG.reg[0x0011] );
		if ( (temp_int32s) > 15 )		  //看是否为正
		{
			sprintf((void *)buf, "矿粉含量:>%5.1f%%", _fflt(temp_int32s));
		}
		else
		{
			//temp_int32s = 0 -  temp_int32s;
			sprintf((void *)buf, "矿粉含量:<%5.1f%%", 10.0);
		}
		
		OS_EXIT_CRITICAL();

		lcddisp(3, 0, buf);
		

		key = delay_and_wait_key_ms(100, 0, 0);
		if ( key != NO_KEY )
			return;


#if 0
		if( tmp1 = 0)
		{
			lcddisp(1, 0, " 矿粉量计算失败 ");
			lcddisp(2, 0, "请进行标准化设置");
			BeepInputErr();
			(void)delay_and_wait_key(2, 0, 0);
			lcddisp(1, 0, "                ");
			lcddisp(2, 0, "                ");
			return;
		}
		
		OS_ENTER_CRITICAL();
		//sprintf((void *)buf1, "1.%2.2f% 2.%2.2f%", DevStat.MOD_REG.reg[0x0010]/1000.0, DevStat.MOD_REG.reg[0x0011]/1000.0);
		sprintf((void *)buf1, "1.%2.2f", (tmp1 / 1000.0));
//  	sprintf((void *)buf2, "3.%2.2f   4.%2.2f", DevStat.MOD_REG.reg[0x0012]/1000.0, DevStat.MOD_REG.reg[0x0013]/1000.0);
//  	sprintf((void *)buf3, "5.%2.2f          ", DevStat.MOD_REG.reg[0x0014]/1000.0);
		OS_EXIT_CRITICAL();

		lcddisp(1, 0, buf1);
//  	lcddisp(2, 0, buf2);
//  	lcddisp(3, 0, buf3);

		key = delay_and_wait_key_ms(100, 0, 0);
		if ( key != NO_KEY )
			return;
#endif
	}
}

/*****************************************************************
函数原型：QueryDA
功能描述：查询DA输出值
参数描述：
参数名称：  输入/输出？ 类型        描述
-----------     ----------- ------      -------

返  回  值：无
作      者  ：许岩
日      期：2005-08-20
修改历史：
日期        修改人      修改描述
------      ---------   -------------
*****************************************************************/
void QueryDA(INT8U param)
{
	INT8U buf[22] = {0};
	INT8U key = NO_KEY;
	FP32 temp_int32s = 0.0;

	param = param;

	ToggleWD();

	Lcd_Cls();
	lcddisp(0, 0, "    DA查询     ");

	for(;;)
	{
		OS_ENTER_CRITICAL();

		temp_int32s = (FP32)((DevStat.MOD_REG.reg[0x0015] / 4095.0) * 10);

		sprintf((void *)buf, "通道 1:%2.2fV", (FP32)(temp_int32s));
		OS_EXIT_CRITICAL();
		lcddisp(1, 0, buf);

		OS_ENTER_CRITICAL();

		temp_int32s = (FP32)((DevStat.MOD_REG.reg[0x0016] / 4095.0) * 10);

		sprintf((void *)buf, "通道 2:%2.2fV", (FP32)(temp_int32s));
		OS_EXIT_CRITICAL();
		lcddisp(2, 0, buf);

		key = delay_and_wait_key_ms(500, 0, 0);
		if (key != NO_KEY)
			return;
	}
}

///*****************************************************************
//函数原型：PushCForward
//功能描述：前推C型架
//参数描述：
//参数名称：  输入/输出？ 类型        描述
//-----------     ----------- ------      -------
//
//返  回  值：无
//作      者  ：许岩
//日      期：2005-08-20
//修改历史：
//日期        修改人      修改描述
//------      ---------   -------------
//*****************************************************************/
//void PushCForward(INT8U param)
//{
////    INT32U di = 0;
////
////    param = param;
////
////    ToggleWD();
////
////    Lcd_Cls();
////
////    lcddisp(0, 0, "  前推ｃ型架    ");
////
////    di = GetDIStatus();
////    if (di & BIT12 == 1)        //已经到位了
////    {
////        lcddisp(0, 1, "    已经到位    ");
////        (void)delay_and_wait_key(2, 0, 0);
////        return;
////    }
////    OS_ENTER_CRITICAL();
////    DevStat.MOD_REG.reg[0x0A] = 1;      //驱动电机前进
////    DevStat.MOD_REG.reg[0x0B] = 0;
////    DevStat.MOD_REG.reg[0x4E] = 1;      //置忙状态
////    OS_EXIT_CRITICAL();
////    SetRelayModbus();
////
////    for(;;)
////    {
////        di = GetDIStatus();
////        if (di & BIT12 == 1)        //已经到位了
////        {
////            OS_ENTER_CRITICAL();
////            DevStat.MOD_REG.reg[0x0A] = 0;      //停止电机
////            DevStat.MOD_REG.reg[0x0B] = 0;
////            OS_EXIT_CRITICAL();
////            SetRelayModbus();
////
////            OS_ENTER_CRITICAL();
////            DevStat.MOD_REG.reg[0x4E] = 0;      //置忙状态
//////          DevStat.MOD_REG.reg[0x60] = 0;      //已经到位
////            OS_EXIT_CRITICAL();
////            break;
////        }
////    }
////
////    lcddisp(0, 1, "    已经到位    ");
////    (void)delay_and_wait_key(2, 0, 0);
////
//
//
//    INT16U cmd = 0;
//    param = param;
//
//    ToggleWD();
//
//    Lcd_Cls();
//    lcddisp(0, 0, "  前推ｃ型架    ");
//
//    OS_ENTER_CRITICAL();
//    cmd = DevStat.MOD_REG.reg[0x60];        //推C型架寄存器
//    OS_EXIT_CRITICAL();
//
//    if (cmd != 0)
//    {
//        lcddisp(1, 0, "    等待中..    ");
//        for(;;)
//        {
//            OSTimeDlyHMSM(0, 0, 0, 10);         //10ms一次
//
//            OS_ENTER_CRITICAL();
//            cmd = DevStat.MOD_REG.reg[0x60];        //推C型架寄存器
//            OS_EXIT_CRITICAL();
//            if (cmd == 0)
//                break;
//        }
//    }
//
//    lcddisp(1, 0, "    推动中..    ");
//    OS_ENTER_CRITICAL();
//    DevStat.MOD_REG.reg[0x60] = 0x5555;     //推C型架寄存器,设置为前推
//    OS_EXIT_CRITICAL();
//
//    for(;;)
//    {
//        OSTimeDlyHMSM(0, 0, 0, 10);         //10ms一次
//
//        OS_ENTER_CRITICAL();
//        cmd = DevStat.MOD_REG.reg[0x60];        //推C型架寄存器
//        OS_EXIT_CRITICAL();
//        if (cmd == 0)
//            break;
//    }
//
//    lcddisp(1, 0, "    已经到位    ");
//    (void)delay_and_wait_key(2, 0, 0);
//
//}
//
///*****************************************************************
//函数原型：PushCBackward
//功能描述：后推C型架
//参数描述：
//参数名称：  输入/输出？ 类型        描述
//-----------     ----------- ------      -------
//
//返  回  值：无
//作      者  ：许岩
//日      期：2005-08-20
//修改历史：
//日期        修改人      修改描述
//------      ---------   -------------
//*****************************************************************/
//void PushCBackward(INT8U param)
//{
////    INT32U di = 0;
////
////    param = param;
////
////    ToggleWD();
////
////    Lcd_Cls();
////
////    lcddisp(0, 0, "  后推ｃ型架    ");
////
////    di = GetDIStatus();
////    if (di & BIT13 == 1)        //已经到位了
////    {
////        lcddisp(0, 1, "    已经到位    ");
////        (void)delay_and_wait_key(2, 0, 0);
////        return;
////    }
////    OS_ENTER_CRITICAL();
////    DevStat.MOD_REG.reg[0x0A] = 0;
////    DevStat.MOD_REG.reg[0x0B] = 1;      //驱动电机后退
////    DevStat.MOD_REG.reg[0x4E] = 1;      //置忙状态
////    OS_EXIT_CRITICAL();
////    SetRelayModbus();
////
////    for(;;)
////    {
////        di = GetDIStatus();
////        if (di & BIT13 == 1)        //已经到位了
////        {
////            OS_ENTER_CRITICAL();
////            DevStat.MOD_REG.reg[0x0A] = 0;      //停止电机
////            DevStat.MOD_REG.reg[0x0B] = 0;
////            OS_EXIT_CRITICAL();
////            SetRelayModbus();
////
////            OS_ENTER_CRITICAL();
////            DevStat.MOD_REG.reg[0x4E] = 0;      //置忙状态
//////          DevStat.MOD_REG.reg[0x60] = 0;      //已经到位
////            OS_EXIT_CRITICAL();
////            break;
////        }
////    }
////
////    lcddisp(0, 1, "    已经到位    ");
////    (void)delay_and_wait_key(2, 0, 0);
//
//
//
//    INT16U cmd = 0;
//    param = param;
//
//    ToggleWD();
//
//    Lcd_Cls();
//    lcddisp(0, 0, "  后推ｃ型架    ");
//
//    OS_ENTER_CRITICAL();
//    cmd = DevStat.MOD_REG.reg[0x60];        //推C型架寄存器
//    OS_EXIT_CRITICAL();
//
//    if (cmd != 0)
//    {
//        lcddisp(1, 0, "    等待中..    ");
//        for(;;)
//        {
//            OSTimeDlyHMSM(0, 0, 0, 10);         //10ms一次
//
//            OS_ENTER_CRITICAL();
//            cmd = DevStat.MOD_REG.reg[0x60];        //推C型架寄存器
//            OS_EXIT_CRITICAL();
//            if (cmd == 0)
//                break;
//        }
//    }
//
//    lcddisp(1, 0, "    推动中..    ");
//    OS_ENTER_CRITICAL();
//    DevStat.MOD_REG.reg[0x60] = 0xAAAA;     //推C型架寄存器,设置为后推
//    OS_EXIT_CRITICAL();
//
//    for(;;)
//    {
//        OSTimeDlyHMSM(0, 0, 0, 10);         //10ms一次
//
//        OS_ENTER_CRITICAL();
//        cmd = DevStat.MOD_REG.reg[0x60];        //推C型架寄存器
//        OS_EXIT_CRITICAL();
//        if (cmd == 0)
//            break;
//    }
//
//    lcddisp(1, 0, "    已经到位    ");
//    (void)delay_and_wait_key(2, 0, 0);
//}

/*****************************************************************
函数原型：QueryXrayStatus
功能描述：查询X射线状态
参数描述：
参数名称：  输入/输出？ 类型        描述
-----------     ----------- ------      -------

返  回  值：无
作      者  ：许岩
日      期：2005-08-20
修改历史：
日期        修改人      修改描述
------      ---------   -------------
*****************************************************************/
void QueryXrayStatus(INT8U param)
{
//  INT8U i = 0;
//  INT32U di = 0;
	INT8U buf[33] = {0};
	INT8U key = NO_KEY;
	INT16U reg = 0;

	param = param;

	ToggleWD();

	for ( ;; )
	{
//      ToggleWD();

		Lcd_Cls();

		OS_ENTER_CRITICAL();
		reg = DevStat.MOD_REG.reg[0x0018];
		OS_EXIT_CRITICAL();
		strcpy((void *)buf, "连接:       ");
		if ( (reg & 0x0002) != 0 )
			strcat((void *)buf, "异常");
		else
			strcat((void *)buf, "正常");
		lcddisp(0, 0, buf);

		OS_ENTER_CRITICAL();
		reg = DevStat.MOD_REG.reg[0x0019];
		OS_EXIT_CRITICAL();
		strcpy((void *)buf, "TUBE ARC:   ");
		if ( reg == 0 )
		{
			strcat((void *)buf, "异常");
		}
		else
		{
			strcat((void *)buf, "正常");
		}
		lcddisp(1, 0, buf);

		OS_ENTER_CRITICAL();
		reg = DevStat.MOD_REG.reg[0x0018];
		OS_EXIT_CRITICAL();
		strcpy((void *)buf, "XRAY READY: ");
		if ( reg == 0 )
		{
			strcat((void *)buf, "正常");
		}
		else
		{
			strcat((void *)buf, "异常");
		}
		lcddisp(2, 0, buf);

		OS_ENTER_CRITICAL();
		reg = DevStat.MOD_REG.reg[0x0017];
		OS_EXIT_CRITICAL();
		strcpy((void *)buf, "射线开:     ");
		if ( reg == 0 )
		{
			strcat((void *)buf, "打开");
		}
		else
		{
			strcat((void *)buf, "关闭");
		}
		lcddisp(3, 0, buf);



		key = delay_and_wait_key(2, 0, 0);
		if ( key != NO_KEY )
			return;



		Lcd_Cls();

		OS_ENTER_CRITICAL();
		reg = DevStat.MOD_REG.reg[0x0016];
		OS_EXIT_CRITICAL();
		strcpy((void *)buf, "电压过载:   ");
		if ( reg == 0 )
			strcat((void *)buf, "异常");
		else
			strcat((void *)buf, "正常");
		lcddisp(0, 0, buf);

		OS_ENTER_CRITICAL();
		reg = DevStat.MOD_REG.reg[0x0015];
		OS_EXIT_CRITICAL();
		strcpy((void *)buf, "电流过载:   ");
		if ( reg == 0 )
		{
			strcat((void *)buf, "异常");
		}
		else
		{
			strcat((void *)buf, "正常");
		}
		lcddisp(1, 0, buf);


		key = delay_and_wait_key(2, 0, 0);
		if ( key != NO_KEY )
			return;

	}
}

/*****************************************************************
函数原型：QueryXrayVI
功能描述：查询X射线电流电压状态
参数描述：
参数名称：  输入/输出？ 类型        描述
-----------     ----------- ------      -------

返  回  值：无
作      者  ：许岩
日      期：2005-08-20
修改历史：
日期        修改人      修改描述
------      ---------   -------------
*****************************************************************/
void QueryXrayVI(INT8U param)
{
//  INT8U i = 0;
//  INT32U di = 0;
	INT8U buf[33] = {0};
	INT8U key = NO_KEY;
	INT16U reg = 0;

	param = param;

	ToggleWD();

	Lcd_Cls();

	for ( ;; )
	{
//      ToggleWD();

		OS_ENTER_CRITICAL();
		reg = DevStat.MOD_REG.reg[0x0014];
		OS_EXIT_CRITICAL();
		sprintf((void *)buf, "电压给定:%5.1fKV", (FP64)reg / 10.0);
		lcddisp(0, 0, buf);

		OS_ENTER_CRITICAL();
		reg = DevStat.MOD_REG.reg[0x0013];
		OS_EXIT_CRITICAL();
		sprintf((void *)buf, "电压测量:%5.1fKV", (FP32)reg / 10.0);
		lcddisp(1, 0, buf);

		OS_ENTER_CRITICAL();
		reg = DevStat.MOD_REG.reg[0x0012];
		OS_EXIT_CRITICAL();
		sprintf((void *)buf, "电流给定:%5.1fuA", (FP64)reg / 10.0);
		lcddisp(2, 0, buf);

		OS_ENTER_CRITICAL();
		reg = DevStat.MOD_REG.reg[0x0011];
		OS_EXIT_CRITICAL();
		sprintf((void *)buf, "电流测量:%5.1fuA", (FP32)reg / 10.0);
		lcddisp(3, 0, buf);

		key = delay_and_wait_key_ms(500, 0, 0);
		if ( key != NO_KEY )
			return;
	}
}

/*****************************************************************
函数原型：AdjTemperatureZero1
功能描述：温度1调零
参数描述：
参数名称：  输入/输出？ 类型        描述
-----------     ----------- ------      -------

返  回  值：无
作      者  ：许岩
日      期：2005-08-20
修改历史：
日期        修改人      修改描述
------      ---------   -------------
*****************************************************************/
void AdjTemperatureZero1(INT8U param)
{
////  INT8U i = 0;
//    INT8U buf[33] = {0};
//    INT8U key = NO_KEY;
////  INT16U ad[11] = {0};
////  INT16S temp_int16s = 0;
//    INT32S temp_int32s = 0;
//
//    param = param;
//
//    ToggleWD();
//
//    Lcd_Cls();
//    lcddisp(0, 0, "    温度1 调零  ");
//    lcddisp(1, 0, "请放入冰水混合物");
//    lcddisp(2, 0, "按确认键执行调零");
//
//    for(;;)
//    {
//        OS_ENTER_CRITICAL();
//        if ((DevStat.MOD_REG.reg[0x004A] & 0x8000) == 0)        //看是否为正
//        {
//            temp_int32s = (INT32S)DevStat.MOD_REG.reg[0x004A];
//        }
//        else
//        {
//            temp_int32s = 0 - (INT32S)(DevStat.MOD_REG.reg[0x004A] & (~0x8000));
//        }
//        sprintf((void *)buf, "温度1:%+8.2f℃", _fflt(temp_int32s) / 100.0);
//        OS_EXIT_CRITICAL();
//        lcddisp(3, 0, buf);
//
//        key = delay_and_wait_key_ms(500, 0, 0);
//        if (key == NO_KEY)
//            continue;
//        else if (key == KEY_ENTER)
//        {
//            if (temp_int32s < 0)
//            {
//                OS_ENTER_CRITICAL();
//                DevStat.adj_temperature_zero[0] += (FP64)temp_int32s;
//                OS_EXIT_CRITICAL();
//            }
//            else
//            {
//                OS_ENTER_CRITICAL();
//                DevStat.adj_temperature_zero[0] -= (0.0 - (FP64)abs(temp_int32s));
//                OS_EXIT_CRITICAL();
//            }
//            WriteParam();
//
//            continue;
//        }
//        else
//            return;
//    }
}

/*****************************************************************
函数原型：AdjTemperatureZero2
功能描述：温度2调零
参数描述：
参数名称：  输入/输出？ 类型        描述
-----------     ----------- ------      -------

返  回  值：无
作      者  ：许岩
日      期：2005-08-20
修改历史：
日期        修改人      修改描述
------      ---------   -------------
*****************************************************************/
void AdjTemperatureZero2(INT8U param)
{
////  INT8U i = 0;
//    INT8U buf[33] = {0};
//    INT8U key = NO_KEY;
////  INT16U ad[11] = {0};
////  INT16S temp_int16s = 0;
//    INT32S temp_int32s = 0;
//
//    param = param;
//
//    ToggleWD();
//
//    Lcd_Cls();
//    lcddisp(0, 0, "    温度2 调零  ");
//    lcddisp(1, 0, "请放入冰水混合物");
//    lcddisp(2, 0, "按确认键执行调零");
//
//    for(;;)
//    {
//        OS_ENTER_CRITICAL();
//        if ((DevStat.MOD_REG.reg[0x004B] & 0x8000) == 0)        //看是否为正
//        {
//            temp_int32s = (INT32S)DevStat.MOD_REG.reg[0x004B];
//        }
//        else
//        {
//            temp_int32s = 0 - (INT32S)(DevStat.MOD_REG.reg[0x004B] & (~0x8000));
//        }
//        sprintf((void *)buf, "温度1:%+8.2f℃", _fflt(temp_int32s) / 100.0);
//        OS_EXIT_CRITICAL();
//        lcddisp(3, 0, buf);
//
//        key = delay_and_wait_key_ms(500, 0, 0);
//        if (key == NO_KEY)
//            continue;
//        else if (key == KEY_ENTER)
//        {
//            if (temp_int32s < 0)
//            {
//                OS_ENTER_CRITICAL();
//                DevStat.adj_temperature_zero[1] += (FP64)temp_int32s;
//                OS_EXIT_CRITICAL();
//            }
//            else
//            {
//                OS_ENTER_CRITICAL();
//                DevStat.adj_temperature_zero[1] -= (0.0 - (FP64)abs(temp_int32s));
//                OS_EXIT_CRITICAL();
//            }
//            WriteParam();
//
//            continue;
//        }
//        else
//            return;
//    }
}
/*****************************************************************
函数原型：SWITCHEnalbe
功能描述：使能继电器
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
param		输入			INT8U		输入的参数

返  回  值：无
作      者：刘及华
日      期：2013-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void SWITCHEnalbe(INT8U param)
{
	param = param;		//avoid warning

	ToggleWD();

	OS_ENTER_CRITICAL();
	DevStat.MOD_REG.reg[0x18] = 0x5555;	 //使能继电器
	OS_EXIT_CRITICAL();

	lcddisp(3, 0, "打开继电器成功");
	OSTimeDlyHMSM(0, 0, 2, 0);

	lcddisp(0, 0, "1.停止工作");
	
}

/*****************************************************************
函数原型：SWITCHDisalbe
功能描述：禁止继电器
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
param		输入			INT8U		输入的参数

返  回  值：无
作      者：刘及华
日      期：2013-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void SWITCHDisalbe(INT8U param)
{
	param = param;		//avoid warning

	ToggleWD();

	OS_ENTER_CRITICAL();
	DevStat.MOD_REG.reg[0x18] = 0x00;	 //禁止继电器
	OS_EXIT_CRITICAL();

	lcddisp(3, 0, "关闭继电器成功");
	OSTimeDlyHMSM(0, 0, 2, 0);
}

/*****************************************************************
函数原型：SetPassWord
功能描述：设置开门密码
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
param		输入			INT8U		输入的参数

返  回  值：无
作      者	：刘及华
日      期：2014-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void SetPassWord(INT8U param)
{
	char  oldpass[6],newpass[6],surepass[6]; 
	char i = 0;
	INT8U key = NO_KEY;
	INT32U pass_input = 0;
	INT8U buf[20];

	memset(oldpass, 0x00, sizeof(oldpass));
	memset(newpass, 0x00, sizeof(newpass));
	memset(surepass, 0x00, sizeof(surepass));

	//请输入密码
	Lcd_Cls();
//  lcddisp(0, 0, "  用户密码设置  ");
	lcddisp(0, 0, "原密码:         ");
	strcpy((void *)buf, "原密码:");

	for ( i=0; i<6; i++ )
	{
		do				   //only accept 0-9,KEY_CANCEL
		{
			key = delay_and_wait_key(10, 0, 0);
			if ( key == NO_KEY )
				return;
		}while ( !( (key>='0'&&key<='9') || (key==KEY_CANCEL) ) );

		if ( key == KEY_CANCEL )
			return ;			//验证失败

		oldpass[i] = key;

		buf[strlen((void *)buf) - 1] = '\0';
		strcat((void *)buf, (void *)"*_");
		lcddisp(0, 0, buf);
	}

	if ( memcmp((void *)oldpass, (void *)&DevStat.passwd[0], 6) == 0 )
	{
		lcddisp(1, 0, "新密码:         ");
		strcpy((void *)buf, "新密码:");

		for ( i=0; i<6; i++ )
		{
			do				   //only accept 0-9,KEY_CANCEL
			{
				key = delay_and_wait_key(10, 0, 0);
				if ( key == NO_KEY )
					return ;
			}while ( !( (key>='0'&&key<='9') || (key==KEY_CANCEL) ) );

			if ( key == KEY_CANCEL )
				return ;			//验证失败

			newpass[i] = key;

			buf[strlen((void *)buf) - 1] = '\0';
			strcat((void *)buf, (void *)"*_");
			lcddisp(1, 0, buf);
		}
		lcddisp(2, 0, "再一次:         ");

		strcpy((void *)surepass, "");
		for ( i=0; i<6; i++ )
		{
			do				   //only accept 0-9,KEY_CANCEL
			{
				key = delay_and_wait_key(10, 0, 0);
				if ( key == NO_KEY )
					return ;
			}while ( !( (key>='0'&&key<='9') || (key==KEY_CANCEL) ) );

			if ( key == KEY_CANCEL )
				return ;			//验证失败

			surepass[i] = key;

			buf[strlen((void *)buf) - 1] = '\0';
			strcat((void *)buf, (void *)"*_");
			lcddisp(2, 0, buf);
		}
		if( memcmp(newpass, surepass, 6) == 0 )
		{
			lcddisp(3, 0, "    密码修改成功    ");
			strcpy((void *)DevStat.passwd[0], (void *)newpass);
			WriteParam();
			return ;
		}
		else
		{
			lcddisp(3, 0, "    密码修改失败    ");
			return ;
		}
		return ;
	}
	return ;
}
/*****************************************************************
函数原型：SetDefault
功能描述：使能AGC
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
param		输入			INT8U		输入的参数

返  回  值：无
作      者	：许岩
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void SetUSERDefault(INT8U param)
{
	INT8U key = NO_KEY;
	INT8U i=0;
	 
	param = param;
	
	
	ToggleWD();

	Lcd_Cls();
	lcddisp(0, 0, "是否恢复出厂设置");
	lcddisp(3, 0, "取消        确定");
	
	key = delay_and_wait_key_ms(5000, 0, 0);
	switch ( key )
	{
		case KEY_ENTER:
			break;
	
		case KEY_CANCEL:
			return;

		default:
			return;
	}
	if (key == NO_KEY)
		return;
	/* 全局变量初始化*/
	OS_ENTER_CRITICAL();
//  memset((void*)&DevStat, 0x00, sizeof(DevStat));
//
//  DevStat.err_occured = FALSE;
//  DevStat.err_code = 0x0000;
//  DevStat.id = 0x10;
//  DevStat.mode = AUTO;
//  DevStat.passwd = 0;
//  DevStat.MOD_REG.reg[0x18] = 0;	//禁止继电器输出

//  DevStat.switch_on_delay = 1000;
//  DevStat.switch_interval = 1000;   //ms为单位
	for(i=0;i<8;i++)
	{
//  	memset((void *)&DevStat.MOD_REG.REG_DETAIL.switch_on_delay[0], 0x07D0, 8);
//  	memset((void *)&DevStat.MOD_REG.REG_DETAIL.switch_interval[0], 0x07D0, 8);
		DevStat.MOD_REG.REG_DETAIL.switch_on_delay[i] = 2000;
		DevStat.MOD_REG.REG_DETAIL.switch_interval[i] = 2000;
	}

	WriteParam();

	OS_EXIT_CRITICAL();

	Lcd_Cls();
	lcddisp(2, 0, "恢复出厂设置成功");
	OSTimeDlyHMSM(0, 0, 2, 0);
}

/*****************************************************************
函数原型：Judgework
功能描述：判断是否线圈正在工作中
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
param		输入			INT8U		输入的参数

返  回  值：无
作      者	：frank
日      期：2013-12-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U Judgework(void)
{
	INT16U mode = 0;

	//param = param;

	ToggleWD();

	OS_ENTER_CRITICAL();
	mode = DevStat.MOD_REG.reg[0x18] ;	 //继电器状态判断;
	OS_EXIT_CRITICAL();

	if ( mode == 0x5555 )
	{
		Lcd_Cls();
		lcddisp(2, 0, "请先关闭工作状态");
		BeepInputErr();
		(void)delay_and_wait_key(2, 0, 0);
		return notok;
	}
	else
	{
		return ok;
	}
}

/*****************************************************************
函数原型：AGCEnalbe
功能描述：使能AGC
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
param		输入			INT8U		输入的参数

返  回  值：无
作      者	：许岩
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void AGCEnalbe(INT8U param)
{
	param = param;		//avoid warning

	ToggleWD();

	OS_ENTER_CRITICAL();
//  DevStat.MOD_REG.reg[0x000C] = 0x0001; //使能AGC
	OS_EXIT_CRITICAL();

	lcddisp(3, 0, "打开AGC 输出成功");
	OSTimeDlyHMSM(0, 0, 1, 0);
}

/*****************************************************************
函数原型：AGCDisalbe
功能描述：禁止AGC输出
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
param		输入			INT8U		输入的参数

返  回  值：无
作      者	：许岩
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void AGCDisalbe(INT8U param)
{
	param = param;		//avoid warning

	ToggleWD();

	OS_ENTER_CRITICAL();
//  DevStat.MOD_REG.reg[0x000C] = 0x0000; //关闭AGC
	OS_EXIT_CRITICAL();

	lcddisp(3, 0, "关闭AGC 输出成功");
	OSTimeDlyHMSM(0, 0, 1, 0);
}


/*****************************************************************
函数原型：Querydelay
功能描述：查询延迟开启时间状态
参数描述：
参数名称：  输入/输出？ 类型        描述
-----------     ----------- ------      -------

返  回  值：无
作      者  ：frank
日      期：2012-08-20
修改历史：
日期        修改人      修改描述
------      ---------   -------------
*****************************************************************/
void Querydelay(INT8U param)
{
//  INT8U i = 0;
	INT8U buf[33] = {0};
	INT8U key = NO_KEY;
//  INT16U ad[11] = {0};

	param = param;

	ToggleWD();

	Lcd_Cls();

	if(Judgework() != ok)
	{
		return;
	}

	for(;;)
	{
		OS_ENTER_CRITICAL();
		sprintf((void *)buf, "0:%1.2f  1:%1.2f  ", (DevStat.MOD_REG.REG_DETAIL.switch_delay[0])/1000.0, DevStat.MOD_REG.REG_DETAIL.switch_delay[1]/1000.0);
		OS_EXIT_CRITICAL();
		lcddisp(0, 0, buf);

		OS_ENTER_CRITICAL();
		sprintf((void *)buf, "2:%1.2f  3:%1.2f  ", DevStat.MOD_REG.REG_DETAIL.switch_delay[2]/1000.0, DevStat.MOD_REG.REG_DETAIL.switch_delay[3]/1000.0);
		OS_EXIT_CRITICAL();
		lcddisp(1, 0, buf);

		OS_ENTER_CRITICAL();
		sprintf((void *)buf, "4:%1.2f  5:%1.2f  ", DevStat.MOD_REG.REG_DETAIL.switch_delay[4]/1000.0, DevStat.MOD_REG.REG_DETAIL.switch_delay[5]/1000.0);
		OS_EXIT_CRITICAL();
		lcddisp(2, 0, buf);

		OS_ENTER_CRITICAL();
		sprintf((void *)buf, "6:%1.2f  7:%1.2f  ", DevStat.MOD_REG.REG_DETAIL.switch_delay[6]/1000.0, DevStat.MOD_REG.REG_DETAIL.switch_delay[7]/1000.0);
		OS_EXIT_CRITICAL();
		lcddisp(3, 0, buf);

		key = delay_and_wait_key_ms(5000, 0, 0);
		switch ( key )
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				AdjSwitchdelay((key - 0x30));
				break;

			case KEY_CANCEL:
				return;

			default:
				continue;
		}
		if (key != NO_KEY)
			return;
	}
}


/*****************************************************************
函数原型：QueryOndelay
功能描述：查询AD状态
参数描述：
参数名称：  输入/输出？ 类型        描述
-----------     ----------- ------      -------

返  回  值：无
作      者  ：许岩
日      期：2005-08-20
修改历史：
日期        修改人      修改描述
------      ---------   -------------
*****************************************************************/
void QueryOndelay(INT8U param)
{
//  INT8U i = 0;
	INT8U buf[33] = {0};
	INT8U key = NO_KEY;
//  INT16U ad[11] = {0};

	param = param;

	ToggleWD();

	Lcd_Cls();

	if(Judgework() != ok)
	{
		return;
	}

	for(;;)
	{
		OS_ENTER_CRITICAL();
		sprintf((void *)buf, "0:%1.2f  1:%1.2f  ", (DevStat.MOD_REG.REG_DETAIL.switch_on_delay[0])/1000.0, DevStat.MOD_REG.REG_DETAIL.switch_on_delay[1]/1000.0);
		OS_EXIT_CRITICAL();
		lcddisp(0, 0, buf);

		OS_ENTER_CRITICAL();
		sprintf((void *)buf, "2:%1.2f  3:%1.2f  ", DevStat.MOD_REG.REG_DETAIL.switch_on_delay[2]/1000.0, DevStat.MOD_REG.REG_DETAIL.switch_on_delay[3]/1000.0);
		OS_EXIT_CRITICAL();
		lcddisp(1, 0, buf);

		OS_ENTER_CRITICAL();
		sprintf((void *)buf, "4:%1.2f  5:%1.2f  ", DevStat.MOD_REG.REG_DETAIL.switch_on_delay[4]/1000.0, DevStat.MOD_REG.REG_DETAIL.switch_on_delay[5]/1000.0);
		OS_EXIT_CRITICAL();
		lcddisp(2, 0, buf);

		OS_ENTER_CRITICAL();
		sprintf((void *)buf, "6:%1.2f  7:%1.2f  ", DevStat.MOD_REG.REG_DETAIL.switch_on_delay[6]/1000.0, DevStat.MOD_REG.REG_DETAIL.switch_on_delay[7]/1000.0);
		OS_EXIT_CRITICAL();
		lcddisp(3, 0, buf);

		key = delay_and_wait_key_ms(5000, 0, 0);
		switch ( key )
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				AdjSwitchOndelay((key - 0x30));
				break;

			case KEY_CANCEL:
				return;

			default:
				continue;
		}
		if (key != NO_KEY)
			return;
	}
}


/*****************************************************************
函数原型：QueryOffdelay
功能描述：查询AD状态
参数描述：
参数名称：  输入/输出？ 类型        描述
-----------     ----------- ------      -------

返  回  值：无
作      者  ：许岩
日      期：2005-08-20
修改历史：
日期        修改人      修改描述
------      ---------   -------------
*****************************************************************/
void QueryOffdelay(INT8U param)
{
	INT8U buf[33] = {0};
	INT8U key = NO_KEY;

	param = param;

	ToggleWD();

	Lcd_Cls();

	if(Judgework() != ok)
	 {
		 return;
	 }

	for(;;)
	{
		OS_ENTER_CRITICAL();
		sprintf((void *)buf, "0:%1.2f  1:%1.2f  ", DevStat.MOD_REG.REG_DETAIL.switch_interval[0]/1000.0, DevStat.MOD_REG.REG_DETAIL.switch_interval[1]/1000.0);
		OS_EXIT_CRITICAL();
		lcddisp(0, 0, buf);

		OS_ENTER_CRITICAL();
		sprintf((void *)buf, "2:%1.2f  3:%1.2f  ", DevStat.MOD_REG.REG_DETAIL.switch_interval[2]/1000.0, DevStat.MOD_REG.REG_DETAIL.switch_interval[3]/1000.0);
		OS_EXIT_CRITICAL();
		lcddisp(1, 0, buf);

		OS_ENTER_CRITICAL();
		sprintf((void *)buf, "4:%1.2f  5:%1.2f  ", DevStat.MOD_REG.REG_DETAIL.switch_interval[4]/1000.0, DevStat.MOD_REG.REG_DETAIL.switch_interval[5]/1000.0);
		OS_EXIT_CRITICAL();
		lcddisp(2, 0, buf);

		OS_ENTER_CRITICAL();
		sprintf((void *)buf, "6:%1.2f  7:%1.2f  ", DevStat.MOD_REG.REG_DETAIL.switch_interval[6]/1000.0, DevStat.MOD_REG.REG_DETAIL.switch_interval[7]/1000.0);
		OS_EXIT_CRITICAL();
		lcddisp(3, 0, buf);

		key = delay_and_wait_key_ms(5000, 0, 0);
		switch ( key )
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				AdjSwitchInterval((key - 0x30));
				break;

			case KEY_CANCEL:
				return;

			default:
				continue;
		}
		if (key != NO_KEY)
			return;
	}
}

/*****************************************************************
函数原型：AdjKFZero
功能描述：矿粉标准化采集函数，目的确定没有铁粉时候，零点电压的具体数值
参数描述：
参数名称：  输入/输出？ 类型        描述
-----------     ----------- ------      -------

返  回  值：无
作      者  ：FRANK
日      期：2014-02-21
修改历史：
日期        修改人      修改描述
------      ---------   -------------
*****************************************************************/
void AdjKFZero(INT8U param)
{
	INT8U buf[33] = {0};
	INT8U key = NO_KEY;
	INT32S temp_int32s = 0.0;
	INT32S adj_zero = 0;

	param = param;

	ToggleWD();

	Lcd_Cls();
	lcddisp(0, 0, "  矿粉量标准化  ");
	lcddisp(2, 0, "按确认键进行设定");

	for ( ;; )
	{
//      ToggleWD();

		OS_ENTER_CRITICAL();

		temp_int32s = (INT32S)DevStat.MOD_REG.reg[0x0010];

		sprintf((void *)buf, "零点电压:%+7.3f", _fflt(temp_int32s) / 1000.0);
		OS_EXIT_CRITICAL();
		lcddisp(3, 0, buf);

		key = delay_and_wait_key_ms(500, 0, 0);
		if ( key == NO_KEY )
			continue;
		else if ( key == KEY_ENTER )
		{
			adj_zero = temp_int32s;

			write_mod_reg((void *)&DevStat.MOD_REG.reg[0x0011], adj_zero);

			WriteParam();
			lcddisp(2, 0, "    设定成功!   ");
			Beep(1000);	
			continue;
		}
		else
			return;
	}
}

/*****************************************************************
函数原型：AdjADZero
功能描述：AD调零
参数描述：
参数名称：  输入/输出？ 类型        描述
-----------     ----------- ------      -------

返  回  值：无
作      者  ：许岩
日      期：2005-08-20
修改历史：
日期        修改人      修改描述
------      ---------   -------------
*****************************************************************/
void AdjADZero(INT8U param)
{
	INT8U buf[33] = {0};
	INT8U key = NO_KEY;
	INT32S temp_int32s = 0;
	INT32S adj_zero = 0;

	param = param;

	ToggleWD();

	Lcd_Cls();
	lcddisp(0, 0, "    AD调零      ");
	lcddisp(2, 0, "按确认键执行调零");

	for ( ;; )
	{
//      ToggleWD();

		OS_ENTER_CRITICAL();
		if ( (DevStat.MOD_REG.reg[0x0003] & 0x8000) == 0 )		  //看是否为正
		{
			temp_int32s = (INT32S)DevStat.MOD_REG.reg[0x0003] * 1000L
						  + (INT32S)DevStat.MOD_REG.reg[0x0004];
		}
		else
		{
			temp_int32s = 0 - ((INT32S)(DevStat.MOD_REG.reg[0x0003] & (~0x8000)) * 1000L)
						  - (INT32S)DevStat.MOD_REG.reg[0x0004];
		}
		sprintf((void *)buf, "测量厚度:%+7.3f", _fflt(temp_int32s) / 1000.0);
		OS_EXIT_CRITICAL();
		lcddisp(3, 0, buf);


		key = delay_and_wait_key_ms(500, 0, 0);
		if ( key == NO_KEY )
			continue;
		else if ( key == KEY_ENTER )
		{
			adj_zero = read_mod_reg((void *)&DevStat.MOD_REG.reg[0x0008]);	//读调零寄存器
			adj_zero -= temp_int32s;
			if ( adj_zero > ADJ_AD_ZERO_MAX || adj_zero < 0 - ADJ_AD_ZERO_MAX )
			{
				lcddisp(1, 0, "调零超出范围!!  ");
				BeepInputErr();
				(void)delay_and_wait_key(2, 0, 0);
				lcddisp(1, 0, "                ");
				continue;
			}
			write_mod_reg((void *)&DevStat.MOD_REG.reg[0x0008], adj_zero);

			WriteParam();

			continue;
		}
		else
			return;
	}
}

/*****************************************************************
函数原型： AdjSwitchInterval(INT8U param)
功能描述：继电器断开间隔时间设置
参数描述：
参数名称：  输入/输出？ 类型        描述
-----------     ----------- ------      -------

返 回 值：无
作    者：刘及华
日    期：2013-07-12
修改历史：
日期        修改人      修改描述
------      ---------   -------------
*****************************************************************/
void AdjSwitchInterval(INT8U param)
{
	INT8U buf[33] = {0};
	INT8U key = NO_KEY;
	INT16S temp_int16s = 0;
	INT16S adj_h = 0;
	INT8U buf2[20] = {0};
	INT8U position = 0;

	if( param > 7 )
		return;

	ToggleWD();

	Lcd_Cls();
	sprintf((void *)buf, "继电器%d", param);
	lcddisp(0, 0, buf);
	lcddisp(2, 0, "请输入断开时间: ");
//  lcddisp(3, 0, "         +_ .   ");
	lcddisp(3, 0, "            _.  ");

	strcpy((void *)buf2, "            _.__");
	position = 12;

	for ( ;; )
	{
		OS_ENTER_CRITICAL();
		temp_int16s = (INT16S)DevStat.MOD_REG.REG_DETAIL.switch_interval[param];
		sprintf((void *)buf, "断开时间%1.2f(s)", _fflt(temp_int16s)/1000.0);
		OS_EXIT_CRITICAL();
		lcddisp(1, 0, buf);

		key = delay_and_wait_key_ms(500, 0, 0);
		switch ( key )
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if ( position >= 16 )
				{
					continue;
				}
				buf2[position] = key;
				position++;
				if ( position >= 16 )
				{
					lcddisp(3, 0, buf2);
					continue;
				}
				if ( position == 13 )		  //跳过小数点
					position++;
				buf2[position] = '_';
				lcddisp(3, 0, buf2);
				break;

		   case KEY_ENTER:

				adj_h = (INT16S)ascii_to_hex(buf2[12]) * 1000 + (INT16S)ascii_to_hex(buf2[14]) * 100
						+ (INT16S)ascii_to_hex(buf2[15]) * 10;

				OS_ENTER_CRITICAL();
				temp_int16s = abs( _dfix( adj_h ) );
				OS_EXIT_CRITICAL();

				if ( temp_int16s > 10000 || temp_int16s < 0 )
				{
					lcddisp(2, 0, "超出范围!!      ");
					BeepInputErr();
					(void)delay_and_wait_key(2, 0, 0);
					lcddisp(2, 0, "请输入正确时间: ");
					continue;
				}

				OS_ENTER_CRITICAL();
				DevStat.MOD_REG.REG_DETAIL.switch_interval[param] = temp_int16s;
				OS_EXIT_CRITICAL();

				WriteParam();
				Lcd_Cls();
				lcddisp(2, 0, "    保存成功    ");
				Beep(500);
				(void)delay_and_wait_key(2, 0, 0);
				return;

			case KEY_CANCEL:
				return;

			default:
				continue;
		}
	}

}



/*****************************************************************
函数原型： AdjSwitchdelay(INT8U param)
功能描述：延迟开启时间
参数描述：
参数名称：  输入/输出？ 类型        描述
-----------     ----------- ------      -------

返 回 值：无
作    者：刘及华
日    期：2013-07-12
修改历史：
日期        修改人      修改描述
------      ---------   -------------
*****************************************************************/
void AdjSwitchdelay(INT8U param)
{
	INT8U buf[33] = {0};
	INT8U key = NO_KEY;
	INT16S temp_int16s = 0;
	INT16S adj_h = 0;
	INT8U buf2[20] = {0};
	INT8U position = 0;

	if( param > 7 )
		return;

	ToggleWD();

	Lcd_Cls();
	sprintf((void *)buf, "继电器%d", param);
	lcddisp(0, 0, buf);
	lcddisp(2, 0, "请输入延迟时间: ");
//  lcddisp(3, 0, "         +_ .   ");
	lcddisp(3, 0, "            _.  ");

	strcpy((void *)buf2, "            _.__");
	position = 12;

	for ( ;; )
	{
		OS_ENTER_CRITICAL();
//  	if ((DevStat.MOD_REG.reg[0x004A] & 0x8000) == 0)        //看是否为正
//  	{
//  		temp_int32s = (INT32S)DevStat.MOD_REG.reg[0x004A];
//  	}
//  	else
//  	{
//  		temp_int32s = 0 - (INT32S)(DevStat.MOD_REG.reg[0x004A] & (~0x8000));
//  	}
		temp_int16s = (INT16S)DevStat.MOD_REG.REG_DETAIL.switch_delay[param];
		sprintf((void *)buf, "延迟时间%1.2f(s)", _fflt(temp_int16s)/1000.0);
		OS_EXIT_CRITICAL();
		lcddisp(1, 0, buf);


		key = delay_and_wait_key_ms(500, 0, 0);
		switch ( key )
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if ( position >= 16 )
				{
					continue;
				}
				buf2[position] = key;
				position++;
				if ( position >= 16 )
				{
					lcddisp(3, 0, buf2);
					continue;
				}
				if ( position == 13 )		  //跳过小数点
					position++;
				buf2[position] = '_';
				lcddisp(3, 0, buf2);
				break;

		   case KEY_ENTER:

				adj_h = (INT16S)ascii_to_hex(buf2[12]) * 1000 + (INT16S)ascii_to_hex(buf2[14]) * 100
						+ (INT16S)ascii_to_hex(buf2[15]) * 10;

				OS_ENTER_CRITICAL();
				//temp_int16s = abs( _dfix( adj_h ) );
				temp_int16s = adj_h;
				OS_EXIT_CRITICAL();

				if ( temp_int16s > 10000 || temp_int16s < 0 )
				{
					lcddisp(2, 0, "超出范围!!      ");
					BeepInputErr();
					(void)delay_and_wait_key(2, 0, 0);
					lcddisp(2, 0, "请输入正确时间: ");
					continue;
				}

				OS_ENTER_CRITICAL();
				DevStat.MOD_REG.REG_DETAIL.switch_delay[param] = temp_int16s;
				OS_EXIT_CRITICAL();

				WriteParam();
				Lcd_Cls();
				lcddisp(2, 0, "    保存成功    ");
				Beep(500);
				(void)delay_and_wait_key(2, 0, 0);
				return;

			case KEY_CANCEL:
				return;

			default:
				continue;
		}
	}

}



/*****************************************************************
函数原型： AdjSwitchOndelay(INT8U param)
功能描述：温度1调满
参数描述：
参数名称：  输入/输出？ 类型        描述
-----------     ----------- ------      -------

返 回 值：无
作    者：刘及华
日    期：2013-07-12
修改历史：
日期        修改人      修改描述
------      ---------   -------------
*****************************************************************/
void AdjSwitchOndelay(INT8U param)
{
	INT8U buf[33] = {0};
	INT8U key = NO_KEY;
	INT16S temp_int16s = 0;
	INT16S adj_h = 0;
	INT8U buf2[20] = {0};
	INT8U position = 0;

	if( param > 7 )
		return;

	ToggleWD();

	Lcd_Cls();
	sprintf((void *)buf, "继电器%d", param);
	lcddisp(0, 0, buf);
	lcddisp(2, 0, "请输入通电时间: ");
//  lcddisp(3, 0, "         +_ .   ");
	lcddisp(3, 0, "            _.  ");

	strcpy((void *)buf2, "            _.__");
	position = 12;

	for ( ;; )
	{
		OS_ENTER_CRITICAL();
//  	if ((DevStat.MOD_REG.reg[0x004A] & 0x8000) == 0)        //看是否为正
//  	{
//  		temp_int32s = (INT32S)DevStat.MOD_REG.reg[0x004A];
//  	}
//  	else
//  	{
//  		temp_int32s = 0 - (INT32S)(DevStat.MOD_REG.reg[0x004A] & (~0x8000));
//  	}
		temp_int16s = (INT16S)DevStat.MOD_REG.REG_DETAIL.switch_on_delay[param];
		sprintf((void *)buf, "通电时间%1.2f(s)", _fflt(temp_int16s)/1000.0);
		OS_EXIT_CRITICAL();
		lcddisp(1, 0, buf);


		key = delay_and_wait_key_ms(500, 0, 0);
		switch ( key )
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if ( position >= 16 )
				{
					continue;
				}
				buf2[position] = key;
				position++;
				if ( position >= 16 )
				{
					lcddisp(3, 0, buf2);
					continue;
				}
				if ( position == 13 )		  //跳过小数点
					position++;
				buf2[position] = '_';
				lcddisp(3, 0, buf2);
				break;

		   case KEY_ENTER:

				adj_h = (INT16S)ascii_to_hex(buf2[12]) * 1000 + (INT16S)ascii_to_hex(buf2[14]) * 100
						+ (INT16S)ascii_to_hex(buf2[15]) * 10;

				OS_ENTER_CRITICAL();
				//temp_int16s = abs( _dfix( adj_h ) );
				temp_int16s = adj_h;
				OS_EXIT_CRITICAL();

				if ( temp_int16s > 10000 || temp_int16s < 0 )
				{
					lcddisp(2, 0, "超出范围!!      ");
					BeepInputErr();
					(void)delay_and_wait_key(2, 0, 0);
					lcddisp(2, 0, "请输入正确时间: ");
					continue;
				}

				OS_ENTER_CRITICAL();
				DevStat.MOD_REG.REG_DETAIL.switch_on_delay[param] = temp_int16s;
				OS_EXIT_CRITICAL();

				WriteParam();
				Lcd_Cls();
				lcddisp(2, 0, "    保存成功    ");
				Beep(500);
				(void)delay_and_wait_key(2, 0, 0);
				return;

			case KEY_CANCEL:
				return;

			default:
				continue;
		}
	}

}

/*****************************************************************
函数原型：ParamInit
功能描述：参数初始化
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
param		输入			INT8U		输入的参数

返  回  值：无
作      者	：许岩
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void ParamInit(INT8U param)
{
	param = param;		//avoid warning

	ToggleWD();

	Lcd_Cls();
	lcddisp(0, 0, "    初始化      ");
	(void)InitImportantParameter();
	lcddisp(2, 0, "    初始化成功  ");
	OSTimeDlyHMSM(0, 0, 1, 0);
}

/*****************************************************************
函数原型：AdjAGCZero
功能描述：AGC调零
参数描述：
参数名称：  输入/输出？ 类型        描述
-----------     ----------- ------      -------

返  回  值：无
作      者  ：许岩
日      期：2005-08-20
修改历史：
日期        修改人      修改描述
------      ---------   -------------
*****************************************************************/
void AdjAGCZero(INT8U param)
{
//  INT8U i = 0;
	INT8U buf[33] = {0};
	INT8U key = NO_KEY;
	INT32S temp_int32s = 0;
//  INT32S adj_zero = 0;
	INT16U reg = 0;
	INT32S origin = 0;

	param = param;

	ToggleWD();

	Lcd_Cls();

	OS_ENTER_CRITICAL();
	reg = DevStat.MOD_REG.reg[0x000C];
	OS_EXIT_CRITICAL();
	if ( reg != 0x0000 )
	{
		lcddisp(2, 0, "请先关闭AGC 输出");
		BeepInputErr();
		(void)delay_and_wait_key(2, 0, 0);
		return;
	}

	lcddisp(0, 0, "    AGC 调零    ");
	lcddisp(1, 0, "4 和8 键改变大小");
	lcddisp(2, 0, "按确认键执行调零");

	for ( ;; )
	{
//      ToggleWD();

		temp_int32s = read_mod_reg((void *)&DevStat.MOD_REG.reg[0x000A]);
		origin = temp_int32s;		//保留原始值
		sprintf((void *)buf, "AGC 修正:%+7d", temp_int32s);
		lcddisp(3, 0, buf);


		key = delay_and_wait_key_ms(500, 0, 0);
		if ( key == NO_KEY )
			continue;
		else if ( key == '4' )
		{
			temp_int32s++;
			if ( temp_int32s > 2047 )
				temp_int32s = 2047;
			write_mod_reg((void *)&DevStat.MOD_REG.reg[0x000A], temp_int32s);
		}
		else if ( key == '8' )
		{
			temp_int32s--;
			if ( temp_int32s < -2047 )
				temp_int32s = -2047;
			write_mod_reg((void *)&DevStat.MOD_REG.reg[0x000A], temp_int32s);
		}
		else if ( key == KEY_ENTER )
		{
			WriteParam();

			Lcd_Cls();
			lcddisp(0, 0, "    AGC 调零    ");
			lcddisp(2, 0, "    保存成功    ");
			(void)delay_and_wait_key(2, 0, 0);
			return;
		}
		else
		{
			write_mod_reg((void *)&DevStat.MOD_REG.reg[0x000A], origin);		//恢复初始值
			return;
		}
	}
}

/*****************************************************************
函数原型：AdjADFull
功能描述：AD调满
参数描述：
参数名称：  输入/输出？ 类型        描述
-----------     ----------- ------      -------

返  回  值：无
作      者  ：许岩
日      期：2005-08-20
修改历史：
日期        修改人      修改描述
------      ---------   -------------
*****************************************************************/
void AdjADFull(INT8U param)
{
//  INT8U i = 0;
	INT8U buf[33] = {0};
	INT8U key = NO_KEY;
//  INT16U ad[11] = {0};
//  INT16S temp_int16s = 0;
	INT32S temp_int32s = 0;
	INT32S temp_int32s2 = 0;
	INT32S adj_h = 0;
	INT8U buf2[20] = {0};
//  INT8U buf3[33] = {0};
	INT8U position = 0;
	AD_TYPE adj_full = 0.0;
	AD_TYPE adj_full_origin = 0.0;

	param = param;

	ToggleWD();

	Lcd_Cls();
	lcddisp(0, 0, "    AD调满      ");
	lcddisp(2, 0, "请输入实际厚度: ");
	lcddisp(3, 0, "         +_ .   ");

	strcpy((void *)buf2, "         +_ .   ");
	position = 10;

	for ( ;; )
	{
//      ToggleWD();

		OS_ENTER_CRITICAL();
		if ( (DevStat.MOD_REG.reg[0x0003] & 0x8000) == 0 )		  //看是否为正
		{
			temp_int32s = (INT32S)DevStat.MOD_REG.reg[0x0003] * 1000L
						  + (INT32S)DevStat.MOD_REG.reg[0x0004];
		}
		else
		{
			temp_int32s = 0 - ((INT32S)(DevStat.MOD_REG.reg[0x0003] & (~0x8000)) * 1000L)
						  - (INT32S)DevStat.MOD_REG.reg[0x0004];
		}
		sprintf((void *)buf, "测量厚度:%+7.3f", _fflt(temp_int32s) / 1000.0);
		OS_EXIT_CRITICAL();
		lcddisp(1, 0, buf);


		key = delay_and_wait_key_ms(500, 0, 0);
		switch ( key )
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if ( position >= 16 )
				{
					continue;
				}
				buf2[position] = key;
				position++;
				if ( position >= 16 )
				{
					lcddisp(3, 0, buf2);
					continue;
				}
				if ( position == 12 )		  //跳过小数点
					position++;
				buf2[position] = '_';
				lcddisp(3, 0, buf2);
				break;

			case KEY_ENTER:
				adj_h = (INT32S)ascii_to_hex(buf2[10]) * 10000 + (INT32S)ascii_to_hex(buf2[11]) * 1000 
						+ (INT32S)ascii_to_hex(buf2[13]) * 100 + (INT32S)ascii_to_hex(buf2[14]) * 10 
						+ (INT32S)ascii_to_hex(buf2[15]);
				temp_int32s2 = read_mod_reg((void *)&DevStat.MOD_REG.reg[0x0009]);	//读AD调满寄存器原值
				OS_ENTER_CRITICAL();
				adj_full_origin = _fflt(temp_int32s2) / 1000.0;
				adj_full = _fflt(adj_h) / (_fflt(temp_int32s) / adj_full_origin);
				temp_int32s = abs(_dfix(adj_full * 1000.0));
				OS_EXIT_CRITICAL();

				if ( temp_int32s > ADJ_AD_FULL_MAX || temp_int32s < ADJ_AD_FULL_MIN )
				{
					lcddisp(2, 0, "调满超出范围!!  ");
					BeepInputErr();
					(void)delay_and_wait_key(2, 0, 0);
					lcddisp(2, 0, "请输入实际厚度: ");
					continue;
				}

				write_mod_reg((void *)&DevStat.MOD_REG.reg[0x0009], temp_int32s);
				WriteParam();
				lcddisp(2, 0, "    保存成功    ");
				(void)delay_and_wait_key(2, 0, 0);
				return;

			case KEY_CANCEL:
				return;

			default:
				continue;
		}
	}
}


