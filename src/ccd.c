#include "ccd.h"
#include "common.h"
#include "ftm.h"
#include "uart.h"

#define PTA_SET *(uint32_t *)0x400ff004u
#define PTA_CLEAN *(uint32_t *)0x400ff008u
#define PTB_SET *(uint32_t *)0x400ff044u
#define PTB_CLEAN *(uint32_t *)0x400ff048u


#define ADC_1 ADC0_SE10_PTA7  //ADC0_SE10_PTA7   ADC_1 ADC1_SE17_PA17 
#define ADC_2 ADC1_SE14_PB10  //ADC_2 ADC0_SE11_PTA8

int steer_pwms=0,last_steer_pwms=0;

int ramp_before_steer_pwm;//上坡前的舵机的打角

extern uint8_t ADC_Value[128]; 
extern uint8_t  Buffer[132];
extern uint8_t buletooth_instance;

uint8_t ADC2_Value[128];
uint8_t ADC3_Value[128];

int ADC_Offset[128];
int up_down_Offset[128];

uint8_t CCD_Threshold=50;

int 	last_CCD1_Left = 0;
int  	last_CCD1_Right = 0;
int   CCD1_Right = 0;
int   CCD1_Left = 0;
int   ADC_Max = 0;
int   ADC_Min = 0;
int   last_ADC_Max = 0;
int   last_ADC_Min = 0;
int   Black_Left = 0;
int   Black_Right = 0;

uint8_t CCD2_Threshold=0;
int   CCD2_left=0;
int   CCD2_Right=0;
int   last_CCD2_Left=0;
int   last_CCD2_Right=0;
int   ADC2_Max;
int   ADC2_Min;

int   CCD_Forward = 0;
int   CCD_Middle=0;
int   last_CCD_Middle=0;

int   Road_Middle = 64;
int   Road_Width=30;

int   White_Width=60;

void CCD1_init(){
		
		 GPIO_QuickInit(HW_GPIOA,5,kGPIO_Mode_OPP);//si
		 //GPIO_WriteBit(HW_GPIOA, 5, 1);
    
		 GPIO_QuickInit(HW_GPIOA,6,kGPIO_Mode_OPP);//clk
	   //GPIO_WriteBit(HW_GPIOA, 6, 1);

		  //ADC_QuickInit(ADC0_SE10_PTA7, kADC_SingleDiff12or13);
	  // ADC_StartConversion(HW_ADC0, 10, kADC_MuxA);
		
	
	  /* 初始化ADC模块 ADC0_SE19_BM0 */
    ADC_InitTypeDef ADC_InitStruct1;
    ADC_InitStruct1.instance = HW_ADC0;
    ADC_InitStruct1.clockDiv = kADC_ClockDiv2; //ADC采样时钟2分频 
    ADC_InitStruct1.resolutionMode = kADC_SingleDiff8or9;
    ADC_InitStruct1.triggerMode = kADC_TriggerSoftware; // 软件触发转换
    ADC_InitStruct1.singleOrDiffMode = kADC_Single; //单端模式 
    ADC_InitStruct1.continueMode = kADC_ContinueConversionEnable; //启动连续转换 转换一次后 自动开始下一次转换
    ADC_InitStruct1.hardwareAveMode = kADC_HardwareAverageDisable; //禁止 硬件平均 功能 
    ADC_InitStruct1.vref = kADC_VoltageVREF;                       // 使用外部VERFH VREFL 作为模拟电压参考 
    ADC_Init(&ADC_InitStruct1);
}

void CCD2_init(){
				
		 GPIO_QuickInit(HW_GPIOA,10,kGPIO_Mode_OPP);//si
		 //GPIO_WriteBit(HW_GPIOA, 5, 1);
    
		 GPIO_QuickInit(HW_GPIOA,9,kGPIO_Mode_OPP);//clk
	   //GPIO_WriteBit(HW_GPIOA, 6, 1);

		// ADC_QuickInit(ADC_2, kADC_SingleDiff8or9);
	  // ADC_StartConversion(HW_ADC0, 10, kADC_MuxA);
}


void CCD_Delay(int time)//延时10个for 循环
{
	int i=0;
	for(i=0;i<time;i++){}
} 


void delay()
{
	unsigned int i;
	for(i=0;i<10;i++)
	{
		__nop();
	}
}


