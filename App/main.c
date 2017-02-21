/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       main.c
 * @brief      山外K60 平台主程序
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-28
 */

#include "common.h"
#include "include.h"

uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组
//uint8 img[CAMERA_W*CAMERA_H];

//函数声明
void PORTA_IRQHandler();
void DMA0_IRQHandler();
void zet_motor(void);
void steer(void);
void zet_oled();
void zet_camera();
void PIT0_IRQHandler(void);
void zf_oled(int16 val);
//void img_extract(uint8 *dst, uint8 *src, uint32 srclen);

/*!
 *  @brief      main函数
 *  @since      v5.3
 *  @note       山外摄像头 LCD 测试实验
 */
void  main(void)
{
   ftm_quad_init(FTM1); 
   pit_init_ms(PIT0, 500);
    zet_motor();
    steer();
    zet_oled();
     camera_init(imgbuff);
    //配置中断服务函数
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //设置LPTMR的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //设置LPTMR的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);
    enable_irq (PIT0_IRQn);  
    
    while(1)
    {
        camera_get_img();                                   //摄像头获取图像
        img_extract((uint8*)img,imgbuff,CAMERA_SIZE);//二值化图像
        dis_bmp(CAMERA_H,CAMERA_W,(uint8*)img,0x7F); 
        //黑白摄像头
        //LCD_Img_Binary_Z(site, size, imgbuff, imgsize);
        vcan_sendimg(imgbuff,CAMERA_SIZE);

    }
    //zet_camera();   
}


/*!
 *  @brief      PORTA中断服务函数
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n;    //引脚号
    uint32 flag;

    while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //清中断标志位

    n = 25;                                             //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //使用行中断
    n = 28;
    if(flag & (1 << n))                                 //PTA28触发中断
    {
        camera_href();
    }
#endif


}

/*!
 *  @brief      DMA0中断服务函数
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}


void zet_motor(void){
    ftm_pwm_init(FTM2,FTM_CH1,10000,0);//驱动FTM初始化
    gpio_init(PTC3,GPO,0);//驱动正向使能初始化
    gpio_init(PTC2,GPO,0);//驱动反向使能初始化
    gpio_set(PTC3,1);//驱动正向使能初始化
    gpio_set(PTC2,0);//驱动正向使能初始化
    ftm_pwm_duty(FTM2,FTM_CH1,56);
    
    ftm_pwm_init(FTM2,FTM_CH0,10000,0);//驱动FTM初始化
    gpio_init(PTB17,GPO,0);//驱动正向使能初始化
    gpio_init(PTB16,GPO,0);//驱动反向使能初始化
    gpio_set(PTB17,0);//驱动正向使能初始化
    gpio_set(PTB16,1);//驱动正向使能初始化
    ftm_pwm_duty(FTM2,FTM_CH0,56);
}

void steer(void){
  ftm_pwm_init(FTM0,FTM_CH3,100,5000);//舵机FTM初始化
  ftm_pwm_duty(FTM0,FTM_CH3,8650);
}

void zet_oled(){
  OLED_Init();
  //oled_display_on();
  //oled_show_logo();
  //OLED_Print_Num(20,6,10);
  //systick_delay_ms(2000);
  OLED_Fill(0x00);
}

void PIT0_IRQHandler(void)
{

    int16 val;
    val = ftm_quad_get(FTM1);          //获取FTM 正交解码 的脉冲数(负数表示反方向)
    ftm_quad_clean(FTM1);
    //zf_oled( val);
    if(val>=0)
    {
        printf("\n正转：%d",val);
    }
    else
    {
        printf("\n反转：%d",-val);
    }

    PIT_Flag_Clear(PIT0);       //清中断标志位
}
void zf_oled(int16 val){
  OLED_ClrPixel(70,88);
  OLED_Print_Num(70,88,val);
  systick_delay_ms(100);
}
