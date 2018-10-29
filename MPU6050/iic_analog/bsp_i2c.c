

/*
	应用说明：
	在访问I2C设备前，请先调用 i2c_CheckDevice() 检测I2C设备是否正常，该函数会配置GPIO
*/

//#include "stm32f10x.h"
#include "ALL_Includes.h"
#include "bsp_i2c.h"

/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define GPIO_PORT_I2C	GPIOA			/* GPIO端口 */
#define RCC_I2C_PORT 	RCC_AHBPeriph_GPIOA		/* GPIO端口时钟 */
#define L_I2C_SCL_PIN		GPIO_Pin_2			/* 连接到SCL时钟线的GPIO */
#define L_I2C_SDA_PIN		GPIO_Pin_1			/* 连接到SDA数据线的GPIO */

/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */

	#define L_I2C_SCL_1()  GPIO_PORT_I2C->BSRR = L_I2C_SCL_PIN				/* SCL = 1 */
	#define L_I2C_SCL_0()  GPIO_PORT_I2C->BRR = L_I2C_SCL_PIN				/* SCL = 0 */
	
	#define L_I2C_SDA_1()  GPIO_PORT_I2C->BSRR = L_I2C_SDA_PIN				/* SDA = 1 */
	#define L_I2C_SDA_0()  GPIO_PORT_I2C->BRR = L_I2C_SDA_PIN				/* SDA = 0 */
	
	#define L_I2C_SDA_READ()  ((GPIO_PORT_I2C->IDR & L_I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */
//#endif


/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
//#define R_GPIO_PORT_I2C	GPIOA			/* GPIO端口 */
//#define R_RCC_I2C_PORT 	RCC_AHBPeriph_GPIOA		/* GPIO端口时钟 */
#define R_I2C_SCL_PIN		GPIO_Pin_4			/* 连接到SCL时钟线的GPIO */
#define R_I2C_SDA_PIN		GPIO_Pin_3			/* 连接到SDA数据线的GPIO */

	#define R_I2C_SCL_1()  GPIO_PORT_I2C->BSRR = R_I2C_SCL_PIN				/* SCL = 1 */
	#define R_I2C_SCL_0()  GPIO_PORT_I2C->BRR = R_I2C_SCL_PIN				/* SCL = 0 */
	
	#define R_I2C_SDA_1()  GPIO_PORT_I2C->BSRR = R_I2C_SDA_PIN				/* SDA = 1 */
	#define R_I2C_SDA_0()  GPIO_PORT_I2C->BRR = R_I2C_SDA_PIN				/* SDA = 0 */
	
	#define R_I2C_SDA_READ()  ((GPIO_PORT_I2C->IDR & R_I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */
//#endif

void L_i2c_GPIO_Config(void);
void R_i2c_GPIO_Config(void);
/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
//static void i2c_Delay(void)
//{
//	uint8_t i;

//	/*　
//	 	下面的时间是通过安富莱AX-Pro逻辑分析仪测试得到的。
//		CPU主频72MHz时，在内部Flash运行, MDK工程不优化
//		循环次数为10时，SCL频率 = 205KHz 
//		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
//	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
//        
//    IAR工程编译效率高，不能设置为7
//	*/
//	for (i = 0; i < 10; i++);
//}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void L_i2c_Start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	L_I2C_SDA_1();
	L_I2C_SCL_1();
//	i2c_Delay();
	__nop();
	L_I2C_SDA_0();
//	i2c_Delay();
	__nop();
	L_I2C_SCL_0();
//	i2c_Delay();
	__nop();
}
void R_i2c_Start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	R_I2C_SDA_1();
	R_I2C_SCL_1();
//	i2c_Delay();
	__nop();
	R_I2C_SDA_0();
//	i2c_Delay();
	__nop();
	R_I2C_SCL_0();
//	i2c_Delay();
	__nop();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线停止信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void L_i2c_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	L_I2C_SDA_0();
	L_I2C_SCL_1();
//	i2c_Delay();
	__nop();
	L_I2C_SDA_1();
}
void R_i2c_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	R_I2C_SDA_0();
	R_I2C_SCL_1();
