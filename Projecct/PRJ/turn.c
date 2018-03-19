#include "turn.h"
#include "math.h"
uint16 adc_value1[5]={0},adc_value2[5]={0},adc_value3[5]={0},adc1=0,adc2=0,adc3=0;

		u8 max=0; u8 min=0;
int16 turn_need;  
double turn_error;
float PWM_TURN;
float adc1_max = 0,adc1_min = 0,adc2_max = 0,adc2_min = 0,adc3_max = 0,adc3_min = 0;

extern float Gyro_Turn;
extern u8 P_T,D_T;
int sensor[3]={0};
void get_track()
{

    int i;
       
   	adc1=0;
		adc2=0;
		adc3=0;
	
	//float en=0,ez=0,ep=0,ecn=0,ecz=0,ecp=0,nn=0,nz=0,np=0,zn=0,zz=0,zp=0,pn=0,pz=0,pp=0,n=0,z=0,p=0;
//	float arr[4]={0};
	
 //启动一次ADC采集 分辨率12位
					for(i=0;i<5;i++)
          {
						
            adc_value1[i] = (uint16)adc_once(ADC0_SE0,ADC_12bit);
						
						if(adc_value1[i] > adc1_max)
							adc1_max = adc_value1[i];
						
						else if(adc_value1[i] <= adc1_min)
							adc1_min = adc_value1[i];
						
						adc1 =adc1+adc_value1[i];//((adc_value1[i]-adc1_min)*700/(adc1_max-adc1_min));
          }
         max = max_sort(adc_value1,5);
          min = min_sort(adc_value1,5);
          adc1 -= adc_value1[max];
          adc1 -= adc_value1[min];
          adc1 = adc1/3/25;   //25        
					
          for(i=0;i<5;i++)
          {
             adc_value2[i] = (uint16)adc_once(ADC0_SE1,ADC_12bit);
						
						if(adc_value2[i] > adc2_max)
							adc2_max = adc_value2[i];
						
						else if(adc_value2[i] <= adc2_min)
							adc2_min = adc_value2[i];
						adc2 =adc2+adc_value2[i];
						//adc2 += (((float)(adc_value2[i]-adc2_min)/(float)(adc2_max-adc2_min))*700);
          }
          max = max_sort(adc_value2,5);
          min = min_sort(adc_value2,5);
          adc2 -= adc_value2[max];
          adc2 -= adc_value2[min];
          adc2 = adc2/3/25;  //25
                    
         for(i=0;i<5;i++)
          {
             adc_value3[i] = adc_once(ADC0_SE2,ADC_12bit);
						
						if(adc_value3[i] > adc3_max)
							adc3_max = adc_value3[i];
						
						else if(adc_value3[i] <= adc3_min)
							adc3_min = adc_value3[i];
						
						adc3 += (((float)(adc_value3[i]-adc3_min)/(float)(adc3_max-adc3_min))*700);
          }
          max = max_sort(adc_value3,5);
          min = min_sort(adc_value3,5);
          adc3 -= adc_value3[max];
          adc3 -= adc_value3[min];
          adc3 = adc3/3/25;
					
#ifdef  DEBUG_MODE
				 	if((adc1<2)&&(adc2<2))
					{
						turn_error = 0;
//						ftm_pwm_duty(ftm2,ftm_ch0,0); //62
//						ftm_pwm_duty(ftm2,ftm_ch1,0); //62
//						ftm_pwm_duty(ftm2,ftm_ch2,0); //62
//						ftm_pwm_duty(ftm2,ftm_ch3,0); //62

					}

						turn_error = (sqrt(adc1)-sqrt(adc2))/(adc1+adc2)*400;//该公式在adc为0~100时，结果为-40~40，单调递增 
					
				/*	if(turn_error<3)
						turn_error = 0;*/
					
					if(turn_error>=0)
						turn_error = turn_error*turn_error/40;
					else
						turn_error = -(turn_error*turn_error/40);
					  sensor[0]=adc1;
	          sensor[1]=adc2;
#else

	
	//赛道判断(采用三横放电感0,1,2，双八字电感3,4)
	if(adc_value[3]>offet_1&&((adc_value[3]-adc_value[4])<offet_0)&&((adc_value[3]-adc_value[4])>-offet_0))   //两八字电感均大于offet_1，且两电感差距不大    十字
	{
		turn_error = (sqrt(adc_value[0])-sqrt(adc_value[3]))/(adc_value[0]+adc_value[3]);//该公式在adc为-100~100时，结果为-40~40，单调递增
	}
	if(max==1) //中间电感值最大
	{
		if(adc_value[3]>=adc_value[0]&&adc_value[3]>=adc_value[2]&&adc_value[3]>adc_value[4])  //左八字电感第二大，左圆环  (可由比较改为做差大于offet_0)
		{
			turn_error -=5;
		}
		else if(adc_value[4]>=adc_value[0]&&adc_value[4]>=adc_value[2]&&adc_value[4]>adc_value[3])    //右八字电感第二大，右圆环
		{
			turn_error +=5;
		}
		else if(((adc_value[0]-adc_value[2])<offet_0)&&((adc_value[0]-adc_value[2])>-offet_0))  //两边两横放电感值几乎相等，直道
		{
			turn_error = 0;
		}
		else                                                                                    //小弯道
		{
			turn_error = (sqrt(adc_value[0])-sqrt(adc_value[3]))/(adc_value[0]+adc_value[3]);
		}
	}
	else if(max==3)//左八字最大
	{
		if(adc_value[1]>=adc_value[0]&&adc_value[1]>=adc_value[2]&&adc_value[1]>adc_value[4])  //中间电感电感第二大，左圆环
		{
			turn_error -=5;
		}
	}
	else if(max==4)//右八字最大
	{
		if(adc_value[1]>=adc_value[0]&&adc_value[1]>=adc_value[2]&&adc_value[1]>adc_value[3])  //中间电感电感第二大，右圆环
		{
			turn_error +=5;
		}
	}
	else //大弯道
	{
		turn_error = (sqrt(adc_value[0])-sqrt(adc_value[3]))/(adc_value[0]+adc_value[3]);
	}

	
#endif

	if(turn_error>40)
		turn_error=40;
	else if(turn_error<-40)
		turn_error=-40;
	
	
}







