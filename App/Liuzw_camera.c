/*
 * @file       Liuzw_camera.c
 * @brief      摄像头驱动函数
 * @author     刘忠威
 * @date       2016-01-20
 */
#include "common.h"
#include "include.h"
#include "Liuzw_camera.h"
#include "Liuzw_camera_reg.h"
#include "VCAN_SCCB.h"
#include "Liuzw_oled.h"
#include "Liuzw_handle.h"

uint8   *camera_img_buff;//图像采集缓冲区
volatile IMG_STATUS_e   camera_img_flag = IMG_FINISH;//图像状态

//内部函数声明
static uint8 camera_reg_init(void);
static void camera_port_init(void);
/*
 *  @brief   摄像头初始化
 */
uint8 camera_init(uint8 *imgaddr)
{
    camera_img_buff = imgaddr;
    while(camera_reg_init() == 0);
    camera_port_init();
    return 0;
}
/*
 *  @brief   摄像头引脚初始化
 */
void camera_port_init(void)
{	
    dma_portx2buff_init(CAMERA_DMA_CH,                //DMA通道选择
						(void *)&PTB_B0_IN,           //触发源(默认上升沿)
						(void *)camera_img_buff,      //源地址
						CAMERA_PCLK_PIN,              //目的地址
						DMA_BYTE1,                    //每次传输1Byte
						CAMERA_DMA_NUM,               //DMA采集次数
						DADDR_KEEPON);                //传输完成后保持目的地址
    DMA_DIS(CAMERA_DMA_CH);                           //禁止通道硬件DMA请求
    disable_irq(PORTA_IRQn);                          //关闭PTA的中断
    DMA_IRQ_CLEAN(CAMERA_DMA_CH);                     //清除通道传输中断标志位
    DMA_IRQ_EN(CAMERA_DMA_CH);                        //允许DMA通道传输完成中断   
	
    port_init(CAMERA_VSYNC_PIN,ALT1 | IRQ_FALLING | PULLDOWN );//场中断,下降沿触发中断, 下拉
    port_init(CAMERA_PCLK_PIN, ALT1 | DMA_FALLING | PULLDOWN );//PCLK,  上升沿触发中断，下拉
    //port_init(CAMERA_HREF_PIN, ALT1 | IRQ_FALLING | PULLDOWN );//行中断,下降沿触发，    下拉
}
/*
 *  @brief   摄像头寄存器初始化
 */
uint8 camera_reg_init(void)
{
    uint16 i = 0;
    uint8 Sensor_IDCode = 0;
    SCCB_GPIO_init();
    if( 0 == SCCB_WriteByte ( OV7725_COM7, 0x80 ) ) /*复位sensor */
    {
		//oled_clear();
        oled_show_string(0,0,"SCCB write data "
						     "error!          "
							 "                "
							 "                ",1,1);
        //printf("\n警告:SCCB写数据错误");
        return 0 ;
    } 
    DELAY_MS(50);
    if( 0 == SCCB_ReadByte( &Sensor_IDCode, 1, OV7725_VER ) ) /* 读取sensor ID号*/
    {
		//oled_clear();
		oled_show_string(0,0, "Warning:failed t"
                              "o read ID!      "
                          	  "                "
                              "                ",1,1);
        //printf("\n警告:读取ID失败");
        return 0;
    }
	//oled_clear();
	oled_show_string(0,0, "Get ID success, "
                          "                "
                          "                "
                          "                ",1,1);
	//DELAY_MS(500);
    //printf("\nGet ID success，SENSOR ID is 0x%x", Sensor_IDCode);
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
                //printf("\n警告:写寄存器0x%x失败", Camera_Reg_Tab[i].addr);
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
	//DELAY_MS(1500);//显示配置成功持续一段时间
    return 1;
}
/*
 *  @brief      ov7725场中断服务函数
 */
void camera_vsync(void)
{

    //场中断需要判断是否是场开始
    if(camera_img_flag == IMG_START)                   //需要开始采集图像
    {
        camera_img_flag = IMG_GATHER;                  //标记图像采集中
        disable_irq(PORTA_IRQn);

#if 1

        PORTA_ISFR = 1 <<  PT28;            //清空PCLK标志位

        DMA_EN(CAMERA_DMA_CH);              //使能通道CHn 硬件请求
        PORTA_ISFR = 1 <<  PT28;            //清空PCLK标志位
        DMA_DADDR(CAMERA_DMA_CH) = (uint32)camera_img_buff;    //恢复地址

#else
        PORTA_ISFR = 1 <<  PT28;            //清空PCLK标志位
        dma_repeat(CAMERA_DMA_CH, (void *)&PTB_B0_IN, (void *)ov7725_eagle_img_buff,CAMERA_DMA_NUM);
#endif
    }
    else                                        //图像采集错误
    {
        disable_irq(PORTA_IRQn);                        //关闭PTA的中断
        camera_img_flag = IMG_FAIL;                     //标记图像采集失败
    }
}

/*!
 *  @brief    DMA中断服务函数,采集完一场触发中断
 */
void camera_dma()
{
    camera_img_flag = IMG_FINISH ;
}

/*!
 *  @brief      采集图像（采集到的数据存储在初始化时配置的地址上）
 */
void camera_get_img()
{
    camera_img_flag = IMG_START;            //开始采集图像
    PORTA_ISFR = ~0;                        //写1清中断标志位(必须的，不然会导致一开中断就马上触发中断)
    enable_irq(PORTA_IRQn);                 //允许PTA的中断
    while(camera_img_flag != IMG_FINISH)    //等待图像采集完毕
    {
        if(camera_img_flag == IMG_FAIL)     //假如图像采集错误，则重新开始采集
        {
            camera_img_flag = IMG_START;    //开始采集图像
            PORTA_ISFR = ~0;                //写1清中断标志位(必须的，不然会导致一开中断就马上触发中断)
            enable_irq(PORTA_IRQn);         //允许PTA的中断
        }
    }
}
/*
*
*/
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
    uint8 colour[2] = {0,1}; //0 和 1 分别对应的颜色
    //注：野火的摄像头 0 表示 白色，1表示 黑色
    uint8 tmpsrc;
    while(srclen --)
    {
        tmpsrc = *src++;
        *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ]; //判断一个字节从高位到低位
        *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}
