#include "steer.h"
#include "common.h"
#include "stdlib.h"
#include "ccd.h"

#define Steer_Init 4000  //舵机中值对应的PWM    左5000  右3000
#define Steer_Angle 1000  //实验数据

struct _pid{
		float Target_Speed;//定义设定值 ，目标速度
    float Actual_Speed;//定义实际值
	  float err;         //定义偏差值
		float err_next;    //定义上一个偏差量
	  float err_last;    //定义最上前的偏差值
		float Kp,Ki,Kd;    //定义比例、积分、微分系数
}pid;

extern int CCD_Forward;
extern int CCD_Left;
extern int CCD_Right;
extern int CCD_Middle;
extern int Black_Left;
extern int Black_Right;
extern int ramp_time;
extern int last_CCD_Middle;
extern int ramp_before_steer_pwm;//上坡前的舵机的打角

int Speed_Keys;

bool turn_left=false;
bool turn_right=false;
bool hold_turn=false;
bool hold_deadly=false;
bool ramp=false;
bool delete_wan;

int steer_Kp=0;
int steer_Kd=0;
int steer_pwm; 
int is_straight=255;
extern int fast_de_speed;

int steerKp=9;//
int steerKd=9;

extern uint8_t ADC_Value[128];
extern uint8_t ADC2_Value[128];
extern int  up_down_Offset[128];


void steer_Init(){
	  UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
	
	  FTM_PWM_QuickInit(FTM0_CH0_PC01, kPWM_EdgeAligned, 100);
	  //FTM_PWM_ChangeDuty(HW_FTM0, 0, 8560); //  8430   8560    8690
  	FTM_PWM_ChangeDuty(HW_FTM0, 0, 8560); 
}


/*void Set_Steer_angle(uint16_t steer){
		FTM_PWM_ChangeDuty(HW_FTM0,0,steer);
}*/

void PID_init(){
		pid.Target_Speed=0.0;
		pid.Actual_Speed=0.0;
		pid.err=0.0;
		pid.err_last=0.0;
		pid.err_next=0.0;
		pid.Kp=0.2;
		pid.Ki=0.015;
		pid.Kd=0.2;
}


void steer_test(){	
	int steer_pwms,last_steer_pwms;
	               //加上与非门，中心占空比为8560  
	  if(ABS(CCD_Middle)<19&&(Black_Right>Black_Left)){
				  CCD_Middle = last_CCD_Middle;
		 }
     steer_pwms =(8560 - 3*CCD_Middle);
		 
		 if(steer_pwms>8700){//防止打角过大
				steer_pwms = 8700;
		 }
		 if(steer_pwms<8430){
				steer_pwms = 8430;
		 }
		
		 
		 last_CCD_Middle = CCD_Middle;   
		 //last_steer_pwms = steer_pwms;
		 
		FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH0, steer_pwms);
}

