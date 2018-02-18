#include "nvic.h"
#include "headfile.h"




void NVIC_Conf(void)
{
	set_irq_priority(PIT_CH0_IRQn,1);	
	
	set_irq_priority(SysTick_IRQn,0);	
	
  set_irq_priority(PIT_CH1_IRQn,2);
	
}


