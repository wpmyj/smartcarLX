/*
 * @file       Liuzw_handle.c
 * @brief      图像处理与分析
 * @author     刘忠威
 * @date       2016-01-20
 */
#include "common.h"
#include "Liuzw_handle.h"
#include "Liuzw_camera.h"
#include "MK60_uart.h"
#include "Liuzw_oled.h"

uint8 imgbuff[CAMERA_SIZE];    //一维图像缓冲区，需解压（一字节表示8个像素）                     
uint8 img[CAMERA_H][CAMERA_W]; //二维图像缓冲区，解压后（一字节表示1个像素）

int8 Road_Left[ROW_MAX]={0};//左线位置_列标
int8 Road_Right[ROW_MAX]={0};//右线位置_列标
int8 Road_Center[ROW_MAX]={0};//中心位置_列标
int8 Road_Width[ROW_MAX]={0};//每行赛道的宽度

uint8 Left_Flag[ROW_MAX]={0};//本行左边线找到标志
uint8 Right_Flag[ROW_MAX]={0};//本行右边线找到标志

uint8 End_Row;//寻线结束行

uint8 Left_Cnt=0; //连续的左边线计数
uint8 Right_Cnt=0;//连续的右边线计数
uint8 Right_cnt=0;//所有的右边线计数
uint8 Left_cnt=0;//所有的左边线计数
uint8 Left_Turn=0;//左拐点
uint8 Right_Turn=0;//右拐点

uint8 All_Black=0;//全黑行位置_行标
uint8 All_White=0;//全白行位置_行标
uint8 Black_Cnt=0;//全黑行计数
uint8 White_Cnt=0;//全白行计数_连续的
uint8 White_cnt=0;//全白行计数_所有的
uint8 White_Ren=0;
uint8 CutPos=0;//切线位置

uint8 Cross_Flag=0;//十字标志
uint8 Cross_Row=0;//十字开始行
uint8 Cross_End_Row=0;

uint8 Road_Change=0;

uint8 Valid_Cnt=0;//有效行计数
uint8 Road_Type=0;

bool Stop_Search=false;//停止寻线标志
bool Hold_Flag=false;//是否保持上次偏差

uint16 Total_Err=0;
uint8 Ave_Err=0;

uint8 X1=30,X2=40,X3=50; 
uint8 Y1=0,Y2=0,Y3=0;
	
/*
 *  @brief      寻找边沿
 *  @author     刘忠威
 */
