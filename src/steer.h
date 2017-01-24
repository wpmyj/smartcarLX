#include "gpio.h"
#include "ccd.h"
#include "ftm.h"

void steer_Init();
void steer_PID();
void Control_test();
int whereIsRoad(bool hpld_turn);
bool delete_wan_speed();
void Suddenly_Turn();
int  ramp_steer_pwm();
int ramp_middle();

void steer_test();
int get_CCDMiddle();