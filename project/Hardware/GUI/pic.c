#include "lcd.h"
#include "delay.h"
#include "gui.h"
#include "test.h"
#include "pic.h"

void UI_Button(u16 x1,u16 y1,u16 x2,u16 y2)
{
	POINT_COLOR=WHITE;
	LCD_DrawFillRectangle(x1, y1, x2, y1+2); //H
	LCD_DrawFillRectangle(x1, y1, x1+2, y2); //V
	
	POINT_COLOR=GRAY1;
	LCD_DrawFillRectangle(x1+3,y2-3,x2,y2-1);  //H
	POINT_COLOR=GRAY2;
	LCD_DrawFillRectangle(x1,y2,x2,y2+1);  //H
	GUI_DrawPoint(x1+2, y2-1, GRAY1);
	GUI_DrawPoint(x1+1, y2-1, GRAY1);
	GUI_DrawPoint(x1+2, y2-2, GRAY1);

	POINT_COLOR=GRAY1;
	LCD_DrawFillRectangle(x2-3,y1+3,x2-1,y2-1);  //V
	POINT_COLOR=GRAY2;
  	LCD_DrawFillRectangle(x2,y1,x2+1,y2+1); //V
	GUI_DrawPoint(x2-2, y1+2, GRAY1);
	GUI_DrawPoint(x2-1, y1+2, GRAY1);
	GUI_DrawPoint(x2-1, y1+1, GRAY1);
}

void UI_cancel(void)
{
	POINT_COLOR = BRRED;
    LCD_DrawFillRectangle(10, 10, 70, 40);
	UI_Button(10, 10, 70, 40); //x1,y1,x2,y2
	Show_Str(17,17,WHITE,BRRED,"Cancel",16,1);
}

void _draw_quarter_circle(int xc, int yc, int x, int y, u16 c)
{
    GUI_DrawPoint(xc + x, yc - y, c); // 右上象限
    GUI_DrawPoint(xc + y, yc - x, c); // 右上象限
}

