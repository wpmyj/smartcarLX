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

//void img_extract(uint8 *dst, uint8 *src, uint32 srclen);

/*!
 *  @brief      main����
 *  @since      v5.3
 *  @note       ɽ������ͷ LCD ����ʵ��
 */
void  main(void)
{
    zet_motor();
    steer();
    zet_oled();
     camera_init(imgbuff);
    //�����жϷ�����
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //����LPTMR���жϷ�����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //����LPTMR���жϷ�����Ϊ PORTA_IRQHandler

    while(1)
    {
        camera_get_img();                                   //����ͷ��ȡͼ��
        img_extract((uint8*)img,imgbuff,CAMERA_SIZE);//��ֵ��ͼ��
        oled_show_picture();
        //�ڰ�����ͷ
        //LCD_Img_Binary_Z(site, size, imgbuff, imgsize);
        vcan_sendimg(imgbuff,CAMERA_SIZE);

    }
    //zet_camera();   
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
    ftm_pwm_duty(FTM2,FTM_CH1,56);
    
    ftm_pwm_init(FTM2,FTM_CH0,10000,0);//����FTM��ʼ��
    gpio_init(PTB17,GPO,0);//��������ʹ�ܳ�ʼ��
    gpio_init(PTB16,GPO,0);//��������ʹ�ܳ�ʼ��
    gpio_set(PTB17,0);//��������ʹ�ܳ�ʼ��
    gpio_set(PTB16,1);//��������ʹ�ܳ�ʼ��
    ftm_pwm_duty(FTM2,FTM_CH0,56);
}

void steer(void){
  ftm_pwm_init(FTM0,FTM_CH3,100,5000);//���FTM��ʼ��
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
    //�����жϷ�����
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //����LPTMR���жϷ�����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //����LPTMR���жϷ�����Ϊ PORTA_IRQHandler

    while(1)
    {
        camera_get_img();                                   //����ͷ��ȡͼ��
        oled_show_picture();
        //�ڰ�����ͷ
        //LCD_Img_Binary_Z(site, size, imgbuff, imgsize);
        //vcan_sendimg(imgbuff,CAMERA_SIZE);

    }
}

/*void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
    uint8 colour[2] = {0,1}; //0 �� 1 �ֱ��Ӧ����ɫ
    //ע��Ұ�������ͷ 0 ��ʾ ��ɫ��1��ʾ ��ɫ
    uint8 tmpsrc;
    while(srclen --)
    {
        tmpsrc = *src++;
        *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ]; //�ж�һ���ֽڴӸ�λ����λ
        *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}*/