void search_line(void)
{
	uint8 Row_Ptr=0;
	uint8 Col_Ptr=0;
	int8 Left_Start=0;
	int8 Left_End=0;
	int8 Right_Start=0;
	int8 Right_End=0;
	
	Cross_Flag=0;
	Stop_Search=false;
	Hold_Flag=false;
	Left_cnt=0;
	Right_cnt=0;
	Left_Cnt=0;
	Right_Cnt=0;
	
	for(Row_Ptr=59;Row_Ptr>0;Row_Ptr--)
	{
		Road_Left[Row_Ptr]=0;//初始化左边沿为0
		Road_Right[Row_Ptr]=79;//初始化右边沿为79
		Left_Flag[Row_Ptr]=0;//初始化左边未找到黑线
		Right_Flag[Row_Ptr]=0;//初始化右边未找到黑线
		Road_Center[Row_Ptr]=0;//初始化中线为0
		Road_Width[Row_Ptr]=0;//初始化赛道宽度为0
	}
	
	for(Row_Ptr=59;Row_Ptr>56;Row_Ptr--)
	{
		for(Col_Ptr=70;Col_Ptr>0;Col_Ptr--)
			if(img[Row_Ptr][Col_Ptr]==0&&img[Row_Ptr][Col_Ptr-1]==1)
			{
				Road_Left[Row_Ptr]=Col_Ptr-1;
				Left_Flag[Row_Ptr]=1;
				Left_Cnt++;
				break;
			}
		for(Col_Ptr=10;Col_Ptr<79;Col_Ptr++)
			if(img[Row_Ptr][Col_Ptr]==0&&img[Row_Ptr][Col_Ptr+1]==1)
			{
				Road_Right[Row_Ptr]=Col_Ptr+1;
				Right_Flag[Row_Ptr]=1;
				Right_Cnt++;
				break;
			}
	}
	
	if(Left_Cnt>2||Right_Cnt>2)
		for(Row_Ptr=56;Row_Ptr>0;Row_Ptr--)
		{
			if(Road_Left[Row_Ptr+1]>Road_Right[Row_Ptr+1])
			{
				End_Row=Row_Ptr+1;
				Stop_Search=true;
			}
			if(Stop_Search==true)
				break;
			
			if(Left_Flag[Row_Ptr]==0)//本行还未找到边线
			{
				Left_Start=Road_Left[Row_Ptr+1]+15;
				Left_End=Road_Left[Row_Ptr+1]-15;
				
				if(Left_Start>79)
					Left_Start=79;
				if(Left_Start>Road_Right[Row_Ptr+1])
					Left_Start=Road_Right[Row_Ptr+1];
				if(Left_End<1)
					Left_End=1;
				
				for(Col_Ptr=Left_Start;Col_Ptr>Left_End;Col_Ptr--)
					if(img[Row_Ptr][Col_Ptr]==0&&img[Row_Ptr][Col_Ptr-1]==1)
					{
						Road_Left[Row_Ptr]=Col_Ptr-1;
						Left_Flag[Row_Ptr]=1;
						Left_cnt++;
						break;
					}
			}
			if(Right_Flag[Row_Ptr]==0)
			{
				Right_Start=Road_Right[Row_Ptr+1]-15;
				Right_End=Road_Right[Row_Ptr+1]+15;;
				
				if(Right_Start<0)
					Right_Start=0;
				if(Right_Start<Road_Left[Row_Ptr+1])
					Right_Start=Road_Left[Row_Ptr+1];
				if(Right_End>78)
					Right_End=78;
				
				for(Col_Ptr=Right_Start;Col_Ptr<Right_End;Col_Ptr++)
					if(img[Row_Ptr][Col_Ptr]==0&&img[Row_Ptr][Col_Ptr+1]==1)
					{
						Road_Right[Row_Ptr]=Col_Ptr+1;
						Right_Flag[Row_Ptr]=1;
						Right_cnt++;
						break;
					}
			}
			
			if(Left_Flag[Row_Ptr]==0&&Right_Flag[Row_Ptr]==0)//两边都没有找到线
			{
				if(img[Row_Ptr][Road_Left[Row_Ptr+1]]==1&&img[Row_Ptr][Road_Right[Row_Ptr+1]]==1)//全黑行标志
				{
					End_Row=Row_Ptr+1;
					Stop_Search=true;
					White_Cnt=0;
					break;
				}	
				else//全白行
				{
					White_Cnt++;
					if(White_Cnt>3)//十字
					{
						Cross_Row=Row_Ptr+5;
						if(Cross_Row>59)
							Cross_Row=59;
						if(Cross_Row>35)
							cross_search2();
						else
						{
							End_Row=Cross_Row;
							Stop_Search=true;
						}						
					}
				}
			}
			else
				White_Cnt=0;
			
			Road_Width[Row_Ptr]=Road_Right[Row_Ptr]-Road_Left[Row_Ptr];
			if(Road_Width[Row_Ptr]-Road_Width[Row_Ptr+1]>2
				 &&Road_Width[Row_Ptr+1]-Road_Width[Row_Ptr+2]>2)
			{
				Cross_Row=Row_Ptr+4;
				if(Cross_Row>59)
					Cross_Row=59;
				if(Cross_Row>30)
					cross_search1();//斜入十字
				else
				{
					End_Row=Cross_Row;
					Stop_Search=true;
				}
			}			
		}	
	else
		cross_search3();
}
/*
 *  @brief      斜入十字处理
 *  @author     刘忠威
 */
