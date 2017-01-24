#include "buletooth.h"
#include "ccd.h"

extern uint8_t ADC_Value[128];
extern uint32_t L_speedValue,R_speedValue;


void send_road_Info(){
	printf("0x02");
	printf("0xFD");
	for(int i=0;i<128;i++){
		printf("%d",ADC_Value[i]);
	}
	printf("0xFD");
	printf("0x02");
}

void send_encoder_Value(){
	//显示编码器计数值
	printf("[CH%d]:%4dHz [CH%d]:%4dHz\r\n", 0, L_speedValue, 1, R_speedValue);
}