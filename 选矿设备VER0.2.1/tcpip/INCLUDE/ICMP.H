#ifdef ICMP_GLOBALS
#define ICMP_EXT 
#else
#define ICMP_EXT extern 
#endif 
typedef struct  {/*包含在IP包中，是IP的上层为0X01的应用*/
                uint8 	type;  		       	/*0X08 PING请求 0X00 PING应答*/
                uint8 	option; 		   	/*0X00 PING*/
                uint16 	Crc;
                uint16 	id;
                uint16 	seq;
                uint8 	icmpdata[32];
              }icmp;
EX_RAM union icmp_rc	{
				icmp e_icmp;
			 	struct 	{uint16 	 	wordbuf[20];}words;
			};

typedef struct {	//ipethernet 	etf;
				union ip_rc			ipf;
				union icmp_rc		icmpf;
			}icmppro;

//ICMP_EXT EX_RAM union icmp_rc	IcmpPacked;
ICMP_EXT EX_RAM union icmp_rc IcmpPacked;

ICMP_EXT uint16  CreateIcmpCrc(union icmp_rc) ;//USE_XDATA keilreentrant ;//
ICMP_EXT uint8 send_icmp_packed( uint8  * ICMPSENDPTR);// USE_XDATA keilreentrant ;
ICMP_EXT void icmp_process(uint8  * ICMPSENDPTR,uint8 num) ;//USE_XDATA keilreentrant ;
ICMP_EXT void Ping_Precess(uint8 * de_ip,uint8 * so_ip);// USE_XDATA keilreentrant ;
ICMP_EXT void Icmp_Send(uint8 type,uint8 option,uint8 *so_ip,uint8 *de_ip, uint8 * REC_FRAME );