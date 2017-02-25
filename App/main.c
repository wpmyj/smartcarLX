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
//void PIT0_IRQHandler(void);
void zf_oled(int16 val);
void PIT1_IRQHandler();
void xx_bluetooth();
void uart3_handler(void);
//void img_extract(uint8 *dst, uint8 *src, uint32 srclen);

/*!
 *  @brief      main函数
 *  @since      v5.3
 *  @note       山外摄像头 LCD 测试实验
   */
void  main(void)
{
     //zet_bluetooth();
  
     
     DisableInterrupts;
     NVIC_SetPriority(PORTA_IRQn,0);
     NVIC_SetPriority(DMA0_IRQn,1);
     
     Init_All();
     
    // pit_init_ms(PIT1, 10);
     EnableInterrupts;
     
     //set_vector_handler(PORTC_PORTD_VECTORn ,PORTC_PORTD_IRQHandler);
     
     set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);  
     set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);    
     //set_vector_handler(PIT1_VECTORn , PIT1_IRQHandler);    
    // enable_irq (PIT1_IRQn);
     
     //设置 PORTE 的中断服务函数为 PORTE_VECTORn
     //enable_irq(PORTC_PORTD_IRQn);
     //int a=nrf_link_check();
    while(1)
    {
        camera_get_img();                                   //摄像头获取图像
        img_extract((uint8*)img,imgbuff,CAMERA_SIZE);//二值化图像
        dis_bmp(CAMERA_H,CAMERA_W,(uint8*)img,0x7F);       
        Search_Line();
        Find_Middle();
        //vcan_sendimg(imgbuff,CAMERA_SIZE);
        
//        race[0]=2;
//        race[1]=5;
//        race[2]=7;
//        nrf_tx(race,2);
        //nrf_rx
//        while(nrf_tx_state() == NRF_TXING);             //等待发送完成
    }
    
    
  /* ftm_quad_init(FTM1); 
   pit_init_ms(PIT0, 500);
    zet_motor();
    steer();
    zet_oled();
    xx_bluetooth();
     camera_init(imgbuff);
    //配置中断服务函数
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //设置LPTMR的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //设置LPTMR的中断服务函数为 PORTA_IRQHandler
    //set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);
    enable_irq (PIT0_IRQn);  
    
    while(1)
    {
        camera_get_img();                                   //摄像头获取图像
        img_extract((uint8*)img,imgbuff,CAMERA_SIZE);//二值化图像
        dis_bmp(CAMERA_H,CAMERA_W,(uint8*)img,0x7F); 
        //黑白摄像头
        //LCD_Img_Binary_Z(site, size, imgbuff, imgsize);
        vcan_sendimg(imgbuff,CAMERA_SIZE);

    }*/
    //zet_camera();   
}

void PIT1_IRQHandler(){
  Search_Line();
  Find_Middle();
  Servo_control();
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
    ftm_pwm_duty(FTM2,FTM_CH1,66);
    
    ftm_pwm_init(FTM2,FTM_CH0,10000,0);//驱动FTM初始化
    gpio_init(PTB17,GPO,0);//驱动正向使能初始化
    gpio_init(PTB16,GPO,0);//驱动反向使能初始化
    gpio_set(PTB17,0);//驱动正向使能初始化
    gpio_set(PTB16,1);//驱动正向使能初始化
    ftm_pwm_duty(FTM2,FTM_CH0,66);
}

void steer(void){
  ftm_pwm_init(FTM0,FTM_CH3,100,8500);//舵机FTM初始化
}

void zet_oled(){
  
  //oled_display_on();
  //oled_show_logo();
  //OLED_Print_Num(20,6,10);
  //systick_delay_ms(2000);
  OLED_Fill(0x00);
}


void zf_oled(int16 val){
  OLED_ClrPixel(70,88);
  OLED_Print_Num(70,88,val);
  systick_delay_ms(100);
}

void uart3_handler(void)
{
    char ch;

    if(uart_query    (UART5) == 1)   //接收数据寄存器满
    {
        //用户需要处理接收数据
        uart_getchar   (UART5, &ch);                    //无限等待接受1个字节
        uart_putchar   (UART5 , ch);                    //发送字符串
        uart_putstr   (UART5 ,"\n\n\n接收中断测试：");
    }
}

void xx_bluetooth()
{
    uart_init(UART5,9600);     //初始化串口(UART3 是工程里配置为printf函数输出端口，故已经进行初始化)
    //uart_putstr   (UART5 ,"\n\n\n接收中断测试：");           //发送字符串
    set_vector_handler(UART5_RX_TX_VECTORn,uart3_handler);   // 设置中断服务函数到中断向量表里
    uart_rx_irq_en (UART5);                                 //开串口接收中断
}