#include "LD3320.h"
#include <stdio.h>
#include "spi.h"

#include "delay.h"
/************************************************************************************
//	nAsrStatus 用来在main主程序中表示程序运行的状态，不是LD3320芯片内部的状态寄存器
//	LD_ASR_NONE:			表示没有在作ASR识别
//	LD_ASR_RUNING：		表示LD3320正在作ASR识别中
//	LD_ASR_FOUNDOK:		表示一次识别流程结束后，有一个识别结果
//	LD_ASR_FOUNDZERO:	表示一次识别流程结束后，没有识别结果
//	LD_ASR_ERROR:			表示一次识别流程中LD3320芯片内部出现不正确的状态
********************************************************************************
*/

#define USE_HAL_LIB//使用HAL库
uint8_t nAsrStatus=0;
uint8_t nAsrRes=0;
uint8_t flag=0;
///用户修改
extern uint8_t nAsrStatus;
uint8_t  nLD_Mode=LD_MODE_IDLE;
uint8_t  ucRegVal;

extern uint8_t confirm_flag;
extern ImageIdx choice;

#define LD3320_SPI	hspi1

/*硬件SPI读写*/
uint8_t SPI_RreadWrite_Data(uint8_t *tx_data, uint8_t len)
{
	uint8_t rx_data[len];
	HAL_SPI_TransmitReceive(&LD3320_SPI,tx_data,rx_data,len,0XFFFF);
	return rx_data[len-1];
}



uint8_t LD3320_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();

	HAL_Delay(500);
	
	HAL_GPIO_WritePin(LD3320_RST_GPIO_Port,LD3320_RST_Pin,GPIO_PIN_SET);//拉低WR引脚
	HAL_GPIO_WritePin(LD3320_CS_GPIO_Port,LD3320_CS_Pin,GPIO_PIN_SET);//拉低WR引脚
	HAL_GPIO_WritePin(LD3320_WR_GPIO_Port,LD3320_WR_Pin,GPIO_PIN_RESET);//拉低WR引脚
	return LD3320_Check();
}


/************************************************************************
功能描述： 	 复位LD模块
入口参数：	 none
返 回 值： 	 none
其他说明：	 none
**************************************************************************/
void LD_Reset(void)
{
	RST_HIGH;
	HAL_Delay(5);
	RST_LOW;
	HAL_Delay(5);
	RST_HIGH;
	
	HAL_Delay(5);
	CS_LOW;
	HAL_Delay(5);
	CS_HIGH;
	HAL_Delay(5);
}
/************************************************************************
功能描述： LD模块命令初始化
入口参数： none
返 回 值： none
其他说明： 该函数为出厂配置，一般不需要修改；
					 有兴趣的客户可对照开发手册根据需要自行修改。
**************************************************************************/
void LD_Init_Common(void)
{
	LD_ReadReg(0x06);  
	LD_WriteReg(0x17, 0x35);

	HAL_Delay(5);
	LD_ReadReg(0x06);  

	LD_WriteReg(0x89, 0x03); 
	HAL_Delay(5);
	LD_WriteReg(0xCF, 0x43);
	HAL_Delay(5);
	LD_WriteReg(0xCB, 0x02);
	
	/*PLL setting*/
	LD_WriteReg(0x11, LD_PLL_11);
	if (nLD_Mode == LD_MODE_MP3)
	{
		LD_WriteReg(0x1E,0x00);
		LD_WriteReg(0x19, LD_PLL_MP3_19); 
		LD_WriteReg(0x1B, LD_PLL_MP3_1B);		
		LD_WriteReg(0x1D, LD_PLL_MP3_1D);
	}
	else
	{
		LD_WriteReg(0x1E,0x00);
		LD_WriteReg(0x19, LD_PLL_ASR_19); 
		LD_WriteReg(0x1B, LD_PLL_ASR_1B);
	  LD_WriteReg(0x1D, LD_PLL_ASR_1D);
	}	
	HAL_Delay(5);
	
	LD_WriteReg(0xCD, 0x04);
	LD_WriteReg(0x17, 0x4c);
	HAL_Delay(5);
	LD_WriteReg(0xB9, 0x00);
	LD_WriteReg(0xCF, 0x4F);
	LD_WriteReg(0x6F, 0xFF);
}

