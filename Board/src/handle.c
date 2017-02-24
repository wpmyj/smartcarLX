#include "common.h"
#include "handle.h"
#include "include.h"
//#include "function.h"

uint8 img[CAMERA_W][CAMERA_H];
int8 Road_Left[ROW_MAX]={0};//左线位置_列标
int8 Road_Right[ROW_MAX]={0};//右线位置_列标
int8 Road_Center[ROW_MAX]={0};//中心位置_列标
int8 Road_Width[ROW_MAX]={0};//每行赛道的宽度
int8 Over_Flag[ROW_MAX]={0};//左右超车符

uint8 Road_Change=0;
uint8 Road_type=0;
uint8 Change_Flag=0;

uint8 Left_Flag[ROW_MAX]={0};//本行左边线找到标志
uint8 Right_Flag[ROW_MAX]={0};//本行右边线找到标志
  
uint8 Left_Cnt=0;//左线计数 所有的
uint8 Right_Cnt=0;//右线计数 所有的
uint8 Right_cnt=0;//连续的左边线计数
uint8 Left_cnt=0;//连续的右边线计数

uint8 All_Black=0;//全黑行位置_行标
uint8 All_White=0;//全白行位置_行标
uint8 Black_Cnt=0;//全黑行计数
uint8 Black_cnt=0;//lianxu
uint8 White_cnt=0;//全白行计数_连续的
uint8 White_Cnt=0;//全白行计数_所有的
uint8 White_Ren=0;
uint8 CutPos=0;

volatile uint8 Ren_Turn=0;

uint32 Servo_value=9090;//舵机输出pwm值
uint32 Servomiddle=9090;
uint32 Servo_max=9200; 
uint32 Servo_min=8970;

uint8 Hinder_Start=0;
uint8 Hinder_Flag=0;

uint8 Cross_Flag=0;
uint8 CrossRow=0;





float CenterLineSlope=0;

int32 error=0;
int32 error1=0;
int32 error2=0;
int8 errorerror=0;


uint8 Stop_Flag=0;
uint8 StopRow=0;
uint8 a=0;
uint8 l=0;
uint8 Lastline=0;

////////////////////////////////////////////
///////////////////////////////////////////
uint8 Left_addwidth[ROW_MAX]={0};//本行左边加宽区标志
uint8 Right_addwidth[ROW_MAX]={0};//本行右边加宽区标志

uint8 Left_c_in=0;
uint8 Left_c_out=0;

uint8 Right_c_in=0;
uint8 Right_c_out=0;

uint8 Left_r_in=0;
uint8 Left_r_out=0;

uint8 Right_r_in=0;
uint8 Right_r_out=0;
uint8 Overtake=0;
///////////////////////////////////////////
//////////////////////////////////////////

void Servo_PID();
//uint8 img[60][80];
void Cross_Search(void);
//

