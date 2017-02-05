#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "dma.h"
#include "ili9320.h"
#include "dma.h"

#include "ov7725.h"
#include "image_display.h"
#include "i2c.h"

#include "camera.h"
/* �뽫I2C.H�е�I2C_GPIO_SIM ��Ϊ 1 */

// �ı�ͼ���С
//0: 80x60
//1: 160x120
//2: 240x180
#define IMAGE_SIZE  1

#if (IMAGE_SIZE  ==  0)
#define OV7620_W    (80)
#define OV7620_H    (60)

#elif (IMAGE_SIZE == 1)
#define OV7620_W    (160)
#define OV7620_H    (120)

#elif (IMAGE_SIZE == 2)
#define OV7620_W    (240)
#define OV7620_H    (180)

#else
#error "Image Size Not Support!"
#endif

// ͼ���ڴ��
uint8_t gImageBuffer[(OV7620_H)*((OV7620_W/8))];   //ʹ���ڲ�RAM

/* ���Ŷ���PCLK VSYNC HREF ���ӵ�ͬһ��PORT��*/
#define BOARD_OV7620_PCLK_PORT      HW_GPIOA
#define BOARD_OV7620_PCLK_PIN       (24)
#define BOARD_OV7620_VSYNC_PORT     HW_GPIOA
#define BOARD_OV7620_VSYNC_PIN      (25)
#define BOARD_OV7620_HREF_PORT      HW_GPIOB
#define BOARD_OV7620_HREF_PIN       (10)
/* 
����ͷ��������PTA8-PTA15 ֻ������ 0 8 16����ֵ 
0 :PTA0-PTA7
8 :PTA8-PTA15
16:PTA16-PTA24
*/
#define BOARD_OV7620_DATA_OFFSET    (8) 

/* ״̬������*/
typedef enum
{
    kIdle,
    kInCapture,
    kComplete,
}OV7620_Status;

static uint8_t status = kIdle;
    

int SCCB_Init(uint32_t I2C_MAP)
{
    int r;
    uint32_t instance;
    instance = I2C_QuickInit(I2C_MAP, 50*1000);
    r = ov7725_probe(instance);
    if(r)
    {
        return 1;
    }
    r = ov7725_set_image_size(IMAGE_SIZE);
    if(r)
    {
        printf("OV7725 set image error\r\n");
        return 1;
    }
    return 0;
}

//���ж϶�ʹ��PTA�ж�
void OV_ISR(uint32_t index)
{
    static uint32_t v_counter;
    /* ���ж�*/
    if(index & (1 << BOARD_OV7620_VSYNC_PIN))
    {
        switch(status)
        {
            case kIdle:     /* ��ʼ��׽*/
                DMA_SetMajorLoopCounter(HW_DMA_CH2, (OV7620_W/8)*OV7620_H);
                DMA_SetDestAddress(HW_DMA_CH2, (uint32_t)gImageBuffer);
                DMA_EnableRequest(HW_DMA_CH2); 
                status = kInCapture;
                break;
            case kInCapture:  /*��׽��ϣ��ر��ж�*/
                GPIO_ITDMAConfig(BOARD_OV7620_VSYNC_PORT, BOARD_OV7620_VSYNC_PIN, kGPIO_IT_FallingEdge, false);
                PORTA->ISFR = 0xFFFFFFFF;  
                status = kComplete;
                break;
        }
    }
}

void camera_init(){
		uint32_t i;
    int vcount = 0;
    DelayInit();
    /* ��ӡ���ڼ�С��*/
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    printf("OV7725 test\r\n");
    
    //��ʼ��GUI
    CHGUI_Init();
    GUI_printf(0, 0, "OV7725 test");
    
    //�������ͷ
    if(SCCB_Init(I2C0_SCL_PB00_SDA_PB01))
    {
        printf("no ov7725device found!\r\n");
        while(1);
    }
    
    printf("OV7620 setup complete\r\n");
    
    DMA_InitTypeDef DMA_InitStruct1 = {0};
    
    /* ����ͷ�������ж� */
    GPIO_QuickInit(BOARD_OV7620_PCLK_PORT, BOARD_OV7620_PCLK_PIN, kGPIO_Mode_IPD);
    GPIO_QuickInit(BOARD_OV7620_VSYNC_PORT, BOARD_OV7620_VSYNC_PIN, kGPIO_Mode_IPD);
    
    /* install callback */
    GPIO_CallbackInstall(BOARD_OV7620_VSYNC_PORT, OV_ISR);
    
    GPIO_ITDMAConfig(BOARD_OV7620_VSYNC_PORT, BOARD_OV7620_VSYNC_PIN, kGPIO_IT_FallingEdge, true);
    GPIO_ITDMAConfig(BOARD_OV7620_PCLK_PORT, BOARD_OV7620_PCLK_PIN, kGPIO_DMA_RisingEdge, true);
    
    /* ��ʼ�����ݶ˿�*/
    for(i=0;i<8;i++)
    {
        GPIO_QuickInit(HW_GPIOA, BOARD_OV7620_DATA_OFFSET+i, kGPIO_Mode_IFT);
    }
    
    //DMA����
    DMA_InitStruct1.chl = HW_DMA_CH2;
    DMA_InitStruct1.chlTriggerSource = PORTA_DMAREQ;
    DMA_InitStruct1.triggerSourceMode = kDMA_TriggerSource_Normal;
    DMA_InitStruct1.minorLoopByteCnt = 1;
    DMA_InitStruct1.majorLoopCnt = ((OV7620_W/8) +1);
    
    DMA_InitStruct1.sAddr = (uint32_t)&PTA->PDIR + BOARD_OV7620_DATA_OFFSET/8;
    DMA_InitStruct1.sLastAddrAdj = 0;
    DMA_InitStruct1.sAddrOffset = 0;
    DMA_InitStruct1.sDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.sMod = kDMA_ModuloDisable;
    
    DMA_InitStruct1.dAddr = NULL;
    DMA_InitStruct1.dLastAddrAdj = 0;
    DMA_InitStruct1.dAddrOffset = 1;
    DMA_InitStruct1.dDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.dMod = kDMA_ModuloDisable;

    /* initialize DMA moudle */
    DMA_Init(&DMA_InitStruct1);
    
    while(1)
    {
        if(status == kComplete)
        {
            GUI_printf(100,0, "frame:%d", vcount++);
            GUI_DispCCDImage(0, 15, OV7620_W, OV7620_H, gImageBuffer);
            
            /* �����жϣ���ʼ��һ�β�׽ */
            status = kIdle;
            GPIO_ITDMAConfig(BOARD_OV7620_VSYNC_PORT, BOARD_OV7620_VSYNC_PIN, kGPIO_IT_FallingEdge, true);
        }
    }
}