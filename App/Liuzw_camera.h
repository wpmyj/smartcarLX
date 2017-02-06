#ifndef __LIUZW_CAMERA_H__
#define __LIUZW_CAMERA_H__

/*camera_pin define*/
#define CAMERA_VSYNC_PIN     PTA27
#define CAMERA_PCLK_PIN      PTA28
#define CAMERA_HREF_PIN      PTA29

#define CAMERA_DMA_CH        DMA_CH0                  //����ͷ��DMA�ɼ�ͨ��
#define CAMERA_W             80                       //����ͷͼ����
#define CAMERA_H             60                       //����ͷͼ��߶�
#define CAMERA_SIZE          (CAMERA_W * CAMERA_H/8 ) //ͼ��ռ�ÿռ��С
#define CAMERA_DMA_NUM       (CAMERA_SIZE)            //DMA�ɼ�����

//����ͼ��ɼ�״̬
typedef enum
{
    IMG_NOTINIT = 0,
    IMG_FINISH,             //ͼ��ɼ����
    IMG_FAIL,               //ͼ��ɼ�ʧ��
    IMG_GATHER,             //ͼ��ɼ���
    IMG_START,              //��ʼ�ɼ�ͼ��
    IMG_STOP,               //��ֹͼ��ɼ�
} IMG_STATUS_e;

extern uint8 camera_init(uint8 *imgaddr);//����ͷ��ʼ��
extern void camera_vsync(void);//���жϷ�����
extern void camera_dma();//dma�жϣ�һ���ɼ������󴥷���
extern void camera_get_img();//�ɼ�ͼƬ
extern void img_extract(uint8 *dst, uint8 *src, uint32 srclen);//��ѹͼ��


#endif