#ifndef __PIC_H
#define __PIC_H 

#include "sys.h"

void UI_Button(u16 x1,u16 y1,u16 x2,u16 y2);
void UI_menu(void);
void UI_menu_select_eat(void);
void UI_menu_select_find(void);
void UI_menu_select_wc(void);
void _draw_quarter_circle(int xc, int yc, int x, int y, u16 c);
void gui_quarter_circle(int xc, int yc, u16 c, int r);
void UI_cancel(void);
void UI_call_eat(void);
void UI_call_wc(void);

#endif
