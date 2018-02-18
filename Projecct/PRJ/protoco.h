#ifndef  __PROTOCO_H
#define  __PROTOCO_H

#include "control.h"


void OutPut_Data(void);
void Send_Wave_int(void);
void Send_Wave_char(void);
void vcan_sendware(uint8 *wareaddr, uint32 waresize);



#endif


