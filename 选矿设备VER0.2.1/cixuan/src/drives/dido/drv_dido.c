/******************************************************************************
Copyright 2005 大连金德姆电子有限公司
All rights riserved.

文件名	：drv_dido.c
模块名称：DIDO驱动程序
功能概要：DIDO驱动程序

取代版本：0.0.1
修改人	：许岩
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include "config.h"


extern volatile DEV_STAT DevStat;



/******************************************************************************
 函数名称：DidoInit
 功能描述： DIDO驱动初始化
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void DidoInit(void)
{
	ToggleWD();

	//要求PINSEL2设置P2口为IO口
	//DI0-7
//  IO2DIR &= (~BIT16);		//P2.16设置为输入
//  IO2DIR &= (~BIT17);		//P2.17设置为输入
//  IO2DIR &= (~BIT18);		//P2.18设置为输入
//  IO2DIR &= (~BIT19);		//P2.19设置为输入
//  IO2DIR &= (~BIT20);		//P2.20设置为输入
//  IO2DIR &= (~BIT21);		//P2.21设置为输入
//  IO2DIR &= (~BIT22);		//P2.22设置为输入
//  IO2DIR &= (~BIT23);		//P2.23设置为输入

	//DO0-7
	IO2DIR |= BIT30;		//P2.30设置为输出
	IO2DIR |= BIT31;		//P2.31设置为输出
//  IO2DIR |= BIT26;        //P2.26设置为输出
//  IO2DIR |= BIT27;        //P2.27设置为输出
//  IO2DIR |= BIT28;		//P2.28设置为输出
//  IO2DIR |= BIT29;		//P2.29设置为输出
//  IO2DIR |= BIT30;		//P2.30设置为输出
//  IO2DIR |= BIT31;		//P2.31设置为输出
//
//  IO2DIR |= BIT0;			//P2.0设置为输出
//  IO2DIR |= BIT1;			//P2.1设置为输出


//	PINSEL2 = PINSEL2 & (~BIT3);        //P1.16~25为通用IO
	IO1DIR |= BIT17;					//P1.17设置为输出
	
//	PINSEL2 = PINSEL2 & (~BIT4);        //P3.28 配置成GPIO
//	PINSEL2 = PINSEL2 & (~BIT5);		//P3.29 配置成GPIO	
	IO3DIR |= BIT28;					//P3.28设置为输出
	IO3DIR |= BIT29;					//P3.29设置为输出   			

//  PINSEL1 = PINSEL1 & (~BIT10);
//  PINSEL1 = PINSEL1 & (~BIT11);		//P0.21 配置成GPIO		DI_CTRL0
//  PINSEL1 = PINSEL1 & (~BIT12);
//  PINSEL1 = PINSEL1 & (~BIT13);		//P0.22 配置成GPIO		DI_CTRL1
//
	PINSEL1 = PINSEL1 & (~BIT24);
	PINSEL1 = PINSEL1 & (~BIT25);		//P0.28 配置成GPIO		DO_CLK
	PINSEL1 = PINSEL1 & (~BIT26);
	PINSEL1 = PINSEL1 & (~BIT27);		//P0.29 配置成GPIO		DO_CTRL0
	PINSEL1 = PINSEL1 & (~BIT28);
	PINSEL1 = PINSEL1 & (~BIT29);		//P0.30 配置成GPIO		DO_CTRL1

//  IO0DIR |= BIT21;		//P0.21设置为输出
//  IO0DIR |= BIT22;		//P0.22设置为输出
	IO0DIR |= BIT28;		//P0.28设置为输出
	IO0DIR |= BIT29;		//P0.29设置为输出
	IO0DIR |= BIT30;		//P0.30设置为输出

//  DI_CTRL0_SET_HIGH();
//  DI_CTRL1_SET_HIGH();
//  DO_CLK_SET_HIGH();
//  DO_CTRL0_SET_HIGH();
//  DO_CTRL1_SET_HIGH();

	SetDO(0x00, 0);

}

/******************************************************************************
 函数名称：GetDIStatus
 功能描述：取得DI状态
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：DI状态，最高8位未使用，从高到低依次对应DI23-DI0
				   
 作 	 者 ：许岩
 日 	 期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT32U GetDIStatus(void)
{
	INT32U di1 = 0;
	INT32U di2 = 0;
	INT32U di3 = 0;
	INT32U temp_int32u = 0;

	ToggleWD();

	OS_ENTER_CRITICAL();

//	DI_CTRL0_SET_LOW();
//	DI_CTRL1_SET_LOW();
//  SleepMs(1);
	Sleep10us(10);
	temp_int32u = IO2PIN;
	di1 = (temp_int32u >> 16) & 0x000000FF;

//	DI_CTRL0_SET_HIGH();
//	DI_CTRL1_SET_LOW();
//  SleepMs(1);
	Sleep10us(10);
	temp_int32u = IO2PIN;
	di2 = (temp_int32u >> 16) & 0x000000FF;

//	DI_CTRL0_SET_LOW();
//	DI_CTRL1_SET_HIGH();
//  SleepMs(1);
	Sleep10us(10);
	temp_int32u = IO2PIN;
	di3 = (temp_int32u >> 16) & 0x000000FF;

//	DI_CTRL0_SET_HIGH();
//	DI_CTRL1_SET_HIGH();

	OS_EXIT_CRITICAL();

	temp_int32u = (di3 << 16) | (di2 << 8) | di1;

	return temp_int32u;
}

/******************************************************************************
 函数名称：SetDO
 功能描述：按给定值设置DO
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 do_set	输入		INT16U			要设置到DO的数，由高位至低位依次对应DO15-DO0
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void SetDO(INT16U do_set, INT8U offset )
{
//	INT8U i = 0;
	INT8U temp_Sec;
	INT16U temp_Mil;

	ToggleWD();

	if(do_set == 0x00)
	{
		IO2SET = BIT30;
		IO2SET = BIT31;
		IO1SET = BIT17;
		IO0SET = BIT28;
		IO3SET = BIT29;
		IO3SET = BIT28;
		IO0SET = BIT29;
		IO0SET = BIT30;
		return;
	}
	else if(do_set == 0xffff)
	{
		switch ( offset )
		{
		   case 0:
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[0]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[0]%1000;
			   
			   if((temp_Sec * 1000 + temp_Mil) <= 10)
				   break;
				IO2CLR = (BIT30);
			   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   //SleepMs(2000);
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[0]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[0]%1000;
			   
			   if((temp_Sec * 1000 + temp_Mil) <= 10)
				   break;
				IO2SET = (BIT30);
			   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   break;
		   case 1:
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[1]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[1]%1000;
			   			   if((temp_Sec * 1000 + temp_Mil) <= 10)
				   break;
			   IO2CLR = (BIT31);
			   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[1]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[1]%1000;
			   			   if((temp_Sec * 1000 + temp_Mil) <= 10)
				   break;
			   IO2SET = (BIT31);
			   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   break;
		   case 2:
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[2]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[2]%1000;
			   			   if((temp_Sec * 1000 + temp_Mil) <= 10)
				   break;
			   IO1CLR = BIT17;
			   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil);
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[2]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[2]%1000;
			   			   if((temp_Sec * 1000 + temp_Mil) <= 10)
				   break;
			   IO1SET = BIT17;
			   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   break;
		   case 3:
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[3]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[3]%1000;
			   			   if((temp_Sec * 1000 + temp_Mil) <= 10)
				   break;
			   IO0CLR = BIT28;
			   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[3]/1000;
			   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[3]%1000;
			   			   if((temp_Sec * 1000 + temp_Mil) <= 10)
				   break;
			   IO0SET = BIT28;
			   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
			   break;
//  	   case 4:
//  		   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[4]/1000;
//  		   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[4]%1000;
//  		   			   if((temp_Sec * 1000 + temp_Mil) <= 10)
//  			   break;
//  		   IO3CLR = BIT29;
//  		   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
//  		   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[4]/1000;
//  		   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[4]%1000;
//  		   			   if((temp_Sec * 1000 + temp_Mil) <= 10)
//  			   break;
//  		   IO3SET = BIT29;
//  		   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
//  		   break;
//  	   case 5:
//  		   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[5]/1000;
//  		   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[5]%1000;
//  		   			   if((temp_Sec * 1000 + temp_Mil) <= 10)
//  			   break;
//  		   IO3CLR = BIT28;
//  		   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
//  		   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[5]/1000;
//  		   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[5]%1000;
//  		   			   if((temp_Sec * 1000 + temp_Mil) <= 10)
//  			   break;
//  		   IO3SET = BIT28;
//  		   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
//  		   break;
//  	   case 6:
//  		   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[6]/1000;
//  		   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[6]%1000;
//  		   	if((temp_Sec * 1000 + temp_Mil) <= 10)
//  			   break;
//  		   IO0CLR = BIT29;
//  		   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
//  		   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[6]/1000;
//  		   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[6]%1000;
//  		   	if((temp_Sec * 1000 + temp_Mil) <= 10)
//  			   break;
//  		   IO0SET = BIT29;
//  		   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
//  		   break;
//  	   case 7:
//  		   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[7]/1000;
//  		   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_on_delay[7]%1000;
//  		   if((temp_Sec * 1000 + temp_Mil) <= 10)
//  			   break;
//  		   IO0CLR = BIT30;
//  		   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
//  		   temp_Sec = DevStat.MOD_REG.REG_DETAIL.switch_interval[7]/1000;
//  		   temp_Mil = DevStat.MOD_REG.REG_DETAIL.switch_interval[7]%1000;
//  		   	if((temp_Sec * 1000 + temp_Mil) <= 10)
//  			   break;
//  		   IO0SET = BIT30;
//  		   OSTimeDlyHMSM(0, 0, temp_Sec, temp_Mil + 1);
//  		   break;
		   default:
			   break;
		}	
		return;
	}

}

/******************************************************************************
 函数名称：SetRelayModbus
 功能描述：按DevStat.mod_coil设置继电器
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
 void SetRelayModbus(INT8U off_set)
{
//	INT8U i = 0;
	INT16U value = 0xffff;

	ToggleWD();

//  OS_ENTER_CRITICAL();
//  for(i=0; i<16; i++)
//  {
//  	if (DevStat.MOD_REG.REG_DETAIL.mod_coil[i] == 1)
//  		value |= (1 << i);
//  }
//  OS_EXIT_CRITICAL();

	SetDO(value, off_set);
}

