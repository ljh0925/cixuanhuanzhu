#ifdef UDP_GLOBALS
#define UDP_EXT
#else
#define UDP_EXT extern 
#endif 

struct  udp_dhead{  
				uint8	rev; 
			   	uint8	NextProtocal;
               	uint16 	TotalLen;         	   /*У���,����αͷ��,udpͷ��,����*/
             	uint8 	SourceIp[4];  	   /*Դ�˿�*/
               	uint8 	DestId[4];     	   /*Ŀ�Ķ˿�*/
			   };
struct  udp_data{   
               uint16 	SourcePort; 	   /*Դ�˿�*/
               uint16 	DestPort;    	   /*Ŀ�Ķ˿�*/
               uint16	length;
               uint16 	Crc;        	   /*У���,����αͷ��,udpͷ��,����*/
			   //uint8 	*udpdata;     /*udp����*/
//               uint8 	udpdata[1478];     /*udp����*/
             };
typedef struct  {
				struct udp_dhead 	UdpDHead;
				struct udp_data		Udp;			
				}udp_send;
typedef struct  {
				uint8 	DestIp[4];     	   /*Ŀ�Ķ˿�*/
               	uint16 	SourcePort; 	   /*Դ�˿�*/
               	uint16 	DestPort;    	   /*Ŀ�Ķ˿�*/
               	uint16	length;
				uint8 	*data_ptr;			
				}udp_socket;

UDP_EXT uint8* Udp_Process(uint8 EX_RAM * Udp_Rec) USE_XDATA keilreentrant ;
UDP_EXT void Udp_Send_Packed() USE_XDATA keilreentrant ;//udp_send Udp_Temp)
//UDP_EXT uint16 CheckSum(uint16 EX_RAM *check,uint16 length) USE_XDATA keilreentrant ;
