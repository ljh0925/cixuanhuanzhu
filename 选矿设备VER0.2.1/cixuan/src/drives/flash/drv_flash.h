/******************************************************************************
		 	大连金德姆电子有限公司
			(Copyright 2005)	All rights riserved.

文件名	：taskmemory.h
项目名称：小额消费机
功能概要：cpu使用philips 的lpc2214 
			操作系统 uCOS-II
创建人	：马骞 (myron)
创建日期：2005.4.10
******************************************************************************/
#ifndef __DRV_FLASH_H__
#define __DRV_FLASH_H__


#define FLASH_OP_RETRY_TIMES	4				//flash写入重试次数



#define  FLASH_RST0_SetLow()  		(IO0CLR = BIT10)
#define  FLASH_RST0_SetHigh()		 	(IO0SET = BIT10)

#define  FLASH_CS0_SetLow()  		(IO0CLR = BIT12)
#define  FLASH_CS0_SetHigh()		 	(IO0SET = BIT12)

#define  FLASH_WP0_SetLow()  		(IO0CLR = BIT11)
#define  FLASH_WP0_SetHigh()		 	(IO0SET = BIT11)

#define  FLASH_SCK0_SetLow()  		(IO0CLR = BIT13)
#define  FLASH_SCK0_SetHigh()		 	(IO0SET = BIT13)

#define  FLASH_SI0_SetLow()  		(IO0CLR = BIT15)
#define  FLASH_SI0_SetHigh()		 	(IO0SET = BIT15)


//flash  命令定义
#define		MAX_PAGE_SIZE	528		//flash每页字节最大值	0~527
#define		USED_PAGE_SIZE	512		//flash每页实际使用值	0~511
// #define		MAXPAGENUM	4096		//一共多少页
#define		MAXPAGENUM	8192		//一共多少页
#define		REC_PER_PAGE	(USED_PAGE_SIZE / (REC_LEN + 2))		//每页多少条记录
#define		BLACK_PER_PAGE	128										//每页多少条黑名单


#define FADDR_REC_NUM	10		//记录指针存储在第10页
#define FADDR_BLACK		100		//黑名单存储在第100-200页
#define FADDR_BLACK2	200		//备份黑名单存储在第200-300页
#define FADDR_REC		500		//记录存储在第500页

#define FADDR_REC_FLASH2	20		//FLASH2记录存储在第20页
#define FADDR_DRIVER_LOG_PTR	8		//司机记录指针
#define FADDR_FP	10					//打印数据存储在第10页
#define FADDR_TODAY_TOTAL	12			//日总计
#define FADDR_TOTAL	14					//总计
#define FADDR_BANCI	16					//班次
#define FADDR_DRIVER1_TOTAL	18			//司机1总计
#define FADDR_DRIVER2_TOTAL	20			//司机2总计
#define FADDR_DRIVER3_TOTAL	22			//司机3总计
#define FADDR_GPS_MODIFY_PARAM	24		//GPS更新计价器参数

#define DRIVER1_LOG_ADDR 100			//司机1记录
#define DRIVER2_LOG_ADDR (INT16U)(DRIVER1_LOG_ADDR + MAX_BUSINESS_LOG_NUM)
#define DRIVER3_LOG_ADDR (INT16U)(DRIVER2_LOG_ADDR + MAX_BUSINESS_LOG_NUM)

#define MAX_BUSINESS_LOG_NUM 	(INT16U)120			//最大120条运营日志

#define GPS_LOG_ADDR (INT16U)(DRIVER3_LOG_ADDR + MAX_BUSINESS_LOG_NUM)	//司机1记录
//#define  FLASH_SCK1_SetLow()        (IO0CLR = BIT17)
//#define  FLASH_SCK1_SetHigh()           (IO0SET = BIT17)
//
//#define  FLASH_SI1_SetLow()         (IO0CLR = BIT19)
//#define  FLASH_SI1_SetHigh()            (IO0SET = BIT19)
//
//#define  FLASH_RST1_SetLow()        (IO0CLR = BIT21)
//#define  FLASH_RST1_SetHigh()           (IO0SET = BIT21)
//
//#define  FLASH_CS1_SetLow()         (IO0CLR = BIT22)
//#define  FLASH_CS1_SetHigh()            (IO0SET = BIT22)
//
//#define  FLASH_WP1_SetLow()         (IO0CLR = BIT23)
//#define  FLASH_WP1_SetHigh()            (IO0SET = BIT23)





//flash  命令定义
#define		MAX_PAGE_SIZE	528		//flash页最大值;	页号为0~527
#define		USED_PAGE_SIZE	512		//flash页实际使用值;	页号为0~511
// #define		MAXPAGENUM	4096		//一共多少页
#define		MAXPAGENUM	8192		//一共多少页
// #define		USED_PAGE_NUM	1024	//记录区占用多少页


#define MAIN_FAT_FRT_STAT_PAGE		6
#define BAK_FAT_FRT_STAT_PAGE		7


