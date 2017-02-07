/*
 * @file       Liuzw_handle.c
 * @brief      ͼ���������
 * @author     ������
 * @date       2016-01-20
 */
#include "common.h"
#include "Liuzw_handle.h"
#include "Liuzw_camera.h"
#include "MK60_uart.h"
#include "Liuzw_oled.h"

uint8 imgbuff[CAMERA_SIZE];    //һάͼ�񻺳��������ѹ��һ�ֽڱ�ʾ8�����أ�                     
uint8 img[CAMERA_H][CAMERA_W]; //��άͼ�񻺳�������ѹ��һ�ֽڱ�ʾ1�����أ�

int8 Road_Left[ROW_MAX]={0};//����λ��_�б�
int8 Road_Right[ROW_MAX]={0};//����λ��_�б�
int8 Road_Center[ROW_MAX]={0};//����λ��_�б�
int8 Road_Width[ROW_MAX]={0};//ÿ�������Ŀ��

uint8 Left_Flag[ROW_MAX]={0};//����������ҵ���־
uint8 Right_Flag[ROW_MAX]={0};//�����ұ����ҵ���־

uint8 End_Row;//Ѱ�߽�����

uint8 Left_Cnt=0; //����������߼���
uint8 Right_Cnt=0;//�������ұ��߼���
uint8 Right_cnt=0;//���е��ұ��߼���
uint8 Left_cnt=0;//���е�����߼���
uint8 Left_Turn=0;//��յ�
uint8 Right_Turn=0;//�ҹյ�

uint8 All_Black=0;//ȫ����λ��_�б�
uint8 All_White=0;//ȫ����λ��_�б�
uint8 Black_Cnt=0;//ȫ���м���
uint8 White_Cnt=0;//ȫ���м���_������
uint8 White_cnt=0;//ȫ���м���_���е�
uint8 White_Ren=0;
uint8 CutPos=0;//����λ��

uint8 Cross_Flag=0;//ʮ�ֱ�־
uint8 Cross_Row=0;//ʮ�ֿ�ʼ��
uint8 Cross_End_Row=0;

uint8 Road_Change=0;

uint8 Valid_Cnt=0;//��Ч�м���
uint8 Road_Type=0;

bool Stop_Search=false;//ֹͣѰ�߱�־
bool Hold_Flag=false;//�Ƿ񱣳��ϴ�ƫ��

uint16 Total_Err=0;
uint8 Ave_Err=0;

uint8 X1=30,X2=40,X3=50; 
uint8 Y1=0,Y2=0,Y3=0;
	
/*
 *  @brief      Ѱ�ұ���
 *  @author     ������
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
		Road_Left[Row_Ptr]=0;//��ʼ�������Ϊ0
		Road_Right[Row_Ptr]=79;//��ʼ���ұ���Ϊ79
		Left_Flag[Row_Ptr]=0;//��ʼ�����δ�ҵ�����
		Right_Flag[Row_Ptr]=0;//��ʼ���ұ�δ�ҵ�����
		Road_Center[Row_Ptr]=0;//��ʼ������Ϊ0
		Road_Width[Row_Ptr]=0;//��ʼ���������Ϊ0
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
			
			if(Left_Flag[Row_Ptr]==0)//���л�δ�ҵ�����
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
			
			if(Left_Flag[Row_Ptr]==0&&Right_Flag[Row_Ptr]==0)//���߶�û���ҵ���
			{
				if(img[Row_Ptr][Road_Left[Row_Ptr+1]]==1&&img[Row_Ptr][Road_Right[Row_Ptr+1]]==1)//ȫ���б�־
				{
					End_Row=Row_Ptr+1;
					Stop_Search=true;
					White_Cnt=0;
					break;
				}	
				else//ȫ����
				{
					White_Cnt++;
					if(White_Cnt>3)//ʮ��
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
					cross_search1();//б��ʮ��
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
 *  @brief      б��ʮ�ִ���
 *  @author     ������
 */