void Search_Line(void){
  uint8 Row_Ptr = 0;
  uint8 Col_Ptr = 0;
  uint8 LFlag = 0;
  uint8 RFlag = 0;
  uint8 WFlag = 0;
  uint8 Cut_Width = 0;
  uint8 Var = 0;
  int8 LEnd = 0;
  int8 REnd = 0;
  Left_Cnt = 0;
  Right_Cnt = 0;
  Left_cnt = 0;
  Right_cnt = 0;
  Cross_Flag = 0;
  Black_Cnt = 0;
  White_Cnt = 0;
 // Black_cnt = 0;
  White_cnt =0;
  All_Black = 0;
  All_White = 0;
  Road_Change = 0;
  Road_Center[0]=0;
  Road_Center[1] = 0;
  CrossRow = 0;
  for(Row_Ptr = 59;Row_Ptr >56;Row_Ptr --)////前三行搜线
  {
    Road_Left[Row_Ptr]=1;
    Road_Right[Row_Ptr]=79;
    Left_Flag[Row_Ptr] = 0;
    Right_Flag[Row_Ptr]=0;
    Road_Center[Row_Ptr] = 0;
    
    
    for(Col_Ptr=40;Col_Ptr>0;Col_Ptr--){                     ////从中心向左边找赛道边界
      if(img[Row_Ptr][Col_Ptr] == 0&&img[Row_Ptr][Col_Ptr-1]==1)
      {
        Road_Left[Row_Ptr] = Col_Ptr-1;
        Left_Flag[Row_Ptr] = 1;
        Left_Cnt++;
        break;                                              ///找到边界，本行左搜索结束
      }
    }
    
    for(Col_Ptr=40;Col_Ptr<80;Col_Ptr++){                       //从中心向右边找边界
      if(img[Row_Ptr][Col_Ptr] == 0&&img[Row_Ptr][Col_Ptr+1]==1)
      {
        Road_Right[Row_Ptr] = Col_Ptr+1;
        Right_Flag[Row_Ptr] = 1;
        Right_Cnt++;
        break;                                              ///找到边界，本行右搜索结束
      }
    
    }
  }
  //前三行赛道宽度
  Road_Width[59] = Road_Right[59]-Road_Left[59];
  Road_Width[58] = Road_Right[58]-Road_Left[58];  
  Road_Width[57] = Road_Right[57]-Road_Left[57];
  
  //其他行找边界
  if(Left_Cnt>0||Right_Cnt>0)
  {
    for(Row_Ptr=56;Row_Ptr>2;Row_Ptr--)
    {
      Road_Left[Row_Ptr]=1;
      Road_Right[Row_Ptr]=79;
      Left_Flag[Row_Ptr]=0;
      Right_Flag[Row_Ptr]=0;
      Road_Center[Row_Ptr]=0;
      //确定左边下一行的搜索范围
      if(Row_Ptr>30){ Col_Ptr=Road_Left[Row_Ptr+1]+6; LEnd = Road_Left[Row_Ptr+1]-6;}
      else if(Row_Ptr>14&&Row_Ptr<=30)  {Col_Ptr=Road_Left[Row_Ptr+1]+10;  LEnd=Road_Left[Row_Ptr+1]-10;}
      else  {Col_Ptr=Road_Left[Row_Ptr+1]+14;  LEnd=Road_Left[Row_Ptr+1]-14;}
      
      if(Cross_Flag==1){Col_Ptr=Road_Right[Row_Ptr+1]-20;LEnd=0;}   ///十字路口
      
      if(Col_Ptr>Road_Right[Row_Ptr+1]) Col_Ptr = Road_Right[Row_Ptr+1];//不能超过右边界
     
      ///防止越界
      if(Col_Ptr>79) Col_Ptr = 79;
      if(LEnd<0) LEnd=0;
      
      //左边开始搜索
      for(;Col_Ptr>LEnd;Col_Ptr--)
      {
        if(img[Row_Ptr][Col_Ptr]==0 && img[Row_Ptr][Col_Ptr-1]==1)
        {
          Road_Left[Row_Ptr]=Col_Ptr-1;
          Left_Flag[Row_Ptr]=1;
          Left_cnt++;
          if(LFlag==0 && Left_Flag[Row_Ptr+1]==1) {Left_Cnt++;}else{LFlag=1;}//统计连续的黑线计数
          break;
        }
      }//结束
      if(Col_Ptr==LEnd && img[Row_Ptr][40] && img[Row_Ptr][41] && img[Row_Ptr][39]) Left_Flag[Row_Ptr]=2;//在搜线范围内没找到
      if(Col_Ptr==LEnd && !img[Row_Ptr][40] && !img[Row_Ptr][41] && !img[Row_Ptr][39]) Left_Flag[Row_Ptr]=3;//在搜线范围内没找到
      if(Col_Ptr==LEnd && !img[Row_Ptr][36] && !img[Row_Ptr][38] && !img[Row_Ptr][37]) White_Ren++;//在搜线范围内没找到

     //确定右边下一行的搜线位置
      if(Row_Ptr>30)                     {Col_Ptr=Road_Right[Row_Ptr+1]-6;REnd=Road_Right[Row_Ptr+1]+6;}
      else if(Row_Ptr>14 && Row_Ptr<=30) {Col_Ptr=Road_Right[Row_Ptr+1]-10;REnd=Road_Right[Row_Ptr+1]+10;}
      else                               {Col_Ptr=Road_Right[Row_Ptr+1]-20;REnd=Road_Right[Row_Ptr+1]+20;}
      if(Cross_Flag==1)
      {
              //Col_Ptr=GetMin(Col_Ptr,Road_Left[Row_Ptr+1]+6);
              //REnd=GetMin(REnd,79);
              Col_Ptr=Road_Left[Row_Ptr+1]+20;
              REnd=79;
              
      }
      if(Col_Ptr<Road_Left[Row_Ptr]) Col_Ptr=Road_Left[Row_Ptr];//不能从左边界的左边开始
      if(Col_Ptr>79) Col_Ptr=79;
      if(LEnd<0) LEnd=0;
      
      //搜索右边
      for(;Col_Ptr<REnd;Col_Ptr++)
      {
              if(img[Row_Ptr][Col_Ptr]==0 && img[Row_Ptr][Col_Ptr+1]==1)
              {
                      Road_Right[Row_Ptr]=Col_Ptr+1;
                      Right_Flag[Row_Ptr]=1;
                      Right_cnt++;
                      if(RFlag==0 && Right_Flag[Row_Ptr+1]==1) {Right_Cnt++;}else{RFlag=1;}//统计连续的黑线计数
                      break;
              }
              
      }//结束for_搜右边
      if(Col_Ptr==REnd && img[Row_Ptr][40] && img[Row_Ptr][41] && img[Row_Ptr][39]) Right_Flag[Row_Ptr]=2;//在搜线范围内没找到_全黑行
      if(Col_Ptr==REnd && !img[Row_Ptr][40] && !img[Row_Ptr][41] && !img[Row_Ptr][39]) Right_Flag[Row_Ptr]=3;//在搜线范围内没找到_全白行
      if(Col_Ptr==REnd && !img[Row_Ptr][36] && !img[Row_Ptr][38] && !img[Row_Ptr][37]) White_Ren++;//在搜线范围内没找到_全白行
      Road_Width[Row_Ptr]=Road_Right[Row_Ptr]-Road_Left[Row_Ptr];
      
      if(abs(Road_Width[Row_Ptr]-Road_Width[Row_Ptr+1])>=Cut_Width)
      {
              Var++;
              if(Var==1) Road_Change=Row_Ptr;//赛道宽度第一次发生突变的位置

      }
      
      if(Left_Flag[Row_Ptr]==2 && Right_Flag[Row_Ptr]==2)
      {
              Black_Cnt++;//统计所有的全黑行
              if(Black_Cnt==1){All_Black=Row_Ptr;}//记录第一次找到全黑行的位置
      }
      //统计全白行个数_十字，人字
      if(Left_Flag[Row_Ptr]==3 && Right_Flag[Row_Ptr]==3)
      {
              White_cnt++;//所有的
              if(White_cnt==1){All_White=Row_Ptr;}//记录第一次找到全白行的位置
      }
      if(Left_Flag[All_White]==3 && Right_Flag[All_White]==3)
      {
              if(Left_Flag[Row_Ptr]==3 && Right_Flag[Row_Ptr]==3 && WFlag==0)
              {
                      White_Cnt++;//连续的
                      if(White_Cnt>=3){Cross_Flag=1;CrossRow=Row_Ptr;}//如果有连续三行丢线，判断为十字
                      //break;
              }
              else WFlag=1;
      }
      //左线位置不能大于右线
      if(Road_Left[Row_Ptr]>Road_Right[Row_Ptr])
      {

              Left_Flag[Row_Ptr]=0;
              Right_Flag[Row_Ptr]=0;
              Road_Left[Row_Ptr]=Road_Left[Row_Ptr+1];
              Road_Right[Row_Ptr]=Road_Right[Row_Ptr+1];
      }
      
    }
    
    if(CrossRow<All_Black) Cross_Flag=0;
  }
  else if(Right_Cnt==0 && Left_Cnt==0)
  {//底部没有线的十字
      Cross_Flag=2;
      //Cross_Search();
  }  
}//寻线函数结束