void cross_search1(void)
{
	int8 Row_Ptr=0;
	int8 Col_Ptr=0;
	bool Left_Cross_Flag=false;//左边十字结束行是否找到
	bool Right_Cross_Flag=false;
	int8 Left_Start=0;
	int8 Left_End=0;
	int8 Right_Start=0;
	int8 Right_End=0;	
	int8 Start_Row=0;//十字补线开始行
	
	if(!wandao_xiuzheng())//不是弯道
	{
		//左斜入十字
	if(Road_Left[Cross_Row-4]<Road_Left[Cross_Row-3]
		 &&Road_Left[Cross_Row-3]<Road_Left[Cross_Row-2])
	{
		if(Road_Left[Cross_Row-2]<60)
		{
			Cross_Flag=1;
			Left_Start=Road_Left[Cross_Row];
			Left_End=Road_Left[Cross_Row]+30;
			if(Left_End>79)
				Left_End=79;
			for(Row_Ptr=Cross_Row-5;Row_Ptr>0;Row_Ptr--)
			{
				for(Col_Ptr=Road_Left[Cross_Row];Col_Ptr<Left_End;Col_Ptr++)
					if(img[Row_Ptr][Col_Ptr]==1&&img[Row_Ptr][Col_Ptr+1]==1)
					{
						Cross_End_Row=Row_Ptr-1;
						Left_Cross_Flag=true;
						break;
					}
				if(Left_Cross_Flag==true)
					break;
				//printf("Row: %d\r\n",Cross_End_Row);
			}
			for(Col_Ptr=Left_End;Col_Ptr>Left_Start;Col_Ptr--)//找十字结束行对应左边线
				if(img[Cross_End_Row][Col_Ptr]==0&&img[Cross_End_Row][Col_Ptr-1]==1)
				{
					Road_Left[Cross_End_Row]=Col_Ptr-1;
					Left_Flag[Cross_End_Row]=1;
					break;
				}
			//printf("Col: %d\r\n",Road_Left[Cross_End_Row]);
		
			for(Col_Ptr=Road_Left[Cross_End_Row];Col_Ptr<79;Col_Ptr++)
				if(img[Cross_End_Row][Col_Ptr]==0&&img[Cross_End_Row][Col_Ptr+1]==1)
				{
					Road_Right[Cross_End_Row]=Col_Ptr+1;
					Right_Flag[Cross_End_Row]=1;
				}
			Start_Row=Cross_Row+2;
			if(Start_Row>59)
				Start_Row=59;
			for(Row_Ptr=Start_Row;Row_Ptr>Cross_End_Row;Row_Ptr--)
			{
				Road_Left[Row_Ptr]=(int8)(Road_Left[Start_Row]+(float)(Road_Left[Cross_End_Row]-Road_Left[Start_Row])/(Cross_End_Row-Start_Row)*(Row_Ptr-Start_Row));
				Left_Flag[Row_Ptr]=1;
				Left_cnt++;
			
				Road_Right[Row_Ptr]=(int8)(Road_Right[Start_Row]+(float)(Road_Right[Cross_End_Row]-Road_Right[Start_Row])/(Cross_End_Row-Start_Row)*(Row_Ptr-Start_Row));
				Right_Flag[Row_Ptr]=1;
				Right_cnt++;
			}
		}
		else
		{
			End_Row=Cross_Row-2;
			Stop_Search=true;//太偏，不找了
		}
		
	}
	//右斜入十字
	else if(Road_Right[Cross_Row-4]>Road_Right[Cross_Row-3]
		 &&Road_Right[Cross_Row-3]>Road_Right[Cross_Row-2]
			 &&Left_Flag[Cross_Row-10]==0&&Left_Flag[Cross_Row-11]==0)
	{
		if(Road_Right[Cross_Row-2]>20)
		{
			Cross_Flag=1;
			Right_Start=Road_Right[Cross_Row];
			Right_End=Road_Right[Cross_Row]-30;
			if(Right_End<0)
				Right_End=0;
			for(Row_Ptr=Cross_Row-5;Row_Ptr>0;Row_Ptr--)
			{
				for(Col_Ptr=Road_Right[Cross_Row];Col_Ptr>Right_End;Col_Ptr--)
					if(img[Row_Ptr][Col_Ptr]==1&&img[Row_Ptr][Col_Ptr-1]==1)
					{
						Cross_End_Row=Row_Ptr-1;
						Right_Cross_Flag=true;
						break;
					}
				if(Right_Cross_Flag==true)
					break;
				//printf("Row: %d\r\n",Cross_End_Row);
			}
			for(Col_Ptr=Right_End;Col_Ptr<Right_Start;Col_Ptr++)//找十字结束行对应左边线
				if(img[Cross_End_Row][Col_Ptr]==0&&img[Cross_End_Row][Col_Ptr+1]==1)
				{
					Road_Right[Cross_End_Row]=Col_Ptr+1;
					Right_Flag[Cross_End_Row]=1;
					break;
				}
			//printf("Col: %d\r\n",Road_Left[Cross_End_Row]);
		
			for(Col_Ptr=Road_Right[Cross_End_Row];Col_Ptr>0;Col_Ptr--)
				if(img[Cross_End_Row][Col_Ptr]==0&&img[Cross_End_Row][Col_Ptr-1]==1)
				{
					Road_Left[Cross_End_Row]=Col_Ptr-1;
					Left_Flag[Cross_End_Row]=1;
				}
			Start_Row=Cross_Row+2;
			if(Start_Row>59)
				Start_Row=59;
			for(Row_Ptr=Start_Row;Row_Ptr>Cross_End_Row;Row_Ptr--)
			{
				Road_Right[Row_Ptr]=(int8)(Road_Right[Start_Row]+(float)(Road_Right[Cross_End_Row]-Road_Right[Start_Row])/(Cross_End_Row-Start_Row)*(Row_Ptr-Start_Row));
				Right_Flag[Row_Ptr]=1;
				Right_cnt++;
			
				Road_Left[Row_Ptr]=(int8)(Road_Left[Start_Row]+(float)(Road_Left[Cross_End_Row]-Road_Left[Start_Row])/(Cross_End_Row-Start_Row)*(Row_Ptr-Start_Row));
				Left_Flag[Row_Ptr]=1;
				Left_cnt++;
			}
		}		
		else
		{
			End_Row=Cross_Row-2;
			Stop_Search=true;//太偏，不找了
		}
	}
	}
	
}
/*
 *  @brief      正常进入十字处理
 *  @author     刘忠威
 */
