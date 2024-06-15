//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F103RCT6,正点原子MiniSTM32开发板,主频72MHZ，晶振12MHZ
//QDtech-TFT液晶驱动 for STM32 IO模拟
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//淘宝网站：http://qdtech.taobao.com
//wiki技术网站：http://www.lcdwiki.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
//技术支持QQ:3002773612  3002778157
//技术交流QQ群:324828016
//创建日期:2018/08/22
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================电源接线================================================//
//     LCD模块                STM32单片机
//      VCC          接        DC5V/3.3V      //电源
//      GND          接          GND          //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为SPI总线
//     LCD模块                STM32单片机    
//       SDA         接          PB15         //液晶屏SPI总线数据写信号
//=======================================液晶屏控制线接线==========================================//
//     LCD模块 					      STM32单片机 
//       LED         接          PB9          //液晶屏背光控制信号，如果不需要控制，接5V或3.3V
//       SCK         接          PB13         //液晶屏SPI总线时钟信号
//       A0          接          PB10         //液晶屏数据/命令控制信号
//       RESET       接          PB12         //液晶屏复位控制信号
//       CS          接          PB11         //液晶屏片选控制信号
//=========================================触摸屏触接线=========================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块                STM32单片机 
//      T_IRQ        接          PC10         //触摸屏触摸中断信号
//      T_DO         接          PC2          //触摸屏SPI总线读信号
//      T_DIN        接          PC3          //触摸屏SPI总线写信号
//      T_CS         接          PC13         //触摸屏片选控制信号
//      T_CLK        接          PC0          //触摸屏SPI总线时钟信号
**************************************************************************************************/	
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/		
#include "lcd.h"
#include "delay.h"
#include "gui.h"
#include "test.h"
#include "pic.h"

//========================variable==========================//
u16 ColorTab[5]={RED,GREEN,BLUE,YELLOW,BRED};//定义颜色数组
//=====================end of variable======================//

/*****************************************************************************
 * @name       :void DrawTestPage(u8 *str)
 * @date       :2018-08-09 
 * @function   :Drawing test interface
 * @parameters :str:the start address of the Chinese and English strings
 * @retvalue   :None
******************************************************************************/ 
void DrawTestPage(u8 *str)
{
//绘制固定栏up
LCD_Clear(WHITE);
LCD_Fill(0,0,lcddev.width,20,BLUE);
//绘制固定栏down
LCD_Fill(0,lcddev.height-20,lcddev.width,lcddev.height,BLUE);
POINT_COLOR=WHITE;
Gui_StrCenter(0,2,WHITE,BLUE,str,16,1);//居中显示
Gui_StrCenter(0,lcddev.height-18,WHITE,BLUE,"www.lcdwiki.com",16,1);//居中显示
//绘制测试区域
//LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
}

/*****************************************************************************
 * @name       :void Display_ButtonUp(u16 x1,u16 y1,u16 x2,u16 y2)
 * @date       :2018-08-24 
 * @function   :Drawing a 3D button
 * @parameters :x1:the bebinning x coordinate of the button
                y1:the bebinning y coordinate of the button
								x2:the ending x coordinate of the button
								y2:the ending y coordinate of the button
 * @retvalue   :None
******************************************************************************/ 
void Display_ButtonUp(u16 x1,u16 y1,u16 x2,u16 y2)
{
	POINT_COLOR=WHITE;
	LCD_DrawLine(x1,  y1,  x2,y1); //H
	LCD_DrawLine(x1,  y1,  x1,y2); //V
	
	POINT_COLOR=GRAY1;
	LCD_DrawLine(x1+1,y2-1,x2,y2-1);  //H
	POINT_COLOR=GRAY2;
	LCD_DrawLine(x1,y2,x2,y2);  //H
	POINT_COLOR=GRAY1;
	LCD_DrawLine(x2-1,y1+1,x2-1,y2);  //V
	POINT_COLOR=GRAY2;
  LCD_DrawLine(x2,y1,x2,y2); //V
}