// 读状态字及页/块擦除
#define		READ_STATUSREAD			0xD7	// Status Register Read
#define		PAGE_ERASE				0x81	// Page Erase
#define		BLOCK_ERASE			0x50	// Block Erase
// Buffer   读写命令
#define		BUFFER1_READ			0xD4	// Read Data From Buffer 1
#define		BUFFER2_READ			0xD6	// Read Data From Buffer 2
#define		BUFFER1_WRITE		    	0x84	// Write Data To Buffer 1
#define		BUFFER2_WRITE			0x87	// Write Data To Buffer 2
// Flash 与Buffer   数据交换命令

#define		MAIN_COMPARE_BUFFER1	0x60 //主存与buffer1比较
#define		MAIN_COMPARE_BUFFER2	0x61 //主存与buffer2比较
#define		MAIN_TO_BUFFER1		0x53	// Main Memory Page Transfer Buffer 1
#define		MAIN_TO_BUFFER2		0x55	// Main Memory Page Transfer Buffer 2
#define		SELF_ERASE_BUFFER1_WRITE_MAIN		0x83	// 带自擦除的bufeer1写入主存
#define		SELF_ERASE_BUFFER2_WRITE_MAIN		0x86	// 带自擦除的bufeer2写入主存
#define		NO_ERASE_BUFFER1_WRITE_MAIN	    	0x88	// 不带自擦除的bufeer1写入主存
#define		NO_ERASE_BUFFER2_WRITE_MAIN 		0x89	// 不带自擦除的bufeer2写入主存
// 通过Buffer  对Flash 进行数据读写
#define		MAIN_BUFFER1_WRITE		0x82	// Main Memory Page Program Through Buffer 1(自擦除)
#define		MAIN_BUFFER2_WRITE		0x85	// Main Memory Page Program Through Buffer 2(自擦除)
#define		MAIN_PAGE_READ				0xD2	// 页读
#define		CONTINOUS_READ			0xE8	//成片连续读（自动跨页）

// //其他
// #define START_FLASH_PAGE 	0                              			//存储记录用的起始页
// #define END_FLASH_PAGE 		(USED_PAGE_NUM - 1)                         //存储记录用的结束页
// #define ERR_FLAG_PAGE_MAIN			4095									//坏块标志的存储页
// #define ERR_FLAG_PAGE_BAK		4087									//坏块标志的存储页的备份区4095-8


/*****************************************************************
函数原型：Cal_Crc
 功能描述：计算一串数据的CRC值，采用CCITT算法
 参数描述：
 参数名称：	输入/输出？	类型		描述
 data_to_cal_crc	输入	INT8U const *		要进行CRC计算的数组的起始指针
 len				 		输入	INT16U				要进行CRC计算的数组的长度

 返  回  值：uint型的CRC值(2字节)
 作      者	：黄立国、许岩
 日      期：2004-02-04
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
*****************************************************************/
INT16U Cal_Crc(INT8U const *data_to_cal_crc, INT16U len);



/******************************************************************************
函数名称：flash_init
功能描述：flash初始化
参数描述：

返  回  值	：无
作      者	：许岩
日      期：2005-04-08
修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
void flash_init(void);



/******************************************************************************
 函数名称：FlashReset
 功能描述：复位flash
 参数名称： 输入/输出？ 类型        描述
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

 返  回  值：无

 作      者 ：许岩
 日      期：2004-11-29
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
void FlashReset (INT8U which);



/******************************************************************************
 函数名称：FlashENWP
 功能描述：始能flash的写保护
 参数名称： 输入/输出？ 类型        描述
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

 返  回  值：无

 作      者 ：许岩
 日      期：2004-11-29
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
void FlashENWP(INT8U which);



/******************************************************************************
 函数名称：FlashDISENWP
 功能描述：去掉flash的写保护
 参数名称： 输入/输出？ 类型        描述
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

 返  回  值：无

 作      者 ：许岩
 日      期：2004-11-29
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
void FlashDISENWP(INT8U which);



/******************************************************************************
 函数名称：Send_SPI_8Bit
 功能描述：向spi总线发送1字节数据
 参数名称： 输入/输出？ 类型        描述
 value        输入        INT8U      要发送的数
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

 返  回  值：无

 作      者 ：许岩
 日      期：2004-11-29
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
void Send_SPI_8Bit (INT8U value, INT8U which);



/******************************************************************************
 函数名称：Receive_SPI_8Bit
 功能描述：从spi总线接收1字节数据
 参数名称： 输入/输出？ 类型        描述
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

 返  回  值：读到的数

 作      者 ：许岩
 日      期：2004-11-29
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
INT8U Receive_SPI_8Bit(INT8U which);



/******************************************************************************
 函数名称：FlashReadStatus
 功能描述：读flash状态寄存器
 参数名称： 输入/输出？ 类型        描述
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

 返  回  值：读到的数

 作      者 ：许岩
 日      期：2004-11-29
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
INT8U FlashReadStatus(INT8U which);



/******************************************************************************
 函数名称：FlashBusy
 功能描述：看flash是否忙
 参数名称： 输入/输出？ 类型        描述
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

 返  回  值：TRUE = BUSY
			   	FALSE = DON'T BUSY

 作      者 ：许岩
 日      期：2004-11-29
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
INT8U FlashBusy(INT8U which);



/******************************************************************************
函数名称：FlashPageWrite512
功能描述：把数据(512 bytes)	写到Flash 中的某一页(先自擦除再写)
参数描述：
参数名称：	输入/输出？	类型		描述
page_address		  输入 				 INT16U 		页地址
*source_address		输出			const INT8U *		源数据(512 bytes)	存放地址		
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

作      者	：许岩
日      期：2004-09-02
修改历史：
日期		修改人		修改描述
------		---------	-------------
******************************************************************************/
void FlashPageWrite512(INT16U page_address, const INT8U *source_address, INT8U which);



