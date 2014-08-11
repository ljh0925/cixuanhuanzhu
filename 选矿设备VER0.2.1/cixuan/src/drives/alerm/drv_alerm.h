
#ifndef __DRV_ALERM_H__
#define	__DRV_ALERM_H__	

#include  "data_type.h"

#define  set_open_door()       (IO0CLR = BIT28)
#define  set_close_door()      (IO0SET = BIT28)

#define  set_alerm_begin()     (IO3CLR = BIT29)
#define  set_alerm_stop()	   (IO3SET = BIT29)

/*****************************************************************
����ԭ�ͣ�alerm_init
�����������������ز��֣���ʼ������
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------

��  ��  ֵ����
��      �ߣ������� 
��      �ڣ�2014-05-12
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
void alerm_door_Init(void);


#endif
