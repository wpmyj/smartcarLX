/*
 * @file       Liuzw_oled.c
 * @brief      oled驱动函数
 * @author     刘忠威
 * @date       2016-01-20
 */
#include "common.h"
#include "MK60_gpio.h"
#include "Liuzw_oled.h"
#include "Liuzw_oledfont.h" 
#include "Liuzw_handle.h"

//显示格式如下
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127
static void oled_set_pos(unsigned char x, unsigned char y);
static void oled_wr_byte(uint8 dat,uint8 cmd,uint8 mode);
//向oled写入一个字节
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
//mode:0,反白显示;1,正常显示，只对写数据有效，写命令必须为1	
static void oled_wr_byte(uint8 dat,uint8 cmd,uint8 mode)
{	
  uint8 i;			  
  if(cmd)
    OLED_DC_Set();
  else 
    OLED_DC_Clr();		  
  //OLED_CS_Clr();
  if(mode==0)
    dat=~dat;
  for(i=0;i<8;i++)
  {
    OLED_SCLK_Clr();
    if(dat&0x80)
      OLED_SDIN_Set();
    else 
      OLED_SDIN_Clr();
    OLED_SCLK_Set();
    dat<<=1;   
  }				 		  
  //OLED_CS_Set();
  OLED_DC_Set();   	  
} 
//选择操作坐标
static void oled_set_pos(unsigned char x, unsigned char y) 
{ 
  oled_wr_byte(0xb0+y,OLED_CMD,1);
  oled_wr_byte(((x&0xf0)>>4)|0x10,OLED_CMD,1);
  oled_wr_byte((x&0x0f)|0x01,OLED_CMD,1); 
}   	  
//开启Oled显示 
void oled_display_on(void)
{
  oled_wr_byte(0X8D,OLED_CMD,1);  
  oled_wr_byte(0X14,OLED_CMD,1);  
  oled_wr_byte(0XAF,OLED_CMD,1);  
}
//关闭oled显示 
void oled_display_off(void)
{
  oled_wr_byte(0X8D,OLED_CMD,1);  
  oled_wr_byte(0X10,OLED_CMD,1);  
  oled_wr_byte(0XAE,OLED_CMD,1); 
}		   			 
//清屏函数，清完屏，整个屏幕是黑色的，和没点亮一样	  
void oled_clear(void)  
{  
  uint8 i,n;		    
  for(i=0;i<8;i++)  
  {  
    oled_wr_byte (0xb0+i,OLED_CMD,1);  
    oled_wr_byte (0x00,OLED_CMD,1);      
    oled_wr_byte (0x10,OLED_CMD,1);      
    for(n=0;n<128;n++)
      oled_wr_byte(0,OLED_DATA,1); 
   } 
}
//在指定位置显示一个字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 1代表16*8,0代表8*6
void oled_show_char(uint8 x,uint8 y,uint8 chr,uint8 size,uint8 mode)
{
  unsigned char c=0,i=0;	
  c=chr-' ';		
  if(size ==1)
  {
    if(x>Max_Column-1){x=0;y=y+2;}
    oled_set_pos(x,y);
    if(mode==1)
    {
      for(i=0;i<8;i++)
        oled_wr_byte(F8X16[c*16+i],OLED_DATA,1);
      oled_set_pos(x,y+1);
      for(i=0;i<8;i++)
        oled_wr_byte(F8X16[c*16+i+8],OLED_DATA,1);
    }
    else
    {
      for(i=0;i<8;i++)
        oled_wr_byte(F8X16[c*16+i],OLED_DATA,0);
      oled_set_pos(x,y+1);
      for(i=0;i<8;i++)
        oled_wr_byte(F8X16[c*16+i+8],OLED_DATA,0);
    }
  }
  else if(size==0)
  {
    if(x>Max_Column-1){x=0;y=y+1;}
    oled_set_pos(x,y);
    if(mode==1)
    {
      for(i=0;i<6;i++)
        oled_wr_byte(F6x8[c][i],OLED_DATA,1);	
    }
    else
      for(i=0;i<6;i++)
        oled_wr_byte(F6x8[c][i],OLED_DATA,0);	
  }
}
//m^n函数
uint32 oled_pow(uint8 m,uint8 n)
{
  uint32 result=1;	 
  while(n--)
    result*=m;    
  return result;
}
/*
 *
 */