void TSL1401_GetLine(){
	
	int i;
	//初始化CCD，CCD开始工作，用I/O口模拟si和clk作为时钟信号，驱动CCD工作
	GPIO_WriteBit(HW_GPIOA, 5, 0);//si
	GPIO_WriteBit(HW_GPIOA, 6, 0);//clk
	DelayUs(15);
	GPIO_WriteBit(HW_GPIOA, 5, 1);
	DelayUs(15);
	GPIO_WriteBit(HW_GPIOA, 6, 1);
	DelayUs(15);
	GPIO_WriteBit(HW_GPIOA, 5, 0);
	DelayUs(15);
		
	//ADC_Value[0] = ADC_QuickReadValue(ADC_1); /* ADC0_SE8_PB0 ???adc.h??? */
	ADC_StartConversion(HW_ADC0, 10, kADC_MuxA);
	ADC_Value[0] = ADC_ReadValue(HW_ADC0,kADC_MuxA);
	
	GPIO_WriteBit(HW_GPIOA, 6, 0);
	
	for(i=1; i<128; i++)
	{
	  DelayUs(15);
		GPIO_WriteBit(HW_GPIOA, 6, 1);//clk
		DelayUs(15);
		//ADC_Value[i] = ADC_QuickReadValue(ADC_1); /* ADC0_SE8_PB0 ???adc.h??? */
		ADC_Value[i] = ADC_ReadValue(HW_ADC0,kADC_MuxA);
		GPIO_WriteBit(HW_GPIOA, 6, 0);
	}
	
	DelayUs(15);
	GPIO_WriteBit(HW_GPIOA, 6, 1);
	DelayUs(15);
	GPIO_WriteBit(HW_GPIOA, 6, 0);
	DelayUs(5);
	
	/*  for(int i=0;i<128;i++){
					printf(" %d",ADC_Value[i]);
				}
	  printf("\r\n");*/
	
}

void TSL1402_GetLine(){
	
	int i;
	//初始化CCD，CCD开始工作，用I/O口模拟si和clk作为时钟信号，驱动CCD工作
	GPIO_WriteBit(HW_GPIOA, 5, 0);//si
	GPIO_WriteBit(HW_GPIOA, 6, 0);//clk
	DelayUs(15);
	GPIO_WriteBit(HW_GPIOA, 5, 1);
	DelayUs(15);
	GPIO_WriteBit(HW_GPIOA, 6, 1);
	DelayUs(15);
	GPIO_WriteBit(HW_GPIOA, 5, 0);
	DelayUs(15);
		
	ADC2_Value[0] = ADC_QuickReadValue(ADC_2); /* ADC0_SE8_PB0 ???adc.h??? */
	
	GPIO_WriteBit(HW_GPIOA, 6, 0);
	
	
	for(i=1; i<128; i++)
	{
	  DelayUs(15);
		GPIO_WriteBit(HW_GPIOA, 6, 1);//clk
		DelayUs(15);
		ADC2_Value[i] = ADC_QuickReadValue(ADC_2); /* ADC0_SE8_PB0 ???adc.h??? */
		GPIO_WriteBit(HW_GPIOA, 6, 0);
	}
	
	DelayUs(15);
	GPIO_WriteBit(HW_GPIOA, 6, 1);
	DelayUs(15);
	GPIO_WriteBit(HW_GPIOA, 6, 0);
	DelayUs(5);
}

void Adapt1_Black_Threah(){
	
	
	int i=0,sum=0,sumcount = 0,m_count[256] = {0};
	last_CCD1_Left=CCD1_Left;   //记住上一次的值
	last_CCD1_Right=CCD1_Right;
	
	CCD1_Left = 0;
	CCD1_Right = 0;
	
	ADC_Max=0;
	ADC_Min=255;
	
	ADC_Offset[0]=0;
	
	for(int i=0;i<127;i++){
		 ADC_Offset[i+1]=(int)ADC_Value[i+1]-(int)ADC_Value[i];
	}
		 
 for(int i=0;i<128;i++){
				if(ADC_Value[i]>ADC_Max)
					ADC_Max = ADC_Value[i];
				if(ADC_Value[i]<ADC_Min)
					ADC_Min = ADC_Value[i];
		 }
		 
	 if(ADC_Max-ADC_Min<ADC_Min){
				ADC_Max = last_ADC_Max;
			  ADC_Min = last_ADC_Min;
		 }
		 	CCD_Threshold=(ADC_Max+ADC_Min)/2;
	    CCD_Threshold=(ADC_Max+CCD_Threshold)/2;
	    CCD_Threshold=((ADC_Max+ADC_Min)/2+CCD_Threshold)/2;
		 
		 
		 //图像二值化处理
		 for(int i=0;i<128;i++){
				if(ADC_Value[i]<=CCD_Threshold){
						ADC_Value[i] = 0;
				}
				else{
						ADC_Value[i] = 1;
				}
		 }
		 //上位机测试
		 	for(int i=0;i<128;i++){
				if(ADC_Value[i]==0)
					Buffer[i+2] = 0;
				else
					Buffer[i+2] = 255;
      }
		 
		 //去除单点噪声
		 for(int i=1; i<127 ;i++){
				if(ADC_Value[i-1]!=ADC_Value[i]&&ADC_Value[i]!=ADC_Value[i+1]){
						ADC_Value[i]=ADC_Value[i+1];
				}
		 }
 
		 /*for(int i=0;i<128;i++){
				printf("%d",ADC_Value[i]);
		 }
		 printf("\r\n");*/
		 up_down_Offset[0]=0;
		 
			for(i=1;i<128;i++)//计算差分  人字识别  防止串道
			{
				/*if(ADC_Value[i]>ADC_Value[i-1])
				up_down_Offset[i]=1;
				else if(ADC_Value[i]<ADC_Value[i-1])
					up_down_Offset[i]=-1;
				else 
					up_down_Offset[i]=0;*/
				up_down_Offset[i] = ADC_Value[i] - ADC_Value[i-1];
			}
			
		 for(int i=0;i<64;i++){
				if(ADC_Value[i] == 0)
					CCD1_Left++;         //左侧黑边宽度
		 } 
		 
		 for(int i=64;i<128;i++){
		     if(ADC_Value[i] == 0)
					 CCD1_Right++;       //右侧黑边宽度
		 }
		 
	 for(int i=0;i<61;i++){
				if(ADC_Value[i]==0&&ADC_Value[i+1]==0&&ADC_Value[i+2]==1&&ADC_Value[i+3]==1)
					Black_Left = i+1;                                        //左侧黑线对应的数组下标
		 }
		 
	 for(int i=125;i>63;i--){
				if(ADC_Value[i]==1&&ADC_Value[i+1]==1&&ADC_Value[i+2]==0&&ADC_Value[i+3]==0)
					Black_Right = i+2;                                        //右侧黑线对应的数组下标
		 }  //Black_Right-Black_Left<某个值，
		 
	 for(int i=CCD1_Left;i<128-CCD1_Right;i++){
				if(ADC_Value[i]==1){
					White_Width++;
				}
		 }
		 last_CCD_Middle=CCD_Middle;
		 CCD_Middle = CCD1_Right-CCD1_Left; 
}

 int get_CCDMiddle(){
	return CCD_Middle;
 }
 
