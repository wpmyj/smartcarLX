#include "common.h"
#include "handle.h"
#include "include.h"
//#include "function.h"

uint8 img[CAMERA_W*CAMERA_H];
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

uint32 Servo_value=8590;//舵机输出pwm值
uint32 Servomiddle=8590;
uint32 Servo_max=8770; 
uint32 Servo_min=8410;

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

//
/*void Search_Line(void)
{
   uint8 Row_Ptr=0;
   uint8 Col_Ptr=0;
   uint8 LFlag=0;
   uint8 RFlag=0;
   uint8 WFlag=0;
   uint8 SFlag=0;
   uint8 Var=0;
   uint8 Cut_Width=10;
   int8 LEnd=0;
   int8 REnd=79;
   uint8 Left_sign=0;
   uint8 Right_sign=0;

   Left_Cnt=0;
   Right_Cnt=0;
   Left_cnt=0;
   Right_cnt=0;
   Cross_Flag=0;
   Black_Cnt=0;
   Black_cnt=0;
   White_cnt=0;
   White_Cnt=0;
   All_Black=0;
   All_White=0;
   Road_Change=0;
   CrossRow=0;
   StopRow=0;
   Stop_Flag=0;
   a=0;
   
    ///////////////////////
   Left_c_in=0;
   Left_c_out=0;
   Right_c_in=0;
   Right_c_out=0;
   
   Left_r_in=0;
   Left_r_out=0;
   Right_r_in=0;
   Right_r_out=0;
   //////////////////////
      
   //前三行搜线开始
   for(Row_Ptr=59;Row_Ptr>56;Row_Ptr--)
   {
       Road_Left[Row_Ptr]=1;
       Road_Right[Row_Ptr]=79;
       Left_Flag[Row_Ptr]=0;
       Right_Flag[Row_Ptr]=0;
       Road_Center[Row_Ptr]=0;
     ////////////////
       Left_addwidth[Row_Ptr]=0;
       Right_addwidth[Row_Ptr]=0;
       //////////////
    //内层for开始 从中心向左边
       for(Col_Ptr=60;Col_Ptr>0;Col_Ptr--)
       {
              if(img[Row_Ptr][Col_Ptr]==0 && img[Row_Ptr][Col_Ptr-1]==1)
              {
                    Road_Left[Row_Ptr]=Col_Ptr-1;
                    Left_Flag[Row_Ptr]=1;
                    Left_Cnt++;
                    break;
              }                     
       }//结束内层for
   //内层for开始从中心向右
      for(Col_Ptr=20;Col_Ptr<80;Col_Ptr++)
      {
              if(img[Row_Ptr][Col_Ptr]==0 && img[Row_Ptr][Col_Ptr+1]==1)
              {
                    Road_Right[Row_Ptr]=Col_Ptr+1;
                    Right_Flag[Row_Ptr]=1;
                    Right_Cnt++;
                    break;
              }
      }//结束内层for
      if(Road_Left[Row_Ptr]>Road_Right[Row_Ptr])
      {
              Left_Flag[Row_Ptr]=0;
              Right_Flag[Row_Ptr]=0;
              Road_Left[Row_Ptr]=Road_Left[Row_Ptr+1];
              Road_Right[Row_Ptr]=Road_Right[Row_Ptr+1];
      }
       
///////////////////////////加宽超车区识别 ///////////////////////////////    
///////////////////////////加宽超车区识别 ///////////////////////////////   

             if((Row_Ptr!=59)&&(Road_Left[Row_Ptr]-Road_Left[Row_Ptr+1]>10))
               //（Road_Left[Row_Ptr]-Road_Left[Row_Ptr+2]>15))
             {  
                      //Road_Right[Row_Ptr]=Road_Right[Row_Ptr]-40;
                      Left_addwidth[Row_Ptr]=1;
                      
                      Left_c_out=Road_Left[Row_Ptr+1];
                      Left_r_out=Row_Ptr+1;
                      //oled_show_num(88,1,Row_Ptr,0,1);
                      //oled_show_num(88,6,10,0,1);
             }//出左超车区
              if(Road_Left[Row_Ptr+1]-Road_Left[Row_Ptr]>10)
               //（Road_Left[Row_Ptr+2]-Road_Left[Row_Ptr]>15))
             {  
                      //Road_Right[Row_Ptr]=Road_Right[Row_Ptr]-40;
                      Left_addwidth[Row_Ptr]=2;
                      
                      Left_c_in=Road_Left[Row_Ptr];
                      Left_r_in=Row_Ptr;
                      //oled_show_num(88,1,Row_Ptr+1,0,1);
                      //oled_show_num(88,6,5,0,1);
             }//入左超车区
             if(Road_Right[Row_Ptr+1]-Road_Right[Row_Ptr]>10)
               //（Road_Right[Row_Ptr+2]-Road_Right[Row_Ptr]>15))
             {  
                      //Road_Right[Row_Ptr]=Road_Right[Row_Ptr]-40;
                      Right_addwidth[Row_Ptr]=1;
                      
                      Right_c_out=Road_Right[Row_Ptr+1];
                      Right_r_out=Row_Ptr+1;
                      //oled_show_num(88,1,Row_Ptr,0,1);
                      //oled_show_num(88,6,10,0,1);
             }//出右超车区
             else if((Row_Ptr!=59)&&(Road_Right[Row_Ptr]-Road_Right[Row_Ptr+1]>10))
               //Road_Right[Row_Ptr]-Road_Right[Row_Ptr+2]>15))
             {  
                      //Road_Right[Row_Ptr]=Road_Right[Row_Ptr]-40;
                      Right_addwidth[Row_Ptr]=2;
                      
                      Right_c_in=Road_Right[Row_Ptr];
                      Right_r_in=Row_Ptr;
                      //oled_show_num(88,1,Row_Ptr+1,0,1);
                      //oled_show_num(88,6,10,0,1);
             }//入右超车区

///////////////////////////加宽超车区识别 ///////////////////////////////    
///////////////////////////加宽超车区识别 ///////////////////////////////    
   }//结束三行搜线 外层for
   Road_Width[59]=Road_Right[59]-Road_Left[59];
   Road_Width[58]=Road_Right[58]-Road_Left[58];
   Road_Width[57]=Road_Right[57]-Road_Left[57];
   //其余行搜索
   //if(Left_Cnt>0 || Right_Cnt>0)
   //{
   for(Row_Ptr=56;Row_Ptr>2;Row_Ptr--)
   {
          Road_Left[Row_Ptr]=1;
          Road_Right[Row_Ptr]=79;
          Left_Flag[Row_Ptr]=0;
          Right_Flag[Row_Ptr]=0;
          Road_Center[Row_Ptr]=0;
  /////////////////////////////////////
              Left_addwidth[Row_Ptr]=0;
              Right_addwidth[Row_Ptr]=0;
  /////////////////////////////////////
          //确定左边下一行的搜范围
          if(Row_Ptr>30)                     {Col_Ptr=Road_Left[Row_Ptr+1]+6;LEnd=Road_Left[Row_Ptr+1]-6;}
          else if(Row_Ptr>14 && Row_Ptr<=30) {Col_Ptr=Road_Left[Row_Ptr+1]+10;LEnd=Road_Left[Row_Ptr+1]-10;}
          else                               {Col_Ptr=Road_Left[Row_Ptr+1]+20;LEnd=Road_Left[Row_Ptr+1]-20;}  
          if(Cross_Flag==1)
          {
            Col_Ptr=Road_Right[Row_Ptr+1]-20;
            LEnd=0;
          }
          if(Col_Ptr>Road_Right[Row_Ptr+1]) Col_Ptr=Road_Right[Row_Ptr+1];//防止越界  
          if(Col_Ptr>79) Col_Ptr=79;
          if(LEnd<0) LEnd=0;//for开始搜索左边 
          for(;Col_Ptr>LEnd;Col_Ptr--)
          {
                  if(img[Row_Ptr][Col_Ptr]==0 && img[Row_Ptr][Col_Ptr-1]==1)
                  {
                          Road_Left[Row_Ptr]=Col_Ptr-1;
                          Left_Flag[Row_Ptr]=1;
                          Left_Cnt++;
                          if(LFlag==0 && Left_Flag[Row_Ptr+1]==1) 
                          {Left_cnt++;}//统计连续的黑线计数
                          else{LFlag=1;}//重找到线
                          break;
                  }                 
          }//结束左边for循环
          if(Col_Ptr==LEnd && img[Row_Ptr][39] && img[Row_Ptr][40] && img[Row_Ptr][41]) Left_Flag[Row_Ptr]=2;//在搜线范围内没找到
          if(Col_Ptr==LEnd && !img[Row_Ptr][39] && !img[Row_Ptr][40] && !img[Row_Ptr][41]) Left_Flag[Row_Ptr]=3;//在搜线范围内没找到 
         // if(Col_Ptr==LEnd&&img[Row_Ptr][10]==0) Left_sign=1;
          //确定右边下一行的搜线位置
          if(Row_Ptr>30)                     {Col_Ptr=Road_Right[Row_Ptr+1]-6;REnd=Road_Right[Row_Ptr+1]+6;}
          else if(Row_Ptr>14 && Row_Ptr<=30) {Col_Ptr=Road_Right[Row_Ptr+1]-10;REnd=Road_Right[Row_Ptr+1]+10;}
          else                               {Col_Ptr=Road_Right[Row_Ptr+1]-20;REnd=Road_Right[Row_Ptr+1]+20;}
          if(Cross_Flag==1) 
          {
                  Col_Ptr=Road_Left[Row_Ptr+1]+20;
                  REnd=79;
                  a=1;
          }                        
          if(Col_Ptr<Road_Left[Row_Ptr+1]) Col_Ptr=Road_Left[Row_Ptr+1];//防止越界                     
          if(Col_Ptr<0) Col_Ptr=0;
          if(REnd>79) REnd=79;//for开始_搜
          
          for(;Col_Ptr<REnd;Col_Ptr++)
          {
                  if(img[Row_Ptr][Col_Ptr]==0 && img[Row_Ptr][Col_Ptr+1]==1)
                  {
                          Road_Right[Row_Ptr]=Col_Ptr+1; 
                          Right_Flag[Row_Ptr]=1;
                          Right_Cnt++;
                          if(RFlag==0 && Right_Flag[Row_Ptr+1]==1) 
                          {Right_cnt++;}//统计连续的黑线计数
                          else{RFlag=1;}//重找到线
                          break;
                  }
          }//结束for_搜右边  
//                             if(abs(Road_Left[Row_Ptr]-Road_Left[Row_Ptr-1])>10&&abs(Road_Left[Row_Ptr+2]-Road_Left[Row_Ptr-1])>10)
//             {  
//                      Road_Right[Row_Ptr]=Road_Right[Row_Ptr]-20;
//                      oled_show_num(88,6,20,0,1);
//             }
///////////////////////////加宽超车区识别 ///////////////////////////////    
///////////////////////////加宽超车区识别 /////////////////////////////// 
             if((Road_Left[Row_Ptr]-Road_Left[Row_Ptr+1]>10))
             {  
                      //Road_Right[Row_Ptr]=Road_Right[Row_Ptr]-40;
                      Left_addwidth[Row_Ptr]=1;
                      
                      Left_c_out=Road_Left[Row_Ptr+1];
                      Left_r_out=Row_Ptr+1;
                      //oled_show_num(88,1,Row_Ptr,0,1);
                      //oled_show_num(88,6,10,0,1);
             }
             else if((Road_Left[Row_Ptr+1]-Road_Left[Row_Ptr]>10))
             {  
                      //Road_Right[Row_Ptr]=Road_Right[Row_Ptr]-40;
                      Left_addwidth[Row_Ptr]=2;
                      Left_c_in=Road_Left[Row_Ptr];
                      Left_r_in=Row_Ptr;
                      //oled_show_num(88,1,Row_Ptr+1,0,1);
                      //oled_show_num(88,6,5,0,1);
             }
             if((Road_Right[Row_Ptr+1]-Road_Right[Row_Ptr]>10))
             {  
                      //Road_Right[Row_Ptr]=Road_Right[Row_Ptr]-40;
                      Right_addwidth[Row_Ptr]=1;
                      
                      Right_c_out=Road_Right[Row_Ptr+1];
                      Right_r_out=Row_Ptr+1;
                      //oled_show_num(88,1,Row_Ptr,0,1);
                      //oled_show_num(88,6,10,0,1);
             }
             else if((Road_Right[Row_Ptr]-Road_Right[Row_Ptr+1]>10))
             {  
                      //Road_Right[Row_Ptr]=Road_Right[Row_Ptr]-40;
                      Right_addwidth[Row_Ptr]=2;
                      
                      Right_c_in=Road_Right[Row_Ptr];
                      Right_r_in=Row_Ptr;
                      //oled_show_num(88,1,Row_Ptr+1,0,1);
                     // oled_show_num(88,6,5,0,1);
             }
             
///////////////////////////加宽超车区识别 ///////////////////////////////    
///////////////////////////加宽超车区识别 /////////////////////////////// 
          if(Col_Ptr==REnd && img[Row_Ptr][39] && img[Row_Ptr][40] && img[Row_Ptr][41]) Right_Flag[Row_Ptr]=2;//在搜线范围内没找到_全黑行
          if(Col_Ptr==REnd&& !img[Row_Ptr][39] && !img[Row_Ptr][40] && !img[Row_Ptr][41]) Right_Flag[Row_Ptr]=3;//在搜线范围内没找到_全白行  
          if(Col_Ptr==REnd&&img[Row_Ptr][70]==0) Right_sign=1;
          Road_Width[Row_Ptr]=Road_Right[Row_Ptr]-Road_Left[Row_Ptr]; 
//          if(abs(Road_Width[Row_Ptr]-Road_Width[Row_Ptr+1])>=20)
//          {
//                  Var++;
//                 if(Var==1) 
//                 {
//                    Road_Change=Row_Ptr;//赛道宽度第一次发生突变的位置
//                    Change_Flag=1;
//                 }
//                 if(Var==2)
//                 {
//                   Change_Flag=0;
//                   Var=0;
//                 }
//          } 
//          if(Change_Flag==1)
//          {
//            if(Road_Right[Row_Ptr]-Road_Right[Row_Ptr+1]>=10)
//            Road_Left[Row_Ptr]+=Road_Width[Row_Ptr]/2;
//            else
//              Road_Right[Row_Ptr]-=Road_Width[Row_Ptr]/2;
//          }
          
//          if(abs(Road_Right[Row_Ptr]-Road_Right[Row_Ptr+1])>10&&abs(Road_Right[Row_Ptr-1]-Road_Right[Row_Ptr+1])>10)
//          {
//                  Road_Left[Row_Ptr]=Road_Right[Row_Ptr+1];
//                  oled_show_num(88,6,50,0,1);
//          }
//          
//          if(Road_Left[Row_Ptr]>Road_Right[Row_Ptr])
//          {
//                  Left_Flag[Row_Ptr]=0;
//                  Right_Flag[Row_Ptr]=0;
//                  Road_Left[Row_Ptr]=Road_Left[Row_Ptr+1];
//                  Road_Right[Row_Ptr]=Road_Right[Row_Ptr+1];
//          }
          if(Left_Flag[Row_Ptr]==2 && Right_Flag[Row_Ptr]==2)
          {
                  Black_Cnt++;//统计所有的全黑行
                  if(Black_Cnt==1){All_Black=Row_Ptr;}//记录第一次找到全黑行的位置
          }
                 
          if(Left_Flag[Row_Ptr]==3 && Right_Flag[Row_Ptr]==3)
          {
                  White_Cnt++;////统计全白行个数_十字
                  if(White_Cnt==1){All_White=Row_Ptr;}//记录第一次找到全白行的位置
          }
          if(Left_Flag[Row_Ptr]==3 && Right_Flag[Row_Ptr]==3&&Left_Flag[Row_Ptr+1]==3 && Right_Flag[Row_Ptr+1]==3 &&Left_Flag[Row_Ptr+2]==3 && Right_Flag[Row_Ptr+2]==3)
              {Cross_Flag=1;CrossRow=Row_Ptr;StopRow=Row_Ptr;}//如果有连续三行丢线，判断为十字
              if(Road_Left[Row_Ptr]>Road_Right[Row_Ptr])
              {
                      Left_Flag[Row_Ptr]=0;
                      Right_Flag[Row_Ptr]=0;
                      Road_Left[Row_Ptr]=Road_Left[Row_Ptr+1];
                      Road_Right[Row_Ptr]=Road_Right[Row_Ptr+1];
              }

        
   }//结束for 行循环
}
*/
//void Flag_judge(int Row)
//{
//  uint8 Left_BlackCnt=0;
//  uint8 Left_Black=0;
//  uint8 Right_BlackCnt=0;
//  uint8 Right_Black=0;
//  uint8 Col_Ptr=0;
//  for(Col_Ptr=0;Col_Ptr<80;Col_Ptr++)
//  {
//    if(img[Row][Col_Ptr]==0&&img[Row][Col_Ptr+1]==1)
//    {
//          Left_BlackCnt++;
//          if(Left_BlackCnt==1)
//          {
//            Left_Black=Col_Ptr+1;
//          }
//    }
//    if(img[Row][Col_Ptr]==1&&img[Row][Col_Ptr+1]==0)
//    {
//          Right_BlackCnt++;
//          if(Right_BlackCnt==1)
//          {
//            Right_Black=Col_Ptr;
//          }
//    }
//  }
//  if(Left_BlackCnt==1&&Right_BlackCnt==1&&Left_Black<Right_Black)
//  {
//       Overtake_Cnt++;
//      if(Overtake_Cnt==1)
//      {
//        First_Black=Row;
//        //if(First_Black>=35)
//          Overtake=1;
//      }
//  }
//}

