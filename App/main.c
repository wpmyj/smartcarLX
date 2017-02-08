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
			
        camera_get_img();//采集赛道
        img_extract((uint8*)img, imgbuff,CAMERA_SIZE);  //挤压图像
        search_line();
        cal_valid_row();
        road_type();
        cal_middle();
        cal_error();
        //send_data();
        vcan_sendimg(imgbuff, CAMERA_SIZE);//发送到上位机
      	oled_show_picture();//oled显示赛道
  
		}*/
}

void zet_motor(void){
    ftm_pwm_init(MOTOR_FTM_A,MOTOR_CH_A,MOTOR_HZ_A,0);//驱动FTM初始化
    gpio_init(MOTOR_DIR1_PIN,GPO,0);//驱动正向使能初始化
    gpio_init(MOTOR_DIR2_PIN,GPO,0);//驱动反向使能初始化
    gpio_set(MOTOR_DIR1_PIN,1);//驱动正向使能初始化
    gpio_set(MOTOR_DIR2_PIN,0);//驱动正向使能初始化
    ftm_pwm_duty(MOTOR_FTM_A,MOTOR_CH_A,76);
    
    ftm_pwm_init(MOTOR_FTM_A,MOTOR_CH_B,MOTOR_HZ_B,0);//驱动FTM初始化
    gpio_init(MOTOR_DIR3_PIN,GPO,0);//驱动正向使能初始化
    gpio_init(MOTOR_DIR4_PIN,GPO,0);//驱动反向使能初始化
    gpio_set(MOTOR_DIR3_PIN,0);//驱动正向使能初始化
    gpio_set(MOTOR_DIR4_PIN,1);//驱动正向使能初始化
    ftm_pwm_duty(MOTOR_FTM_B,MOTOR_CH_B,76);
}

void steer(void){
  ftm_pwm_init(SERVO_FTM,SERVO_CH,SERVO_HZ,SERVO_MIDDLE);//舵机FTM初始化
  ftm_pwm_duty(SERVO_FTM,SERVO_CH,8650);
}
