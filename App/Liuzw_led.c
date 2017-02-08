 /*
 * ����ͷ�ļ�
 */
#include "common.h"
#include "MK60_port.h"
#include "MK60_gpio.h"
#include "Liuzw_led.h"


/*
 * ����LED ��Ŷ�Ӧ�Ĺܽ�
 */
PTXn_e LED_PTxn[LED_MAX] = {PTE12};


/*!
 *  @brief      ��ʼ��LED�˿�
 *  @param      LED_e    LED���
 *  @since      v5.0
 *  Sample usage:       led_init (LED0);    //��ʼ�� LED0
 */
void    led_init(LED_e ledn)
{
    if(ledn < LED_MAX)
    {
        gpio_init(LED_PTxn[ledn],GPO,LED_ON);
    }
    else
    {
        ledn = LED_MAX;
        while(ledn--)
        {
            gpio_init(LED_PTxn[ledn],GPO,LED_ON);
        }

    }
}
/*!
 *  @brief      ����LED������
 *  @param      LED_e           LED��ţ�LED0��LED1��LED2��LED3��
 *  @param      LED_status      LED����״̬��LED_ON��LED_OFF��
 *  @since      v5.2
 *  Sample usage:       led_set(LED0,LED_ON);    //���� LED0
 */
void    led_set(LED_e ledn,LED_status status)
{
    if(ledn < LED_MAX)
    {
        gpio_set(LED_PTxn[ledn],status);
    }
    else
    {
        ledn = LED_MAX;
        while(ledn--)
        {
            gpio_set(LED_PTxn[ledn],status);
        }
    }
}
/*!
 *  @brief      ����LED������ת
 *  @param      LED_e           LED��ţ�LED0��LED1��LED2��LED3��
 *  @since      v5.2
 *  Sample usage:       led_turn (LED0);    // LED0������ת
 */
void led_turn(LED_e ledn)
{
    if(ledn < LED_MAX)
    {
        gpio_turn(LED_PTxn[ledn]);
    }
    else
    {
        ledn = LED_MAX;
        while(ledn--)
        {
            gpio_turn(LED_PTxn[ledn]);
        }
    }
}