void Servo_control(void)
{ 
  uint32 Centerline_ave=0;
  int16 Servo_temp=0;
  uint32 Servo_value;
  uint8 Row_Ptr=0;
  int32 error=0; 
  int32 error1=0;
  int32 error2=0;
  float Kp=4.0;
  float Kd=3.5;
   //////
  uint8 Row=0;
  uint8 J0=0;
  uint8 J1=0;
  uint8 J2=0;
  
  uint8 K0=0;
  uint8 K1=0;
  uint8 K2=0;
   
  float Servo_t=0;
  //////
  for(Row=15;Row<59;Row++) 
{
              if(Left_addwidth[Row]==0)
              { 
                  J0=1;
              }          
              else if(Left_addwidth[Row]==1)
              {                  
                  J1=1;
              }
              else if(Left_addwidth[Row]==2)                    
              {
                  J2=1;
              }
              ////////////////////////////////
              ///////////////////////////////
              if(Right_addwidth[Row]==0)
              {
                  K0=1;
              }

              else if(Right_addwidth[Row]==1)
              {
                  K1=1;
              }
              else if(Right_addwidth[Row]==2)
              {
                  K2=1;
              }                 
                  
 }


// oled_show_num(88,1,J0,0,1);
// oled_show_num(88,2,J1,0,1);
// oled_show_num(88,3,J2,0,1);
// oled_show_num(88,4,K0,0,1);
// oled_show_num(88,5,K1,0,1);
//oled_show_num(88,6,K2,0,1);
 

  if(J2==1&&All_Black==0&&K1==0&&K2==0&&Cross_Flag==0)
  {
              Servo_t=Left_r_in+(110-Left_c_in);
              Servo_value=Servomiddle-Servo_t;

              if(Left_r_in=25)
                  Overtake=1;
             /////
              oled_show_num(88,2,100,0,1);
              oled_show_num(88,1,Left_r_in,0,1);
              oled_show_num(88,5,Left_c_in,0,1);
              oled_show_num(88,6,2,0,1);

  }
  else if(J1==1&&All_Black==0&&K1==0&&K2==0)
  {
              Servo_t=(60-Left_r_out)+Left_c_out-5;
              Servo_value=Servomiddle+Servo_t;
              
              oled_show_num(88,1,Left_r_out,0,1);
              oled_show_num(88,5,Left_c_out,0,1);
              oled_show_num(88,6,1,0,1);    
  }
  else if(J1==0&&J2==0&&K2==1&&All_Black==0)
  {
              Servo_t=Right_r_in+Right_c_in/2-4;
              Servo_value=Servomiddle+Servo_t;
              
              /////
              if(Left_r_in=25)
                  Overtake=1;
               
              /////
              
              oled_show_num(88,1,Right_r_in,0,1);
              oled_show_num(88,5,Right_c_in,0,1);
              oled_show_num(88,6,4,0,1);   
  }
  else if(J1==0&&J2==0&&K1==1&&All_Black==0&&Cross_Flag==0)
  {
              Servo_t=(60-Right_r_out)+Right_c_out-5;
              Servo_value=Servomiddle-Servo_t;
             
              oled_show_num(88,1,Right_r_out,0,1);      
              oled_show_num(88,5,Right_c_out,0,1);
              oled_show_num(88,6,3,0,1);
  }
 
  else 
 {  

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
           Servo_temp=5.0*error;
          else
           Servo_temp=4.0*error;
        }
        else if(All_Black>=8&&All_Black<=14)
        {
          if(error<0)
          {
            Kp=3.0;Kd=2.5;
          }
          else
          {
            Kp=2.5;Kd=2.5;
          }
          Servo_temp=Kp*error+Kd*errorerror;
        }
        else 
        {
          if(error<0)
          {
            Kd=11.0;
          }
          else
          {
            Kd=9.5;
          }
          Servo_temp=Kd*error;
        } 
        Servo_value=Servomiddle+Servo_temp;
 }
  if(Servo_value>Servo_max) 
    Servo_value=Servo_max;
  if(Servo_value<Servo_min)
    Servo_value=Servo_min;
   //oled_show_num(88,1,All_Black,0,1); 
   //oled_show_num(88,2,error,0,1); 
   //oled_show_num(88,3,errorerror,0,1);
   //oled_show_num(88,4,Servo_value,0,1);
   //oled_show_num(88,5,Road_Change,0,1);
   //oled_show_num(88,6,First_Black,0,1);
  //tpm_pwm_duty(TPM2,TPM_CH0,Servo_value);
  //if(All_Black>50)
    //tpm_pwm_duty(TPM0,TPM_CH0,0);
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
	// 计算中值  
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
   
