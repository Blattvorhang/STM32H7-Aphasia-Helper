#ifndef __SYN6288_H
#define __SYN6288_H
 
#include "stm32h7xx_hal.h"
void SYN_FrameInfo(uint8_t Music, uint8_t *HZdata);
void YS_SYN_Set(uint8_t *Info_data);
void SYN_Speak(char *text);
 
#endif
