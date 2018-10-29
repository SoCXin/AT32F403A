//IIC 起始信号 
void IIC_Start(void) 
{ 
  SDA_OUT();						//sda 线输出，作为输出 
  SDA=1;             
  delay_us(4); 
  SCL=1; 
  delay_us(4); 
  SDA=0;								//拉低总线   
  delay_us(4); 
  SCL=0;								//钳住 I2C 总线，准备发送或接收数据 
  delay_us(4);  
  }       
//产生 IIC 停止信号 
昨天花了一个小时写的mpu6050 I/0模拟 i2c的底层驱动你可以看看。我已经成功读出mpu6050。因为我当初上当买了神州王的板子。主板硬件定义和原子的不一样，注意自己修改。

#define	MPU6050_Addr   0xD0	 //定义器件在IIC总线中的从地址,根据ALT ADDRESS地址引脚不同修改 //这里注意了！！！i2c器件地址是7bit 不是8bit，而模拟i2c的用7bit传送因此是0xd0 器件A0引脚是接地。但是对于who am I 寄存器读出后是0x68. bin编码方向而已。
//****************************************
// 定义MPU6050内部地址
//****************************************
#define SMPLRT_DIV      0x19    //陀螺仪采样率，典型值：0x07(125Hz)
#define CONFIG          0x1A    //低通滤波频率，典型值：0x06(5Hz)
#define GYRO_CONFIG     0x1B    //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define ACCEL_CONFIG    0x1C    //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define ACCEL_XOUT_H    0x3B
#define ACCEL_XOUT_L    0x3C
#define ACCEL_YOUT_H    0x3D
#define ACCEL_YOUT_L    0x3E
#define ACCEL_ZOUT_H    0x3F
#define ACCEL_ZOUT_L    0x40
#define TEMP_OUT_H      0x41
#define TEMP_OUT_L      0x42
#define GYRO_XOUT_H     0x43
#define GYRO_XOUT_L     0x44    
#define GYRO_YOUT_H     0x45
#define GYRO_YOUT_L     0x46
#define GYRO_ZOUT_H     0x47
#define GYRO_ZOUT_L     0x48
#define PWR_MGMT_1      0x6B    //电源管理，典型值：0x00(正常启用)
#define WHO_AM_I        0x75    //IIC地址寄存器(默认数值0x68，只读)

void MPU6050_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/*******************************************************************************
* Function Name  : I2C_Start 
* Description    : Master Start Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : Wheather	Start
****************************************************************************** */
bool MPU6050_I2C_Start(void)
{
	MPU6050_SDA_H;
	MPU6050_SCL_H;
	delay_us(4);
	if(!MPU6050_SDA_read)return FALSE;	//SDA线为低电平则总线忙,退出
	MPU6050_SDA_L;
	delay_us(4);
	if(MPU6050_SDA_read) return FALSE;	//SDA线为高电平则总线出错,退出
	MPU6050_SDA_L;
	delay_us(4);
	return TRUE;
}
/*******************************************************************************
* Function Name  : I2C_Stop
* Description    : Master Stop Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void MPU6050_I2C_Stop(void)
{
	MPU6050_SCL_L;
	delay_us(4);
	MPU6050_SDA_L;
	delay_us(4);
	MPU6050_SCL_H;
	delay_us(4);
	MPU6050_SDA_H;
	delay_us(4);
} 
/*******************************************************************************
* Function Name  : I2C_Ack
* Description    : Master Send Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void MPU6050_I2C_Ack(void)
{	
	MPU6050_SCL_L;
	delay_us(1);
	MPU6050_SDA_L;
	delay_us(1);
	MPU6050_SCL_H;
	delay_us(1);
	MPU6050_SCL_L;
	delay_us(1);
}   
/*******************************************************************************
* Function Name  : I2C_NoAck
* Description    : Master Send No Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void MPU6050_I2C_NoAck(void)
{	
	MPU6050_SCL_L;
	delay_us(1);
	MPU6050_SDA_H;
	delay_us(1);
	MPU6050_SCL_H;
	delay_us(1);
	MPU6050_SCL_L;
	delay_us(1);
} 
/*******************************************************************************
* Function Name  : I2C_WaitAck
* Description    : Master Reserive Slave Acknowledge Single
* Input          : None
* Output         : None
* Return         : Wheather	Reserive Slave Acknowledge Single
****************************************************************************** */
bool MPU6050_I2C_WaitAck(void) 	//返回为:=1有ACK,=0无ACK
{
	MPU6050_SCL_L;
	delay_us(1);
	MPU6050_SDA_H;			
	delay_us(1);
	MPU6050_SCL_H;
	delay_us(1);
	if(MPU6050_SDA_read)
	{
      MPU6050_SCL_L;
	 delay_us(1);
      return FALSE;
	}
	MPU6050_SCL_L;
	delay_us(1);
	return TRUE;
}
/*******************************************************************************
* Function Name  : I2C_SendByte
* Description    : Master Send a Byte to Slave
* Input          : Will Send Date
* Output         : None
* Return         : None
****************************************************************************** */
void MPU6050_I2C_SendByte(u8 SendByte) //数据从高位到低位//
{
    u8 i=8;
    while(i--)
    {
        MPU6050_SCL_L;
        delay_us(1);
      if(SendByte&0x80)
        MPU6050_SDA_H;  
      else 
        MPU6050_SDA_L;   
        SendByte<<=1;
        delay_us(1);
		MPU6050_SCL_H;
        delay_us(1);
    }
    MPU6050_SCL_L;
}  
/*******************************************************************************
* Function Name  : I2C_RadeByte
* Description    : Master Reserive a Byte From Slave
* Input          : None
* Output         : None
* Return         : Date From Slave 
****************************************************************************** */
unsigned char MPU6050_I2C_RadeByte(void)  //数据从高位到低位//
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    MPU6050_SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;      
      MPU6050_SCL_L;
      delay_us(1);
	 MPU6050_SCL_H;
      delay_us(1);	
      if(MPU6050_SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    MPU6050_SCL_L;
    return ReceiveByte;
}

