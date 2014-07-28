/**********************************************************************
**º¯ÊýÔ­ÐÍ:	    unsigned int CheckSum(unsigned int xdata *check,unsigned int length)
**Èë¿Ú²ÎÊý:º	check	:	±»Ð£ÑéÊý¾ÝÆðÊ¼µØÖ·
				length	:	±»Ð£ÑéÊý¾Ý³¤¶È£¬µ¥Î»×Ö
**³ö¿Ú²ÎÊý:		ÎÞ
**·µ »Ø Öµ:		~((sum)&0xffff))		       
**Ëµ    Ã÷:		¼ÆËãÐ£ÑéºÍ 
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
if(length&0x01)//³¤¶ÈÎªÆæÊý¸öÊ±£¬Òª½øÐÐ¸Ã²Ù×÷
	{
	sum = sum + ((*check)&0xff00);
	}
sum = (sum&0xffff) + ((sum>>16)&0xffff);//¸ß16Î»ºÍµÍ16Î»Ïà¼Ó
if(sum & 0xffff0000)
	{//±íÊ¾ÓÐ½øÎ»
	sum++;
	}
return ( (unsigned int)(~((sum)&0xffff))  );
}

/**********************************************************************
**º¯ÊýÔ­ÐÍ£º    unsigned char  VerifyIpHeadCrc()
**Èë¿Ú²ÎÊý:º	ÎÞ
**³ö¿Ú²ÎÊý:		ÎÞ
**·µ »Ø Öµ£º	0   :  	Ð£Ñé´íÎó
**				1   :	Ð£ÑéÕýÈ·       
**Ëµ    Ã÷£º	¶ÔIPÍ·½øÐÐÐ£Ñé 
************************************************************************/
unsigned char  VerifyIpHeadCrc()
{
unsigned int crc;
crc = CheckSum(&RxdNetBuff.IpPacket.IpPacket[0],(RxdNetBuff.IpFrame.VerandIphLen&0x0f)*4);//1Î»´ú±í1×Ö½Ú
if(crc == 0)
	{
	return (1);
	}
return(0);
}
//===============================================================
/**********************************************************************
**º¯ÊýÔ­ÐÍ:º    unsigned char  VerifyTcpCrc()
**Èë¿Ú²ÎÊý:º	ÎÞ
**³ö¿Ú²ÎÊý:		ÎÞ
**·µ »Ø Öµ:º	0   :º  Ð£Ñé´íÎó
**				1   :º	Ð£ÑéÕýÈ·       
**Ëµ    Ã÷:º	¶ÔTCPÍ·½øÐÐÐ£Ñé 
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
**º¯ÊýÔ­ÐÍ£º    void 	CreateIpHeadCrc()
**Èë¿Ú²ÎÊý:º	ÎÞ
**³ö¿Ú²ÎÊý:		ÎÞ
**·µ »Ø Öµ£º	crc £º  16Î»µÄÐ£ÑéºÍ        
**Ëµ    Ã÷£º	²úÉúIPÊ×²¿µÄÐ£ÑéºÍ 
************************************************************************/
uint  CreateIpHeadCrc()
{
uchar i;
CrcTemp.dwords = 0;
for(i=9;i<19;i++)              				//IPÊ×²¿¹²20×Ö½Ú
	{
 	CrcTemp.dwords = CrcTemp.dwords + TxdNetBuff.words.wordbuf[i];
	}
while(CrcTemp.words.high>0) 
	{
    CrcTemp.dwords = (ulong)(CrcTemp.words.high + CrcTemp.words.low);
    }
CrcTemp.words.low = 0xffff - CrcTemp.words.low;  //È¡·´
return(CrcTemp.words.low);
}
//================================================================
/**********************************************************************
**º¯ÊýÔ­ÐÍ£º    void 	CreateIcmpCrc( )
**Èë¿Ú²ÎÊý:º	ÎÞ
**³ö¿Ú²ÎÊý:		ÎÞ
**·µ »Ø Öµ£º	crc £º  16Î»µÄÐ£ÑéºÍ        
**Ëµ    Ã÷£º	²úÉúICMPÊý¾Ý¶ÎµÄÐ£ÑéºÍ 
************************************************************************/
uint  CreateIcmpCrc()//
{
uchar i;
CrcTemp.dwords=0;
for(i=19;i<39;i++)				//ÓÃÓÚ²éÑ¯µÄICMP±¨ÎÄ¹²40×Ö½Ú
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
**º¯ÊýÔ­ÐÍ£º    unsigned char  VerifyUdpCrc()
**Èë¿Ú²ÎÊý:º	ÎÞ
**³ö¿Ú²ÎÊý:		ÎÞ
**·µ »Ø Öµ£º	0   £º  Ð£Ñé´íÎó
**		1   £º	Ð£ÑéÕýÈ·       
**Ëµ    Ã÷£º	¶ÔUDPÍ·½øÐÐÐ£Ñé 
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
**º¯ÊýÔ­ÐÍ£º    void 	CreateTcpCrc()
**Èë¿Ú²ÎÊý:º	ÎÞ
**³ö¿Ú²ÎÊý:		ÎÞ
**·µ »Ø Öµ£º	crc £º  16Î»µÄÐ£ÑéºÍ       
**Ëµ    Ã÷£º	²úÉúTCPÊý¾Ý¶ÎµÄÐ£ÑéºÍ 
************************************************************************/
uint CreateTcpCrc()
{
uint crc;
crc = CheckSum(&TxdNetBuff.IpPacket.IpPacket[4],TxdNetBuff.IpFrame.Crc+12);
return (crc);
}