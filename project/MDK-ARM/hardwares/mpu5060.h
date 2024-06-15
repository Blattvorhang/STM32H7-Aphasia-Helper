#include "main.h"
#include "i2c.h"
#include <stdio.h>
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

#define ADDRESS_W 0xD0   //Ð´µØÖ· 0x68
#define ADDRESS_R 0xD1   //¶ÁµØÖ·  0x69
#define MPU_PWR_MGMT1_REG		0X6B	//µçÔ´¹ÜÀí¼Ä´æÆ÷1
#define MPU_GYRO_CFG_REG		0X1B	//ÍÓÂÝÒÇÅäÖÃ¼Ä´æÆ÷
#define MPU_ACCEL_CFG_REG		0X1C	//¼ÓËÙ¶È¼ÆÅäÖÃ¼Ä´æÆ÷
#define MPU_SAMPLE_RATE_REG		0X19	//ÍÓÂÝÒÇ²ÉÑùÆµÂÊ·ÖÆµÆ÷
#define MPU_INT_EN_REG			0X38	//ÖÐ¶ÏÊ¹ÄÜ¼Ä´æÆ÷
#define MPU_USER_CTRL_REG		0X6A	//ÓÃ»§¿ØÖÆ¼Ä´æÆ÷
#define MPU_FIFO_EN_REG			0X23	//FIFOÊ¹ÄÜ¼Ä´æÆ÷
#define MPU_INTBP_CFG_REG		0X37	//ÖÐ¶Ï/ÅÔÂ·ÉèÖÃ¼Ä´æÆ÷
#define MPU_DEVICE_ID_REG		0X75	//Æ÷¼þID¼Ä´æÆ÷
#define MPU_PWR_MGMT2_REG		0X6C	//µçÔ´¹ÜÀí¼Ä´æÆ÷2 
#define MPU_CFG_REG				0X1A	//ÅäÖÃ¼Ä´æÆ÷ µÍÍ¨ÂË²¨Æ÷ÅäÖÃ¼Ä´æÆ÷
#define MPU_TEMP_OUTH_REG		0X41	//ÎÂ¶ÈÖµ¸ß°ËÎ»¼Ä´æÆ÷
#define MPU_TEMP_OUTL_REG		0X42	//ÎÂ¶ÈÖµµÍ8Î»¼Ä´æÆ÷

#define MPU_ACCEL_XOUTH_REG		0X3B	//¼ÓËÙ¶ÈÖµ,XÖá¸ß8Î»¼Ä´æÆ÷
#define MPU_ACCEL_XOUTL_REG		0X3C	//¼ÓËÙ¶ÈÖµ,XÖáµÍ8Î»¼Ä´æÆ÷
#define MPU_ACCEL_YOUTH_REG		0X3D	//¼ÓËÙ¶ÈÖµ,YÖá¸ß8Î»¼Ä´æÆ÷
#define MPU_ACCEL_YOUTL_REG		0X3E	//¼ÓËÙ¶ÈÖµ,YÖáµÍ8Î»¼Ä´æÆ÷
#define MPU_ACCEL_ZOUTH_REG		0X3F	//¼ÓËÙ¶ÈÖµ,ZÖá¸ß8Î»¼Ä´æÆ÷
#define MPU_ACCEL_ZOUTL_REG		0X40	//¼ÓËÙ¶ÈÖµ,ZÖáµÍ8Î»¼Ä´æÆ÷

#define MPU_GYRO_XOUTH_REG		0X43	//ÍÓÂÝÒÇÖµ,XÖá¸ß8Î»¼Ä´æÆ÷
#define MPU_GYRO_XOUTL_REG		0X44	//ÍÓÂÝÒÇÖµ,XÖáµÍ8Î»¼Ä´æÆ÷
#define MPU_GYRO_YOUTH_REG		0X45	//ÍÓÂÝÒÇÖµ,YÖá¸ß8Î»¼Ä´æÆ÷
#define MPU_GYRO_YOUTL_REG		0X46	//ÍÓÂÝÒÇÖµ,YÖáµÍ8Î»¼Ä´æÆ÷
#define MPU_GYRO_ZOUTH_REG		0X47	//ÍÓÂÝÒÇÖµ,ZÖá¸ß8Î»¼Ä´æÆ÷
#define MPU_GYRO_ZOUTL_REG		0X48	//ÍÓÂÝÒÇÖµ,ZÖáµÍ8Î»¼Ä´æÆ÷

#define DEFAULT_MPU_HZ  (100)		//¶¨ÒåÊä³öËÙ¶È100Hz
#define q30  1073741824.0f			//q30¸ñÊ½,long×ªfloatÊ±µÄ³ýÊý.

void MPU_6050_Init(void);
unsigned char  MPU_Set_LPF(unsigned short lpf);
//DMP¿âÐèÒªµÄº¯Êý
unsigned char HAL_i2c_write(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const *data);
unsigned char HAL_i2c_read(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const *data);
//¶ÁÈ¡ ÎÂ¶È´«¸ÐÆ÷   ÍÓÂÝÒÇ  ¼ÓËÙ¶ÈÊý¾Ý
void read_all(void);
//ÍÓÂÝÒÇ·½Ïò¿ØÖÆ
unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx);
//·½Ïò×ª»»
unsigned short inv_row_2_scale(const signed char *row);
//MPU6050×Ô²âÊÔ
//·µ»ØÖµ:0,Õý³£
//    ÆäËû,Ê§°Ü
unsigned char run_self_test(void);
//mpu6050,dmp³õÊ¼»¯
//·µ»ØÖµ:0,Õý³£
//    ÆäËû,Ê§°Ü
//    ÆäËû,Ê§°Ü
unsigned char mpu_dmp_get_data(float *pitch,float *roll,float *yaw);
