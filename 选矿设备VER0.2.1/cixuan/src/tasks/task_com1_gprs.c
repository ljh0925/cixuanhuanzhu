/******************************************************************************
Copyright 2005 �����״�ͨ�������޹�˾
All rights riserved.

�ļ���	��task_com1_gprs.c
ģ�����ƣ�com1ͨѶ���� GPRSͨѶ
���ܸ�Ҫ��com1ͨѶ���� GPRSͨѶ

ȡ���汾��0.0.1
�޸���	������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include 		"config.h"


OS_STK  TASK_COM1_STACK[TASK_STK_BIG_SIZE];

extern volatile DEV_STAT DevStat;

extern INT8U Com1SndBuf[UART1_SEND_QUEUE_LENGTH];              //���ڷ��ͻ�����
extern INT8U Com1RcvBuf[UART1_RCV_QUEUE_LENGTH];               //���ڽ��ջ�����

extern int UART1RcvBuf[(UART1_RCV_QUEUE_LENGTH + sizeof(int)- 1)/ sizeof(int)];

//INT8U SndBuf[UART3_SEND_QUEUE_LENGTH];
OS_EVENT        *gprs_sem;

typedef struct _GPRS
{
	INT16U record_base_ptr;                 //δ�ϴ���¼ָ����ʼָ��(ԭp0+2)
	INT16U record_number;                   //δ�ϴ��ļ�¼����(ԭp0+4)

	INT8U line_number[2];                   //�̻���
	INT8U bus_number[3];					//�豸���	

	BUS_TIME last_commu_time;               //�ϴ�ͨѶʱ��
	INT8U have_driver_rec;                  //�Ƿ���˾�����°��¼
	INT16U snd_len;                         //���ͳ���
	INT16U rcv_len;                         //���ճ���
    INT16U package_num;
    INT16U last_packgae_rec_num;

	INT8U driver_rec_num_temp;              //˾�����°��¼���� ��ʱ
	INT8U driver_rec_num;                   //˾�����°��¼����
	INT8U driver_rec_ptr;                   //˾�����°��¼����������������
//  INT8U drec[7][18];                      //˾����¼ 2�ֽ�˾�����+8�ֽڳ���+7�ֽ�ʱ��+1�ֽ����°��־
//  INT8U drec[7][20];                      //˾����¼ 4�ֽ�˾�����+8�ֽڳ���+7�ֽ�ʱ��+1�ֽ����°��־

	INT8U black_name_version[7];            //�������汾��
	INT16U black_num;                       //Ҫ����ĺ���������
	INT16U black_ptr;						//����������ָ��


	INT8U drec[20];							//˾����¼ 4�ֽ�˾�����+8�ֽڳ���+7�ֽ�ʱ��+1�ֽ����°��־
	INT8U have_drec;						//�Ƿ���δ�ϴ���drec����=TRUE ��=FALSE
	INT8U update_time;						//�Ƿ�׼���������ͬ��ʱ�䣬��=TRUE ��=FALSE
	INT8U driver_is_working;				//���°��־

}GPRS;


GPRS gprs;


INT8U g_tmp_tbuf[300];
INT8U g_tmp_tbuf2[100];

//debug
char disbuf[100];
//debug end


typedef struct _GPRS_ERR
{
	INT8U comm_break;						//�����������Ƿ��жϹ� �жϹ�=TRUE û�жϹ�=FALSE

	INT8U black_name_version[7];            //�ж�ʱ�̵ĺ������汾��
	INT16U black_num;                       //�ж�ʱ�̵�Ҫ����ĺ���������
	INT16U black_ptr;						//�ж�ʱ�̵ĺ���������ָ��

}GPRS_ERR;

GPRS_ERR gprs_err;

/******************************************************************************
 �������ƣ�TaskCom1Gprs
 ����������com1gprsͨѶ����
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ����
				   
 �� 	 �ߣ�����
 �� 	 �ڣ�2010-06-18
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void Task_Com1(void *data)
{
	INT16U k = 0;
	//  INT8U num;
	INT8U err = 0;
	BUS_TIME time;
//	BUS_TIME last_time;
	INT8U rec[REC_LEN + 2];
	INT8U i = 0;
	INT16U j = 0;
	INT16U rcv_len = 0;
	INT8U dispbuf[30];
//	INT32S time_last;
//	INT32S time_cur;
//
//	INT16U trans_times = 0;
//	INT16U last_package_black_num = 0;
//	INT16U comm_retry = 0;
//	INT8U black_rec[4];
//	INT16U m = 0;
//	INT16U crc = 0;

#define REC_NUM_PER_PACK	10			//ÿ��4����¼
#define COMM_RETRY_TIMES	1			//ͨѶʱ�����Դ���

	data = data;

	UART1SetBPS(9600, NONE);

	gprs.driver_rec_num_temp = 0;
	gprs.driver_rec_num = 0;
	gprs.driver_rec_ptr = 0;
//	gprs_err.comm_break = FALSE;


//	g_pack_num = 0;				//����
//	g_turn_bit = 0;				//����λ
//	g_retx = 0;					//�Ƿ�Ϊ�ط�

	for (;;)
	{
		OSTimeDlyHMSM(0, 5, 0, 0);			//�����һ��

//com0_start:

		////////////////////////////////////////////////////
		//�������ͬ��ʱ��
		////////////////////////////////////////////////////
		OSSemPend(gprs_sem, 0, &err);
		memcpy((void *)&gprs.bus_number[0], (void *)&DevStat.bus_number[0], sizeof(gprs.bus_number));
		memcpy((void *)&gprs.line_number[0], (void *)&DevStat.line_number[0], sizeof(gprs.line_number));
		(void)OSSemPost(gprs_sem);

		strcpy((void *)Com1SndBuf, "TIME000");
		i = gprs_snd_and_rcv_protocol1(Com1SndBuf, strlen((void *)Com1SndBuf), (void *)Com1RcvBuf, &rcv_len, 10000, 100);
		if ( i == ok )
		{
			time.century = (ascii_to_hex(Com1RcvBuf[19]) << 4) | ascii_to_hex(Com1RcvBuf[20]);
			time.year = (ascii_to_hex(Com1RcvBuf[21]) << 4) | ascii_to_hex(Com1RcvBuf[22]);
			time.month = (ascii_to_hex(Com1RcvBuf[23]) << 4) | ascii_to_hex(Com1RcvBuf[24]);
			time.day = (ascii_to_hex(Com1RcvBuf[25]) << 4) | ascii_to_hex(Com1RcvBuf[26]);
			time.hour = (ascii_to_hex(Com1RcvBuf[27]) << 4) | ascii_to_hex(Com1RcvBuf[28]);
			time.minute = (ascii_to_hex(Com1RcvBuf[29]) << 4) | ascii_to_hex(Com1RcvBuf[30]);
			time.second = (ascii_to_hex(Com1RcvBuf[31]) << 4) | ascii_to_hex(Com1RcvBuf[32]);

			OSSemPend(gprs_sem, 0, &err);
			Modify_Time(&time);
			DevStat.gprs_connected = TRUE;
			(void)OSSemPost(gprs_sem);
		}
		else
		{
			OSSemPend(gprs_sem, 0, &err);
			DevStat.gprs_connected = FALSE;
			(void)OSSemPost(gprs_sem);
			continue;
		}

		////////////////////////////////////////////////////
		//�����¼
		////////////////////////////////////////////////////

		OSSemPend(gprs_sem, 0, &err);
		if (DevStat.request_all_rec == TRUE)
		{
			gprs.record_base_ptr = 0;
			ReadParamRecNum();
			{
				gprs.record_number = DevStat.record_base_ptr + DevStat.record_number;   //��������ʱ�ļ�¼��ʼָ�������
			}
		}
		else
		{
			gprs.record_base_ptr = DevStat.record_base_ptr;
			ReadParamRecNum();
			gprs.record_number = DevStat.record_number;
		}
  		(void)OSSemPost(gprs_sem);

		if ( gprs.record_number == 0 )
		{
			continue;
		}

		Get_Time(&time);

		gprs.package_num = gprs.record_number / REC_NUM_PER_PACK;				//����
		gprs.last_packgae_rec_num = gprs.record_number % REC_NUM_PER_PACK;		//ʣ������

		for(j=0; j<gprs.package_num; j++)
		{
			sprintf((char *)Com1SndBuf, "UPDD%03d%02x%02x%02x%02x%02x", 
					(REC_LEN * REC_NUM_PER_PACK + 22), 
					gprs.line_number[0], gprs.line_number[1],
					gprs.bus_number[0], gprs.bus_number[1], gprs.bus_number[2]);
			sprintf((char *)&Com1SndBuf[17], "%02x%02x%02x%02x%02x%02x", time.year, time.month,
				time.day, time.hour, time.minute, time.second);

			for( k = 0; k < REC_NUM_PER_PACK; k++)
			{						
				OSSemPend(gprs_sem, 0, &err);
				Get_Record((INT8U *)&rec, gprs.record_base_ptr, (j * REC_NUM_PER_PACK) + k);	
				(void)OSSemPost(gprs_sem);
				memcpy(&Com1SndBuf[7 + 22 + k * REC_LEN], (INT8U *)&rec, REC_LEN);	
			}

			i = gprs_snd_and_rcv_protocol1(Com1SndBuf, 7 + 22 + REC_NUM_PER_PACK * REC_LEN, (void *)Com1RcvBuf, &rcv_len, 10000, 100);
			if ( i == ok )
			{
				//do nothing
			}
			else
			{
				OSSemPend(gprs_sem, 0, &err);
//  			if(DevStat.request_all_rec == FALSE)
//  			{
//  				sprintf((void *)dispbuf, "����:%d-%d OFF    ", DevStat.consume_cnt, DevStat.record_number);
//  				lcddisp(0, 0, dispbuf);
//  			}
				DevStat.gprs_connected = FALSE;
				(void)OSSemPost(gprs_sem);

				continue;
			}

		}

		//ʣ���¼��һ����
		if ( gprs.last_packgae_rec_num > 0 )
		{

			sprintf((char *)Com1SndBuf, "UPDD%03d%02x%02x%02x%02x%02x", 
					(REC_LEN * gprs.last_packgae_rec_num + 22), 
					gprs.line_number[0], gprs.line_number[1],
					gprs.bus_number[0], gprs.bus_number[1], gprs.bus_number[2]);
			sprintf((char *)&Com1SndBuf[17], "%02x%02x%02x%02x%02x%02x", time.year, time.month,
				time.day, time.hour, time.minute, time.second);

			for( k = 0; k < gprs.last_packgae_rec_num; k++)
			{	
				OSSemPend(gprs_sem, 0, &err);
				if( DevStat.request_all_rec == TRUE )
				{
					sprintf((void *)dispbuf, "����:%d-%d      ", gprs.record_number, k + gprs.package_num * REC_NUM_PER_PACK);
					lcddisp(0, 0, dispbuf);
				}	
				Get_Record((INT8U *)&rec, gprs.record_base_ptr, (gprs.package_num * REC_NUM_PER_PACK) + k);	
				(void)OSSemPost(gprs_sem);
				memcpy(&Com1SndBuf[7 + 22 + k * REC_LEN], (INT8U *)&rec, REC_LEN);	
			}

			i = gprs_snd_and_rcv_protocol1(Com1SndBuf, 7 + 22 + gprs.last_packgae_rec_num * REC_LEN, (void *)Com1RcvBuf, &rcv_len, 10000, 100);
			if ( i == ok )
			{
				//do nothing
			}
			else
			{
				OSSemPend(gprs_sem, 0, &err);
				if(DevStat.request_all_rec == FALSE)
				{
					sprintf((void *)dispbuf, "����:%d-%d OFF    ", DevStat.consume_cnt, DevStat.record_number);
					lcddisp(0, 0, dispbuf);
				}
				DevStat.gprs_connected = FALSE;
				(void)OSSemPost(gprs_sem);

				continue;
			}

		}

		//////////////////����β����///////////////////////////
		sprintf((char *)Com1SndBuf, "UPDE022%02x%02x%02x%02x%02x", 
				gprs.line_number[0], gprs.line_number[1],
				gprs.bus_number[0], gprs.bus_number[1], gprs.bus_number[2]);
		sprintf((char *)&Com1SndBuf[17], "%02x%02x%02x%02x%02x%02x", time.year, time.month,
			time.day, time.hour, time.minute, time.second);

		i = gprs_snd_and_rcv_protocol1(Com1SndBuf, strlen((void *)Com1SndBuf), (void *)Com1RcvBuf, &rcv_len, 10000, 100);
		if ( i == ok )
		{
			OSSemPend(gprs_sem, 0, &err);

			if (DevStat.request_all_rec == FALSE)
			{
				DevStat.record_base_ptr += gprs.record_number;
				DevStat.record_number -= gprs.record_number;
				//WriteParamRecNum();
				WriteParam();
			}
			else
			{
				DevStat.request_all_rec = FALSE;
			}

			(void)OSSemPost(gprs_sem);
		}
		else
		{
			OSSemPend(gprs_sem, 0, &err);
			if(DevStat.request_all_rec == FALSE)
			{
				sprintf((void *)dispbuf, "����:%d-%d OFF    ", DevStat.consume_cnt, DevStat.record_number);
				lcddisp(0, 0, dispbuf);
			}
			DevStat.gprs_connected = FALSE;
			(void)OSSemPost(gprs_sem);

			continue;
		}

	}

}

/*****************************************************************
����ԭ�ͣ�gprs_snd_and_rcv_protocol0
����������gprs���������,Э��0
����������
�������ƣ�	����/�����	����		����

��  ��  ֵ��ok-�ɹ�
			notok(0xFF)-ʧ�ܣ���ʱ

��      ��	������
��      �ڣ�2005-05-19
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U gprs_snd_and_rcv_protocol1(INT8U *str_to_snd, INT16U snd_len, INT8U *str_rcv, INT16U *rcv_len, INT16U timeout_start, INT16U timeout)
{
	INT8U i = 0;
	INT8U j = 0;
	INT8U packno2[10];

	DevStat.packno++;
	if ( DevStat.packno >= 100 )
		DevStat.packno = 0;
	sprintf((void *)packno2, "%02d", DevStat.packno);

	CleanCom1Buf();

	gprs_protocol1_add_end(str_to_snd, snd_len);

	for ( i = 0; i <= COMM_RETRY_TIMES; i++ )
	{
		//		CleanCom0Buf();
		com1_send_hex(str_to_snd, snd_len + 20);

		j = com1_gets(str_rcv, rcv_len, timeout_start, timeout);
		if ( j == ok )
		{
			if ( str_rcv[0] != '@' )            //�жϿ�ͷ
				continue;
			if ( CRC_Check(str_rcv, *rcv_len) != ok )
				continue;
			if ( (str_rcv[19] == 'E' && str_rcv[20] == 'R') )
				continue;
			if ( !(str_rcv[6] == packno2[0] && str_rcv[7] == packno2[1]) )      //�жϰ���
				continue;
			return ( ok );
		}

	}
	return ( notok );
}


/*****************************************************************
 ����ԭ�ͣ�gprs_protocol0_add_end
 ��������������gprsͨѶЭ��0���ϰ�ͷ��β����ͷ��βһ������20�ֽ�
 ����������
 �������ƣ�		����/�����	����		����
 -----------	-----------	------	   	-------
 snd_buf		����		INT8U  * 	���ͻ�����
 snd_length		����		INT16U constҪ���͵ĳ���

 �� �� ֵ����
 ��    �ߣ�����
 ��    �ڣ�2013-10-22
 �޸���ʷ��
 ����		�޸���		�޸�����
 ------		---------	-------------
*****************************************************************/
void gprs_protocol1_add_end(INT8U *snd_buf, INT16U const snd_length)
{
	INT16U i = 0;
	INT16U crc;
	INT8U buf[20];

	memmove(&snd_buf[12], snd_buf, snd_length);         //����12�ֽ�
//  sprintf((void *)buf, "@START%02X%02X%02X", gprs.line_number[0], gprs.line_number[1], gprs.bus_number[2]);
	sprintf((void *)buf, "@START%02d%02X%02X", DevStat.packno, gprs.bus_number[1], gprs.bus_number[2]);
	memcpy(snd_buf, buf, 12);

	i = snd_length;

	crc = Cal_Crc((void *)snd_buf, i + 12);
	snd_buf[12 + i] = ascii((INT8U)(crc >> 8) >> 4);
	snd_buf[12 + i + 1] = ascii((INT8U)(crc >> 8) & 0x0F);
	snd_buf[12 + i + 2] = ascii((INT8U)crc >> 4);
	snd_buf[12 + i + 3] = ascii((INT8U)crc & 0x0F);

	(void)memcpy((void *)&snd_buf[12 + i + 4], "END\r", 5);

}