//void Edge_Filter()
//{
//  uint8 Edge[3]={0};
//  uint8 Row_Ptr=0;
//  for(Row_Ptr=CrossRow;Row_Ptr>=StopRow;Row_Ptr--)
//  {
//  	Edge[0]=Road_Left[Row_Ptr+1];
//	Edge[1]=Road_Left[Row_Ptr+2];
//	Edge[2]=Road_Left[Row_Ptr+3];
//	Road_Left[Row_Ptr]=GetMedianNum(Edge,3);
//        
//	Edge[0]=Road_Right[Row_Ptr+1];
//	Edge[1]=Road_Right[Row_Ptr+2];
//	Edge[2]=Road_Right[Row_Ptr+3];
//	Road_Right[Row_Ptr]=GetMedianNum(Edge,3);
//  }   
//}
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
/*void Find_Middle()
{
    uint8 Row_Ptr=0; 
    uint8 CutLen=8;
    uint8 Cut=0;
    uint8 Edge[3]={0};
    uint8 CutPos=0;//中线断开位置
    int a=Road_Center[Row_Ptr];
    volatile uint8 Var=0;
    if(Cross_Flag==1)
    {
      Edge_Filter();
    }
    for(Row_Ptr=59;Row_Ptr>56;Row_Ptr--)
    {     
          Road_Center[Row_Ptr]=(Road_Right[Row_Ptr]+Road_Left[Row_Ptr])/2;     
    }
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

*/
