/**
 ******************************************************************************
 * @file    lcm32f039_I2cEmulation.c
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    2021-05-21
 * @brief   This file provides all the i2ce emulation firmware functions.
 ******************************************************************************/
/**
 ******************************************************************************
 * @attention:
 *
 *
 *
 *
 ******************************************************************************/
#include "LCM32F039.h"
#include "LCM32F039_gpio.h"
#include "lcm32f039_i2c.h"

#define AT24C01 127
#define AT24C02 255
#define AT24C04 511
#define AT24C08 1023
#define AT24C16 2047
#define AT24C32 4095
#define AT24C64 8191
#define AT24C128 16383
#define AT24C256 32767

#define IIC_SCL_H GPIO_SetBits(GPIOA, GPIO_Pin_9)
#define IIC_SCL_L GPIO_ResetBits(GPIOA, GPIO_Pin_9)
#define IIC_SDA_H GPIO_SetBits(GPIOA, GPIO_Pin_10)
#define IIC_SDA_L GPIO_ResetBits(GPIOA, GPIO_Pin_10)
#define READ_SDA GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10)
#define SDA_IN()   \
	{    \
		GPIOA->MODER &= 0xFFCFFFFF; \
	}
#define SDA_OUT()  \
	{    \
		GPIOA->MODER |= 0x00100000; \
	}
void delay_us(int i)
{
	while (i--)
		;
}
/**************
when CLK is high,DATA change form high to low
**************************************************/
void IIC_Start(void)
{
	SDA_OUT(); // sda����� /Line output
	IIC_SCL_H;
	IIC_SDA_H;
	delay_us(5);
	IIC_SDA_L;
	delay_us(5);
}
/**************
when CLK is high DATA change form low to high
**************************************************/
void IIC_Stop(void)
{
	SDA_OUT(); // sda����� /Line output
	IIC_SDA_L;
	IIC_SCL_L;
	delay_us(5);
	IIC_SCL_H;
	IIC_SDA_H; //����I2C���߽����ź� /Send the I2C bus end signal
	delay_us(5);
}
/**************
�ȴ�Ӧ���źŵ���
����ֵ��1:����Ӧ��ʧ��;   0:����Ӧ��ɹ�
**************************************************/

/**************
Wait for the response signal to arrive
returned value��1:Failure to receive a reply;   0:Response received successfully
**************************************************/

uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime = 0;
	SDA_IN(); // SDA����Ϊ���� /Set to input
	IIC_SDA_H;
	delay_us(5);
	IIC_SCL_H;
	delay_us(5);
	while (READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_L; //ʱ�����0 /Clock output 0
	return 0;
}
/**************
����ACKӦ�� /Generating an ACK response
**************************************************/
void IIC_Ack(void)
{
	IIC_SCL_L;
	SDA_OUT();
	IIC_SDA_L;
	delay_us(5);
	IIC_SCL_H;
	delay_us(5);
	IIC_SCL_L;
}
/**************
������ACKӦ�� /No ACK response is generated
**************************************************/
void IIC_NAck(void)
{
	IIC_SCL_L;
	SDA_OUT();
	IIC_SDA_H;
	delay_us(5);
	IIC_SCL_H;
	delay_us(5);
	IIC_SCL_L;
}

