
#define TCP_GLOBALS
#include "config.h"
#include "../include/cfg_net.h"

OS_EVENT  		*AppSemSend[MAX_TCP_LINKS];
OS_EVENT  		*AppSemCon[MAX_TCP_LINKS];
OS_EVENT  		*AppSemDisc[MAX_TCP_LINKS];
		
struct Socket_Type TcpStatus[MAX_TCP_LINKS];

tcp_send Tcp_Packed;

/**********************************************************************
**函数原型:	    unsigned int CheckSum(unsigned int xdata *check,unsigned int length)
**入口参数:�	check	:	被校验数据起始地址
				length	:	被校验数据长度，单位字
**出口参数:		无
**返 回 值:		~((sum)&0xffff))		       
**说    明:		计算校验和 
************************************************************************/
//=================================================================================
uint16 CheckSumTcp1(uint8 num,uint16 length)
{
uint32 sum=0;
uint16 i;
for (i=0;i<6;i++)
{
	sum = sum + ((uint32)TcpStatus[num].TcpDHeadUint8[2*i]<<8)+(uint32)TcpStatus[num].TcpDHeadUint8[2*i+1];
}
length=length-12;
i=0;
while(i<(length/2))
{
	sum = sum + ((uint32)TcpStatus[num].TcpHeadUint8[2*i]<<8)+(uint32)TcpStatus[num].TcpHeadUint8[2*i+1];
	i++;
}
if(length&0x0001)//长度为奇数个时，要进行该操作
	{
	//sum = sum + (uint32)(TcpStatus[num].TcpHeadUint8[2*i])<<8;
	sum = sum + (uint32)(TcpStatus[num].TcpHeadUint8[2*i])*256;
	}
sum = (sum&0x0000ffff) + ((sum>>16)&0x0000ffff);//高16位和低16位相加
if(sum & 0xffff0000)
	{//表示有进位
	sum++;
	if(sum & 0xffff0000)
		{//表示有进位
		sum++;
		}
	}

return ( (uint16)(~((sum)&0xffff))  );
}
void Tcp_Initial(void) 
{
uint8 i;
	for(i=0;i<MAX_TCP_LINKS;i++)
	{
		TcpStatus[i].Dest_Port=0;
		TcpStatus[i].My_Port=0;
		TcpStatus[i].State=TCP_STATE_CLOSED;
		TcpStatus[i].StaSem=0;
		TcpStatus[i].Send_Next=0;
		TcpStatus[i].ResendTime=0;
		TcpStatus[i].ResendState=0;
		TcpStatus[i].RecPassSeq=0;
		TcpStatus[i].SenPassSeq=0;
		TcpStatus[i].SenFutureSeq=0;
		TcpStatus[i].RecPassAck=0;
		TcpStatus[i].SenPassAck=0;
		TcpStatus[i].RecFutureAck=0;
		TcpStatus[i].Snd_Window=MAX_TCP_DATA;
		TcpStatus[i].TcpDataQWrPtr=0;
		TcpStatus[i].TcpDataQRdPtr=0;
		AppSemSend[i]=	OSSemCreate(0);
		AppSemCon[i]=	OSSemCreate(0);
		AppSemDisc[i]=	OSSemCreate(0);
	}
	Tcp_Packed.TcpDHead.rev=0;
	Tcp_Packed.TcpDHead.NextProtocal=6;
}
/**********************************************************************
**函数原型：    void        Send_Reset()
**入口参数: 	无
**出口参数: 	无
**返 回 值:		无
**功能说明: 	发送TCP_RST数据报文,复位对方连接
******************************************************************/
void Send_Reset(uint8 num) 
{ 
uint16 i;
	struct _pkst TxdData;
	i=Tcp_Packed.Tcp.SourcePort;
	Tcp_Packed.Tcp.SourcePort=Tcp_Packed.Tcp.DestPort;
	Tcp_Packed.Tcp.DestPort=i;
	Tcp_Packed.Tcp.offset=0x50;
	Tcp_Packed.Tcp.window=TcpStatus[num].Snd_Window;
	Tcp_Packed.Tcp.urg=0;
	Tcp_Packed.Tcp.Crc=0;
	/*
		Tcp_Packed.Tcp.tcpdata[0]=0X02;
		Tcp_Packed.Tcp.tcpdata[1]=0X04;
		Tcp_Packed.Tcp.tcpdata[2]=MAX_TCP_DATA/256;
		Tcp_Packed.Tcp.tcpdata[3]=MAX_TCP_DATA%256;
		Tcp_Packed.Tcp.tcpdata[4]=0X01;
		Tcp_Packed.Tcp.tcpdata[5]=0X01;
		Tcp_Packed.Tcp.tcpdata[6]=0X01;
		Tcp_Packed.Tcp.tcpdata[7]=0X01;*/
	//*********添加TCP假头**********//
	Tcp_Packed.TcpDHead.rev=0;
	Tcp_Packed.TcpDHead.NextProtocal=6;
	Tcp_Packed.TcpDHead.TotalLen=20;
	//Tcp_Packed.Tcp.Crc=CheckSumTcp((uint16 EX_RAM *)&Tcp_Packed,Tcp_Packed.TcpDHead.TotalLen+12);
	TxdData.STPTR=NULL;
	TxdData.length=Tcp_Packed.TcpDHead.TotalLen;
	TxdData.DAPTR=(uint8 EX_RAM *)&Tcp_Packed.Tcp;
	Send_Ip_Frame	
			(
				&TxdData,
				TcpStatus[num].Dest_Ip,
				TcpStatus[num].My_Ip,
				6
			);
	//memcpy (&(TcpStatus[num].resend_data),&(Tcp_Packed.Tcp) , Tcp_Packed.TcpDHead.TotalLen);
	TcpStatus[num].ResendLength=Tcp_Packed.TcpDHead.TotalLen;
}

/**********************************************************************
**函数原型：    void        Delete_Socket()
**入口参数: 	无
**出口参数: 	无
**返 回 值：	无
**功能说明: 	撤销本地连接，并清空重发缓冲区
******************************************************************/
void Delete_Socket(uint8 num) 
{
uint16	i;
OS_ENTER_CRITICAL();
if((TcpStatus[num].StaSem&0x80)==0x80)
{
	TcpStatus[num].State=TCP_STATE_LISTEN;
	TcpStatus[num].StaSem=0x82;
}
else
{
	TcpStatus[num].State=TCP_STATE_CLOSED;
	TcpStatus[num].StaSem=0;
	TcpStatus[num].My_Port=0;
	TcpStatus[num].My_Ip[0]=0;
	TcpStatus[num].My_Ip[1]=0;
	TcpStatus[num].My_Ip[2]=0;
	TcpStatus[num].My_Ip[3]=0;
}
do
{
	i=OSSemAccept(AppSemCon[num]);
}while(i!=0);
do
{
	i=OSSemAccept(AppSemSend[num]);
}while(i!=0);
do
{
	i=OSSemAccept(AppSemDisc[num]);
}while(i!=0);
TcpStatus[num].RecPassSeq=0;
TcpStatus[num].RecPassAck=0;
TcpStatus[num].SenPassSeq=0;
TcpStatus[num].SenPassAck=0;
TcpStatus[num].RecFutureAck=0;
TcpStatus[num].SenFutureSeq=0;
TcpStatus[num].ResendState=0;//表示该重发缓冲区没有数据
TcpStatus[num].ResendTime=0;
TcpStatus[num].Dest_Port=0;
TcpStatus[num].Dest_Ip[0]=0;
TcpStatus[num].Dest_Ip[1]=0;
TcpStatus[num].Dest_Ip[2]=0;
TcpStatus[num].Dest_Ip[3]=0;
TcpStatus[num].TcpDataQWrPtr=0;
TcpStatus[num].TcpDataQRdPtr=0;
OS_EXIT_CRITICAL();
}

