

#ifndef __TSL1401_H__
#define __TSL1401_H__

#include "stdint.h"
#include "gpio.h"
#include "adc.h"
#include "uart.h"

#define TSL1401_SI(x)	GPIO_WriteBit(HW_GPIOA, 5, x);
#define TSL1401_CLK(x)	GPIO_WriteBit(HW_GPIOA, 6, x);
#define CCD_SI(x)	GPIO_WriteBit(HW_GPIOA, 10, x);
#define CCD_CLK(x)	GPIO_WriteBit(HW_GPIOA,9, x);

void TSL1401_Init(void);
void TSL1401_GetLine();
void CCD_Init(void);
void CCD_GetLine(uint8_t *);
int get_CCDMiddle();

#endif

void Adapt1_Black_Threah();
void Adapt2_Black_Thread();
void CCD1_init();
void CCD2_init();
void CCD3_init();
void CCD1_Value(int us);
void CCD2_Value(int us);
void CCD3_Value(int us);
void CCD_Value(int us);
void Adapted_Black_Thread();
void Adapted2_Black_Thread();
void road_middle_width();
void modify_CCD_Middle();
void CCD_Delay(int time);	 
void guassfilter();
void HeapAdjust(int array[],int i,int nLength);
void HeapSort(int array[],int length);