void oled_show_num(uint8 x,uint8 y,int32 num,uint8 size,uint8 mode)
{ 
	//oled_clear();
  	uint8 t,temp;
  	uint8 enshow=0;
  	uint8 len=0;
  	if(num<0)
  	{
    	num=ABS(num);
    	if(mode==1)
      	if(size==1)       
        	oled_show_char(x,y,'-',1,1);
      	else
        	oled_show_char(x,y,'-',0,1);
    	else
      	if(size==1)       
        	oled_show_char(x,y,'-',1,0);
      	else
        	oled_show_char(x,y,'-',0,0);
    	if(size==1)
      	x+=8;
    	else
      	x+=6;
  	}
  	if(num<10)
    	len=1;
  	else if(num<100)
    	len=2;
  	else if(num<1000)
    	len=3;
  	else if(num<10000)
    	len=4;
 	 else if(num<100000)
    	len=5;
  	else if(num<1000000)
    	len=6;
  	else if(num<10000000)
    	len=7;   
  	for(t=0;t<len;t++)
  	{
    	temp=(num/oled_pow(10,len-t-1))%10;
    	if(enshow==0&&t<(len-1))
    	{
      	if(temp==0)
      	{
        	if(mode==1)
          	oled_show_char(x+(size/2)*t,y,' ',size,1);
        	else
          	oled_show_char(x+(size/2)*t,y,' ',size,0);
        	continue;
      	}
      	else 
        	enshow=1; 		 	 
    	}
    	if(mode==1)
      	if(size==1)       
        	oled_show_char(x+8*t,y,temp+'0',1,1);
      	else
        	oled_show_char(x+6*t,y,temp+'0',0,1);
    	else
      	if(size==1)       
        	oled_show_char(x+8*t,y,temp+'0',1,0);
      	else
        	oled_show_char(x+6*t,y,temp+'0',0,0);
  	}
} 
//显示一个字符串
void oled_show_string(uint8 x,uint8 y,uint8 *chr,uint8 size,uint8 mode)
{
	//oled_clear();
  unsigned char j=0;
  while (chr[j]!='\0')
  {
    if(mode==1)
      oled_show_char(x,y,chr[j],size,1);
    else
      oled_show_char(x,y,chr[j],size,0);
    if(size==1)
    {
      x+=8;
      if(x>120)//换行
      {
        x=0;
        y+=2;
      }
      j++;
    }
    else if(size==0)
    {
       x+=6;
      if(x>122)//换行
      {
        x=0;
        y+=1;
      }
      j++;
    }
  }
}
void oled_show_logo(void)
{
	unsigned char i,j=0;
	for(i=0;i<8;i++)
	{
		oled_set_pos(0,i); 
		for(j=0;j<128;j++)			
			oled_wr_byte(Logo[i][j],OLED_DATA,1);
	}
}
//显示采集图像
void oled_show_picture(void)
{
	//oled_clear();
    uint8 ucData,*pucTable;
    uint16 i,j,k,m,n;
    uint16 usRowTmp,usRowNum,usColumnNum;
	  
    usRowNum=CAMERA_H;
    usColumnNum=CAMERA_W;
	 
    pucTable=img[0];
    m=usRowNum>>3;   //计算图片行数以8位为一组完整的组数
    n=usRowNum%8;    //计算分完组后剩下的行数
    
    for(i=0;i<m;i++) //完整组行扫描
    {
        oled_set_pos(0,i);
        usRowTmp=i<<3;    //计算当前所在行的下标 0,8,16,24 
        
        for(j=0;j<80;j++) //列扫描                           
        {
            ucData=0; 
            for(k=0; k<8;k++) //在i组中对这8行扫描
            {
                ucData=ucData>>1;
                if((pucTable+(usRowTmp+k)*usColumnNum)[j]==1)
                {
                    ucData=ucData|0x80;
                }
                
            }
            oled_wr_byte(ucData,OLED_DATA,1);
        }
    }
    
    oled_set_pos(0,(uint8)i); //设置剩下的行显示的起始坐标
    usRowTmp=i<<3;       //计算当前所在行的下标                  
    for(j=0;j<80;j++) //列扫描        
    {
        ucData=0;
        for(k=0;k<n;k++) //对剩下的行扫描
        {
            ucData=ucData>>1;
            if((pucTable+(usRowTmp + k)*usColumnNum)[j]==1) //pucTable + (usRowTmp + k) * usColumnNum Adderss point to a line
            {
                ucData=ucData|0x80;
            }
            
        }
        ucData=ucData>>(8-n);
        oled_wr_byte(ucData,OLED_DATA,1);
    }
}
//初始化
void oled_init(void)
{
  gpio_init(OLED_SCLK_Pin , GPO, 1); //初始化其为gpio模式
  gpio_init(OLED_SDIN_Pin, GPO, 1);  //初始化其为gpio模式
  gpio_init(OLED_RST_Pin, GPO, 1);   //初始化其为gpio模式
  gpio_init(OLED_DC_Pin, GPO, 1);    //初始化其为gpio模式
  //gpio_init(OLED_CS_Pin, GPO, 1);    //初始化其为gpio模式
  
  OLED_RST_Set();
  DELAY_MS(100);
  OLED_RST_Clr();
  DELAY_MS(200);
  OLED_RST_Set(); 
					  
  oled_wr_byte(0xAE,OLED_CMD,1);//--turn off oled panel
  oled_wr_byte(0x00,OLED_CMD,1);//---set low column address
  oled_wr_byte(0x10,OLED_CMD,1);//---set high column address
  oled_wr_byte(0x40,OLED_CMD,1);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  oled_wr_byte(0x81,OLED_CMD,1);//--set contrast control register
  oled_wr_byte(0xCF,OLED_CMD,1); // Set SEG Output Current Brightness
  oled_wr_byte(0xA1,OLED_CMD,1);//--Set SEG/Column Mapping     0xa0×óóò·′?? 0xa1?y3￡
  oled_wr_byte(0xC8,OLED_CMD,1);//Set COM/Row Scan Direction   0xc0é???·′?? 0xc8?y3￡
  oled_wr_byte(0xA6,OLED_CMD,1);//--set normal display
  oled_wr_byte(0xA8,OLED_CMD,1);//--set multiplex ratio(1 to 64)
  oled_wr_byte(0x3f,OLED_CMD,1);//--1/64 duty
  oled_wr_byte(0xD3,OLED_CMD,1);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
  oled_wr_byte(0x00,OLED_CMD,1);//-not offset
  oled_wr_byte(0xd5,OLED_CMD,1);//--set display clock divide ratio/oscillator frequency
  oled_wr_byte(0x80,OLED_CMD,1);//--set divide ratio, Set Clock as 100 Frames/Sec
  oled_wr_byte(0xD9,OLED_CMD,1);//--set pre-charge period
  oled_wr_byte(0xF1,OLED_CMD,1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  oled_wr_byte(0xDA,OLED_CMD,1);//--set com pins hardware configuration
  oled_wr_byte(0x12,OLED_CMD,1);
  oled_wr_byte(0xDB,OLED_CMD,1);//--set vcomh
  oled_wr_byte(0x40,OLED_CMD,1);//Set VCOM Deselect Level
  oled_wr_byte(0x20,OLED_CMD,1);//-Set Page Addressing Mode (0x00/0x01/0x02)
  oled_wr_byte(0x02,OLED_CMD,1);//
  oled_wr_byte(0x8D,OLED_CMD,1);//--set Charge Pump enable/disable
  oled_wr_byte(0x14,OLED_CMD,1);//--set(0x10) disable
  oled_wr_byte(0xA4,OLED_CMD,1);// Disable Entire Display On (0xa4/0xa5)
  oled_wr_byte(0xA6,OLED_CMD,1);// Disable Inverse Display On (0xa6/a7) 
  oled_wr_byte(0xAF,OLED_CMD,1);//--turn on oled panel
	
  oled_wr_byte(0xAF,OLED_CMD,1); /*display ON*/ 
  oled_clear();
  oled_set_pos(0,0); 	
}
