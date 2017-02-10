#ifndef __LIUZW_HANDLE_H__
#define __LIUZW_HANDLE_H__

//#include "Liuzw_camera.h"
#include "VCAN_OV7725_Eagle.h"
#define  ROW_MAX    60
#define  COLUMN_MAX 80

extern uint8 imgbuff[CAMERA_SIZE];        //һάͼ�񻺳���                       
extern uint8 img[CAMERA_H][CAMERA_W];     //��άͼ�񻺳��� 

extern int8 Road_Left[ROW_MAX];//����λ��_�б�
extern int8 Road_Right[ROW_MAX];//����λ��_�б�
extern int8 Road_Center[ROW_MAX];//����λ��_�б�
extern int8 Road_Width[ROW_MAX];//ÿ�������Ŀ��

extern uint8 Left_Flag[ROW_MAX];//����������ҵ���־
extern uint8 Right_Flag[ROW_MAX];//�����ұ����ҵ���־

extern uint8 Left_Cnt; //����������߼���
extern uint8 Right_Cnt;//�������ұ��߼���
extern uint8 Right_cnt;//���е��ұ��߼���
extern uint8 Left_cnt;//���е�����߼���
extern uint8 Left_Turn;//��յ�
extern uint8 Right_Turn;//�ҹյ�

extern uint8 All_Black;//ȫ����λ��_�б�
extern uint8 All_White;//ȫ����λ��_�б�
extern uint8 Black_Cnt;//ȫ���м���
extern uint8 White_Cnt;//ȫ���м���_������
extern uint8 White_cnt;//ȫ���м���_���е�
extern uint8 White_Ren;
extern uint8 CutPos;

extern uint8 Cross_Flag;//ʮ�ֱ�־
extern uint8 Cross_Row;//ʮ��������
extern uint8 Cross_End_Row;

extern uint8 Road_Change;

extern uint8 Valid_Cnt;//��Ч�м���
extern uint8 Road_Type;//���������ж�

extern uint8 Ave_Err;

extern int8 get_min(int8 a,int8 b);
extern uint8 get_mediannum(uint8 *bArray,uint8 iFilterLen);
extern int8 get_max(int8 a,int8 b);
extern float slope_calculate(uint8 begin,uint8 end,int8 *array);

extern void search_line(void);
extern void cross_search1(void);
extern void cross_search2(void);
extern void cross_search3(void);
extern void road_type(void);
extern int wandao_xiuzheng(void);
extern void cal_middle(void);
extern void cal_valid_row(void);
extern void cal_error(void);
extern void vcan_sendimg(void *imgaddr, uint32_t imgsize);
extern void send_data(void);

#endif