/************************************************************************
功能描述： 	 LD模块 ASR功能初始化
入口参数：	 none
返 回 值： 	 none
其他说明：	 该函数为出厂配置，一般不需要修改；
					 有兴趣的客户可对照开发手册根据需要自行修改。
**************************************************************************/
void LD_Init_ASR(void)
{
	nLD_Mode=LD_MODE_ASR_RUN;
	LD_Init_Common();

	LD_WriteReg(0xBD, 0x00);
	LD_WriteReg(0x17, 0x48);
	HAL_Delay(5);

	LD_WriteReg(0x3C, 0x80);
	LD_WriteReg(0x3E, 0x07);
	LD_WriteReg(0x38, 0xff); 
	LD_WriteReg(0x3A, 0x07);
	
	LD_WriteReg(0x40, 0);
	LD_WriteReg(0x42, 8);
	LD_WriteReg(0x44, 0); 
	LD_WriteReg(0x46, 8);
	HAL_Delay(5);
}

/************************************************************************
功能描述： 	中断处理函数
入口参数：	 none
返 回 值： 	 none
其他说明：	当LD模块接收到音频信号时，将进入该函数，
						判断识别是否有结果，如果没有从新配置寄
            存器准备下一次的识别。
**************************************************************************/
void ProcessInt(void)
{
	uint8_t nAsrResCount=0;

	ucRegVal = LD_ReadReg(0x2B);
	LD_WriteReg(0x29,0) ;
	LD_WriteReg(0x02,0) ;
	if((ucRegVal & 0x10)&&LD_ReadReg(0xb2)==0x21&&LD_ReadReg(0xbf)==0x35)			/*识别成功*/
	{	
		nAsrResCount = LD_ReadReg(0xba);
		if(nAsrResCount>0 && nAsrResCount<=4) 
		{
			nAsrStatus=LD_ASR_FOUNDOK;
		}
		else
	  {
			nAsrStatus=LD_ASR_FOUNDZERO;
		}	
	}															 /*没有识别结果*/
	else
	{	 
		nAsrStatus=LD_ASR_FOUNDZERO;
	}
		
  LD_WriteReg(0x2b, 0);
  LD_WriteReg(0x1C,0);/*写0:ADC不可用*/
	LD_WriteReg(0x29,0);
	LD_WriteReg(0x02,0);
	LD_WriteReg(0x2B,0);
	LD_WriteReg(0xBA,0);	
	LD_WriteReg(0xBC,0);	
	LD_WriteReg(0x08,1);	 /*清除FIFO_DATA*/
	LD_WriteReg(0x08,0);	/*清除FIFO_DATA后 再次写0*/
}

/************************************************************************
功能描述：  检测LD模块是否空闲
入口参数：	none
返 回 值： 	flag：1-> 空闲
其他说明：	none
**************************************************************************/
uint8_t LD_Check_ASRBusyFlag_b2(void)
{
	uint8_t j,i;
	uint8_t flag = 0;
	for (j=0; j<5; j++)
	{
		i=LD_ReadReg(0xb2);
		if ( i== 0x21)
		{
			flag = 1;						
			break;
		}
		HAL_Delay(20);		
	}
	return flag;
}
/************************************************************************
功能描述： 	启动ASR
入口参数：	none
返 回 值： 	none
其他说明：	none
**************************************************************************/
void LD_AsrStart(void)
{
	LD_Init_ASR();
}
/************************************************************************
功能描述： 	运行ASR
入口参数：	none
返 回 值： 	1：启动成功
其他说明：	none
**************************************************************************/
uint8_t LD_AsrRun(void)
{
	LD_WriteReg(0x35, MIC_VOL);
	LD_WriteReg(0x1C, 0x09);
	LD_WriteReg(0xBD, 0x20);
	LD_WriteReg(0x08, 0x01);
	HAL_Delay(5);
	LD_WriteReg(0x08, 0x00);
	HAL_Delay(5);

	if(LD_Check_ASRBusyFlag_b2() == 0)
	{
		return 0;
	}

	LD_WriteReg(0xB2, 0xff);
	LD_WriteReg(0x37, 0x06);
	HAL_Delay(5);
    LD_WriteReg(0x37, 0x06);
	HAL_Delay(5);
	LD_WriteReg(0x1C, 0x0b);
	LD_WriteReg(0x29, 0x10);	
	LD_WriteReg(0xBD, 0x00);
	return 1;
}


