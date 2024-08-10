#include "SYN6288.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
 
//Music:选择背景音乐。0:无背景音乐，1~15：选择背景音乐
void SYN_FrameInfo(uint8_t Music, uint8_t *HZdata)
{
  /****************需要发送的文本**********************************/
  unsigned  char  Frame_Info[50];
  unsigned  char  HZ_Length;
  unsigned  char  ecc  = 0;  			//定义校验字节
  unsigned  int i = 0;
  HZ_Length = strlen((char*)HZdata); 			//需要发送文本的长度
 
  /*****************帧固定配置信息**************************************/
  Frame_Info[0] = 0xFD ; 			//构造帧头FD
  Frame_Info[1] = 0x00 ; 			//构造数据区长度的高字节
  Frame_Info[2] = HZ_Length + 3; 		//构造数据区长度的低字节
  Frame_Info[3] = 0x01 ; 			//构造命令字：合成播放命令
  Frame_Info[4] = 0x01 | Music << 4 ; //构造命令参数：背景音乐设定
 
  /*******************校验码计算***************************************/
  for(i = 0; i < 5; i++)   				//依次发送构造好的5个帧头字节
  {
    ecc = ecc ^ (Frame_Info[i]);		//对发送的字节进行异或校验
  }
 
  for(i = 0; i < HZ_Length; i++)   		//依次发送待合成的文本数据
  {
    ecc = ecc ^ (HZdata[i]); 				//对发送的字节进行异或校验
  }
  /*******************发送帧信息***************************************/
  memcpy(&Frame_Info[5], HZdata, HZ_Length);
  Frame_Info[5 + HZ_Length] = ecc;
  HAL_UART_Transmit(&huart6,Frame_Info,5 + HZ_Length + 1,10000);
}
 
 
/***********************************************************
* 名    称： YS_SYN_Set(u8 *Info_data)
* 功    能： 主函数	程序入口
* 入口参数： *Info_data:固定的配置信息变量
* 出口参数：
* 说    明：本函数用于配置，停止合成、暂停合成等设置 ，默认波特率9600bps。
* 调用方法：通过调用已经定义的相关数组进行配置。
**********************************************************/
void YS_SYN_Set(uint8_t *Info_data)
{
  uint8_t Com_Len;
  Com_Len = strlen((char*)Info_data);
  HAL_UART_Transmit(&huart6,Info_data,Com_Len,10000);
}

void SYN_Speak(char *text) {
  char str[100];
  // v: 音量，取值范围0~15；m: 背景音乐音量，取值范围0~15；t: 语速，取值范围0~5
  sprintf(str, "[v16][m7][t5]%s", text);
  SYN_FrameInfo(0, (unsigned char* )str);  // 0: 无背景音乐
  for (int i = 0; i < 25; i++) {  // 固定等待时间，确保播放完毕
    HAL_Delay(100);
    if (HAL_GPIO_ReadPin(INFRARED_GPIO_Port, INFRARED_Pin) == 0) {  // 每播放100ms，判断一次红外，遮挡则停止播放，以便丝滑地停止
      break;
    }
  }
}
