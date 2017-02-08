#ifndef __LIUZW_CONTROL_H__
#define __LIUZW_CONTROL_H__

/*FTM模块与通道，对应的引脚在PORT_cfg.h里*/

/*#define SERVO_FTM         FTM0   //舵机FTM模块
#define SERVO_CH          FTM_CH3//舵机FTM通道
#define SERVO_HZ          100    //舵机PWM频率

#define MOTOR_FTM_A         FTM1   //驱动A FTM模块
#define MOTOR_CH_A          FTM_CH0//驱动A FTM通道
#define MOTOR_HZ_A          1000  //驱动A PWM频率
#define MOTOR_DIR1_PIN    PTC3   //驱动A正向使能引脚
#define MOTOR_DIR2_PIN    PTC2   //驱动A反向使能引脚

#define MOTOR_FTM_B         FTM2   //驱动B FTM模块
#define MOTOR_CH_B          FTM_CH0//驱动B FTM通道
#define MOTOR_HZ_B          1000  //驱动B PWM频率
#define MOTOR_DIR3_PIN    PTB17  //驱动B正向使能引脚
#define MOTOR_DIR4_PIN    PTB16  //驱动B反向使能引脚
*/
/*
张瑞卿的
*/
///*
#define SERVO_FTM         FTM0   //舵机FTM模块
#define SERVO_CH          FTM_CH0//舵机FTM通道
#define SERVO_HZ          100    //舵机PWM频率

#define MOTOR_FTM_A         FTM2   //驱动A FTM模块
#define MOTOR_CH_A          FTM_CH1//驱动A FTM通道
#define MOTOR_HZ_A          1000  //驱动A PWM频率
#define MOTOR_DIR1_PIN    PTB20   //驱动A正向使能引脚
#define MOTOR_DIR2_PIN    PTB21   //驱动A反向使能引脚

#define MOTOR_FTM_B         FTM2   //驱动B FTM模块
#define MOTOR_CH_B          FTM_CH0//驱动B FTM通道
#define MOTOR_HZ_B          1000  //驱动B PWM频率
#define MOTOR_DIR3_PIN    PTB17  //驱动B正向使能引脚
#define MOTOR_DIR4_PIN    PTB16  //驱动B反向使能引脚
//*/


#define SERVO_MIDDLE  5000//舵机打正对应占空比
#define SERVO_MAX     5650//舵机左转到底对应占空比
#define SERVO_MIN     4350 //舵机右转到底对应占空比

extern void init_all(void);
extern void srervo_PD_select(void);
extern void sevo_PD(void);
extern void motor_PID(void);

#endif