void Servo_control(void)
{
  
  int16 Servo_temp=0;
  uint32 Servo_value;
  uint8 Row_Ptr=0;
  int32 error=0; 
  int32 error1=0;
  int32 error2=0;
  uint8 Kp=40;
  uint8 Kd=30;
   //////

   
  //float Servo_t=0;
  if(All_Black>2)
  {
    Lastline=All_Black;
  }
  else {Lastline=2;}
  l=59-Lastline;
  for(Row_Ptr=59;Row_Ptr>Lastline;Row_Ptr--)
  {
    error+=Road_Center[Row_Ptr]-40;
  }
  error=error/l;
  for(Row_Ptr=59;Row_Ptr>59-l/2;Row_Ptr--)
  {
    error1+=Road_Center[Row_Ptr]-40;
  } 
  for(Row_Ptr=59-l/2;Row_Ptr>Lastline;Row_Ptr--)
  {
    error2+=Road_Center[Row_Ptr]-40;
  }
  error1=error1*2/l;error2=error2/(59-l/2-Lastline);
  errorerror=error2-error1;
  if(All_Black<8)
  {
    if(error<0)
     Servo_temp=5*error;
    else
     Servo_temp=4*error;
  }
  else if(All_Black>=8&&All_Black<=14)
  {
    if(error<0)
    {
      Kp=30;Kd=25;
    }
    else
    {
      Kp=25;Kd=25;
    }
    Servo_temp=(Kp*error+Kd*errorerror)/10;
  }
  else 
  {
    if(error<0)
    {
      Kd=110;
    }
    else
    {
      Kd=95;
    }
    Servo_temp=Kd*error/10;
  } 
  Servo_value=Servomiddle+Servo_temp;
  if(Servo_value>Servo_max) 
    Servo_value=Servo_max;
  if(Servo_value<Servo_min)
    Servo_value=Servo_min;
  ftm_pwm_duty(FTM0,FTM_CH3,Servo_value);
}

