 /*!
  *     COPYRIGHT NOTICE
  *     Copyright (c) 2013,ɽ��Ƽ�
  *     All rights reserved. 
  *     �������ۣ�ɽ����̳ http://www.vcan123.com
  *
  *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
  *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
  *
  * @file       VCAN_LED.h
  * @brief      led����ͷ�ļ�
  * @author     ɽ��Ƽ�
  * @version    v5.0
  * @date       2013-07-9
  */

#ifndef __LIUZW_LED_H__
#define __LIUZW_LED_H__


/*! ö��LED��� */
typedef enum
{
  LED0,
  LED_MAX,
}LED_e;

/*! ö��LED����״̬ */
typedef enum 
{
  LED_ON  =   1,            //����(��Ӧ�ߵ�ƽ)
  LED_OFF =   0             //�ư�(��Ӧ�͵�ƽ)
}LED_status; 


extern void    led_init(LED_e);            //��ʼ��LED�˿�
extern void    led_set(LED_e,LED_status);  //����LED������
extern void    led_turn(LED_e);            //����LED������ת

#endif  //__VCAN_LED_H__