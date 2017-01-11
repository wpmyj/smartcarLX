#include <stdio.h>

#include "common.h"
#include "gpio.h"
#include "uart.h"


int main(void)
{
	DelayInit(); 
	GPIO_Init(HW_GPIOC,1,kGPIO_OPPL);
	GPIO_Init(HW_GPIOC,2,kGPIO_OPPL);
	UART_Init(UART0_RX_PD06_TX_PD07,115200);
	printf("GPIO Interrupt, press key to test\r\n");
	uint32_t i=0;
	
	while(i<15)
	{
		GPIO_PinToggle(HW_GPIOC,2);
		GPIO_SetIntMode(HW_GPIOC, 2, kGPIO_Int_RE, true);
		DelayMs(500);
		i++;
	}	
}

void PORTC_IRQHandler(void)
{
	PORTC->ISFR = (1<<2);
	printf("ÖÐ¶ÏÀ´ÁË\r\n");
	uint16_t i=0;
	while(i<6)
	{
		GPIO_PinToggle(HW_GPIOC,1);
		DelayMs(500);
		i++;
	}
	return;
}
