/******************************************************************************
Copyright 2005 �������ķ�������޹�˾
All rights riserved.

�ļ���	��task_general_control.c
ģ�����ƣ��ܿ����������
���ܸ�Ҫ���ܿ����������

ȡ���汾��0.0.1
�޸���	������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include 		"config.h"


// extern volatile CARD_INFO          CardInfo;				   //��Ƭ��Ϣ�����׼�¼�ɴ˱�������
// extern volatile ERROR_CARD_INFO    ErrCardInfo;	   //����״̬

//OS_STK          TaskStartStk[TASK_STK_TCPIP_SIZE * 4];
//OS_STK          TASK_GENCTRL_STK[TASK_STK_TCPIP_SIZE * 10];

//OS_STK          TaskStartStk[TASK_STK_TCPIP_SIZE];
//OS_STK          TASK_GENCTRL_STK[TASK_STK_TCPIP_SIZE];

extern volatile DEV_STAT DevStat;
//extern OS_EVENT        *Beep_Mbox;
//extern OS_EVENT        *Dis_Time_Dly_Mbox;
//extern OS_EVENT        *ICCard_Mbox;

extern volatile INT8U DisplayTimeEN;										//�Ƿ�����ˢ��ʱ�䣬=FALSE������=TRUE������ ����

//extern const  INT8U       ReadPOSNumber[5];                         //ȡPOS��
//extern const  INT8U       SELECT_SAM_ADF1[7];       //ѡ1001Ŀ¼

//TRANS_STK trans_stk;
//TRANS_STK* ptrans_stk = &trans_stk;
//CAL_STK cal_stk;
//CAL_STK* pcal_stk = &cal_stk;


extern struct       MENU*       curMenu;		//��ǰ�˵�
extern struct       MENU        mu[MENU_MAX_ITEM];



extern char * VERSION;			 //����ʱ��ʾ�İ汾��


/******************************************************************************
 �������ƣ�TaskGeneralCtrl
 �����������ܿ�������
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ������
 �� 	 �ڣ�2005-04-07
 �޸���ʷ��
		����		�޸���		�޸�����
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

	//��ʼ��
	InitMenu();
	Lcd_Cls();
	SetBacklightON();

	for ( ;; )
	{
//      ToggleWD();

		OSTimeDlyHMSM(0, 0, 0, 50);

//      if ( DevStat.err_occured == TRUE )
//      {   //������豸��������ʾ������룬������ˢ��
//          DisplayErr(DevStat.err_code);
//          continue;
//      }

		OS_ENTER_CRITICAL();
		mode = DevStat.mode;
		OS_EXIT_CRITICAL();
		
		if ( mode == AUTO )
		{
//          strcpy((void *)buf, "�Զ�   ");
			sprintf((void *)buf, "�Զ�%3d", DevStat.reset_times);
		}
		else
		{
//          strcpy((void *)buf, "�ֶ�   ");
			sprintf((void *)buf, "�ֶ�%3d", DevStat.reset_times);
		}
		strcat((void *)buf, VERSION);
		lcddisp(0, 0, buf);
      	lcddisp(1, 0, "  ��ѡ�����豸  ");
    	lcddisp(2, 0, "    ���Ƶ�Ԫ    ");
	    lcddisp(3, 0, "  �˵�--ȷ�ϼ�  ");

//        OS_ENTER_CRITICAL();
//        reg = DevStat.MOD_REG.reg[0x000D];
//        OS_EXIT_CRITICAL();
//        sprintf((void *)buf, "Ŀ����:%+7.3f", (FP64)reg / 1000.0);
//        lcddisp(1, 0, buf);
//
//        OS_ENTER_CRITICAL();
////    reg = DevStat.MOD_REG.reg[0x0003];
//        if ( (DevStat.MOD_REG.reg[0x0003] & 0x8000) == 0 )        //���Ƿ�Ϊ��
//        {
//            temp_int32s = (INT32S)DevStat.MOD_REG.reg[0x0003] * 1000L
//                          + (INT32S)DevStat.MOD_REG.reg[0x0004];
//        }
//        else
//        {
//            temp_int32s = 0 - ((INT32S)(DevStat.MOD_REG.reg[0x0003] & (~0x8000)) * 1000L)
//                          - (INT32S)DevStat.MOD_REG.reg[0x0004];
//        }
//        sprintf((void *)buf, "�������:%+7.3f", _fflt(temp_int32s) / 1000.0);
//        OS_EXIT_CRITICAL();
//        lcddisp(2, 0, buf);
//
////      OS_ENTER_CRITICAL();
////      reg = DevStat.MOD_REG.reg[0x0005];
////      OS_EXIT_CRITICAL();
////      sprintf((void *)buf, " AGC���:%+6.2fV", (FP64)reg / 1000.0);
////      lcddisp(3, 0, buf);
//
//        OS_ENTER_CRITICAL();
//        reg = DevStat.MOD_REG.reg[0x0005];
//        if ( (reg & 0x8000) == 0 )        //���Ƿ�Ϊ��
//        {
//            temp_int32s = reg;
//        }
//        else
//        {
//            temp_int32s = 0 - (reg & (~0x8000));
//        }
//        sprintf((void *)buf, "AGC ���:%+6.2fV", _fflt(temp_int32s) / 100.0);
//        OS_EXIT_CRITICAL();
//        lcddisp(3, 0, buf);


		key = delay_and_wait_key_ms(100, 0, 0);
		switch ( key )
		{
			case KEY_ENTER:					//�˵�
				SetupProcess();
				break;

			default:
				break;
		}

	}
}


///*****************************************************************
//����ԭ�ͣ�DisplayErr
//������������ʾ�������
//����������
//�������ƣ�  ����/����� ����        ����
//err_code    ����            INT16U      �������, �������, DevStat.err_code
//
//��  ��  ֵ����
//
//��      ��  ������
//��      �ڣ�2005-05-19
//�޸���ʷ��
//����        �޸���      �޸�����
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
//    lcddisp(1, 0, "    ϵͳ����!   ");
//
//    if ( (err_code & READ_CLOCK_ERROR) != 0 )               //��ȡʱ�Ӵ���
//    {
//        lcddisp(2, 0, "  ʱ��оƬ����  ");
//        OSMboxPost(Dis_Time_Dly_Mbox, (void *)&display_time_dly);                   //2��֮�ڲ�����ˢ��ʱ���������ʱ��ˢ��
//        SleepMs(1000);
//    }
//
//    if ( (err_code & CLOCK_FORMAT_ERROR) != 0 )             //ʱ�Ӹ�ʽ����
//    {
//        lcddisp(2, 0, "  ʱ�Ӹ�ʽ����  ");
//        OSMboxPost(Dis_Time_Dly_Mbox, (void *)&display_time_dly);                   //2��֮�ڲ�����ˢ��ʱ���������ʱ��ˢ��
//        SleepMs(1000);
//    }
//
//    if ( (err_code & RC500_ERROR) != 0 )                //���ö�дͷ����
//    {
//        lcddisp(2, 0, "  ��дͷ����    ");
//        OSMboxPost(Dis_Time_Dly_Mbox, (void *)&display_time_dly);                   //2��֮�ڲ�����ˢ��ʱ���������ʱ��ˢ��
//        SleepMs(1000);
//    }
//
//    if ( (err_code & FLASH_ERROR) != 0 )                //FLASH����
//    {
//        lcddisp(2, 0, "  �洢оƬ����  ");
//        OSMboxPost(Dis_Time_Dly_Mbox, (void *)&display_time_dly);                   //2��֮�ڲ�����ˢ��ʱ���������ʱ��ˢ��
//        SleepMs(1000);
//    }
//
//    if ( (err_code & PARAMETER_ERROR) != 0 )                //��������
//    {
//        lcddisp(2, 0, "  ��ȡ��������  ");
//        OSMboxPost(Dis_Time_Dly_Mbox, (void *)&display_time_dly);                   //2��֮�ڲ�����ˢ��ʱ���������ʱ��ˢ��
//        SleepMs(1000);
//    }
//
//    if ( (err_code & ST16C554_ERROR) != 0 )             //16C554����
//    {
//        lcddisp(2, 0, "  16C554����");
//        OSMboxPost(Dis_Time_Dly_Mbox, (void *)&display_time_dly);                   //2��֮�ڲ�����ˢ��ʱ���������ʱ��ˢ��
//        SleepMs(1000);
//    }
//
//    if ( (err_code & GPRS_ERROR) != 0 )             //GPRSģ�����
//    {
//        lcddisp(2, 0, "  GPRSģ�����");
//        OSMboxPost(Dis_Time_Dly_Mbox, (void *)&display_time_dly);                   //2��֮�ڲ�����ˢ��ʱ���������ʱ��ˢ��
//        SleepMs(1000);
//    }
//
//}

// /*****************************************************************
// ����ԭ�ͣ�calc
// �������������̲�������ʾ
// ����������
// �������ƣ�	����/�����	����		����
// price			���		INT32U*		������Ա����ļ۸񣬵�λ��
//
// ��  ��  ֵ��KEY_ENTER = �м۸�����
// 				KEY_CANCEL = �˳�
// 				KEY_MODE = ������ģʽ��
// 				KEY_FUNC = �����˹��ܼ�
//
// ��      ��	������
// ��      �ڣ�2005-10-21
// �޸���ʷ��
// ����		�޸���		�޸�����
// ------		---------	-------------
// *****************************************************************/
// INT8U calc(INT32U* price)
// {
// 	INT8U i;
// 	INT8U key;
// 	FP64 sum;										//�ܽ���λԪ
// 	char str[MAX_INPUT_LENGTH + 3];
// 	char translated[MAX_INPUT_LENGTH * 2];			//��str����ɵĺ�׺���ʽ
// //     char *pstr = str;
// //     char* init = "";
// 	INT8U len = 0;
// 	INT8U point_num = 0;					//С����ĸ��������ڷ�ֹ����12.34.56�����
// 	INT8U cancel_times = 0;					//��������cancel��
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
// 					{	//С�����ֻ��������λ��
// 						BeepInputErr();
// 						break;
// 					}
// 				}
//
// 				if ( len != 0 )
// 				{
// 					if ( len == 1 && str[len - 1] == '0' && key == '0' )
// 					{		//��ͷ��������0
// 						BeepInputErr();
// 						break;
// 					}
// 					if ( str[len - 1] == '0' && ( (str[len - 2] == '+') || (str[len - 2] == '-') ||(str[len - 2] == '*') ) && key == '0' )
// 					{	//���ź�������������0
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
// 					{	//�Ѿ�����һ��С������
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
// 				for ( i=0; i<len; i++ )					   //���������������Ƿ��������
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
// 					trans(str, translated);			//���������ʽ����ɺ�׺���ʽ
// 					sum = compvalue(translated);		//������
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
// 						LCD_Display_Str(2, 0, "���С��0!  �Ƿ�", NORMAL);
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
// 			case KEY_ENTER:		//����ͬ=��
// 				if ( len == 0 )
// 					break;
// 				if ( (str[len - 1] == '+') || (str[len - 1] == '-') || (str[len - 1] == '*') || (str[len - 1] == '.') )
// 				{
// 					BeepInputErr();
// 					break;
// 				}
//
// 				for ( i=0; i<len; i++ )					   //���������������Ƿ��������
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
// 					trans(str, translated);			//���������ʽ����ɺ�׺���ʽ
// 					sum = compvalue(translated);		//������
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
// 						break;			//0Ԫ���������ܣ���������
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
// 						LCD_Display_Str(2, 0, "���С��0!  �Ƿ�", NORMAL);
// 						BeepInputErr();
// 						OSTimeDlyHMSM(0, 0, 1, 0);
// 						return KEY_CANCEL;
// 					}
//
// 				}
// 				else
// 				{		//û�з��ţ����뼴���ѽ��
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
// 						break;			//0Ԫ���������ܣ���������
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
// 						LCD_Display_Str(2, 0, "���С��0!  �Ƿ�", NORMAL);
// 						BeepInputErr();
// 						OSTimeDlyHMSM(0, 0, 1, 0);
// 						return KEY_CANCEL;
// 					}
// 				}
// 				break;
//
// 			case KEY_CANCEL:
// 				if ( cancel_times == 0 )
// 				{	//��һ�ΰ�cancel��
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
// 				{	//��������cancel��
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
// 	if ( s->top >= MAX_INPUT_LENGTH )		  //����
// 		return;
// 	s->top++;
// 	s->stack[s->top] = ch;
// }
//
// void pop_trans_stk(TRANS_STK *s)
// {
// 	if ( s->top <= -1 )		  //����
// 		return;
// 	s->top--;
// }
//
// char gettop_trans_stk(TRANS_STK *s)
// {
// 	if ( s->top <= -1 )		  //ջ��
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
// ����ԭ�ͣ�trans
// �������������������ʽת��Ϊ��׺���ʽ
// ����������
// �������ƣ�	����/�����	����		����
// str				����		char*		��Ҫת�����������ʽ
// ret				���		char*		ת����ĺ�׺���ʽ
//
// ��  ��  ֵ����
// ��      ��	������
// ��      �ڣ�2005-10-21
// �޸���ʷ��
// ����		�޸���		�޸�����
// ------		---------	-------------
// *****************************************************************/
// void trans(char* str, char* ret)
// {
// 	INT8U p_str = 0;
// 	INT8U p_ret = 0;
// //     INT16S top = -1;            //��ջָ�룬ʹ�ö�ջʱ��+1��Ȼ��д������
// //     char stk[MAX_INPUT_LENGTH + 3];     //����ʹ�õĶ�ջ
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
// 				ret[p_ret++] = '#';			//��־��һ�����Ľ���
// 				while ( empty_trans_stk(ptrans_stk) == FALSE )
// 				{
// 					ret[p_ret++] = gettop_trans_stk(ptrans_stk);			//��ǰ�ķ��Ŷ���ջ
// 					pop_trans_stk(ptrans_stk);
// 				}
// 				push_trans_stk(ptrans_stk, str[p_str]);			//������ķ�����ջ
// 				p_str++;
// 				break;
//
// 			case '*':
// 				ret[p_ret++] = '#';			//��־��һ�����Ľ���
// 				while ( ((ch = gettop_trans_stk(ptrans_stk)) == '*') && (empty_trans_stk(ptrans_stk) == FALSE) )
// 				{
// 					ret[p_ret++] = ch;			//ջ���*��/����ջ
// 					pop_trans_stk(ptrans_stk);
// 				}
// 				push_trans_stk(ptrans_stk, str[p_str]);			//������ķ�����ջ
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
// 				//����δ֪�ַ�������
// 				p_str++;
// 				break;
// 		}
// 	}
// 	if ( isdigit(ret[p_ret - 1]) != 0 )
// 	{	//������Ϊ��β
// 		ret[p_ret++] = '#';
// 	}
//
// 	while ( empty_trans_stk(ptrans_stk) == FALSE )
// 	{
// 		ret[p_ret++] = gettop_trans_stk(ptrans_stk);			//��ǰ�ķ��Ŷ���ջ
// 		pop_trans_stk(ptrans_stk);
// 	}
//
// 	if ( isdigit(ret[p_ret - 1]) != 0 )
// 	{	//������Ϊ��β
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
// 	if ( s->top >= MAX_INPUT_LENGTH )		  //����
// 		return;
// 	s->top++;
// 	s->stack[s->top] = ch;
// }
//
// void pop_cal_stk(CAL_STK *s)
// {
// 	if ( s->top <= -1 )		  //����
// 		return;
// 	s->top--;
// }
//
// FP64 gettop_cal_stk(CAL_STK *s)
// {
// 	if ( s->top <= -1 )		  //ջ��
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
// ����ԭ�ͣ�compvalue
// ���������������׺���ʽ������
// ����������
// �������ƣ�	����/�����	����		����
// trans			����		char*		��׺���ʽ
//
// ��  ��  ֵ�������õ��Ľ��
// ��      ��	������
// ��      �ڣ�2005-10-21
// �޸���ʷ��
// ����		�޸���		�޸�����
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
// 						return 0.0;				//����ĺ�׺���ʽ������
// 				}
// 				else
// 					return 0.0;				//����ĺ�׺���ʽ������
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
// 						return 0.0;				//����ĺ�׺���ʽ������
// 				}
// 				else
// 					return 0.0;				//����ĺ�׺���ʽ������
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
// 						return 0.0;				//����ĺ�׺���ʽ������
// 				}
// 				else
// 					return 0.0;				//����ĺ�׺���ʽ������
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
// 				//����δ֪�ַ�������
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



