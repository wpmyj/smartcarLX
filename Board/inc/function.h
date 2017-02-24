#ifndef __FUNCTION_H__
#define __FUNCTION_H__


extern void Init_All(void);
extern void Motor_Init(void);
extern void Motor_Out(void);
extern uint8 speed_get;
extern uint16 var;//∫ÏÕ‚≤‚æ‡£ª
extern uint16 speed_goal;
extern void xx_bluetooth();
extern void uart3_handler(void);
#endif