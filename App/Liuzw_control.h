#ifndef __LIUZW_CONTROL_H__
#define __LIUZW_CONTROL_H__

/*FTMģ����ͨ������Ӧ��������PORT_cfg.h��*/
#define SERVO_FTM         FTM1   //���FTMģ��
#define SERVO_CH          FTM_CH0//���FTMͨ��
#define SERVO_HZ          300    //���PWMƵ��
#define MOTOR_FTM         FTM0   //����FTMģ��
#define MOTOR_CH          FTM_CH0//����FTMͨ��
#define MOTOR_HZ          1000  //����PWMƵ��
#define MOTOR_DIR1_PIN    PTC0   //��������ʹ������
#define MOTOR_DIR2_PIN    PTC2   //��������ʹ������

#define SERVO_MIDDLE  5000//���������Ӧռ�ձ�
#define SERVO_MAX     5650//�����ת���׶�Ӧռ�ձ�
#define SERVO_MIN     4350 //�����ת���׶�Ӧռ�ձ�

extern void init_all(void);
extern void servo_PD_select(void);
extern void servo_PD(void);
extern void motor_PID(void);

#endif