void Tcp_Listen(uint8 num) 
{
static uint32 initalseq=32451;
struct _pkst TxdData;
	if(Tcp_Packed.Tcp.control&TCP_SYN)
	{
		TcpStatus[num].State=TCP_STATE_SYN_RCVD;//TCP_STATE_SYN_RCVD;//Tcp_SYN_Rec;
		//TcpStatus[num].Dest_Ip[0]=Tcp_Packed.TcpDHead.DestId[0];
		//TcpStatus[num].Dest_Ip[1]=Tcp_Packed.TcpDHead.DestId[1];
		//TcpStatus[num].Dest_Ip[2]=Tcp_Packed.TcpDHead.DestId[2];
		//TcpStatus[num].Dest_Ip[3]=Tcp_Packed.TcpDHead.DestId[3];
		TcpStatus[num].Dest_Port=Tcp_Packed.Tcp.SourcePort;//对方端口
		//TcpStatus[num].My_Port=Tcp_Packed.Tcp.DestPort;
		TcpStatus[num].IRS=Tcp_Packed.Tcp.SeqNum;//对方的初始化顺序号
		TcpStatus[num].RecPassSeq=Tcp_Packed.Tcp.SeqNum;
		TcpStatus[num].RecPassAck=0;
		TcpStatus[num].SenPassSeq=initalseq;
		TcpStatus[num].SenPassAck=TcpStatus[num].RecPassSeq+1;
		TcpStatus[num].RecFutureAck=initalseq+1;
		TcpStatus[num].SenFutureSeq=initalseq+1;
		TcpStatus[num].Rcv_Next=Tcp_Packed.Tcp.SeqNum+1;//对方的顺序号,用于确认
		TcpStatus[num].ISS=TcpStatus[num].Send_Next;//我的初始化顺序号
		TcpStatus[num].Sent_UnAck=TcpStatus[num].ISS;//我的未确认得序号
		TcpStatus[num].Send_Next=TcpStatus[num].ISS+1;//我的顺序号,用于发送
		TcpStatus[num].My_Wl1=Tcp_Packed.Tcp.SeqNum;//seq
		TcpStatus[num].My_Wl2=TcpStatus[num].Send_Next;
		TcpStatus[num].Rcv_Window=Tcp_Packed.Tcp.window;//对方的WINDOW大小
		TcpStatus[num].Snd_Window=MAX_TCP_DATA;//通知对方本地最大接收1024字节的包,用于流控
		TcpStatus[num].Dest_Max_Seg_Size=MAX_TCP_DATA;//默认为560
		if(Tcp_Packed.Tcp.offset>50)
		if(Tcp_Packed.Tcp.tcpdata[0]==0x02)
		if(Tcp_Packed.Tcp.tcpdata[1]==0x04)		//0204为最大segment选项
		{
			TcpStatus[num].Dest_Max_Seg_Size=Tcp_Packed.Tcp.tcpdata[2]*256+Tcp_Packed.Tcp.tcpdata[3];
		}
		TcpStatus[num].My_Max_Seg_Size=MAX_TCP_DATA;//本地机可以接受最大的以太网数据包
		//********************************************//
		TcpStatus[num].TcpHeadUint8[0]=(TcpStatus[num].My_Port&0xff00)>>8;
		TcpStatus[num].TcpHeadUint8[1]=TcpStatus[num].My_Port&0x00ff;
		Tcp_Packed.Tcp.SourcePort=TcpStatus[num].My_Port;
		TcpStatus[num].TcpHeadUint8[2]=(TcpStatus[num].Dest_Port&0xff00)>>8;
		TcpStatus[num].TcpHeadUint8[3]=TcpStatus[num].Dest_Port&0x00ff;
		Tcp_Packed.Tcp.DestPort=TcpStatus[num].Dest_Port;
		TcpStatus[num].TcpHeadUint8[4]=(TcpStatus[num].SenPassSeq&0xff000000)>>24;
		TcpStatus[num].TcpHeadUint8[5]=(TcpStatus[num].SenPassSeq&0x00ff0000)>>16;
		TcpStatus[num].TcpHeadUint8[6]=(TcpStatus[num].SenPassSeq&0x0000ff00)>>8;
		TcpStatus[num].TcpHeadUint8[7]=(TcpStatus[num].SenPassSeq&0x000000ff);
		Tcp_Packed.Tcp.SeqNum=TcpStatus[num].ISS;
		TcpStatus[num].TcpHeadUint8[8]=(TcpStatus[num].SenPassAck&0xff000000)>>24;
		TcpStatus[num].TcpHeadUint8[9]=(TcpStatus[num].SenPassAck&0x00ff0000)>>16;
		TcpStatus[num].TcpHeadUint8[10]=(TcpStatus[num].SenPassAck&0x0000ff00)>>8;
		TcpStatus[num].TcpHeadUint8[11]=(TcpStatus[num].SenPassAck&0x000000ff);
		Tcp_Packed.Tcp.AckNum=TcpStatus[num].Rcv_Next;
		TcpStatus[num].TcpHeadUint8[12]=0x70;
		Tcp_Packed.Tcp.offset=0x70;
		TcpStatus[num].TcpHeadUint8[13]=0x12;
		Tcp_Packed.Tcp.control=0x12; //syn+ack
		TcpStatus[num].TcpHeadUint8[14]=(TcpStatus[num].Snd_Window&0xff00)>>8;
		TcpStatus[num].TcpHeadUint8[15]=TcpStatus[num].Snd_Window&0x00ff;
		Tcp_Packed.Tcp.window=TcpStatus[num].Snd_Window;
		TcpStatus[num].TcpHeadUint8[16]=0;
		TcpStatus[num].TcpHeadUint8[17]=0;
		Tcp_Packed.Tcp.Crc=0;
		TcpStatus[num].TcpHeadUint8[18]=0;
		TcpStatus[num].TcpHeadUint8[19]=0;
		Tcp_Packed.Tcp.urg=0;
		//******添加TCP头选项*************//
		TcpStatus[num].TcpHeadUint8[20]=0X02;
		Tcp_Packed.Tcp.tcpdata[0]=0X02;
		TcpStatus[num].TcpHeadUint8[21]=0X04;
		Tcp_Packed.Tcp.tcpdata[1]=0X04;
		TcpStatus[num].TcpHeadUint8[22]=MAX_TCP_DATA/256;
		Tcp_Packed.Tcp.tcpdata[2]=MAX_TCP_DATA/256;
		TcpStatus[num].TcpHeadUint8[23]=MAX_TCP_DATA%256;
		Tcp_Packed.Tcp.tcpdata[3]=MAX_TCP_DATA%256;
		TcpStatus[num].TcpHeadUint8[24]=0X01;
		Tcp_Packed.Tcp.tcpdata[4]=0X01;
		TcpStatus[num].TcpHeadUint8[25]=0X01;
		Tcp_Packed.Tcp.tcpdata[5]=0X01;
		TcpStatus[num].TcpHeadUint8[26]=0X01;
		Tcp_Packed.Tcp.tcpdata[6]=0X01;
		TcpStatus[num].TcpHeadUint8[27]=0X01;
		Tcp_Packed.Tcp.tcpdata[7]=0X01;
		//*********添加TCP假头**********//
		TcpStatus[num].TcpDHeadUint8[0]=0;
		Tcp_Packed.TcpDHead.rev=0;
		TcpStatus[num].TcpDHeadUint8[1]=6;
		Tcp_Packed.TcpDHead.NextProtocal=6;
		TcpStatus[num].TcpDHeadUint8[2]=0;
		TcpStatus[num].TcpDHeadUint8[3]=28;
		Tcp_Packed.TcpDHead.TotalLen=28;
		TcpStatus[num].TcpDHeadUint8[4]=TcpStatus[num].My_Ip[0];
		TcpStatus[num].TcpDHeadUint8[5]=TcpStatus[num].My_Ip[1];
		TcpStatus[num].TcpDHeadUint8[6]=TcpStatus[num].My_Ip[2];
		TcpStatus[num].TcpDHeadUint8[7]=TcpStatus[num].My_Ip[3];
		TcpStatus[num].TcpDHeadUint8[8]=TcpStatus[num].Dest_Ip[0];
		TcpStatus[num].TcpDHeadUint8[9]=TcpStatus[num].Dest_Ip[1];
		TcpStatus[num].TcpDHeadUint8[10]=TcpStatus[num].Dest_Ip[2];
		TcpStatus[num].TcpDHeadUint8[11]=TcpStatus[num].Dest_Ip[3];
		//Tcp_Packed.Tcp.Crc=CheckSumTcp((uint16 EX_RAM *)&Tcp_Packed,Tcp_Packed.TcpDHead.TotalLen+12);
		Tcp_Packed.Tcp.Crc=CheckSumTcp1(num,40);//12+28
		TcpStatus[num].TcpHeadUint8[16]=(Tcp_Packed.Tcp.Crc&0xff00)>>8;;
		TcpStatus[num].TcpHeadUint8[17]=Tcp_Packed.Tcp.Crc&0x00ff;
		//memcpy (&(TcpStatus[num].resend_data),&(Tcp_Packed.Tcp) , Tcp_Packed.TcpDHead.TotalLen);
		TcpStatus[num].ResendLength=Tcp_Packed.TcpDHead.TotalLen;
		TcpStatus[num].TcpDataQWrPtr=0;
		TcpStatus[num].TcpDataQRdPtr=0;
		TxdData.STPTR=NULL;
		TxdData.length=Tcp_Packed.TcpDHead.TotalLen;
		TxdData.DAPTR=TcpStatus[num].TcpHeadUint8;
		Send_Ip_Frame	
				(
					&TxdData,
					TcpStatus[num].Dest_Ip,
					TcpStatus[num].My_Ip,
					6
				);

	}
	else if(Tcp_Packed.Tcp.control&TCP_RST)
		{;}
	/*
	else	
		{
			Send_Reset(num);
		}*/	
}
/**********************************************************************
**函数原型：    void        Tcp_SYN_Rec( )
**入口参数:		无
**出口参数:		无
**返 回 值:�	无
**功能说明:		从tcp_listen接收到syn后,即可进入该状态
**        :		该状态可接收tcp_syn和tcp_ack或tcp_rst
**************************************************************/
void Tcp_SYN_Rec(uint8 num) 
{struct _pkst TxdData;
if(Tcp_Packed.Tcp.control&(TCP_RST))//reset//处理reset,对方不接受请求
	{
	Delete_Socket(num);
	//Send_Reset(num);
	}
else if(Tcp_Packed.Tcp.control&(TCP_SYN))//收到同步信号
	{
		TcpStatus[num].RecPassAck=Tcp_Packed.Tcp.AckNum;//确认
		TcpStatus[num].RecPassSeq=Tcp_Packed.Tcp.SeqNum;//对方的初始化顺序号
		TcpStatus[num].SenPassSeq=TcpStatus[num].RecPassAck;
		TcpStatus[num].SenPassAck=TcpStatus[num].RecPassSeq;//我的顺序号,用于发送
		TcpHeadHandle(num ,0x5000+TCP_RST);//headlength 0x20,
		TcpStatus[num].TcpDHeadUint8[2]=0;
		TcpStatus[num].TcpDHeadUint8[3]=20;
		Tcp_Packed.TcpDHead.TotalLen=20;
		Tcp_Packed.Tcp.Crc=CheckSumTcp1(num,32);//12+28
		TcpStatus[num].TcpHeadUint8[16]=(Tcp_Packed.Tcp.Crc&0xff00)>>8;;
		TcpStatus[num].TcpHeadUint8[17]=Tcp_Packed.Tcp.Crc&0x00ff;
		TxdData.STPTR=NULL;
		TxdData.length=20;
		TxdData.DAPTR=TcpStatus[num].TcpHeadUint8;
		Send_Ip_Frame	
			(
				&TxdData,
				TcpStatus[num].Dest_Ip,
				TcpStatus[num].My_Ip,
				6
			);
		Delete_Socket(num);
	//Tcp_Listen(num) ;
	//Send_Reset(num);
	}
else if(Tcp_Packed.Tcp.control&TCP_ACK)//表示这是一个3次握手的确认表明连接建立
	{
 	if((TcpStatus[num].SenPassSeq<=Tcp_Packed.Tcp.AckNum)
	&&(Tcp_Packed.Tcp.AckNum<=TcpStatus[num].RecFutureAck))
		{
		TcpStatus[num].RecPassAck=Tcp_Packed.Tcp.AckNum;//确认
		TcpStatus[num].RecPassSeq=Tcp_Packed.Tcp.SeqNum;//确认
		TcpStatus[num].State=TCP_STATE_ESTABLISHED;//tcp_syn_established;
		if((TcpStatus[num].StaSem&0x02)==0x02)
				OSSemPost(AppSemCon[num]);
	 	//TcpConnected=1;
 	    }
    	}
}
uint8 Tcp_SYN_Sent(uint8 num)
{
static uint32 initalseq=32451;
struct _pkst TxdData;
	if((Tcp_Packed.Tcp.control&TCP_SYN)&&(Tcp_Packed.Tcp.control&TCP_ACK))
	{
		if(TcpStatus[num].RecFutureAck<=Tcp_Packed.Tcp.AckNum)
		{
			TcpStatus[num].RecPassAck=Tcp_Packed.Tcp.AckNum;//确认
			TcpStatus[num].RecPassSeq=Tcp_Packed.Tcp.SeqNum;//对方的初始化顺序号
			TcpStatus[num].SenPassSeq=TcpStatus[num].RecPassAck;
			TcpStatus[num].SenPassAck=TcpStatus[num].RecPassSeq+1;//我的顺序号,用于发送
			TcpStatus[num].My_Wl1=TcpStatus[num].RecPassSeq;//seq
			TcpStatus[num].Rcv_Window=Tcp_Packed.Tcp.window;//对方的WINDOW大小
			TcpStatus[num].Dest_Max_Seg_Size=MAX_TCP_DATA;//默认为560
			if(Tcp_Packed.Tcp.offset>20)
			if(Tcp_Packed.Tcp.tcpdata[0]==0x02)
			if(Tcp_Packed.Tcp.tcpdata[1]==0x04)		//0204为最大segment选项
			{
				TcpStatus[num].Dest_Max_Seg_Size=Tcp_Packed.Tcp.tcpdata[2]*256+Tcp_Packed.Tcp.tcpdata[3];
			}
			//********************************************//
			TcpHeadHandle(num ,0x5000+TCP_ACK);
			TcpStatus[num].TcpDHeadUint8[2]=0;
			TcpStatus[num].TcpDHeadUint8[3]=20;
			Tcp_Packed.Tcp.Crc=CheckSumTcp1(num,32);//12+28
			TcpStatus[num].TcpHeadUint8[16]=(Tcp_Packed.Tcp.Crc&0xff00)>>8;;
			TcpStatus[num].TcpHeadUint8[17]=Tcp_Packed.Tcp.Crc&0x00ff;
			TxdData.STPTR=NULL;
			TxdData.length=20;
			TxdData.DAPTR=TcpStatus[num].TcpHeadUint8;
			Send_Ip_Frame	
					(
						&TxdData,
						TcpStatus[num].Dest_Ip,
						TcpStatus[num].My_Ip,
						6
					);
			TcpStatus[num].State=TCP_STATE_ESTABLISHED;
			if((TcpStatus[num].StaSem&0x02)==0x02)
				OSSemPost(AppSemCon[num]);
			return (0);
 	    }
 	    return (0);
	}
	else if(Tcp_Packed.Tcp.control&TCP_SYN)
	{
		TcpStatus[num].State=TCP_STATE_SYN_RCVD;//TCP_STATE_SYN_RCVD;//Tcp_SYN_Rec;
		//TcpStatus[num].Dest_Ip[0]=Tcp_Packed.TcpDHead.DestId[0];
		//TcpStatus[num].Dest_Ip[1]=Tcp_Packed.TcpDHead.DestId[1];
		//TcpStatus[num].Dest_Ip[2]=Tcp_Packed.TcpDHead.DestId[2];
		//TcpStatus[num].Dest_Ip[3]=Tcp_Packed.TcpDHead.DestId[3];
		TcpStatus[num].Dest_Port=Tcp_Packed.Tcp.SourcePort;//对方端口
		//TcpStatus[num].My_Port=Tcp_Packed.Tcp.DestPort;
		TcpStatus[num].IRS=Tcp_Packed.Tcp.SeqNum;//对方的初始化顺序号
		TcpStatus[num].RecPassSeq=Tcp_Packed.Tcp.SeqNum;
		TcpStatus[num].RecPassAck=0;
		TcpStatus[num].SenPassSeq=initalseq;
		TcpStatus[num].SenPassAck=TcpStatus[num].RecPassSeq+1;
		TcpStatus[num].RecFutureAck=initalseq+1;
		TcpStatus[num].SenFutureSeq=initalseq+1;
		TcpStatus[num].Rcv_Next=Tcp_Packed.Tcp.SeqNum+1;//对方的顺序号,用于确认
		TcpStatus[num].ISS=TcpStatus[num].Send_Next;//我的初始化顺序号
		TcpStatus[num].Sent_UnAck=TcpStatus[num].ISS;//我的未确认得序号
		TcpStatus[num].Send_Next=TcpStatus[num].ISS+1;//我的顺序号,用于发送
		TcpStatus[num].My_Wl1=Tcp_Packed.Tcp.SeqNum;//seq
		TcpStatus[num].My_Wl2=TcpStatus[num].Send_Next;
		TcpStatus[num].Rcv_Window=Tcp_Packed.Tcp.window;//对方的WINDOW大小
		TcpStatus[num].Snd_Window=MAX_TCP_DATA;//通知对方本地最大接收1024字节的包,用于流控
		TcpStatus[num].Dest_Max_Seg_Size=MAX_TCP_DATA;//默认为560
		if(Tcp_Packed.Tcp.offset>50)
		if(Tcp_Packed.Tcp.tcpdata[0]==0x02)
		if(Tcp_Packed.Tcp.tcpdata[1]==0x04)		//0204为最大segment选项
		{
			TcpStatus[num].Dest_Max_Seg_Size=Tcp_Packed.Tcp.tcpdata[2]*256+Tcp_Packed.Tcp.tcpdata[3];
		}
		TcpStatus[num].My_Max_Seg_Size=MAX_TCP_DATA;//本地机可以接受最大的以太网数据包
		//********************************************//
		TcpStatus[num].TcpHeadUint8[0]=(TcpStatus[num].My_Port&0xff00)>>8;
		TcpStatus[num].TcpHeadUint8[1]=TcpStatus[num].My_Port&0x00ff;
		Tcp_Packed.Tcp.SourcePort=TcpStatus[num].My_Port;
		TcpStatus[num].TcpHeadUint8[2]=(TcpStatus[num].Dest_Port&0xff00)>>8;
		TcpStatus[num].TcpHeadUint8[3]=TcpStatus[num].Dest_Port&0x00ff;
		Tcp_Packed.Tcp.DestPort=TcpStatus[num].Dest_Port;
		TcpStatus[num].TcpHeadUint8[4]=(TcpStatus[num].SenPassSeq&0xff000000)>>24;
		TcpStatus[num].TcpHeadUint8[5]=(TcpStatus[num].SenPassSeq&0x00ff0000)>>16;
		TcpStatus[num].TcpHeadUint8[6]=(TcpStatus[num].SenPassSeq&0x0000ff00)>>8;
		TcpStatus[num].TcpHeadUint8[7]=(TcpStatus[num].SenPassSeq&0x000000ff);
		Tcp_Packed.Tcp.SeqNum=TcpStatus[num].ISS;
		TcpStatus[num].TcpHeadUint8[8]=(TcpStatus[num].SenPassAck&0xff000000)>>24;
		TcpStatus[num].TcpHeadUint8[9]=(TcpStatus[num].SenPassAck&0x00ff0000)>>16;
		TcpStatus[num].TcpHeadUint8[10]=(TcpStatus[num].SenPassAck&0x0000ff00)>>8;
		TcpStatus[num].TcpHeadUint8[11]=(TcpStatus[num].SenPassAck&0x000000ff);
		Tcp_Packed.Tcp.AckNum=TcpStatus[num].Rcv_Next;
		TcpStatus[num].TcpHeadUint8[12]=0x70;
		Tcp_Packed.Tcp.offset=0x70;
		TcpStatus[num].TcpHeadUint8[13]=0x12;
		Tcp_Packed.Tcp.control=0x12; //syn+ack
		TcpStatus[num].TcpHeadUint8[14]=(TcpStatus[num].Snd_Window&0xff00)>>8;
		TcpStatus[num].TcpHeadUint8[15]=TcpStatus[num].Snd_Window&0x00ff;
		Tcp_Packed.Tcp.window=TcpStatus[num].Snd_Window;
		TcpStatus[num].TcpHeadUint8[16]=0;
		TcpStatus[num].TcpHeadUint8[17]=0;
		Tcp_Packed.Tcp.Crc=0;
		TcpStatus[num].TcpHeadUint8[18]=0;
		TcpStatus[num].TcpHeadUint8[19]=0;
		Tcp_Packed.Tcp.urg=0;
		//******添加TCP头选项*************//
		TcpStatus[num].TcpHeadUint8[20]=0X02;
		Tcp_Packed.Tcp.tcpdata[0]=0X02;
		TcpStatus[num].TcpHeadUint8[21]=0X04;
		Tcp_Packed.Tcp.tcpdata[1]=0X04;
		TcpStatus[num].TcpHeadUint8[22]=MAX_TCP_DATA/256;
		Tcp_Packed.Tcp.tcpdata[2]=MAX_TCP_DATA/256;
		TcpStatus[num].TcpHeadUint8[23]=MAX_TCP_DATA%256;
		Tcp_Packed.Tcp.tcpdata[3]=MAX_TCP_DATA%256;
		TcpStatus[num].TcpHeadUint8[24]=0X01;
		Tcp_Packed.Tcp.tcpdata[4]=0X01;
		TcpStatus[num].TcpHeadUint8[25]=0X01;
		Tcp_Packed.Tcp.tcpdata[5]=0X01;
		TcpStatus[num].TcpHeadUint8[26]=0X01;
		Tcp_Packed.Tcp.tcpdata[6]=0X01;
		TcpStatus[num].TcpHeadUint8[27]=0X01;
		Tcp_Packed.Tcp.tcpdata[7]=0X01;
		//*********添加TCP假头**********//
		TcpStatus[num].TcpDHeadUint8[0]=0;
		Tcp_Packed.TcpDHead.rev=0;
		TcpStatus[num].TcpDHeadUint8[1]=6;
		Tcp_Packed.TcpDHead.NextProtocal=6;
		TcpStatus[num].TcpDHeadUint8[2]=0;
		TcpStatus[num].TcpDHeadUint8[3]=28;
		Tcp_Packed.TcpDHead.TotalLen=28;
		TcpStatus[num].TcpDHeadUint8[4]=TcpStatus[num].My_Ip[0];
		TcpStatus[num].TcpDHeadUint8[5]=TcpStatus[num].My_Ip[1];
		TcpStatus[num].TcpDHeadUint8[6]=TcpStatus[num].My_Ip[2];
		TcpStatus[num].TcpDHeadUint8[7]=TcpStatus[num].My_Ip[3];
		TcpStatus[num].TcpDHeadUint8[8]=TcpStatus[num].Dest_Ip[0];
		TcpStatus[num].TcpDHeadUint8[9]=TcpStatus[num].Dest_Ip[1];
		TcpStatus[num].TcpDHeadUint8[10]=TcpStatus[num].Dest_Ip[2];
		TcpStatus[num].TcpDHeadUint8[11]=TcpStatus[num].Dest_Ip[3];
		//Tcp_Packed.Tcp.Crc=CheckSumTcp((uint16 EX_RAM *)&Tcp_Packed,Tcp_Packed.TcpDHead.TotalLen+12);
		Tcp_Packed.Tcp.Crc=CheckSumTcp1(num,40);//12+28
		TcpStatus[num].TcpHeadUint8[16]=(Tcp_Packed.Tcp.Crc&0xff00)>>8;;
		TcpStatus[num].TcpHeadUint8[17]=Tcp_Packed.Tcp.Crc&0x00ff;
		//memcpy (&(TcpStatus[num].resend_data),&(Tcp_Packed.Tcp) , Tcp_Packed.TcpDHead.TotalLen);
		TcpStatus[num].ResendLength=Tcp_Packed.TcpDHead.TotalLen;
		
		TxdData.STPTR=NULL;
		TxdData.length=Tcp_Packed.TcpDHead.TotalLen;
		TxdData.DAPTR=TcpStatus[num].TcpHeadUint8;
		Send_Ip_Frame	
				(
					&TxdData,
					TcpStatus[num].Dest_Ip,
					TcpStatus[num].My_Ip,
					6
				);
	}
	else if(Tcp_Packed.Tcp.control&TCP_RST)
	{
		Delete_Socket(num);
	}
	else if((Tcp_Packed.Tcp.control&TCP_ACK)==TCP_ACK)
	{
		TcpStatus[num].RecPassAck=Tcp_Packed.Tcp.AckNum;//确认
		TcpStatus[num].RecPassSeq=Tcp_Packed.Tcp.SeqNum;//对方的初始化顺序号
		TcpStatus[num].SenPassSeq=TcpStatus[num].RecPassAck;
		TcpStatus[num].SenPassAck=TcpStatus[num].RecPassSeq;//我的顺序号,用于发送
		TcpHeadHandle(num ,0x5000+TCP_RST);//headlength 0x20,
		TcpStatus[num].TcpDHeadUint8[2]=0;
		TcpStatus[num].TcpDHeadUint8[3]=20;
		Tcp_Packed.TcpDHead.TotalLen=20;
		Tcp_Packed.Tcp.Crc=CheckSumTcp1(num,32);//12+28
		TcpStatus[num].TcpHeadUint8[16]=(Tcp_Packed.Tcp.Crc&0xff00)>>8;;
		TcpStatus[num].TcpHeadUint8[17]=Tcp_Packed.Tcp.Crc&0x00ff;
		TxdData.STPTR=NULL;
		TxdData.length=20;
		TxdData.DAPTR=TcpStatus[num].TcpHeadUint8;
		Send_Ip_Frame	
			(
				&TxdData,
				TcpStatus[num].Dest_Ip,
				TcpStatus[num].My_Ip,
				6
			);
	}
	return 0;
}
/**********************************************************************
**函数原型：    void        Tcp_Close_Wait( )
**入口参数:		无
**出口参数:		无
**返 回 值：	无
**说    明：	TCP等待关闭状态的处理
************************************************************************/
/*void Tcp_Close_Wait(uint8 num)
{
	//TcpStatus[num].Send_Next++;
uint16 i;
	i=Tcp_Packed.Tcp.SourcePort;
	Tcp_Packed.Tcp.SourcePort=Tcp_Packed.Tcp.DestPort;
	Tcp_Packed.Tcp.DestPort=i;
	Tcp_Packed.Tcp.SeqNum=TcpStatus[num].Send_Next;
	Tcp_Packed.Tcp.AckNum=TcpStatus[num].Rcv_Next;
	Tcp_Packed.Tcp.control=TCP_FIN+TCP_ACK;
	TcpStatus[num].Sent_UnAck=TcpStatus[num].Send_Next;
	TcpStatus[num].State=TCP_STATE_CLOSED;//TCP_STATE_LAST_ACK ;
	Send_Reset(num);//复位对方连接		
//	TcpStatus[num].State=TCP_STATE_LAST_ACK ;
}
*/
/**********************************************************************
**函数原型：    void        Tcp_Last_Ack( )
**入口参数:		无
**出口参数:		无
**返 回 值：	无
**说    明：	接收到TCP_RST,TCP_SYN或TCP_ACK报文时,关闭连接
************************************************************************/
void Tcp_Last_Ack(uint8 num) 
{

if(Tcp_Packed.Tcp.control&(TCP_RST+TCP_SYN+TCP_ACK))
	{//对方不接受请求,关闭本地连接
     		Delete_Socket(num);
	}
}