void cross_search2(void)
{
	int8 Row_Ptr=0;
	int8 Col_Ptr=0;
	bool Left_Cross_Flag=false;//左边十字结束行是否找到
	bool Right_Cross_Flag=false;
	int8 Left_Start=0;
	int8 Left_End=0;
	int8 Right_Start=0;
	int8 Right_End=0;	
	int8 Start_Row=0;//十字补线开始行
	//补左边线
	Cross_Flag=1;
	Left_Start=Road_Left[Cross_Row];
	Left_End=Road_Left[Cross_Row]+30;
	if(Left_End>79)
		Left_End=79;
	for(Row_Ptr=Cross_Row-5;Row_Ptr>0;Row_Ptr--)
	{
		for(Col_Ptr=Road_Left[Cross_Row];Col_Ptr<Left_End;Col_Ptr++)
			if(img[Row_Ptr][Col_Ptr]==1&&img[Row_Ptr][Col_Ptr+1]==1)
			{
				Cross_End_Row=Row_Ptr-1;
				Left_Cross_Flag=true;
				break;
			}
		if(Left_Cross_Flag==true)
			break;
				//printf("Row: %d\r\n",Cross_End_Row);
	}
	for(Col_Ptr=Left_End;Col_Ptr>Left_Start;Col_Ptr--)//找十字结束行对应左边线
		if(img[Cross_End_Row][Col_Ptr]==0&&img[Cross_End_Row][Col_Ptr-1]==1)
		{
			Road_Left[Cross_End_Row]=Col_Ptr-1;
			Left_Flag[Cross_End_Row]=1;
			break;
		}
	//printf("Col: %d\r\n",Road_Left[Cross_End_Row]);
		
	Start_Row=Cross_Row+2;
	if(Start_Row>59)
		Start_Row=59;
	for(Row_Ptr=Start_Row;Row_Ptr>Cross_End_Row;Row_Ptr--)
	{
		Road_Left[Row_Ptr]=(int8)(Road_Left[Start_Row]+(float)(Road_Left[Cross_End_Row]-Road_Left[Start_Row])/(Cross_End_Row-Start_Row)*(Row_Ptr-Start_Row));
		Left_Flag[Row_Ptr]=1;
		Left_cnt++;
	}

	Right_Start=Road_Right[Cross_Row];
	Right_End=Road_Right[Cross_Row]-30;
	if(Right_End<0)
		Right_End=0;
	for(Row_Ptr=Cross_Row-5;Row_Ptr>0;Row_Ptr--)
	{
		for(Col_Ptr=Road_Right[Cross_Row];Col_Ptr>Right_End;Col_Ptr--)
			if(img[Row_Ptr][Col_Ptr]==1&&img[Row_Ptr][Col_Ptr-1]==1)
			{
				Cross_End_Row=Row_Ptr-1;
				Right_Cross_Flag=true;
				break;
			}
		if(Right_Cross_Flag==true)
			break;
				//printf("Row: %d\r\n",Cross_End_Row);
	}
	for(Col_Ptr=Right_End;Col_Ptr<Right_Start;Col_Ptr++)//找十字结束行对应左边线
		if(img[Cross_End_Row][Col_Ptr]==0&&img[Cross_End_Row][Col_Ptr+1]==1)
		{
			Road_Right[Cross_End_Row]=Col_Ptr+1;
			Right_Flag[Cross_End_Row]=1;
			break;
		}
			//printf("Col: %d\r\n",Road_Left[Cross_End_Row]);
		
	Start_Row=Cross_Row+2;
	if(Start_Row>59)
		Start_Row=59;
	for(Row_Ptr=Start_Row;Row_Ptr>Cross_End_Row;Row_Ptr--)
	{
		Road_Right[Row_Ptr]=(int8)(Road_Right[Start_Row]+(float)(Road_Right[Cross_End_Row]-Road_Right[Start_Row])/(Cross_End_Row-Start_Row)*(Row_Ptr-Start_Row));
		Right_Flag[Row_Ptr]=1;
		Right_cnt++;
	}
//	for(Row_Ptr=Cross_Row-5;Row_Ptr>0;Row_Ptr--)//往上捅
//		if(img[Row_Ptr][Road_Left[Cross_Row]]==0&&img[Row_Ptr-1][Road_Left[Cross_Row]]==1)
//		{
//			Cross_End_Row=Row_Ptr-2;
//			break;
//		}
//	for(Col_Ptr=Road_Left[Cross_Row]+30;Col_Ptr>0;Col_Ptr--)//从右往左找
//		if(img[Cross_End_Row][Col_Ptr]==0&&img[Cross_End_Row][Col_Ptr-1]==1)
//		{
//			Road_Left[Cross_End_Row]=Col_Ptr-1;
//			Left_Flag[Cross_End_Row]=1;
//			break;
//		}
//	
//	Start_Row=Cross_Row+2;
//	if(Start_Row>59)
//		Start_Row=59;
//	
//	for(Row_Ptr=Start_Row;Row_Ptr>Cross_End_Row;Row_Ptr--)
//	{
//		Road_Left[Row_Ptr]=(int8)(Road_Left[Start_Row]+(float)(Road_Left[Cross_End_Row]-Road_Left[Start_Row])/(Cross_End_Row-Start_Row)*(Row_Ptr-Start_Row));
//		Left_Flag[Row_Ptr]=1;
//	}
//		
//	//补右边线
//	for(Row_Ptr=Cross_Row-5;Row_Ptr>0;Row_Ptr--)//往上捅
//		if(img[Row_Ptr][Road_Right[Cross_Row]]==0&&img[Row_Ptr-1][Road_Right[Cross_Row]]==1)
//		{
//			Cross_End_Row=Row_Ptr-2;
//			break;
//		}
//	for(Col_Ptr=Road_Right[Cross_Row]-30;Col_Ptr<79;Col_Ptr++)//从左往右找
//		if(img[Cross_End_Row][Col_Ptr]==0&&img[Cross_End_Row][Col_Ptr+1]==1)
//		{
//			Road_Right[Cross_End_Row]=Col_Ptr+1;
//			Right_Flag[Cross_End_Row]=1;
//			break;
//		}
//	
//	Start_Row=Cross_Row+2;
//	if(Start_Row>59)
//		Start_Row=59;
//	
//	for(Row_Ptr=Start_Row;Row_Ptr>Cross_End_Row;Row_Ptr--)
//	{
//		Road_Right[Row_Ptr]=(int8)(Road_Right[Start_Row]+(float)(Road_Right[Cross_End_Row]-Road_Right[Start_Row])/(Cross_End_Row-Start_Row)*(Row_Ptr-Start_Row));
//		Right_Flag[Row_Ptr]=1;
//	}
}
/*
 *  @brief      下面没有搜到线十字寻处理
 *  @author     刘忠威
 */
