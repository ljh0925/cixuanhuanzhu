#ifdef ARP_GLOBALS
#define ARP_EXT
#else
#define ARP_EXT extern 
#endif 

//ARP_EXT unsigned char IN_CODE ARP_REP_MAC[6];
ARP_EXT uint8 EX_RAM Ping_Ip_Address[4];
ARP_EXT uint8 EX_RAM Ping_IP_TTL;
ARP_EXT uint8 EX_RAM Gateway_MAC[6];
ARP_EXT uint8 EX_RAM Ping_MAC[6];
ARP_EXT uint8 EX_RAM Gateway_IP_TTL;
ARP_EXT uint8 IN_CODE My_Ip_Address[4];
//ARP_EXT uint8 IN_CODE My_Gateway_Ip_Address[4];
ARP_EXT uint8 IN_CODE MY_IP_MARK[4];
ARP_EXT EX_RAM struct a_t ARP_TERM[MAX_ARP_TERM];
//ARP_EXT 
ARP_EXT uint8  Arp_Answer(uint8 EX_RAM * ARP_REC_PTR,uint8 num);// USE_XDATA keilreentrant ;
ARP_EXT uint8  REC_ARP_REQ(uint8 EX_RAM * ARP_REC_REQ_PTR,uint8 num);// USE_XDATA keilreentrant ;
ARP_EXT void Arp_Request(uint8 * ip_address,uint8 num);// USE_XDATA keilreentrant ;
ARP_EXT void PROCESS_ARP_REC(uint8 EX_RAM * ARP_PTR,uint8 num);// USE_XDATA keilreentrant ;
ARP_EXT void Initial_arp(void);//USE_XDATA keilreentrant ;
