/******************************************************************************
Copyright 2005 ���������������޹�˾
All rights riserved.

�ļ���	��TASK_Link.c
ģ�����ƣ�RTL8091�������ӳ���
���ܸ�Ҫ��ͨ�������ź����������������ӹ���

ȡ���汾��0.0.1
�޸���	��������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include "config.h"
#include "../include/cfg_net.h"

extern OS_EVENT 		*RecPackedFlag;
extern OS_EVENT  		*RecTcpPackedFlag;
extern volatile DEV_STAT DevStat;
/******************************************************************************
 �������ƣ�Task_Link(void *pdata)
 ��������������������������
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2005-04-11
 �޸���ʷ��
		����		�޸���		�޸�����
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
 �������ƣ�Task_Net(void *pdata)
 ����������TCPIP��������
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2005-04-11
 �޸���ʷ��
		����		�޸���		�޸�����
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

