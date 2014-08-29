/******************************************************************************
Copyright 2005 大连英奇科技发展有限公司
All rights riserved.

文件名	：task_com0.c
模块名称：COM0通讯任务程序
功能概要：COM0通讯任务程序

取代版本：0.0.1
修改人	：刘及华
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include 		"config.h"


extern volatile DEV_STAT DevStat;
extern INT8U Com0SndBuf[];				//串口发送缓冲区
extern INT8U Com0RcvBuf[];				//串口接收缓冲区

INT16U Com0ModbusRcvlen = 0;			//串口0接收长度

union f_c
{
    float  l;
    unsigned char c[4];
}f;

/******************************************************************************
 函数名称：Task_Com0
 功能描述：COM1通讯任务
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
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

	for( ;; )										//连接，读取皮带秤的工作状态
	{
		ToggleWD();

		OSTimeDlyHMSM(0, 0, 0, 200);                //200ms一次

		CleanCom0Buf();
		memcpy((void *)Com0SndBuf, "\x30\x33\x30\x30\x30\x38\x30\x30\x30\x31", 10);    //读取皮带秤，工作状态	
		i = com0_snd_and_rcv(Com0SndBuf, 10, Com0RcvBuf, &Com0ModbusRcvlen);
		if (i != ok)
		{
			OS_ENTER_CRITICAL();
			DevStat.connect_scale |= BIT1;		    //连接失败
			OS_EXIT_CRITICAL();
			continue;
		}
		else
		{
			if(memcmp(&Com0RcvBuf[7], (void *)"\x30\x30\x46\x46", 4) == 0)
			{
				OS_ENTER_CRITICAL();
				DevStat.connect_scale &= (~BIT1);	//连接正常,并且在工作状态
				OS_EXIT_CRITICAL();
				break;
			}
			continue;
		}
	}


	for ( ;; )    
	{
		ToggleWD();

		OSTimeDlyHMSM(0, 0, 0, 200);               //200ms一次
		
		CleanCom0Buf();
		memcpy((void *)Com0SndBuf, "\x30\x33\x30\x30\x30\x30\x30\x30\x30\x38", 10);  //读累计量、速度、流量和给定流量数据
		i = com0_snd_and_rcv(Com0SndBuf, 10, Com0RcvBuf, &Com0ModbusRcvlen);
		if (i != ok)
		{
			OS_ENTER_CRITICAL();
			DevStat.connect_scale |= BIT1;       //连接失败
			OS_EXIT_CRITICAL();
			goto com1_start;
		}
		if ((Com0RcvBuf[5] == 0x31) && (Com0RcvBuf[6] == 0x30))	//字节数满足16个
		{
			OS_ENTER_CRITICAL();
			DevStat.connect_scale &= (~BIT1);	//连接正常,并且在工作状态
			OS_EXIT_CRITICAL();
		}
		else
		{
			goto com1_start;
		}

		OS_ENTER_CRITICAL();

		memcpy(temp, (char *)&Com0RcvBuf[7], 8);
		DevStat.MOD_REG.REG_DETAIL.total_weight = atol((void *)temp);    //累积量

		memcpy(temp, (char *)&Com0RcvBuf[15], 8);   //速度
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
		DevStat.MOD_REG.REG_DETAIL.flow = f.l;    	  //流量

		memcpy(temp, (char *)&Com0RcvBuf[31], 8);
		proess_data(temp, 8);
		f.c[0]=(temp[6]<<4)+(temp[7]);
		f.c[1]=(temp[4]<<4)+(temp[5]);
		f.c[2]=(temp[2]<<4)+(temp[3]);
		f.c[3]=(temp[0]<<4)+(temp[1]);
		DevStat.MOD_REG.REG_DETAIL.set_flow = f.l;    //给定流量
//  	sprintf((void *)temp,"%7.3f",f.l);
		OS_EXIT_CRITICAL();

	
		ToggleWD();

		OSTimeDlyHMSM(0, 0, 0, 200);               //200ms一次

//  	CleanCom0Buf();
//  	memcpy((void *)Com0SndBuf, "\x30\x33\x30\x30\x30\x39\x30\x30\x30\x32", 10);  //读当班累积量数据
//  	i = com0_snd_and_rcv(Com0SndBuf, 10, Com0RcvBuf, &Com0ModbusRcvlen);
//  	if (i != ok)
//  	{
//  		OS_ENTER_CRITICAL();
//  		DevStat.connect_scale |= BIT1;       //连接失败
//  		OS_EXIT_CRITICAL();
//  		goto com1_start;
//  	}
//  	OS_ENTER_CRITICAL();
//  	memcpy(temp, (char *)&Com0RcvBuf[7], 2);
//  	DevStat.MOD_REG.REG_DETAIL.class_weight = atol((void *)temp);    //当班累积量
//  	OS_EXIT_CRITICAL();

		CleanCom0Buf();
		memcpy((void *)Com0SndBuf, "\x30\x33\x30\x30\x31\x31\x30\x30\x30\x32", 10);  //读当天累积量数据
		i = com0_snd_and_rcv(Com0SndBuf, 10, Com0RcvBuf, &Com0ModbusRcvlen);
		if (i != ok)
		{
			OS_ENTER_CRITICAL();
			DevStat.connect_scale |= BIT1;       //连接失败
			OS_EXIT_CRITICAL();
			goto com1_start;
		}
		OS_ENTER_CRITICAL();
		memcpy(temp, (char *)&Com0RcvBuf[7], 2);
		DevStat.MOD_REG.REG_DETAIL.today_weight = atol((void *)temp);    //当天累积量
		OS_EXIT_CRITICAL();

		CleanCom0Buf();
		memcpy((void *)Com0SndBuf, "\x30\x33\x30\x30\x31\x33\x30\x30\x30\x32", 10);  //读当月累积量数据
		i = com0_snd_and_rcv(Com0SndBuf, 10, Com0RcvBuf, &Com0ModbusRcvlen);
		if (i != ok)
		{
			OS_ENTER_CRITICAL();
			DevStat.connect_scale |= BIT1;       //连接失败
			OS_EXIT_CRITICAL();
			goto com1_start;
		}
		OS_ENTER_CRITICAL();
		memcpy(temp, (char *)&Com0RcvBuf[7], 2);
		DevStat.MOD_REG.REG_DETAIL.month_weight = atol((void *)temp);    //当月累积量
		OS_EXIT_CRITICAL();

		time_cur = cal_sec(&ltime);
		time_last = cal_sec(&last_time);
		if ( (time_cur - time_last >= (INT32S)(24 * 60 * 60)) || (time_last - time_cur >= (INT32S)(24 * 60 * 60)) )   //超过1天
		{
			//存记录
			StoreRecord();
		}
		else
			continue;
	}
}

/******************************************************************************
 函数名称：StoreRecord
 功能描述：交易记录存储函数
 参数名称：	输入/输出？	类型		描述
	info	输入		当天生产的所有信息
				
 返  回  值：无
 
 作      者	：许岩
 日      期：2004-09-23
 修改历史：
		日期		修改人		修改描述
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
	//组织交易记录格式，相同部份
	//////////////////////////////////////////

	temp_int32u = ReadParamRecNum();
	if(temp_int32u >= 2000)
	{
		lcddisp(0, 0, "请尽快将记录上传");
		temp_int32u = 2000;
	}RECORD

	record[0] = DevStat.kind_of_iron;			//矿石类型	1字节
	record[1] = DevStat.ratio_of_iron;			//铁粉含量比 	1字节	

	weight_of_day = DevStat.MOD_REG.REG_DETAIL.

	memcpy(&record[19], (void *)&cardinfo->card_in_time, 7);			//交易日期4字节和时间 3字节 
	memcpy(&record[26], (void *)&cardinfo->purchase_serial_num[0], 2);	//用户卡脱机交易序号 2字节	

	memcpy(&record[32], (void *)&DevStat.line_number[0], 2);			//2字节线路号(HEX)
	memcpy(&record[34], (void *)&DevStat.bus_number[0], 3); 			//3字节车号(BCD)
	memcpy(&record[37], (void *)&DevStat.cur_driver_number[2], 4);		//4字节驾驶员号(BCD)
	memcpy(&record[41], (void *)&DevStat.pos_number[0], 6); 			//6字节PSAM卡号(HEX)

	//站点编号
    memcpy((INT8U *)&record[0], (INT8U *)&DevStat.line_number[0], 2);				

	//设备编号
    memcpy((INT8U *)&DevStat.MOD_REG.REG_DETAIL.bus_number[0], (INT8U *)&DevStat.bus_number[0], 3);				

	//报警 产生日期4字节和时间3字节
	if(DevStat.alermed == 1)
	{
		memcpy((BUS_TIME *)&DevStat.MOD_REG.REG_DETAIL.alerm_on_time, (BUS_TIME *)&DevStat.alerm_time, 7);
	}
	else
	{
		memset((BUS_TIME *)&DevStat.MOD_REG.REG_DETAIL.alerm_on_time, 0x00, 7);
		DevStat.alermed = 0; 
	}

	

//  //当天矿料重量
//  memcpy((INT8U *)&DevStat.MOD_REG.REG_DETAIL.today_weight[0], &DevStat.MOD_REG.REG_DETAIL.today_weight[0], 4);
//  //当月矿料重量
//  memcpy((INT8U *)&DevStat.MOD_REG.REG_DETAIL.month_weight[0], &DevStat.MOD_REG.REG_DETAIL.month_weight[0], 4);
//
//  memcpy((uchar *)&prec->price, (uchar *)&DevStat.price[DevStat.fill_mode], 4); //单价
//  var_bcd2asc(buf+buf_len, (uchar *)&prec->price, 4);
//  sprintf((void *)(buf+buf_len), "%08d", prec->price);

	WriteParam();
}
/******************************************************************************
 函数名称：cal_sec
 功能描述：计算从2000年1月1日到输入时间的秒数
 参数描述：
 参数名称：	输入/输出？	类型		描述
 time			输入			BUS_TIME*	要进行计算的时间，要求输入时间大于2000年
 
 返  回  值：秒数
   
 作      者	：许岩
 日      期：2006.1.9
 修改历史：
		日期		修改人		修改描述

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

	if ( time->century != 0x20 )		  //输入时间必须为2000年以后
		return 0;

	//计算出当前年的秒数
	i = Is_Leap((INT16U)bcdhex(time->century) * 100 + (INT16U)bcdhex(time->year));
	if ( i == TRUE )
	{	//是闰年
		sec = ( (days_leap[bcdhex(time->month)] + (INT32U)bcdhex(time->day)) * 24 * 60 * 60 
				+ (INT32U)bcdhex(time->hour) * 60 * 60 + (INT32U)bcdhex(time->minute) * 60 + (INT32U)bcdhex(time->second) );
	}
	else
	{
		sec = ( (days[bcdhex(time->month)] + (INT32U)bcdhex(time->day)) * 24 * 60 * 60 
				+ (INT32U)bcdhex(time->hour) * 60 * 60 + (INT32U)bcdhex(time->minute) * 60 + (INT32U)bcdhex(time->second) );
	}

	//计算出从前一年到2000年之间的秒数
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
函数原型：Is_Leap
功能描述： 计算是否为闰年
参数描述：
参数名称：	输入/输出？ 类型		描述
			year	输入		uint 		需要判断是否为闰年的年份
			
返	回	 值：	0-不是闰年
				1-是闰年
				
作		 者 ：许岩
日		 期：2003-11-27
修改历史：
		日期		修改人		修改描述
		------		---------	-------------
*****************************************************************/
INT8U Is_Leap(INT16U year)
{
	return(year % 4 == 0) && ( (year % 100 != 0) || (year % 400 == 0) );
}

