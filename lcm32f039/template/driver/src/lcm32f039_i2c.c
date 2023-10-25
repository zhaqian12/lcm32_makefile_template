/**
 ******************************************************************************
 * @file    lcm32f039_iic.c
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    2021-05-21
 * @brief   This file provides all the iic emulation firmware functions.
 ******************************************************************************/

#include "lcm32f039_i2c.h"
#include "lcm32f039.h"
#include "lcm32f039_rcc.h"
/*************************************
 *  config as a i2c slave
 *************************************/

void I2C_DeInit(I2C_TypeDef *I2Cx)
{
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C0, ENABLE);

  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C0, DISABLE);
}

/*--------------------------------------------------------------------------------------------
   I2C �յ� General Call �Ƿ񷵻� ACK/NACK�����ڴӻ�ģʽ����Ч
   /I2C Determine whether to return an ACK or NACK after receiving the General Call. This is valid only in slave mode
  --------------------------------------------------------------------------------------------*/
void SlaveAckGeneralCall(I2C_TypeDef *I2Cx, bool status)
{
  if (status == TRUE)
  {
    I2Cx->ACKGENERALCALL |= I2C_ACK_GEN_CALL_Msk;
  }
  if (status == FALSE)
  {
    I2Cx->ACKGENERALCALL &= ~I2C_ACK_GEN_CALL_Msk;
  }
}

/*--------------------------------------------------------------------------------------------
//����General Call �㲥,������Ҫ�ֶ����
/A General Call broadcast is generated and manually cleared
--------------------------------------------------------------------------------------------*/
void I2C_GenerateGeneralCall(I2C_TypeDef *I2Cx)
{
  I2Cx->TAR |= I2C_TAR_SPECIAL_ON;    // special set
  I2Cx->TAR &= ~I2C_TAR_GC_START_Msk; // GC_OR_START reset
}

/*--------------------------------------------------------------------------------------------
//��� General Call �㲥
/Clear the General Call broadcast
--------------------------------------------------------------------------------------------*/
void I2C_ClearGeneralCall(I2C_TypeDef *I2Cx)
{
  I2Cx->TAR &= ~I2C_TAR_SPECIAL_ON;
}

/*--------------------------------------------------------------------------------------------
//����Start Byte��û˵����Ҫ�ֶ����
/Start Byte is generated, but must be manually cleared
--------------------------------------------------------------------------------------------*/
void I2C_GenerateStartByte(I2C_TypeDef *I2Cx)
{
  I2Cx->TAR |= I2C_TAR_SPECIAL_ON;  // special set
  I2Cx->TAR |= I2C_TAR_GC_START_ON; // GC_OR_START set
}

/*--------------------------------------------------------------------------------------------
//��ȷ�����շ��Ĳ����ʣ�SCLʱ��;
/Clear data transceiver baud rate, SCL clock;
--------------------------------------------------------------------------------------------*/
void I2C_SetFrequent(I2C_TypeDef *I2Cx, int sclClock)
{
  uint32_t tmpreg = 0;
  uint32_t freq, LCNT, HCNT, SPKLEN;
  RCC_ClocksTypeDef RCC_Clocks;
  SpeedSelectType Mode;
  tmpreg = I2Cx->CON;
  tmpreg &= 0xFFFFFFF9;
  if (sclClock <= 100000)
  {
    Mode = STANDARD_MODE;

    tmpreg |= I2C_SPEED_STANDARD_MODE;
  }
  else if (sclClock <= 400000)
  {
    Mode = FAST_MODE;
    tmpreg |= I2C_SPEED_FAST_MODE;
  }
  else if (sclClock <= 1000000)
  {
    Mode = HIGH_SPEED;
    tmpreg |= I2C_SPEED_FAST_MODE;
  }
  I2Cx->CON = tmpreg;
  RCC_GetClockFreq(&RCC_Clocks);
  freq = RCC_Clocks.I2C0_Frequency;
  SPKLEN = I2Cx->FSSPKLEN;
  LCNT = (freq - 2 * sclClock) / (2 * sclClock);
  HCNT = (LCNT - SPKLEN - 6);
  if ((HCNT + SPKLEN + 7 > 17) && (LCNT + 1 > 16))
  {
    if (Mode == STANDARD_MODE)
    {
      I2Cx->SSLCNT = LCNT;
      I2Cx->SSHCNT = HCNT;
    }
    else if (Mode == FAST_MODE)
    {
      I2Cx->FSLCNT = LCNT - 1;
      I2Cx->FSHCNT = HCNT - 1;
    }
    else if (Mode == HIGH_SPEED)
    {
      I2Cx->FSLCNT = LCNT - 2;
      I2Cx->FSHCNT = HCNT - 2;
    }
  }
}
/*********************************************
 *threshold value of TX_FIFO to raise the interrupt
���� TX_EMPTY �жϴ�������ֵ�趨����Χ 0~7
 * *******************************************/
