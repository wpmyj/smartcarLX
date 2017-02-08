#ifndef __LIUZW_CONTROL_H__
#define __LIUZW_CONTROL_H__

/*FTMģ����ͨ������Ӧ��������PORT_cfg.h��*/

/*#define SERVO_FTM         FTM0   //���FTMģ��
#define SERVO_CH          FTM_CH3//���FTMͨ��
#define SERVO_HZ          100    //���PWMƵ��

#define MOTOR_FTM_A         FTM1   //����A FTMģ��
#define MOTOR_CH_A          FTM_CH0//����A FTMͨ��
#define MOTOR_HZ_A          1000  //����A PWMƵ��
#define MOTOR_DIR1_PIN    PTC3   //����A����ʹ������
#define MOTOR_DIR2_PIN    PTC2   //����A����ʹ������

#define MOTOR_FTM_B         FTM2   //����B FTMģ��
#define MOTOR_CH_B          FTM_CH0//����B FTMͨ��
#define MOTOR_HZ_B          1000  //����B PWMƵ��
#define MOTOR_DIR3_PIN    PTB17  //����B����ʹ������
#define MOTOR_DIR4_PIN    PTB16  //����B����ʹ������
*/
/*
�������
*/
///*
#define SERVO_FTM         FTM0   //���FTMģ��
#define SERVO_CH          FTM_CH0//���FTMͨ��
#define SERVO_HZ          100    //���PWMƵ��

#define MOTOR_FTM_A         FTM2   //����A FTMģ��
#define MOTOR_CH_A          FTM_CH1//����A FTMͨ��
#define MOTOR_HZ_A          1000  //����A PWMƵ��
#define MOTOR_DIR1_PIN    PTB20   //����A����ʹ������
#define MOTOR_DIR2_PIN    PTB21   //����A����ʹ������

#define MOTOR_FTM_B         FTM2   //����B FTMģ��
#define MOTOR_CH_B          FTM_CH0//����B FTMͨ��
#define MOTOR_HZ_B          1000  //����B PWMƵ��
#define MOTOR_DIR3_PIN    PTB17  //����B����ʹ������
#define MOTOR_DIR4_PIN    PTB16  //����B����ʹ������
//*/


#define SERVO_MIDDLE  5000//���������Ӧռ�ձ�
#define SERVO_MAX     5650//�����ת���׶�Ӧռ�ձ�
#define SERVO_MIN     4350 //�����ת���׶�Ӧռ�ձ�

extern void init_all(void);
extern void srervo_PD_select(void);
extern void sevo_PD(void);
extern void motor_PID(void);

#endif