/************************************************************************
功能描述： 	运行ASR识别流程
入口参数：	none
返 回 值：  asrflag：1->启动成功， 0—>启动失败
其他说明：	识别顺序如下:
						1、RunASR()函数实现了一次完整的ASR语音识别流程
						2、LD_AsrStart() 函数实现了ASR初始化
						3、LD_AsrAddFixed() 函数实现了添加关键词语到LD3320芯片中
						4、LD_AsrRun()	函数启动了一次ASR语音识别流程					
						任何一次ASR识别流程，都需要按照这个顺序，从初始化开始
**************************************************************************/
uint8_t RunASR(void)
{
	uint8_t i=0;
	uint8_t asrflag=0;
	for (i=0; i<5; i++)			//	防止由于硬件原因导致LD3320芯片工作不正常，所以一共尝试5次启动ASR识别流程
	{
		LD_AsrStart();
		HAL_Delay(5);
		if (LD_AsrAddFixed()==0)
		{
			LD_Reset();			//	LD3320芯片内部出现不正常，立即重启LD3320芯片
			HAL_Delay(5);			//	并从初始化开始重新ASR识别流程
			continue;
		}
		delay_us(5);
		if (LD_AsrRun() == 0)
		{
			LD_Reset();			//	LD3320芯片内部出现不正常，立即重启LD3320芯片
			HAL_Delay(5);			//	并从初始化开始重新ASR识别流程
			continue;
		}	
		asrflag=1;
		break;					//	ASR流程启动成功，退出当前for循环。开始等待LD3320送出的中断信号
	}
	return asrflag;
}

/************************************************************************
功能描述： 	获取识别结果
入口参数：	none
返 回 值： 	LD_ReadReg(0xc5 )；  读取内部寄存器返回识别码。
其他说明：	none
**************************************************************************/
uint8_t LD_GetResult(void)
{	
	return LD_ReadReg(0xc5 );
}

/*写寄存器*/
//void LD_WriteReg(uint8_t address,uint8_t dataout)
//{
//	#ifndef USE_HAL_LIB
//	uint8_t i = 0;
//	uint8_t command=0x04;
//	SCS = 0;
//	delay_us(1);
// 
//	//write command
//	for (i=0;i < 8; i++)
//	{
//		if (command & 0x80) 
//			SDI = 1;
//		else
//			SDI = 0;
//		
//		delay_us(1);
//		SDCK = 0;
//		command = (command << 1);  
//		delay_us(1);
//		SDCK = 1;  
//	}
//	//write address
//	for (i=0;i < 8; i++)
//	{
//		if (address & 0x80) 
//			SDI = 1;
//		else
//			SDI = 0;
//		delay_us(1);
//		SDCK = 0;
//		address = (address << 1); 
//		delay_us(1);
//		SDCK = 1;  
//	}
//	//write data
//	for (i=0;i < 8; i++)
//	{
//		if (dataout & 0x80) 
//			SDI = 1;
//		else
//			SDI = 0;
//		delay_us(1);
//		SDCK = 0;
//		dataout = (dataout << 1); 
//		delay_us(1);
//		SDCK = 1;  
//	}
//	delay_us(1);
//	SCS = 1;
//	#else
//	uint8_t tx_data[3];
//	CS_LOW;
//	delay_us(1);
//	tx_data[0] = 0x04;
//	tx_data[1] = address;
//	tx_data[2] = dataout;
//	SPI_RreadWrite_Data(tx_data,3);
//	delay_us(1);
//	CS_HIGH;
//	#endif
//}
/*读寄存器*/
//uint8_t LD_ReadReg(uint8_t address)
//{
//	#ifndef USE_HAL_LIB
//	uint8_t i = 0; 
//	uint8_t datain =0 ;
//	uint8_t temp = 0; 
//	uint8_t command=0x05;
//	SCS = 0;
//	delay_us(1);
// 
//	//write command
//	for (i=0;i < 8; i++)
//	{
//		if (command & 0x80) 
//			SDI = 1;
//		else
//			SDI = 0;
//		delay_us(1);
//		SDCK = 0;
//		command = (command << 1);  
//		delay_us(1);
//		SDCK = 1;  
//	}
// 
//	//write address
//	for (i=0;i < 8; i++)
//	{
//		if (address & 0x80) 
//			SDI = 1;
//		else
//			SDI = 0;
//		delay_us(1);
//		SDCK = 0;
//		address = (address << 1); 
//		delay_us(1);
//		SDCK = 1;  
//	}
//	delay_us(1);
// 
//	//Read
//	for (i=0;i < 8; i++)
//	{
//		datain = (datain << 1);
//		temp = SDO;
//		delay_us(1);
//		SDCK = 0;  
//		if (temp == 1)  
//			datain |= 0x01; 
//		delay_us(1);
//		SDCK = 1;  
//	}
// 
//	delay_us(1);
//	SCS = 1;
//	return datain;
//	#else
//	uint8_t rx_data;
//	uint8_t tx_data[3];
// 
//  CS_LOW;
//	delay_us(1);
//	tx_data[0] = 0x05;
//	tx_data[1] = address;
//	tx_data[2] = 0x00;
//	rx_data = SPI_RreadWrite_Data(tx_data,3);
//	delay_us(1);
//	CS_HIGH;
//	return rx_data;
//	#endif
//}



