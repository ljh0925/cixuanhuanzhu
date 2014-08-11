/******************************************************************************
Copyright 2005 �������ķ�������޹�˾
All rights riserved.

�ļ���	��task_com0.c
ģ�����ƣ�COM0ͨѶ�������
���ܸ�Ҫ��COM0ͨѶ�������

ȡ���汾��0.0.1
�޸���	������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include 		"config.h"


extern volatile DEV_STAT DevStat;
extern INT8U Com0SndBuf[];				//���ڷ��ͻ�����
extern INT8U Com0RcvBuf[];				//���ڽ��ջ�����
extern  int  UART0SendBuf[];
INT16U Com0ModbusRcvlen = 0;					//����0���ճ���

/******************************************************************************
 �������ƣ�Task_Com0
 ����������COM0ͨѶ����
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ������
 �� 	 �ڣ�2005-04-11
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void Task_Com0(void * data)
{

	ToggleWD();

	data = data;

//  UART0_Init(9600);
	UART0_Init(19200);

	for ( ;; )		   //����
	{
		ToggleWD();

//      OSTimeDlyHMSM(0, 0, 0, 200);            //200msһ��
		OSTimeDlyHMSM(0, 0, 1, 0);			//400msһ��
//		sprintf((void *)Com0SndBuf, "%s", (void *)"ABCDEFG");
		strcpy((void *)Com0SndBuf, (void *)"ABCDEFG");
		com0_send_hex_daxian(Com0SndBuf, strlen((void *)Com0SndBuf));
	}
}
/******************************************************************************
 �������ƣ�com1_send_hex_daxian
 ������������������
 ����������
 �������ƣ� ����/����� ����		����
 str_to_send	����	INT8U *		Ҫ���͵��ַ���
 len				����	INT16U		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ������
 �� 	 �ڣ�2005-04-07
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void com0_send_hex_daxian(INT8U * str_to_send, INT16U len)
{
	INT16U i = 0;

	ToggleWD();

	for ( i=0; i<len; i++ )
	{
		ToggleWD();
		SleepMs(1);
		UART0Putch(str_to_send[i]);
	}

}

/************************************************************************************
		Mb_test_crc : check the crc of a packet
*************************************************************************************
input :
-------
trame  : packet with is crc
n      : lenght of the packet without tht crc
							  ^^^^^^^
answer :
--------
notok = crc fealure
ok = crc ok
************************************************************************************/
INT8U Mb_test_crc(INT8U trame[], INT16S n)
{
	INT16U crc,i,j,carry_flag,a;

	crc=0xffff;
	for ( i=0;i<n;i++ )
	{
		ToggleWD();

		crc=crc^trame[i];
		for ( j=0;j<8;j++ )
		{
			a=crc;
			carry_flag=a&0x0001;
			crc=crc>>1;
			if ( carry_flag==1 )
				crc=crc^0xa001;
		}
	}
	if ( (trame[n+1]!=(crc>>8)) || (trame[n]!=(crc&255)) )
		return notok;
	else
		return ok;
}

/************************************************************************************
		Mb_calcul_crc : compute the crc of a packet and put it at the end
*************************************************************************************
input :
-------
trame  : packet with is crc
n      : lenght of the packet without tht crc
							  ^^^^^^^
answer :
--------
crc
************************************************************************************/
INT16U Mb_calcul_crc(INT8U trame[],INT16U n)
{
	INT16U crc,i,j,carry_flag,a;

	crc=0xffff;
	for ( i=0;i<n;i++ )
	{
		ToggleWD();

		crc=crc^trame[i];
		for ( j=0;j<8;j++ )
		{
			a=crc;
			carry_flag=a&0x0001;
			crc=crc>>1;
			if ( carry_flag==1 )
				crc=crc^0xa001;
		}
	}
	trame[n+1]=crc>>8;
	trame[n]=crc&255;
	return crc;
}

