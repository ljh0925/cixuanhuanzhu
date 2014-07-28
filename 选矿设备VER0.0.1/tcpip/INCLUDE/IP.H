#ifdef IP_GLOBALS
#define IP_EXT 
#else
#define IP_EXT extern 
#endif 

#ifdef Big_End
union   w {  uint32 	dwords;
             struct {uint16 	high ;uint16 	 low;}words;
	  	     struct {uint8 	byte3;uint8  byte2;uint8 	byte1;uint8 	byte0;}bytes;
          };
#endif

#ifdef Little_End
union   w {  uint32 	dwords;
             struct {uint16 	 low;uint16 	 high;}words;
	  	     struct {uint8 	byte0;uint8  byte1;uint8 	byte2;uint8 	byte3;}bytes;
          };
#endif

typedef struct { 
            uint8 	VerandIphLen;   /*版本与头长度*/
            uint8 	ServerType;     /*服务类型*/
            uint16 	TotalLen;       /*总长度*/
            uint16 	FrameIndex;     /*IP帧序号*/
            uint16 	Segment;        /*分段标志*/
            uint8 	ttl;            /*生存时间*/
            uint8 	NextProtocal;   /*下一层协议*/
            uint16 	Crc;            /*校验和*/
            uint8 	SourceIp[4];    /*源IP*/
            uint8 	DestId[4];      /*目的IP*/
}eip;

EX_RAM union ip_rc	{
				eip e_ip;
			 	struct 	{uint16 	 	wordbuf[10];}words;
			};
typedef struct{ uint8 	*RecDataPtr;
				uint16			length;
				uint8			num;
				uint8 			ip[4];
				uint8			My_Ip[4];
				}Rec_Ptr;
#define Q_Max_Size 20
//IP_EXT Rec_Ptr	RECQ[MAX_REC_BUFF];
IP_EXT void * 	RecIcmpQ[Q_Max_Size];
IP_EXT void * 	RecUdpQ[Q_Max_Size];
IP_EXT void * 	RecTcpQ[Q_Max_Size];

IP_EXT OS_EVENT  		*RecIcmpQFlag;
IP_EXT OS_EVENT  		*RecTcpQFlag;
IP_EXT OS_EVENT  		*RecUdpQFlag;

IP_EXT OS_EVENT  		*SendFlag; 

IP_EXT  uint16  CreateIpHeadCrc(uint8 * Ip);// USE_XDATA ;
IP_EXT uint8 Send_Ip_Frame(struct _pkst *TxdData,uint8 * de_ip,uint8 * so_ip,uint8 PROTOCOL);// USE_XDATA  keilreentrant ;//keilreentrant ;

IP_EXT uint8 IP_PROCESS(uint8 * RecData,uint8 num);// USE_XDATA keilreentrant ;
