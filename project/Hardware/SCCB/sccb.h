#ifndef __SCCB_H
#define __SCCB_H
#include "sys.h"
#include "main.h"

//#define SCCB_SCL_L    		HAL_GPIO_WritePin(GPIOE,SCCB_SCL_Pin,GPIO_PIN_RESET)
//#define SCCB_SCL_H    		HAL_GPIO_WritePin(GPIOE,SCCB_SCL_Pin,GPIO_PIN_SET)
//#define SCCB_SDA_L    		HAL_GPIO_WritePin(GPIOE,SCCB_SDA_Pin,GPIO_PIN_RESET)
//#define SCCB_SDA_H    		HAL_GPIO_WritePin(GPIOE,SCCB_SDA_Pin,GPIO_PIN_SET)
#define SCCB_SCL_L    		GPIOE->BSRR =  (SCCB_SCL_Pin << 16)
#define SCCB_SCL_H        GPIOE->BSRR =  SCCB_SCL_Pin
#define SCCB_SDA_L    		GPIOE->BSRR =  (SCCB_SDA_Pin << 16)
#define SCCB_SDA_H        GPIOE->BSRR =  SCCB_SDA_Pin

//#define SCCB_READ_SDA    	HAL_GPIO_ReadPin(GPIOE, SCCB_SDA_Pin)
#define SCCB_READ_SDA     ((GPIOE->IDR & SCCB_SDA_Pin) != 0)
#define SCCB_ID_W   	    0X42  			//OV7670 ID for Write
#define SCCB_ID_R   	    0X43  			//OV7670 ID for Read

//#define OV7670_PWDN         HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11,GPIO_PIN_SET)
//#define OV7670_PWUP         HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11,GPIO_PIN_RESET)
//#define OV7670_RST  	      HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_RESET)
//#define OV7670_RUN  	      HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_SET)
//#define OV7670_VSYNC 	      HAL_GPIO_ReadPin(GPIOB, OV7670_VSYNC_Pin)
#define OV7670_VSYNC        ((GPIOA->IDR & OV7670_VSYNC_Pin) != 0)
//#define OV7670_HREF  	      HAL_GPIO_ReadPin(GPIOA, OV7670_HSYNC_Pin)
//#define OV7670_PCLK  	      HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)


void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
uint8_t SCCB_WR_Byte(uint8_t data);
uint8_t SCCB_RD_Byte(void);
uint8_t SCCB_WR_Reg(uint8_t reg,uint8_t data);
uint8_t SCCB_RD_Reg(uint8_t reg);

void SCCB_SDA_IN(void);
void SCCB_SDA_OUT(void);

#define ticknumber 12*10

void SCCB_Rst(void);


#endif













