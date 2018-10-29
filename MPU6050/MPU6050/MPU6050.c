//#include "delay.h"
//#include "myiic.h"
#include "MPU6050.h"
#include "ALL_Includes.h"
#include "bsp_i2c.h"


void Delay_us(u32 dly)
{
	u8 i;
	while(dly--) for(i=0;i<10;i++);
}
//
void Delay_ms(u32 dly)
{
	while(dly--) Delay_us(1000);
}



void MPU6050_Init(void)		
{
  L_i2c_GPIO_Config();   //初始化IIC总线
//	L_Init_MPU_6050();		//初始化MPU6050 
  R_i2c_GPIO_Config();   //初始化IIC总线
//	R_Init_MPU_6050();		//初始化MPU6050 	
	Init_MPU_6050();		//初始化MPU6050 
}

void L_Single_Write(u8 SlaveAddress,u8 REG_Address,u8 REG_Data)
{
	L_i2c_Start();					//开始信号
	L_i2c_SendByte(SlaveAddress);    //发送设备地址+写信号
	L_i2c_WaitAck(); 				//等待应答
	L_i2c_SendByte(REG_Address);		//发送设备寄存器地址
	L_i2c_WaitAck(); 				//等待应答
	L_i2c_SendByte(REG_Data);		//写数据
	L_i2c_WaitAck(); 				//等待应答
	L_i2c_Stop();						//停止信号
//	Delay_ms(5);
	__nop();__nop();__nop();__nop();
}

void R_Single_Write(u8 SlaveAddress,u8 REG_Address,u8 REG_Data)
{
	R_i2c_Start();					//开始信号
	R_i2c_SendByte(SlaveAddress);    //发送设备地址+写信号
	R_i2c_WaitAck(); 				//等待应答
	R_i2c_SendByte(REG_Address);		//发送设备寄存器地址
	R_i2c_WaitAck(); 				//等待应答
	R_i2c_SendByte(REG_Data);		//写数据
	R_i2c_WaitAck(); 				//等待应答
	R_i2c_Stop();						//停止信号
//	Delay_ms(5);
	__nop();__nop();__nop();__nop();
}

u8 L_Single_Read(u8 SlaveAddress,u8 REG_Address)
{
	u8 REG_Data;
	L_i2c_Start();					//开始信号
	L_i2c_SendByte(SlaveAddress);    //发送设备地址+写信号
	L_i2c_WaitAck();  				//等待应答
	L_i2c_SendByte(REG_Address);    //发送设备寄存器地址
	L_i2c_WaitAck();  				//等待应答
	L_i2c_Start();					//再次开始信号
	L_i2c_SendByte(SlaveAddress + 1);    //发送设备地址+读信号
	L_i2c_WaitAck();    			//等待应答
	REG_Data = L_i2c_ReadByte(1);		//获取数据
	L_i2c_NAck();						//不再应答
	L_i2c_Stop();						//停止信号
//	Delay_ms(5);	
  __nop();__nop();__nop();__nop();
	return REG_Data;			
}

u8 R_Single_Read(u8 SlaveAddress,u8 REG_Address)
{
	u8 REG_Data;
	R_i2c_Start();					//开始信号
	R_i2c_SendByte(SlaveAddress);    //发送设备地址+写信号
	R_i2c_WaitAck();  				//等待应答
	R_i2c_SendByte(REG_Address);    //发送设备寄存器地址
	R_i2c_WaitAck();  				//等待应答
	R_i2c_Start();					//再次开始信号
	R_i2c_SendByte(SlaveAddress + 1);    //发送设备地址+读信号
	R_i2c_WaitAck();    			//等待应答
	REG_Data = R_i2c_ReadByte(1);		//获取数据
	R_i2c_NAck();						//不再应答
	R_i2c_Stop();						//停止信号
//	Delay_ms(5);	
  __nop();__nop();__nop();__nop();
	return REG_Data;			
}

