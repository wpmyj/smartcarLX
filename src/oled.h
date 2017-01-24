#ifndef __oled_H__  
#define __oled_H__   

#include "common.h"
#define OLED_DC_PORT    HW_GPIOE  
#define OLED_SDA_PORT   HW_GPIOE
#define OLED_SCL_PORT 	HW_GPIOE
#define OLED_RST_PORT   HW_GPIOE    
#define OLED_DC_PIN      12
#define OLED_SDA_PIN     10
#define OLED_SCL_PIN     9
#define OLED_RST_PIN     11
#define OLED_DC     PEout(OLED_DC_PIN)  
#define OLED_SDA    PEout(OLED_SDA_PIN)  
#define OLED_SCL    PEout(OLED_SCL_PIN)  
#define OLED_RST    PEout(OLED_RST_PIN)  
  
#define XLevelL     0x00  
#define XLevelH     0x10  
#define XLevel      ((XLevelH&0x0F)*16+XLevelL)  
#define Max_Column  128  
#define Max_Row     64  
#define Brightness  0xCF   
#define X_WIDTH 128  
#define Y_WIDTH 64  
#define u8 uint8_t
//������ʵ�ֵĽӿں���  
void OLED_WRCMD(u8 com);  
void OLED_WRDATA(u8 dat);  
void OLED_Setxy(u8 x, u8 y);    //OLED ��������  
void OLED_Fill(u8 bmp_dat);   //bmp_dat=0x00ȫ����bmp_dat=0xffȫ����  
void OLED_Init(void); //OLED��ʼ��    
void OLED_6x8(u8 x,u8 y,u8 ch[]);//��ʾ6*8һ���׼ASCII�ַ���    ��ʾ�����꣨x,y����yΪҳ��Χ0��7  
void OLED_Cler_6x8(u8 x,u8 y,u8 n);//���N��6*8�ַ�  
void OLED_8x16(u8 x,u8 y,u8 ch[]);//��ʾ8*16һ���׼ASCII�ַ���   ��ʾ�����꣨x,y����yΪҳ��Χ0��7  
void OLED_Cler_8x16(u8 x,u8 y,u8 n); //���n��8*16�ַ�  
void OLED_16x16(u8 x,u8 y,u8 N);//��ʾ16*16����  ��ʾ�����꣨x,y����yΪҳ��Χ0��7  
void OLED_Cler_16x16(u8 x,u8 y,u8 n);//���n��16*16��  
void Draw_BMP(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[]);//��ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*  
void OLED_Refresh_Gram(void);  
void OLED_DRAWPoint(u8 x,u8 y,u8 t);  

void road_Data(uint8_t y,uint8_t *pixel1/*,uint8_t *pixel2*/);
void show_Road();
#endif  
