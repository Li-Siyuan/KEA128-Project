#include "turn.h"
#include "math.h"
uint16 adc_value1[5]={0},adc_value2[5]={0},adc_value3[5]={0},adc1=0,adc2=0,adc3=0;

		u8 max=0; u8 min=0;
int16 turn_error,turn_need;                     
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
	
 //����һ��ADC�ɼ� �ֱ���12λ
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
          adc1 = adc1/3/25;
                   
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
          adc2 = adc2/3/25;
                    
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
					if(adc1<10&&adc2<10)
						turn_error = 0;
					else
						turn_error = (sqrt(adc1)-sqrt(adc2))/(adc1+adc2)*400;//�ù�ʽ��adcΪ0~100ʱ�����Ϊ-40~40���������� 
					  sensor[0]=adc1;
	          sensor[1]=adc2;
#else

	
	//�����ж�(��������ŵ��0,1,2��˫���ֵ��3,4)
	if(adc_value[3]>offet_1&&((adc_value[3]-adc_value[4])<offet_0)&&((adc_value[3]-adc_value[4])>-offet_0))   //�����ֵ�о�����offet_1��������в�಻��    ʮ��
	{
		turn_error = (sqrt(adc_value[0])-sqrt(adc_value[3]))/(adc_value[0]+adc_value[3]);//�ù�ʽ��adcΪ-100~100ʱ�����Ϊ-40~40����������
	}
	if(max==1) //�м���ֵ���
	{
		if(adc_value[3]>=adc_value[0]&&adc_value[3]>=adc_value[2]&&adc_value[3]>adc_value[4])  //����ֵ�еڶ�����Բ��  (���ɱȽϸ�Ϊ�������offet_0)
		{
			turn_error -=5;
		}
		else if(adc_value[4]>=adc_value[0]&&adc_value[4]>=adc_value[2]&&adc_value[4]>adc_value[3])    //�Ұ��ֵ�еڶ�����Բ��
		{
			turn_error +=5;
		}
		else if(((adc_value[0]-adc_value[2])<offet_0)&&((adc_value[0]-adc_value[2])>-offet_0))  //��������ŵ��ֵ������ȣ�ֱ��
		{
			turn_error = 0;
		}
		else                                                                                    //С���
		{
			turn_error = (sqrt(adc_value[0])-sqrt(adc_value[3]))/(adc_value[0]+adc_value[3]);
		}
	}
	else if(max==3)//��������
	{
		if(adc_value[1]>=adc_value[0]&&adc_value[1]>=adc_value[2]&&adc_value[1]>adc_value[4])  //�м��е�еڶ�����Բ��
		{
			turn_error -=5;
		}
	}
	else if(max==4)//�Ұ������
	{
		if(adc_value[1]>=adc_value[0]&&adc_value[1]>=adc_value[2]&&adc_value[1]>adc_value[3])  //�м��е�еڶ�����Բ��
		{
			turn_error +=5;
		}
	}
	else //�����
	{
		turn_error = (sqrt(adc_value[0])-sqrt(adc_value[3]))/(adc_value[0]+adc_value[3]);
	}

	
#endif

	if(turn_error>40)
		turn_error=40;
	else if(turn_error<-40)
		turn_error=-40;
	
	
}







//ת��  //��(-40,40)��ʾturn_error����λ��
void duty_turn()
{
	get_track();
	
	/******************************************************��ͨPD***************************************************/
	#ifdef  USE_TURN_PD
	PWM_TURN = P_TURN*turn_error*5 - D_TURN*Gyro_Turn/10;
	#endif
	
	
	/******************************************************����PD�������ٶȣ�***************************************/
	#ifdef  USE_TURN_MOVE_PD

	
	
	#endif
	
	/*******************************************************ģ��PD***************************************************/
	#ifdef  USE_TURN_BLUR_PD
	//��e��ecģ������������������������(�ݶ�ȡֵ��Χe:-40~40,ec:-100~100)
	//ͨ������ƥ����������
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
	//��ȡС���㣬ͨ��������ǰ������ÿ��Ŷ�
	nn = en>ecn?ecn:en;
	nz = en>ecz?ecz:en;
	np = en>ecp?ecp:en;
	zn = ez>ecn?ecn:ez;
	zz = ez>ecz?ecz:ez;
	zp = ez>ecp?ecp:ez;
	pn = ep>ecn?ecn:ep;
	pz = ep>ecz?ecz:ep;
	pp = ep>ecp?ecp:ep;
	
	//��ȡ�����㣬ͨ��P_TURN��ģ�������õ�ģ��ϵͳ�����
	arr[0]=nn;arr[1]=nz;arr[2]=np;arr[3]=zn;
	n = arr[max_sort((uint16*)arr,4)];
	z = zz;
	arr[0]=pn;arr[1]=pz;arr[2]=pp;arr[3]=zp;
	p = arr[max_sort((uint16*)arr,4)];
	//�����ģ�����������ƽ������ģ����(����������������)���ݶ���Χ0~20��
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
		
	//��ȡ�����㣬ͨ��D_TURN��ģ�������õ�ģ��ϵͳ�����
	arr[0]=nn;arr[1]=nz;arr[2]=zn;arr[3]=pn;
	n = arr[max_sort((uint16*)arr,4)];
	z = zz;
	arr[0]=np;arr[1]=pz;arr[2]=pp;arr[3]=zp;
	p = arr[max_sort((uint16*)arr,4)];
	//�����ģ�����������ƽ������ģ����(����������������)���ݶ���Χ0~20��
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
	
	
/******************************************************���ת��뾶����***************************************/
	#ifdef  MAX_RADIUS1
	if(abs(PWM_TURN )>400)
	{
		if(PWM_TURN<0)
			PWM_TURN=-400;
		else
	    PWM_TURN=400;
	}
	
	#endif
	
		
}