//转向环  //用(-40,40)表示turn_error所处位置
void duty_turn()
{
	get_track();
	
	/******************************************************普通PD***************************************************/
	#ifdef  USE_TURN_PD
	PWM_TURN = P_TURN*turn_error - D_TURN*Gyro_Turn/10;
	#endif
	
	
	/******************************************************分组PD（关联速度）***************************************/
	#ifdef  USE_TURN_MOVE_PD

	
	
	#endif
	
	/*******************************************************模糊PD***************************************************/
	#ifdef  USE_TURN_BLUR_PD
	//对e和ec模糊化（经典三角隶属函数）(暂定取值范围e:-40~40,ec:-100~100)
	//通过规则匹配求隶属度
	if((turn_error >= (-40))&&(turn_error<=0))
	{
		en = -turn_error/40;
		ez = (turn_error+40)/40;
	}
	else if(turn_error>40&&turn_error<=80)
	{
		ez = (turn_error-40)/40;
		ep = turn_error/40;
	}
	if(Gyro_Turn>=-100&&Gyro_Turn<=0)
	{
		ecn = -Gyro_Turn/100;
		ecz = (Gyro_Turn+100)/100;
	}
	else if(Gyro_Turn>0&&Gyro_Turn<=100)
	{
		ecz = (Gyro_Turn-100)/100;
		ecp = Gyro_Turn/100;
	}
	//由取小运算，通过隶属度前提推理得可信度
	nn = en>ecn?ecn:en;
	nz = en>ecz?ecz:en;
	np = en>ecp?ecp:en;
	zn = ez>ecn?ecn:ez;
	zz = ez>ecz?ecz:ez;
	zp = ez>ecp?ecp:ez;
	pn = ep>ecn?ecn:ep;
	pz = ep>ecz?ecz:ep;
	pp = ep>ecp?ecp:ep;
	
	//由取大运算，通过P_TURN的模糊规则表得到模糊系统总输出
	arr[0]=nn;arr[1]=nz;arr[2]=np;arr[3]=zn;
	n = arr[max_sort((uint16*)arr,4)];
	z = zz;
	arr[0]=pn;arr[1]=pz;arr[2]=pp;arr[3]=zp;
	p = arr[max_sort((uint16*)arr,4)];
	//对输出模糊量利用最大平均法反模糊化(经典三角隶属函数)（暂定范围0~20）
	if(n>=p&&n>=z)
	{
		P_TURN = P_TURN - 10*n;
	}
	else if(z>=n&&z>=p)
	{
		P_TURN += ((10*z-10+10*z+10)/2);
	}
	else if(p>z&&p>n)
	{
		P_TURN += (10*n);
	}
		
	//由取大运算，通过D_TURN的模糊规则表得到模糊系统总输出
	arr[0]=nn;arr[1]=nz;arr[2]=zn;arr[3]=pn;
	n = arr[max_sort((uint16*)arr,4)];
	z = zz;
	arr[0]=np;arr[1]=pz;arr[2]=pp;arr[3]=zp;
	p = arr[max_sort((uint16*)arr,4)];
	//对输出模糊量利用最大平均法反模糊化(经典三角隶属函数)（暂定范围0~20）
	if(n>=p&&n>=z)
	{
		D_TURN -= (-10*n);
	}
	else if(z>=n&&z>=p)
	{
		D_TURN += ((10*z-10+10*z+10)/2);
	}
	else if(p>z&&p>n)
	{
		D_TURN += (10*n);
	}	
	P_TURN = P_TURN+10;
	D_TURN = D_TURN+10;
	#endif
	
	
/******************************************************最大转弯半径限制***************************************/

	if(PWM_TURN >400)

		  PWM_TURN=400;
	else if(PWM_TURN <-400)
			PWM_TURN=-400;
	

	

	
		
}







