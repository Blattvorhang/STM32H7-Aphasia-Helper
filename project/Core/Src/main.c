/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"
#include "app_x-cube-ai.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "test.h"
#include "ov7670.h"
#include "LD3320.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "mpu6050.h"
#include "music.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern uint8_t ov_sta;	//在stm32h7xx_it里面定义
uint16_t screen_buffer[240][320];
signed char gyro_orientation[9] = { 1,  0,  0,
                                    0, -1,  0,
                                    0,  0, -1};
extern float pitch,roll,yaw; 		//欧拉角
uint8_t mpu_count = 0;
uint8_t rev_flag = 0;						
																		
uint8_t confirm_flag = 0;
ImageIdx choice = IMG_CAMERA;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */
void camera_refresh(void);
void ChioceConfirm(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint8_t last_infrared = 0;
	ImageIdx last_choice = NO_CHOICE;
	uint8_t mpu_success_flag = 0;  // 表示陀螺仪成功读到数据，一旦成功一次，后续基本都可以持续读数据
  /* USER CODE END 1 */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */
	delay_init(280);	     //延时初始化
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_HS_USB_Init();
  MX_SPI3_Init();
  MX_SPI1_Init();
  MX_I2C1_Init();
  MX_TIM7_Init();
  MX_X_CUBE_AI_Init();
  /* USER CODE BEGIN 2 */
	LCD_Init();	   //液晶屏初始化
	HAL_Delay(50);
	
	HAL_GPIO_WritePin(GPIOC, OV7670_RST_Pin,GPIO_PIN_SET); // before init of OV7670
	while(OV7670_Init())//初始化OV7670
	{
		LCD_ShowString(100,20,16,"ERROR",1);
		delay_ms(200);
		LCD_Fill(0,80,239,206,BLACK);
	}
	LCD_ShowString(200,200,16,"ok",1);     
	delay_ms(200);	 
	OV7670_Light_Mode(4);
	OV7670_Brightness(4);	
	//OV7670_Color_Saturation(2);
	//OV7670_Contrast(2);
 	//OV7670_Special_Effects(0);
  HAL_GPIO_WritePin(GPIOD, OV7670_OE_Pin, GPIO_PIN_RESET);	
	LCD_Clear(BLACK);

	// LD3320_Init();
	MPU_6050_Init();// 可以尝试 直接打开FIFO
	mpu_dmp_init();//初始化DMP
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		/* 非确认状态 */
		if (!confirm_flag) {		
			// 读取陀螺仪数据
			if(!rev_flag)
			{
				mpu_success_flag = 1;
				//printf("%.2f, %.2f, %.2f \r\n",pitch,roll,yaw);
				if (roll < -45) 
					printf("forward\r\n");
				else if (roll > 45) 
					printf("back\r\n");
				if (pitch < -45) 
				{
					printf("left\r\n");
					choice = IMG_EAT;	
					if (choice != last_choice)
						UI_menu_select_eat();
				}
				else if (pitch > 45) 
				{
					printf("right\r\n");
					choice = IMG_WC;
					if (choice != last_choice)
						UI_menu_select_wc();
				}
				if (roll < 30 && roll > -30 && pitch < 30 && pitch > -30) 
				{
					printf("still\r\n");
					choice = IMG_CAMERA;
					if (choice != last_choice)
						UI_menu_select_find();
				}
				last_choice = choice;
			}  
			else {
				if (mpu_count >= 8) {
					printf("rev_flag %d\n",rev_flag);
					mpu_count = 0;
				}
			}// end of if (!rev_flag)
			HAL_Delay(50);
		}  // end of if (!confirm_flag) 
		
		else {  // 确认状态
			HAL_TIM_Base_Stop_IT(&htim7); // 关闭陀螺仪的定时器中断，便于执行代码的顺利运行
			switch (choice) {
				case IMG_CAMERA:
					camera_refresh();
					break;
				case IMG_EAT:
					PlaySong(LORD_OF_CINDER);
					break;
				case IMG_WC:
					PlaySong(CITY_OF_TEARS);
					break;
				default:
					break;
			}
			HAL_TIM_Base_Start_IT(&htim7); // 恢复定时器中断
		}
		
		if (!mpu_success_flag) {  // 陀螺仪没读到数据时，持续显示加载页面，读一次数据再判断
			LCD_Clear(WHITE);
			LCD_ShowString(120,120,16,"Loading...",1);
			HAL_Delay(20);
			continue;
 		}

		// 切换到遮挡红外，代表确认或取消
		if (HAL_GPIO_ReadPin(INFRARED_GPIO_Port, INFRARED_Pin) == 0 && last_infrared == 1) 
		{
			if (!confirm_flag){
				last_choice = NO_CHOICE;
				ChioceConfirm();
			}
			confirm_flag = !confirm_flag;
		}
		/*
		// 如果没有遮挡红外，调用语音模块作为default，直接修改confirm_flag和choice
		else if (HAL_GPIO_ReadPin(INFRARED_GPIO_Port, INFRARED_Pin)) 
		{
			LD3320_main();
			if (confirm_flag) 
				ChioceConfirm();
		}
		*/
		last_infrared = HAL_GPIO_ReadPin(INFRARED_GPIO_Port, INFRARED_Pin);
		
    /* USER CODE END WHILE */

  //MX_X_CUBE_AI_Process();
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /*AXI clock gating */
  RCC->CKGAENR = 0xFFFFFFFF;

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 70;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_SPI3
                              |RCC_PERIPHCLK_SPI1;
  PeriphClkInitStruct.PLL3.PLL3M = 1;
  PeriphClkInitStruct.PLL3.PLL3N = 24;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 4;
  PeriphClkInitStruct.PLL3.PLL3R = 2;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_3;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL3;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL3;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void camera_refresh(void)
{
	u32 i, j;
 	u16 color;
	//static uint16_t ov_frame = 0;
	if(ov_sta)//有帧中断更新？
	{
	  OV_EXTI_Control(DISABLE);
		LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);
		
		OV7670_RRST_L;				//开始复位读指针 
		OV7670_RCK_L;
		OV7670_RCK_H;
		OV7670_RCK_L;
		OV7670_RRST_H;				//复位读指针结束 
		OV7670_RCK_H;

		for (i = 0; i < 240; i++) {
			for(j = 0; j < 320;j++)   //此种方式需清楚TFT内部显示方向控制寄存器值 速度较快
			{
				// High 8 bits
				color = 0;
				OV7670_RCK_L;
				color = GPIOD->IDR&0XFF;//读数据
				OV7670_RCK_H; 
				
				// Low 8 bits
				color <<= 8;
				OV7670_RCK_L;
				color|= GPIOD->IDR&0XFF;//读数据
				OV7670_RCK_H; 
				
				screen_buffer[i][j] = color;
			}
		}
		
		for(i = 0; i < 240; i++)
			for(j = 0; j < 320; j++)
				Lcd_WriteData_16Bit(screen_buffer[i][j]);
		
		//ov_frame++;
		//printf("Frame:%d\r\n", ov_frame);
		OV_EXTI_Control(ENABLE);
		__HAL_GPIO_EXTI_CLEAR_IT(OV7670_VSYNC_Pin);
 		ov_sta=0;					//下一次扫描
	} 
}

void ChioceConfirm(void)
{
	// 马达震动表示确认
	HAL_GPIO_WritePin(MOTOR_1_GPIO_Port, MOTOR_1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MOTOR_2_GPIO_Port, MOTOR_2_Pin, GPIO_PIN_SET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(MOTOR_1_GPIO_Port, MOTOR_1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_2_GPIO_Port, MOTOR_2_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);

	HAL_GPIO_WritePin(MOTOR_1_GPIO_Port, MOTOR_1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MOTOR_2_GPIO_Port, MOTOR_2_Pin, GPIO_PIN_SET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(MOTOR_1_GPIO_Port, MOTOR_1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_2_GPIO_Port, MOTOR_2_Pin, GPIO_PIN_RESET);
	// 震动结束，切换画面
	switch (choice)
	{
		case IMG_EAT:
			UI_call_eat();
			break;
		case IMG_WC:
			UI_call_wc();
			break;
		default:
			break;
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