//u8 L_MPU6050_Check(void)
//{
//	u8 temp;
//	temp=L_Single_Read(MPU_ADDRESS,WHO_AM_I);			  
//	if(temp==0X68)return 0;		  
//	else	
//	return 1;											 
//}


//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败 
void L_MPU_Set_Gyro_Fsr(u8 fsr)
{
	L_Single_Write(MPU_ADDRESS,MPU_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围  
}
void R_MPU_Set_Gyro_Fsr(u8 fsr)
{
	R_Single_Write(MPU_ADDRESS,MPU_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围  
}

//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g

void L_MPU_Set_Accel_Fsr(u8 fsr)
{
	L_Single_Write(MPU_ADDRESS,MPU_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围  
}
void R_MPU_Set_Accel_Fsr(u8 fsr)
{
	R_Single_Write(MPU_ADDRESS,MPU_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围  
}
//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败 
//side: 1 Left side set ,0 Right side set
u8 MPU_Set_LPF(u16 lpf,u8 side)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	if(side==1)
	{
	  L_Single_Write(MPU_ADDRESS,MPU_CFG_REG,data);//设置数字低通滤波器 
	}
	else
	{
	  R_Single_Write(MPU_ADDRESS,MPU_CFG_REG,data);//设置数字低通滤波器 
	}
	return 0;
}


//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败 
//side: 1 Left side set ,0 Right side set
u8 MPU_Set_Rate(u16 rate,u8 side)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	L_Single_Write(MPU_ADDRESS,MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
 	return MPU_Set_LPF(rate/2,side);	//自动设置LPF为采样率的一半
}

////IIC连续写
////addr:器件地址 
////reg:寄存器地址
////len:写入长度
////buf:数据区
////返回值:0,正常
////    其他,错误代码
//u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
//{
//	u8 i; 
//    i2c_Start(); 
//	  i2c_SendByte((addr<<1)|0);//发送器件地址+写命令	
//	if(i2c_WaitAck())	//等待应答
//	{
//		i2c_Stop();		 
//		return 1;		
//	}
//    i2c_SendByte(reg);	//写寄存器地址
//    i2c_WaitAck();		//等待应答
//	for(i=0;i<len;i++)
//	{
//		i2c_SendByte(buf[i]);	//发送数据
//		if(i2c_WaitAck())		//等待ACK
//		{
//			i2c_Stop();	 
//			return 1;		 
//		}		
//	}    
//    i2c_Stop();	 
//	return 0;	
//} 
//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//    其他,错误代码
u8 L_MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	L_i2c_Start(); 
	L_i2c_SendByte((addr<<1)|0);//发送器件地址+写命令	
	if(L_i2c_WaitAck())	//等待应答
	{
		L_i2c_Stop();		 
		return 1;		
	}
    L_i2c_SendByte(reg);	//写寄存器地址
    L_i2c_WaitAck();		//等待应答
    L_i2c_Start();
	  L_i2c_SendByte((addr<<1)|1);//发送器件地址+读命令	
    L_i2c_WaitAck();		//等待应答 
	while(len)
	{
		if(len==1)*buf=L_i2c_ReadByte(0);//读数据,发送nACK 
		else *buf=L_i2c_ReadByte(1);		//读数据,发送ACK  
		len--;
		buf++; 
	}    
    L_i2c_Stop();	//产生一个停止条件 
	return 0;	
}

u8 R_MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	R_i2c_Start(); 
	R_i2c_SendByte((addr<<1)|0);//发送器件地址+写命令	
	if(R_i2c_WaitAck())	//等待应答
	{
		R_i2c_Stop();		 
		return 1;		
	}
    R_i2c_SendByte(reg);	//写寄存器地址
    R_i2c_WaitAck();		//等待应答
    R_i2c_Start();
	  R_i2c_SendByte((addr<<1)|1);//发送器件地址+读命令	
    R_i2c_WaitAck();		//等待应答 
	while(len)
	{
		if(len==1)*buf=R_i2c_ReadByte(0);//读数据,发送nACK 
		else *buf=R_i2c_ReadByte(1);		//读数据,发送ACK  
		len--;
		buf++; 
	}    
    R_i2c_Stop();	//产生一个停止条件 
	return 0;	
}

////IIC写一个字节 
////reg:寄存器地址
////data:数据
////返回值:0,正常
////    其他,错误代码
//u8 MPU_Write_Byte(u8 reg,u8 data) 				 
//{ 
//  i2c_Start(); 
//	i2c_SendByte(MPU_ADDRESS);//发送器件地址+写命令	
//	if(i2c_WaitAck())	//等待应答
//	{
//		i2c_Stop();		 
//		return 1;		
//	}
//  i2c_SendByte(reg);	//写寄存器地址
//  i2c_WaitAck();		//等待应答 
//	i2c_SendByte(data);//发送数据
//	if(i2c_WaitAck())	//等待ACK
//	{
//		i2c_Stop();	 
//		return 1;		 
//	}		 
//    i2c_Stop();	 
//	return 0;
//}
////IIC读一个字节 
////reg:寄存器地址 
////返回值:读到的数据
//u8 MPU_Read_Byte(u8 reg)
//{
//	u8 res;
//  i2c_Start(); 
//	i2c_SendByte(MPU_ADDRESS);//发送器件地址+写命令	
//	i2c_WaitAck();		//等待应答 
//  i2c_SendByte(reg);	//写寄存器地址
//  i2c_WaitAck();		//等待应答
//  i2c_Start();
//	i2c_SendByte(MPU_ADDRESS+1);//发送器件地址+读命令	
//  i2c_WaitAck();		//等待应答 
//	res=i2c_ReadByte(0);//读取数据,发送nACK 
//  i2c_Stop();			//产生一个停止条件 
//	return res;		
//}

//读MPU605 ID
u8 L_ReadMPUID(void)
{
	u8 res;
	res=L_Single_Read(MPU_ADDRESS,WHO_AM_I);
	return res;
}

u8 R_ReadMPUID(void)
{
	u8 res;
	res=R_Single_Read(MPU_ADDRESS,WHO_AM_I);
	return res;
}

//得到温度值
//返回值:温度值(扩大了100倍)
//side: 1 left side ,0 right side
short MPU_Get_Temperature(u8 side)
{
    u8 buf[2]; 
    short raw;
	float temp;
	if(side==1)
	{
	  L_MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
	}
	else
	{
	  R_MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 		
	}
    raw=((u16)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
    return temp*100;;
}

//short L_MPU_Get_Temperature(void)
//{
//    u8 buf[2]; 
//    short raw;
//	float temp;
//	L_MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
//    raw=((u16)buf[0]<<8)|buf[1];  
//    temp=36.53+((double)raw)/340;  
//    return temp*100;;
//}

//short R_MPU_Get_Temperature(void)
//{
//    u8 buf[2]; 
//    short raw;
//	float temp;
//	R_MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
//    raw=((u16)buf[0]<<8)|buf[1];  
//    temp=36.53+((double)raw)/340;  
//    return temp*100;;
//}

//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
//side: 1 left side, 0 right side
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az,u8 side)
{
    u8 buf[6],res;  
	if(side==1)
	{
	  res=L_MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	}
	else
	{
	  res=R_MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);		
	}
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}

