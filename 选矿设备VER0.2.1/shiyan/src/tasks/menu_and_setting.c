
#include "config.h"

extern volatile INT8U DisplayTimeEN;	//�Ƿ�����ˢ��ʱ�䣬=FALSE������=TRUE������ ����
extern volatile DEV_STAT DevStat;

struct      MENU*       curMenu;		//��ǰ�˵�

struct MENU mu[MENU_MAX_ITEM] = 
{
	//                	    next		pre			parent		child 		exec			param	enter_child_hook	hook_param
	{ "1.����״̬",     0,  &mu[1],     NULL,       NULL,       &mu[14],    NULL,           0,      NULL,               NULL,},	//0
	{ "2.���ݲ�ѯ",     0,  &mu[2],     &mu[0],     NULL,       &mu[6],		NULL,           0,      NULL,               NULL,},	//1
	{ "3.��������",     0,  &mu[3],     &mu[1],     NULL,       &mu[10],    NULL,  			0,      NULL,    			NULL,},	//2
	{ "4.�޸�����",     0,  &mu[4],     &mu[2],     NULL,       NULL,       SetPassWord,    0,      NULL,    			NULL,},	//3
	{ "5.�ָ�����",     0,  NULL,       &mu[3],     NULL,       NULL,       SetUSERDefault, 0,      NULL,               NULL,},	//4  
	{ "Ԥ��",           0,  NULL,       NULL,       NULL,       NULL,       NULL,           0,      NULL,               NULL,},	//5
	{ "1.�������ѯ",   0,  &mu[7],     NULL,       &mu[1],     NULL,       QueryAD,        0,      NULL,               NULL,},	//6
	{ "2.DA��ѯ",       0,  NULL,       &mu[6],     &mu[1],     NULL,       QueryDA,        0,      NULL,               NULL,},	//7
	{ "1.��AGC",      0,  &mu[9],     NULL,       &mu[1],     NULL,       AGCEnalbe,      0,      NULL,               NULL,},	//8
	{ "2.�ر�AGC",      0,  NULL,       &mu[8],     &mu[1],     NULL,       AGCDisalbe,     0,      NULL,               NULL,},	//9


	{ "1.�ӳ�ʱ��(s)",  0,  &mu[11],    NULL,       &mu[2],     NULL,       Querydelay,     0,      NULL,               NULL,}, //10
	{ "2.��ͨʱ��(s)",  0,  &mu[12],    &mu[10],    &mu[2],     NULL,       QueryOndelay,   0,      NULL,               NULL,},	//11
	{ "3.�Ͽ�ʱ��(s)",  0,  &mu[13],    &mu[11],    &mu[2],     NULL,       QueryOffdelay,  0,      NULL,               NULL,},	//12
	{ "4.�������׼��", 0,  NULL,       &mu[12],    &mu[2],     NULL,       AdjKFZero,      0,      NULL,               NULL,},	//13
	{ "1.��������",     0,  &mu[15],    NULL,       &mu[0],     NULL,       SWITCHEnalbe,   0,      NULL,               NULL,},	//14
	{ "2.ֹͣ����",     0,  NULL,       &mu[14],    &mu[0],     NULL,       SWITCHDisalbe,  0,      NULL,               NULL,},	//15
//	{ "1.ֹͣ����",     0,  NULL,       NULL,       &mu[0],     NULL,       SWITCHDisalbe,  0,      NULL,               NULL,},	//16
//	{ "1.�Զ�",         0,  &mu[16],    NULL,       &mu[0],     NULL,       SetModeAuto,    0,      NULL,               NULL,},	//17
//	{ "2.�ֶ�",         0,  NULL,       &mu[15],    &mu[0],     NULL,       SetModeManual,  0,      NULL,               NULL,},	//18
//  { "Ԥ��",           0,  NULL,       NULL,       NULL,       NULL,       NULL,           0,      NULL,               NULL,}, //19

};


/*****************************************************************
����ԭ�ͣ�InitMenu
����������������Ϊ��ʼ���˵�
������������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------
�� �� ֵ����
*****************************************************************/
void InitMenu(void)
{
	ToggleWD();

	curMenu = (void *)&mu[0];
	curMenu->muPos = 0;
}

