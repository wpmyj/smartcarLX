#ifndef __LIUZW_CAMERA_H__
#define __LIUZW_CAMERA_H__

/*camera_pin define*/
#define CAMERA_VSYNC_PIN     PTA27
#define CAMERA_PCLK_PIN      PTA28
#define CAMERA_HREF_PIN      PTA29

#define CAMERA_DMA_CH        DMA_CH0                  //摄像头的DMA采集通道
#define CAMERA_W             80                       //摄像头图像宽度
#define CAMERA_H             60                       //摄像头图像高度
#define CAMERA_SIZE          (CAMERA_W * CAMERA_H/8 ) //图像占用空间大小
#define CAMERA_DMA_NUM       (CAMERA_SIZE)            //DMA采集次数

//定义图像采集状态
typedef enum
{
    IMG_NOTINIT = 0,
    IMG_FINISH,             //图像采集完毕
    IMG_FAIL,               //图像采集失败
    IMG_GATHER,             //图像采集中
    IMG_START,              //开始采集图像
    IMG_STOP,               //禁止图像采集
} IMG_STATUS_e;

extern uint8 camera_init(uint8 *imgaddr);//摄像头初始化
extern void camera_vsync(void);//场中断服务函数
extern void camera_dma();//dma中断（一场采集结束后触发）
extern void camera_get_img();//采集图片
extern void img_extract(uint8 *dst, uint8 *src, uint32 srclen);//解压图像


#endif