/******************************************************************************
 函数名称：proess_data
 功能描述：modbus通许，数据处理
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2014-04-11
 修改历史：
		日期		修改人		修改描述
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
 函数名称：com0_snd_and_rcv
 功能描述：发送并接收
 参数描述：
 参数名称： 输入/输出？ 类型        描述
 str_to_send    输入    INT8U *     要发送的字符串
 len			输入	INT16U      发送长度
 str_rcvd		输出	INT8U *		接收到的字符串
 rcv_len		输出	INT16U *	接收长度

 返  回  值：ok-接收成功
			notok-接收失败

 作      者 ：刘及华
 日      期：2005-04-07
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
INT8U com0_snd_and_rcv(INT8U * str_to_send, INT16U len, INT8U *str_rcvd, INT16U* rcv_len)
{
	INT8U i = 0;
	INT8U j = 0;
	//INT8U buf[5];

	ToggleWD();

	memmove(&str_to_send[3], str_to_send, len);         //后移3字节

	str_to_send[0] = 0x3A;
	str_to_send[1] = DevStat.Modbus_client[0];
	str_to_send[2] = DevStat.Modbus_client[1];

	for ( i=0; i<COM1_RETRY_TIMES; i++ )
	{
		CleanCom0Buf();

		com0_send_hex_modbus((void *)str_to_send, len);
		j = com0_gets((void *)str_rcvd, rcv_len, 200, 20);          //等待接收一包数据
		if ( j == ok )
		{
			if ( Mb_test_lrc(str_rcvd, *rcv_len - 4) == ok )        // 校验LRC
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
 函数名称：xray_ad_to_v
 功能描述：X射线源控制器的AD值，转化为电压值(350表示35kV)
 参数描述：
 参数名称： 输入/输出？ 类型        描述
 ad			输入		INT16U      X射线源控制器的AD值

 返  回  值：电压值(350表示35kV)

 作      者 ：刘及华
 日      期：2005-04-07
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
INT16U xray_ad_to_v(INT16U ad)
{
	ToggleWD();

//   v     ad
//  --- = ---
//   80   4095
//原来的高压源的电压和电流的对应关系：
//
//设置值：
//    5V = 80KV；（高压）
//    5V = 250VA；（电流）
//监测值：
//    10V = 80KV；
//    10V = 250UA；
//
//现在的高压源的电压和电流的对应关系：
//
//设置值：
//    10V = 80KV；（高压）
//    10V = 1500VA；（电流）
//监测值：
//    1.6V = 80KV；
//    3V = 1500UA；

//   v     ad
//  --- = ---
//   80   1311

	return (INT16U)(((FP64)ad * 800.0) / 1311.0);	//X射线电压实际值
}

/******************************************************************************
 函数名称：xray_v_to_ad
 功能描述：电压值(350表示35kV)，转化为X射线源控制器的AD值
 参数描述：
 参数名称： 输入/输出？ 类型        描述
 v			输入		INT16U      电压值(350表示35kV)

 返  回  值：X射线源控制器的AD值

 作      者 ：刘及华
 日      期：2005-04-07
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
INT16U xray_v_to_ad(INT16U v)
{
	ToggleWD();

	return (INT16U)(((FP64)v * 4095.0) / 800.0);
}

/******************************************************************************
 函数名称：xray_ad_to_i
 功能描述：X射线源控制器的AD值，转化为电流值(100表示10uA)
 参数描述：
 参数名称： 输入/输出？ 类型        描述
 ad			输入		INT16U      X射线源控制器的AD值

 返  回  值：电压值(350表示35kV)

 作      者 ：刘及华
 日      期：2005-04-07
 修改历史：
		日期        修改人      修改描述
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

	return (INT16U)(((FP64)ad * 15000.0) / 2458.0);	//X射线电流实际值
}

/******************************************************************************
 函数名称：xray_i_to_ad
 功能描述：电流值(100表示10uA)，转化为X射线源控制器的AD值
 参数描述：
 参数名称： 输入/输出？ 类型        描述
 i			输入		INT16U      电流值(100表示10uA)

 返  回  值：X射线源控制器的AD值

 作      者 ：刘及华
 日      期：2005-04-07
 修改历史：
		日期        修改人      修改描述
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
 函数名称：com0_send_hex_modbus
 功能描述：封装协议包尾,发送出去
 参数描述：
 参数名称： 输入/输出？ 类型        描述
 str_to_send    输入    INT8U *     要发送的字符串
 len                输入        INT16U      发送长度

 返  回  值：无

 作      者 ：刘及华
 日      期：2005-04-07
 修改历史：
		日期        修改人      修改描述
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