//mpu6050 api
//初始化MPU6050，根据需要请参考pdf进行修改************************
void Init_MPU6050(void)
{
   Single_Write(MPU6050_Addr,PWR_MGMT_1, 0x00);   //
   Single_Write(MPU6050_Addr,SMPLRT_DIV, 0x07);    //
   Single_Write(MPU6050_Addr,CONFIG,0x03);			//输出采样率=1khz/1+7（smpl——div）=125hz
   Single_Write(MPU6050_Addr,GYRO_CONFIG, 0x00);    //±250°
   Single_Write(MPU6050_Addr,ACCEL_CONFIG, 0x00 );  //
  
}
//单字节写入*******************************************

bool Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)		    //void
{
  	if(!MPU6050_I2C_Start())return FALSE;
    MPU6050_I2C_SendByte(SlaveAddress);   //发送设备地址+写信号//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//设置高起始地址+器件地址 
    if(!MPU6050_I2C_WaitAck()){MPU6050_I2C_Stop(); return FALSE;}
    MPU6050_I2C_SendByte(REG_Address );   //设置低起始地址      
    MPU6050_I2C_WaitAck();	
    MPU6050_I2C_SendByte(REG_data);
    MPU6050_I2C_WaitAck();   
    MPU6050_I2C_Stop(); 
    delay_ms(5);
    return TRUE;
}

//单字节读取*****************************************
unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address)
{   unsigned char REG_data;     	
	if(!MPU6050_I2C_Start())return FALSE;
    MPU6050_I2C_SendByte(SlaveAddress); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//设置高起始地址+器件地址 
    if(!MPU6050_I2C_WaitAck()){MPU6050_I2C_Stop(); return FALSE;}
    MPU6050_I2C_SendByte((u8) REG_Address);   //设置低起始地址      
    MPU6050_I2C_WaitAck();
    MPU6050_I2C_Start();
    MPU6050_I2C_SendByte(SlaveAddress+1);
    MPU6050_I2C_WaitAck();

	REG_data= MPU6050_I2C_RadeByte();
    MPU6050_I2C_NoAck();
    MPU6050_I2C_Stop();
    //return TRUE;
	return REG_data;

}//检查MPU6050是否正常
//对于6050检查who am I 寄存器 
//返回1:检测失败
//返回0:检测成功
u8 MPU6050_Check(void)
{
	u8 temp;
	temp=Single_Read(MPU6050_Addr,WHO_AM_I);			  
	if(temp==0X68)return 0;		  
	else	
	return 1;											 
}


