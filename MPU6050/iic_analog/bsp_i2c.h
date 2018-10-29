#ifndef _BSP_I2C_H
#define _BSP_I2C_H

#include <inttypes.h>

#define I2C_WR	0		/* Ð´¿ØÖÆbit */
#define I2C_RD	1		/* ¶Á¿ØÖÆbit */

void L_i2c_Start(void);
void L_i2c_Stop(void);
void L_i2c_SendByte(uint8_t _ucByte);
uint8_t L_i2c_ReadByte(uint8_t ack);
uint8_t L_i2c_WaitAck(void);
void L_i2c_Ack(void);
void L_i2c_NAck(void);
uint8_t L_i2c_CheckDevice(uint8_t _Address);
void L_i2c_GPIO_Config(void);

void R_i2c_Start(void);
void R_i2c_Stop(void);
void R_i2c_SendByte(uint8_t _ucByte);
uint8_t R_i2c_ReadByte(uint8_t ack);
uint8_t R_i2c_WaitAck(void);
void R_i2c_Ack(void);
void R_i2c_NAck(void);
uint8_t R_i2c_CheckDevice(uint8_t _Address);
void R_i2c_GPIO_Config(void);

#endif