void I2C_TxFifoITthreshold(uint32_t TxFifoValue)
{
  assert_param(IS_I2C_TX_FIFO_IT(TxFifoValue));
  I2C0->TXTL = TxFifoValue;
}
/*********************************************
 *threshold value of RX_FIFO to raise the interrupt
���� RX_FULL �жϴ�������ֵ�趨����Χ 0~7
 * *******************************************/
void I2C_RxFifoITthreshold(uint32_t RxFifoValue)
{
  assert_param(IS_I2C_RX_FIFO_IT(RxFifoValue));
  I2C0->RXTL = RxFifoValue;
}

//=========================================================================
/** ����Ŀ���ַ master mode
 * @brief  Handles I2Cx communication when starting transfer or during transfer (TC or TCR flag are set).
 */
void I2C_TransferHandling(I2C_TypeDef *I2Cx, uint32_t Address)
{
  uint32_t tmpreg = 0;
  tmpreg = I2Cx->TAR;
  tmpreg &= (uint32_t)0xFFFFEC00;
  tmpreg |= Address;
  if (tmpreg & 0x00000300)
    tmpreg |= I2C_TAR_10ADDR_MAS; //��������10λͨ�ŵ�ַ
  else
    tmpreg &= ~I2C_TAR_10ADDR_MAS; //��������7λͨ�ŵ�ַ
  I2Cx->TAR = tmpreg;
}
/****************************************
���ôӻ���ַ
config the Slaver Address when working in I2C slave mode.
 **********************************************************************************/
void I2C_SlaveAddressConfig(I2C_TypeDef *I2Cx, uint32_t Address)
{
  I2Cx->SAR = Address & ((uint32_t)0x000003FF); // �ӻ����õ�ַ��
}

/**************************************************
 * I2Cʹ��
 * ****************************************************/
void I2C_Cmd(I2C_TypeDef *I2Cx, FunctionalState NewState)
{

  if (NewState != DISABLE)
  {
    /* Enable the selected I2C peripheral */
    I2Cx->ENABLE |= (I2C_ENABLE_ON);
  }
  else
  {
    /* Disable the selected I2C peripheral */
    I2Cx->ENABLE &= (~I2C_ENABLE_ON);
  }
}

void I2C_StructInit(I2C_InitTypeDef *I2C_InitStruct)
{
  /*---------------- Reset I2C init structure parameters values --------------*/
  I2C_InitStruct->I2C_mode = I2C_MasterOnlyMode;                  //����ģʽ
  I2C_InitStruct->I2C_StopDetMaster = I2C_StopDet_MasterAlways;   //����ģʽ�µ�STOP_DET�ж�
  I2C_InitStruct->I2C_RxFifoFullHold = I2C_RxFifoFullHold_ENABLE; //����I2C��RX FIFO�ﵽRX_RX_BUFFER_DEPTHʱ�Ƿ�hold����
  I2C_InitStruct->I2C_TxEmptyIntr = I2C_TxEmptyIntr_ENABLE;       //����TX_EMPTY�жϵĲ�����ʽ
  I2C_InitStruct->I2C_Speed = 400000;                             // SCL��������
}