void cross_search3(void)
{
	int8 Row_Ptr;
	int8 Col_Ptr;
	int8 Left_Start=0;
	int8 Left_End=0;
	int8 Right_Start=0;
	int8 Right_End=0;
	
	road_type();
	if(!wandao_xiuzheng())//不是弯道
	{
		Cross_Flag=1;
	
	for(Row_Ptr=59;Row_Ptr>20;Row_Ptr--)
		if(img[Row_Ptr][0]==0&&img[Row_Ptr-1][0]==1)//向上找到边沿
		{ 
			for(Col_Ptr=0;Col_Ptr<60;Col_Ptr++)
				if(img[Row_Ptr-2][Col_Ptr]==1&&img[Row_Ptr-2][Col_Ptr+1]==0)//向右找到边沿
				{
					Cross_Row=Row_Ptr-2;
					End_Row=Cross_Row;
					Road_Left[Cross_Row]=Col_Ptr;
					Left_Flag[Cross_Row]=true;
					for(Row_Ptr=Cross_Row;Row_Ptr<60;Row_Ptr++)//下边补线
					{
						Road_Left[Row_Ptr]=(int8)(Road_Left[Cross_Row]-0.5*(Row_Ptr-Cross_Row));
						Left_Flag[Row_Ptr]=true;
					}
					break;
				}
			break;
		}
	
	for(Row_Ptr=59;Row_Ptr>20;Row_Ptr--)
		if(img[Row_Ptr][79]==0&&img[Row_Ptr-1][79]==1)//向上找到边沿
		{ 
			for(Col_Ptr=79;Col_Ptr>20;Col_Ptr--)
				if(img[Row_Ptr-2][Col_Ptr]==1&&img[Row_Ptr-2][Col_Ptr-1]==0)//向右找到边沿
				{
					Cross_Row=Row_Ptr-2;
					Road_Right[Row_Ptr-2]=Col_Ptr;
					Right_Flag[Row_Ptr-2]=true;
					for(Row_Ptr=Cross_Row;Row_Ptr<60;Row_Ptr++)//下边补线
					{
						Road_Right[Row_Ptr]=(int8)(Road_Right[Cross_Row]+0.5*(Row_Ptr-Cross_Row));
						Right_Flag[Row_Ptr]=true;
					}
					break;
				}
			break;
		}
	}
	
	else//弯道（下面没有找到线）
	{
		for(Row_Ptr=56;Row_Ptr>0;Row_Ptr--)
		{
			if(Road_Left[Row_Ptr+1]>Road_Right[Row_Ptr+1])
			{
				End_Row=Row_Ptr+1;
				Stop_Search=true;
			}
			if(Stop_Search==true)
				break;
			
			if(Left_Flag[Row_Ptr]==0)//本行还未找到边线
			{
				Left_Start=Road_Left[Row_Ptr+1]+15;
				Left_End=Road_Left[Row_Ptr+1]-15;
				
				if(Left_Start>79)
					Left_Start=79;
				if(Left_Start>Road_Right[Row_Ptr+1])
					Left_Start=Road_Right[Row_Ptr+1];
				if(Left_End<1)
					Left_End=1;
				
				for(Col_Ptr=Left_Start;Col_Ptr>Left_End;Col_Ptr--)
					if(img[Row_Ptr][Col_Ptr]==0&&img[Row_Ptr][Col_Ptr-1]==1)
					{
						Road_Left[Row_Ptr]=Col_Ptr-1;
						Left_Flag[Row_Ptr]=1;
						Left_cnt++;
						break;
					}
			}
			if(Right_Flag[Row_Ptr]==0)
			{
				Right_Start=Road_Right[Row_Ptr+1]-15;
				Right_End=Road_Right[Row_Ptr+1]+15;;
				
				if(Right_Start<0)
					Right_Start=0;
				if(Right_Start<Road_Left[Row_Ptr+1])
					Right_Start=Road_Left[Row_Ptr+1];
				if(Right_End>78)
					Right_End=78;
				
				for(Col_Ptr=Right_Start;Col_Ptr<Right_End;Col_Ptr++)
					if(img[Row_Ptr][Col_Ptr]==0&&img[Row_Ptr][Col_Ptr+1]==1)
					{
						Road_Right[Row_Ptr]=Col_Ptr+1;
						Right_Flag[Row_Ptr]=1;
						Right_cnt++;
						break;
					}
			}
			
			if(Left_Flag[Row_Ptr]==0&&Right_Flag[Row_Ptr]==0)//两边都没有找到线
			{
				if(img[Row_Ptr][Road_Left[Row_Ptr+1]]==1&&img[Row_Ptr][Road_Right[Row_Ptr+1]]==1)//全黑行标志
				{
					End_Row=Row_Ptr+1;
					Stop_Search=true;
					break;
				}		
			}
		}
	}	
}
/*
 *  @brief      底部没有黑线的弯道（防止按黑线处理）
 *  @author     刘忠威
 */
