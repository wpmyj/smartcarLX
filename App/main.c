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
#include "MK60_it.h"
#include "Liuzw_oled.h"
#include "Liuzw_camera.h"
#include "Liuzw_handle.h"
#include "Liuzw_control.h"
//#include "Liuzw_menu.h"
//#include "Liuzw_buzzer.h"


//uint8 imgbuff[CAMERA_SIZE];                             //����洢����ͼ�������
//uint8 img[CAMERA_W*CAMERA_H];

//��������
void PORTA_IRQHandler();
void DMA0_IRQHandler();



/*!
 *  @brief      main����
 *  @since      v5.3
 *  @note       ɽ������ͷ LCD ����ʵ��
 */
void  main(void)
{
  /*zet
    Site_t site     = {0, 0};                           //��ʾͼ�����Ͻ�λ��
    Size_t imgsize  = {CAMERA_W, CAMERA_H};             //ͼ���С
    Size_t size;                   //��ʾ����ͼ���С
    size.H = LCD_H;
    size.W = LCD_W;
*/
    camera_init(imgbuff);

    //LCD_str            (site,"Cam init OK!",FCOLOUR,BCOLOUR);
    //�����жϷ�����
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //����LPTMR���жϷ�����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //����LPTMR���жϷ�����Ϊ PORTA_IRQHandler

    while(1)
    {
        camera_get_img();                                   //����ͷ��ȡͼ��

        //�ڰ�����ͷ
        //LCD_Img_Binary_Z(site, size, imgbuff, imgsize);
        oled_show_picture();

        /*����ͼƬ����λ��
        vcan_sendimg(imgbuff,CAMERA_SIZE);
        */
    }
}


/*!
 *  @brief      PORTA�жϷ�����
 *  @since      v5.0
 */
/*void PORTA_IRQHandler()
{
    uint8  n;    //���ź�
    uint32 flag;

    while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //���жϱ�־λ

    n = 29;                                             //���ж�
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


}*/

/*!
 *  @brief      DMA0�жϷ�����
 *  @since      v5.0
 */
/*void DMA0_IRQHandler()
{
    camera_dma();
}*/


