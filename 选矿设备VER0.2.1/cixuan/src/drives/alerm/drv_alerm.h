
#ifndef __DRV_ALERM_H__
#define	__DRV_ALERM_H__	

#include  "data_type.h"

#define  set_open_door()       (IO0CLR = BIT28)
#define  set_close_door()      (IO0SET = BIT28)

#define  set_alerm_begin()     (IO3CLR = BIT29)
#define  set_alerm_stop()	   (IO3SET = BIT29)

/*****************************************************************
函数原型：alerm_init
功能描述：报警开关部分，初始化设置
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：无
作      者：刘及华 
日      期：2014-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void alerm_door_Init(void);


#endif
