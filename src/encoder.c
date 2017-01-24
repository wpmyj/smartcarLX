#include "encoder.h"

/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请修改 CMSIS标准文件 system_MKxxxx.c 中的 CLOCK_SETUP 宏 */

static uint32_t ChlValue[2];
static uint32_t R_speed=0;
static uint32_t L_speed=0;

static const uint32_t DMA_PORT_TriggerSourceTable[] = 
{
    PORTA_DMAREQ,
    PORTB_DMAREQ,
    PORTC_DMAREQ,
    PORTD_DMAREQ,
    PORTE_DMAREQ,
};

static void DMA_PulseCountInit(uint32_t dmaChl, uint32_t instance, uint32_t pinIndex)
{
    /* 开启2路引脚 配置为DMA触发 */
    GPIO_QuickInit(instance, pinIndex, kGPIO_Mode_IFT);
    /* 配置为DMA上升沿触发 */
    GPIO_ITDMAConfig(instance, pinIndex, kGPIO_DMA_RisingEdge, true);
    /* 配置DMA */
    static uint8_t dummy1, dummy2;
    DMA_InitTypeDef DMA_InitStruct1 = {0};  
    DMA_InitStruct1.chl = dmaChl;  
    DMA_InitStruct1.chlTriggerSource = DMA_PORT_TriggerSourceTable[instance];
    DMA_InitStruct1.triggerSourceMode = kDMA_TriggerSource_Normal; 
    DMA_InitStruct1.minorLoopByteCnt = 1;
    DMA_InitStruct1.majorLoopCnt = DMA_CITER_ELINKNO_CITER_MASK; /* 最大值 */
    
    DMA_InitStruct1.sAddr = (uint32_t)&dummy1;
    DMA_InitStruct1.sLastAddrAdj = 0; 
    DMA_InitStruct1.sAddrOffset = 0;
    DMA_InitStruct1.sDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.sMod = kDMA_ModuloDisable;
    
    DMA_InitStruct1.dAddr = (uint32_t)&dummy2; 
    DMA_InitStruct1.dLastAddrAdj = 0;
    DMA_InitStruct1.dAddrOffset = 0; 
    DMA_InitStruct1.dDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.dMod = kDMA_ModuloDisable;
    DMA_Init(&DMA_InitStruct1);
    /* 启动传输 */
    DMA_EnableRequest(dmaChl);
}

static void getencoder(){
		 /* 由于DMA 是倒计数的 所需需要用最大值减一下 */
    /* CH0 */
    ChlValue[0] = DMA_CITER_ELINKNO_CITER_MASK - DMA_GetMajorLoopCount(HW_DMA_CH0);
    /* CH1 */
    ChlValue[1] = DMA_CITER_ELINKNO_CITER_MASK - DMA_GetMajorLoopCount(HW_DMA_CH1);
    /* 清零计数 */
    DMA_CancelTransfer();
    DMA_SetMajorLoopCounter(HW_DMA_CH0, DMA_CITER_ELINKNO_CITER_MASK);
    DMA_SetMajorLoopCounter(HW_DMA_CH1, DMA_CITER_ELINKNO_CITER_MASK);
    /* 开始下一次传输 */
    DMA_EnableRequest(HW_DMA_CH0);
    DMA_EnableRequest(HW_DMA_CH1);
}

void getSpeed(){
	  /* 开启DMA捕捉引脚脉冲信号 (每个端口只能测量一路DMA 也就是说DMA脉冲最多只能测量5路(PTA,PTB,PTC,PTD,PTE))*/
	
    DMA_PulseCountInit(HW_DMA_CH0, HW_GPIOC, 0);//encoder1
    DMA_PulseCountInit(HW_DMA_CH1, HW_GPIOB, 2);//encoder2
	
}