void Adapt2_Black_Thread(){
	  int sum=0,average=0; 
		ADC2_Max=0;
	  ADC2_Min=255;
	CCD_Forward=0;
		for(int i=0;i<128;i++){  // 实验数据--------------
			if(ADC2_Value[i]>ADC2_Max){
				ADC2_Max=ADC2_Value[i];
			}
			if(ADC2_Value[i]<ADC2_Min){
				ADC2_Min=ADC2_Value[i];
			}
			sum+=ADC2_Value[i];
		}
		average=sum/128;
		
			//if(ADC2_Max-ADC2_Min>ADC2_Min){
				//printf("\n-------------------------------------------------------------------\n");
					//if(average>=CCD_Threshold)//根据CCD1中的门限值，出现绝大多数的点像素都是白点时，就降低门限，使得CCD2中的像素点全判定为白色。
						//CCD2_Threshold=0;
					//else
						//CCD2_Threshold=255;
		//	}
			//else {
		 if(ADC2_Max-ADC_Min<ADC2_Min){
				ADC_Max = last_ADC_Max;
			  ADC_Min = last_ADC_Min;
		 }
					CCD2_Threshold=(ADC2_Max+ADC2_Min)/2;
		      CCD2_Threshold=(ADC2_Max+CCD2_Threshold)/2;
		      CCD2_Threshold=((ADC2_Max+ADC2_Min)/2+CCD2_Threshold)/2;
			//}
			
			//二值化
			for(int i=0;i<128;i++){
				if(ADC2_Value[i]<CCD2_Threshold)
					ADC2_Value[i]=0;
			  else 
					ADC2_Value[i]=1;
			}
			
			//可以使用去噪算法
			for(int i=1; i<127 ;i++){
				if(ADC2_Value[i-1]!=ADC2_Value[i]&&ADC2_Value[i]!=ADC2_Value[i+1]){
						ADC2_Value[i]=ADC2_Value[i+1];
				}
		  }
			
			
			for(int i=0;i<126;i++){
				                       //0多于3个
				if(ADC2_Value[i]+ADC2_Value[i+1]+ADC2_Value[i+2]>1){
					CCD_Forward++;
				}
				                       //2个0
				if(ADC2_Value[i]+ADC2_Value[i+1]+ADC2_Value[i+2]<2)//单点噪声不会对实验结果有影响
					break;
			}

			//printf(" %d\r\n",CCD_Forward);
}


void road_middle_width(){
	int last_road_middle=0,Road_Right=64,Road_Left=0;
	last_road_middle = Road_Middle;
	for(int i=Road_Middle;i<128;i++){
		if(ADC_Value[i]!=1){
			Road_Right=i;
			break;
		}
		else
			Road_Right = i;
	}
	for(int i=Road_Middle;i>0;i--){
		if(ADC_Value[i]!=1){
			Road_Left=i;
			break;
		}
		else
			Road_Left=i;
	}
	Road_Middle = (Road_Right + Road_Left)/2;  //与64进行比对，pid算法
	
	if(ABS(Road_Middle-last_road_middle)>6){
		Road_Middle = last_road_middle*(9/10)+Road_Middle*(1/10);
	}
	if(Road_Right>Road_Left){
		Road_Width = Road_Right - Road_Left;
	}
}