/******************************************************************************
 �������ƣ�dle_add
 �����������������ݰ������dle
 ����������
 �������ƣ� ����/����� ����		����  
 buf		�������	INT8U *		Ҫ����dle��������ݰ�  
 len		�������	INT8U *		���� 
				
 ��  ��  ֵ����
				   
 �� 	 �ߣ�����
 �� 	 �ڣ�2010-06-18
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void dle_add(INT8U *buf, INT16U *len)
{
	//  INT16U len1 = *len;
	INT16U len2 = *len;
	INT16U k = 0;

	for (;;)
	{
		if ( buf[k] == 0x02 || buf[k] == 0x03 || buf[k] == 0x10 )
		{
			memmove(&buf[k + 1], &buf[k], len2 - k);
			buf[k] = 0x10;
			len2++;
			k++;
		}
		k++;
		if ( k >= len2 )
		{
			*len = len2;
			return;
		}
	}

}

/******************************************************************************
 �������ƣ�dle_del
 �����������������ݰ���ɾ��dle
 ����������
 �������ƣ� ����/����� ����		����  
 buf		�������	INT8U *		Ҫ����dle��������ݰ�  
 len		����		INT16U *	���� 
				
 ��  ��  ֵ����
				   
 �� 	 �ߣ�����
 �� 	 �ڣ�2010-06-18
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void dle_del(INT8U *buf, INT16U *len)
{
	//  INT16U len1 = *len;
	INT16U len2 = *len;
	INT16U k = 0;

	for (;;)
	{
		if ( buf[k] == 0x10 )
		{
			if ( buf[k + 1] == 0x02 || buf[k + 1] == 0x03 || buf[k + 1] == 0x10 )
			{
				memmove(&buf[k], &buf[k + 1], len2 - k);
				len2--;
			}
		}

		k++;
		if ( k >= len2 )
		{
			*len = len2;
			return;
		}

	}

}

/******************************************************************************
 �������ƣ�form_comm_package
 �����������������ݰ������bcc dle ��ͷ��β��ά������λ
 ����������
 �������ƣ� ����/����� ����		����  
 buf		�������	INT8U *		Ҫ������������ݰ�  
 len		�������	INT8U *		���� 
				
 ��  ��  ֵ����
				   
 �� 	 �ߣ�����
 �� 	 �ڣ�2010-06-18
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void form_comm_package(INT8U *buf, INT16U *len)
{
	INT8U crc = 0;
	INT16U llen = *len;
	char	END[10];

	memcpy(Com1SndBuf, (INT8U *)buf, llen);

	crc = Cal_Crc((unsigned char*)Com1SndBuf, llen);	
	sprintf(END, (void *)"%02X%02XEND\r", ((crc >> 8)& 0xff), (crc & 0xff));
	memcpy(Com1SndBuf+llen, END, 8);
	llen = llen + 8;
	Com1SndBuf[llen] = '\0';

	*len = llen;
}

///******************************************************************************
// �������ƣ�form_comm_package
// �����������������ݰ������bcc dle ��ͷ��β��ά������λ
// ����������
// �������ƣ� ����/����� ����        ����
// buf        �������    INT8U *     Ҫ������������ݰ�
// len        �������    INT8U *     ����
//
// ��  ��  ֵ����
//
// ��      �ߣ�����
// ��      �ڣ�2010-06-18
// �޸���ʷ��
//        ����        �޸���      �޸�����
//        ------      ---------   -------------
//******************************************************************************/
//void form_comm_package(INT8U *buf, INT16U *len)
//{
//
//            //������������̣�������������汾�ź�����
//
//            memcpy((void *)g_tmp_tbuf, "\x04\x03\x12", 3);
//            g_pack_num++;
//            g_tmp_tbuf[3] = g_pack_num;
//
//            g_retx = FALSE;
//
//
//            //��ʼͨѶ
//            for ( j = 0; j < 2; j++ )
//            {
//                gprs.snd_len = 4;
//                form_comm_package(g_tmp_tbuf, &gprs.snd_len);
//
//                CleanCom1Buf();
//                com1_send_hex(Com1SndBuf, gprs.snd_len);
//                i = com1_gets(Com1RcvBuf, &gprs.rcv_len, 1000, 10);
//                if ( i == ok )
//                {
//                    break;
//                }
//
//                g_retx = TRUE;
//            }
//            if ( j >= 2 )
//            {
//                continue;
//            }
//
//            i = com1_gets(Com1RcvBuf, &gprs.rcv_len, 10000, 10);
//            if ( i == ok && Com1RcvBuf[0] == 0x02 )
//            {
//                //do nothing
//            }
//            else
//            {
//                continue;
//            }
//
//            Com1SndBuf[0] = 0x06;
//            Com1SndBuf[1] = Com1RcvBuf[1];
//            com1_send_hex(Com1SndBuf, 2);
//
//
//            dle_del(Com1RcvBuf, &gprs.rcv_len);
//
//            if ( Com1RcvBuf[2] == 0x12 && Com1RcvBuf[3] == g_pack_num )
//            {
//                //do nothing
//            }
//            else
//            {
//                continue;
//            }
//
//            crc = Cal_Crc(&Com1RcvBuf[2], 11);
//            if ( (Com1RcvBuf[2 + 11] == (INT8U)(crc >> 8)) && (Com1RcvBuf[2 + 11 + 1] = (INT8U)(crc & 0x00FF)) )
//            {
//                //do nothing
//            }
//            else
//            {
//                continue;
//            }
//
//}

/******************************************************************************
 �������ƣ�com1puts
 ��������������1�������
 ����������
 �������ƣ� ����/����� ����		����
 snd_buf	����			void *		Ҫ���͵��ַ���
				
 ��  ��  ֵ����
				   
 �� 	 �� ������
 �� 	 �ڣ�2005-04-07
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void com1puts(const void * snd_buf)
{
	com1_send_hex((void *)snd_buf, (INT16U)strlen(snd_buf));
}
