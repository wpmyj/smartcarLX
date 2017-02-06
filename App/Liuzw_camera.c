/*
 * @file       Liuzw_camera.c
 * @brief      ����ͷ��������
 * @author     ������
 * @date       2016-01-20
 */
#include "common.h"
#include "include.h"
#include "Liuzw_camera.h"
#include "Liuzw_camera_reg.h"
#include "VCAN_SCCB.h"
#include "Liuzw_oled.h"
#include "Liuzw_handle.h"

uint8   *camera_img_buff;//ͼ��ɼ�������
volatile IMG_STATUS_e   camera_img_flag = IMG_FINISH;//ͼ��״̬

//�ڲ���������
static uint8 camera_reg_init(void);
static void camera_port_init(void);
/*
 *  @brief   ����ͷ��ʼ��
 */
uint8 camera_init(uint8 *imgaddr)
{
    camera_img_buff = imgaddr;
    while(camera_reg_init() == 0);
    camera_port_init();
    return 0;
}
/*
 *  @brief   ����ͷ���ų�ʼ��
 */
void camera_port_init(void)
{	
    dma_portx2buff_init(CAMERA_DMA_CH,                //DMAͨ��ѡ��
						(void *)&PTB_B0_IN,           //����Դ(Ĭ��������)
						(void *)camera_img_buff,      //Դ��ַ
						CAMERA_PCLK_PIN,              //Ŀ�ĵ�ַ
						DMA_BYTE1,                    //ÿ�δ���1Byte
						CAMERA_DMA_NUM,               //DMA�ɼ�����
						DADDR_KEEPON);                //������ɺ󱣳�Ŀ�ĵ�ַ
    DMA_DIS(CAMERA_DMA_CH);                           //��ֹͨ��Ӳ��DMA����
    disable_irq(PORTA_IRQn);                          //�ر�PTA���ж�
    DMA_IRQ_CLEAN(CAMERA_DMA_CH);                     //���ͨ�������жϱ�־λ
    DMA_IRQ_EN(CAMERA_DMA_CH);                        //����DMAͨ����������ж�   
	
    port_init(CAMERA_VSYNC_PIN,ALT1 | IRQ_FALLING | PULLDOWN );//���ж�,�½��ش����ж�, ����
    port_init(CAMERA_PCLK_PIN, ALT1 | DMA_FALLING | PULLDOWN );//PCLK,  �����ش����жϣ�����
    //port_init(CAMERA_HREF_PIN, ALT1 | IRQ_FALLING | PULLDOWN );//���ж�,�½��ش�����    ����
}
/*
 *  @brief   ����ͷ�Ĵ�����ʼ��
 */
uint8 camera_reg_init(void)
{
    uint16 i = 0;
    uint8 Sensor_IDCode = 0;
    SCCB_GPIO_init();
    if( 0 == SCCB_WriteByte ( OV7725_COM7, 0x80 ) ) /*��λsensor */
    {
		//oled_clear();
        oled_show_string(0,0,"SCCB write data "
						     "error!          "
							 "                "
							 "                ",1,1);
        //printf("\n����:SCCBд���ݴ���");
        return 0 ;
    } 
    DELAY_MS(50);
    if( 0 == SCCB_ReadByte( &Sensor_IDCode, 1, OV7725_VER ) ) /* ��ȡsensor ID��*/
    {
		//oled_clear();
		oled_show_string(0,0, "Warning:failed t"
                              "o read ID!      "
                          	  "                "
                              "                ",1,1);
        //printf("\n����:��ȡIDʧ��");
        return 0;
    }
	//oled_clear();
	oled_show_string(0,0, "Get ID success, "
                          "                "
                          "                "
                          "                ",1,1);
	//DELAY_MS(500);
    //printf("\nGet ID success��SENSOR ID is 0x%x", Sensor_IDCode);
    if(Sensor_IDCode == OV7725_ID)
    {
        for( i = 0 ; i < ARR_SIZE(Camera_Reg_Tab); i++ )
        {
            if( 0 == SCCB_WriteByte(Camera_Reg_Tab[i].addr, Camera_Reg_Tab[i].val) )
            {
				//oled_clear();
				oled_show_string(0,0, "Warning:failed t"
                                      "o write regiser!"
                                      "                "
                                      "                ",1,1);
                //printf("\n����:д�Ĵ���0x%xʧ��", Camera_Reg_Tab[i].addr);
                return 0;
            }
        }
    }
    else
    {
        return 0;
    }
    //printf("\nOV7725 Register Config Success!");
	//oled_clear();
	oled_show_string(0,0, "OV7725 Register "
                          "Config Success! "
                          "                "
                          "                ",1,1);
	//DELAY_MS(1500);//��ʾ���óɹ�����һ��ʱ��
    return 1;
}
/*
 *  @brief      ov7725���жϷ�����
 */
