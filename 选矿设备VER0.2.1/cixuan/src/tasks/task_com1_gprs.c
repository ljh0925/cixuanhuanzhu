/******************************************************************************
Copyright 2005 大连易达通电子有限公司
All rights riserved.

文件名	：task_com1_gprs.c
模块名称：com1通讯任务 GPRS通讯
功能概要：com1通讯任务 GPRS通讯

取代版本：0.0.1
修改人	：许岩
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include 		"config.h"


OS_STK  TASK_COM1_STACK[TASK_STK_BIG_SIZE];

extern volatile DEV_STAT DevStat;

extern INT8U Com1SndBuf[UART1_SEND_QUEUE_LENGTH];              //串口发送缓冲区
extern INT8U Com1RcvBuf[UART1_RCV_QUEUE_LENGTH];               //串口接收缓冲区

extern int UART1RcvBuf[(UART1_RCV_QUEUE_LENGTH + sizeof(int)- 1)/ sizeof(int)];

//INT8U SndBuf[UART3_SEND_QUEUE_LENGTH];
OS_EVENT        *gprs_sem;

typedef struct _GPRS
{
	INT16U record_base_ptr;                 //未上传记录指针起始指针(原p0+2)
	INT16U record_number;                   //未上传的记录条数(原p0+4)

	INT8U line_number[2];                   //商户号
	INT8U bus_number[3];					//设备编号	

	BUS_TIME last_commu_time;               //上次通讯时间
	INT8U have_driver_rec;                  //是否是司机上下班记录
	INT16U snd_len;                         //发送长度
	INT16U rcv_len;                         //接收长度
    INT16U package_num;
    INT16U last_packgae_rec_num;

	INT8U driver_rec_num_temp;              //司机上下班记录条数 临时
	INT8U driver_rec_num;                   //司机上下班记录条数
	INT8U driver_rec_ptr;                   //司机上下班记录条数，传到那条了
//  INT8U drec[7][18];                      //司机记录 2字节司机编号+8字节车号+7字节时间+1字节上下班标志
//  INT8U drec[7][20];                      //司机记录 4字节司机编号+8字节车号+7字节时间+1字节上下班标志

	INT8U black_name_version[7];            //黑名单版本号
	INT16U black_num;                       //要传输的黑名单条数
	INT16U black_ptr;						//黑名单传输指针


	INT8U drec[20];							//司机记录 4字节司机编号+8字节车号+7字节时间+1字节上下班标志
	INT8U have_drec;						//是否有未上传的drec，是=TRUE 否=FALSE
	INT8U update_time;						//是否准备与服务器同步时间，是=TRUE 否=FALSE
	INT8U driver_is_working;				//上下班标志

}GPRS;


GPRS gprs;


INT8U g_tmp_tbuf[300];
INT8U g_tmp_tbuf2[100];

//debug
char disbuf[100];
//debug end


typedef struct _GPRS_ERR
{
	INT8U comm_break;						//黑名单传输是否中断过 中断过=TRUE 没中断过=FALSE

	INT8U black_name_version[7];            //中断时刻的黑名单版本号
	INT16U black_num;                       //中断时刻的要传输的黑名单条数
	INT16U black_ptr;						//中断时刻的黑名单传输指针

}GPRS_ERR;

GPRS_ERR gprs_err;

/******************************************************************************
 函数名称：TaskCom1Gprs
 功能描述：com1gprs通讯任务
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者：许岩
 日 	 期：2010-06-18
 修改历史：
		日期		修改人		修改描述
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

#define REC_NUM_PER_PACK	10			//每包4条记录
#define COMM_RETRY_TIMES	1			//通讯时候重试次数

	data = data;

	UART1SetBPS(9600, NONE);

	gprs.driver_rec_num_temp = 0;
	gprs.driver_rec_num = 0;
	gprs.driver_rec_ptr = 0;
//	gprs_err.comm_break = FALSE;


//	g_pack_num = 0;				//包号
//	g_turn_bit = 0;				//交替位
//	g_retx = 0;					//是否为重发

	for (;;)
	{
		OSTimeDlyHMSM(0, 5, 0, 0);			//五分钟一次

//com0_start:

		////////////////////////////////////////////////////
		//与服务器同步时间
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
		//传输记录
		////////////////////////////////////////////////////

		OSSemPend(gprs_sem, 0, &err);
		if (DevStat.request_all_rec == TRUE)
		{
			gprs.record_base_ptr = 0;
			ReadParamRecNum();
			{
				gprs.record_number = DevStat.record_base_ptr + DevStat.record_number;   //正常补采时的记录起始指针和条数
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

		gprs.package_num = gprs.record_number / REC_NUM_PER_PACK;				//包数
		gprs.last_packgae_rec_num = gprs.record_number % REC_NUM_PER_PACK;		//剩余条数

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
//  				sprintf((void *)dispbuf, "消费:%d-%d OFF    ", DevStat.consume_cnt, DevStat.record_number);
//  				lcddisp(0, 0, dispbuf);
//  			}
				DevStat.gprs_connected = FALSE;
				(void)OSSemPost(gprs_sem);

				continue;
			}

		}

		//剩余记录打一个包
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
					sprintf((void *)dispbuf, "补采:%d-%d      ", gprs.record_number, k + gprs.package_num * REC_NUM_PER_PACK);
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
					sprintf((void *)dispbuf, "消费:%d-%d OFF    ", DevStat.consume_cnt, DevStat.record_number);
					lcddisp(0, 0, dispbuf);
				}
				DevStat.gprs_connected = FALSE;
				(void)OSSemPost(gprs_sem);

				continue;
			}

		}

		//////////////////最后结尾函数///////////////////////////
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
				sprintf((void *)dispbuf, "消费:%d-%d OFF    ", DevStat.consume_cnt, DevStat.record_number);
				lcddisp(0, 0, dispbuf);
			}
			DevStat.gprs_connected = FALSE;
			(void)OSSemPost(gprs_sem);

			continue;
		}

	}

}

/*****************************************************************
函数原型：gprs_snd_and_rcv_protocol0
功能描述：gprs发送与接收,协议0
参数描述：
参数名称：	输入/输出？	类型		描述

返  回  值：ok-成功
			notok(0xFF)-失败，超时

作      者	：许岩
日      期：2005-05-19
修改历史：
日期		修改人		修改描述
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
			if ( str_rcv[0] != '@' )            //判断开头
				continue;
			if ( CRC_Check(str_rcv, *rcv_len) != ok )
				continue;
			if ( (str_rcv[19] == 'E' && str_rcv[20] == 'R') )
				continue;
			if ( !(str_rcv[6] == packno2[0] && str_rcv[7] == packno2[1]) )      //判断包号
				continue;
			return ( ok );
		}

	}
	return ( notok );
}


/*****************************************************************
 函数原型：gprs_protocol0_add_end
 功能描述：按照gprs通讯协议0加上包头包尾，包头包尾一共附带20字节
 参数描述：
 参数名称：		输入/输出？	类型		描述
 -----------	-----------	------	   	-------
 snd_buf		输入		INT8U  * 	发送缓冲区
 snd_length		输入		INT16U const要发送的长度

 返 回 值：无
 作    者：许岩
 日    期：2013-10-22
 修改历史：
 日期		修改人		修改描述
 ------		---------	-------------
*****************************************************************/
void gprs_protocol1_add_end(INT8U *snd_buf, INT16U const snd_length)
{
	INT16U i = 0;
	INT16U crc;
	INT8U buf[20];

	memmove(&snd_buf[12], snd_buf, snd_length);         //后移12字节
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
 函数名称：dle_add
 功能描述：整理数据包，添加dle
 参数描述：
 参数名称： 输入/输出？ 类型		描述  
 buf		输入输出	INT8U *		要进行dle整理的数据包  
 len		输入输出	INT8U *		长度 
				
 返  回  值：无
				   
 作 	 者：许岩
 日 	 期：2010-06-18
 修改历史：
		日期		修改人		修改描述
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
 函数名称：dle_del
 功能描述：整理数据包，删除dle
 参数描述：
 参数名称： 输入/输出？ 类型		描述  
 buf		输入输出	INT8U *		要进行dle整理的数据包  
 len		输入		INT16U *	长度 
				
 返  回  值：无
				   
 作 	 者：许岩
 日 	 期：2010-06-18
 修改历史：
		日期		修改人		修改描述
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
 函数名称：form_comm_package
 功能描述：整理数据包，添加bcc dle 包头包尾，维护交替位
 参数描述：
 参数名称： 输入/输出？ 类型		描述  
 buf		输入输出	INT8U *		要进行整理的数据包  
 len		输入输出	INT8U *		长度 
				
 返  回  值：无
				   
 作 	 者：许岩
 日 	 期：2010-06-18
 修改历史：
		日期		修改人		修改描述
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
// 函数名称：form_comm_package
// 功能描述：整理数据包，添加bcc dle 包头包尾，维护交替位
// 参数描述：
// 参数名称： 输入/输出？ 类型        描述
// buf        输入输出    INT8U *     要进行整理的数据包
// len        输入输出    INT8U *     长度
//
// 返  回  值：无
//
// 作      者：许岩
// 日      期：2010-06-18
// 修改历史：
//        日期        修改人      修改描述
//        ------      ---------   -------------
//******************************************************************************/
//void form_comm_package(INT8U *buf, INT16U *len)
//{
//
//            //传输黑名单流程，先请求黑名单版本号和条数
//
//            memcpy((void *)g_tmp_tbuf, "\x04\x03\x12", 3);
//            g_pack_num++;
//            g_tmp_tbuf[3] = g_pack_num;
//
//            g_retx = FALSE;
//
//
//            //开始通讯
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
 函数名称：com1puts
 功能描述：串口1输出函数
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 snd_buf	输入			void *		要发送的字符串
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2005-04-07
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void com1puts(const void * snd_buf)
{
	com1_send_hex((void *)snd_buf, (INT16U)strlen(snd_buf));
}
