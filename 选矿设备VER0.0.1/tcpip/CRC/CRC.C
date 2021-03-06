/**********************************************************************
**函数原型:	    unsigned int CheckSum(unsigned int xdata *check,unsigned int length)
**入口参数:�	check	:	被校验数据起始地址
				length	:	被校验数据长度，单位字
**出口参数:		无
**返 回 值:		~((sum)&0xffff))		       
**说    明:		计算校验和 
************************************************************************/
//=================================================================================
unsigned int CheckSum(unsigned int xdata *check,unsigned int length)
{
unsigned long sum=0;
unsigned int i;
for (i=0;i<(length)/2;i++)
	{
	sum += *check++;
	}
if(length&0x01)//长度为奇数个时，要进行该操作
	{
	sum = sum + ((*check)&0xff00);
	}
sum = (sum&0xffff) + ((sum>>16)&0xffff);//高16位和低16位相加
if(sum & 0xffff0000)
	{//表示有进位
	sum++;
	}
return ( (unsigned int)(~((sum)&0xffff))  );
}

/**********************************************************************
**函数原型：    unsigned char  VerifyIpHeadCrc()
**入口参数:�	无
**出口参数:		无
**返 回 值：	0   :  	校验错误
**				1   :	校验正确       
**说    明：	对IP头进行校验 
************************************************************************/
unsigned char  VerifyIpHeadCrc()
{
unsigned int crc;
crc = CheckSum(&RxdNetBuff.IpPacket.IpPacket[0],(RxdNetBuff.IpFrame.VerandIphLen&0x0f)*4);//1位代表1字节
if(crc == 0)
	{
	return (1);
	}
return(0);
}
//===============================================================
/**********************************************************************
**函数原型:�    unsigned char  VerifyTcpCrc()
**入口参数:�	无
**出口参数:		无
**返 回 值:�	0   :�  校验错误
**				1   :�	校验正确       
**说    明:�	对TCP头进行校验 
************************************************************************/
unsigned char  VerifyTcpCrc()
{
unsigned int crc;
RxdNetBuff.IpFrame.ttl=0;
RxdNetBuff.IpFrame.Crc=RxdNetBuff.IpFrame.TotalLen-(RxdNetBuff.IpFrame.VerandIphLen&0x0f)*4;
crc = CheckSum(&RxdNetBuff.IpPacket.IpPacket[4],RxdNetBuff.IpFrame.Crc+12);
if(crc == 0)
	{
	 return (1);
	}
return(0);
}
//================================================================

/**********************************************************************
**函数原型：    void 	CreateIpHeadCrc()
**入口参数:�	无
**出口参数:		无
**返 回 值：	crc ：  16位的校验和        
**说    明：	产生IP首部的校验和 
************************************************************************/
uint  CreateIpHeadCrc()
{
uchar i;
CrcTemp.dwords = 0;
for(i=9;i<19;i++)              				//IP首部共20字节
	{
 	CrcTemp.dwords = CrcTemp.dwords + TxdNetBuff.words.wordbuf[i];
	}
while(CrcTemp.words.high>0) 
	{
    CrcTemp.dwords = (ulong)(CrcTemp.words.high + CrcTemp.words.low);
    }
CrcTemp.words.low = 0xffff - CrcTemp.words.low;  //取反
return(CrcTemp.words.low);
}
//================================================================
/**********************************************************************
**函数原型：    void 	CreateIcmpCrc( )
**入口参数:�	无
**出口参数:		无
**返 回 值：	crc ：  16位的校验和        
**说    明：	产生ICMP数据段的校验和 
************************************************************************/
uint  CreateIcmpCrc()//
{
uchar i;
CrcTemp.dwords=0;
for(i=19;i<39;i++)				//用于查询的ICMP报文共40字节
	{
	CrcTemp.dwords = CrcTemp.dwords + TxdNetBuff.words.wordbuf[i];
	}
while(CrcTemp.words.high>0) 
	{
    CrcTemp.dwords = (ulong)(CrcTemp.words.high+CrcTemp.words.low);
    }
CrcTemp.words.low = 0xffff - CrcTemp.words.low;
return(CrcTemp.words.low);
}
//=================================================================
/**********************************************************************
**函数原型：    unsigned char  VerifyUdpCrc()
**入口参数:�	无
**出口参数:		无
**返 回 值：	0   ：  校验错误
**		1   ：	校验正确       
**说    明：	对UDP头进行校验 
************************************************************************/
unsigned char  VerifyUdpCrc()
{
unsigned int crc;

RxdNetBuff.IpFrame.ttl = 0;
RxdNetBuff.IpFrame.Crc = RxdNetBuff.IpFrame.TotalLen - (RxdNetBuff.IpFrame.VerandIphLen&0x0f)*4;
crc = CheckSum(&RxdNetBuff.IpPacket.IpPacket[4],RxdNetBuff.IpFrame.Crc+12);
if(crc==0)
	{
	 return (1);
 	}
return(0);
}
//==================================================================
/**********************************************************************
**函数原型：    void 	CreateTcpCrc()
**入口参数:�	无
**出口参数:		无
**返 回 值：	crc ：  16位的校验和       
**说    明：	产生TCP数据段的校验和 
************************************************************************/
uint CreateTcpCrc()
{
uint crc;
crc = CheckSum(&TxdNetBuff.IpPacket.IpPacket[4],TxdNetBuff.IpFrame.Crc+12);
return (crc);
}