void gui_quarter_circle(int xc, int yc, u16 c, int r)
{
    int x = 0, y = r, yi;
    int d = 3 - 2 * r;

    while (x <= y) {
		for (yi = y-3; yi <= y; yi++)
        	_draw_quarter_circle(xc, yc, x, yi, c);
        if (d < 0) {
            d = d + 4 * x + 6;
        } 
		else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void UI_menu(void)
{
	LCD_Clear(GRAY0);
	
    POINT_COLOR = DARKGREEN;
    LCD_DrawFillRectangle(10, 10, 103, 230);
	UI_Button(10, 10, 103, 230); //x1,y1,x2,y2
	Show_Str(40,110,WHITE,DARKGREEN,"EAT",16,1);

    POINT_COLOR = DARKBLUE;
    LCD_DrawFillRectangle(113, 10, 207, 230);
	UI_Button(113, 10, 207, 230); //x1,y1,x2,y2
	Show_Str(143,110,WHITE,DARKBLUE,"FIND",16,1);

    POINT_COLOR = DARKYELLOW;
    LCD_DrawFillRectangle(217, 10, 310, 230);
	UI_Button(217, 10, 310, 230); //x1,y1,x2,y2
	Show_Str(247,110,WHITE,YELLOW,"WC",16,1);
}

void UI_menu_select_eat(void) 
{
    LCD_Clear(GRAY0);
    
    // 放大 EAT
    POINT_COLOR = DARKGREEN;
    LCD_DrawFillRectangle(10, 10, 124, 230); // 放大至 1.2 倍宽度
    UI_Button(10, 10, 124, 230);
    Show_Str(55, 110, WHITE, DARKGREEN, "EAT", 16, 1);
    
    // 缩小 FIND
    POINT_COLOR = DARKBLUE;
    LCD_DrawFillRectangle(134, 40, 217, 200); // 缩小至合适宽度
    UI_Button(134, 40, 217, 200);
    Show_Str(157, 110, WHITE, DARKBLUE, "FIND", 16, 1);
    
    // 缩小 WC
    POINT_COLOR = DARKYELLOW;
    LCD_DrawFillRectangle(227, 40, 310, 200); // 缩小至合适宽度
    UI_Button(227, 40, 310, 200);
    Show_Str(260, 110, WHITE, DARKYELLOW, "WC", 16, 1);
}

void UI_menu_select_find(void) 
{
    LCD_Clear(GRAY0);
    
    // 缩小 EAT
    POINT_COLOR = DARKGREEN;
    LCD_DrawFillRectangle(10, 40, 93, 200); // 缩小至合适宽度
    UI_Button(10, 40, 93, 200);
    Show_Str(40, 110, WHITE, DARKGREEN, "EAT", 16, 1);
    
    // 放大 FIND
    POINT_COLOR = DARKBLUE;
    LCD_DrawFillRectangle(103, 10, 217, 230); // 放大至 1.2 倍宽度
    UI_Button(103, 10, 217, 230);
    Show_Str(150, 110, WHITE, DARKBLUE, "FIND", 16, 1);
    
    // 缩小 WC
    POINT_COLOR = DARKYELLOW;
    LCD_DrawFillRectangle(227, 40, 310, 200); // 缩小至合适宽度
    UI_Button(227, 40, 310, 200);
    Show_Str(260, 110, WHITE, DARKYELLOW, "WC", 16, 1);
}

void UI_menu_select_wc(void) 
{
    LCD_Clear(GRAY0);
    
    // 缩小 EAT
    POINT_COLOR = DARKGREEN;
    LCD_DrawFillRectangle(10, 40, 93, 200); // 缩小至合适宽度
    UI_Button(10, 40, 93, 200);
    Show_Str(40, 110, WHITE, DARKGREEN, "EAT", 16, 1);
    
    // 缩小 FIND
    POINT_COLOR = DARKBLUE;
    LCD_DrawFillRectangle(103, 40, 186, 200); // 缩小至合适宽度
    UI_Button(103, 40, 186, 200);
    Show_Str(133, 110, WHITE, DARKBLUE, "FIND", 16, 1);
    
    // 放大 WC
    POINT_COLOR = DARKYELLOW;
    LCD_DrawFillRectangle(196, 10, 310, 230); // 放大至 1.2 倍宽度
    UI_Button(196, 10, 310, 230);
    Show_Str(243, 110, WHITE, DARKYELLOW, "WC", 16, 1);
}

void UI_call_eat(void)
{
	LCD_Clear(GRAY0);

	gui_quarter_circle(185, 90, DARKGREEN, 30);
	gui_quarter_circle(185, 90, DARKGREEN, 20);
	gui_quarter_circle(185, 90, DARKGREEN, 10);
	POINT_COLOR = DARKGREEN;
    LCD_DrawFillRectangle(135, 90, 185, 172); 
	POINT_COLOR = WHITE;
    LCD_DrawFillRectangle(138, 140, 182, 142); 
    LCD_DrawFillRectangle(138, 150, 182, 152); 
    LCD_DrawFillRectangle(138, 160, 182, 162); 
	LCD_DrawFillRectangle(138, 169, 182, 170); 

    LCD_DrawFillRectangle(138, 140, 139, 170); 
    LCD_DrawFillRectangle(148, 140, 150, 170); 
    LCD_DrawFillRectangle(159, 140, 161, 170); 
    LCD_DrawFillRectangle(170, 140, 172, 170); 
	LCD_DrawFillRectangle(181, 140, 182, 170); 

	LCD_DrawRectangle(138, 93, 182, 136);
	LCD_DrawRectangle(139, 94, 181, 135);

    Show_Str(125, 180, BLACK, GRAY0, "Calling...", 16, 1);
	UI_cancel();
}

void UI_call_wc(void)
{
	LCD_Clear(GRAY0);

	gui_quarter_circle(185, 90, DARKYELLOW, 30);
	gui_quarter_circle(185, 90, DARKYELLOW, 20);
	gui_quarter_circle(185, 90, DARKYELLOW, 10);
	POINT_COLOR = DARKYELLOW;
    LCD_DrawFillRectangle(135, 90, 185, 172); 
	POINT_COLOR = WHITE;
    LCD_DrawFillRectangle(138, 140, 182, 142); 
    LCD_DrawFillRectangle(138, 150, 182, 152); 
    LCD_DrawFillRectangle(138, 160, 182, 162); 
	LCD_DrawFillRectangle(138, 169, 182, 170); 

    LCD_DrawFillRectangle(138, 140, 139, 170); 
    LCD_DrawFillRectangle(148, 140, 150, 170); 
    LCD_DrawFillRectangle(159, 140, 161, 170); 
    LCD_DrawFillRectangle(170, 140, 172, 170); 
	LCD_DrawFillRectangle(181, 140, 182, 170); 

	LCD_DrawRectangle(138, 93, 182, 136);
	LCD_DrawRectangle(139, 94, 181, 135);
	
    Show_Str(125, 180, BLACK, GRAY0, "Calling...", 16, 1);
	UI_cancel();
}