void IIC_Send_Byte(uint8_t txd)
{
	uint8_t t = 0;
	SDA_OUT();
	IIC_SCL_L; //����ʱ�ӿ�ʼ���ݴ��� /Lower the clock to start data transmission
	for (t = 0; t < 8; t++)
	{
		if (txd & 0x80)
			IIC_SDA_H;
		else
			IIC_SDA_L;
		txd <<= 1;
		delay_us(5);
		IIC_SCL_H;
		delay_us(5);
		IIC_SCL_L;
		delay_us(5);
	}
}
/********************
��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK  /Read 1 byte, ACK =1, send ACK, ack=0, send nACK
*******************************************/
uint8_t IIC_Read_Byte(uint8_t ack)
{
	uint8_t receive = 0;
	uint8_t i = 0;
	SDA_IN(); // SDA����Ϊ����/Set to input
	for (i = 0; i < 8; i++)
	{
		IIC_SCL_L;
		delay_us(5);
		IIC_SCL_H;
		receive <<= 1;
		if (READ_SDA)
			receive++;
		delay_us(5);
	}
	if (!ack)
		IIC_NAck(); //����nACK /Send nACK
	else
		IIC_Ack(); //����ACK /Send ACK
	return receive;
}
void IIC_Init(void)
{
	IIC_SCL_H;
	IIC_SDA_H;
	delay_us(5000);
}
/***************
��AT24CXXָ����ַ����һ������
 ReadAddr:��ʼ�����ĵ�ַ
����ֵ  :����������
**************************************/
/***************
Read a piece of data at AT24CXX at the address specified
 ReadAddr:Address to start reading
return  :The data I read
**************************************/
uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr)
{
	uint8_t temp = 0;
	IIC_Start();
	if (AT24C02 > AT24C16)
	{
		IIC_Send_Byte(0XA0); //����д����/Sending Write Commands
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr >> 8); //���͸ߵ�ַ/Send high address
		IIC_Wait_Ack();
	}
	else
		IIC_Send_Byte(0XA0 + ((ReadAddr / 256) << 1)); //����������ַ0XA0,д����/Sending device address 0XA0 writes data

	IIC_Wait_Ack();
	IIC_Send_Byte(ReadAddr % 256); //���͵͵�ַ /Send low address
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(0XA1); //�������ģʽ /Enter receive mode
	IIC_Wait_Ack();
	temp = IIC_Read_Byte(0);
	IIC_Stop(); //����һ��ֹͣ���� /Produces a stop condition
	return temp;
}
/****************************
��AT24CXXָ����ַд��һ������
WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ
DataToWrite:Ҫд�������
****************************************/
/****************************
Writes a piece of data at AT24CXX at the address specified
WriteAddr  :Destination address of data to be written
DataToWrite:The data to be written
****************************************/
void AT24CXX_WriteOneByte(uint16_t WriteAddr, uint8_t DataToWrite)
{
	IIC_Start();
	if (AT24C02 > AT24C16)
	{
		IIC_Send_Byte(0XA0); //����д����/Sending Write Commands
		IIC_Wait_Ack();
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr >> 8); //���͸ߵ�ַ/Send high address
	}
	else
	{
		IIC_Send_Byte(0XA0 + ((WriteAddr / 256) << 1)); //����������ַ0XA0,д����/Send device address 0XA0 and write data
	}
	IIC_Wait_Ack();
	IIC_Send_Byte(WriteAddr % 256); //���͵͵�ַ /Send low address
	IIC_Wait_Ack();
	IIC_Send_Byte(DataToWrite); //�����ֽ� /Send byte
	IIC_Wait_Ack();
	IIC_Stop(); //����һ��ֹͣ���� /Produces a stop condition
	delay_us(1000);
}
/************************************************
��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
�ú�������д��16bit����32bit������.
WriteAddr  :��ʼд��ĵ�ַ
DataToWrite:���������׵�ַ
Len   :Ҫд�����ݵĳ���2,4
***********************************************/
/************************************************
The specified address in AT24CXX starts writing data of length Len
This function is used to write 16bit or 32bit data.
WriteAddr  :Address to start writing
DataToWrite:Data array starting address
Len   :The length of data to be written is 2,4
***********************************************/
void AT24CXX_WriteLenByte(uint16_t WriteAddr, uint32_t DataToWrite, uint8_t Len)
{
	uint8_t t;
	for (t = 0; t < Len; t++)
	{
		AT24CXX_WriteOneByte(WriteAddr + t, (DataToWrite >> (8 * t)) & 0xff);
	}
}
/****************************************************
��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
�ú������ڶ���16bit����32bit������.
ReadAddr   :��ʼ�����ĵ�ַ
����ֵ:����
Len   :Ҫ�������ݵĳ���2,4
*************************************/
/****************************************************
The specified address in AT24CXX starts reading data of length Len
This function is used to read 16bit or 32bit data.
ReadAddr   :Address to start reading
return:data
Len   :The length of the data to be read is 2,4
*************************************/
uint32_t AT24CXX_ReadLenByte(uint16_t ReadAddr, uint8_t Len)
{
	uint8_t t;
	uint32_t temp = 0;
	for (t = 0; t < Len; t++)
	{
		temp <<= 8;
		temp += AT24CXX_ReadOneByte(ReadAddr + Len - t - 1);
	}
	return temp;
}
/***************************************
���AT24CXX�Ƿ�����
��������24XX�����һ����ַ(255)���洢��־��.
���������24Cϵ��,�����ַҪ�޸�
����1:���ʧ��
����0:���ɹ�
***********************************/
/***************************************
Check whether the AT24CXX is normal
The last address of 24XX (255) is used here to store the token word..
If using other 24C series, this address should be changed
Return1:detecting failed
return0:detecting successful
***********************************/
uint8_t AT24CXX_Check(void)
{
	uint8_t temp;
	temp = AT24CXX_ReadOneByte(255); //����ÿ�ο�����дAT24CXX ��Avoid writing AT24CXX every time you boot up
	if (temp == 0X55)
		return 0;
	else //�ų���һ�γ�ʼ������� ��Exclude the first initialization
	{
		AT24CXX_WriteOneByte(255, 0X55);
		temp = AT24CXX_ReadOneByte(255);
		if (temp == 0X55)
			return 0;
	}
	return 1;
}
/****************************************
��AT24CXX�����ָ����ַ��ʼ����ָ������������
ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
pBuffer  :���������׵�ַ
NumToRead:Ҫ�������ݵĸ���
************************************/
/****************************************
The specified address in AT24CXX starts to read a specified amount of data
ReadAddr :Address to start reading for 24c02  is 0~255
pBuffer  :Data array starting address
NumToRead:The number of data to be read out
************************************/
void AT24CXX_Read(uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead)
{
	while (NumToRead)
	{
		*pBuffer++ = AT24CXX_ReadOneByte(ReadAddr++);
		NumToRead--;
	}
}
/*************************************
��AT24CXX�����ָ����ַ��ʼд��ָ������������
WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
pBuffer   :���������׵�ַ
NumToWrite:Ҫд�����ݵĸ���
*******************************************/
/*************************************
The specified address in AT24CXX starts writing a specified amount of data
WriteAddr :Address to start writing for 24c02 is 0~255
pBuffer   :Data array starting address
NumToWrite:The number of data to be written
*******************************************/
void AT24CXX_Write(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite)
{
	while (NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr, *pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