uint8 Tcp_Established(uint8 num) 
{//
	uint16 i;
	uint16 temp;
//	uint16 tcplength;
	struct _pkst TxdData;
	if(Tcp_Packed.Tcp.control&(TCP_RST+TCP_SYN))
	{
		Delete_Socket(num);
		return (0);
	}
	if(Tcp_Packed.Tcp.control&(TCP_FIN))
	{
		TcpStatus[num].RecPassSeq=Tcp_Packed.Tcp.SeqNum;
		TcpStatus[num].RecPassAck=Tcp_Packed.Tcp.AckNum;
		//Tcp_Packed.Tcp.control=TCP_ACK;
		TcpStatus[num].SenPassSeq=TcpStatus[num].RecPassAck;
		TcpStatus[num].SenPassAck=TcpStatus[num].RecPassSeq+1;
		TcpStatus[num].SenFutureSeq=TcpStatus[num].SenPassSeq;
		TcpStatus[num].RecFutureAck=TcpStatus[num].SenPassSeq+1;
		TcpHeadHandle(num ,0x5000+TCP_ACK);//headlength 0x20,
		TcpStatus[num].TcpDHeadUint8[2]=0;
		TcpStatus[num].TcpDHeadUint8[3]=20;
		Tcp_Packed.TcpDHead.TotalLen=20;
		Tcp_Packed.Tcp.Crc=CheckSumTcp1(num,32);//12+28
		TcpStatus[num].TcpHeadUint8[16]=(Tcp_Packed.Tcp.Crc&0xff00)>>8;;
		TcpStatus[num].TcpHeadUint8[17]=Tcp_Packed.Tcp.Crc&0x00ff;
		TxdData.STPTR=NULL;
		TxdData.length=20;
		TxdData.DAPTR=TcpStatus[num].TcpHeadUint8;
		Send_Ip_Frame	
			(
				&TxdData,
				TcpStatus[num].Dest_Ip,
				TcpStatus[num].My_Ip,
				6
			);
		/***************CLOSING WAIT*****************/	
		//Tcp_Packed.Tcp.control=TCP_FIN+TCP_ACK;
		TcpHeadHandle(num ,0x5000+TCP_FIN+TCP_ACK);//headlength 0x20,
		TcpStatus[num].TcpDHeadUint8[2]=0;
		TcpStatus[num].TcpDHeadUint8[3]=20;
		Tcp_Packed.TcpDHead.TotalLen=20;
		Tcp_Packed.Tcp.Crc=CheckSumTcp1(num,32);//12+20
		TcpStatus[num].TcpHeadUint8[16]=(Tcp_Packed.Tcp.Crc&0xff00)>>8;;
		TcpStatus[num].TcpHeadUint8[17]=Tcp_Packed.Tcp.Crc&0x00ff;
		TxdData.STPTR=NULL;
		TxdData.length=20;
		TxdData.DAPTR=TcpStatus[num].TcpHeadUint8;
		Send_Ip_Frame	
			(
				&TxdData,
				TcpStatus[num].Dest_Ip,
				TcpStatus[num].My_Ip,
				6
			);
		//Tcp_Close_Wait(num);
		TcpStatus[num].State=TCP_STATE_LAST_ACK;
		return(0);
	}
	temp=Tcp_Packed.Tcp.offset>>4;
	temp=temp*4;	//IP首部占20字节
	temp=Tcp_Packed.TcpDHead.TotalLen-temp;//数据报总长度减去TCP和IP首部长度,得到数据长度
	if((temp>0)&&(TcpStatus[num].Snd_Window>=temp))
	{
		if(Tcp_Packed.Tcp.SeqNum>=TcpStatus[num].RecFutureAck)//??
		{
			for(i=0;i<temp;i++)
			{
				/************把接收的数据放到接收缓冲区***************/
				TcpStatus[num].TcpDataQ[TcpStatus[num].TcpDataQWrPtr]=Tcp_Packed.Tcp.tcpdata[i];
				if(TcpStatus[num].TcpDataQWrPtr>=(MAX_TCP_DATA-1))
					TcpStatus[num].TcpDataQWrPtr=0;
				else
					TcpStatus[num].TcpDataQWrPtr++;
			}
			TcpStatus[num].Snd_Window=TcpStatus[num].Snd_Window-temp;
			TcpStatus[num].SenPassAck=TcpStatus[num].SenPassAck+temp;
		}
	
		if(Tcp_Packed.Tcp.control&TCP_ACK)
		{//表示这是一个3次握手的确认
			if((TcpStatus[num].SenPassSeq<=Tcp_Packed.Tcp.AckNum)
			&&(Tcp_Packed.Tcp.AckNum<=TcpStatus[num].RecFutureAck))
			{
				TcpStatus[num].RecPassAck=Tcp_Packed.Tcp.AckNum;//确认
				
				//if(Resend_Buff.TcpFrame.SeqNum<TCP1024.Sent_UnAck);
			}
			TcpStatus[num].RecPassSeq=Tcp_Packed.Tcp.SeqNum;
			TcpStatus[num].SenPassSeq=Tcp_Packed.Tcp.AckNum;
			TcpStatus[num].SenFutureSeq=TcpStatus[num].SenFutureSeq;
			TcpStatus[num].RecFutureAck=TcpStatus[num].SenPassSeq;
			TcpHeadHandle(num ,0x5010);//headlength 0x20 , ack
			TcpStatus[num].TcpDHeadUint8[2]=0;
			TcpStatus[num].TcpDHeadUint8[3]=20;
			Tcp_Packed.TcpDHead.TotalLen=20;
			Tcp_Packed.Tcp.Crc=CheckSumTcp1(num,32);//12+28
			TcpStatus[num].TcpHeadUint8[16]=(Tcp_Packed.Tcp.Crc&0xff00)>>8;;
			TcpStatus[num].TcpHeadUint8[17]=Tcp_Packed.Tcp.Crc&0x00ff;
			TxdData.STPTR=NULL;
			TxdData.length=20;
			TxdData.DAPTR=TcpStatus[num].TcpHeadUint8;
			Send_Ip_Frame	
				(
					&TxdData,
					TcpStatus[num].Dest_Ip,
					TcpStatus[num].My_Ip,
					6
				);
		}
		
		return (1);
	}
	else if(((Tcp_Packed.Tcp.control&TCP_ACK)==TCP_ACK)&&(temp>0)) 
	{
		TcpStatus[num].RecPassSeq=Tcp_Packed.Tcp.SeqNum;
		TcpStatus[num].SenPassSeq=Tcp_Packed.Tcp.AckNum;
		TcpStatus[num].SenFutureSeq=TcpStatus[num].SenFutureSeq;
		TcpStatus[num].RecFutureAck=TcpStatus[num].SenPassSeq;
		TcpHeadHandle(num ,0x5010);//headlength 0x20 , ack
		TcpStatus[num].TcpDHeadUint8[2]=0;
		TcpStatus[num].TcpDHeadUint8[3]=20;
		Tcp_Packed.TcpDHead.TotalLen=20;
		Tcp_Packed.Tcp.Crc=CheckSumTcp1(num,32);//12+28
		TcpStatus[num].TcpHeadUint8[16]=(Tcp_Packed.Tcp.Crc&0xff00)>>8;;
		TcpStatus[num].TcpHeadUint8[17]=Tcp_Packed.Tcp.Crc&0x00ff;
		TxdData.STPTR=NULL;
		TxdData.length=20;
		TxdData.DAPTR=TcpStatus[num].TcpHeadUint8;
		Send_Ip_Frame	
			(
				&TxdData,
				TcpStatus[num].Dest_Ip,
				TcpStatus[num].My_Ip,
				6
			);
	}
	else if(((Tcp_Packed.Tcp.control&TCP_ACK)==TCP_ACK)&&(temp==0))
	{//表示这是一个3次握手的确认
		if((TcpStatus[num].SenPassSeq<=Tcp_Packed.Tcp.AckNum)
		&&(Tcp_Packed.Tcp.AckNum<=TcpStatus[num].RecFutureAck))
		{
			TcpStatus[num].RecPassAck=Tcp_Packed.Tcp.AckNum;//确认
			if((TcpStatus[num].StaSem&0x01)==0x01)
			{
				TcpStatus[num].StaSem=TcpStatus[num].StaSem&0xFE;
				OSSemPost(AppSemSend[num]);
			}
			return (2);
		}
		return (3);
	}
	return (4);
}