//u8 L_MPU_Get_Accelerometer(short *ax,short *ay,short *az)
//{
//    u8 buf[6],res;  
//	res=L_MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
//	if(res==0)
//	{
//		*ax=((u16)buf[0]<<8)|buf[1];  
//		*ay=((u16)buf[2]<<8)|buf[3];  
//		*az=((u16)buf[4]<<8)|buf[5];
//	} 	
//    return res;;
//}
//u8 R_MPU_Get_Accelerometer(short *ax,short *ay,short *az)
//{
//    u8 buf[6],res;  
//	res=R_MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
//	if(res==0)
//	{
//		*ax=((u16)buf[0]<<8)|buf[1];  
//		*ay=((u16)buf[2]<<8)|buf[3];  
//		*az=((u16)buf[4]<<8)|buf[5];
//	} 	
//    return res;;
//}

//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
//side:1 left side, 0 right side
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz,u8 side)
{
    u8 buf[6],res;  
	if(side==1)
	{
	  res=L_MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	}
	else 
	{
	  res=R_MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);		
	}
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}
//u8 L_MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
//{
//    u8 buf[6],res;  
//	res=L_MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
//	if(res==0)
//	{
//		*gx=((u16)buf[0]<<8)|buf[1];  
//		*gy=((u16)buf[2]<<8)|buf[3];  
//		*gz=((u16)buf[4]<<8)|buf[5];
//	} 	
//    return res;;
//}
//u8 R_MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
//{
//    u8 buf[6],res;  
//	res=R_MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
//	if(res==0)
//	{
//		*gx=((u16)buf[0]<<8)|buf[1];  
//		*gy=((u16)buf[2]<<8)|buf[3];  
//		*gz=((u16)buf[4]<<8)|buf[5];
//	} 	
//    return res;;
//}