//	i2c_Delay();
	__nop();
	R_I2C_SDA_1();
}
/*
*********************************************************************************************************
*	函 数 名: i2c_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参：_ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void L_i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			L_I2C_SDA_1();
		}
		else
		{
			L_I2C_SDA_0();
		}
//		i2c_Delay();
		__nop();
		L_I2C_SCL_1();
//		i2c_Delay();
		__nop(); 
		L_I2C_SCL_0();
		if (i == 7)
		{
			 L_I2C_SDA_1(); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
//		i2c_Delay();
		__nop();
	}
}
void R_i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			R_I2C_SDA_1();
		}
		else
		{
			R_I2C_SDA_0();
		}
//		i2c_Delay();
		__nop();
		R_I2C_SCL_1();
//		i2c_Delay();
		__nop(); 
		R_I2C_SCL_0();
		if (i == 7)
		{
			 R_I2C_SDA_1(); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
//		i2c_Delay();
		__nop();
	}
}

/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参：无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t L_i2c_ReadByte(u8 ack)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		L_I2C_SCL_1();
//		i2c_Delay();
		__nop();
		if (L_I2C_SDA_READ())
		{
			value++;
		}
		L_I2C_SCL_0();
//		i2c_Delay();
		__nop();
	}
	if(ack==0)
		L_i2c_NAck();
	else
		L_i2c_Ack();
	return value;
}
uint8_t R_i2c_ReadByte(u8 ack)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		R_I2C_SCL_1();
//		i2c_Delay();
		__nop();
		if (R_I2C_SDA_READ())
		{
			value++;
		}
		R_I2C_SCL_0();
//		i2c_Delay();
		__nop();
	}
	if(ack==0)
		R_i2c_NAck();
	else
		R_i2c_Ack();
	return value;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t L_i2c_WaitAck(void)
{
	uint8_t re;

	L_I2C_SDA_1();	/* CPU释放SDA总线 */
//	i2c_Delay();
	__nop();
	L_I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
//	i2c_Delay();
	__nop();
	if (L_I2C_SDA_READ())	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	L_I2C_SCL_0();
//	i2c_Delay();
	__nop();
	return re;
}
uint8_t R_i2c_WaitAck(void)
{
	uint8_t re;

	R_I2C_SDA_1();	/* CPU释放SDA总线 */
//	i2c_Delay();
	__nop();
	R_I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
//	i2c_Delay();
	__nop();
	if (R_I2C_SDA_READ())	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	R_I2C_SCL_0();
//	i2c_Delay();
	__nop();
	return re;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void L_i2c_Ack(void)
{
	L_I2C_SDA_0();	/* CPU驱动SDA = 0 */
//	i2c_Delay();
	__nop();__nop();
	L_I2C_SCL_1();	/* CPU产生1个时钟 */
//	i2c_Delay();
	__nop();__nop();
	L_I2C_SCL_0();
//	i2c_Delay();
	__nop();__nop();
	L_I2C_SDA_1();	/* CPU释放SDA总线 */
}
void R_i2c_Ack(void)
{
	R_I2C_SDA_0();	/* CPU驱动SDA = 0 */
//	i2c_Delay();
	__nop();__nop();
	R_I2C_SCL_1();	/* CPU产生1个时钟 */
//	i2c_Delay();
	__nop();__nop();
	R_I2C_SCL_0();
//	i2c_Delay();
	__nop();__nop();
	R_I2C_SDA_1();	/* CPU释放SDA总线 */
}
/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void L_i2c_NAck(void)
{
	L_I2C_SDA_1();	/* CPU驱动SDA = 1 */
//	i2c_Delay();
	__nop();__nop();
	L_I2C_SCL_1();	/* CPU产生1个时钟 */
//	i2c_Delay();
	__nop();__nop();
	L_I2C_SCL_0();
//	i2c_Delay();	
	__nop();__nop();
}
void R_i2c_NAck(void)
{
	R_I2C_SDA_1();	/* CPU驱动SDA = 1 */
//	i2c_Delay();
	__nop();__nop();
	R_I2C_SCL_1();	/* CPU产生1个时钟 */
//	i2c_Delay();
	__nop();__nop();
	R_I2C_SCL_0();
//	i2c_Delay();	
	__nop();__nop();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_GPIO_Config
*	功能说明: 配置I2C总线的GPIO，采用模拟IO的方式实现
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void L_i2c_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

//	RCC_APB2PeriphClockCmd(RCC_I2C_PORT, ENABLE);	/* 打开GPIO时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA , ENABLE);

	GPIO_InitStructure.GPIO_Pin = L_I2C_SCL_PIN | L_I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  	/* 开漏输出 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;	
	GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStructure);

	/* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
	L_i2c_Stop();
}

void R_i2c_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

//	RCC_APB2PeriphClockCmd(RCC_I2C_PORT, ENABLE);	/* 打开GPIO时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA , ENABLE);

	GPIO_InitStructure.GPIO_Pin = R_I2C_SCL_PIN | R_I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  	/* 开漏输出 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;	
	GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStructure);

	/* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
	R_i2c_Stop();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参：_Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/
uint8_t L_i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	L_i2c_GPIO_Config();		/* 配置GPIO */
	
	L_i2c_Start();		/* 发送启动信号 */

	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	L_i2c_SendByte(_Address|I2C_WR);
	ucAck = L_i2c_WaitAck();	/* 检测设备的ACK应答 */

	L_i2c_Stop();			/* 发送停止信号 */

	return ucAck;
}

uint8_t R_i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	R_i2c_GPIO_Config();		/* 配置GPIO */
	
	R_i2c_Start();		/* 发送启动信号 */

	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	R_i2c_SendByte(_Address|I2C_WR);
	ucAck = R_i2c_WaitAck();	/* 检测设备的ACK应答 */

	R_i2c_Stop();			/* 发送停止信号 */

	return ucAck;
}