void camera_vsync(void)
{

    //���ж���Ҫ�ж��Ƿ��ǳ���ʼ
    if(camera_img_flag == IMG_START)                   //��Ҫ��ʼ�ɼ�ͼ��
    {
        camera_img_flag = IMG_GATHER;                  //���ͼ��ɼ���
        disable_irq(PORTA_IRQn);

#if 1

        PORTA_ISFR = 1 <<  PT28;            //���PCLK��־λ

        DMA_EN(CAMERA_DMA_CH);              //ʹ��ͨ��CHn Ӳ������
        PORTA_ISFR = 1 <<  PT28;            //���PCLK��־λ
        DMA_DADDR(CAMERA_DMA_CH) = (uint32)camera_img_buff;    //�ָ���ַ

#else
        PORTA_ISFR = 1 <<  PT28;            //���PCLK��־λ
        dma_repeat(CAMERA_DMA_CH, (void *)&PTB_B0_IN, (void *)ov7725_eagle_img_buff,CAMERA_DMA_NUM);
#endif
    }
    else                                        //ͼ��ɼ�����
    {
        disable_irq(PORTA_IRQn);                        //�ر�PTA���ж�
        camera_img_flag = IMG_FAIL;                     //���ͼ��ɼ�ʧ��
    }
}

/*!
 *  @brief    DMA�жϷ�����,�ɼ���һ�������ж�
 */
void camera_dma()
{
    camera_img_flag = IMG_FINISH ;
}

/*!
 *  @brief      �ɼ�ͼ�񣨲ɼ��������ݴ洢�ڳ�ʼ��ʱ���õĵ�ַ�ϣ�
 */
void camera_get_img()
{
    camera_img_flag = IMG_START;            //��ʼ�ɼ�ͼ��
    PORTA_ISFR = ~0;                        //д1���жϱ�־λ(����ģ���Ȼ�ᵼ��һ���жϾ����ϴ����ж�)
    enable_irq(PORTA_IRQn);                 //����PTA���ж�
    while(camera_img_flag != IMG_FINISH)    //�ȴ�ͼ��ɼ����
    {
        if(camera_img_flag == IMG_FAIL)     //����ͼ��ɼ����������¿�ʼ�ɼ�
        {
            camera_img_flag = IMG_START;    //��ʼ�ɼ�ͼ��
            PORTA_ISFR = ~0;                //д1���жϱ�־λ(����ģ���Ȼ�ᵼ��һ���жϾ����ϴ����ж�)
            enable_irq(PORTA_IRQn);         //����PTA���ж�
        }
    }
}
/*
*
*/
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
    uint8 colour[2] = {0,1}; //0 �� 1 �ֱ��Ӧ����ɫ
    //ע��Ұ�������ͷ 0 ��ʾ ��ɫ��1��ʾ ��ɫ
    uint8 tmpsrc;
    while(srclen --)
    {
        tmpsrc = *src++;
        *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ]; //�ж�һ���ֽڴӸ�λ����λ
        *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}