int wandao_xiuzheng(void)
{
	uint8 Row_Ptr;
	Y1=59;
	Y2=59;
	Y3=59;
	
	for(Row_Ptr=59;Row_Ptr>0;Row_Ptr--)
		if(img[Row_Ptr][X1]==0&&img[Row_Ptr-1][X1]==1)
		{
			Y1=Row_Ptr-1;
			break;
		}
	for(Row_Ptr=59;Row_Ptr>0;Row_Ptr--)
		if(img[Row_Ptr][X2]==0&&img[Row_Ptr-1][X2]==1)
		{
			Y2=Row_Ptr-1;
			break;
		}
	for(Row_Ptr=59;Row_Ptr>0;Row_Ptr--)
		if(img[Row_Ptr][X3]==0&&img[Row_Ptr-1][X3]==1)
		{
			Y3=Row_Ptr-1;
			break;
		}
	
	if((Y1>Y2&&Y2>Y3&&(Y1-Y2)<8&&(Y1-Y2)>(Y2-Y3))
		 ||(Y3>Y2&&Y2>Y1&&(Y3-Y2)<8&&(Y3-Y2)>(Y2-Y1)))
		return 1;
	else
		return 0;
}
/*
 *  @brief      底部没有黑线的弯道（防止按黑线处理）
 *  @author     刘忠威
 */
