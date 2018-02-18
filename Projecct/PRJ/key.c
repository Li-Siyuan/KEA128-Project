#include "key.h"

extern float speed_need,MOVE;
extern u8 P_A,D_A,P_S,I_S,P_T,D_T;
extern char Debug_Mode;
//extern int16 Angle_Need;
float abc = 0;
int8 mode=0;
char key_flag = 1;

void KEY_SCAN()
{
	if(gpio_get(KEY1)==0)
		{
      DisableInterrupts;
			
			if(mode==8)
			{
				WRITE_8_DATA();
			
				OLED_ShowString(0,6,(uint8*)"Write OK",16);
				while(gpio_get(KEY1)==0);
				DISPLAY();
			}
			else if(mode>8&&mode<12)
			{
				Debug_Mode = mode-9;
				OLED_ShowString(0,6,(uint8*)"Debug_Ok    ",16);
			}
			else
			{
				abc++;
				OLED_ShowString(0,6,(uint8*)"abc:    ",16);OLED_MY_ShowNum(25,6,abc,16);  
				while(gpio_get(KEY1)==0){}					
			}
		
      EnableInterrupts;
		}
		
		
		if(gpio_get(KEY2)==0)
		{
      DisableInterrupts;
			
			abc--;
			OLED_ShowString(0,6,(uint8*)"abc:    ",16);OLED_MY_ShowNum(25,6,abc,16);                       
			
			while(gpio_get(KEY2)==0){}			 
      EnableInterrupts;
		}
		
		
		if(gpio_get(KEY3)==0)
		{
      DisableInterrupts;
			if(key_flag==0)
			{
				 mode++;
				 if(mode>11)
					 mode=0;
				 
				 switch(mode)
				 {
					 case 0:  abc=P_A; key_flag=1;OLED_ShowString(0,6,(uint8*)"PA:    ",16);OLED_MY_ShowNum(25,6,P_A,16);break;
					 case 1:  abc=D_A; key_flag=1;OLED_ShowString(0,6,(uint8*)"DA:    ",16);OLED_MY_ShowNum(25,6,D_A,16);break;
					 case 2:  abc=P_S; key_flag=1;OLED_ShowString(0,6,(uint8*)"PS:    ",16);OLED_MY_ShowNum(25,6,P_S,16);break;
					 case 3:  abc=I_S; key_flag=1;OLED_ShowString(0,6,(uint8*)"IS:    ",16);OLED_MY_ShowNum(25,6,I_S,16);break;
					 case 4:  mode=5;
					 case 5:  abc=P_T; key_flag=1;OLED_ShowString(0,6,(uint8*)"PT:    ",16);OLED_MY_ShowNum(25,6,P_T,16);break;
					 case 6:  abc=D_T; key_flag=1;OLED_ShowString(0,6,(uint8*)"DT:    ",16);OLED_MY_ShowNum(25,6,D_T,16);break;
					 case 7:  abc=MOVE*10; key_flag=1;OLED_ShowString(0,6,(uint8*)"MO:    ",16);OLED_MY_ShowNum(25,6,MOVE*10,16);break;
					 case 8:  OLED_ShowString(0,6,(uint8*)"Flash:      ",16);break;
					 case 9: 	OLED_ShowString(0,6,(uint8*)"Angle_Debug ",16);break;
					 case 10:  OLED_ShowString(0,6,(uint8*)"Speed_Debug ",16);break;
					 case 11:  OLED_ShowString(0,6,(uint8*)"Turn_Debug  ",16);break;
					 default: break;
				 }
		  }
			else if(key_flag==1)
			{
					switch(mode)
				 {
					 case 0:  P_A=abc; key_flag=0;OLED_ShowString(0,6,(uint8*)"PA:    ",16);OLED_MY_ShowNum(25,6,P_A,16);break;
					 case 1:  D_A=abc; key_flag=0;OLED_ShowString(0,6,(uint8*)"DA:    ",16);OLED_MY_ShowNum(25,6,D_A,16);break;
					 case 2:  P_S=abc; key_flag=0;OLED_ShowString(0,6,(uint8*)"PS:    ",16);OLED_MY_ShowNum(25,6,P_S,16);break;
					 case 3:  I_S=abc; key_flag=0;OLED_ShowString(0,6,(uint8*)"IS:    ",16);OLED_MY_ShowNum(25,6,I_S,16);break;
					 case 4:  mode=5;
					 case 5:  P_T=abc; key_flag=0;OLED_ShowString(0,6,(uint8*)"PT:    ",16);OLED_MY_ShowNum(25,6,P_T,16);break;
					 case 6:  D_T=abc; key_flag=0;OLED_ShowString(0,6,(uint8*)"DT:    ",16);OLED_MY_ShowNum(25,6,D_T,16);break;
					 case 7:  MOVE=abc/10; key_flag=0;OLED_ShowString(0,6,(uint8*)"MO:    ",16);OLED_MY_ShowNum(25,6,MOVE*10,16);break;
					 default: break;
				 }
			}
			 		 
			while(gpio_get(KEY3)==0){}			
      EnableInterrupts;
		}
		
		
}

