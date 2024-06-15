#include "sys.h"
#include "sccb.h"
#include "delay.h"

uint32_t tickOFdelay;

void SCCB_SDA_OUT(void)
{
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
	  __HAL_RCC_GPIOE_CLK_ENABLE();
	  GPIO_InitStruct.Pin = SCCB_SDA_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

void SCCB_SDA_IN(void)
{
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
	  __HAL_RCC_GPIOE_CLK_ENABLE();
	  GPIO_InitStruct.Pin = SCCB_SDA_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}		 

//SCCB��ʼ�ź�
//��ʱ��Ϊ�ߵ�ʱ��,�����ߵĸߵ���,ΪSCCB��ʼ�ź�
//�ڼ���״̬��,SDA��SCL��Ϊ�͵�ƽ
void SCCB_Start(void)
{
    SCCB_SDA_H;
    SCCB_SCL_H;

    tickOFdelay = ticknumber;while(tickOFdelay--);
    SCCB_SDA_L;

    tickOFdelay = ticknumber;while(tickOFdelay--);
    SCCB_SCL_L;  
}

//SCCBֹͣ�ź�
//��ʱ��Ϊ�ߵ�ʱ��,�����ߵĵ͵���,ΪSCCBֹͣ�ź�
//����״����,SDA,SCL��Ϊ�ߵ�ƽ
void SCCB_Stop(void)
{
    SCCB_SDA_L;

    tickOFdelay = ticknumber;while(tickOFdelay--);
    SCCB_SCL_H;

    tickOFdelay = ticknumber;while(tickOFdelay--);
    SCCB_SDA_H;

    tickOFdelay = ticknumber;while(tickOFdelay--);
}  
//����NA�ź�
void SCCB_No_Ack(void)
{
	HAL_Delay(1);
	SCCB_SDA_H;
	SCCB_SCL_H;

	tickOFdelay = ticknumber;while(tickOFdelay--);
	SCCB_SCL_L;

	tickOFdelay = ticknumber;while(tickOFdelay--);
	SCCB_SDA_L;

	tickOFdelay = ticknumber;while(tickOFdelay--);
}
//SCCB,д��һ���ֽ�
//����ֵ:0,�ɹ�;1,ʧ��. 
u8 SCCB_WR_Byte(u8 dat)
{
	uint8_t j,res;
	for(j=0;j<8;j++)
	{
		if(dat&0x80)SCCB_SDA_H;
		else SCCB_SDA_L;
		dat<<=1;

		tickOFdelay = ticknumber;while(tickOFdelay--);
		SCCB_SCL_H;

		tickOFdelay = ticknumber;while(tickOFdelay--);
		SCCB_SCL_L;
	}
	SCCB_SDA_IN();

	tickOFdelay = ticknumber;while(tickOFdelay--);
	SCCB_SCL_H;

	tickOFdelay = ticknumber;while(tickOFdelay--);
	if(SCCB_READ_SDA)res=1;
	else res=0;
	SCCB_SCL_L;
	SCCB_SDA_OUT();
	return res;
}	 
//SCCB ��ȡһ���ֽ�
//��SCL��������,��������
//����ֵ:����������
u8 SCCB_RD_Byte(void)
{
	uint8_t temp=0,j;
	SCCB_SDA_IN();
	for(j=8;j>0;j--)
	{

		tickOFdelay = ticknumber;while(tickOFdelay--);
	    SCCB_SCL_H;
		temp=temp<<1;
		if(SCCB_READ_SDA)temp++;

		tickOFdelay = ticknumber;while(tickOFdelay--);
		SCCB_SCL_L;
	}
	SCCB_SDA_OUT();
	return temp;
} 							    
//д�Ĵ���
//����ֵ:0,�ɹ�;1,ʧ��.
u8 SCCB_WR_Reg(u8 reg,u8 data)
{
	uint8_t res=0;
	SCCB_Start();
	if(SCCB_WR_Byte(SCCB_ID_W))res=1;

	tickOFdelay = ticknumber;while(tickOFdelay--);
	if(SCCB_WR_Byte(reg))res=1;

	tickOFdelay = ticknumber;while(tickOFdelay--);
  if(SCCB_WR_Byte(data))res=1;
  SCCB_Stop();
  return	res;
}		  					    
//���Ĵ���
//����ֵ:�����ļĴ���ֵ
u8 SCCB_RD_Reg(u8 reg)
{
	uint8_t val=0;
	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID_W);

	tickOFdelay = ticknumber;while(tickOFdelay--);
  SCCB_WR_Byte(reg);

  tickOFdelay = ticknumber;while(tickOFdelay--);
	SCCB_Stop();

	tickOFdelay = ticknumber;while(tickOFdelay--);

	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID_R);

	tickOFdelay = ticknumber;while(tickOFdelay--);
  val=SCCB_RD_Byte();
  SCCB_No_Ack();
  SCCB_Stop();
  return val;
}















