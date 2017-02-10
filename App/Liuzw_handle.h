#ifndef __LIUZW_HANDLE_H__
#define __LIUZW_HANDLE_H__

//#include "Liuzw_camera.h"
#include "VCAN_OV7725_Eagle.h"
#define  ROW_MAX    60
#define  COLUMN_MAX 80

extern uint8 imgbuff[CAMERA_SIZE];        //一维图像缓冲区                       
extern uint8 img[CAMERA_H][CAMERA_W];     //二维图像缓冲区 

extern int8 Road_Left[ROW_MAX];//左线位置_列标
extern int8 Road_Right[ROW_MAX];//右线位置_列标
extern int8 Road_Center[ROW_MAX];//中心位置_列标
extern int8 Road_Width[ROW_MAX];//每行赛道的宽度

extern uint8 Left_Flag[ROW_MAX];//本行左边线找到标志
extern uint8 Right_Flag[ROW_MAX];//本行右边线找到标志

extern uint8 Left_Cnt; //连续的左边线计数
extern uint8 Right_Cnt;//连续的右边线计数
extern uint8 Right_cnt;//所有的右边线计数
extern uint8 Left_cnt;//所有的左边线计数
extern uint8 Left_Turn;//左拐点
extern uint8 Right_Turn;//右拐点

extern uint8 All_Black;//全黑行位置_行标
extern uint8 All_White;//全白行位置_行标
extern uint8 Black_Cnt;//全黑行计数
extern uint8 White_Cnt;//全白行计数_连续的
extern uint8 White_cnt;//全白行计数_所有的
extern uint8 White_Ren;
extern uint8 CutPos;

extern uint8 Cross_Flag;//十字标志
extern uint8 Cross_Row;//十字所在行
extern uint8 Cross_End_Row;

extern uint8 Road_Change;

extern uint8 Valid_Cnt;//有效行计数
extern uint8 Road_Type;//赛道类型判断

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