void cross_search1(void)
{
	int8 Row_Ptr=0;
	int8 Col_Ptr=0;
	bool Left_Cross_Flag=false;//���ʮ�ֽ������Ƿ��ҵ�
	bool Right_Cross_Flag=false;
	int8 Left_Start=0;
	int8 Left_End=0;
	int8 Right_Start=0;
	int8 Right_End=0;	
	int8 Start_Row=0;//ʮ�ֲ��߿�ʼ��
	
	if(!wandao_xiuzheng())//�������
	{
		//��б��ʮ��
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
			for(Col_Ptr=Left_End;Col_Ptr>Left_Start;Col_Ptr--)//��ʮ�ֽ����ж�Ӧ�����
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
			Stop_Search=true;//̫ƫ��������
		}
		
	}
	//��б��ʮ��
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
			for(Col_Ptr=Right_End;Col_Ptr<Right_Start;Col_Ptr++)//��ʮ�ֽ����ж�Ӧ�����
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
			Stop_Search=true;//̫ƫ��������
		}
	}
	}
	
}
/*
 *  @brief      ��������ʮ�ִ���
 *  @author     ������
 */
void cross_search2(void)
{
	int8 Row_Ptr=0;
	int8 Col_Ptr=0;
	bool Left_Cross_Flag=false;//���ʮ�ֽ������Ƿ��ҵ�
	bool Right_Cross_Flag=false;
	int8 Left_Start=0;
	int8 Left_End=0;
	int8 Right_Start=0;
	int8 Right_End=0;	
	int8 Start_Row=0;//ʮ�ֲ��߿�ʼ��
	//�������
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
	for(Col_Ptr=Left_End;Col_Ptr>Left_Start;Col_Ptr--)//��ʮ�ֽ����ж�Ӧ�����
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
	for(Col_Ptr=Right_End;Col_Ptr<Right_Start;Col_Ptr++)//��ʮ�ֽ����ж�Ӧ�����
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
//	for(Row_Ptr=Cross_Row-5;Row_Ptr>0;Row_Ptr--)//����ͱ
//		if(img[Row_Ptr][Road_Left[Cross_Row]]==0&&img[Row_Ptr-1][Road_Left[Cross_Row]]==1)
//		{
//			Cross_End_Row=Row_Ptr-2;
//			break;
//		}
//	for(Col_Ptr=Road_Left[Cross_Row]+30;Col_Ptr>0;Col_Ptr--)//����������
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
//	//���ұ���
//	for(Row_Ptr=Cross_Row-5;Row_Ptr>0;Row_Ptr--)//����ͱ
//		if(img[Row_Ptr][Road_Right[Cross_Row]]==0&&img[Row_Ptr-1][Road_Right[Cross_Row]]==1)
//		{
//			Cross_End_Row=Row_Ptr-2;
//			break;
//		}
//	for(Col_Ptr=Road_Right[Cross_Row]-30;Col_Ptr<79;Col_Ptr++)//����������
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
 *  @brief      ����û���ѵ���ʮ��Ѱ����
 *  @author     ������
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
	if(!wandao_xiuzheng())//�������
	{
		Cross_Flag=1;
	
	for(Row_Ptr=59;Row_Ptr>20;Row_Ptr--)
		if(img[Row_Ptr][0]==0&&img[Row_Ptr-1][0]==1)//�����ҵ�����
		{ 
			for(Col_Ptr=0;Col_Ptr<60;Col_Ptr++)
				if(img[Row_Ptr-2][Col_Ptr]==1&&img[Row_Ptr-2][Col_Ptr+1]==0)//�����ҵ�����
				{
					Cross_Row=Row_Ptr-2;
					End_Row=Cross_Row;
					Road_Left[Cross_Row]=Col_Ptr;
					Left_Flag[Cross_Row]=true;
					for(Row_Ptr=Cross_Row;Row_Ptr<60;Row_Ptr++)//�±߲���
					{
						Road_Left[Row_Ptr]=(int8)(Road_Left[Cross_Row]-0.5*(Row_Ptr-Cross_Row));
						Left_Flag[Row_Ptr]=true;
					}
					break;
				}
			break;
		}
	
	for(Row_Ptr=59;Row_Ptr>20;Row_Ptr--)
		if(img[Row_Ptr][79]==0&&img[Row_Ptr-1][79]==1)//�����ҵ�����
		{ 
			for(Col_Ptr=79;Col_Ptr>20;Col_Ptr--)
				if(img[Row_Ptr-2][Col_Ptr]==1&&img[Row_Ptr-2][Col_Ptr-1]==0)//�����ҵ�����
				{
					Cross_Row=Row_Ptr-2;
					Road_Right[Row_Ptr-2]=Col_Ptr;
					Right_Flag[Row_Ptr-2]=true;
					for(Row_Ptr=Cross_Row;Row_Ptr<60;Row_Ptr++)//�±߲���
					{
						Road_Right[Row_Ptr]=(int8)(Road_Right[Cross_Row]+0.5*(Row_Ptr-Cross_Row));
						Right_Flag[Row_Ptr]=true;
					}
					break;
				}
			break;
		}
	}
	
	else//���������û���ҵ��ߣ�
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
			
			if(Left_Flag[Row_Ptr]==0)//���л�δ�ҵ�����
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
			
			if(Left_Flag[Row_Ptr]==0&&Right_Flag[Row_Ptr]==0)//���߶�û���ҵ���
			{
				if(img[Row_Ptr][Road_Left[Row_Ptr+1]]==1&&img[Row_Ptr][Road_Right[Row_Ptr+1]]==1)//ȫ���б�־
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
 *  @brief      �ײ�û�к��ߵ��������ֹ�����ߴ���
 *  @author     ������
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
 *  @brief      �ײ�û�к��ߵ��������ֹ�����ߴ���
 *  @author     ������
 */
int wandao_xiuzheng2(void)
{
	
}	
/*
 *  @brief      ���������ж�
 *  @author     ������
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
	
	
	//��ʼ�����ж�
	if(Y3>Y2&&Y2>Y1&&Y1>28&&((Y3-Y2)>(Y2-Y1))&&Valid_Cnt<35)
	{
		Road_Type=1;//��ת��
		oled_show_char(81,0,'W',1,1);
	}
	else if(Y1>Y2&&Y2>Y3&&Y3>28&&((Y1-Y2)>(Y2-Y3))&&Valid_Cnt<35)
	{
		Road_Type=2;//��ת��
		oled_show_char(81,0,'W',1,1);
	}
	else
	{
		Road_Type=0;//��ת��
		oled_show_char(81,0,'S',1,1);
	}
}	
/*
 *  @brief      ��������
 *  @author     ������
 */
void cal_middle(void)
{
	uint8 Row_Ptr=0;
	for(Row_Ptr=59;Row_Ptr>56;Row_Ptr--)
		Road_Center[Row_Ptr]=(Road_Left[Row_Ptr]+Road_Right[Row_Ptr])/2;
	for(Row_Ptr=56;Row_Ptr>60-Valid_Cnt;Row_Ptr--)
	{
		if(Left_Flag[Row_Ptr]==1 && Right_Flag[Row_Ptr]==1)//���Ҷ��к���
			Road_Center[Row_Ptr]=(Road_Right[Row_Ptr]+Road_Left[Row_Ptr])/2;
		else if(Left_Flag[Row_Ptr]==1)//����к���
			Road_Center[Row_Ptr]=Road_Center[Row_Ptr+1]+(Road_Left[Row_Ptr]-Road_Left[Row_Ptr+1]);
		else if(Right_Flag[Row_Ptr]==1)//�ұ��к���
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
 *  @brief      ������Ч����
 *  @author     ������
 */
void cal_valid_row(void)
{
	volatile uint8 Var=0;
	
	Var=MAX(End_Row,20);
	
	Valid_Cnt=60-Var;
}
/*
 *  @brief      ����ƫ��
 *  @author     ������
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
 *  @brief      ���͵���λ��
 *  @author     ������
 */
/*void vcan_sendimg(void *imgaddr, uint32_t imgsize)
{
		#define CMD_IMG     1
    uint8_t cmdf[2] = {CMD_IMG, ~CMD_IMG};    //ɽ����λ�� ʹ�õ�����
    //uint8_t cmdr[2] = {~CMD_IMG, CMD_IMG};    //ɽ����λ�� ʹ�õ�����

    uart_putbuff(UART_PORT, cmdf, sizeof(cmdf));    //�ȷ�������
    uart_putbuff(UART_PORT, (uint8_t *)imgaddr, imgsize); //�ٷ���ͼ��		
		for(int i= 0;i<60;i++)				
		    uart_putchar(UART_PORT,Road_Right[i]);
		for(int i= 0;i<60;i++)	
		   	uart_putchar(UART_PORT,Road_Left[i]);
		for(int i= 0;i<60;i++)
		   	uart_putchar(UART_PORT,Road_Center[i]);
    //uart_putbuff(UART_PORT, cmdr, sizeof(cmdr));    //�ȷ�������
}*/
/*
 *  @brief      �������ݵ���λ��
 *  @author     ������
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