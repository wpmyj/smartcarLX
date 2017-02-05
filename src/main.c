#include "gpio.h"
#include "common.h"
#include "ftm.h"
#include "motor.h"
#include "steer.h"
#include "pit.h"
#include "interupt.h"
#include "oled.h"
#include "uart.h"
#include "getspeed.h"
#include "camera.h"
uint8_t ADC_Value[128];

int CCD_Left;
uint32_t buletooth_instance;
uint8_t  Buffer[132];

extern int CCD_Right;
extern int CCD1_Left;
extern int CCD1_Right;
extern int CCD2_Left;
extern int CCD2_Right;
extern uint32_t L_speedValue;
extern uint32_t R_speedValue;

void Zrq_interupt(PIT_InitTypeDef* Time){//中断初始化程序
		Time->chl = HW_PIT_CH0;
	  Time->timeInUs = 30*1000;
	  PIT_Init(Time);
}

void r_motor(int C_speed,int clockwise,int anticlockwise){
	FTM_PWM_QuickInit(FTM1_CH0_PB00, kPWM_EdgeAligned, 10000);
	
	GPIO_QuickInit(HW_GPIOC, 3, kGPIO_Mode_OPP);
	GPIO_QuickInit(HW_GPIOC, 2, kGPIO_Mode_OPP);	 
	
	GPIO_WriteBit(HW_GPIOB,3,clockwise);
	GPIO_WriteBit(HW_GPIOB,2,anticlockwise);//电机正转
	FTM_PWM_ChangeDuty (HW_FTM1, HW_FTM_CH0, C_speed);
	
	
		//GPIO_WriteBit(HW_GPIOB,20,anticlockwise);
	  //GPIO_WriteBit(HW_GPIOB,21,clockwise);//电机反转
}

void l_motor(int C_speed,int clockwise,int anticlockwise){
	 FTM_PWM_QuickInit(FTM2_CH0_PB18, kPWM_EdgeAligned, 10000);
	
	 
	 GPIO_QuickInit(HW_GPIOB, 16, kGPIO_Mode_OPP);
	 GPIO_QuickInit(HW_GPIOB, 17, kGPIO_Mode_OPP);
	
 	 GPIO_WriteBit(HW_GPIOB,17,anticlockwise);
   GPIO_WriteBit(HW_GPIOB,16,clockwise);//电机正转
	 FTM_PWM_ChangeDuty(HW_FTM2,HW_FTM_CH0,C_speed);
	
	//GPIO_WriteBit(HW_GPIOB,16,anticlockwise);
  //GPIO_WriteBit(HW_GPIOB,17,clockwise);//电机反转
}

void motor(){
	 int C_speed=7500;
	 int clockwise=0;
	 int anticlockwise=1;
	  //B1电机
	  r_motor (C_speed,clockwise,anticlockwise);
	  //B2电机
		l_motor(C_speed,clockwise,anticlockwise);
}
void steer_init(){
		FTM_PWM_QuickInit(FTM0_CH3_PC04, kPWM_EdgeAligned, 100);
	  //FTM_PWM_ChangeDuty(HW_FTM0, 0, 8560); //  8430   8560    8690
  	FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3, 8560); 
}


void steer_change(){
		DelayInit();
		uint16_t i;
    while(1)
    {

        for(i = 1010;i<2220;i++)
        {
            FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3,10000-i);
						DelayMs(5);
        }

        for(;i>1010;i--)
        {
            FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3,10000-i);
            DelayMs(5);
        }
    }
}



void zhangRuiQing_MotorAndSteer(){
	 int C_speed=7400;
	 int clockwise=0;
	 int anticlockwise=1;
	 FTM_PWM_QuickInit(FTM2_CH1_PB19, kPWM_EdgeAligned, 10000);
	 FTM_PWM_QuickInit(FTM2_CH0_PB18, kPWM_EdgeAligned, 10000);
	
	 GPIO_QuickInit(HW_GPIOB, 20, kGPIO_Mode_OPP);
	 GPIO_QuickInit(HW_GPIOB, 21, kGPIO_Mode_OPP);	 
	 GPIO_QuickInit(HW_GPIOB, 16, kGPIO_Mode_OPP);
	 GPIO_QuickInit(HW_GPIOB, 17, kGPIO_Mode_OPP);
	
	
	  //右电机
	  GPIO_WriteBit(HW_GPIOB,20,clockwise);
	  GPIO_WriteBit(HW_GPIOB,21,anticlockwise);//电机正转
	  FTM_PWM_ChangeDuty (HW_FTM2, HW_FTM_CH1, C_speed);
	
	
		//GPIO_WriteBit(HW_GPIOB,20,anticlockwise);
	  //GPIO_WriteBit(HW_GPIOB,21,clockwise);//电机反转
	
	  //左电机
		 GPIO_WriteBit(HW_GPIOB,16,anticlockwise);
	   GPIO_WriteBit(HW_GPIOB,17,clockwise);//电机正转
	   FTM_PWM_ChangeDuty(HW_FTM2,HW_FTM_CH0,C_speed);
	  
		
	   //GPIO_WriteBit(HW_GPIOB,16,anticlockwise);
	   //GPIO_WriteBit(HW_GPIOB,17,clockwise);//电机反转
		 
		FTM_PWM_QuickInit(FTM0_CH0_PC01, kPWM_EdgeAligned, 100);
		//FTM_PWM_ChangeDuty(HW_FTM0, 0, 8560); //  8430   8560    8690
  	FTM_PWM_ChangeDuty(HW_FTM0, 0, 8560); 
		
		DelayInit();
		uint16_t i;
    while(1)
    {

        for(i = 1010;i<2220;i++)
        {
            FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH0,10000-i);
						DelayMs(5);
        }

        for(;i>1010;i--)
        {
            FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH0,10000-i);
            DelayMs(5);
        }
    }
}





int main(void)
{ 
	motor();
	steer_init();
	steer_change();
	OLED_Init();
}

