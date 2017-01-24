#include "steer.h"
#include "common.h"
#include "stdlib.h"
#include "ccd.h"

#define Steer_Init 4000  //�����ֵ��Ӧ��PWM    ��5000  ��3000
#define Steer_Angle 1000  //ʵ������

struct _pid{
		float Target_Speed;//�����趨ֵ ��Ŀ���ٶ�
    float Actual_Speed;//����ʵ��ֵ
	  float err;         //����ƫ��ֵ
		float err_next;    //������һ��ƫ����
	  float err_last;    //��������ǰ��ƫ��ֵ
		float Kp,Ki,Kd;    //������������֡�΢��ϵ��
}pid;

extern int CCD_Forward;
extern int CCD_Left;
extern int CCD_Right;
extern int CCD_Middle;
extern int Black_Left;
extern int Black_Right;
extern int ramp_time;
extern int last_CCD_Middle;
extern int ramp_before_steer_pwm;//����ǰ�Ķ���Ĵ��

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
	               //��������ţ�����ռ�ձ�Ϊ8560  
	  if(ABS(CCD_Middle)<19&&(Black_Right>Black_Left)){
				  CCD_Middle = last_CCD_Middle;
		 }
     steer_pwms =(8560 - 3*CCD_Middle);
		 
		 if(steer_pwms>8700){//��ֹ��ǹ���
				steer_pwms = 8700;
		 }
		 if(steer_pwms<8430){
				steer_pwms = 8430;
		 }
		
		 
		 last_CCD_Middle = CCD_Middle;   
		 //last_steer_pwms = steer_pwms;
		 
		FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH0, steer_pwms);
}