void Init_MPU_6050(void)  				//初始化MPU6050
{
	Delay_ms(100);
//	__nop();__nop();__nop();__nop();
	L_Single_Write(MPU_ADDRESS,MPU_PWR_MGMT1_REG,0X80);	//Reset MPU6050
	R_Single_Write(MPU_ADDRESS,MPU_PWR_MGMT1_REG,0X80);	//Reset MPU6050	
	Delay_ms(100);
//	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	L_Single_Write(MPU_ADDRESS,MPU_PWR_MGMT1_REG,0x00);//解除休眠状态
	R_Single_Write(MPU_ADDRESS,MPU_PWR_MGMT1_REG,0x00);//解除休眠状态	
//	Single_Write(MPU_ADDRESS,MPU_SAMPLE_RATE_REG,0x07);//陀螺仪采样率，1KHz
//	Single_Write(MPU_ADDRESS,MPU_CFG_REG,0x03);		
//	Single_Write(MPU_ADDRESS,MPU_GYRO_CFG_REG,0x00);//陀螺仪自检及测量范围，典型值：0x1238(不自检，2000deg/s)
//	Single_Write(MPU_ADDRESS,MPU_ACCEL_CFG_REG,0x00); //配置加速度传感器工作在2G模式，不自检  
	L_MPU_Set_Gyro_Fsr(3);					//陀螺仪传感器,±2000dps
	L_MPU_Set_Accel_Fsr(0);					//加速度传感器,±2g
	MPU_Set_Rate(50,1);						//设置采样率50Hz
	L_Single_Write(MPU_ADDRESS,MPU_INT_EN_REG,0X00);	//关闭所有中断
	L_Single_Write(MPU_ADDRESS,MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
	L_Single_Write(MPU_ADDRESS,MPU_FIFO_EN_REG,0X00);	//关闭FIFO
	L_Single_Write(MPU_ADDRESS,MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
	L_Single_Write(MPU_ADDRESS,MPU_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
	L_Single_Write(MPU_ADDRESS,MPU_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作
	MPU_Set_Rate(100,1);						//设置采样率为50Hz
	
	R_MPU_Set_Gyro_Fsr(3);					//陀螺仪传感器,±2000dps
	R_MPU_Set_Accel_Fsr(0);					//加速度传感器,±2g
	MPU_Set_Rate(50,0);						//设置采样率50Hz
	R_Single_Write(MPU_ADDRESS,MPU_INT_EN_REG,0X00);	//关闭所有中断
	R_Single_Write(MPU_ADDRESS,MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
	R_Single_Write(MPU_ADDRESS,MPU_FIFO_EN_REG,0X00);	//关闭FIFO
	R_Single_Write(MPU_ADDRESS,MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
	R_Single_Write(MPU_ADDRESS,MPU_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
	R_Single_Write(MPU_ADDRESS,MPU_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作
	MPU_Set_Rate(100,0);						//设置采样率为50Hz	
}

//void L_Init_MPU_6050(void)  				//初始化MPU6050
//{
//	Delay_ms(100);
////	__nop();__nop();__nop();__nop();
//	L_Single_Write(MPU_ADDRESS,MPU_PWR_MGMT1_REG,0X80);	//Reset MPU6050
//	Delay_ms(100);
////	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
//	L_Single_Write(MPU_ADDRESS,MPU_PWR_MGMT1_REG,0x00);//解除休眠状态
////	Single_Write(MPU_ADDRESS,MPU_SAMPLE_RATE_REG,0x07);//陀螺仪采样率，1KHz
////	Single_Write(MPU_ADDRESS,MPU_CFG_REG,0x03);		
////	Single_Write(MPU_ADDRESS,MPU_GYRO_CFG_REG,0x00);//陀螺仪自检及测量范围，典型值：0x1238(不自检，2000deg/s)
////	Single_Write(MPU_ADDRESS,MPU_ACCEL_CFG_REG,0x00); //配置加速度传感器工作在2G模式，不自检  
//	L_MPU_Set_Gyro_Fsr(3);					//陀螺仪传感器,±2000dps
//	L_MPU_Set_Accel_Fsr(0);					//加速度传感器,±2g
//	MPU_Set_Rate(50,1);						//设置采样率50Hz
//	L_Single_Write(MPU_ADDRESS,MPU_INT_EN_REG,0X00);	//关闭所有中断
//	L_Single_Write(MPU_ADDRESS,MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
//	L_Single_Write(MPU_ADDRESS,MPU_FIFO_EN_REG,0X00);	//关闭FIFO
//	L_Single_Write(MPU_ADDRESS,MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
//	L_Single_Write(MPU_ADDRESS,MPU_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
//	L_Single_Write(MPU_ADDRESS,MPU_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作
//	MPU_Set_Rate(100,1);						//设置采样率为50Hz
//}

//void R_Init_MPU_6050(void)  				//初始化MPU6050
//{
//	Delay_ms(100);
////	__nop();__nop();__nop();__nop();
//	R_Single_Write(MPU_ADDRESS,MPU_PWR_MGMT1_REG,0X80);	//Reset MPU6050
//	Delay_ms(100);
////	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
//	R_Single_Write(MPU_ADDRESS,MPU_PWR_MGMT1_REG,0x00);//解除休眠状态
////	Single_Write(MPU_ADDRESS,MPU_SAMPLE_RATE_REG,0x07);//陀螺仪采样率，1KHz
////	Single_Write(MPU_ADDRESS,MPU_CFG_REG,0x03);		
////	Single_Write(MPU_ADDRESS,MPU_GYRO_CFG_REG,0x00);//陀螺仪自检及测量范围，典型值：0x1238(不自检，2000deg/s)
////	Single_Write(MPU_ADDRESS,MPU_ACCEL_CFG_REG,0x00); //配置加速度传感器工作在2G模式，不自检  
//	R_MPU_Set_Gyro_Fsr(3);					//陀螺仪传感器,±2000dps
//	R_MPU_Set_Accel_Fsr(0);					//加速度传感器,±2g
//	MPU_Set_Rate(50,0);						//设置采样率50Hz
//	R_Single_Write(MPU_ADDRESS,MPU_INT_EN_REG,0X00);	//关闭所有中断
//	R_Single_Write(MPU_ADDRESS,MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
//	R_Single_Write(MPU_ADDRESS,MPU_FIFO_EN_REG,0X00);	//关闭FIFO
//	R_Single_Write(MPU_ADDRESS,MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
//	R_Single_Write(MPU_ADDRESS,MPU_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
//	R_Single_Write(MPU_ADDRESS,MPU_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作
//	MPU_Set_Rate(100,0);						//设置采样率为50Hz
//}


