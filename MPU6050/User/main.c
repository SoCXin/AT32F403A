/*
**  作者：RandyChan
**  联系：154722299@qq.com
**  修改日期：2016.08.22
**  说明: 模拟I2C接口读取MPU6050加速度原始数据，计算X,Y,Z轴与水平面夹角，
          当任意角度大于45度，LED亮.
					可读取陀螺仪原始数据。
*/

/*

引脚
LED1  PA5
LED2  PA6

左MPU6050 
SDA   PA1
SCL   PA2

右MPU6050 
SDA   PA3
SCL   PA4

*/

#include "ALL_Includes.h"//包含所需的头文件
//#include "anbt_dmp_fun.h"
//#include "anbt_i2c.h"
//#include "anbt_dmp_mpu6050.h"
//#include "anbt_dmp_driver.h"
//#include "mpu6050.h"
#include "USART1.h"
#include "MPU6050.h"
#include "math.h"

//#define q30  1073741824.0f

//void ALL_Config(void);

u8 Readbuf=0;

//u16 TemperH=0;
//u16 TemperL=0;
//u16 Temper=0;
//u16 accelxL=0;   //加速度X轴高位
//u16 accelxH=0;   //加速度X轴低位
//u16 accelyL=0;   //加速度y轴高位
//u16 accelyH=0;   //加速度y轴低位
//u16 accelzL=0;   //加速度z轴高位
//u16 accelzH=0;   //加速度z轴低位
//u16 gyroxL=0;    //陀螺仪X轴高位
//u16 gyroxH=0;    //陀螺仪X轴低位
//u16 gyroyL=0;    //陀螺仪y轴高位
//u16 gyroyH=0;    //陀螺仪y轴低位
//u16 gyrozL=0;    //陀螺仪z轴高位
//u16 gyrozH=0;    //陀螺仪z轴低位
short Laacx,Laacy,Laacz;		//加速度传感器原始数据
short Lgyrox,Lgyroy,Lgyroz;	//陀螺仪原始数据
short LTemper;     //温度原始数据
short LangleX;     //X与自然X轴的角度
short LangleY;      //Y与自然Y轴的角度
short LangleZ;       //Z与自然Z轴的角度
short Raacx,Raacy,Raacz;		//加速度传感器原始数据
short Rgyrox,Rgyroy,Rgyroz;	//陀螺仪原始数据
short RTemper;     //温度原始数据
short RangleX;      //X与自然X轴的角度
short RangleY;       //Y与自然Y轴的角度
short RangleZ;       //Z与自然Z轴的角度

u8 LEDbuf1=0;    //LED1显示标志
u8 LEDbuf2=0;    //LED2显示标志

//float YawR=0.00,RollR=0.00,PitchR=0.00;//欧拉角
//float YawL=0.00,RollL=0.00,PitchL=0.00;//欧拉角
//long quat[4];//四元数存放数组


//得到角度
//x,y,z:x,y,z方向的重力加速度分量(不需要单位,直接数值即可)
//dir:要获得的角度.0,与Z轴的角度;1,与X轴的角度;2,与Y轴的角度.
//返回值:角度值.单位0.1°.
short MPU_Get_Angle(float x,float y,float z,u8 dir)
{
	float temp;
 	float res=0;
	switch(dir)
	{
		case 0://与自然Z轴的角度
 			temp=sqrt((x*x+y*y))/z;
 			res=atan(temp);
 			break;
		case 1://与自然X轴的角度
 			temp=x/sqrt((y*y+z*z));
 			res=atan(temp);
 			break;
 		case 2://与自然Y轴的角度
 			temp=y/sqrt((x*x+z*z));
 			res=atan(temp);
 			break;
 	}
	return res*573.25;
}




