/******************************************************************************
Copyright 2005 大连华科仪器有限公司
All rights riserved.

文件名	：TASK_Link.c
模块名称：RTL8091网卡连接程序
功能概要：通过建立信号量，建立网络连接过程

取代版本：0.0.1
修改人	：刘及华
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include "config.h"
#include "../include/cfg_net.h"

extern OS_EVENT 		*RecPackedFlag;
extern OS_EVENT  		*RecTcpPackedFlag;
extern volatile DEV_STAT DevStat;
/******************************************************************************
 函数名称：Task_Link(void *pdata)
 功能描述：建立网络连接任务
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
//               void Task_Link(void *pdata) 
//{
//	void * TcpTemp;
//	uint8 err;   
//    while (1)
//    {
//       	TcpTemp=OSQPend(RecTcpQFlag,0,&err);
//		if(err==OS_NO_ERR)
//		{
//			Process_Tcp1((Rec_Ptr *)TcpTemp);
//			TcpTemp=TcpTemp;
//		}		
//    } 
//   
//}
                 void Task_Link(void *pdata) 

{
	void * UdpTemp;
	uint8 eer;    

    while (1)
    {
		UdpTemp=OSQPend(RecUdpQFlag,0,&eer);
       	if(eer==OS_NO_ERR)
	{
		Udp_Process((Rec_Ptr *)UdpTemp);
		eer=eer;
	}
        
    }    


}

/******************************************************************************
 函数名称：Task_Net(void *pdata)
 功能描述：TCPIP网络任务
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
		void Task_Net(void *pdata) 
{
	//uint8 add[36]={"ZLGMCUyhbabcdefghijklmnopqrstuvwxyz"};
	//uint8 add1[10]={"123456789"};
	uint8 rec_buffer[100];
	SOCKET	s; 
	//uint8 couter_byte=5;
	INT8U Temp[2];
	int rec_coute,send_coute;
	uint16 iii;
	struct sockaddr servaddr,clientaddr;
	OSTimeDly(60);
	servaddr.sin_family=0;
	servaddr.sin_addr[0]=192;
	servaddr.sin_addr[1]=168;
	servaddr.sin_addr[2]=0;
	servaddr.sin_addr[3]=55;
	servaddr.sin_port=1026;  
		OSTimeDly(60);
		s=*socket(0, SOCK_DGRAM, UDP_PROTOCOL);
		clientaddr.sin_family=0;
	clientaddr.sin_addr[0]=NetPort[0].My_Ip[0];
	clientaddr.sin_addr[1]=NetPort[0].My_Ip[1];
	clientaddr.sin_addr[2]=NetPort[0].My_Ip[2];
	clientaddr.sin_addr[3]=NetPort[0].My_Ip[3];
	clientaddr.sin_port=1025;  
	iii=bind( (SOCKET * )&s, (struct sockaddr*)&clientaddr,sizeof(clientaddr));
	while (1)
	{
		rec_coute=recvfrom( s, rec_buffer, 100, 0, (struct sockaddr*)&servaddr, &iii ); 
		if(rec_coute>0)
		{
			if(rec_buffer[0]=='A')
			{				
				Temp[0] = DevStat.MOD_REG.reg[0x0002] >> 8;
				Temp[1] = DevStat.MOD_REG.reg[0x0002];
				send_coute=sendto(s, Temp, 2, 0, (struct sockaddr*)&servaddr, sizeof(servaddr) );			
			}
				
		}
		//while(couter_byte!=0) 
		//{
		//	send_coute=sendto(s, add, 36, 0, (struct sockaddr*)&servaddr, sizeof(servaddr) );
		//	if(send_coute==34)
		//		OSTimeDly(40);
		//	else
		//		send_coute=sendto(s, add1, 9, 0, (struct sockaddr*)&servaddr, sizeof(servaddr) );
		//	OSTimeDly(40);
		//	couter_byte--;
		//}	  
	}	 
}

