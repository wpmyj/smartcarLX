#include "common.h"
#include "include.h"
#include "function.h"

#include "handle.h"

uint16 speed_goal;
uint8 speed_get;
uint8 speed_last=0;
uint16 speed_nexterr=0;
uint16 speed_lasterr=0;
uint32 speed_PWM=0;
int16  speed_err;
float speed_increment;
uint16 var;
void xx_bluetooth();
void uart3_handler(void);
extern uint8 imgbuff[CAMERA_SIZE];

/*void PIT_IRQHandler(void)
{
    if(PIT_TFLG(PIT0) == 1 )        //判断是否 PIT0 进入中断
    {
        var = adc_once   (ADC0_SE9, ADC_8bit);
        speed_get=lptmr_pulse_get();//保存脉冲计数器计算值
        Motor_Out();
        lptmr_pulse_clean();//清空脉冲计数器计算值（马上清空，这样才能保证计数值准确）
        PIT_Flag_Clear(PIT0);       //清中断标志位
    }
}*/
void PIT0_IRQHandler(void)
{

    //int16 val;
    speed_get = ftm_quad_get(FTM1);          //获取FTM 正交解码 的脉冲数(负数表示反方向)
    Motor_Out();
    Search_Line();
    Find_Middle();
    Servo_control();
    ftm_quad_clean(FTM1);
    //zf_oled( val);
    PIT_Flag_Clear(PIT0);       //清中断标志位
}
void Init_All(void)
{
  Motor_Init();
  OLED_Init();
  ov7725_eagle_init(imgbuff);
  //xx_bluetooth();
  //PID_Init();
  //lptmr_pulse_init(LPT0_ALT2,0xFFFF , LPT_Rising);
  ftm_quad_init(FTM1);
  //adc_init(ADC0_SE9);
  pit_init_ms(PIT0, 2);                             //初始化PIT0，定时时间为： 1000ms       
  set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);       //设置PIT0的中断服务函数为 PIT_IRQHandler
  enable_irq (PIT0_IRQn);                                //使能PIT0中断
}
//电机舵机初始化
void Motor_Init(void)
{
  /*tpm_pwm_init(TPM0,TPM_CH0,10*1000,0);
  tpm_pwm_init(TPM2,TPM_CH0,100,8590);
  gpio_init(PTC1,GPO,1);
  gpio_init(PTC2,GPO,0);
  */
    ftm_pwm_init(FTM2,FTM_CH1,10000,0);//驱动FTM初始化
    gpio_init(PTC3,GPO,0);//驱动正向使能初始化
    gpio_init(PTC2,GPO,0);//驱动反向使能初始化
    gpio_set(PTC3,1);//驱动正向使能初始化
    gpio_set(PTC2,0);//驱动正向使能初始化
    
    
    ftm_pwm_init(FTM2,FTM_CH0,10000,0);//驱动FTM初始化
    gpio_init(PTB17,GPO,0);//驱动正向使能初始化
    gpio_init(PTB16,GPO,0);//驱动反向使能初始化
    gpio_set(PTB17,0);//驱动正向使能初始化
    gpio_set(PTB16,1);//驱动正向使能初始化
    
    ftm_pwm_init(FTM0,FTM_CH3,100,0);
    ftm_pwm_duty(FTM0, FTM_CH3, 9100);
    //gpio_init(PTC4,GPO,1);
    //gpio_ddr(PTC4, GPO);
}

void Motor_Out(void)
{
  speed_PWM=63;
 /* float speed_Ki=1.5;
  float speed_Kd=0.0;
  float speed_Kp=0.0;
  if(Overtake==1)
  {
      if(speed_get<5)
      {
        speed_goal=0;
      }
      //gpio_init(PTC1,GPO,0);
      //gpio_init(PTC2,GPO,1);
      speed_PWM=65;
  } 
  else
  {
      if(All_Black<=2)
        speed_goal=75;
      else if(All_Black>2&&All_Black<40)
      {
        speed_goal=80;
      } 
      else
      speed_goal=0;
      speed_err=speed_goal-speed_get;
      speed_increment=speed_Ki*speed_err; 
      speed_PWM=(uint32)(59+speed_increment);
  }
  if(speed_PWM>95)
    speed_PWM=95;
  //tpm_pwm_duty(TPM0,TPM_CH0,speed_PWM);*/
   ftm_pwm_duty(FTM2,FTM_CH1,(uint32)speed_PWM);
   ftm_pwm_duty(FTM2,FTM_CH0,(uint32)speed_PWM);
}