int main(void)
{	

	//    Delay_Init(48);
		LED_Init();          //LED引脚初始化
//		i2c_GPIO_Config();
	  USART1_Init(115200);	 //串口初始化
		MPU6050_Init();		//IIC初始化，MPU6050初始化,包括IO口初始化
	
	
	  LEDbuf1=1;
	  LEDbuf2=0;
	
//  Readbuf=L_ReadMPUID();
	while(1)
	{
//			LED_ON();
			Delay_ms(10);
//			LED_OFF();
//			delay_ms(500);

			if(LEDbuf1==1)
			{
				LED_ON(1);
			}
			else
			{
				LED_OFF(1);
			}

			if(LEDbuf2==1)
			{
				LED_ON(2);
			}
			else
			{
				LED_OFF(2);
			}
		

//			 TemperH=(u16)Single_Read(MPU_ADDRESS,MPU_TEMP_OUTH_REG);
//			 TemperL=Single_Read(MPU_ADDRESS,MPU_TEMP_OUTL_REG);
//			 Temper=((TemperH<<8)|((u16)TemperL));
//			LTemper=MPU_Get_Temperature(1);   //左温度数据
//			RTemper=MPU_Get_Temperature(0);    //右温度数据


			MPU_Get_Accelerometer(&Laacx,&Laacy,&Laacz,1);	//得到左加速度传感器数据
			MPU_Get_Gyroscope(&Lgyrox,&Lgyroy,&Lgyroz,1);	//得到左陀螺仪数据
      MPU_Get_Accelerometer(&Raacx,&Raacy,&Raacz,0);	//得到右加速度传感器数据
			MPU_Get_Gyroscope(&Rgyrox,&Rgyroy,&Rgyroz,1);	//得到左陀螺仪数据
			
			LangleZ=MPU_Get_Angle(Laacx,Laacy,Laacz,0);
			LangleX=MPU_Get_Angle(Laacx,Laacy,Laacz,1);
			LangleY=MPU_Get_Angle(Laacx,Laacy,Laacz,2);

			RangleZ=MPU_Get_Angle(Raacx,Raacy,Raacz,0);
			RangleX=MPU_Get_Angle(Raacx,Raacy,Raacz,1);
			RangleY=MPU_Get_Angle(Raacx,Raacy,Raacz,2);
			
			if((LangleX>450)|(LangleY>450)|(LangleZ>450))
			{
				LEDbuf1=1;
			}
			else
			{
				LEDbuf1=0;
			}
			if((RangleX>450)|(RangleY>450)|(RangleZ>450))
			{
				LEDbuf2=1;
			}
			else
			{
				LEDbuf2=0;
			}
			
			//加速度X轴数据
//			 accelxH=Single_Read(MPU_ADDRESS,MPU_ACCEL_XOUTH_REG);
//			 accelxL=Single_Read(MPU_ADDRESS,MPU_ACCEL_XOUTL_REG);
//			 accelx=((accelxH<<8)|accelxL);      
//			accelx=GetData(MPU_ACCEL_XOUTH_REG);
			//加速度Y轴数据
//			 accelyH=Single_Read(MPU_ADDRESS,MPU_ACCEL_YOUTH_REG);
//			 accelyL=Single_Read(MPU_ADDRESS,MPU_ACCEL_YOUTL_REG);
//			 accely=((accelyH<<8)|accelyL);
//			accely=GetData(MPU_ACCEL_YOUTH_REG);
			//加速度Z轴数据
//			 accelzH=Single_Read(MPU_ADDRESS,MPU_ACCEL_ZOUTH_REG);
//			 accelzL=Single_Read(MPU_ADDRESS,MPU_ACCEL_ZOUTL_REG);
//			 accelz=((accelzH<<8)|accelzL);			
//			accelz=GetData(MPU_ACCEL_ZOUTH_REG);
			
			 //陀螺仪X轴数据
//			 gyroxH=Single_Read(MPU_ADDRESS,MPU_GYRO_XOUTH_REG);
//			 gyroxL=Single_Read(MPU_ADDRESS,MPU_GYRO_XOUTL_REG);
//			 gyrox=((gyroxH<<8)|gyroxL);
//			 gyrox=GetData(MPU_GYRO_XOUTH_REG);
			 //陀螺仪Y轴数据
//			 gyroyH=Single_Read(MPU_ADDRESS,MPU_GYRO_YOUTH_REG);
//			 gyroyL=Single_Read(MPU_ADDRESS,MPU_GYRO_YOUTL_REG);
//			 gyroy=((gyroyH<<8)|gyroyL);
//			 gyroy=GetData(MPU_GYRO_YOUTH_REG);
       //陀螺仪Z轴数据
//			 gyrozH=Single_Read(MPU_ADDRESS,MPU_GYRO_ZOUTH_REG);
//			 gyrozL=Single_Read(MPU_ADDRESS,MPU_GYRO_ZOUTL_REG);
//			 gyroz=((gyrozH<<8)|gyrozL);
//       gyroz=GetData(MPU_GYRO_ZOUTH_REG);
			 
			 
//					dmp_read_fifoR(gyro, accel, quat, &sensor_timestamp, &sensors,&more);				
//					if(sensors&INV_WXYZ_QUAT)
//					 {					 
//						 q0=quat[0] / q30;
//						 q1=quat[1] / q30;
//						 q2=quat[2] / q30;
//						 q3=quat[3] / q30;
//						 PitchR = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
//						 RollR = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
//						 YawR = 	atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;
//					 }		
				 
//					dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more);				
//					if(sensors&INV_WXYZ_QUAT)
//					 { 
//						 q0=quat[0] / q30;
//						 q1=quat[1] / q30;
//						 q2=quat[2] / q30;
//						 q3=quat[3] / q30;
//						 PitchL = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
//						 RollL = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
//						 YawL = 	atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;
//					 }		

	}
}




//#ifdef  USE_FULL_ASSERT

//void assert_failed(uint8_t* file, uint32_t line)
//{

//  while (1)
//  {
//  }
//}
//#endif