//ʹ�ýṹ���ʼ��I2C==============���====================================
void I2C_Init(I2C_TypeDef *I2Cx, I2C_InitTypeDef *I2C_InitStruct)
{
  uint32_t tmpreg = 0;
  tmpreg = I2Cx->CON;
  tmpreg &= 0xFFFFF830;
  I2Cx->ENABLE &= (~I2C_ENABLE_ON);
  if (I2C_InitStruct->I2C_mode & 0x01)
  {
    tmpreg |= (I2C_InitStruct->I2C_mode) | (I2C_InitStruct->I2C_TxEmptyIntr) |
              (I2C_InitStruct->I2C_RxFifoFullHold) | (I2C_InitStruct->I2C_StopDetMaster);
  }
  else
  {
    tmpreg |= (I2C_InitStruct->I2C_mode) | (I2C_InitStruct->I2C_SlaveAddressedLength) |
              (I2C_InitStruct->I2C_StopDetSlave) | (I2C_InitStruct->I2C_TxEmptyIntr) |
              (I2C_InitStruct->I2C_RxFifoFullHold);
  }
  I2Cx->CON = tmpreg;
  if (I2C_InitStruct->I2C_mode & 0x01)
  {
    I2C_SetFrequent(I2Cx, I2C_InitStruct->I2C_Speed);
  }
}

/************************************************************
 * I2C registers management functions
 * @brief  Reads the specified I2C register and returns its value.
 * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
 * @param  I2C_Register: specifies the register to read.
 *********************************************************************/
uint32_t I2C_ReadRegister(I2C_TypeDef *I2Cx, uint8_t I2C_Register)
{
  __IO uint32_t tmp = 0;

  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_REGISTER(I2C_Register));

  tmp = (uint32_t)I2Cx;
  tmp += I2C_Register;

  /* Return the selected register value */
  return (*(__IO uint32_t *)tmp);
}
/******************************************************
 * ����д�����ͣ�
 * master write(send)
 * *******************************************/
void I2C_WriteData(I2C_TypeDef *I2Cx, uint8_t Data)
{
 /* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	uint32_t tmpPreg;
	tmpPreg = I2Cx->DATACMD;
	tmpPreg &= 0xFFFFFE00; // д
	tmpPreg |= Data;
	I2Cx->DATACMD = tmpPreg;
}
/******************************************************
 * ����д�����ͣ����һ֡���STOP
 * master send the last frame and send STOP sign
 * *******************************************/
void I2C_WriteData_Stop(I2C_TypeDef *I2Cx, uint8_t Data)
{
 /* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	uint32_t tmpPreg;
	tmpPreg = I2Cx->DATACMD;
	tmpPreg &= 0xFFFFFE00; // д
	tmpPreg |= (1 << 9) | Data; // STOP
 I2Cx->DATACMD = tmpPreg;
}
/******************************************************
 * ������
 * master read
 * *******************************************/
uint8_t I2C_ReadData(I2C_TypeDef *I2Cx)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  uint8_t tempData;
  I2Cx->DATACMD |= I2C_DATA_CMD_CMD_Msk; //��
  while (I2C_GetITStatus(I2C0, I2C_STATUS_RFNF) == RESET)
    ;
  tempData = ((uint8_t)(I2Cx->DATACMD & 0xff));
  return tempData;
}
/******************************************************
 * ���������һ֡���STOP
 * master read the last frame and send STOP sign
 * *******************************************/
uint8_t I2C_ReadData_Stop(I2C_TypeDef *I2Cx)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  uint8_t tempData;
  uint32_t tmpPreg;
  tmpPreg |= I2C_DATA_CMD_CMD_Msk | (1 << 9); //��+STOP
  I2Cx->DATACMD |= tmpPreg;
  while (I2C_GetITStatus(I2C0, I2C_STATUS_RFNF) == RESET)
    ;
  tempData = ((uint8_t)(I2Cx->DATACMD & 0xff));
  return tempData;
}

