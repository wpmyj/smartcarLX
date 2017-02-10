#include "common.h"
#include "include.h"
#include "MK60_it.h"
#include "Liuzw_oled.h"
//#include "Liuzw_camera.h"
#include "Liuzw_handle.h"
#include "Liuzw_control.h"
//#include "Liuzw_menu.h"
//#include "Liuzw_buzzer.h"

char Para_Name[7][12]={"PID_ANGLE_P\0","PID_ANGLE_D\0","PID_SPEED_P\0",
"PID_SPEED_I\0","PID_SPEED_D\0","PID_DIREC_P\0","PID_DIREC_D\0"};



//float PID_ANGLE_P,PID_ANGLE_D,PID_SPEED_P,PID_SPEED_I,PID_SPEED_D,PID_TURN_P,PID_TURN_D;

float Control_Para[7];


float Voltage=7.26;

uint8 Oled_Show=1,Para_Index=0,Para_Checked=0,Para_Index_Limit=7,Step_Index=3;

float Step[8]={0.0001,0.001,0.01,0.1,1.0,10.0,100.0,1000.0};   //默认调节步长为0.01

void zet_motor(void);
void steer(void);
void zet_oled();
void zet_camera();

/*void OLED_Draw_UI()
{
     uint8 i;
     OLED_P6x8Str(0,0,"Voltage=");                          //显示电池电压
     OLED_PrintValueF(48, 0,Voltage,2);                     
     OLED_PrintValueF(72, 0,Step[Step_Index],5);            //显示调节步进值
     
    for(i=0;i<7;i++)
    {
     if(i==Para_Index&&Para_Checked==false)
      {
       reverse=1;
       OLED_P6x8Str(0,i+1,Para_Name[i]);   //将参量名反转显示
       reverse=0;
      }
      else OLED_P6x8Str(0,i+1,Para_Name[i]);

    
     OLED_P6x8Char('=');
    
      if(i==Para_Index&&Para_Checked)
      {
        reverse=1;
        OLED_PrintValueF(72, i+1,Control_Para[i],5);
        reverse=0;
      }
      else  OLED_PrintValueF(72, i+1,Control_Para[i],5);
      
    }
     
}*/



void  main(void)
{ 
    led_init(LED0);
    zet_motor();
    steer();
    zet_oled();
    zet_camera();
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

void zet_oled(){
  oled_init();
  oled_display_on();
  oled_show_logo();
  DELAY_MS(2000);
  oled_clear();
}

void zet_camera(void){
/*  Site_t site     = {0, 0};                           //显示图像左上角位置
    Size_t imgsize  = {CAMERA_W, CAMERA_H};             //图像大小
    Size_t size;                   //显示区域图像大小
*/
    //LCD_init();
    //LCD_str            (site,"Cam init ing",FCOLOUR,BCOLOUR);

    //size.H = LCD_H;
    //size.W = LCD_W;

    camera_init(imgbuff);

    //LCD_str            (site,"Cam init OK!",FCOLOUR,BCOLOUR);
    //site.y = 110;
    //LCD_FSTR_CH(site,vcan_str,FCOLOUR,BCOLOUR);

    //配置中断服务函数
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //设置 PORTA 的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //设置 DMA0 的中断服务函数为 PORTA_IRQHandler

    while(1)
    {
        camera_get_img();                                   //摄像头获取图像
        img_extract((uint8*)img,imgbuff,CAMERA_SIZE);//二值化图像

        oled_show_picture();//oled显示采集的图像    
                      //黑白摄像头
        //LCD_Img_Binary_Z(site, size, imgbuff, imgsize);

        /******************** 山外多功能调试助手 【黑白模式】 发送图像到上位机 ***********************/
        //vcan_sendimg(imgbuff, sizeof(imgbuff));

        /******************** 山外多功能调试助手 【灰度模式】 发送图像到上位机 ***********************/
        //img_extract(img, imgbuff, CAMERA_SIZE);          //解压为灰度图像，方便发送到上位机显
        //vcan_sendimg(img, sizeof(img));

        /******************** 发送图像到上位机 ***********************/
        //img_extract(img, imgbuff, CAMERA_SIZE);          //解压为灰度图像，方便发送到上位机显
        //sendimg(img, CAMERA_W * CAMERA_H);                    //发送到上位机
    }
}



 
