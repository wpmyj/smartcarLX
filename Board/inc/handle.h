#ifndef __HANDLE_H__
#define __HANDLE_H__

#include "include.h"

#define  ROW_MAX    60
#define  COLUMN_MAX 80
#define line_width 2
//extern int CAMERA_H=60,CAMERA_W=80;
extern uint8 img[CAMERA_W*CAMERA_H];     //80*60  
extern int8 Road_Left[ROW_MAX];
extern int8 Road_Right[ROW_MAX];
extern int8 Road_Center[ROW_MAX];
extern int8 Road_Width[ROW_MAX];
extern uint8 Left_Flag[ROW_MAX];
extern uint8 Right_Flag[ROW_MAX];

extern uint8 Cross_Flag;
extern float Midd_Slope;
//              
extern uint8 Left_Cnt;
extern uint8 Right_Cnt;
extern uint8 Right_cnt;
extern uint8 Left_cnt;
extern uint8 Black_Cnt;
extern uint8 White_Cnt;
extern uint8 White_cnt;
extern uint8 All_Black;
extern uint8 All_White;
extern uint8 CutPos;
extern uint8 Road_Change;
extern uint8 Stop_Flag;
extern uint8 Change_Flag;
extern uint8 Road_type;
extern int8 errorerror;
extern uint8 l;
extern  uint8 Lastline;

extern uint8 CrossRow;
extern uint8 Overtake;
//
extern uint8 GetMedianNum(uint8 *bArray,uint8 iFilterLen);
extern void Search_Line(void);
extern void Start_Line(void);
extern void Find_Middle(void);
extern void Edge_Filter(void);
extern void Servo_control(void);
#endif