/******************************************************
 * �ӻ�����
 * *******************************************/
uint32_t I2C_ReceiveData(I2C_TypeDef *I2Cx)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  uint32_t tempData;
  tempData = I2Cx->DATACMD & 0x08ff;
  return tempData;
}

/******************************************************
 * �ӻ�����
 * *******************************************/
void I2C_SendData(I2C_TypeDef *I2Cx, uint8_t Data)
{
   uint32_t tmpPreg;
	 assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	 tmpPreg = I2Cx->DATACMD;
	 tmpPreg &= 0xFFFFFF00;
	 tmpPreg |= Data;
	 I2Cx->DATACMD = tmpPreg;
}
//===DMA����ʹ��=======�ѽ���======================================================
void I2C_DMACmd(I2C_TypeDef *I2Cx, uint32_t I2C_DMAReq, FunctionalState NewState)
{
  assert_param(IS_I2C_DMAReq(I2C_DMAReq));
  if (I2C_DMAReq == I2C_DMAReq_Rx)
  {
    (NewState != DISABLE) ? (I2Cx->DMACR |= I2C_DMAReq_Rx) : (I2Cx->DMACR &= ~I2C_DMAReq_Rx);
  }
  else
  {
    (NewState != DISABLE) ? (I2Cx->DMACR |= I2C_DMAReq_Tx) : (I2Cx->DMACR &= ~I2C_DMAReq_Tx);
  }
}
/*********************************************
 *threshold value of TX_DMA to raise the interrupt
���� TX_EMPTY �жϴ�������ֵ�趨����Χ 0~7
 * *******************************************/
void I2C_TxDMAITthreshold(uint32_t TxDMAValue)
{
  assert_param(IS_I2C_TX_DMA_IT(TxDMAValue));
  I2C0->DMATDLR = TxDMAValue;
}
/*********************************************
 *threshold value of RX_FIFO to raise the interrupt
���� RX_FULL �жϴ�������ֵ�趨����Χ 0~7
 * *******************************************/
void I2C_RxDMAITthreshold(uint32_t RxDMAValue)
{
  assert_param(IS_I2C_RX_DMA_IT(RxDMAValue));
  I2C0->DMARDLR = RxDMAValue;
}

/*************************************
 *  I2C �ж�ԭʼ״̬
 * ************************************************************************/
FlagStatus I2C_GetFlagStatus(I2C_TypeDef *I2Cx, uint32_t I2C_FLAG)
{
  assert_param(IS_I2C_GET_FLAG(I2C_FLAG));
  unsigned int tmpreg = 0;
  FlagStatus bitstatus = RESET;
  tmpreg = I2Cx->RAWINTRSTAT;
  tmpreg &= I2C_FLAG;
  if (tmpreg != 0)
  {
    /* I2C_FLAG is set */
    bitstatus = SET;
  }
  else
  {
    /* I2C_FLAG is reset */
    bitstatus = RESET;
  }
  return bitstatus;
}
/**************************************
//��ȡ״̬��־λ��set or reset
****************************************************/
FlagStatus I2C_GetITStatus(I2C_TypeDef *I2Cx, uint32_t I2C_STATUS)
{
  assert_param(IS_I2C_GET_STATUS(I2C_STATUS));
  unsigned int tmpreg = 0;
  FlagStatus bitstatus = RESET;
  tmpreg = I2Cx->STATUS;
  tmpreg &= I2C_STATUS;
  if (tmpreg != 0)
  {
    /* I2C_FLAG is set */
    bitstatus = SET;
  }
  else
  {
    /* I2C_FLAG is reset */
    bitstatus = RESET;
  }
  return bitstatus;
}

//=============================�ѽ���=====================================/finished

