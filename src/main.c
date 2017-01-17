/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,????
 *     All rights reserved.
 *     ????:???? http://www.vcan123.com
 *
 *     ??????,????????????????,????,????????,
 *     ???????????????????
 *
 * @file       main.c
 * @brief      ??KL26 ?????
 * @author     ????
 * @version    v5.2
 * @date       2014-10-26
 */

#include "common.h"
#include "include.h"

#define MOTOR_HZ    (10*1000)
 
void main()
{
    tpm_pwm_init(TPM0,TPM_CH0,10*1000,0);
    tpm_pwm_init(TPM2,TPM_CH0,80,1000);
    //tpm_pwm_init(TPM2,TPM_CH0,100,8590);
    gpio_init(PTC1,GPO,1);
    gpio_init(PTC2,GPO,0);
    tpm_pwm_duty(TPM0,TPM_CH0,76);
    
    uint8 i;
    while(1)
    {

        for(i = 101;i<122;i++)
        {
            tpm_pwm_duty(TPM2, TPM_CH0,1000-i);
            DELAY_MS(1200);
        }

        for(;i>101;i--)
        {
            tpm_pwm_duty(TPM2, TPM_CH0,1000-i);
            DELAY_MS(1200);
        }
    }
}
