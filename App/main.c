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

float Step[8]={0.0001,0.001,0.01,0.1,1.0,10.0,100.0,1000.0};   //Ĭ�ϵ��ڲ���Ϊ0.01

void zet_motor(void);
void steer(void);
void zet_oled();
void zet_camera();

/*void OLED_Draw_UI()
{
     uint8 i;
     OLED_P6x8Str(0,0,"Voltage=");                          //��ʾ��ص�ѹ
     OLED_PrintValueF(48, 0,Voltage,2);                     
     OLED_PrintValueF(72, 0,Step[Step_Index],5);            //��ʾ���ڲ���ֵ
     
    for(i=0;i<7;i++)
    {
     if(i==Para_Index&&Para_Checked==false)
      {
       reverse=1;
       OLED_P6x8Str(0,i+1,Para_Name[i]);   //����������ת��ʾ
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

void zet_oled(){
  oled_init();
  oled_display_on();
  oled_show_logo();
  DELAY_MS(2000);
  oled_clear();
}

void zet_camera(void){
/*  Site_t site     = {0, 0};                           //��ʾͼ�����Ͻ�λ��
    Size_t imgsize  = {CAMERA_W, CAMERA_H};             //ͼ���С
    Size_t size;                   //��ʾ����ͼ���С
*/
    //LCD_init();
    //LCD_str            (site,"Cam init ing",FCOLOUR,BCOLOUR);

    //size.H = LCD_H;
    //size.W = LCD_W;

    camera_init(imgbuff);

    //LCD_str            (site,"Cam init OK!",FCOLOUR,BCOLOUR);
    //site.y = 110;
    //LCD_FSTR_CH(site,vcan_str,FCOLOUR,BCOLOUR);

    //�����жϷ�����
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //���� PORTA ���жϷ�����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //���� DMA0 ���жϷ�����Ϊ PORTA_IRQHandler

    while(1)
    {
        camera_get_img();                                   //����ͷ��ȡͼ��
        img_extract((uint8*)img,imgbuff,CAMERA_SIZE);//��ֵ��ͼ��

        oled_show_picture();//oled��ʾ�ɼ���ͼ��    
                      //�ڰ�����ͷ
        //LCD_Img_Binary_Z(site, size, imgbuff, imgsize);

        /******************** ɽ��๦�ܵ������� ���ڰ�ģʽ�� ����ͼ����λ�� ***********************/
        //vcan_sendimg(imgbuff, sizeof(imgbuff));

        /******************** ɽ��๦�ܵ������� ���Ҷ�ģʽ�� ����ͼ����λ�� ***********************/
        //img_extract(img, imgbuff, CAMERA_SIZE);          //��ѹΪ�Ҷ�ͼ�񣬷��㷢�͵���λ����
        //vcan_sendimg(img, sizeof(img));

        /******************** ����ͼ����λ�� ***********************/
        //img_extract(img, imgbuff, CAMERA_SIZE);          //��ѹΪ�Ҷ�ͼ�񣬷��㷢�͵���λ����
        //sendimg(img, CAMERA_W * CAMERA_H);                    //���͵���λ��
    }
}



 