/******************************************************************************
 �������ƣ�CmdProcess
 ����������ͨѶ��������ʹ���
 ����������
 �������ƣ� ����/����� ����        ����

 ��  ��  ֵ��ok-�ɹ�
			notok(0xFF)-ʧ��
 ��      �� ������
 ��      �ڣ�2004-09-02
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
INT8U CmdProcess(void)
{
//  INT8U i = 0;
	INT16S j = 0;
//  INT16U k = 0;
//  INT8U m = 0;
	INT16U start_addr = 0;
	INT16U reg_num = 0;
//  INT16U cmd_value = 0;
//  INT8U *pstart_addr;
	INT16U *pstart_addr;
//     INT16U data_space   reg_num;            //�Ĵ�����ַ
//     INT16U data_space   reg_length;         //�Ĵ�������
//     INT16U data_space data_len = 0;                                     //�����򳤶�
	INT8U mode = AUTO;

	ToggleWD();

	switch ( Com0RcvBuf[1] )
	{
//      ////////////////////////////////////////
//      //��ȡ�̵���״̬
//      ////////////////////////////////////////
//      case 0x01:              //��ȡ�̵���״̬
//      case 0x02:              //��ȡ����״̬
//          if ( Com0ModbusRcvlen != 8 )
//              return notok;      //���Ȳ���
//          start_addr = ((INT16U)Com0RcvBuf[2] << 8 ) | (INT16U)Com0RcvBuf[3];
//          if ( start_addr >= 0x40 )
//              return notok;           //��ַ����Χ
//          reg_num = ((INT16U)Com0RcvBuf[4] << 8 ) | (INT16U)Com0RcvBuf[5];
//          if ( reg_num == 0 )
//              return notok;           //��ʽ�Ƿ�
//          if ( start_addr + reg_num > 0x40 )
//              return notok;           //��ַ����Χ
//
//          pstart_addr =  (INT16U *)DevStat.MOD_REG.reg + start_addr;
//
//          Com0SndBuf[0] = DevStat.id;
//          Com0SndBuf[1] = Com0RcvBuf[1];
//          if ( reg_num % 8 == 0 )
//          {
//              Com0SndBuf[2] = (INT8U)(reg_num / 8);
//          }
//          else
//          {
//              Com0SndBuf[2] = (INT8U)((reg_num / 8) + 1);
//          }
//          OS_ENTER_CRITICAL();
//          for ( j=0; j<Com0SndBuf[2]-1; j++ )
//          {
//              Com0SndBuf[3 + j] = 0;
//              for ( i=0; i<8; i++ )
//              {
//                  if ( pstart_addr[j * 8 + i] == 1 )
//                      Com0SndBuf[3 + j] |= (1 << i);
//              }
//
//          }
//          OS_EXIT_CRITICAL();
//
//          //���һ������
//          Com0SndBuf[3 + j] = 0;
//          if ( reg_num % 8 == 0 )
//              m = 8;
//          else
//              m = reg_num % 8;
//
//          OS_ENTER_CRITICAL();
//          for ( i=0; i<m; i++ )
//          {
//              if ( pstart_addr[j * 8 + i] == 1 )
//                  Com0SndBuf[3 + j] |= (1 << i);
//          }
//          OS_EXIT_CRITICAL();
//
//          com0_send_hex_modbus((void *)Com0SndBuf, Com0SndBuf[2] + 3);
//          break;

		////////////////////////////////////////
		//��ȡ�Ĵ���
		////////////////////////////////////////
		case 0x03:			//��ȡ���ּĴ���
		case 0x04:			//��ȡ����Ĵ���
			if ( Com0ModbusRcvlen != 8 )
				return notok;	   //���Ȳ���
			start_addr = ((INT16U)Com0RcvBuf[2] << 8 ) | (INT16U)Com0RcvBuf[3];
//          if ( start_addr >= 0x90 )
			if ( start_addr >= 16 + HOU_DU_BIAO_LEN )
				return notok;			//��ַ����Χ
			reg_num = ((INT16U)Com0RcvBuf[4] << 8 ) | (INT16U)Com0RcvBuf[5];
			if ( reg_num == 0 )
				return notok;			//��ʽ�Ƿ�
//          if ( start_addr + reg_num > 0x90 )
			if ( start_addr + reg_num > 16 + HOU_DU_BIAO_LEN )
				return notok;			//��ַ����Χ

			pstart_addr =  (INT16U *)DevStat.MOD_REG.reg + start_addr;

			Com0SndBuf[0] = DevStat.id;
			Com0SndBuf[1] = Com0RcvBuf[1];
			Com0SndBuf[2] = (INT8U)(reg_num * 2); //���س���
			OS_ENTER_CRITICAL();
			for ( j=0; j<reg_num; j++ )
			{
				Com0SndBuf[3 + j * 2] = (INT8U)(pstart_addr[j] / 256);
				Com0SndBuf[3 + j * 2 + 1] = (INT8U)(pstart_addr[j] % 256);
			}
			OS_EXIT_CRITICAL();

			com0_send_hex_modbus((void *)Com0SndBuf, Com0SndBuf[2] + 3);
			break;

//      ////////////////////////////////////////
//      //ǿ�õ��̵���
//      ////////////////////////////////////////
//      case 0x05:
//          OS_ENTER_CRITICAL();
//          mode = DevStat.mode;
//          OS_EXIT_CRITICAL();
//          if (mode == MANUAL)
//              return notok;
//
//          if ( Com0ModbusRcvlen != 8 )
//              return notok;      //���Ȳ���
//          start_addr = ((INT16U)Com0RcvBuf[2] << 8 ) | (INT16U)Com0RcvBuf[3];
//          if ( start_addr >= 0x20 )
//              return notok;          //��ַ����Χ
//
//          cmd_value = ((INT16U)Com0RcvBuf[4] << 8 ) | (INT16U)Com0RcvBuf[5];
//          if ( cmd_value == 0xFF00 )   //ǿ������
//          {
//              OS_ENTER_CRITICAL();
//              DevStat.MOD_REG.REG_DETAIL.mod_coil[start_addr] = 1;
//              OS_EXIT_CRITICAL();
//              SetRelayModbus();
//          }
//          else if ( cmd_value == 0x0000 )   //ǿ�ƶϿ�
//          {
//              OS_ENTER_CRITICAL();
//              DevStat.MOD_REG.REG_DETAIL.mod_coil[start_addr] = 0;
//              OS_EXIT_CRITICAL();
//              SetRelayModbus();
//          }
//          else
//              return notok;      //��ʽ�Ƿ�
//
//          com0_send_hex(Com0RcvBuf, Com0ModbusRcvlen);                  //��ʲô��ʲô
//          break;

		////////////////////////////////////////
		//Ԥ�õ��Ĵ���
		////////////////////////////////////////
		case 0x06:
			OS_ENTER_CRITICAL();
			mode = DevStat.mode;
			OS_EXIT_CRITICAL();
			if (mode == MANUAL)
				return notok;

			if ( Com0ModbusRcvlen != 8 )
				return notok;	   //���Ȳ���
			start_addr = ((INT16U)Com0RcvBuf[2] << 8 ) | (INT16U)Com0RcvBuf[3];
//          if ( start_addr >= 0x90 || start_addr < 0x08 )
			if ( start_addr >= 16 + HOU_DU_BIAO_LEN || start_addr < 0x08 )
				return notok;			//��ַ����Χ

			OS_ENTER_CRITICAL();
			DevStat.MOD_REG.reg[start_addr] = ((INT16U)Com0RcvBuf[4] << 8 ) | (INT16U)Com0RcvBuf[5];
			OS_EXIT_CRITICAL();

			WriteParam();
			com0_send_hex(Com0RcvBuf, Com0ModbusRcvlen);				  //��ʲô��ʲô
			break;

		////////////////////////////////////////
		//��ȡ�쳣״̬
		////////////////////////////////////////
		case 0x07:
			if ( Com0ModbusRcvlen != 4 )
				return notok;	   //���Ȳ���

			Com0SndBuf[0] = DevStat.id;
			Com0SndBuf[1] = Com0RcvBuf[1];
			Com0SndBuf[2] = (INT8U)DevStat.MOD_REG.reg[0x0001]; //�쳣״̬

			com0_send_hex_modbus((void *)Com0SndBuf, 3);
			break;

		////////////////////////////////////////
		//�������У��
		////////////////////////////////////////
		case 0x08:
			com0_send_hex(Com0RcvBuf, Com0ModbusRcvlen);				  //��ʲô��ʲô
			break;

//      ////////////////////////////////////////
//      //ǿ�ö�̵���
//      ////////////////////////////////////////
//      case 15:
//          OS_ENTER_CRITICAL();
//          mode = DevStat.mode;
//          OS_EXIT_CRITICAL();
//          if (mode == MANUAL)
//              return notok;
//
//          if ( Com0ModbusRcvlen < 10 || Com0ModbusRcvlen != Com0RcvBuf[6] + 9 )
//              return notok;      //���Ȳ���
//          start_addr = ((INT16U)Com0RcvBuf[2] << 8 ) | (INT16U)Com0RcvBuf[3];
//          if ( start_addr >= 0x20 )
//              return notok;           //��ַ����Χ
//          reg_num = ((INT16U)Com0RcvBuf[4] << 8 ) | (INT16U)Com0RcvBuf[5];
//          if ( reg_num == 0 )
//              return notok;           //��ʽ�Ƿ�
//          if ( start_addr + reg_num > 0x20 )
//              return notok;           //��ַ����Χ
//
//          pstart_addr =  (INT16U *)DevStat.MOD_REG.reg + start_addr;
//
//          OS_ENTER_CRITICAL();
//          for(j=0; j<reg_num / 8; j++)
//          {
//              for(i=0; i<8; i++)
//              {
//                  if ((Com0RcvBuf[7 + j] >> i) & 0x0001 == 0x0001)
//                  {
//                      pstart_addr[j * 8 + i] = 0x0001;
//                  }
//                  else
//                  {
//                      pstart_addr[j * 8 + i] = 0x0000;
//                  }
//              }
//          }
//
//          //���һ��
//          for(i=0; i<reg_num % 8; i++)
//          {
//              if ((Com0RcvBuf[7 + j] >> i) & 0x0001 == 0x0001)
//              {
//                  pstart_addr[j * 8 + i] = 0x0001;
//              }
//              else
//              {
//                  pstart_addr[j * 8 + i] = 0x0000;
//              }
//          }
//          OS_EXIT_CRITICAL();
//
//          SetRelayModbus();
//
//          memcpy((void *)Com0SndBuf, (void *)Com0RcvBuf, 6);
//          com0_send_hex_modbus((void *)Com0SndBuf, 6);
//          break;

		////////////////////////////////////////
		//Ԥ�ö�Ĵ���
		////////////////////////////////////////
		case 16:
			OS_ENTER_CRITICAL();
			mode = DevStat.mode;
			OS_EXIT_CRITICAL();
			if (mode == MANUAL)
				return notok;

//          if ( Com0ModbusRcvlen < 11 || Com0ModbusRcvlen != Com0RcvBuf[6] + 9 )
//              return notok;      //���Ȳ���
			if ( Com0ModbusRcvlen < 11)
				return notok;	   //���Ȳ���
			if ( Com0ModbusRcvlen != Com0RcvBuf[6] + 9 )
				return notok;	   //���Ȳ���

			start_addr = ((INT16U)Com0RcvBuf[2] << 8 ) | (INT16U)Com0RcvBuf[3];
//          if ( start_addr >= 0x90 || start_addr < 0x08 )
			if ( start_addr >= 16 + HOU_DU_BIAO_LEN || start_addr < 0x08 )
				return notok;			//��ַ����Χ
			reg_num = ((INT16U)Com0RcvBuf[4] << 8 ) | (INT16U)Com0RcvBuf[5];
			if ( reg_num == 0 )
				return notok;			//��ʽ�Ƿ�
//          if ( start_addr + reg_num > 0x90 )
			if ( start_addr + reg_num > 16 + HOU_DU_BIAO_LEN )
				return notok;			//��ַ����Χ

			pstart_addr =  (INT16U *)DevStat.MOD_REG.reg + start_addr;

			OS_ENTER_CRITICAL();
			for(j=0; j<reg_num; j++)
			{
				DevStat.MOD_REG.reg[start_addr + j] = 
					((INT16U)Com0RcvBuf[j * 2 + 7] << 8 ) | (INT16U)Com0RcvBuf[j * 2 + 8];
			}
			OS_EXIT_CRITICAL();

			WriteParam();

			memcpy((void *)Com0SndBuf, (void *)Com0RcvBuf, 6);
			com0_send_hex_modbus((void *)Com0SndBuf, 6);
			break;

		default:	//do nothing
			return notok;
	}

	return ok;
}

/******************************************************************************
 �������ƣ�com0_send_hex_modbus
 ������������װЭ���β,���ͳ�ȥ
 ����������
 �������ƣ� ����/����� ����        ����
 str_to_send    ����    INT8U *     Ҫ���͵��ַ���
 len                ����        INT16U      ���ͳ���

 ��  ��  ֵ����

 ��      �� ������
 ��      �ڣ�2005-04-07
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
void com0_send_hex_modbus(INT8U * str_to_send, INT16U len)
{
	ToggleWD();

	(void)Mb_calcul_crc(str_to_send, len);
	com0_send_hex(str_to_send, len + 2);
}