int wandao_xiuzheng2(void)
{
	
}	
/*
 *  @brief      赛道类型判断
 *  @author     刘忠威
 */
void road_type(void)
{
	uint8 Row_Ptr=0;
	//uint8 Col_Ptr=0;
	
	if(img[59][X1]==1)
		Y1=59;
	else
	{
		for(Row_Ptr=59;Row_Ptr>0;Row_Ptr--)
		{
			if(img[Row_Ptr][X1]==0&&img[Row_Ptr-1][X1]==1)
			{
				Y1=Row_Ptr-1;
				break;
			}
		}
	}
	
	if(img[59][X2]==1)
		Y2=59;	
	else
	{
		for(Row_Ptr=59;Row_Ptr>0;Row_Ptr--)
		if(img[Row_Ptr][X2]==0&&img[Row_Ptr-1][X2]==1)
		{
			Y2=Row_Ptr-1;
			break;
		}
	}
	
	if(img[59][X3]==1)
		Y3=59;
	else
	{
		for(Row_Ptr=59;Row_Ptr>0;Row_Ptr--)
		if(img[Row_Ptr][X3]==0&&img[Row_Ptr-1][X3]==1)
		{
			Y3=Row_Ptr-1;
			break;
		}
	}
	
	
	//开始进行判断
	if(Y3>Y2&&Y2>Y1&&Y1>28&&((Y3-Y2)>(Y2-Y1))&&Valid_Cnt<35)
	{
		Road_Type=1;//右转弯
		oled_show_char(81,0,'W',1,1);
	}
	else if(Y1>Y2&&Y2>Y3&&Y3>28&&((Y1-Y2)>(Y2-Y3))&&Valid_Cnt<35)
	{
		Road_Type=2;//左转弯
		oled_show_char(81,0,'W',1,1);
	}
	else
	{
		Road_Type=0;//左转弯
		oled_show_char(81,0,'S',1,1);
	}
}	
/*
 *  @brief      计算中线
 *  @author     刘忠威
 */
