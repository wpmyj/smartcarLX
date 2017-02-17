/*
 * @file       Liuzw_control.c
 * @brief      С�����ƺ���
 * @author     ������
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

/*�������*/
struct
{
  float Kp;//���Pֵ
  float Kd;//���Dֵ
}servo;
/*�������*/
struct
{
  int8 dir;
}motor;
/*ƫ�����*/
float Deviation_This=0;//ƫ��
float Deviation_Last=0;//ƫ��
float Deviation_Error=0;//ƫ�����
/*
 *  @brief      ��ʼ������Ӳ��
 *  @author     ������
 */
void init_all(void)
{
    DisableInterrupts;//�ر����ж�
    /*�����ж����ȼ�*/
    NVIC_SetPriorityGrouping(3);            
    NVIC_SetPriority(PORTA_IRQn,0);              
    NVIC_SetPriority(DMA0_IRQn,1);          
    NVIC_SetPriority(PIT0_IRQn,2); 
    NVIC_SetPriority(PIT1_IRQn,3); 

    /*Hardware init*/
    ftm_pwm_init(SERVO_FTM,SERVO_CH,SERVO_HZ,SERVO_MIDDLE);//���FTM��ʼ��
    ftm_pwm_init(MOTOR_FTM_A,MOTOR_CH_A,MOTOR_HZ,0);//����FTM��ʼ��
    gpio_init(MOTOR_DIR1_PIN,GPO,0);//��������ʹ�ܳ�ʼ��
    gpio_init(MOTOR_DIR2_PIN,GPO,0);//��������ʹ�ܳ�ʼ��
	
    oled_init();//oled��ʼ��
    //buzzer_init();

    camera_init(imgbuff);//����ͷ��ʼ��
    oled_clear();
    set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);//����PORTA���жϷ�����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);//����DMA0���жϷ�����Ϊ DMA0_IRQHandler
	
    led_init(LED_MAX);//led��ʼ��
    //key_init(KEY_MAX);//�˵�������ʼ��
    //SWITCH_init(SWITCH_MAX);//���뿪�س�ʼ��
  
    uart_init (UART0,115200);//UART��ʼ��
    //set_vector_handler(UART0_RX_TX_VECTORn,UART0_IRQHandler);//����UART0���жϷ�����ΪUART0_IRQHandler
    //uart_rx_irq_en(UART0);//ʹ�ܴ��ڽ����ж�
	
    pit_init_ms(PIT0,5);//PIT0��ʼ������ʱ2ms                               
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);//����PIT0���жϷ�����ΪPIT0_IRQHandler
    enable_irq (PIT0_IRQn);//ʹ��PIT0�ж�
	
    pit_init_ms(PIT1,1000);//PIT1��ʼ������ʱ1s                               
    set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);//����PIT0���жϷ�����ΪPIT0_IRQHandler
    enable_irq (PIT1_IRQn);//ʹ��PIT0�ж�
	
		EnableInterrupts; //�������ж�
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
	
  float Servo_Temp;//����ı���
  uint32 Servo;//���ʵ�����ֵ
  Servo_Temp=servo.Kp*Deviation_This+servo.Kd*Deviation_Error;//����ı�������
  Servo=(uint32)(SERVO_MIDDLE+Servo_Temp);//���ʵ�����ֵ����
  if(Servo>SERVO_MAX)//�������ֵ
    Servo=SERVO_MAX;
  if(Servo<SERVO_MIN)//С����Сֵ
    Servo=SERVO_MIN;  
	//printf("Servo: %d\r\n",Servo);	
  ftm_pwm_duty(SERVO_FTM,SERVO_CH,Servo);
}

void motor_PID(void)
{
	
	gpio_set(MOTOR_DIR1_PIN,1);//��������ʹ�ܳ�ʼ��
	gpio_set(MOTOR_DIR2_PIN,0);//��������ʹ�ܳ�ʼ��
	ftm_pwm_duty(MOTOR_FTM_A,MOTOR_CH_A,16);
}