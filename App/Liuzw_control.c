/*
 * @file       Liuzw_control.c
 * @brief      小车控制函数
 * @author     刘忠威
 * @date       2016-01-20
 */
#include "common.h"
#include "include.h"
#include "Liuzw_control.h"
#include "Liuzw_handle.h"
#include "MK60_it.h"
#include "Liuzw_oled.h"
//#include "Liuzw_camera.h"
#include "VCAN_OV7725_Eagle.h"
//#include "Liuzw_menu.h"
//#include "Liuzw_buzzer.h"

/*舵机参量*/
struct
{
  float Kp;//舵机P值
  float Kd;//舵机D值
}servo;
/*电机参量*/
struct
{
  int8 dir;
}motor;
/*偏差参量*/
float Deviation_This=0;//偏差
float Deviation_Last=0;//偏差
float Deviation_Error=0;//偏差差量
/*
 *  @brief      初始化所有硬件
 *  @author     刘忠威
 */
void init_all(void)
{
    DisableInterrupts;//关闭总中断
    /*设置中断优先级*/
    NVIC_SetPriorityGrouping(3);            
    NVIC_SetPriority(PORTA_IRQn,0);              
    NVIC_SetPriority(DMA0_IRQn,1);          
    NVIC_SetPriority(PIT0_IRQn,2); 
    NVIC_SetPriority(PIT1_IRQn,3); 

    /*Hardware init*/
    ftm_pwm_init(SERVO_FTM,SERVO_CH,SERVO_HZ,SERVO_MIDDLE);//舵机FTM初始化
    ftm_pwm_init(MOTOR_FTM_A,MOTOR_CH_A,MOTOR_HZ,0);//驱动FTM初始化
    gpio_init(MOTOR_DIR1_PIN,GPO,0);//驱动正向使能初始化
    gpio_init(MOTOR_DIR2_PIN,GPO,0);//驱动反向使能初始化
	
    oled_init();//oled初始化
    //buzzer_init();

    camera_init(imgbuff);//摄像头初始化
    oled_clear();
    set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);//设置PORTA的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);//设置DMA0的中断服务函数为 DMA0_IRQHandler
	
    led_init(LED_MAX);//led初始化
    //key_init(KEY_MAX);//菜单按键初始化
    //SWITCH_init(SWITCH_MAX);//拨码开关初始化
  
    uart_init (UART0,115200);//UART初始化
    //set_vector_handler(UART0_RX_TX_VECTORn,UART0_IRQHandler);//设置UART0的中断服务函数为UART0_IRQHandler
    //uart_rx_irq_en(UART0);//使能串口接收中断
	
    pit_init_ms(PIT0,5);//PIT0初始化，定时2ms                               
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);//设置PIT0的中断服务函数为PIT0_IRQHandler
    enable_irq (PIT0_IRQn);//使能PIT0中断
	
    pit_init_ms(PIT1,1000);//PIT1初始化，定时1s                               
    set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);//设置PIT0的中断服务函数为PIT0_IRQHandler
    enable_irq (PIT1_IRQn);//使能PIT0中断
	
		EnableInterrupts; //开启总中断
}
void servo_PD_select(void)
{
	if(Road_Left[59]>15||Road_Right[59]<65)
		{
			servo.Kp=250;
			servo.Kd=100;
		}
	else
	{
		if(Road_Type==0)
		{
			servo.Kp=20;
			servo.Kd=100;
		}
		else
		{
			servo.Kp=150;
			servo.Kd=100;
		
		}
	}
}
void servo_PD(void)
{
	Deviation_This=38-Ave_Err;
	Deviation_Error=Deviation_This-Deviation_Last;
	Deviation_Last=Deviation_This;
	
  float Servo_Temp;//舵机改变量
  uint32 Servo;//舵机实际输出值
  Servo_Temp=servo.Kp*Deviation_This+servo.Kd*Deviation_Error;//舵机改变量计算
  Servo=(uint32)(SERVO_MIDDLE+Servo_Temp);//舵机实际输出值计算
  if(Servo>SERVO_MAX)//大于最大值
    Servo=SERVO_MAX;
  if(Servo<SERVO_MIN)//小于最小值
    Servo=SERVO_MIN;  
	//printf("Servo: %d\r\n",Servo);	
  ftm_pwm_duty(SERVO_FTM,SERVO_CH,Servo);
}

void motor_PID(void)
{
	
	gpio_set(MOTOR_DIR1_PIN,1);//驱动正向使能初始化
	gpio_set(MOTOR_DIR2_PIN,0);//驱动正向使能初始化
	ftm_pwm_duty(MOTOR_FTM_A,MOTOR_CH_A,16);
}