void cal_middle(void)
{
	uint8 Row_Ptr=0;
	for(Row_Ptr=59;Row_Ptr>56;Row_Ptr--)
		Road_Center[Row_Ptr]=(Road_Left[Row_Ptr]+Road_Right[Row_Ptr])/2;
	for(Row_Ptr=56;Row_Ptr>60-Valid_Cnt;Row_Ptr--)
	{
		if(Left_Flag[Row_Ptr]==1 && Right_Flag[Row_Ptr]==1)//左右都有黑线
			Road_Center[Row_Ptr]=(Road_Right[Row_Ptr]+Road_Left[Row_Ptr])/2;
		else if(Left_Flag[Row_Ptr]==1)//左边有黑线
			Road_Center[Row_Ptr]=Road_Center[Row_Ptr+1]+(Road_Left[Row_Ptr]-Road_Left[Row_Ptr+1]);
		else if(Right_Flag[Row_Ptr]==1)//右边有黑线
			Road_Center[Row_Ptr]=Road_Center[Row_Ptr+1]+(Road_Right[Row_Ptr]-Road_Right[Row_Ptr+1]);
		else			
			Road_Center[Row_Ptr]=(Road_Right[Row_Ptr]+Road_Left[Row_Ptr])/2;
		if(Road_Center[Row_Ptr]<=0) 
			Road_Center[Row_Ptr]=0;
		if(Road_Center[Row_Ptr]>79) 
			Road_Center[Row_Ptr]=79;
	}
}
/*
 *  @brief      计算有效行数
 *  @author     刘忠威
 */
void cal_valid_row(void)
{
	volatile uint8 Var=0;
	
	Var=MAX(End_Row,20);
	
	Valid_Cnt=60-Var;
}
/*
 *  @brief      计算偏差
 *  @author     刘忠威
 */
void cal_error(void)
{
	uint8 Row_Ptr=0;
	int Coeff_Sum=0;
	
	for(Row_Ptr=59;Row_Ptr>59-Valid_Cnt;Row_Ptr--)
		Coeff_Sum+=Road_Center[Row_Ptr];
		Ave_Err=(uint8)(Coeff_Sum/Valid_Cnt);
}
/*
 *  @brief      发送到上位机
 *  @author     刘忠威
 */
/*void vcan_sendimg(void *imgaddr, uint32_t imgsize)
{
		#define CMD_IMG     1
    uint8_t cmdf[2] = {CMD_IMG, ~CMD_IMG};    //山外上位机 使用的命令
    //uint8_t cmdr[2] = {~CMD_IMG, CMD_IMG};    //山外上位机 使用的命令

    uart_putbuff(UART_PORT, cmdf, sizeof(cmdf));    //先发送命令
    uart_putbuff(UART_PORT, (uint8_t *)imgaddr, imgsize); //再发送图像		
		for(int i= 0;i<60;i++)				
		    uart_putchar(UART_PORT,Road_Right[i]);
		for(int i= 0;i<60;i++)	
		   	uart_putchar(UART_PORT,Road_Left[i]);
		for(int i= 0;i<60;i++)
		   	uart_putchar(UART_PORT,Road_Center[i]);
    //uart_putbuff(UART_PORT, cmdr, sizeof(cmdr));    //先发送命令
}*/
/*
 *  @brief      发送数据到上位机
 *  @author     刘忠威
 */
void send_data(void)
{
//	printf("Left_Cnt: %d\r\n",Left_Cnt);
//	printf("Right_Cnt: %d\r\n",Right_Cnt);
//	printf("Left_cnt: %d\r\n",Left_cnt);
//	printf("Right_cnt: %d\r\n",Right_cnt);
	printf("End_Row: %d\r\n",End_Row);	
	printf("Valid_Cnt: %d\r\n",Valid_Cnt);
	printf("Y1: %d\r\n",Y1);
	printf("Y2: %d\r\n",Y2);
	printf("Y3: %d\r\n",Y3);
}