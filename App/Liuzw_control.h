#ifndef __LIUZW_CONTROL_H__
#define __LIUZW_CONTROL_H__

/*FTM模块与通道，对应的引脚在PORT_cfg.h里*/
#define SERVO_FTM         FTM1   //舵机FTM模块
#define SERVO_CH          FTM_CH0//舵机FTM通道
#define SERVO_HZ          300    //舵机PWM频率
#define MOTOR_FTM         FTM0   //驱动FTM模块
#define MOTOR_CH          FTM_CH0//驱动FTM通道
#define MOTOR_HZ          1000  //驱动PWM频率
#define MOTOR_DIR1_PIN    PTC0   //驱动正向使能引脚
#define MOTOR_DIR2_PIN    PTC2   //驱动反向使能引脚

#define SERVO_MIDDLE  5000//舵机打正对应占空比
#define SERVO_MAX     5650//舵机左转到底对应占空比
#define SERVO_MIN     4350 //舵机右转到底对应占空比

extern void init_all(void);
extern void servo_PD_select(void);
extern void servo_PD(void);
extern void motor_PID(void);

#endif
