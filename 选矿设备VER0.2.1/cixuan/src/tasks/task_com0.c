/******************************************************************************
Copyright 2005 ����Ӣ��Ƽ���չ���޹�˾
All rights riserved.

�ļ���	��task_com0.c
ģ�����ƣ�COM0ͨѶ�������
���ܸ�Ҫ��COM0ͨѶ�������

ȡ���汾��0.0.1
�޸���	��������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include 		"config.h"


extern volatile DEV_STAT DevStat;
extern INT8U Com0SndBuf[];				//���ڷ��ͻ�����
extern INT8U Com0RcvBuf[];				//���ڽ��ջ�����

INT16U Com0ModbusRcvlen = 0;			//����0���ճ���

union f_c
{
    float  l;
    unsigned char c[4];
}f;

/******************************************************************************
 �������ƣ�Task_Com0
 ����������COM1ͨѶ����
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2005-04-11
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void Task_Com0(void * data)
{
	INT8U i = 0;
	char temp[10];
	INT32S time_last;
	INT32S time_cur;
	BUS_TIME ltime;
	BUS_TIME last_time;

	data = data;

	ToggleWD();

	memset(temp, 0, sizeof(temp));

	UART0_Init(9600);

com1_start:

	for( ;; )										//���ӣ���ȡƤ���ӵĹ���״̬
	{
		ToggleWD();

		OSTimeDlyHMSM(0, 0, 0, 200);                //200msһ��

		CleanCom0Buf();
		memcpy((void *)Com0SndBuf, "\x30\x33\x30\x30\x30\x38\x30\x30\x30\x31", 10);    //��ȡƤ���ӣ�����״̬	
		i = com0_snd_and_rcv(Com0SndBuf, 10, Com0RcvBuf, &Com0ModbusRcvlen);
		if (i != ok)
		{
			OS_ENTER_CRITICAL();
			DevStat.connect_scale |= BIT1;		    //����ʧ��
			OS_EXIT_CRITICAL();
			continue;
		}
		else
		{
			if(memcmp(&Com0RcvBuf[7], (void *)"\x30\x30\x46\x46", 4) == 0)
			{
				OS_ENTER_CRITICAL();
				DevStat.connect_scale &= (~BIT1);	//��������,�����ڹ���״̬
				OS_EXIT_CRITICAL();
				break;
			}
			continue;
		}
	}


	for ( ;; )    
	{
		ToggleWD();

		OSTimeDlyHMSM(0, 0, 0, 200);               //200msһ��
		
		CleanCom0Buf();
		memcpy((void *)Com0SndBuf, "\x30\x33\x30\x30\x30\x30\x30\x30\x30\x38", 10);  //���ۼ������ٶȡ������͸�����������
		i = com0_snd_and_rcv(Com0SndBuf, 10, Com0RcvBuf, &Com0ModbusRcvlen);
		if (i != ok)
		{
			OS_ENTER_CRITICAL();
			DevStat.connect_scale |= BIT1;       //����ʧ��
			OS_EXIT_CRITICAL();
			goto com1_start;
		}
		if ((Com0RcvBuf[5] == 0x31) && (Com0RcvBuf[6] == 0x30))	//�ֽ�������16��
		{
			OS_ENTER_CRITICAL();
			DevStat.connect_scale &= (~BIT1);	//��������,�����ڹ���״̬
			OS_EXIT_CRITICAL();
		}
		else
		{
			goto com1_start;
		}

		OS_ENTER_CRITICAL();

		memcpy(temp, (char *)&Com0RcvBuf[7], 8);
		DevStat.MOD_REG.REG_DETAIL.total_weight = atol((void *)temp);    //�ۻ���

		memcpy(temp, (char *)&Com0RcvBuf[15], 8);   //�ٶ�
		proess_data(temp, 8);
		f.c[0]=(temp[6]<<4)+(temp[7]);
		f.c[1]=(temp[4]<<4)+(temp[5]);
		f.c[2]=(temp[2]<<4)+(temp[3]);
		f.c[3]=(temp[0]<<4)+(temp[1]);
		DevStat.MOD_REG.REG_DETAIL.speed = f.l;

		memcpy(temp, (char *)&Com0RcvBuf[23], 8);
		proess_data(temp, 8);
		f.c[0]=(temp[6]<<4)+(temp[7]);
		f.c[1]=(temp[4]<<4)+(temp[5]);
		f.c[2]=(temp[2]<<4)+(temp[3]);
		f.c[3]=(temp[0]<<4)+(temp[1]);
		DevStat.MOD_REG.REG_DETAIL.flow = f.l;    	  //����

		memcpy(temp, (char *)&Com0RcvBuf[31], 8);
		proess_data(temp, 8);
		f.c[0]=(temp[6]<<4)+(temp[7]);
		f.c[1]=(temp[4]<<4)+(temp[5]);
		f.c[2]=(temp[2]<<4)+(temp[3]);
		f.c[3]=(temp[0]<<4)+(temp[1]);
		DevStat.MOD_REG.REG_DETAIL.set_flow = f.l;    //��������
//  	sprintf((void *)temp,"%7.3f",f.l);
		OS_EXIT_CRITICAL();

	
		ToggleWD();

		OSTimeDlyHMSM(0, 0, 0, 200);               //200msһ��

//  	CleanCom0Buf();
//  	memcpy((void *)Com0SndBuf, "\x30\x33\x30\x30\x30\x39\x30\x30\x30\x32", 10);  //�������ۻ�������
//  	i = com0_snd_and_rcv(Com0SndBuf, 10, Com0RcvBuf, &Com0ModbusRcvlen);
//  	if (i != ok)
//  	{
//  		OS_ENTER_CRITICAL();
//  		DevStat.connect_scale |= BIT1;       //����ʧ��
//  		OS_EXIT_CRITICAL();
//  		goto com1_start;
//  	}
//  	OS_ENTER_CRITICAL();
//  	memcpy(temp, (char *)&Com0RcvBuf[7], 2);
//  	DevStat.MOD_REG.REG_DETAIL.class_weight = atol((void *)temp);    //�����ۻ���
//  	OS_EXIT_CRITICAL();

		CleanCom0Buf();
		memcpy((void *)Com0SndBuf, "\x30\x33\x30\x30\x31\x31\x30\x30\x30\x32", 10);  //�������ۻ�������
		i = com0_snd_and_rcv(Com0SndBuf, 10, Com0RcvBuf, &Com0ModbusRcvlen);
		if (i != ok)
		{
			OS_ENTER_CRITICAL();
			DevStat.connect_scale |= BIT1;       //����ʧ��
			OS_EXIT_CRITICAL();
			goto com1_start;
		}
		OS_ENTER_CRITICAL();
		memcpy(temp, (char *)&Com0RcvBuf[7], 2);
		DevStat.MOD_REG.REG_DETAIL.today_weight = atol((void *)temp);    //�����ۻ���
		OS_EXIT_CRITICAL();

		CleanCom0Buf();
		memcpy((void *)Com0SndBuf, "\x30\x33\x30\x30\x31\x33\x30\x30\x30\x32", 10);  //�������ۻ�������
		i = com0_snd_and_rcv(Com0SndBuf, 10, Com0RcvBuf, &Com0ModbusRcvlen);
		if (i != ok)
		{
			OS_ENTER_CRITICAL();
			DevStat.connect_scale |= BIT1;       //����ʧ��
			OS_EXIT_CRITICAL();
			goto com1_start;
		}
		OS_ENTER_CRITICAL();
		memcpy(temp, (char *)&Com0RcvBuf[7], 2);
		DevStat.MOD_REG.REG_DETAIL.month_weight = atol((void *)temp);    //�����ۻ���
		OS_EXIT_CRITICAL();

		time_cur = cal_sec(&ltime);
		time_last = cal_sec(&last_time);
		if ( (time_cur - time_last >= (INT32S)(24 * 60 * 60)) || (time_last - time_cur >= (INT32S)(24 * 60 * 60)) )   //����1��
		{
			//���¼
			StoreRecord();
		}
		else
			continue;
	}
}

/******************************************************************************
 �������ƣ�StoreRecord
 �������������׼�¼�洢����
 �������ƣ�	����/�����	����		����
	info	����		����������������Ϣ
				
 ��  ��  ֵ����
 
 ��      ��	������
 ��      �ڣ�2004-09-23
 �޸���ʷ��
		����		�޸���		�޸�����
		2005.2.18		myron	                     in function 
******************************************************************************/
INT8U StoreRecord()
{
	INT8U  i = 0;
	INT8U record[REC_LEN + 2];
	int  temp_int32u =0;

	ToggleWD();

	memset(record, 0x00, sizeof(record));

	//////////////////////////////////////////
	//��֯���׼�¼��ʽ����ͬ����
	//////////////////////////////////////////

	temp_int32u = ReadParamRecNum();
	if(temp_int32u >= 2000)
	{
		lcddisp(0, 0, "�뾡�콫��¼�ϴ�");
		temp_int32u = 2000;
	}RECORD

	record[0] = DevStat.kind_of_iron;			//��ʯ����	1�ֽ�
	record[1] = DevStat.ratio_of_iron;			//���ۺ����� 	1�ֽ�	

	weight_of_day = DevStat.MOD_REG.REG_DETAIL.

	memcpy(&record[19], (void *)&cardinfo->card_in_time, 7);			//��������4�ֽں�ʱ�� 3�ֽ� 
	memcpy(&record[26], (void *)&cardinfo->purchase_serial_num[0], 2);	//�û����ѻ�������� 2�ֽ�	

	memcpy(&record[32], (void *)&DevStat.line_number[0], 2);			//2�ֽ���·��(HEX)
	memcpy(&record[34], (void *)&DevStat.bus_number[0], 3); 			//3�ֽڳ���(BCD)
	memcpy(&record[37], (void *)&DevStat.cur_driver_number[2], 4);		//4�ֽڼ�ʻԱ��(BCD)
	memcpy(&record[41], (void *)&DevStat.pos_number[0], 6); 			//6�ֽ�PSAM����(HEX)

	//վ����
    memcpy((INT8U *)&record[0], (INT8U *)&DevStat.line_number[0], 2);				

	//�豸���
    memcpy((INT8U *)&DevStat.MOD_REG.REG_DETAIL.bus_number[0], (INT8U *)&DevStat.bus_number[0], 3);				

	//���� ��������4�ֽں�ʱ��3�ֽ�
	if(DevStat.alermed == 1)
	{
		memcpy((BUS_TIME *)&DevStat.MOD_REG.REG_DETAIL.alerm_on_time, (BUS_TIME *)&DevStat.alerm_time, 7);
	}
	else
	{
		memset((BUS_TIME *)&DevStat.MOD_REG.REG_DETAIL.alerm_on_time, 0x00, 7);
		DevStat.alermed = 0; 
	}

	

//  //�����������
//  memcpy((INT8U *)&DevStat.MOD_REG.REG_DETAIL.today_weight[0], &DevStat.MOD_REG.REG_DETAIL.today_weight[0], 4);
//  //���¿�������
//  memcpy((INT8U *)&DevStat.MOD_REG.REG_DETAIL.month_weight[0], &DevStat.MOD_REG.REG_DETAIL.month_weight[0], 4);
//
//  memcpy((uchar *)&prec->price, (uchar *)&DevStat.price[DevStat.fill_mode], 4); //����
//  var_bcd2asc(buf+buf_len, (uchar *)&prec->price, 4);
//  sprintf((void *)(buf+buf_len), "%08d", prec->price);

	WriteParam();
}
/******************************************************************************
 �������ƣ�cal_sec
 ���������������2000��1��1�յ�����ʱ�������
 ����������
 �������ƣ�	����/�����	����		����
 time			����			BUS_TIME*	Ҫ���м����ʱ�䣬Ҫ������ʱ�����2000��
 
 ��  ��  ֵ������
   
 ��      ��	������
 ��      �ڣ�2006.1.9
 �޸���ʷ��
		����		�޸���		�޸�����

******************************************************************************/
INT32U cal_sec(_TIME * time)
{
	INT8U i;
	INT16U year;
	INT32U sec;

	const INT32U days[] = 
	{
		0,
		0,
		0 + 31,
		0 + 31 + 28,
		0 + 31 + 28 + 31,
		0 + 31 + 28 + 31 + 30,
		0 + 31 + 28 + 31 + 30 + 31,
		0 + 31 + 28 + 31 + 30 + 31 + 30,
		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31,
		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31,
		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,
	};

	const INT32U days_leap[] = 
	{
		0,
		0,
		0 + 31,
		0 + 31 + 29,
		0 + 31 + 29 + 31,
		0 + 31 + 29 + 31 + 30,
		0 + 31 + 29 + 31 + 30 + 31,
		0 + 31 + 29 + 31 + 30 + 31 + 30,
		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31,
		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31,
		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,
	};

	if ( time->century != 0x20 )		  //����ʱ�����Ϊ2000���Ժ�
		return 0;

	//�������ǰ�������
	i = Is_Leap((INT16U)bcdhex(time->century) * 100 + (INT16U)bcdhex(time->year));
	if ( i == TRUE )
	{	//������
		sec = ( (days_leap[bcdhex(time->month)] + (INT32U)bcdhex(time->day)) * 24 * 60 * 60 
				+ (INT32U)bcdhex(time->hour) * 60 * 60 + (INT32U)bcdhex(time->minute) * 60 + (INT32U)bcdhex(time->second) );
	}
	else
	{
		sec = ( (days[bcdhex(time->month)] + (INT32U)bcdhex(time->day)) * 24 * 60 * 60 
				+ (INT32U)bcdhex(time->hour) * 60 * 60 + (INT32U)bcdhex(time->minute) * 60 + (INT32U)bcdhex(time->second) );
	}

	//�������ǰһ�굽2000��֮�������
	for ( year = 2000; year < ((INT16U)bcdhex(time->century) * 100 + (INT16U)bcdhex(time->year)); year++ )
	{
		if ( Is_Leap(year) == TRUE )
			sec += 366 * 24 * 60 * 60;
		else
			sec	+= 365 * 24 * 60 * 60;
	}

	return sec;
}