/*写寄存器*/
void LD_WriteReg(uint8_t address,uint8_t dataout)
{
	uint8_t tx_data[3];
	CS_LOW;
	delay_us(1);
	tx_data[0] = 0x04;
	tx_data[1] = address;
	tx_data[2] = dataout;
	SPI_RreadWrite_Data(tx_data,3);
	delay_us(1);
	CS_HIGH;
}
/*读寄存器*/
uint8_t LD_ReadReg(uint8_t address)
{
	uint8_t rx_data;
	uint8_t tx_data[3];
  	CS_LOW;
	delay_us(1);
	tx_data[0] = 0x05;
	tx_data[1] = address;
	tx_data[2] = 0x00;
	rx_data = SPI_RreadWrite_Data(tx_data,3);
	delay_us(1);
	CS_HIGH;
	return rx_data;
}

/************************************************************************
功能描述： 模块检测（检查寄存器的读写是否正确）
入口参数：	none
返 回 值：成功返回 0 ，失败返回1
其他说明：	none
**************************************************************************/
uint8_t LD3320_Check(void)
{
	uint8_t flag = 1;
	LD_Reset();//语音复位
	LD_ReadReg(0x6);
	LD_WriteReg(0x35, 0x33);
	LD_WriteReg(0x1b, 0x55);
	LD_WriteReg(0xb3, 0xaa);

	if(LD_ReadReg(0x35) == 0x33 && LD_ReadReg(0x1b) == 0x55 && LD_ReadReg(0xb3) == 0xaa)
	{
		flag = 0;
	}
	return flag;
}

