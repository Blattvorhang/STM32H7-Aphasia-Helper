//OV7670摄像头模块接线说明：

//D0-D7：PF0-PF7
//WEN：PB6
//RCLK：PB7
//VSYNC：PA7
//SDA：PC6
//SCL：PC4
//RRST：PC2
//OE：PC3
//WRST：PE6
//GND：GND
//3.3V：3.3V

#ifndef _OV7670_H
#define _OV7670_H
#include "sys.h"
#include "sccb.h"							

//#define OV7670_RCK_H	HAL_GPIO_WritePin(GPIOE, OV7670_RCK_Pin, GPIO_PIN_SET)//设置读数据时钟高电平
//#define OV7670_RCK_L	HAL_GPIO_WritePin(GPIOE, OV7670_RCK_Pin, GPIO_PIN_RESET)//设置读数据时钟低电平
#define OV7670_RCK_H	GPIOE->BSRR =  OV7670_RCK_Pin//设置读数据时钟高电平
#define OV7670_RCK_L	GPIOE->BSRR = (OV7670_RCK_Pin << 16)//设置读数据时钟低电平
#define OV7670_RRST_H	GPIOF->BSRR =  OV7670_RRST_Pin
#define OV7670_RRST_L	GPIOF->BSRR = (OV7670_RRST_Pin << 16)
//#define OV7670_WRST		PFout(9)		//写指针复位
//#define OV7670_WREN		PFout(7)		//写入FIFO使能
//#define OV7670_RRST		PFout(8)  		//读指针复位
//#define OV7670_CS			PDout(10)  		//片选信号(OE)


u8   OV7670_Init(void);		  	   		 
void OV7670_Light_Mode(u8 mode);
void OV7670_Color_Saturation(u8 sat);
void OV7670_Brightness(u8 bright);
void OV7670_Contrast(u8 contrast);
void OV7670_Special_Effects(u8 eft);
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height);
void OV7670_colorbar_test (void);
void OV_EXTI_Control(FunctionalState State);

#endif




















