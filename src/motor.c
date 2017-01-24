#include "motor.h"

int super_speed=0;
int top_speed=0;
int middle_speed=0;
int l_system_speed=0;
int r_system_speed=0;

int fast_de_speed=255;

int l_motor_pwm=0;
int r_motor_pwm=0;

int min_speed=0;
int target_speed=42;

extern int CCD_Middle;
extern int CCD_Forward;
extern bool hold_turn;
extern uint32_t L_speedValue;
extern uint32_t R_speedValue;

static struct _pid{
		float Target_Speed;//定义设定值 ，目标速度
    float Actual_Speed;//定义实际值
	  float err;         //定义偏差值
		float err_next;    //定义上一个偏差量
	  float err_last;    //定义最上前的偏差值
		float Kp,Ki,Kd;    //定义比例、积分、微分系数
}motor_Pid;


void motor_PID_Init(){
		motor_Pid.Target_Speed=0.0;
		motor_Pid.Actual_Speed=0.0;
		motor_Pid.err=0.0;
		motor_Pid.err_last=0.0;
		motor_Pid.err_next=0.0;
		motor_Pid.Kp=0.2;
		motor_Pid.Ki=0.015;
		motor_Pid.Kd=0.2;
}


void motor_Init(){
	UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
	
	 int C_speed=0;
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
		 motor_PID_Init();
}


void set_L_motor_Pwm(int clockwise,int antilclockwise,int motor_pwms){ //clockwise = 0 ;  antilclockwise = 1 正转
	//左电机
	  if(motor_pwms>6000){
				motor_pwms=6000;
		}
	  GPIO_WriteBit(HW_GPIOB,20,clockwise);
	  GPIO_WriteBit(HW_GPIOB,21,antilclockwise);//    20:0   21:1  电机正转
	
	  FTM_PWM_ChangeDuty (HW_FTM2, HW_FTM_CH1,motor_pwms);
}

void set_R_motor_Pwm(int clockwise,int antilclockwise,int motor_pwms){ //clockwise = 0 ;  antilclockwise = 1 正转
	   //右电机
		if(motor_pwms>6000){
				motor_pwms=6000;
		}
	   GPIO_WriteBit(HW_GPIOB,16,antilclockwise);
	   GPIO_WriteBit(HW_GPIOB,17,clockwise);//  16:1   17:0  电机正转
	
	   FTM_PWM_ChangeDuty(HW_FTM2,HW_FTM_CH0,motor_pwms);	
}


void motor_PID(){
	//volatile static int err=0,last_err=0,derr,motorKd=50,motorKp=130;
	
}

void l_motor_PID_Test(){
	  motor_Pid.Actual_Speed=L_speedValue;
	
		motor_Pid.Target_Speed=200;
		motor_Pid.err=motor_Pid.Target_Speed-motor_Pid.Actual_Speed;
	  float	incrementSpeed=motor_Pid.Kp*(motor_Pid.err-motor_Pid.err_next)+motor_Pid.Ki*motor_Pid.err+motor_Pid.Kd*(motor_Pid.err-2*motor_Pid.err_next+motor_Pid.err_last);
		//motor_Pid.Actual_Speed+=incrementSpeed;
	  motor_Pid.err_last=motor_Pid.err_next;
	  motor_Pid.err_next=motor_Pid.err;
	  l_motor_pwm += incrementSpeed;//motor_Pid.Actual_Speed;
	  set_L_motor_Pwm(0,1,l_motor_pwm);
	  printf(" \n");
	  printf("\nLeft:  %f   ",incrementSpeed);
	  //printf("Left : %d",L_speedValue);
	  printf("Left : %d  ",l_motor_pwm);
}

void r_motor_PID_Test(){
	  motor_Pid.Actual_Speed=R_speedValue;
	
		motor_Pid.Target_Speed=200;
		motor_Pid.err=motor_Pid.Target_Speed-motor_Pid.Actual_Speed;
	  float	incrementSpeed=motor_Pid.Kp*(motor_Pid.err-motor_Pid.err_next)+motor_Pid.Ki*motor_Pid.err+motor_Pid.Kd*(motor_Pid.err-2*motor_Pid.err_next+motor_Pid.err_last);
		//motor_Pid.Actual_Speed+=incrementSpeed;
	  motor_Pid.err_last=motor_Pid.err_next;
	  motor_Pid.err_next=motor_Pid.err;
	  r_motor_pwm += incrementSpeed;//motor_Pid.Actual_Speed;
	  set_R_motor_Pwm(0,1,r_motor_pwm);
	  printf("Right:  %f   ",incrementSpeed);
	  //printf("Left : %d",R_speedValue);
	  printf("Right : %d\n",r_motor_pwm);	  
}


void motor_test(){
		int  motor_pwms;
	  int  l_motor_pwms;
	  int  r_motor_pwms;
		int  clockwise = 0;
    int  antilclockwise = 1;
	  int  target_Speed = 0;
    int  err_speed=0;
	  
	//err_speed = speedValue[1] - speedValue[0];//err_speed>0 表示右电机转速快
	
	if(ABS(CCD_Middle)>20){
			motor_pwms = 6000;
	}
	else{
			motor_pwms = 6700;
	}
  
	  l_motor_pwms=motor_pwms + err_speed;
	  r_motor_pwms=motor_pwms - err_speed;
  	//左电机
	  GPIO_WriteBit(HW_GPIOB,20,clockwise);
	  GPIO_WriteBit(HW_GPIOB,21,antilclockwise);//    20:0   21:1  电机正转
	
	  FTM_PWM_ChangeDuty (HW_FTM2, HW_FTM_CH1,l_motor_pwms);
		
	
	   //右电机
	   GPIO_WriteBit(HW_GPIOB,16,antilclockwise);
	   GPIO_WriteBit(HW_GPIOB,17,clockwise);//  16:1   17:0  电机正转
	
	   FTM_PWM_ChangeDuty(HW_FTM2,HW_FTM_CH0,r_motor_pwms);
}
