#ifndef  MASTER_H
#define  MASTER_H

#include "headfile.h"

extern u8 P_A,D_A,P_S,I_S,P_T,D_T;
void order(void);
void get_pid(void);
void send_pid(void);
void send_sensor(void);
void send_road(char data);

#endif
