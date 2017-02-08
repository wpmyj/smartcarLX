/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_it.c
 * @brief      山外K60 平台中断服务函数
 * @author     山外科技
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
	 
bool Second_Flag;//1s中断标志位，防止程序卡死
	 
/*!
 *  @brief      PORTA中断服务函数
 */
void PORTA_IRQHandler()
{
    uint8  n = 0;    //引脚号
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                    //清中断标志位
	
    n = 27;                              //场中断
    if(flag & (1 << n))                  //PTA27触发中断
    {
        camera_vsync();
    }
}

/*!
 *  @brief      DMA0中断服务函数
 */
void DMA0_IRQHandler()
{
	DMA_IRQ_CLEAN(CAMERA_DMA_CH);//清除通道传输中断标志位
    camera_dma();
}
/*!
 *  @brief      PIT0中断服务函数
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
	PIT_Flag_Clear(PIT0);//清除中断标志位，等待下一次中断进入  
		
}
/*!
 *  @brief      PIT0中断服务函数
 */
void PIT1_IRQHandler()
{
	PIT_Flag_Clear(PIT1);//清除中断标志位，等待下一次中断进入 
	if(Second_Flag == true)
		Second_Flag = false;
	else 
		Second_Flag = true;
}
/*!
 *  @brief      UART0中断服务函数
 */
void UART0_IRQHandler()
{
    unsigned char ch;
    static int n;
    static bool Enable_Recieve_Flag=false;//允许接收标志
    unsigned char Buff[20];//接收数据缓冲区
    UARTn_e uratn = UART0;
	//DisableInterrupts;
    if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   //接收数据寄存器满
    {
        ch =  UART_D_REG(UARTN[uratn]);
        //uart_getchar(uratn,&ch);
        if(ch==0xaa)//开始接收标志
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