/******************************************************************************
函数名称：FlashPageRead512
功能描述：读Flash 中的某一页
参数描述：
参数名称：	输入/输出？	类型		描述
page_address		  输入 				 INT16U 		页地址
*source_address		输出			INT8U *		读出来的数据(长度为512)存放地址		
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

作      者	：许岩
日      期：2004-09-02
修改历史：
日期		修改人		修改描述
------		---------	-------------
******************************************************************************/
void FlashPageRead512(INT16U page_address,INT8U *source_address, INT8U which);



/******************************************************************************
 函数名称：WriteParam
 功能描述：保存参数
 参数描述：
 参数名称：	输入/输出？	类型		描述
 
 返  回  值：无
				   
 作      者	：许岩
 日      期：2006-08-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void WriteParam(void);

/******************************************************************************
 函数名称：ReadParam
 功能描述：读取参数
 参数描述：
 参数名称：	输入/输出？	类型		描述
 
 返  回  值：无
				   
 作      者	：许岩
 日      期：2006-08-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void ReadParam(void);


/******************************************************************************
 函数名称：ReadDevState
 功能描述：读取设备状态
 参数描述：
 参数名称：	输入/输出？	类型		描述
 
 返  回  值：无
				   
 作      者	：刘及华
 日      期：2014-08-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void ReadDevState(void);
/******************************************************************************
 函数名称：WriteDevState
 功能描述：保存设备状态
 参数描述：
 参数名称：	输入/输出？	类型		描述
 
 返  回  值：无
				   
 作      者	：刘及华
 日      期：2014-08-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void SaveDevState(void);

/*****************************************************************
函数原型：Get_Record
功能描述：取指定地址的记录

参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
 rec_buf		输出	INT8U *		取回记录存放在此
 start_position	输入	INT16U		p0+2，即NewPtr，即DevStat.record_base_ptr
 rec_pointer	输入	INT16U		要取记录的指针，即DevStat.record_number

返  回  值：无
作      者：许岩
日      期：2004-10-10
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void Get_Record(INT8U *l_rec_buf, INT16U start_position, INT16U rec_pointer);

/******************************************************************************
函数名称：FlashWriteWithoutErase
功能描述：把数据写到Flash 中的某一页(无自擦除)
参数描述：
参数名称：	输入/输出？	类型		描述
page_address		  输入 				 INT16U 		页地址
offset_in_page			输入			INT16U			页内地址
*source_address		输出			const INT8U *		源数据(512 bytes)	存放地址		
len							输入				INT16U			写入长度
which        输入        INT8U      主FLASH还是从FLASH，0 或 1

作      者	：许岩
日      期：2004-09-02
修改历史：
日期		修改人		修改描述
------		---------	-------------
******************************************************************************/
void FlashWriteWithoutErase(INT16U page_address, INT16U offset_in_page, const INT8U *source_address, INT16U len, INT8U which);



/******************************************************************************
函数名称：FlashPageWrite256
功能描述：把数据(512 bytes)	写到Flash 中的某一页(先自擦除再写)
参数描述：
参数名称：	输入/输出？	类型		描述
page_address		  输入 				 INT16U 		页地址
*source_address		输出			const INT8U *		源数据(512 bytes)	存放地址		
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

作      者	：许岩
日      期：2004-09-02
修改历史：
日期		修改人		修改描述
------		---------	-------------
******************************************************************************/
void FlashPageWrite256(INT16U page_address, const INT8U *source_address, INT8U which);



/******************************************************************************
函数名称：FlashPageRead256
功能描述：读Flash 中的某一页
参数描述：
参数名称：	输入/输出？	类型		描述
page_address		  输入 				 INT16U 		页地址
*source_address		输出			INT8U *		读出来的数据(长度为512)存放地址		
 which        输入        INT8U      主FLASH还是从FLASH，0 或 1

作      者	：许岩
日      期：2004-09-02
修改历史：
日期		修改人		修改描述
------		---------	-------------
******************************************************************************/
void FlashPageRead256(INT16U page_address,INT8U *source_address, INT8U which);








#endif
