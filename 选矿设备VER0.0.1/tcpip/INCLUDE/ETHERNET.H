#ifdef ETHERNET_GLOBALS
#define ETHERNET_EXT
#else
#define ETHERNET_EXT extern 
#endif 
#define IP_PACKED	0x01
#define ARP_PACKED	0x02
#define MAX_NET_PORT	0x01
typedef struct  {  
                    uint8 	DestMacId[6];    	/*目的网卡地址*/
                    uint8 	SourceMacId[6];   	/*源网卡地址*/
                    uint16 	NextProtocal;       /*下一层协议*/
                 } ipethernet;

typedef struct  { 
					uint8 IP[4];
				} 	IP_NUMBER;
typedef struct 	{
			uint8	My_Ip[4];
			uint8	My_Gateway[4];
			uint8	My_Ip_Mark[4];
			uint8	My_Mac[6];
			uint8	My_Gateway_Mac[6];
			uint8	Gateway_IP_TTL;
			struct a_t ARP_TERM[MAX_ARP_TERM];
		} NetP;
ETHERNET_EXT NetP NetPort[MAX_NET_PORT];
//ETHERNET_EXT uint8 IN_CODE MY_MAC[6];

ETHERNET_EXT uint8 Send_ethernet_Frame	(
							struct _pkst *TxdData,//结构指针
							uint8 * de_mac,			//对方的MAC地址指针
							uint8 PROTOCOL,			//IP协议或ARP协议
							uint8	num
									)  ;
ETHERNET_EXT uint8 Send_Ip_To_LLC(struct _pkst *TxdData,uint8 * de_ip,uint8 num); //USE_XDATA keilreentrant  ;
ETHERNET_EXT uint8 Rec_Ethernet_Packed(uint8 * RecData,uint8 num) ;//USE_XDATA keilreentrant ;
ETHERNET_EXT void SetNetPort(void);