uint8 GetMedianNum(uint8 *Array,uint8 FilterLen)  
{
  uint8 i,j;// 循环变量  
  uint8 Temp;
  for (j=0;j<FilterLen-1;j++)// 对数组进行排序   
  {  
          for (i =0;i<FilterLen-j-1;i++)   
          {  
                  if (Array[i]>Array[i + 1])	
                  {   
                          Temp = Array[i];	
                          Array[i] = Array[i+1];	
                          Array[i+1] = Temp;	
                  }  
          }  
  }
  //计算中值
  if ((FilterLen & 1) > 0)  
  {  
          // 数组有奇数个元素，返回中间一个元素  
          Temp = Array[(FilterLen + 1) / 2];  
  }  
  else  
  {  
          // 数组有偶数个元素，返回中间两个元素平均值  
          Temp = (Array[FilterLen/2] + Array[FilterLen/2 + 1])/2;	
  }  
  return Temp;
}
   
void Edge_Filter()
{
  uint8 Row_Ptr=0;
  for(Row_Ptr=CrossRow;Row_Ptr>=StopRow;Row_Ptr--)
  {
    Road_Left[Row_Ptr]=Road_Left[Row_Ptr+1]*2-Road_Left[Row_Ptr+2]; 
    Road_Right[Row_Ptr]=Road_Right[Row_Ptr+1]*2-Road_Right[Row_Ptr+2];
    if(Road_Left[Row_Ptr]<0)
      Road_Left[Row_Ptr]=0;
    if(Road_Right[Row_Ptr]>79)
      Road_Right[Row_Ptr]=79;
  }
}
void Find_Middle()
{
    uint8 Row_Ptr=0; 
    //uint8 CutLen=8;
    //uint8 Cut=0;
    uint8 Edge[3]={0};
    //uint8 CutPos=0;//中线断开位置
    int a=Road_Center[Row_Ptr];
    volatile uint8 Var=0;
    if(Cross_Flag==1)
    {
      Edge_Filter();
    }
    
    //前三行
    for(Row_Ptr=59;Row_Ptr>56;Row_Ptr--)
    {     
          Road_Center[Row_Ptr]=(Road_Right[Row_Ptr]+Road_Left[Row_Ptr])/2;     
    }
    //其他行
    for(Row_Ptr=56;Row_Ptr>2;Row_Ptr--)
    {

	if(Left_Flag[Row_Ptr]==1 && Right_Flag[Row_Ptr]==1)
	{
		Road_Center[Row_Ptr]=(Road_Right[Row_Ptr]+Road_Left[Row_Ptr])/2;
	}
	else if(Left_Flag[Row_Ptr]==1)//左边有线
	{
		Road_Center[Row_Ptr]=Road_Center[Row_Ptr+1]+(Road_Left[Row_Ptr]-Road_Left[Row_Ptr+1]);
	}
	else if(Right_Flag[Row_Ptr]==1)//右边有线左转
	{
		Road_Center[Row_Ptr]=Road_Center[Row_Ptr+1]+(Road_Right[Row_Ptr]-Road_Right[Row_Ptr+1]);
	}
	else
	{
		Road_Center[Row_Ptr]=(Road_Right[Row_Ptr]+Road_Left[Row_Ptr])/2;
	}
	//if(Road_Center[Row_Ptr]<0) Road_Center[Row_Ptr]=0;
	//if(Road_Center[Row_Ptr]>79) Road_Center[Row_Ptr]=79;
    }//结束for
    for(Row_Ptr=59;Row_Ptr>2;Row_Ptr--)
    {
      Edge[0]=Road_Center[Row_Ptr];
      Edge[1]=Road_Center[Row_Ptr-1];
      Edge[2]=Road_Center[Row_Ptr-2];
      Road_Center[Row_Ptr]=GetMedianNum(Edge,3);
      
      img[Row_Ptr][a]=1;
      a=Road_Left[Row_Ptr];
      img[Row_Ptr][a]=1;
      a=Road_Right[Row_Ptr];
      img[Row_Ptr][a]=1;
    }
    //结束for_滤中线
}
