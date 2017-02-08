/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_it.c
 * @brief      ɽ��K60 ƽ̨�жϷ�����
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-06-26
 */

#include "common.h"
#include "MK60_it.h"
#include "MK60_dma.h"
#include "MK60_pit.h"
#include "MK60_uart.h"
#include "Liuzw_camera.h"
#include "PC_link.h"
#include "Liuzw_led.h"
#include "Liuzw_control.h"
#include "Liuzw_oled.h"
#include "Liuzw_handle.h"
	 
bool Second_Flag;//1s�жϱ�־λ����ֹ������
	 
/*!
 *  @brief      PORTA�жϷ�����
 */
void PORTA_IRQHandler()
{
    uint8  n = 0;    //���ź�
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                    //���жϱ�־λ
	
    n = 27;                              //���ж�
    if(flag & (1 << n))                  //PTA27�����ж�
    {
        camera_vsync();
    }
}

/*!
 *  @brief      DMA0�жϷ�����
 */
void DMA0_IRQHandler()
{
	DMA_IRQ_CLEAN(CAMERA_DMA_CH);//���ͨ�������жϱ�־λ
    camera_dma();
}
/*!
 *  @brief      PIT0�жϷ�����
 */
void PIT0_IRQHandler()
{
	
	servo_PD_select();
	servo_PD();
	motor_PID();
	//oled_show_num(81,1,Ave_Err,1,1);
	
	//if(Second_Flag == true)
		//led_set(LED1 , LED_ON);
	//else
		//led_set(LED1 , LED_OFF);
	PIT_Flag_Clear(PIT0);//����жϱ�־λ���ȴ���һ���жϽ���  
		
}
/*!
 *  @brief      PIT0�жϷ�����
 */
void PIT1_IRQHandler()
{
	PIT_Flag_Clear(PIT1);//����жϱ�־λ���ȴ���һ���жϽ��� 
	if(Second_Flag == true)
		Second_Flag = false;
	else 
		Second_Flag = true;
}
/*!
 *  @brief      UART0�жϷ�����
 */
void UART0_IRQHandler()
{
    unsigned char ch;
    static int n;
    static bool Enable_Recieve_Flag=false;//������ձ�־
    unsigned char Buff[20];//�������ݻ�����
    UARTn_e uratn = UART0;
	//DisableInterrupts;
    if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   //�������ݼĴ�����
    {
        ch =  UART_D_REG(UARTN[uratn]);
        //uart_getchar(uratn,&ch);
        if(ch==0xaa)//��ʼ���ձ�־
        {
            Enable_Recieve_Flag=true;
            n=0;
        }
        if(Enable_Recieve_Flag==true)
        {
            Buff[n]=ch;
            n++;
        }
    }
}
