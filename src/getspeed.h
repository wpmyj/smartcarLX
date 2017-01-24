#include "gpio.h"
#include "dma.h"

static void DMA_PulseCountInit(uint32_t dmaChl, uint32_t instance, uint32_t pinIndex);

void encoder_Init();

void get_Speed(void);