void TCP_FIN_WAIT1(uint8 num)
{
	if(Tcp_Packed.Tcp.control&(TCP_ACK))
	{
		TcpStatus[num].RecPassSeq=Tcp_Packed.Tcp.SeqNum;
		TcpStatus[num].RecPassAck=Tcp_Packed.Tcp.AckNum;
		TcpStatus[num].RecFutureAck=TcpStatus[num].RecPassAck;
		TcpStatus[num].State=TCP_STATE_FIN_WAIT2;
	}
	else
		OSSemPost(AppSemDisc[num]);
}     
void TCP_FIN_WAIT2(uint8 num)
{
struct _pkst TxdData;
	if(Tcp_Packed.Tcp.control&(TCP_FIN))
	{
		TcpStatus[num].RecPassSeq=Tcp_Packed.Tcp.SeqNum;
		TcpStatus[num].RecPassAck=Tcp_Packed.Tcp.AckNum;
		TcpStatus[num].SenPassSeq=TcpStatus[num].RecPassAck;
		TcpStatus[num].SenPassAck=TcpStatus[num].RecPassSeq+1;
		TcpHeadHandle(num ,0x5000+TCP_ACK);//headlength 0x20,
		TcpStatus[num].TcpDHeadUint8[2]=0;
		TcpStatus[num].TcpDHeadUint8[3]=20;
		Tcp_Packed.TcpDHead.TotalLen=20;
		Tcp_Packed.Tcp.Crc=CheckSumTcp1(num,32);//12+28
		TcpStatus[num].TcpHeadUint8[16]=(Tcp_Packed.Tcp.Crc&0xff00)>>8;;
		TcpStatus[num].TcpHeadUint8[17]=Tcp_Packed.Tcp.Crc&0x00ff;
		TxdData.STPTR=NULL;
		TxdData.length=20;
		TxdData.DAPTR=TcpStatus[num].TcpHeadUint8;
		Send_Ip_Frame	
			(
				&TxdData,
				TcpStatus[num].Dest_Ip,
				TcpStatus[num].My_Ip,
				6
			);
	}
	OSSemPost(AppSemDisc[num]);
}
uint8 Process_Tcp1(Rec_Ptr * Tcp_Rec)  
{
uint8 i,j;
uint8 * temp_ptr;
struct _pkst TxdData;
	if((*Tcp_Rec).length<=(MAX_TCP_DATA+20))
	{
		OS_ENTER_CRITICAL();
		memcpy (&(Tcp_Packed.Tcp), (*Tcp_Rec).RecDataPtr, (*Tcp_Rec).length);
		OS_EXIT_CRITICAL();
	}
	else
	{
		return 0;
	}
	temp_ptr=(uint8 *)&(Tcp_Packed.Tcp);
	Tcp_Packed.Tcp.SourcePort=Char2ToInt16(temp_ptr);
	temp_ptr=temp_ptr+2;
	Tcp_Packed.Tcp.DestPort=Char2ToInt16(temp_ptr);
	temp_ptr=temp_ptr+2;
	Tcp_Packed.Tcp.SeqNum=Char4ToInt32(temp_ptr);
	temp_ptr=temp_ptr+4;
	Tcp_Packed.Tcp.AckNum=Char4ToInt32(temp_ptr);
	temp_ptr=temp_ptr+4;
	temp_ptr=temp_ptr+2;
	Tcp_Packed.Tcp.window=Char2ToInt16(temp_ptr);
	temp_ptr=temp_ptr+4;
	Tcp_Packed.Tcp.urg=Char2ToInt16(temp_ptr);
	i=0;
	j=MAX_TCP_LINKS;
	Tcp_Packed.TcpDHead.TotalLen=(*Tcp_Rec).length;
	for(i=0;i<MAX_TCP_LINKS;i++)
	{
		if(TcpStatus[i].State>=TCP_STATE_SYN_RCVD)//已经建立的连接
		{
			if(TcpStatus[i].Dest_Ip[0]==((Rec_Ptr*)Tcp_Rec)->ip[0])
			if(TcpStatus[i].Dest_Ip[1]==((Rec_Ptr*)Tcp_Rec)->ip[1])
			if(TcpStatus[i].Dest_Ip[2]==((Rec_Ptr*)Tcp_Rec)->ip[2])
			if(TcpStatus[i].Dest_Ip[3]==((Rec_Ptr*)Tcp_Rec)->ip[3])
			if(TcpStatus[i].My_Ip[0]==((Rec_Ptr*)Tcp_Rec)->My_Ip[0])
			if(TcpStatus[i].My_Ip[1]==((Rec_Ptr*)Tcp_Rec)->My_Ip[1])
			if(TcpStatus[i].My_Ip[2]==((Rec_Ptr*)Tcp_Rec)->My_Ip[2])
			if(TcpStatus[i].My_Ip[3]==((Rec_Ptr*)Tcp_Rec)->My_Ip[3])
			if(Tcp_Packed.Tcp.DestPort==TcpStatus[i].My_Port)
			if(Tcp_Packed.Tcp.SourcePort==TcpStatus[i].Dest_Port)
			{
				break;
			}
		}
	}
	if(i<MAX_TCP_LINKS)
	{
		switch (TcpStatus[i].State)
		{
 			case 1:    //Tcp_Listen(i);
            			break;
 			case 2:    Tcp_SYN_Rec(i);
            			break;
			case 3:    Tcp_SYN_Sent(i);
            			break;
			case 4:    Tcp_Established(i);
            			break;
            		case 5:	   TCP_FIN_WAIT1(i);    
				break;  
			case 6:	   TCP_FIN_WAIT2(i);
				break;          		
 			case 8:    //Tcp_Close_Wait();
            			break;
 			case 9:    Tcp_Last_Ack(i);//关闭
            			break;
 	
		}
		return (2);
	}
	else
	{
		for(i=0;i<MAX_TCP_LINKS;i++)
		{
			if(TcpStatus[i].My_Ip[0]==((Rec_Ptr*)Tcp_Rec)->My_Ip[0])
			if(TcpStatus[i].My_Ip[1]==((Rec_Ptr*)Tcp_Rec)->My_Ip[1])
			if(TcpStatus[i].My_Ip[2]==((Rec_Ptr*)Tcp_Rec)->My_Ip[2])
			if(TcpStatus[i].My_Ip[3]==((Rec_Ptr*)Tcp_Rec)->My_Ip[3])
			if(Tcp_Packed.Tcp.DestPort==TcpStatus[i].My_Port)
			if(TcpStatus[i].State==TCP_STATE_LISTEN)
			{
				break;
			}
		}
		if(i<MAX_TCP_LINKS)
		{
			TcpStatus[i].Dest_Ip[0]=((Rec_Ptr*)Tcp_Rec)->ip[0];
			TcpStatus[i].Dest_Ip[1]=((Rec_Ptr*)Tcp_Rec)->ip[1];
			TcpStatus[i].Dest_Ip[2]=((Rec_Ptr*)Tcp_Rec)->ip[2];
			TcpStatus[i].Dest_Ip[3]=((Rec_Ptr*)Tcp_Rec)->ip[3];
			Tcp_Listen(i);
			return (2);
		}
		else
		{
			
			Tcp_Packed.TcpDHead.TotalLen=0x14;
			Tcp_Packed.TcpDHead.rev=0;
			Tcp_Packed.TcpDHead.NextProtocal=6;
			Tcp_Packed.TcpDHead.SourceIp[0]=((Rec_Ptr*)Tcp_Rec)->My_Ip[0];
			Tcp_Packed.TcpDHead.SourceIp[1]=((Rec_Ptr*)Tcp_Rec)->My_Ip[1];
			Tcp_Packed.TcpDHead.SourceIp[2]=((Rec_Ptr*)Tcp_Rec)->My_Ip[2];
			Tcp_Packed.TcpDHead.SourceIp[3]=((Rec_Ptr*)Tcp_Rec)->My_Ip[3];
			Tcp_Packed.TcpDHead.DestId[0]=((Rec_Ptr*)Tcp_Rec)->ip[0];
			Tcp_Packed.TcpDHead.DestId[1]=((Rec_Ptr*)Tcp_Rec)->ip[1];
			Tcp_Packed.TcpDHead.DestId[2]=((Rec_Ptr*)Tcp_Rec)->ip[2];
			Tcp_Packed.TcpDHead.DestId[3]=((Rec_Ptr*)Tcp_Rec)->ip[3];
			
#ifdef Little_End
			Tcp_Packed.TcpDHead.TotalLen=0x1400;
			Tcp_Packed.Tcp.SourcePort=swap_int16(Tcp_Packed.Tcp.SourcePort);
			Tcp_Packed.Tcp.DestPort=swap_int16(Tcp_Packed.Tcp.DestPort);
#endif			
			Tcp_Packed.Tcp.Crc=Tcp_Packed.Tcp.DestPort;
			Tcp_Packed.Tcp.DestPort=Tcp_Packed.Tcp.SourcePort;
			Tcp_Packed.Tcp.SourcePort=Tcp_Packed.Tcp.Crc;
			Tcp_Packed.Tcp.Crc=0;
			Tcp_Packed.Tcp.AckNum=Tcp_Packed.Tcp.SeqNum+1;
#ifdef Little_End
			Tcp_Packed.Tcp.AckNum=swap_int32(Tcp_Packed.Tcp.AckNum);
#endif	
			
			Tcp_Packed.Tcp.SeqNum=0;
			Tcp_Packed.Tcp.offset=0x50;
			Tcp_Packed.Tcp.control=0x14;
			Tcp_Packed.Tcp.window=0;
			Tcp_Packed.Tcp.urg=0;
			Tcp_Packed.Tcp.Crc=CheckSumUdp((uint8 *)&Tcp_Packed,32);
#ifdef Little_End
			Tcp_Packed.Tcp.Crc=swap_int16(Tcp_Packed.Tcp.Crc);
#endif	
			TxdData.STPTR=NULL;
			TxdData.length=20;
			TxdData.DAPTR=(uint8 *)&Tcp_Packed.Tcp;
			Send_Ip_Frame	
			(
				&TxdData,
				Tcp_Packed.TcpDHead.DestId,
				Tcp_Packed.TcpDHead.SourceIp,
				6
			);
			return (1);
		}
		
	}
}
void TcpHeadHandle(uint8 num ,uint16 Tcp_headlength_Control)
{
	TcpStatus[num].TcpHeadUint8[0]=(TcpStatus[num].My_Port&0xff00)>>8;
	TcpStatus[num].TcpHeadUint8[1]=TcpStatus[num].My_Port&0x00ff;
	TcpStatus[num].TcpHeadUint8[2]=(TcpStatus[num].Dest_Port&0xff00)>>8;
	TcpStatus[num].TcpHeadUint8[3]=TcpStatus[num].Dest_Port&0x00ff;
	TcpStatus[num].TcpHeadUint8[4]=(TcpStatus[num].SenPassSeq&0xff000000)>>24;
	TcpStatus[num].TcpHeadUint8[5]=(TcpStatus[num].SenPassSeq&0x00ff0000)>>16;
	TcpStatus[num].TcpHeadUint8[6]=(TcpStatus[num].SenPassSeq&0x0000ff00)>>8;
	TcpStatus[num].TcpHeadUint8[7]=(TcpStatus[num].SenPassSeq&0x000000ff);
	TcpStatus[num].TcpHeadUint8[8]=(TcpStatus[num].SenPassAck&0xff000000)>>24;
	TcpStatus[num].TcpHeadUint8[9]=(TcpStatus[num].SenPassAck&0x00ff0000)>>16;
	TcpStatus[num].TcpHeadUint8[10]=(TcpStatus[num].SenPassAck&0x0000ff00)>>8;
	TcpStatus[num].TcpHeadUint8[11]=(TcpStatus[num].SenPassAck&0x000000ff);
	TcpStatus[num].TcpHeadUint8[12]=(Tcp_headlength_Control&0xff00)>>8;
	TcpStatus[num].TcpHeadUint8[13]=Tcp_headlength_Control&0x00ff;
	TcpStatus[num].TcpHeadUint8[14]=(TcpStatus[num].Snd_Window&0xff00)>>8;
	TcpStatus[num].TcpHeadUint8[15]=TcpStatus[num].Snd_Window&0x00ff;
	TcpStatus[num].TcpHeadUint8[16]=0;
	TcpStatus[num].TcpHeadUint8[17]=0;
	TcpStatus[num].TcpHeadUint8[18]=0;
	TcpStatus[num].TcpHeadUint8[19]=0;
	TcpStatus[num].TcpDHeadUint8[0]=0;
	TcpStatus[num].TcpDHeadUint8[1]=6;
	TcpStatus[num].TcpDHeadUint8[4]=TcpStatus[num].My_Ip[0];
	TcpStatus[num].TcpDHeadUint8[5]=TcpStatus[num].My_Ip[1];
	TcpStatus[num].TcpDHeadUint8[6]=TcpStatus[num].My_Ip[2];
	TcpStatus[num].TcpDHeadUint8[7]=TcpStatus[num].My_Ip[3];
	TcpStatus[num].TcpDHeadUint8[8]=TcpStatus[num].Dest_Ip[0];
	TcpStatus[num].TcpDHeadUint8[9]=TcpStatus[num].Dest_Ip[1];
	TcpStatus[num].TcpDHeadUint8[10]=TcpStatus[num].Dest_Ip[2];
	TcpStatus[num].TcpDHeadUint8[11]=TcpStatus[num].Dest_Ip[3];
}