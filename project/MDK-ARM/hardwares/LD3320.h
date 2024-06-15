#ifndef LD3320_H__
#define LD3320_H__

#include "main.h"
#include "stm32h7xx_hal.h"
//	以下三个状态定义用来记录程序是在运行ASR识别还是在运行MP3播放
#define LD_MODE_IDLE			0x00
#define LD_MODE_ASR_RUN		0x08
#define LD_MODE_MP3		 		0x40


//	以下五个状态定义用来记录程序是在运行ASR识别过程中的哪个状态
#define LD_ASR_NONE				0x00	//	表示没有在作ASR识别
#define LD_ASR_RUNING			0x01	//	表示LD3320正在作ASR识别中
#define LD_ASR_FOUNDOK		0x10	//	表示一次识别流程结束后，有一个识别结果
#define LD_ASR_FOUNDZERO 	0x11	//	表示一次识别流程结束后，没有识别结果
#define LD_ASR_ERROR	 		0x31	//	表示一次识别流程中LD3320芯片内部出现不正确的状态


#define CLK_IN   				96	/* 用户注意修改输入的晶振时钟大小 */
#define LD_PLL_11				(uint8_t)((CLK_IN/2.0)-1)
#define LD_PLL_MP3_19		0x0f
#define LD_PLL_MP3_1B		0x18
#define LD_PLL_MP3_1D   (uint8_t)(((90.0*((LD_PLL_11)+1))/(CLK_IN))-1)

#define LD_PLL_ASR_19 	(uint8_t)(CLK_IN*32.0/(LD_PLL_11+1) - 0.51)
#define LD_PLL_ASR_1B 	0x48
#define LD_PLL_ASR_1D 	0x1f

#define MIC_VOL 0x43	 //咪头增益



//识别码客户修改处
#define CODE_CMD   0x00   //该命令码0x00用户不可进行修改。
#define CODE_DMCS  0x01		//代码测试
#define CODE_CSWB  0x02	  //测试完毕

#define CODE_1KL1 0x03
#define CODE_1KL2 0x04
#define CODE_1KL3 0x05
#define CODE_1KL4 0x06
                  
#define CODE_2KL1 0x18
#define CODE_2KL2 0x19
#define CODE_2KL3 0x1A
#define CODE_2KL4 0x1B
                  
#define CODE_3KL1 0x1C
#define CODE_3KL2 0x1D
#define CODE_3KL3 0x1E
#define CODE_3KL4 0x1F
                  
#define CODE_4KL1 0x20
#define CODE_4KL2 0x21
#define CODE_4KL3 0x22
#define CODE_4KL4 0x23

#define CODE_5KL1 0x24

#define RST_HIGH			HAL_GPIO_WritePin(LD3320_RST_GPIO_Port,LD3320_RST_Pin,GPIO_PIN_SET)
#define RST_LOW				HAL_GPIO_WritePin(LD3320_RST_GPIO_Port,LD3320_RST_Pin,GPIO_PIN_RESET)

#define SCK_HIGH			HAL_GPIO_WritePin(LD3320_SCK_GPIO_Port,LD3320_SCK_Pin,GPIO_PIN_SET)
#define SCK_LOW				HAL_GPIO_WritePin(LD3320_SCK_GPIO_Port,LD3320_SCK_Pin,GPIO_PIN_RESET)

#define MOSI_HIGH			HAL_GPIO_WritePin(LD3320_MOSI_GPIO_Port,LD3320_MOSI_Pin,GPIO_PIN_SET)
#define MOSI_LOW			HAL_GPIO_WritePin(LD3320_MOSI_GPIO_Port,LD3320_MOSI_Pin,GPIO_PIN_RESET)

#define IRQ_HIGH			HAL_GPIO_WritePin(LD3320_IRQ_GPIO_Port,LD3320_IRQ_Pin,GPIO_PIN_SET)
#define IRQ_LOW				HAL_GPIO_WritePin(LD3320_IRQ_GPIO_Port,LD3320_IRQ_Pin,GPIO_PIN_RESET)

#define CS_HIGH				HAL_GPIO_WritePin(LD3320_CS_GPIO_Port,LD3320_CS_Pin,GPIO_PIN_SET)
#define CS_LOW				HAL_GPIO_WritePin(LD3320_CS_GPIO_Port,LD3320_CS_Pin,GPIO_PIN_RESET)
void LD3320_main(void);

void User_Modification(uint8_t dat);

//修改这两个函数可以改变指令
// LD_AsrAddFixed
//User_Modification
//函数声明
void LD_Reset(void);
void LD_Init_Common(void);
void LD_Init_ASR(void);
void ProcessInt(void);
void LD_AsrStart(void);
uint8_t LD_AsrRun(void);
uint8_t LD_AsrAddFixed(void);
uint8_t RunASR(void);
uint8_t LD_GetResult(void);

uint8_t LD3320_Init(void);
void LD_WriteReg(uint8_t address,uint8_t dataout);
uint8_t LD_ReadReg(uint8_t address);
uint8_t LD3320_Check(void);

#endif