/*****************************************************************************
 * @name       :void menu_test(void)
 * @date       :2018-08-24 
 * @function   :Drawing a 3D menu UI
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 
void menu_test(void)
{
	LCD_Clear(GRAY0);
	
	Display_ButtonUp(lcddev.width / 4,8,lcddev.width * 3 / 4,28); //x1,y1,x2,y2
	Gui_StrCenter(0,10,BRED,BLUE,"图形显示测试",16,1);

	Display_ButtonUp(lcddev.width / 4,38,lcddev.width * 3 / 4,58); //x1,y1,x2,y2
	Gui_StrCenter(0,40,BLACK,GRAY0,"纯色填充测试",16,1);
	
	Display_ButtonUp(lcddev.width / 4,68,lcddev.width * 3 / 4,88); //x1,y1,x2,y2
	Gui_StrCenter(0,70,BLUE,GRAY0,"中文显示测试",16,1);

	Display_ButtonUp(lcddev.width / 4,98,lcddev.width * 3 / 4,118); //x1,y1,x2,y2
	Gui_StrCenter(16,100,RED,GRAY0,"图片显示测试",16,1);
	delay_ms(2000);
}

/*****************************************************************************
 * @name       :void main_test(void)
 * @date       :2018-08-09 
 * @function   :Drawing the main Interface of the Comprehensive Test Program
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void main_test(void)
{
	DrawTestPage("综合测试程序");	
	Gui_StrCenter(0,23,RED,BLUE,"全动电子",16,1);//居中显示
	Gui_StrCenter(0,40,RED,BLUE,"综合测试程序",16,1);//居中显示	
	Gui_StrCenter(0,57,GREEN,BLUE,"1.44\" ST7735S",16,1);//居中显示
	Gui_StrCenter(0,74,GREEN,BLUE,"128X128",16,1);//居中显示
	Gui_StrCenter(0,91,BLUE,BLUE,"2018-08-24",16,1);//居中显示
	delay_ms(1500);		
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Test_Color(void)
 * @date       :2018-08-09 
 * @function   :Color fill test(white,black,red,green,blue)
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Color(void)
{
	//DrawTestPage("测试1:纯色填充测试");
	LCD_Fill(0,0, lcddev.width - 1, lcddev.height - 1, WHITE);
	Show_Str(20,30,BLUE,YELLOW,"BL Test",16,1);delay_ms(800);
	LCD_Fill(0,0, lcddev.width - 1, lcddev.height - 1, RED);
	Show_Str(20,30,BLUE,YELLOW,"RED ",16,1);delay_ms(800);
	LCD_Fill(0,0, lcddev.width - 1, lcddev.height - 1,GREEN);
	Show_Str(20,30,BLUE,YELLOW,"GREEN ",16,1);delay_ms(800);
	LCD_Fill(0,0, lcddev.width - 1, lcddev.height - 1, BLUE);
	Show_Str(20,30,RED,YELLOW,"BLUE ",16,1);delay_ms(800);
}

/*****************************************************************************
 * @name       :void Test_FillRec(void)
 * @date       :2018-08-09 
 * @function   :Rectangular display and fill test
								Display red,green,blue,yellow,pink rectangular boxes in turn,
								1500 milliseconds later,
								Fill the rectangle in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_FillRec(void)
{
	u8 i=0;
	DrawTestPage("GUI矩形填充测试");
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
	for (i=0; i<5; i++) 
	{
		POINT_COLOR=ColorTab[i];
		LCD_DrawRectangle(lcddev.width/2-40+(i*16),lcddev.height/2-40+(i*13),lcddev.width/2-40+(i*16)+30,lcddev.height/2-40+(i*13)+30); 
	}
	delay_ms(1500);	
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE); 
	for (i=0; i<5; i++) 
	{
		POINT_COLOR=ColorTab[i];
		LCD_DrawFillRectangle(lcddev.width/2-40+(i*16),lcddev.height/2-40+(i*13),lcddev.width/2-40+(i*16)+30,lcddev.height/2-40+(i*13)+30); 
	}
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Test_Circle(void)
 * @date       :2018-08-09 
 * @function   :circular display and fill test
								Display red,green,blue,yellow,pink circular boxes in turn,
								1500 milliseconds later,
								Fill the circular in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Circle(void)
{
	u8 i=0;
	DrawTestPage("GUI画圆填充测试");
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
	for (i=0; i<5; i++)  
		gui_circle(lcddev.width/2-40+(i*15),lcddev.height/2-25+(i*13),ColorTab[i],15,0);
	delay_ms(1500);	
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE); 
	for (i=0; i<5; i++) 
	  	gui_circle(lcddev.width/2-40+(i*15),lcddev.height/2-25+(i*13),ColorTab[i],15,1);
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void English_Font_test(void)
 * @date       :2018-08-09 
 * @function   :English display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void English_Font_test(void)
{
	DrawTestPage("英文显示测试");
	Show_Str(10,22,BLUE,YELLOW,"6X12:abcdefgh01234567",12,0);
	Show_Str(10,34,BLUE,YELLOW,"6X12:ABCDEFGH01234567",12,1);
	Show_Str(10,47,BLUE,YELLOW,"6X12:~!@#$%^&*()_+{}:",12,0);
	Show_Str(10,60,BLUE,YELLOW,"8X16:abcde01234",16,0);
	Show_Str(10,76,BLUE,YELLOW,"8X16:ABCDE01234",16,1);
	Show_Str(10,92,BLUE,YELLOW,"8X16:~!@#$%^&*()",16,0); 
	delay_ms(1200);
}

/*****************************************************************************
 * @name       :void Test_Triangle(void)
 * @date       :2018-08-09 
 * @function   :triangle display and fill test
								Display red,green,blue,yellow,pink triangle boxes in turn,
								1500 milliseconds later,
								Fill the triangle in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Triangle(void)
{
	u8 i=0;
	DrawTestPage("GUI Tri填充测试");
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
	for(i=0;i<5;i++)
	{
		POINT_COLOR=ColorTab[i];
		Draw_Triangel(lcddev.width/2-40+(i*15),lcddev.height/2-12+(i*11),lcddev.width/2-25-1+(i*15),lcddev.height/2-12-26-1+(i*11),lcddev.width/2-10-1+(i*15),lcddev.height/2-12+(i*11));
	}
	delay_ms(1500);	
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE); 
	for(i=0;i<5;i++)
	{
		POINT_COLOR=ColorTab[i];
		Fill_Triangel(lcddev.width/2-40+(i*15),lcddev.height/2-12+(i*11),lcddev.width/2-25-1+(i*15),lcddev.height/2-12-26-1+(i*11),lcddev.width/2-10-1+(i*15),lcddev.height/2-12+(i*11));
	}
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Chinese_Font_test(void)
 * @date       :2018-08-09 
 * @function   :chinese display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Chinese_Font_test(void)
{	
	DrawTestPage("中文显示测试");
	Show_Str(10,25,BLUE,YELLOW,"16X16:全动电子欢迎",16,0);
	Show_Str(10,45,BLUE,YELLOW,"24X24:中文测试",24,1);
	Show_Str(10,70,BLUE,YELLOW,"32X32:字体测试",32,1);
	delay_ms(1200);
}

/*****************************************************************************
 * @name       :void Pic_test(void)
 * @date       :2018-08-09 
 * @function   :picture display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Pic_test(void)
{
// 	DrawTestPage("图片显示测试");
// 	//LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
// 	Gui_Drawbmp16(20,30,gImage_qq);
// 	Show_Str(20+12,75,BLUE,YELLOW,"QQ",16,1);
// 	Gui_Drawbmp16(70,30,gImage_qq);
// 	Show_Str(70+12,75,BLUE,YELLOW,"QQ",16,1);
// //	Gui_Drawbmp16(150,30,gImage_qq);
// //	Show_Str(150+12,75,BLUE,YELLOW,"QQ",16,1);
	Gui_Drawimg(0, 0, 320, 240, gImage_choose_eat);
	delay_ms(50);
	Gui_Drawimg(0, 0, 320, 240, gImage_choose_camera);
	delay_ms(50);
	Gui_Drawimg(0, 0, 320, 240, gImage_choose_wc);
	delay_ms(50);
	Gui_Drawimg(0, 0, 320, 240, gImage_comfirm_eat);
	delay_ms(50);
	Gui_Drawimg(0, 0, 320, 240, gImage_comfirm_camera);
	delay_ms(50);
	Gui_Drawimg(0, 0, 320, 240, gImage_comfirm_wc);
	delay_ms(50);
	Gui_Drawimg(0, 0, 320, 240, gImage_call_eat);
	delay_ms(50);
	Gui_Drawimg(0, 0, 320, 240, gImage_call_camera);
	delay_ms(50);
	Gui_Drawimg(0, 0, 320, 240, gImage_call_wc);
	delay_ms(1200);
}

/*****************************************************************************
 * @name       :void Rotate_Test(void)
 * @date       :2018-08-09 
 * @function   :rotate test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Rotate_Test(void)
{
	u8 i=0;
	u8 *Direction[4]={"Rotation:0","Rotation:90","Rotation:180","Rotation:270"};
	
	for(i=0;i<4;i++)
	{
	LCD_direction(i);
	DrawTestPage("屏幕旋转测试");
	Show_Str(20,30,BLUE,YELLOW,Direction[i],16,1);
	//Gui_Drawbmp16(30,50,gImage_qq);
	Gui_Drawimg(0, 0, 320, 240, gImage_choose_camera);
	delay_ms(1000);delay_ms(1000);
	}
	LCD_direction(USE_HORIZONTAL);
}

void Frame_Rate_Test(void)
{
	u8 i = 0;
	for (i = 0; i < 5; i++) {
		LCD_Fill(0,0,lcddev.width,lcddev.height,WHITE);
		LCD_Fill(0,0,lcddev.width,lcddev.height,RED);
		LCD_Fill(0,0,lcddev.width,lcddev.height,GREEN);
		LCD_Fill(0,0,lcddev.width,lcddev.height,BLUE);
	}
}
