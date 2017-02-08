#include "common.h"
#include "include.h"
#include "MK60_it.h"
#include "Liuzw_oled.h"
#include "Liuzw_camera.h"
#include "Liuzw_handle.h"
#include "Liuzw_control.h"
//#include "Liuzw_menu.h"
//#include "Liuzw_buzzer.h"

void zet_motor(void);
void steer(void);

void  main(void)
{ 
    led_init(LED0);
    zet_motor();
    steer();
    while (1)
    {
      led_set(LED0,LED_ON);
      DELAY_MS(1000);
      led_set(LED0,LED_OFF);
      DELAY_MS(1000);
    }
    
    /*init_all(); 
	 
		while(1)
		{
			
        camera_get_img();//�ɼ�����
        img_extract((uint8*)img, imgbuff,CAMERA_SIZE);  //��ѹͼ��
        search_line();
        cal_valid_row();
        road_type();
        cal_middle();
        cal_error();
        //send_data();
        vcan_sendimg(imgbuff, CAMERA_SIZE);//���͵���λ��
      	oled_show_picture();//oled��ʾ����
  
		}*/
}

void zet_motor(void){
    ftm_pwm_init(MOTOR_FTM_A,MOTOR_CH_A,MOTOR_HZ_A,0);//����FTM��ʼ��
    gpio_init(MOTOR_DIR1_PIN,GPO,0);//��������ʹ�ܳ�ʼ��
    gpio_init(MOTOR_DIR2_PIN,GPO,0);//��������ʹ�ܳ�ʼ��
    gpio_set(MOTOR_DIR1_PIN,1);//��������ʹ�ܳ�ʼ��
    gpio_set(MOTOR_DIR2_PIN,0);//��������ʹ�ܳ�ʼ��
    ftm_pwm_duty(MOTOR_FTM_A,MOTOR_CH_A,76);
    
    ftm_pwm_init(MOTOR_FTM_A,MOTOR_CH_B,MOTOR_HZ_B,0);//����FTM��ʼ��
    gpio_init(MOTOR_DIR3_PIN,GPO,0);//��������ʹ�ܳ�ʼ��
    gpio_init(MOTOR_DIR4_PIN,GPO,0);//��������ʹ�ܳ�ʼ��
    gpio_set(MOTOR_DIR3_PIN,0);//��������ʹ�ܳ�ʼ��
    gpio_set(MOTOR_DIR4_PIN,1);//��������ʹ�ܳ�ʼ��
    ftm_pwm_duty(MOTOR_FTM_B,MOTOR_CH_B,76);
}

void steer(void){
  ftm_pwm_init(SERVO_FTM,SERVO_CH,SERVO_HZ,SERVO_MIDDLE);//���FTM��ʼ��
  ftm_pwm_duty(SERVO_FTM,SERVO_CH,8650);
}
