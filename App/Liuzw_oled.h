#ifndef __LIUZW_OLED_H__
#define __LIUZW_OLED_H__

#define OLED_SCLK_Pin    PTC13     //D0
#define OLED_SDIN_Pin    PTC11      //D1
#define OLED_RST_Pin     PTC10     //RST
#define OLED_DC_Pin      PTC9      //DC
//#define OLED_CS_Pin      PTA6      //CS

#define OLED_SCLK_Clr()   PTXn_T(OLED_SCLK_Pin,OUT)=0//CLK
#define OLED_SCLK_Set()   PTXn_T(OLED_SCLK_Pin,OUT)=1

#define OLED_SDIN_Clr()   PTXn_T(OLED_SDIN_Pin,OUT)=0//DIN
#define OLED_SDIN_Set()   PTXn_T(OLED_SDIN_Pin,OUT)=1

#define OLED_RST_Clr()    PTXn_T(OLED_RST_Pin,OUT)=0//RES
#define OLED_RST_Set()    PTXn_T(OLED_RST_Pin,OUT)=1

#define OLED_DC_Clr()     PTXn_T(OLED_DC_Pin,OUT)=0//DC
#define OLED_DC_Set()     PTXn_T(OLED_DC_Pin,OUT)=1
 		     
//#define OLED_CS_Clr()     PTXn_T(OLED_CS_Pin,OUT)=0//CS
#define OLED_CS_Set()     PTXn_T(OLED_CS_Pin,OUT)=1

#define Max_Column	128
#define Max_Row		64
#define SIZE            16

#define OLED_CMD  0	//§Õ??
#define OLED_DATA 1	//§Õ??
	    
extern void oled_display_on(void);
extern void oled_display_off(void);	   							   		    
extern void oled_init(void);
extern void oled_clear(void);
extern void oled_show_char(uint8 x,uint8 y,uint8 chr,uint8 size,uint8 mode);
extern void oled_show_num(uint8 x,uint8 y,int32 num,uint8 size,uint8 mode);
extern void oled_show_string(uint8 x,uint8 y, uint8 *p,uint8 size,uint8 mode);	
extern void oled_show_logo(void);
extern void oled_show_picture(void);

#endif  