/*****************************************************************
����ԭ�ͣ�Is_Leap
���������� �����Ƿ�Ϊ����
����������
�������ƣ�	����/����� ����		����
			year	����		uint 		��Ҫ�ж��Ƿ�Ϊ��������
			
��	��	 ֵ��	0-��������
				1-������
				
��		 �� ������
��		 �ڣ�2003-11-27
�޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
*****************************************************************/
INT8U Is_Leap(INT16U year)
{
	return(year % 4 == 0) && ( (year % 100 != 0) || (year % 400 == 0) );
}

/******************************************************************************
 �������ƣ�proess_data
 ����������modbusͨ�����ݴ���
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2014-04-11
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void proess_data(char dest[], int lenth)
{
	int i=0;

	for(i=0; i<lenth; i++)
	{
		if(dest[i]>'9')
			dest[i]-='7';
		else
			dest[i]-='0';
	}

}

/******************************************************************************
 �������ƣ�com0_snd_and_rcv
 �������������Ͳ�����
 ����������
 �������ƣ� ����/����� ����        ����
 str_to_send    ����    INT8U *     Ҫ���͵��ַ���
 len			����	INT16U      ���ͳ���
 str_rcvd		���	INT8U *		���յ����ַ���
 rcv_len		���	INT16U *	���ճ���

 ��  ��  ֵ��ok-���ճɹ�
			notok-����ʧ��

 ��      �� ��������
 ��      �ڣ�2005-04-07
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
INT8U com0_snd_and_rcv(INT8U * str_to_send, INT16U len, INT8U *str_rcvd, INT16U* rcv_len)
{
	INT8U i = 0;
	INT8U j = 0;
	//INT8U buf[5];

	ToggleWD();

	memmove(&str_to_send[3], str_to_send, len);         //����3�ֽ�

	str_to_send[0] = 0x3A;
	str_to_send[1] = DevStat.Modbus_client[0];
	str_to_send[2] = DevStat.Modbus_client[1];

	for ( i=0; i<COM1_RETRY_TIMES; i++ )
	{
		CleanCom0Buf();

		com0_send_hex_modbus((void *)str_to_send, len);
		j = com0_gets((void *)str_rcvd, rcv_len, 200, 20);          //�ȴ�����һ������
		if ( j == ok )
		{
			if ( Mb_test_lrc(str_rcvd, *rcv_len - 4) == ok )        // У��LRC
			{
				break;
			}
			continue;
		}
		else if ( j == KEY_CANCEL )
		{
			//@@@
		}
	}
	if ( i >= COM1_RETRY_TIMES )
	{
		return notok;
	}
	return ok;
}

/******************************************************************************
 �������ƣ�xray_ad_to_v
 ����������X����Դ��������ADֵ��ת��Ϊ��ѹֵ(350��ʾ35kV)
 ����������
 �������ƣ� ����/����� ����        ����
 ad			����		INT16U      X����Դ��������ADֵ

 ��  ��  ֵ����ѹֵ(350��ʾ35kV)

 ��      �� ��������
 ��      �ڣ�2005-04-07
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
INT16U xray_ad_to_v(INT16U ad)
{
	ToggleWD();

//   v     ad
//  --- = ---
//   80   4095
//ԭ���ĸ�ѹԴ�ĵ�ѹ�͵����Ķ�Ӧ��ϵ��
//
//����ֵ��
//    5V = 80KV������ѹ��
//    5V = 250VA����������
//���ֵ��
//    10V = 80KV��
//    10V = 250UA��
//
//���ڵĸ�ѹԴ�ĵ�ѹ�͵����Ķ�Ӧ��ϵ��
//
//����ֵ��
//    10V = 80KV������ѹ��
//    10V = 1500VA����������
//���ֵ��
//    1.6V = 80KV��
//    3V = 1500UA��

//   v     ad
//  --- = ---
//   80   1311

	return (INT16U)(((FP64)ad * 800.0) / 1311.0);	//X���ߵ�ѹʵ��ֵ
}

/******************************************************************************
 �������ƣ�xray_v_to_ad
 ������������ѹֵ(350��ʾ35kV)��ת��ΪX����Դ��������ADֵ
 ����������
 �������ƣ� ����/����� ����        ����
 v			����		INT16U      ��ѹֵ(350��ʾ35kV)

 ��  ��  ֵ��X����Դ��������ADֵ

 ��      �� ��������
 ��      �ڣ�2005-04-07
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
INT16U xray_v_to_ad(INT16U v)
{
	ToggleWD();

	return (INT16U)(((FP64)v * 4095.0) / 800.0);
}

/******************************************************************************
 �������ƣ�xray_ad_to_i
 ����������X����Դ��������ADֵ��ת��Ϊ����ֵ(100��ʾ10uA)
 ����������
 �������ƣ� ����/����� ����        ����
 ad			����		INT16U      X����Դ��������ADֵ

 ��  ��  ֵ����ѹֵ(350��ʾ35kV)

 ��      �� ��������
 ��      �ڣ�2005-04-07
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
INT16U xray_ad_to_i(INT16U ad)
{
	ToggleWD();

//   i     ad
//  --- = ---
//  250   4095
//   i     ad
//  --- = ---
//  1500   2458

	return (INT16U)(((FP64)ad * 15000.0) / 2458.0);	//X���ߵ���ʵ��ֵ
}

/******************************************************************************
 �������ƣ�xray_i_to_ad
 ��������������ֵ(100��ʾ10uA)��ת��ΪX����Դ��������ADֵ
 ����������
 �������ƣ� ����/����� ����        ����
 i			����		INT16U      ����ֵ(100��ʾ10uA)

 ��  ��  ֵ��X����Դ��������ADֵ

 ��      �� ��������
 ��      �ڣ�2005-04-07
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
INT16U xray_i_to_ad(INT16U i)
{
	ToggleWD();

//  return (INT16U)(((FP64)i * 4095.0) / 2500.0);
	return (INT16U)(((FP64)i * 4095.0) / 15000.0);
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
INT8U Mb_test_lrc(INT8U trame[], INT16S n)
{
	INT16U i;
	char Data_tmp, ucLRC, buf[2];
	
	ToggleWD();

	for(i=1; i<n; i++)
	{ 
		ToggleWD();
		ucLRC += trame[i]; 	/* add buffer byte without carry */		
	} 
	Data_tmp = (~ucLRC);
	Data_tmp ++;
	htoa(buf, (char *)&Data_tmp, 1);

	if ( (trame[n] != buf[0]) || (trame[n + 1] != buf[1]) )
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
INT8U Mb_calcul_lrc(INT8U trame[],INT16U n)
{
	int i;
	char ucLRC = 0, tmp ;	/* LRC char initialized */
	ToggleWD();

	for(i=1; i<n; i++)
	{ 
		ToggleWD();
		ucLRC += trame[i]; 	/* add buffer byte without carry */		
	} 
	tmp = (~ucLRC);
	tmp++;

	return tmp;

//  INT16U crc,i,j,carry_flag,a;
//
//  ToggleWD();
//
//  crc=0xffff;
//  for ( i=0;i<n;i++ )
//  {
//  	ToggleWD();
//
//  	crc=crc^trame[i];
//  	for ( j=0;j<8;j++ )
//  	{
//  		a=crc;
//  		carry_flag=a&0x0001;
//  		crc=crc>>1;
//  		if ( carry_flag==1 )
//  			crc=crc^0xa001;
//  	}
//  }
//  trame[n+1]=crc>>8;
//  trame[n]=crc&255;
//  return crc;
}


/******************************************************************************
 �������ƣ�com0_send_hex_modbus
 ������������װЭ���β,���ͳ�ȥ
 ����������
 �������ƣ� ����/����� ����        ����
 str_to_send    ����    INT8U *     Ҫ���͵��ַ���
 len                ����        INT16U      ���ͳ���

 ��  ��  ֵ����

 ��      �� ��������
 ��      �ڣ�2005-04-07
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
void com0_send_hex_modbus(INT8U * str_to_send, INT16U len)
{
	INT8U Lrc;
	INT8U buf[2];

	ToggleWD();

	Lrc = Mb_calcul_lrc(str_to_send, len);

	htoa(buf, (char *)&Lrc, 1);
//  strcat((INT8U *)str_to_send, buf);
	str_to_send[len] = buf[0];
	str_to_send[len + 1] = buf[1];
	str_to_send[len + 2] = 0x0D;
	str_to_send[len + 3] = 0x0A;

	com0_send_hex(str_to_send, len + 4);
}


