/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       main.c
 * @brief      ɽ��K60 ƽ̨������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-08-28
 */

#include "common.h"
#include "include.h"

uint8 imgbuff[CAMERA_SIZE];                             //����洢����ͼ�������
//uint8 img[CAMERA_W*CAMERA_H];

//��������
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
 *  @brief      main����
 *  @since      v5.3
 *  @note       ɽ������ͷ LCD ����ʵ��
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
     
     //���� PORTE ���жϷ�����Ϊ PORTE_VECTORn
     //enable_irq(PORTC_PORTD_IRQn);
     //int a=nrf_link_check();
    while(1)
    {
        camera_get_img();                                   //����ͷ��ȡͼ��
        img_extract((uint8*)img,imgbuff,CAMERA_SIZE);//��ֵ��ͼ��
        dis_bmp(CAMERA_H,CAMERA_W,(uint8*)img,0x7F);       
        Search_Line();
        Find_Middle();
        //vcan_sendimg(imgbuff,CAMERA_SIZE);
        
//        race[0]=2;
//        race[1]=5;
//        race[2]=7;
//        nrf_tx(race,2);
        //nrf_rx
//        while(nrf_tx_state() == NRF_TXING);             //�ȴ��������
    }
    
    
  /* ftm_quad_init(FTM1); 
   pit_init_ms(PIT0, 500);
    zet_motor();
    steer();
    zet_oled();
    xx_bluetooth();
     camera_init(imgbuff);
    //�����жϷ�����
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //����LPTMR���жϷ�����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //����LPTMR���жϷ�����Ϊ PORTA_IRQHandler
    //set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);
    enable_irq (PIT0_IRQn);  
    
    while(1)
    {
        camera_get_img();                                   //����ͷ��ȡͼ��
        img_extract((uint8*)img,imgbuff,CAMERA_SIZE);//��ֵ��ͼ��
        dis_bmp(CAMERA_H,CAMERA_W,(uint8*)img,0x7F); 
        //�ڰ�����ͷ
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
 *  @brief      PORTA�жϷ�����
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n;    //���ź�
    uint32 flag;

    while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //���жϱ�־λ

    n = 25;                                             //���ж�
    if(flag & (1 << n))                                 //PTA29�����ж�
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //ʹ�����ж�
    n = 28;
    if(flag & (1 << n))                                 //PTA28�����ж�
    {
        camera_href();
    }
#endif


}

/*!
 *  @brief      DMA0�жϷ�����
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}


void zet_motor(void){
    ftm_pwm_init(FTM2,FTM_CH1,10000,0);//����FTM��ʼ��
    gpio_init(PTC3,GPO,0);//��������ʹ�ܳ�ʼ��
    gpio_init(PTC2,GPO,0);//��������ʹ�ܳ�ʼ��
    gpio_set(PTC3,1);//��������ʹ�ܳ�ʼ��
    gpio_set(PTC2,0);//��������ʹ�ܳ�ʼ��
    ftm_pwm_duty(FTM2,FTM_CH1,66);
    
    ftm_pwm_init(FTM2,FTM_CH0,10000,0);//����FTM��ʼ��
    gpio_init(PTB17,GPO,0);//��������ʹ�ܳ�ʼ��
    gpio_init(PTB16,GPO,0);//��������ʹ�ܳ�ʼ��
    gpio_set(PTB17,0);//��������ʹ�ܳ�ʼ��
    gpio_set(PTB16,1);//��������ʹ�ܳ�ʼ��
    ftm_pwm_duty(FTM2,FTM_CH0,66);
}

void steer(void){
  ftm_pwm_init(FTM0,FTM_CH3,100,8500);//���FTM��ʼ��
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

    if(uart_query    (UART5) == 1)   //�������ݼĴ�����
    {
        //�û���Ҫ�����������
        uart_getchar   (UART5, &ch);                    //���޵ȴ�����1���ֽ�
        uart_putchar   (UART5 , ch);                    //�����ַ���
        uart_putstr   (UART5 ,"\n\n\n�����жϲ��ԣ�");
    }
}

void xx_bluetooth()
{
    uart_init(UART5,9600);     //��ʼ������(UART3 �ǹ���������Ϊprintf��������˿ڣ����Ѿ����г�ʼ��)
    //uart_putstr   (UART5 ,"\n\n\n�����жϲ��ԣ�");           //�����ַ���
    set_vector_handler(UART5_RX_TX_VECTORn,uart3_handler);   // �����жϷ��������ж���������
    uart_rx_irq_en (UART5);                                 //�����ڽ����ж�
}