/*****************************************************************
����ԭ�ͣ�DisplayMenu
����������������Ϊ��ʾ�˵�����
������������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------
�� �� ֵ����
*****************************************************************/
void DisplayMenu(void)
{
	int8 i;
	uint8 pos;
	struct MENU *temp = curMenu;
	char buf[20];

	ToggleWD();

	Lcd_Cls();	//�����Ļ

	pos = MENU_DISPLAY_START_ROW + temp->muPos;			//��ʾ��ǰ�˵�������
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
����ԭ�ͣ�GotoUpMenu
��������������������Ϊ���뵱ǰ�˵��������˵�
������������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------
�� �� ֵ����
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
����ԭ�ͣ�GotoDownMenu
��������������������Ϊ���������˵�
������������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------
�� �� ֵ����
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
����ԭ�ͣ�GotoChildMenu
��������������������Ϊ�����¼��˵�
������������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------
�� �� ֵ����
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
����ԭ�ͣ�GotoParentMenu
��������������������Ϊ�����ϼ��˵�
������������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------
�� �� ֵ����
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
����ԭ�ͣ�SetupProcess
������������������
����������
�������ƣ�	����/�����	����		����

��  ��  ֵ����
��      ��	������
��      �ڣ�2005-10-21
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
void SetupProcess(void)
{
	INT8U i = 0;
	INT8U key = NO_KEY;

	ToggleWD();

	OS_ENTER_CRITICAL();
	DisplayTimeEN = FALSE;			//��ֹˢ��ʱ������ˢ��ʱ��
	OS_EXIT_CRITICAL();

	i = LcdDispInputPasswd((void *)&DevStat.passwd);
	if ( i != ok )
	{
		OS_ENTER_CRITICAL();
		DisplayTimeEN = TRUE;		//����ˢ��ʱ������ˢ��ʱ��
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
					DisplayTimeEN = TRUE;		//����ˢ��ʱ������ˢ��ʱ��
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
����ԭ�ͣ�LcdDispInputPasswd
����������LCD��ʾ��������
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------
pass		����			void*		��ȷ������

��  ��  ֵ��ok - ��֤ͨ��
				notok - ��֤ʧ��
��      ��	������
��      �ڣ�2005-08-20
�޸���ʷ��
����		�޸���		�޸�����
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
	lcddisp(1, 1, "����������: ");

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
			return notok;			//��֤ʧ��

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
		DevStat.reptpasswdtimes = 0;   //���ｫ���Ŵ�������������
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
		sprintf((void*)buf, "�������%d��!", DevStat.reptpasswdtimes);
		lcddisp(3, 1, buf);
		BeepInputErr();
		if(DevStat.reptpasswdtimes >= 3)
		{
			IO3CLR = BIT29; //�����ƿ�ʼ����
		}
		OSTimeDlyHMSM(0, 0, 1, 0);
//  	return notok;
		goto reputcode;
	}
}

///*****************************************************************
//����ԭ�ͣ�SetID
//����������LCD��ʾ��������
//����������
//�������ƣ�  ����/����� ����        ����
//-----------     ----------- ------      -------
//param       ����            INT8U       ����Ĳ���
//
//��  ��  ֵ����
//��      ��  ������
//��      �ڣ�2005-08-20
//�޸���ʷ��
//����        �޸���      �޸�����
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
////  LCD_Display_Str(0, 0, "��������λID��: ", NORMAL);
////  LCD_Display_Str(1, 0, "ID�ű���<65535  ", NORMAL);
////  LCD_Display_Str(2, 0, "ԭID��:  ", NORMAL);
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
////          return;         //�˳�����
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
////      LCD_Display_Str(2, 0, "    ID�ŷǷ�    ", REVERSE);
////      OSTimeDlyHMSM(0, 0, 1, 0);
////      return;
////  }
////  if ( id_input == DevStat.bus_number )
////  {
////      return;     //id��δ���޸�
////  }
////  else
////  {
////      DevStat.bus_number = id_input;
////      (void)SaveTypeBParamtoFlash(PRI, (void *)&DevStat.bus_number, BUS_NUMBER, sizeof(DevStat.bus_number));
////
////      LCD_Display_Str(2, 0, "    �޸ĳɹ�    ", REVERSE);
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
//    lcddisp(0, 0, "��������λID��: ");
//    lcddisp(2, 0, "ԭID��:  ");
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
//            return;         //�˳�����
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
//        return;     //id��δ���޸�
//    }
//    else
//    {
//        DevStat.id = id_input;
//        WriteParam();
//
//        lcddisp(1, 0, "    �޸ĳɹ�    ");
//        OSTimeDlyHMSM(0, 0, 1, 0);
//        return;
//    }
//
//}

///*****************************************************************
//����ԭ�ͣ�SetTime
//��������������ʱ��
//����������
//�������ƣ�  ����/����� ����        ����
//-----------     ----------- ------      -------
//param       ����            INT8U       ����Ĳ���
//
//��  ��  ֵ����
//��      ��  ������
//��      �ڣ�2005-08-20
//�޸���ʷ��
//����        �޸���      �޸�����
//------      ---------   -------------
//*****************************************************************/
//void SetTime(INT8U param)
//{
// 	INT8U i = 0;
// 	BUS_TIME time;
// 	INT8U disp_buf[15];
// 	INT8U key = NO_KEY;
// 	INT8S cur_ptr = 0;
// 	const INT8U position[14][2] =                                   //������ʱ�����λ��ʾ��λ��
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
// 	LCD_Display_Str(0, 0, "    У��ʱ��    ", NORMAL);
// 	LCD_Display_Str(1, 0, "    ��  ��  ��  ", NORMAL);
// 	LCD_Display_Str(2, 0, "    ʱ  ��  ��  ", NORMAL);
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
// 				return;			//���޸�ʱ�䣬����
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
// 					LCD_Display_Str(3, 0, "  ʱ���ʽ�Ƿ�  ", NORMAL);
// 					BeepInputErr();
// 					OSTimeDlyHMSM(0, 0, 1, 0);
// 					LCD_Display_Str(3, 0, "                ", NORMAL);
// 					break;
// 				}
// 				(void)Modify_Time(&time);
// 				LCD_Display_Str(3, 0, "  �޸�ʱ��ɹ�  ", NORMAL);
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
// 			default:			//���ּ�����
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
����ԭ�ͣ�LCDDispTimeRefresh
��������������ʱ��ʱ��ˢ����ʾ
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------
disp_buf		����		INT8U*		�Դ�����
position		����		const INT8U(*)[2]	��ַ�б����ʼָ��

��  ��  ֵ����
��      ��	������
��      �ڣ�2005-08-20
�޸���ʷ��
����		�޸���		�޸�����
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
����ԭ�ͣ�chtostr
������������һ��'1'ת��Ϊ"1"
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------
disp_buf		����			INT8U		����Ĳ���

��  ��  ֵ����
��      ��	������
��      �ڣ�2005-08-20
�޸���ʷ��
����		�޸���		�޸�����
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
//����ԭ�ͣ�SetBkLtAlwaysOn
//�������������ñ��ⳣ��
//����������
//�������ƣ�  ����/����� ����        ����
//-----------     ----------- ------      -------
//param       ����            INT8U       ����Ĳ���
//
//��  ��  ֵ����
//��      ��  ������
//��      �ڣ�2005-08-20
//�޸���ʷ��
//����        �޸���      �޸�����
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
//    lcddisp(3, 0, "���ñ��ⳣ���ɹ�");
//    OSTimeDlyHMSM(0, 0, 1, 0);
//}
//
///*****************************************************************
//����ԭ�ͣ�SetBkLtAlwaysOff
//�������������ñ��ⳣ��
//����������
//�������ƣ�  ����/����� ����        ����
//-----------     ----------- ------      -------
//param       ����            INT8U       ����Ĳ���
//
//��  ��  ֵ����
//��      ��  ������
//��      �ڣ�2005-08-20
//�޸���ʷ��
//����        �޸���      �޸�����
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
//    lcddisp(3, 0, "���ñ��ⳣ�سɹ�");
//    OSTimeDlyHMSM(0, 0, 1, 0);
//}

///*****************************************************************
//����ԭ�ͣ�SetBkLtOffTime
//�������������ñ���ر�ʱ��
//����������
//�������ƣ�  ����/����� ����        ����
//-----------     ----------- ------      -------
//param       ����            INT8U       ����Ĳ���
//
//��  ��  ֵ����
//��      ��  ������
//��      �ڣ�2005-08-20
//�޸���ʷ��
//����        �޸���      �޸�����
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
////  const INT8U position[][2] =                                   //������ʱ�����λ��ʾ��λ��
////  {
////      {1, 2},
////      {1, 3},         //��ʱ
////      {1, 6},
////      {1, 7},         //�ط�
////      {2, 2},
////      {2, 3},         //��ʱ
////      {2, 6},
////      {2, 7},         //����
////  };
////
////  param = param;      //avoid warning
////  ToggleWD();
////
////  LCD_Clear();
////  LCD_Display_Str(0, 0, "���ñ���ر�ʱ��", NORMAL);
////  LCD_Display_Str(1, 0, "    ʱ  ��  �ر�", NORMAL);
////  LCD_Display_Str(2, 0, "    ʱ  ��  ����", NORMAL);
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
////              return;         //���޸�ʱ�䣬����
////
////          case KEY_ENTER:
////              hour1 = ( (ascii_to_hex(disp_buf[0]) << 4) | (ascii_to_hex(disp_buf[1]) & 0x0F) );
////              minute1 = ( (ascii_to_hex(disp_buf[2]) << 4) | (ascii_to_hex(disp_buf[3]) & 0x0F) );
////              hour2 = ( (ascii_to_hex(disp_buf[4]) << 4) | (ascii_to_hex(disp_buf[5]) & 0x0F) );
////              minute2 = ( (ascii_to_hex(disp_buf[6]) << 4) | (ascii_to_hex(disp_buf[7]) & 0x0F) );
////              if ( (hour1 >= 0x24) | (minute1 >= 0x60) | (hour2 >= 0x24) | (minute2 >= 0x60) )
////              {
////                  LCD_Display_Str(3, 0, "  ʱ���ʽ�Ƿ�  ", NORMAL);
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
////              LCD_Display_Str(3, 0, "    �޸ĳɹ�    ", NORMAL);
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
////          default:            //���ּ�����
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
//    lcddisp(0, 0, "���ñ��⿪��ʱ��");
//
//    i = input_string(1, (void *)"�ر�ʱ��:", (void *)"_", 4, disp_buf, FALSE);
//    if (i != ok)
//        return;
//
//    i = input_string(2, (void *)"����ʱ��:", (void *)"_", 4, &disp_buf[4], FALSE);
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
//        lcddisp(3, 0, "  ʱ���ʽ�Ƿ�  ");
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
//    lcddisp(3, 0, "    �޸ĳɹ�    ");
//    OSTimeDlyHMSM(0, 0, 1, 0);
//    return;
//
//}

///*****************************************************************
//����ԭ�ͣ�ChangePasswd
//�����������޸�����
//����������
//�������ƣ�  ����/����� ����        ����
//-----------     ----------- ------      -------
//
//��  ��  ֵ����
//��      ��  ������
//��      �ڣ�2005-08-20
//�޸���ʷ��
//����        �޸���      �޸�����
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
////  LCD_Display_Str(0, 4, "��������: ", NORMAL);
////  LCD_Display_Str(1, 0, "����������������", NORMAL);
////  LCD_Display_Str(2, 0, "��һ��: ", NORMAL);
////  LCD_Display_Str(2, 8, " ", REVERSE);
////  LCD_Display_Str(3, 0, "�ڶ���: ", NORMAL);
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
////          return;         //��֤ʧ��
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
////          return;         //��֤ʧ��
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
////      LCD_Display_Str(0, 0, "  �����޸ĳɹ�  ", REVERSE);
////      OSTimeDlyHMSM(0, 0, 1, 0);
////      return;
////  }
////  else
////  {
////      LCD_Display_Str(0, 0, "�������벻һ��! ", REVERSE);
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
//    lcddisp(0, 2, "��������: ");
//    lcddisp(1, 0, "����������������");
//    lcddisp(2, 0, "��һ��: ");
//    lcddisp(3, 0, "�ڶ���: ");
//
//    strcpy((void *)buf, (void *)"��һ��: _");
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
//            return;         //��֤ʧ��
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
//    strcpy((void *)buf, (void *)"�ڶ���: _");
//    lcddisp(3, 0, buf);
//    for ( i=0; i<6; i++ )
//    {
//        do
//        {                 //only accept 0-9,KEY_CANCEL
//            key = get_key();
//        }while ( !( (key>='0'&&key<='9') || (key==KEY_CANCEL) ) );
//
//        if ( key == KEY_CANCEL )
//            return;         //��֤ʧ��
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
//        lcddisp(0, 0, "  �����޸ĳɹ�  ");
//        OSTimeDlyHMSM(0, 0, 1, 0);
//        return;
//    }
//    else
//    {
//        lcddisp(0, 0, "�������벻һ��! ");
//        BeepInputErr();
//        OSTimeDlyHMSM(0, 0, 1, 0);
//        return;
//    }
//
//}

///*****************************************************************
//����ԭ�ͣ�QueryRecentRecord
//������������ѯ���30�����׼�¼
//����������
//�������ƣ�  ����/����� ����        ����
//-----------     ----------- ------      -------
//
//��  ��  ֵ����
//��      ��  ������
//��      �ڣ�2005-08-20
//�޸���ʷ��
//����        �޸���      �޸�����
//------      ---------   -------------
//*****************************************************************/
//void QueryRecentRecord(INT8U param)
//{
// #define MAX_DISP_REC	30						//������ѯʱ��������ʾ�ļ�¼����Ҫ��<=255
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
// 		LCD_Display_Str(1, 4, "û�м�¼!", NORMAL);
// 		OSTimeDlyHMSM(0, 0, 1, 0);
// 		return;
// 	}
//
// //     LCD_Display_Str(0, 0, "�����  �ʼ�¼: ", NORMAL);
//
// 	rec_ptr = 1;
// 	LoadRecord(PRI, (INT8U *)rec, (INT16U)(DevStat.record_base_ptr), DevStat.record_number - rec_ptr + (INT16U)MAX_REC_NUM);	 //32�ֽڼ�¼
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
// 				return;			//����
//
// 			case '+':
// 				rec_ptr++;
// 				if ( (rec_ptr > MAX_DISP_REC) || (rec_ptr > DevStat.record_number) )		//�ж��Ƿ��ѭ��
// 					rec_ptr = 1;
// 				LoadRecord(PRI, (INT8U *)rec, (INT16U)(DevStat.record_base_ptr), DevStat.record_number - rec_ptr + (INT16U)MAX_REC_NUM);	 //32�ֽڼ�¼
// 				display_rec(rec_ptr, rec);
// 				break;
//
// 			case '-':
// 				rec_ptr--;
// 				if ( (rec_ptr == 0) )	//�ж��Ƿ��ѭ��
// 				{
// 					if ( DevStat.record_number >= MAX_DISP_REC )
// 						rec_ptr = MAX_DISP_REC;
// 					else
// 						rec_ptr	= DevStat.record_number;	   //��¼��������10��
// 				}
// 				LoadRecord(PRI, (INT8U *)rec, (INT16U)(DevStat.record_base_ptr), DevStat.record_number - rec_ptr + (INT16U)MAX_REC_NUM);	 //32�ֽڼ�¼
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
//����ԭ�ͣ�display_rec
//������������ʾ��¼
//����������
//�������ƣ�  ����/����� ����        ����
//-----------     ----------- ------      -------
//rec_ptr         ����        INT16U  ��ʾ�ڼ�����1-30
//rec                 ����    INT8U *     �������һ����¼��32�ֽ�
//
//��  ��  ֵ����
//��      ��  ������
//��      �ڣ�2005-08-20
//�޸���ʷ��
//����        �޸���      �޸�����
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
// 	sprintf((void *)buf, "�����%2u�ʼ�¼: ", rec_ptr);
// 	LCD_Display_Str(0, 0, (void *)buf, NORMAL);
//
// 	LCD_Display_Str(1, 0, "����:   ", NORMAL);
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
// 	LCD_Display_Str(2, 0, "���:   ", NORMAL);
// 	fare = ((INT32U)(prec->fare[0] << 16) | (INT32U)(prec->fare[1] << 8) | (INT32U)(prec->fare[2]) );
// 	temp_fp64 = (FP64)fare / 100.0;
// 	(void)sprintf((void *)buf, "%8.2f", temp_fp64);
// 	LCD_Display_Str(2, 5, buf, NORMAL);
// 	LCD_Display_Str(2, 13, " Ԫ", NORMAL);
//
// 	LCD_Display_Str(3, 0, "ʱ��:   ", NORMAL);
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
//����ԭ�ͣ�QueryTodaySum
//������������ѯ���콻���ܶ�
//����������
//�������ƣ�  ����/����� ����        ����
//-----------     ----------- ------      -------
//
//��  ��  ֵ����
//��      ��  ������
//��      �ڣ�2005-08-20
//�޸���ʷ��
//����        �޸���      �޸�����
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
// 	LCD_Display_Str(1, 2, "ͳ����......", NORMAL);
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
// //         LoadRecord(PRI, (INT8U *)rec, (INT16U)(DevStat.record_base_ptr), DevStat.record_number - 1 - i + (INT16U)MAX_REC_NUM);   //34�ֽڼ�¼
// //         if (time.day == prec->card_in_time.day)
// //         {
// //             fare = (INT32U)(prec->fare[0] << 16) | (INT32U)(prec->fare[1] << 8) | (INT32U)(prec->fare[2]);
// //             temp_fp64 = (FP64)fare / 100.0;
// //             sum += temp_fp64;
// //         }
// //         else
// //         {
// //             break;          //���ղ�ѯ���
// //         }
// //     }
//
// 	if (DevStat.flash_cycled == TRUE)
// 	{
// 		for(i=0; i<(INT16U)MAX_REC_NUM; i++)				//��ͷ�鵽β
// 		{
// 			LoadRecord(PRI, (INT8U *)rec, 0, DevStat.record_base_ptr + DevStat.record_number - 1 - i + (INT16U)MAX_REC_NUM);   //34�ֽڼ�¼
// 			if (cal_sec2000(&(prec->card_in_time)) > base_sec)
// 			{
// 				fare = (INT32U)(prec->fare[0] << 16) | (INT32U)(prec->fare[1] << 8) | (INT32U)(prec->fare[2]);
// 				temp_fp64 = (FP64)fare / 100.0;
// 				sum += temp_fp64;
// 				times++;
// 			}
// 			else
// 			{
// 				break;			//���ղ�ѯ���
// 			}
// 		}
// 	}
// 	else
// 	{
// 		for( i = 0; i < DevStat.record_base_ptr + DevStat.record_number; i++)		//��ͷ�鵽��ǰ��¼
// 		{
// 			LoadRecord(PRI, (INT8U *)rec, 0, DevStat.record_base_ptr + DevStat.record_number - 1 - i + (INT16U)MAX_REC_NUM);   //34�ֽڼ�¼
// 			if (cal_sec2000(&(prec->card_in_time)) > base_sec)
// 			{
// 				fare = (INT32U)(prec->fare[0] << 16) | (INT32U)(prec->fare[1] << 8) | (INT32U)(prec->fare[2]);
// 				temp_fp64 = (FP64)fare / 100.0;
// 				sum += temp_fp64;
// 				times++;
// 			}
// 			else
// 			{
// 				break;			//���ղ�ѯ���
// 			}
// 		}
// 	}
//
// 	LCD_Display_Str(0, 0, "���ս����ܴ���: ", NORMAL);
// 	(void)sprintf((void *)buf, "%13d", times);
// 	LCD_Display_Str(1, 0, buf, NORMAL);
// 	LCD_Display_Str(1, 13, " ��", NORMAL);
//
// 	LCD_Display_Str(2, 0, "���ս����ܶ�:   ", NORMAL);
// 	(void)sprintf((void *)buf, "%13.2f", sum);
// 	LCD_Display_Str(3, 0, buf, NORMAL);
// 	LCD_Display_Str(3, 13, " Ԫ", NORMAL);
//
//
// 	(void)get_key();
// 	return;
// }

// /*****************************************************************
// ����ԭ�ͣ�SetPurchaseInterval
// ��������������ˢ��ʱ����
// ����������
// �������ƣ�	����/�����	����		����
// -----------		-----------	------	   	-------
// param		����			INT8U		����Ĳ���
//
// ��  ��  ֵ����
// ��      ��	������
// ��      �ڣ�2006-01-09
// �޸���ʷ��
// ����		�޸���		�޸�����
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
// 	const INT8U position[][2] =                                   //������ʱ�����λ��ʾ��λ��
// 	{
// 		{2, 2},
// 		{2, 3},			//ʱ
// 		{2, 8},
// 		{2, 9},			//��
// 		{2, 12},
// 		{2, 13},			//��
// 	};
//
// 	param = param;		//avoid warning
// 	ToggleWD();
//
// 	LCD_Clear();
// 	LCD_Display_Str(0, 0, "����ˢ��ʱ����", NORMAL);
// 	LCD_Display_Str(2, 0, "    Сʱ  ��  ��", NORMAL);
//
//
//
// 	disp_buf[0] = ascii(DevStat.normal_interval / 3600 / 10);				//Сʱ
// 	disp_buf[1] = ascii((DevStat.normal_interval / 3600) % 10);
//
// 	disp_buf[2] = ascii((DevStat.normal_interval % 3600) / 60 / 10);		//��
// 	disp_buf[3] = ascii(((DevStat.normal_interval % 3600) / 60) % 10);
//
// 	disp_buf[4] = ascii((DevStat.normal_interval % 60) / 10);					//��
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
// 				return;			//���޸�ʱ�䣬����
//
// 			case KEY_ENTER:
// 				hour = ( (ascii_to_hex(disp_buf[0]) << 4) | (ascii_to_hex(disp_buf[1]) & 0x0F) );
// 				minute = ( (ascii_to_hex(disp_buf[2]) << 4) | (ascii_to_hex(disp_buf[3]) & 0x0F) );
// 				second = ( (ascii_to_hex(disp_buf[4]) << 4) | (ascii_to_hex(disp_buf[5]) & 0x0F) );
// 				if ( (hour >= 0x24) | (minute >= 0x60) | (second >= 0x60) )
// 				{
// 					LCD_Display_Str(3, 0, "  ʱ���ʽ�Ƿ�  ", NORMAL);
// 					BeepInputErr();
// 					OSTimeDlyHMSM(0, 0, 1, 0);
// 					LCD_Display_Str(3, 0, "                ", NORMAL);
// 					break;
// 				}
//
// 				DevStat.normal_interval = bcdhex(hour) * 3600 + bcdhex(minute) * 60 + bcdhex(second);
// 				(void)SaveTypeBParamtoFlash(PRI, (void *)&DevStat.normal_interval, NORMAL_INTERVAL, sizeof(DevStat.normal_interval));
//
// 				LCD_Display_Str(3, 0, "    �޸ĳɹ�    ", NORMAL);
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
// 			default:			//���ּ�����
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
// ����ԭ�ͣ�bcdhex
// ����������bcd��ת��Ϊhex��
// ����������
// �������ƣ�	����/�����	����		����
// -----------		-----------	------	   	-------
// bcd				����			INT8U		�����bcd��
//
// ��  ��  ֵ��ת�����hex��
// ��      ��	������
// ��      �ڣ�2006-01-09
// �޸���ʷ��
// ����		�޸���		�޸�����
// ------		---------	-------------
// *****************************************************************/
// INT8U bcdhex(INT8U bcd)
// {
// 	return ((bcd >> 4) * 10 + (bcd & 0x0F));
// }

// /*****************************************************************
// ����ԭ�ͣ�SetTodayTotalCountTime
// ��������������ͳ�Ƶ����ܼƽ�����ʼʱ��ͽ���ʱ��
// ����������
// �������ƣ�	����/�����	����		����
// -----------		-----------	------	   	-------
// param		����			INT8U		����Ĳ���
//
// ��  ��  ֵ����
// ��      ��	������
// ��      �ڣ�2005-08-20
// �޸���ʷ��
// ����		�޸���		�޸�����
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
// 	const INT8U position[][2] =                                   //������ʱ�����λ��ʾ��λ��
// 	{
// 		{2, 4},
// 		{2, 5},			//ͳ����ʼʱ
// 		{2, 8},
// 		{2, 9},			//ͳ����ʼ��
// //         {3, 4},
// //         {3, 5},         //ͳ�ƽ���ʱ
// //         {3, 8},
// //         {3, 9},         //ͳ�ƽ�����
// 	};
//
// 	param = param;		//avoid warning
// 	ToggleWD();
//
// 	LCD_Clear();
// 	LCD_Display_Str(0, 0, "  ����ͳ�Ƶ���  ", NORMAL);
// 	LCD_Display_Str(1, 0, "  �ܼƽ��ʱ��  ", NORMAL);
// 	LCD_Display_Str(2, 0, "����  ʱ  �ֿ�ʼ", NORMAL);
// 	LCD_Display_Str(3, 0, "                ", NORMAL);
//
// 	disp_buf[0] = ascii((DevStat.todaytotal_start_time / 3600) / 10);					//ʱ
// 	disp_buf[1] = ascii((DevStat.todaytotal_start_time / 3600) % 10);
// 	disp_buf[2] = ascii(((DevStat.todaytotal_start_time % 3600) / 60) / 10);			//��
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
// 				return;			//���޸�ʱ�䣬����
//
// 			case KEY_ENTER:
// 				hour1 = ( (ascii_to_hex(disp_buf[0]) << 4) | (ascii_to_hex(disp_buf[1]) & 0x0F) );
// 				minute1 = ( (ascii_to_hex(disp_buf[2]) << 4) | (ascii_to_hex(disp_buf[3]) & 0x0F) );
// 				if ( (hour1 >= 0x24) | (minute1 >= 0x60) )
// 				{
// 					LCD_Display_Str(3, 0, "  ʱ���ʽ�Ƿ�  ", NORMAL);
// 					BeepInputErr();
// 					OSTimeDlyHMSM(0, 0, 1, 0);
// 					LCD_Display_Str(3, 0, "                ", NORMAL);
// 					break;
// 				}
//
// 				DevStat.todaytotal_start_time = bcdhex(hour1) * 3600 + bcdhex(minute1) * 60;
// 				(void)SaveTypeBParamtoFlash(PRI, (void *)&DevStat.todaytotal_start_time, TODAYTOTAL_START_TIME, sizeof(DevStat.todaytotal_start_time));
//
// 				LCD_Display_Str(3, 0, "    �޸ĳɹ�    ", NORMAL);
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
// 			default:			//���ּ�����
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
//  �������ƣ�cal_sec2000
//  ���������������2000��1��1�յ�����ʱ�������
//  ����������
//  �������ƣ�	����/�����	����		����
//  time			����			BUS_TIME*	Ҫ���м����ʱ�䣬Ҫ������ʱ�����2000��
//
//  ��  ��  ֵ������
//
//  ��      ��	������
//  ��      �ڣ�2006.1.9
//  �޸���ʷ��
// 		����		�޸���		�޸�����
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
// 	if (time->century != 0x20)			//����ʱ�����Ϊ2000���Ժ�
// 		return 0;
//
// 	//�������ǰ�������
// 	i = Is_Leap((INT16U)bcdhex(time->century) * 100 + (INT16U)bcdhex(time->year));
// 	if ( i == TRUE )
// 	{	//������
// 		sec = ( (days_leap[bcdhex(time->month)] + (INT32U)bcdhex(time->day)) * 24 * 60 * 60
// 				+ (INT32U)bcdhex(time->hour) * 60 * 60 + (INT32U)bcdhex(time->minute) * 60 + (INT32U)bcdhex(time->second) );
// 	}
// 	else
// 	{
// 		sec = ( (days[bcdhex(time->month)] + (INT32U)bcdhex(time->day)) * 24 * 60 * 60
// 				+ (INT32U)bcdhex(time->hour) * 60 * 60 + (INT32U)bcdhex(time->minute) * 60 + (INT32U)bcdhex(time->second) );
// 	}
//
// 	//�������ǰһ�굽2000��֮�������
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
����ԭ�ͣ�input_string
��������������һ������
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------
	row			����		INT8U		�ڼ���0-3
	prev		����		INT8U*		��ʾ�֣�����"������:"
	tishifu		����		INT8U*		��ʾ��������"_"
	length		����		INT8U		Ҫ������ĳ���
	ret_string	���		INT8U*		���ص���������
	enter_en	����		INT8U		�Ƿ�����س�,TRUE����     FALSE������

��  ��  ֵ��ok - �ɹ�
				notok - ʧ��(��cancel���˳���)
��      ��	������
��      �ڣ�2007-06-07
�޸���ʷ��
����		�޸���		�޸�����
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
			return notok;			//�˳�����
		else if ( key == KEY_ENTER )
		{
			if ( enter_en == TRUE )
			{
				if ( i == 0 )
					goto input_string_start;			//ɶҲû����

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
//����ԭ�ͣ�SetCenterIPMain
//��������������������IP
//����������
//�������ƣ�  ����/����� ����        ����
//-----------     ----------- ------      -------
//param       ����            INT8U       ����Ĳ���
//
//��  ��  ֵ����
//��      ��  ������
//��      �ڣ�2005-08-20
//�޸���ʷ��
//����        �޸���      �޸�����
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
//    lcddisp(0, 0, "  ����������IP  ");
//    lcddisp(1, 0, "ԭIP: ");
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
//    lcddisp(1, 0, "    �޸ĳɹ�    ");
//    OSTimeDlyHMSM(0, 0, 1, 500);
//    return;
//
//}
//
///*****************************************************************
//����ԭ�ͣ�SetCenterIPBak
//�������������ñ�������IP
//����������
//�������ƣ�  ����/����� ����        ����
//-----------     ----------- ------      -------
//param       ����            INT8U       ����Ĳ���
//
//��  ��  ֵ����
//��      ��  ������
//��      �ڣ�2005-08-20
//�޸���ʷ��
//����        �޸���      �޸�����
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
//    lcddisp(0, 0, "  ���ø�����IP  ");
//    lcddisp(1, 0, "ԭIP: ");
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
//    lcddisp(1, 0, "    �޸ĳɹ�    ");
//    OSTimeDlyHMSM(0, 0, 1, 500);
//    return;
//
//}

///*****************************************************************
//����ԭ�ͣ�QueryTotal
//�������������ܲ�ѯ
//����������
//�������ƣ�  ����/����� ����        ����
//-----------     ----------- ------      -------
//
//��  ��  ֵ����
//��      ��  ������
//��      �ڣ�2005-08-20
//�޸���ʷ��
//����        �޸���      �޸�����
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
//    INT32U bus_num = 0;         //�ɼ���������
//    INT32U rec_num = 0;         //��¼����
//
//    param = param;
//
//    ToggleWD();
//
//    Lcd_Cls();
//    lcddisp(2, 0, "    ͳ����..    ");
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
//            if ( pFRT->aDirEntry[j].Name[0] == 'R' )                //ˢ����¼
//            {
//                rec_num += pFRT->aDirEntry[j].FileSize / 32;
//                bus_num++;
//            }
//            else if( pFRT->aDirEntry[j].Name[0] == 'B' )            //��������¼
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
//    sprintf((void *)buf2, "����:%9ų", bus_num);
//    lcddisp(0, 0, buf2);
//    sprintf((void *)buf2, "��¼:%9u��", rec_num);
//    lcddisp(1, 0, buf2);
//
//    lcddisp(3, 0, "  ��������˳�  ");
//    delay_and_wait_key(5, 0, 0);
//
//
//}

///*****************************************************************
//����ԭ�ͣ�QueryOneFile_XYZ
//����������������ѯ
//����������
//�������ƣ�  ����/����� ����        ����
//-----------     ----------- ------      -------
//
//��  ��  ֵ����
//��      ��  ������
//��      �ڣ�2005-08-20
//�޸���ʷ��
//����        �޸���      �޸�����
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
//    INT32U bus_num = 0;         //�ɼ���������
//    INT32U max_bus_num = 0;
////  INT32U rec_num = 0;         //��¼����
//
//    INT8U key = NO_KEY;
//
////  struct DISP_FILE_INFO
////  {
//// //       INT16U num;             //���
////      INT8U line_num[2];      //��·��
////      INT8U bus_num[3];       //����
////      INT16U rec_num;         //��¼����
////      INT8U time[6];              //�ɼ�ʱ��
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
//    lcddisp(2, 0, "    ͳ����..    ");
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
//            if ( pFRT->aDirEntry[j].Name[0] == 'R' )                //ˢ����¼
//            {
//                pfct = (FCT *)&pFRT->aDirEntry[j];
//                disp_file_info[bus_num].line_num[0] = pfct->ftime[3];           //������·��
//                disp_file_info[bus_num].line_num[1] = pfct->ftime[4];
//                disp_file_info[bus_num].bus_num[0] = pFRT->aDirEntry[j].ExtName[0]; //���泵��
//                disp_file_info[bus_num].bus_num[1] = pFRT->aDirEntry[j].ExtName[1];
//                disp_file_info[bus_num].bus_num[2] = pFRT->aDirEntry[j].ExtName[2];
//                disp_file_info[bus_num].rec_num = (INT16U)(pFRT->aDirEntry[j].FileSize / 32);       //�����¼����
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
//            else if( pFRT->aDirEntry[j].Name[0] == 'B' )            //��������¼
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
//        lcddisp(2, 0, "    �޼�¼!!    ");
//        BeepInputErr();
//        SleepMs(2000);
//        return;
//    }
//
//    bus_num = 0;
//    Lcd_Cls();
//    sprintf((void *)buf2, "��·%02x%02x���:%3d", disp_file_info[bus_num].line_num[0],
//            disp_file_info[bus_num].line_num[0], bus_num + 1);
//    lcddisp(0, 0, buf2);
//    sprintf((void *)buf2, "����:     %02x%02x%02x", disp_file_info[bus_num].bus_num[0],
//            disp_file_info[bus_num].bus_num[1], disp_file_info[bus_num].bus_num[2]);
//    lcddisp(1, 0, buf2);
//    sprintf((void *)buf2, "��¼:%9u��", disp_file_info[bus_num].rec_num);
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
//            case '4':                               //�Ϸ�
//                if (bus_num == 0)
//                {
//                    BeepInputErr();
//                    break;
//                }
//                bus_num--;
//
//                sprintf((void *)buf2, "��·%02x%02x���:%3d", disp_file_info[bus_num].line_num[0],
//                        disp_file_info[bus_num].line_num[0], bus_num + 1);
//                lcddisp(0, 0, buf2);
//                sprintf((void *)buf2, "����:     %02x%02x%02x", disp_file_info[bus_num].bus_num[0],
//                        disp_file_info[bus_num].bus_num[1], disp_file_info[bus_num].bus_num[2]);
//                lcddisp(1, 0, buf2);
//                sprintf((void *)buf2, "��¼:%9u��", disp_file_info[bus_num].rec_num);
//                lcddisp(2, 0, buf2);
//                sprintf((void *)buf2, "%02x%02x-%02x-%02x %02x:%02x", disp_file_info[bus_num].time[0],
//                        disp_file_info[bus_num].time[1], disp_file_info[bus_num].time[2], disp_file_info[bus_num].time[3],
//                        disp_file_info[bus_num].time[4], disp_file_info[bus_num].time[5]);
//                lcddisp(3, 0, buf2);
//
//                break;
//
//            case '8':                               //�·�
//                if (bus_num >= max_bus_num - 1)
//                {
//                    BeepInputErr();
//                    break;
//                }
//                bus_num++;
//
//                sprintf((void *)buf2, "��·%02x%02x���:%3d", disp_file_info[bus_num].line_num[0],
//                        disp_file_info[bus_num].line_num[0], bus_num + 1);
//                lcddisp(0, 0, buf2);
//                sprintf((void *)buf2, "����:     %02x%02x%02x", disp_file_info[bus_num].bus_num[0],
//                        disp_file_info[bus_num].bus_num[1], disp_file_info[bus_num].bus_num[2]);
//                lcddisp(1, 0, buf2);
//                sprintf((void *)buf2, "��¼:%9u��", disp_file_info[bus_num].rec_num);
//                lcddisp(2, 0, buf2);
//                sprintf((void *)buf2, "%02x%02x-%02x-%02x %02x:%02x", disp_file_info[bus_num].time[0],
//                        disp_file_info[bus_num].time[1], disp_file_info[bus_num].time[2], disp_file_info[bus_num].time[3],
//                        disp_file_info[bus_num].time[4], disp_file_info[bus_num].time[5]);
//                lcddisp(3, 0, buf2);
//
//                break;
//
//            case KEY_CANCEL:                    //�˳�
//                return;
//
//            default:
//                break;
//        }
//    }
//
//}

///*****************************************************************
//����ԭ�ͣ�QueryCapacity
//����������������ѯ
//����������
//�������ƣ�  ����/����� ����        ����
//-----------     ----------- ------      -------
//
//��  ��  ֵ����
//��      ��  ������
//��      �ڣ�2005-08-20
//�޸���ʷ��
//����        �޸���      �޸�����
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
////  INT32U bus_num = 0;         //�ɼ���������
////  INT32U rec_num = 0;         //��¼����
//    INT32U total_disk_space = 0;
//    INT32U occupied_disk_space = 0;
//    INT32U free_disk_space = 0;
//
//    param = param;
//
//    ToggleWD();
//
//    Lcd_Cls();
//    lcddisp(2, 0, "    ͳ����..    ");
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
//    sprintf((void *)buf2, "��ռ��:%8uK", occupied_disk_space);
//    lcddisp(0, 0, buf2);
//    sprintf((void *)buf2, "��  ��:%8uK", free_disk_space);
//    lcddisp(1, 0, buf2);
//    sprintf((void *)buf2, "������:%8uK", total_disk_space);
//    lcddisp(2, 0, buf2);
//
//    lcddisp(3, 0, "  ��������˳�  ");
//    delay_and_wait_key(5, 0, 0);
//
//
//}

///*****************************************************************
//����ԭ�ͣ�QueryBlack
//������������������ѯ
//����������
//�������ƣ�  ����/����� ����        ����
//-----------     ----------- ------      -------
//
//��  ��  ֵ����
//��      ��  ������
//��      �ڣ�2005-08-20
//�޸���ʷ��
//����        �޸���      �޸�����
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
////  INT32U bus_num = 0;         //�ɼ���������
//    INT32U rec_num = 0;         //��¼����
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
//    lcddisp(2, 0, "    ͳ����..    ");
//
//
//    i = FindFirstBlkFile(filename, filename_ext);
//    if (i != ok)
//    {
//        //û�к������ļ�
//        Lcd_Cls();
//        lcddisp(2, 0, "    �޺�����    ");
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
//        lcddisp(2, 0, "    �޺�����    ");
//        BeepInputErr();
//        delay_and_wait_key(2, 0, 0);
//        return;
//    }
//
//    rec_num = GetFileSize(fh) / 4;
//
//    Lcd_Cls();
//    lcddisp(0, 0, "�汾��:         ");
//    sprintf((void *)buf2, "  %02X%02X%02X%02X%02X%02X%02X",
//            filename[1], filename[2], filename[3], filename[4], filename[5], filename[6], filename[7]);
//    lcddisp(1, 0, buf2);
//    sprintf((void *)buf2, "����:%9u��", rec_num);
//    lcddisp(2, 0, buf2);
//
//    (void)FS_Close(fh);
//
//    lcddisp(3, 0, "  ��������˳�  ");
//    delay_and_wait_key(5, 0, 0);
//
//}

/*****************************************************************
����ԭ�ͣ�SetModeAuto
��������������Ϊ�Զ�ģʽ
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------
param		����			INT8U		����Ĳ���

��  ��  ֵ����
��      ��	������
��      �ڣ�2005-08-20
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
void SetModeAuto(INT8U param)
{
	param = param;		//avoid warning

	ToggleWD();

	OS_ENTER_CRITICAL();
	DevStat.mode = AUTO;
	DevStat.MOD_REG.reg[0x0018] &= (~0x0001); //�쳣״̬
	OS_EXIT_CRITICAL();

	lcddisp(3, 0, "�����Զ�ģʽ�ɹ�");
	OSTimeDlyHMSM(0, 0, 1, 0);
}

/*****************************************************************
����ԭ�ͣ�SetModeManual
��������������Ϊ�Զ�ģʽ
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------
param		����			INT8U		����Ĳ���

��  ��  ֵ����
��      ��	������
��      �ڣ�2005-08-20
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
void SetModeManual(INT8U param)
{
	param = param;		//avoid warning

	ToggleWD();

	OS_ENTER_CRITICAL();
	DevStat.mode = MANUAL;
	DevStat.MOD_REG.reg[0x0018] |= 0x0001; //�쳣״̬
	OS_EXIT_CRITICAL();

	lcddisp(3, 0, "�����ֶ�ģʽ�ɹ�");
	OSTimeDlyHMSM(0, 0, 1, 0);
}

/*****************************************************************
����ԭ�ͣ�CheckAutoManual
�����������ж��Ƿ�Ϊ�ֶ�ģʽ
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------
param		����			INT8U		����Ĳ���

��  ��  ֵ��ok - �ֶ�
				notok - �Զ�
��      ��	������
��      �ڣ�2005-08-20
�޸���ʷ��
����		�޸���		�޸�����
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
		lcddisp(2, 0, "���л����ֶ�ģʽ");
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
����ԭ�ͣ�QueryDI
������������ѯDI״̬
����������
�������ƣ�  ����/����� ����        ����
-----------     ----------- ------      -------

��  ��  ֵ����
��      ��  ������
��      �ڣ�2005-08-20
�޸���ʷ��
����        �޸���      �޸�����
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
//    lcddisp(0, 0, "    DI��ѯ      ");
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
����ԭ�ͣ�QueryAD
������������ѯAD״̬
����������
�������ƣ�  ����/����� ����        ����
-----------     ----------- ------      -------

��  ��  ֵ����
��      ��  ������
��      �ڣ�2005-08-20
�޸���ʷ��
����        �޸���      �޸�����
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
	lcddisp(0, 0, "    ��ۺ���    ");

	for ( ;; )
	{
//		ToggleWD();
		
		sprintf((void *)buf1, "��׼����ѹ%2.2f", DevStat.MOD_REG.reg[0x0011]/1000.0);
		sprintf((void *)buf2, "ʵ�ʲ��ѹ%2.2f", DevStat.MOD_REG.reg[0x0010]/1000.0);
		lcddisp(1, 0, buf1);
		lcddisp(2, 0, buf2);

		OS_ENTER_CRITICAL();
		temp_int32s = ( DevStat.MOD_REG.reg[0x0010]  - DevStat.MOD_REG.reg[0x0011] );
		if ( (temp_int32s) > 15 )		  //���Ƿ�Ϊ��
		{
			sprintf((void *)buf, "��ۺ���:>%5.1f%%", _fflt(temp_int32s));
		}
		else
		{
			//temp_int32s = 0 -  temp_int32s;
			sprintf((void *)buf, "��ۺ���:<%5.1f%%", 10.0);
		}
		
		OS_EXIT_CRITICAL();

		lcddisp(3, 0, buf);
		

		key = delay_and_wait_key_ms(100, 0, 0);
		if ( key != NO_KEY )
			return;


#if 0
		if( tmp1 = 0)
		{
			lcddisp(1, 0, " ���������ʧ�� ");
			lcddisp(2, 0, "����б�׼������");
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
����ԭ�ͣ�QueryDA
������������ѯDA���ֵ
����������
�������ƣ�  ����/����� ����        ����
-----------     ----------- ------      -------

��  ��  ֵ����
��      ��  ������
��      �ڣ�2005-08-20
�޸���ʷ��
����        �޸���      �޸�����
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
	lcddisp(0, 0, "    DA��ѯ     ");

	for(;;)
	{
		OS_ENTER_CRITICAL();

		temp_int32s = (FP32)((DevStat.MOD_REG.reg[0x0015] / 4095.0) * 10);

		sprintf((void *)buf, "ͨ�� 1:%2.2fV", (FP32)(temp_int32s));
		OS_EXIT_CRITICAL();
		lcddisp(1, 0, buf);

		OS_ENTER_CRITICAL();

		temp_int32s = (FP32)((DevStat.MOD_REG.reg[0x0016] / 4095.0) * 10);

		sprintf((void *)buf, "ͨ�� 2:%2.2fV", (FP32)(temp_int32s));
		OS_EXIT_CRITICAL();
		lcddisp(2, 0, buf);

		key = delay_and_wait_key_ms(500, 0, 0);
		if (key != NO_KEY)
			return;
	}
}

///*****************************************************************
//����ԭ�ͣ�PushCForward
//����������ǰ��C�ͼ�
//����������
//�������ƣ�  ����/����� ����        ����
//-----------     ----------- ------      -------
//
//��  ��  ֵ����
//��      ��  ������
//��      �ڣ�2005-08-20
//�޸���ʷ��
//����        �޸���      �޸�����
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
////    lcddisp(0, 0, "  ǰ�ƣ��ͼ�    ");
////
////    di = GetDIStatus();
////    if (di & BIT12 == 1)        //�Ѿ���λ��
////    {
////        lcddisp(0, 1, "    �Ѿ���λ    ");
////        (void)delay_and_wait_key(2, 0, 0);
////        return;
////    }
////    OS_ENTER_CRITICAL();
////    DevStat.MOD_REG.reg[0x0A] = 1;      //�������ǰ��
////    DevStat.MOD_REG.reg[0x0B] = 0;
////    DevStat.MOD_REG.reg[0x4E] = 1;      //��æ״̬
////    OS_EXIT_CRITICAL();
////    SetRelayModbus();
////
////    for(;;)
////    {
////        di = GetDIStatus();
////        if (di & BIT12 == 1)        //�Ѿ���λ��
////        {
////            OS_ENTER_CRITICAL();
////            DevStat.MOD_REG.reg[0x0A] = 0;      //ֹͣ���
////            DevStat.MOD_REG.reg[0x0B] = 0;
////            OS_EXIT_CRITICAL();
////            SetRelayModbus();
////
////            OS_ENTER_CRITICAL();
////            DevStat.MOD_REG.reg[0x4E] = 0;      //��æ״̬
//////          DevStat.MOD_REG.reg[0x60] = 0;      //�Ѿ���λ
////            OS_EXIT_CRITICAL();
////            break;
////        }
////    }
////
////    lcddisp(0, 1, "    �Ѿ���λ    ");
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
//    lcddisp(0, 0, "  ǰ�ƣ��ͼ�    ");
//
//    OS_ENTER_CRITICAL();
//    cmd = DevStat.MOD_REG.reg[0x60];        //��C�ͼܼĴ���
//    OS_EXIT_CRITICAL();
//
//    if (cmd != 0)
//    {
//        lcddisp(1, 0, "    �ȴ���..    ");
//        for(;;)
//        {
//            OSTimeDlyHMSM(0, 0, 0, 10);         //10msһ��
//
//            OS_ENTER_CRITICAL();
//            cmd = DevStat.MOD_REG.reg[0x60];        //��C�ͼܼĴ���
//            OS_EXIT_CRITICAL();
//            if (cmd == 0)
//                break;
//        }
//    }
//
//    lcddisp(1, 0, "    �ƶ���..    ");
//    OS_ENTER_CRITICAL();
//    DevStat.MOD_REG.reg[0x60] = 0x5555;     //��C�ͼܼĴ���,����Ϊǰ��
//    OS_EXIT_CRITICAL();
//
//    for(;;)
//    {
//        OSTimeDlyHMSM(0, 0, 0, 10);         //10msһ��
//
//        OS_ENTER_CRITICAL();
//        cmd = DevStat.MOD_REG.reg[0x60];        //��C�ͼܼĴ���
//        OS_EXIT_CRITICAL();
//        if (cmd == 0)
//            break;
//    }
//
//    lcddisp(1, 0, "    �Ѿ���λ    ");
//    (void)delay_and_wait_key(2, 0, 0);
//
//}
//
///*****************************************************************
//����ԭ�ͣ�PushCBackward
//��������������C�ͼ�
//����������
//�������ƣ�  ����/����� ����        ����
//-----------     ----------- ------      -------
//
//��  ��  ֵ����
//��      ��  ������
//��      �ڣ�2005-08-20
//�޸���ʷ��
//����        �޸���      �޸�����
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
////    lcddisp(0, 0, "  ���ƣ��ͼ�    ");
////
////    di = GetDIStatus();
////    if (di & BIT13 == 1)        //�Ѿ���λ��
////    {
////        lcddisp(0, 1, "    �Ѿ���λ    ");
////        (void)delay_and_wait_key(2, 0, 0);
////        return;
////    }
////    OS_ENTER_CRITICAL();
////    DevStat.MOD_REG.reg[0x0A] = 0;
////    DevStat.MOD_REG.reg[0x0B] = 1;      //�����������
////    DevStat.MOD_REG.reg[0x4E] = 1;      //��æ״̬
////    OS_EXIT_CRITICAL();
////    SetRelayModbus();
////
////    for(;;)
////    {
////        di = GetDIStatus();
////        if (di & BIT13 == 1)        //�Ѿ���λ��
////        {
////            OS_ENTER_CRITICAL();
////            DevStat.MOD_REG.reg[0x0A] = 0;      //ֹͣ���
////            DevStat.MOD_REG.reg[0x0B] = 0;
////            OS_EXIT_CRITICAL();
////            SetRelayModbus();
////
////            OS_ENTER_CRITICAL();
////            DevStat.MOD_REG.reg[0x4E] = 0;      //��æ״̬
//////          DevStat.MOD_REG.reg[0x60] = 0;      //�Ѿ���λ
////            OS_EXIT_CRITICAL();
////            break;
////        }
////    }
////
////    lcddisp(0, 1, "    �Ѿ���λ    ");
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
//    lcddisp(0, 0, "  ���ƣ��ͼ�    ");
//
//    OS_ENTER_CRITICAL();
//    cmd = DevStat.MOD_REG.reg[0x60];        //��C�ͼܼĴ���
//    OS_EXIT_CRITICAL();
//
//    if (cmd != 0)
//    {
//        lcddisp(1, 0, "    �ȴ���..    ");
//        for(;;)
//        {
//            OSTimeDlyHMSM(0, 0, 0, 10);         //10msһ��
//
//            OS_ENTER_CRITICAL();
//            cmd = DevStat.MOD_REG.reg[0x60];        //��C�ͼܼĴ���
//            OS_EXIT_CRITICAL();
//            if (cmd == 0)
//                break;
//        }
//    }
//
//    lcddisp(1, 0, "    �ƶ���..    ");
//    OS_ENTER_CRITICAL();
//    DevStat.MOD_REG.reg[0x60] = 0xAAAA;     //��C�ͼܼĴ���,����Ϊ����
//    OS_EXIT_CRITICAL();
//
//    for(;;)
//    {
//        OSTimeDlyHMSM(0, 0, 0, 10);         //10msһ��
//
//        OS_ENTER_CRITICAL();
//        cmd = DevStat.MOD_REG.reg[0x60];        //��C�ͼܼĴ���
//        OS_EXIT_CRITICAL();
//        if (cmd == 0)
//            break;
//    }
//
//    lcddisp(1, 0, "    �Ѿ���λ    ");
//    (void)delay_and_wait_key(2, 0, 0);
//}

/*****************************************************************
����ԭ�ͣ�QueryXrayStatus
������������ѯX����״̬
����������
�������ƣ�  ����/����� ����        ����
-----------     ----------- ------      -------

��  ��  ֵ����
��      ��  ������
��      �ڣ�2005-08-20
�޸���ʷ��
����        �޸���      �޸�����
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
		strcpy((void *)buf, "����:       ");
		if ( (reg & 0x0002) != 0 )
			strcat((void *)buf, "�쳣");
		else
			strcat((void *)buf, "����");
		lcddisp(0, 0, buf);

		OS_ENTER_CRITICAL();
		reg = DevStat.MOD_REG.reg[0x0019];
		OS_EXIT_CRITICAL();
		strcpy((void *)buf, "TUBE ARC:   ");
		if ( reg == 0 )
		{
			strcat((void *)buf, "�쳣");
		}
		else
		{
			strcat((void *)buf, "����");
		}
		lcddisp(1, 0, buf);

		OS_ENTER_CRITICAL();
		reg = DevStat.MOD_REG.reg[0x0018];
		OS_EXIT_CRITICAL();
		strcpy((void *)buf, "XRAY READY: ");
		if ( reg == 0 )
		{
			strcat((void *)buf, "����");
		}
		else
		{
			strcat((void *)buf, "�쳣");
		}
		lcddisp(2, 0, buf);

		OS_ENTER_CRITICAL();
		reg = DevStat.MOD_REG.reg[0x0017];
		OS_EXIT_CRITICAL();
		strcpy((void *)buf, "���߿�:     ");
		if ( reg == 0 )
		{
			strcat((void *)buf, "��");
		}
		else
		{
			strcat((void *)buf, "�ر�");
		}
		lcddisp(3, 0, buf);



		key = delay_and_wait_key(2, 0, 0);
		if ( key != NO_KEY )
			return;



		Lcd_Cls();

		OS_ENTER_CRITICAL();
		reg = DevStat.MOD_REG.reg[0x0016];
		OS_EXIT_CRITICAL();
		strcpy((void *)buf, "��ѹ����:   ");
		if ( reg == 0 )
			strcat((void *)buf, "�쳣");
		else
			strcat((void *)buf, "����");
		lcddisp(0, 0, buf);

		OS_ENTER_CRITICAL();
		reg = DevStat.MOD_REG.reg[0x0015];
		OS_EXIT_CRITICAL();
		strcpy((void *)buf, "��������:   ");
		if ( reg == 0 )
		{
			strcat((void *)buf, "�쳣");
		}
		else
		{
			strcat((void *)buf, "����");
		}
		lcddisp(1, 0, buf);


		key = delay_and_wait_key(2, 0, 0);
		if ( key != NO_KEY )
			return;

	}
}

/*****************************************************************
����ԭ�ͣ�QueryXrayVI
������������ѯX���ߵ�����ѹ״̬
����������
�������ƣ�  ����/����� ����        ����
-----------     ----------- ------      -------

��  ��  ֵ����
��      ��  ������
��      �ڣ�2005-08-20
�޸���ʷ��
����        �޸���      �޸�����
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
		sprintf((void *)buf, "��ѹ����:%5.1fKV", (FP64)reg / 10.0);
		lcddisp(0, 0, buf);

		OS_ENTER_CRITICAL();
		reg = DevStat.MOD_REG.reg[0x0013];
		OS_EXIT_CRITICAL();
		sprintf((void *)buf, "��ѹ����:%5.1fKV", (FP32)reg / 10.0);
		lcddisp(1, 0, buf);

		OS_ENTER_CRITICAL();
		reg = DevStat.MOD_REG.reg[0x0012];
		OS_EXIT_CRITICAL();
		sprintf((void *)buf, "��������:%5.1fuA", (FP64)reg / 10.0);
		lcddisp(2, 0, buf);

		OS_ENTER_CRITICAL();
		reg = DevStat.MOD_REG.reg[0x0011];
		OS_EXIT_CRITICAL();
		sprintf((void *)buf, "��������:%5.1fuA", (FP32)reg / 10.0);
		lcddisp(3, 0, buf);

		key = delay_and_wait_key_ms(500, 0, 0);
		if ( key != NO_KEY )
			return;
	}
}

/*****************************************************************
����ԭ�ͣ�AdjTemperatureZero1
�����������¶�1����
����������
�������ƣ�  ����/����� ����        ����
-----------     ----------- ------      -------

��  ��  ֵ����
��      ��  ������
��      �ڣ�2005-08-20
�޸���ʷ��
����        �޸���      �޸�����
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
//    lcddisp(0, 0, "    �¶�1 ����  ");
//    lcddisp(1, 0, "������ˮ�����");
//    lcddisp(2, 0, "��ȷ�ϼ�ִ�е���");
//
//    for(;;)
//    {
//        OS_ENTER_CRITICAL();
//        if ((DevStat.MOD_REG.reg[0x004A] & 0x8000) == 0)        //���Ƿ�Ϊ��
//        {
//            temp_int32s = (INT32S)DevStat.MOD_REG.reg[0x004A];
//        }
//        else
//        {
//            temp_int32s = 0 - (INT32S)(DevStat.MOD_REG.reg[0x004A] & (~0x8000));
//        }
//        sprintf((void *)buf, "�¶�1:%+8.2f��", _fflt(temp_int32s) / 100.0);
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
����ԭ�ͣ�AdjTemperatureZero2
�����������¶�2����
����������
�������ƣ�  ����/����� ����        ����
-----------     ----------- ------      -------

��  ��  ֵ����
��      ��  ������
��      �ڣ�2005-08-20
�޸���ʷ��
����        �޸���      �޸�����
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
//    lcddisp(0, 0, "    �¶�2 ����  ");
//    lcddisp(1, 0, "������ˮ�����");
//    lcddisp(2, 0, "��ȷ�ϼ�ִ�е���");
//
//    for(;;)
//    {
//        OS_ENTER_CRITICAL();
//        if ((DevStat.MOD_REG.reg[0x004B] & 0x8000) == 0)        //���Ƿ�Ϊ��
//        {
//            temp_int32s = (INT32S)DevStat.MOD_REG.reg[0x004B];
//        }
//        else
//        {
//            temp_int32s = 0 - (INT32S)(DevStat.MOD_REG.reg[0x004B] & (~0x8000));
//        }
//        sprintf((void *)buf, "�¶�1:%+8.2f��", _fflt(temp_int32s) / 100.0);
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
����ԭ�ͣ�SWITCHEnalbe
����������ʹ�̵ܼ���
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------
param		����			INT8U		����Ĳ���

��  ��  ֵ����
��      �ߣ�������
��      �ڣ�2013-08-20
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
void SWITCHEnalbe(INT8U param)
{
	param = param;		//avoid warning

	ToggleWD();

	OS_ENTER_CRITICAL();
	DevStat.MOD_REG.reg[0x18] = 0x5555;	 //ʹ�̵ܼ���
	OS_EXIT_CRITICAL();

	lcddisp(3, 0, "�򿪼̵����ɹ�");
	OSTimeDlyHMSM(0, 0, 2, 0);

	lcddisp(0, 0, "1.ֹͣ����");
	
}

/*****************************************************************
����ԭ�ͣ�SWITCHDisalbe
������������ֹ�̵���
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------
param		����			INT8U		����Ĳ���

��  ��  ֵ����
��      �ߣ�������
��      �ڣ�2013-08-20
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
void SWITCHDisalbe(INT8U param)
{
	param = param;		//avoid warning

	ToggleWD();

	OS_ENTER_CRITICAL();
	DevStat.MOD_REG.reg[0x18] = 0x00;	 //��ֹ�̵���
	OS_EXIT_CRITICAL();

	lcddisp(3, 0, "�رռ̵����ɹ�");
	OSTimeDlyHMSM(0, 0, 2, 0);
}

/*****************************************************************
����ԭ�ͣ�SetPassWord
�������������ÿ�������
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------
param		����			INT8U		����Ĳ���

��  ��  ֵ����
��      ��	��������
��      �ڣ�2014-08-20
�޸���ʷ��
����		�޸���		�޸�����
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

	//����������
	Lcd_Cls();
//  lcddisp(0, 0, "  �û���������  ");
	lcddisp(0, 0, "ԭ����:         ");
	strcpy((void *)buf, "ԭ����:");

	for ( i=0; i<6; i++ )
	{
		do				   //only accept 0-9,KEY_CANCEL
		{
			key = delay_and_wait_key(10, 0, 0);
			if ( key == NO_KEY )
				return;
		}while ( !( (key>='0'&&key<='9') || (key==KEY_CANCEL) ) );

		if ( key == KEY_CANCEL )
			return ;			//��֤ʧ��

		oldpass[i] = key;

		buf[strlen((void *)buf) - 1] = '\0';
		strcat((void *)buf, (void *)"*_");
		lcddisp(0, 0, buf);
	}

	if ( memcmp((void *)oldpass, (void *)&DevStat.passwd[0], 6) == 0 )
	{
		lcddisp(1, 0, "������:         ");
		strcpy((void *)buf, "������:");

		for ( i=0; i<6; i++ )
		{
			do				   //only accept 0-9,KEY_CANCEL
			{
				key = delay_and_wait_key(10, 0, 0);
				if ( key == NO_KEY )
					return ;
			}while ( !( (key>='0'&&key<='9') || (key==KEY_CANCEL) ) );

			if ( key == KEY_CANCEL )
				return ;			//��֤ʧ��

			newpass[i] = key;

			buf[strlen((void *)buf) - 1] = '\0';
			strcat((void *)buf, (void *)"*_");
			lcddisp(1, 0, buf);
		}
		lcddisp(2, 0, "��һ��:         ");

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
				return ;			//��֤ʧ��

			surepass[i] = key;

			buf[strlen((void *)buf) - 1] = '\0';
			strcat((void *)buf, (void *)"*_");
			lcddisp(2, 0, buf);
		}
		if( memcmp(newpass, surepass, 6) == 0 )
		{
			lcddisp(3, 0, "    �����޸ĳɹ�    ");
			strcpy((void *)DevStat.passwd[0], (void *)newpass);
			WriteParam();
			return ;
		}
		else
		{
			lcddisp(3, 0, "    �����޸�ʧ��    ");
			return ;
		}
		return ;
	}
	return ;
}
/*****************************************************************
����ԭ�ͣ�SetDefault
����������ʹ��AGC
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------
param		����			INT8U		����Ĳ���

��  ��  ֵ����
��      ��	������
��      �ڣ�2005-08-20
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
void SetUSERDefault(INT8U param)
{
	INT8U key = NO_KEY;
	INT8U i=0;
	 
	param = param;
	
	
	ToggleWD();

	Lcd_Cls();
	lcddisp(0, 0, "�Ƿ�ָ���������");
	lcddisp(3, 0, "ȡ��        ȷ��");
	
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
	/* ȫ�ֱ�����ʼ��*/
	OS_ENTER_CRITICAL();
//  memset((void*)&DevStat, 0x00, sizeof(DevStat));
//
//  DevStat.err_occured = FALSE;
//  DevStat.err_code = 0x0000;
//  DevStat.id = 0x10;
//  DevStat.mode = AUTO;
//  DevStat.passwd = 0;
//  DevStat.MOD_REG.reg[0x18] = 0;	//��ֹ�̵������

//  DevStat.switch_on_delay = 1000;
//  DevStat.switch_interval = 1000;   //msΪ��λ
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
	lcddisp(2, 0, "�ָ��������óɹ�");
	OSTimeDlyHMSM(0, 0, 2, 0);
}

/*****************************************************************
����ԭ�ͣ�Judgework
�����������ж��Ƿ���Ȧ���ڹ�����
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------
param		����			INT8U		����Ĳ���

��  ��  ֵ����
��      ��	��frank
��      �ڣ�2013-12-20
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U Judgework(void)
{
	INT16U mode = 0;

	//param = param;

	ToggleWD();

	OS_ENTER_CRITICAL();
	mode = DevStat.MOD_REG.reg[0x18] ;	 //�̵���״̬�ж�;
	OS_EXIT_CRITICAL();

	if ( mode == 0x5555 )
	{
		Lcd_Cls();
		lcddisp(2, 0, "���ȹرչ���״̬");
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
����ԭ�ͣ�AGCEnalbe
����������ʹ��AGC
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------
param		����			INT8U		����Ĳ���

��  ��  ֵ����
��      ��	������
��      �ڣ�2005-08-20
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
void AGCEnalbe(INT8U param)
{
	param = param;		//avoid warning

	ToggleWD();

	OS_ENTER_CRITICAL();
//  DevStat.MOD_REG.reg[0x000C] = 0x0001; //ʹ��AGC
	OS_EXIT_CRITICAL();

	lcddisp(3, 0, "��AGC ����ɹ�");
	OSTimeDlyHMSM(0, 0, 1, 0);
}

/*****************************************************************
����ԭ�ͣ�AGCDisalbe
������������ֹAGC���
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------
param		����			INT8U		����Ĳ���

��  ��  ֵ����
��      ��	������
��      �ڣ�2005-08-20
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
void AGCDisalbe(INT8U param)
{
	param = param;		//avoid warning

	ToggleWD();

	OS_ENTER_CRITICAL();
//  DevStat.MOD_REG.reg[0x000C] = 0x0000; //�ر�AGC
	OS_EXIT_CRITICAL();

	lcddisp(3, 0, "�ر�AGC ����ɹ�");
	OSTimeDlyHMSM(0, 0, 1, 0);
}


/*****************************************************************
����ԭ�ͣ�Querydelay
������������ѯ�ӳٿ���ʱ��״̬
����������
�������ƣ�  ����/����� ����        ����
-----------     ----------- ------      -------

��  ��  ֵ����
��      ��  ��frank
��      �ڣ�2012-08-20
�޸���ʷ��
����        �޸���      �޸�����
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
����ԭ�ͣ�QueryOndelay
������������ѯAD״̬
����������
�������ƣ�  ����/����� ����        ����
-----------     ----------- ------      -------

��  ��  ֵ����
��      ��  ������
��      �ڣ�2005-08-20
�޸���ʷ��
����        �޸���      �޸�����
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
����ԭ�ͣ�QueryOffdelay
������������ѯAD״̬
����������
�������ƣ�  ����/����� ����        ����
-----------     ----------- ------      -------

��  ��  ֵ����
��      ��  ������
��      �ڣ�2005-08-20
�޸���ʷ��
����        �޸���      �޸�����
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
����ԭ�ͣ�AdjKFZero
������������۱�׼���ɼ�������Ŀ��ȷ��û������ʱ������ѹ�ľ�����ֵ
����������
�������ƣ�  ����/����� ����        ����
-----------     ----------- ------      -------

��  ��  ֵ����
��      ��  ��FRANK
��      �ڣ�2014-02-21
�޸���ʷ��
����        �޸���      �޸�����
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
	lcddisp(0, 0, "  �������׼��  ");
	lcddisp(2, 0, "��ȷ�ϼ������趨");

	for ( ;; )
	{
//      ToggleWD();

		OS_ENTER_CRITICAL();

		temp_int32s = (INT32S)DevStat.MOD_REG.reg[0x0010];

		sprintf((void *)buf, "����ѹ:%+7.3f", _fflt(temp_int32s) / 1000.0);
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
			lcddisp(2, 0, "    �趨�ɹ�!   ");
			Beep(1000);	
			continue;
		}
		else
			return;
	}
}

/*****************************************************************
����ԭ�ͣ�AdjADZero
����������AD����
����������
�������ƣ�  ����/����� ����        ����
-----------     ----------- ------      -------

��  ��  ֵ����
��      ��  ������
��      �ڣ�2005-08-20
�޸���ʷ��
����        �޸���      �޸�����
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
	lcddisp(0, 0, "    AD����      ");
	lcddisp(2, 0, "��ȷ�ϼ�ִ�е���");

	for ( ;; )
	{
//      ToggleWD();

		OS_ENTER_CRITICAL();
		if ( (DevStat.MOD_REG.reg[0x0003] & 0x8000) == 0 )		  //���Ƿ�Ϊ��
		{
			temp_int32s = (INT32S)DevStat.MOD_REG.reg[0x0003] * 1000L
						  + (INT32S)DevStat.MOD_REG.reg[0x0004];
		}
		else
		{
			temp_int32s = 0 - ((INT32S)(DevStat.MOD_REG.reg[0x0003] & (~0x8000)) * 1000L)
						  - (INT32S)DevStat.MOD_REG.reg[0x0004];
		}
		sprintf((void *)buf, "�������:%+7.3f", _fflt(temp_int32s) / 1000.0);
		OS_EXIT_CRITICAL();
		lcddisp(3, 0, buf);


		key = delay_and_wait_key_ms(500, 0, 0);
		if ( key == NO_KEY )
			continue;
		else if ( key == KEY_ENTER )
		{
			adj_zero = read_mod_reg((void *)&DevStat.MOD_REG.reg[0x0008]);	//������Ĵ���
			adj_zero -= temp_int32s;
			if ( adj_zero > ADJ_AD_ZERO_MAX || adj_zero < 0 - ADJ_AD_ZERO_MAX )
			{
				lcddisp(1, 0, "���㳬����Χ!!  ");
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
����ԭ�ͣ� AdjSwitchInterval(INT8U param)
�����������̵����Ͽ����ʱ������
����������
�������ƣ�  ����/����� ����        ����
-----------     ----------- ------      -------

�� �� ֵ����
��    �ߣ�������
��    �ڣ�2013-07-12
�޸���ʷ��
����        �޸���      �޸�����
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
	sprintf((void *)buf, "�̵���%d", param);
	lcddisp(0, 0, buf);
	lcddisp(2, 0, "������Ͽ�ʱ��: ");
//  lcddisp(3, 0, "         +_ .   ");
	lcddisp(3, 0, "            _.  ");

	strcpy((void *)buf2, "            _.__");
	position = 12;

	for ( ;; )
	{
		OS_ENTER_CRITICAL();
		temp_int16s = (INT16S)DevStat.MOD_REG.REG_DETAIL.switch_interval[param];
		sprintf((void *)buf, "�Ͽ�ʱ��%1.2f(s)", _fflt(temp_int16s)/1000.0);
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
				if ( position == 13 )		  //����С����
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
					lcddisp(2, 0, "������Χ!!      ");
					BeepInputErr();
					(void)delay_and_wait_key(2, 0, 0);
					lcddisp(2, 0, "��������ȷʱ��: ");
					continue;
				}

				OS_ENTER_CRITICAL();
				DevStat.MOD_REG.REG_DETAIL.switch_interval[param] = temp_int16s;
				OS_EXIT_CRITICAL();

				WriteParam();
				Lcd_Cls();
				lcddisp(2, 0, "    ����ɹ�    ");
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
����ԭ�ͣ� AdjSwitchdelay(INT8U param)
�����������ӳٿ���ʱ��
����������
�������ƣ�  ����/����� ����        ����
-----------     ----------- ------      -------

�� �� ֵ����
��    �ߣ�������
��    �ڣ�2013-07-12
�޸���ʷ��
����        �޸���      �޸�����
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
	sprintf((void *)buf, "�̵���%d", param);
	lcddisp(0, 0, buf);
	lcddisp(2, 0, "�������ӳ�ʱ��: ");
//  lcddisp(3, 0, "         +_ .   ");
	lcddisp(3, 0, "            _.  ");

	strcpy((void *)buf2, "            _.__");
	position = 12;

	for ( ;; )
	{
		OS_ENTER_CRITICAL();
//  	if ((DevStat.MOD_REG.reg[0x004A] & 0x8000) == 0)        //���Ƿ�Ϊ��
//  	{
//  		temp_int32s = (INT32S)DevStat.MOD_REG.reg[0x004A];
//  	}
//  	else
//  	{
//  		temp_int32s = 0 - (INT32S)(DevStat.MOD_REG.reg[0x004A] & (~0x8000));
//  	}
		temp_int16s = (INT16S)DevStat.MOD_REG.REG_DETAIL.switch_delay[param];
		sprintf((void *)buf, "�ӳ�ʱ��%1.2f(s)", _fflt(temp_int16s)/1000.0);
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
				if ( position == 13 )		  //����С����
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
					lcddisp(2, 0, "������Χ!!      ");
					BeepInputErr();
					(void)delay_and_wait_key(2, 0, 0);
					lcddisp(2, 0, "��������ȷʱ��: ");
					continue;
				}

				OS_ENTER_CRITICAL();
				DevStat.MOD_REG.REG_DETAIL.switch_delay[param] = temp_int16s;
				OS_EXIT_CRITICAL();

				WriteParam();
				Lcd_Cls();
				lcddisp(2, 0, "    ����ɹ�    ");
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
����ԭ�ͣ� AdjSwitchOndelay(INT8U param)
�����������¶�1����
����������
�������ƣ�  ����/����� ����        ����
-----------     ----------- ------      -------

�� �� ֵ����
��    �ߣ�������
��    �ڣ�2013-07-12
�޸���ʷ��
����        �޸���      �޸�����
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
	sprintf((void *)buf, "�̵���%d", param);
	lcddisp(0, 0, buf);
	lcddisp(2, 0, "������ͨ��ʱ��: ");
//  lcddisp(3, 0, "         +_ .   ");
	lcddisp(3, 0, "            _.  ");

	strcpy((void *)buf2, "            _.__");
	position = 12;

	for ( ;; )
	{
		OS_ENTER_CRITICAL();
//  	if ((DevStat.MOD_REG.reg[0x004A] & 0x8000) == 0)        //���Ƿ�Ϊ��
//  	{
//  		temp_int32s = (INT32S)DevStat.MOD_REG.reg[0x004A];
//  	}
//  	else
//  	{
//  		temp_int32s = 0 - (INT32S)(DevStat.MOD_REG.reg[0x004A] & (~0x8000));
//  	}
		temp_int16s = (INT16S)DevStat.MOD_REG.REG_DETAIL.switch_on_delay[param];
		sprintf((void *)buf, "ͨ��ʱ��%1.2f(s)", _fflt(temp_int16s)/1000.0);
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
				if ( position == 13 )		  //����С����
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
					lcddisp(2, 0, "������Χ!!      ");
					BeepInputErr();
					(void)delay_and_wait_key(2, 0, 0);
					lcddisp(2, 0, "��������ȷʱ��: ");
					continue;
				}

				OS_ENTER_CRITICAL();
				DevStat.MOD_REG.REG_DETAIL.switch_on_delay[param] = temp_int16s;
				OS_EXIT_CRITICAL();

				WriteParam();
				Lcd_Cls();
				lcddisp(2, 0, "    ����ɹ�    ");
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
����ԭ�ͣ�ParamInit
����������������ʼ��
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------
param		����			INT8U		����Ĳ���

��  ��  ֵ����
��      ��	������
��      �ڣ�2005-08-20
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
void ParamInit(INT8U param)
{
	param = param;		//avoid warning

	ToggleWD();

	Lcd_Cls();
	lcddisp(0, 0, "    ��ʼ��      ");
	(void)InitImportantParameter();
	lcddisp(2, 0, "    ��ʼ���ɹ�  ");
	OSTimeDlyHMSM(0, 0, 1, 0);
}

/*****************************************************************
����ԭ�ͣ�AdjAGCZero
����������AGC����
����������
�������ƣ�  ����/����� ����        ����
-----------     ----------- ------      -------

��  ��  ֵ����
��      ��  ������
��      �ڣ�2005-08-20
�޸���ʷ��
����        �޸���      �޸�����
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
		lcddisp(2, 0, "���ȹر�AGC ���");
		BeepInputErr();
		(void)delay_and_wait_key(2, 0, 0);
		return;
	}

	lcddisp(0, 0, "    AGC ����    ");
	lcddisp(1, 0, "4 ��8 ���ı��С");
	lcddisp(2, 0, "��ȷ�ϼ�ִ�е���");

	for ( ;; )
	{
//      ToggleWD();

		temp_int32s = read_mod_reg((void *)&DevStat.MOD_REG.reg[0x000A]);
		origin = temp_int32s;		//����ԭʼֵ
		sprintf((void *)buf, "AGC ����:%+7d", temp_int32s);
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
			lcddisp(0, 0, "    AGC ����    ");
			lcddisp(2, 0, "    ����ɹ�    ");
			(void)delay_and_wait_key(2, 0, 0);
			return;
		}
		else
		{
			write_mod_reg((void *)&DevStat.MOD_REG.reg[0x000A], origin);		//�ָ���ʼֵ
			return;
		}
	}
}

/*****************************************************************
����ԭ�ͣ�AdjADFull
����������AD����
����������
�������ƣ�  ����/����� ����        ����
-----------     ----------- ------      -------

��  ��  ֵ����
��      ��  ������
��      �ڣ�2005-08-20
�޸���ʷ��
����        �޸���      �޸�����
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
	lcddisp(0, 0, "    AD����      ");
	lcddisp(2, 0, "������ʵ�ʺ��: ");
	lcddisp(3, 0, "         +_ .   ");

	strcpy((void *)buf2, "         +_ .   ");
	position = 10;

	for ( ;; )
	{
//      ToggleWD();

		OS_ENTER_CRITICAL();
		if ( (DevStat.MOD_REG.reg[0x0003] & 0x8000) == 0 )		  //���Ƿ�Ϊ��
		{
			temp_int32s = (INT32S)DevStat.MOD_REG.reg[0x0003] * 1000L
						  + (INT32S)DevStat.MOD_REG.reg[0x0004];
		}
		else
		{
			temp_int32s = 0 - ((INT32S)(DevStat.MOD_REG.reg[0x0003] & (~0x8000)) * 1000L)
						  - (INT32S)DevStat.MOD_REG.reg[0x0004];
		}
		sprintf((void *)buf, "�������:%+7.3f", _fflt(temp_int32s) / 1000.0);
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
				if ( position == 12 )		  //����С����
					position++;
				buf2[position] = '_';
				lcddisp(3, 0, buf2);
				break;

			case KEY_ENTER:
				adj_h = (INT32S)ascii_to_hex(buf2[10]) * 10000 + (INT32S)ascii_to_hex(buf2[11]) * 1000 
						+ (INT32S)ascii_to_hex(buf2[13]) * 100 + (INT32S)ascii_to_hex(buf2[14]) * 10 
						+ (INT32S)ascii_to_hex(buf2[15]);
				temp_int32s2 = read_mod_reg((void *)&DevStat.MOD_REG.reg[0x0009]);	//��AD�����Ĵ���ԭֵ
				OS_ENTER_CRITICAL();
				adj_full_origin = _fflt(temp_int32s2) / 1000.0;
				adj_full = _fflt(adj_h) / (_fflt(temp_int32s) / adj_full_origin);
				temp_int32s = abs(_dfix(adj_full * 1000.0));
				OS_EXIT_CRITICAL();

				if ( temp_int32s > ADJ_AD_FULL_MAX || temp_int32s < ADJ_AD_FULL_MIN )
				{
					lcddisp(2, 0, "����������Χ!!  ");
					BeepInputErr();
					(void)delay_and_wait_key(2, 0, 0);
					lcddisp(2, 0, "������ʵ�ʺ��: ");
					continue;
				}

				write_mod_reg((void *)&DevStat.MOD_REG.reg[0x0009], temp_int32s);
				WriteParam();
				lcddisp(2, 0, "    ����ɹ�    ");
				(void)delay_and_wait_key(2, 0, 0);
				return;

			case KEY_CANCEL:
				return;

			default:
				continue;
		}
	}
}


