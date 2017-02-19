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

//void img_extract(uint8 *dst, uint8 *src, uint32 srclen);

/*!
 *  @brief      main函数
 *  @since      v5.3
 *  @note       山外摄像头 LCD 测试实验
 */
void  main(void)
{
    zet_motor();
    steer();
    zet_oled();
     camera_init(imgbuff);
    //配置中断服务函数
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //设置LPTMR的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //设置LPTMR的中断服务函数为 PORTA_IRQHandler

    while(1)
    {
        camera_get_img();                                   //摄像头获取图像
        img_extract((uint8*)img,imgbuff,CAMERA_SIZE);//二值化图像
        oled_show_picture();
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
  oled_init();
  oled_display_on();
  //oled_show_logo();
  oled_show_num(20,6,10,0,1);
  
}

void zet_camera(void){
    camera_init(imgbuff);
    //配置中断服务函数
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //设置LPTMR的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //设置LPTMR的中断服务函数为 PORTA_IRQHandler

    while(1)
    {
        camera_get_img();                                   //摄像头获取图像
        oled_show_picture();
        //黑白摄像头
        //LCD_Img_Binary_Z(site, size, imgbuff, imgsize);
        //vcan_sendimg(imgbuff,CAMERA_SIZE);

    }
}

/*void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
    uint8 colour[2] = {0,1}; //0 和 1 分别对应的颜色
    //注：野火的摄像头 0 表示 白色，1表示 黑色
    uint8 tmpsrc;
    while(srclen --)
    {
        tmpsrc = *src++;
        *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ]; //判断一个字节从高位到低位
        *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}*/