/************************************************************************
功能描述： 向LD模块添加关键词
入口参数： none
返 回 值： flag：1->添加成功
其他说明： 用户修改.
					 1、根据如下格式添加拼音关键词，同时注意修改sRecog 和pCode 数组的长度
					 和对应变了k的循环置。拼音串和识别码是一一对应的。
					 2、开发者可以学习"语音识别芯片LD3320高阶秘籍.pdf"中
           关于垃圾词语吸收错误的用法，来提供识别效果。
**************************************************************************/
uint8_t LD_AsrAddFixed(void)
{
	uint8_t k, flag;
	uint8_t nAsrAddLength;
	
	#define DATE_A 10   //数组二维数值
	#define DATE_B 15		//数组一维数值
	
	
	uint8_t sRecog[DATE_A][DATE_B] = {					
																		"ni hao xiao v",\
																		"dai ma ce shi",\
																		"ce shi wan bi",\
		
																		"que ren",\
																		"qu xiao",\
																		"chi fan",\
																		"ce suo",\
		
																		"shui",\
																		"zhao dong xi",\
		
																		"bu shi ge men",\
																 };	/*添加关键词，用户修改*/
	uint8_t pCode[DATE_A] = {
													CODE_1KL1,\
													CODE_DMCS,\
													CODE_CSWB,\
												
													CODE_1KL1,\
		                      CODE_1KL2,\
		                      CODE_1KL3,\
		                      CODE_1KL4,\
		
													CODE_2KL1,\
		                      CODE_2KL2,\
		
													CODE_5KL1,
												};	/*添加识别码，用户修改*/	
	flag = 1;
	for (k=0; k<DATE_A; k++)
	{
			
		if(LD_Check_ASRBusyFlag_b2() == 0)
		{
			flag = 0;
			break;
		}
		
		LD_WriteReg(0xc1, pCode[k] );
		LD_WriteReg(0xc3, 0 );
		LD_WriteReg(0x08, 0x04);
		HAL_Delay(1);
		LD_WriteReg(0x08, 0x00);
		HAL_Delay(1);

		for (nAsrAddLength=0; nAsrAddLength<DATE_B; nAsrAddLength++)
		{
			if (sRecog[k][nAsrAddLength] == 0)
				break;
			LD_WriteReg(0x5, sRecog[k][nAsrAddLength]);
		}
		LD_WriteReg(0xb9, nAsrAddLength);
		LD_WriteReg(0xb2, 0xff);
		LD_WriteReg(0x37, 0x04);
	}
    return flag;
}
/***********************************************************
* 名    称：用户执行函数 
* 功    能：识别成功后，执行动作可在此进行修改 
* 入口参数： 无 
* 出口参数：无
* 说    明： 					 
**********************************************************/
void User_Modification(uint8_t dat)
{
	if(dat)
	{
		switch(nAsrRes)		   /*对结果执行相关操作,客户修改*/
		{
			case CODE_DMCS:			/*命令“代码测试”*/
					printf(" code test succeeded\r\n"); /*text.....*/
					break;
			case CODE_CSWB:			/*命令“测试完毕”*/
					printf(" test finish succeeded\r\n"); /*text.....*/
					break;
			
			case CODE_1KL1:	 /*命令“确认”*/
					printf(" confirm succeeded\r\n"); /*text.....*/
					confirm_flag = 1;
					break;
			case CODE_1KL2:		/*命令“取消”*/
					printf(" cancel succeeded\r\n"); /*text.....*/
					confirm_flag = 0;
					break;
			case CODE_1KL3:	 /*命令“吃饭”*/
					printf(" eat succeeded\r\n"); /*text.....*/
					confirm_flag = 1;
					choice = IMG_EAT;
					break;
			case CODE_1KL4:		/*命令“厕所”*/				
					printf(" WC succeeded\r\n"); /*text.....*/
					confirm_flag = 1;
					choice = IMG_WC;
					break;
			
			case CODE_2KL1:	 /*命令“水”*/
					printf(" water succeeded\r\n"); /*text.....*/
					confirm_flag = 1;
					choice = IMG_EAT;
					break;
			case CODE_2KL2:	 /*命令“找东西”*/
					printf(" find succeeded\r\n"); /*text.....*/
					confirm_flag = 1;
					choice = IMG_CAMERA;
					break;
						
			case CODE_5KL1:	 /*命令“不是哥们”*/
					printf(" bushi gemen succeeded\r\n"); /*text.....*/
					break;
//		case CODE_3KL4:	 /*命令“....”*/
//				printf("\"代码测试\"识别成功"); /*text.....*/
//											break;
//					
//					case CODE_4KL1:	 /*命令“....”*/
//							printf("O"); /*text.....*/
//														break;
//					case CODE_4KL2:	 /*命令“....”*/
//							printf("P"); /*text.....*/
//														break;
//					case CODE_4KL3:	 /*命令“....”*/
//							printf("Q"); /*text.....*/
//														break;
//					case CODE_4KL4:	 /*命令“....”*/
//							printf("R"); /*text.....*/
//														break;
			
			default:break;
		}
	}
	
}

void LD3320_main(void)
{
	uint8_t end_flag = 0;
	nAsrStatus = LD_ASR_NONE;		//	初始状态：没有在作ASR
	//CS_LOW;
	printf("identifying...\r\n");
	while(1)
	{
		switch(nAsrStatus)
		{
			case LD_ASR_RUNING:
			case LD_ASR_ERROR:	
				break;
			case LD_ASR_NONE:
			{
				nAsrStatus=LD_ASR_RUNING;
				if (RunASR()==0)	/*	启动一次ASR识别流程：ASR初始化，ASR添加关键词语，启动ASR运算*/
				{
					nAsrStatus = LD_ASR_ERROR;
				}
				break;
			}

			case LD_ASR_FOUNDOK: /*	一次ASR识别流程结束，去取ASR识别结果*/
			{
				nAsrRes = LD_GetResult();		/*获取结果*/												
				User_Modification(nAsrRes);
				end_flag = 1;
				nAsrStatus = LD_ASR_NONE;
				break;
			}
			case LD_ASR_FOUNDZERO:
			default:
			{
				nAsrStatus = LD_ASR_NONE;
				break;
			}
		} 
		if (end_flag) break;
	}
}


/*中断回调函数*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == LD3320_IRQ_Pin)
	{
		if(HAL_GPIO_ReadPin(LD3320_IRQ_GPIO_Port,LD3320_IRQ_Pin) == GPIO_PIN_RESET)
		{
			ProcessInt(); 
			printf("LD3320 interrupt entered.\r\n");	
		}
	}
}