//����ж�����/Lift interrupt suspension
void I2C_ClearITPendingBit(I2C_TypeDef *I2Cx, uint32_t I2C_IT)
{
  assert_param(IS_I2C_GET_IT(I2C_IT));
  /* Clear the selected flag */
  switch (I2C_IT)
  {
  //��master txfifoΪ��ʱ������ʱ������   #13  /Enable clock extension when the Master TXFIFO is null
  case I2C_IT_MST_ON_HOLD:;
    break; // auto clear by hardware
  // slave���յ�restart�źŽ����ж�ʹ��   #12  /Lave receives the RESTART signal to enable the interrupt function
  case I2C_IT_RESTART_DET:
    I2Cx->CLRRESTARTDET;
    break;
  //�ӻ�������Ӧ�㲥��ַ�����ж�   #11  /Slave machine enter response broadcast address enter interrupt
  case I2C_IT_GEN_CALL:
    I2Cx->CLRGENCALL;
    break;
  // I2C�ӿ��Ƿ������start����restart�źţ�master��slaver������    #10  /Whether the I2C interface generates a start or restart signal. This parameter applies to the master or SLAver interface
  case I2C_IT_START_DET:
    I2Cx->CLRSTARTDET;
    break;
  // i2c�ӿ����Ƿ������stop�ź�#9   /i2c�ӿ����Ƿ������stop�ź�
  case I2C_IT_STOP_DET:
    I2Cx->CLRSTOP_DET;
    break;
  // i2c active ��־�ж�#8  /I2c ACTIVE flag interrupt 
  case I2C_IT_ACTIVITY:
    I2Cx->CLRACTIVITY;
    break;
  //*�ӻ�*����1byte������δӦ��slave�����жϣ�   #7   /Slave sends 1byte, the host does not respond (slave type interrupt)
  case I2C_IT_RX_DONE:
    I2Cx->CLRRXDONE;
    break;
  //���Ͳ��ܼ��������ж�,master slave������#6  /The send cannot continue to generate interrupts. This applies to both master slaves
  case I2C_IT_TX_ABRT:
    I2Cx->CLRTXABRT;
    break;
  //�ӻ�׼����������ǰ������ʱ��ֱ��д�����ݺ�����жϣ��ӻ�ģʽ�жϣ� #5  /Before the slave is ready to send data, pull down the clock until the interrupt is cleared after the data is written (slave mode interrupt)
  case I2C_IT_RD_REQ:
    I2Cx->CLRRDREQ;
    break;
  // Txfifo empty intr #4
  case I2C_IT_TX_EMPTY:;
    break;
  // Txfifo ����ж�   #3 /Overflow trap
  case I2C_IT_TX_OVER:
    I2Cx->CLRTXOVER;
    break;
  // Rxfifo ���ж�#2   /Full interrupt
  case I2C_IT_RX_FULL:;
    break;
  // RXfifo�����ж�    #1 Overflow interrupt
  case I2C_IT_RX_OVER:
    I2Cx->CLRRXOVER;
    break;
  // rxfifo �����պ���Ŷ������������ж�#0
  case I2C_IT_RX_UNDER:
    I2Cx->CLRRXUNDER;
    break;
  case I2C_IT_ALL:
    I2Cx->CLRINTR;
    break;
  default:
    break;
  }
}

/**********************************
 * Enables or disables the specified  interrupts.
 * ********************************************/
void I2C_ITConfig(I2C_TypeDef *I2Cx, uint32_t I2C_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_GET_IT(I2C_IT));

  I2Cx->INTRMASK = 0x00000000;
  if (NewState != DISABLE)
  {
    /* Enable the selected I2C interrupts */
    I2Cx->INTRMASK |= I2C_IT;
  }
  else
  {
    /* Disable the selected I2C interrupts */
    I2Cx->INTRMASK &= ~I2C_IT;
  }
}

//====================��===��=====��=====��======��=================/Content to be processed
