#define IN_RAM 	
#define EX_RAM 			
#define IN_CODE 	
#define USE_DATA 	

struct 	_pkst{
					struct _pkst 	*STPTR;
					unsigned int 	length;
					unsigned char	*DAPTR;
				};//typedef 

struct 	a_t{
					unsigned char	TTL;
					unsigned char 	IP_NUM[4];
					unsigned char	MAC_NUM[6];
				};

#define MAX_ARP_TERM	50
#define	MAX_TCP_DATA	1460
#define MAX_TCP_LINKS   4
#define keilreentrant 


#define Little_End

#include "..\include\hardware.h"

#include "..\include\ethernet.h"
#include "..\include\ARP.h"

//#include "mcu_hardware.h"
#include "..\include\ip.h"
#include "..\include\icmp.h"
#include "..\include\udp.h"
#include "..\include\tcp.h"
#include "..\include\zlg_socket.h"

/************** first Net Port Config **************************/
#define My_Mac_1ID 		{0x52,0x54,0x4c,0x19,0xf7,0x42}//将RTL8019的物理地址存储在程序空间?
#define IP_MARK_1ID 		{255,255,255,0} 		//255.255.255.0,子网掩码
#define MY_IP_1ID       	{192,168,0,174}
#define MY_GATEWAY_1ID  	{192,168,0,1}
/**************End Net Port Config **************************/
#if (MAX_NET_PORT>=2)
/************** first Net Port Config **************************/
#define My_Mac_2ID 		{0x52,0x54,0x4c,0x19,0xf8,0x32}//将RTL8019的物理地址存储在程序空间?
#define IP_MARK_2ID 		{255,255,255,0} 		//255.255.255.0,子网掩码
#define MY_IP_2ID       	{192,168,0,174}
#define MY_GATEWAY_2ID  	{192,168,0,1}
/**************End Net Port Config **************************/
#endif