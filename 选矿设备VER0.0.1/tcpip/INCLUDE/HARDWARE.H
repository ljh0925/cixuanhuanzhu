#ifdef HARDWARE_GLOBALS
#define HARDWARE_EXT
#else
#define HARDWARE_EXT extern 
#endif 

#define MAX_REC_BUFF 	5
union  REC_BUFF_UNION
	{
		uint32	Dwords[384];
		uint16	words[768];
		uint8 	bytes[1536];
	};

//HARDWARE_EXT unsigned char IN_CODE My_Ip_Address[4];
//HARDWARE_EXT EX_RAM ipethernet ethernet_head;
HARDWARE_EXT union  REC_BUFF_UNION	REC_BUFF[MAX_REC_BUFF];
HARDWARE_EXT uint16 swap_int16(uint16 temp);
HARDWARE_EXT uint32 swap_int32(uint32 temp);
HARDWARE_EXT uint32 Char4ToInt32(uint8 * temp);
HARDWARE_EXT uint16 Char2ToInt16(uint8 * temp);
HARDWARE_EXT void page(uint8 pagenumber) USE_DATA ;
HARDWARE_EXT void SetMacID(uint8 IN_CODE * mac_ptr) USE_DATA ;
HARDWARE_EXT void Send_Packet(struct _pkst *TxdData) USE_DATA ;// 
HARDWARE_EXT uint8 Rec_Packet(void) USE_DATA ;
HARDWARE_EXT void InitNic(uint8 num) ;
//HARDWARE_EXT unsigned char rec_data() ;//USE_DATA ;
