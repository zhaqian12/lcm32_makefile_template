/**
 ******************************************************************************
 * @file    lcm32f039_opa.c
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    2021-05-21
 * @brief   This file provides all the opa emulation firmware functions.
 ******************************************************************************/
/**
 ******************************************************************************
 * @attention:
 *
 *
 *
 *
 ******************************************************************************/
#include "lcm32f039_rcc.h"
#include "lcm32f039_opa.h"
#include "lcm32f039_conf.h"
/***************** OPA **************/

/*--------------------------------------------------------------------------------------------
ANACTRLʱ�ӹر� :DAC ACMP OPA ���߹���һ��ʱ��ʹ��,��һ�μ���
/ANACTRL clock off: Enables the DAC, ACMP, and OPA to share one clock
  --------------------------------------------------------------------------------------------*/
void OPA_DeInit(void)
{
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_ANACTRL, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_ANACTRL, DISABLE);
}

/*--------------------------------------------------------------------------------------------
OPA�˷ų�ʼ�� ���Դ�У׼ /OPA OPamp initialization: built-in calibration
 --------------------------------------------------------------------------------------------*/
void OPA_Init(OPA_TypeDef *opa_ptr, OPA_InitTypeDef *OPA_InitStruct)
{
	uint32_t dataTRIM = 0;
	uint32_t tmpreg = 0;
	if (opa_ptr == OPA0)
	{
		OPA0_TRIM();
		dataTRIM = get_opa0_trim();
		ANACTRL->OPA0_CSR = (dataTRIM << 16);
	}
	if (opa_ptr == OPA1)
	{
		OPA1_TRIM();
		dataTRIM = get_opa1_trim();
		ANACTRL->OPA1_CSR = (dataTRIM << 16);
	}
	if (opa_ptr == OPA2)
	{
		OPA2_TRIM();
		dataTRIM = get_opa2_trim();
		ANACTRL->OPA2_CSR = (dataTRIM << 16);
	}
	if (opa_ptr == OPA3)
	{
		OPA3_TRIM();
		dataTRIM = get_opa3_trim();
		ANACTRL->OPA3_CSR = (dataTRIM << 16);
	}
	tmpreg = opa_ptr->OPA_CSR;
	tmpreg &= ~(0xF0007FF);
	tmpreg = (OPA_InitStruct->OPA_Oppselect | OPA_InitStruct->OPA_Opnselect | OPA_InitStruct->OPA_Gain |
			  OPA_InitStruct->OPA_BiasVoltage | OPA_InitStruct->OPA_BiasVoltageSource | OPA_InitStruct->OPA_FeedbackRes | OPA_InitStruct->OPA_Opoto_Gpio |
			  OPA_InitStruct->OPA_Opoto2_Gpio | OPA_InitStruct->OPA_OppRES_short | OPA_InitStruct->OPA_OpnRES_short);
	opa_ptr->OPA_CSR |= tmpreg;
}
/*********************
OPA_NIN����ѡ��/OPA NIN pin selection
********************************************/
void OPA_NINSelect(uint32_t NINSelect)
{
	ANACTRL->ANA_CSR &= 0xFFFFFCFF;
	ANACTRL->ANA_CSR |= NINSelect;
}
/*--------------------------------------------------------------------------------------------
�˷Ų����ṹ�� �����ʼ��     /Opamp parameter structure     Define the initialization
--------------------------------------------------------------------------------------------*/
void OPA_StructInit(OPA_InitTypeDef *OPA_InitStruct)
{
	/*--------------- Reset OPA init structure parameters values -----------------*/
	OPA_InitStruct->OPA_Oppselect = OPA0_Oppselect_PA2;

	OPA_InitStruct->OPA_Opnselect = OPA0_Opnselect_PA3;

	OPA_InitStruct->OPA_Gain = OPA_Gain_20;

	OPA_InitStruct->OPA_BiasVoltage = OPA_BiasVoltage_DISEN;

	OPA_InitStruct->OPA_FeedbackRes = OPA_FeedbackRes_EN;

	OPA_InitStruct->OPA_Opoto_Gpio = OPA_OPOEX_DISEN;

	OPA_InitStruct->OPA_Opoto2_Gpio = OPA_OPOEX2_DISEN;

	OPA_InitStruct->OPA_OppRES_short = OPA_OppRES_short_DISEN;

	OPA_InitStruct->OPA_OpnRES_short = OPA_OpnRES_short_DISEN;

	OPA_InitStruct->OPA_BiasVoltageSource = OPA_OPA_BiasVoltageSource_HALFVRH;
}

/*--------------------------------------------------------------------------------------------
�˷�ʹ�ܺ���   /The op-amp enable function
  --------------------------------------------------------------------------------------------*/
void OPA_Cmd(OPA_TypeDef *opa_ptr, FunctionalState NewState)
{
	if (NewState != DISABLE)
	{

		opa_ptr->OPA_CSR |= (0x00000001);
	}
	else
	{

		opa_ptr->OPA_CSR &= (~(0x00000001));
	}
}

uint32_t opa0_trim_FLAG(void)
{
	return ((ANACTRL->OPA0_CSR & ANACTRL_OPA0_CSR_OPTRIMO_Msk) >> ANACTRL_OPA0_CSR_OPTRIMO_Pos);
}
uint32_t opa1_trim_FLAG(void)
{
	return ((ANACTRL->OPA1_CSR & ANACTRL_OPA1_CSR_OPTRIMO_Msk) >> ANACTRL_OPA1_CSR_OPTRIMO_Pos);
}
uint32_t opa2_trim_FLAG(void)
{
	return ((ANACTRL->OPA2_CSR & ANACTRL_OPA2_CSR_OPTRIMO_Msk) >> ANACTRL_OPA2_CSR_OPTRIMO_Pos);
}
uint32_t opa3_trim_FLAG(void)
{
	return ((ANACTRL->OPA3_CSR & ANACTRL_OPA3_CSR_OPTRIMO_Msk) >> ANACTRL_OPA3_CSR_OPTRIMO_Pos);
}
/*--------------------------------------------------------------------------------------------
�˷�У׼������ֵ����   /Opamp calibration parameter assignment function
  --------------------------------------------------------------------------------------------*/
void opa0_trim(uint32_t trim)
{

	ANACTRL->OPA0_CSR = (ANACTRL->OPA0_CSR & ~ANACTRL_OPA0_CSR_OPOS_Msk) |
						(trim << ANACTRL_OPA0_CSR_OPOS_Pos);
}
uint32_t get_opa0_trim(void)
{
	return ((ANACTRL->OPA0_CSR & ANACTRL_OPA0_CSR_OPOS_Msk) >> ANACTRL_OPA0_CSR_OPOS_Pos);
}
void opa1_trim(uint32_t trim)
{

	ANACTRL->OPA1_CSR = (ANACTRL->OPA1_CSR & ~ANACTRL_OPA1_CSR_OPOS_Msk) |
						(trim << ANACTRL_OPA1_CSR_OPOS_Pos);
}

uint32_t get_opa1_trim(void)
{
	return ((ANACTRL->OPA1_CSR & ANACTRL_OPA1_CSR_OPOS_Msk) >> ANACTRL_OPA1_CSR_OPOS_Pos);
}

void opa2_trim(uint32_t trim)
{

	ANACTRL->OPA2_CSR = (ANACTRL->OPA2_CSR & ~ANACTRL_OPA2_CSR_OPOS_Msk) |
						(trim << ANACTRL_OPA2_CSR_OPOS_Pos);
}

uint32_t get_opa2_trim(void)
{
	return ((ANACTRL->OPA2_CSR & ANACTRL_OPA2_CSR_OPOS_Msk) >> ANACTRL_OPA2_CSR_OPOS_Pos);
}
void opa3_trim(uint32_t trim)
{

	ANACTRL->OPA3_CSR = (ANACTRL->OPA3_CSR & ~ANACTRL_OPA3_CSR_OPOS_Msk) |
						(trim << ANACTRL_OPA3_CSR_OPOS_Pos);
}

uint32_t get_opa3_trim(void)
{
	return ((ANACTRL->OPA3_CSR & ANACTRL_OPA3_CSR_OPOS_Msk) >> ANACTRL_OPA3_CSR_OPOS_Pos);
}
/*--------------------------------------------------------------------------------------------
�˷�У׼����  /Opamp calibration function
  --------------------------------------------------------------------------------------------*/
void OPA0_TRIM()
{
	uint8_t CTRIMOLD = 0;
	uint8_t CTRIM = 0x1f;
	uint8_t direction = 0;
	unsigned char flag = 0;
	uint32_t mode = 0;
	uint32_t temp_ANA = 0;
	temp_ANA = ANACTRL->ANA_CSR;
	ANACTRL->ANA_CSR = 0x00000000;
	ANACTRL->OPA0_CSR = 0X4a000001;
	opa0_trim(CTRIM);
	mode = opa0_trim_FLAG();
	while (flag == 0)
	{
		opa0_trim(CTRIM);
		delay10us(20); // 200uS
		mode = opa0_trim_FLAG();
		switch (direction)
		{
		case 0:
		{
			if (mode == 0)
			{
				CTRIM--;
			}
			if ((mode == 1) || (CTRIM == 0))
			{
				CTRIMOLD = CTRIM;
				CTRIM = 0;
				direction = 1;
			}
			break;
		}
		case 1:
		{
			if (mode == 1)
			{
				CTRIM++;
			}
			if ((CTRIM == 0x1f) || (mode == 0))
			{
				flag = 1;
				if (CTRIMOLD == 0)
				{
					opa0_trim(CTRIM);
				}
				else
				{
					CTRIM = (CTRIM + CTRIMOLD) >> 1;
					opa0_trim(CTRIM);
				}
			}
			break;
		}
		default:
			opa0_trim(0);
			break;
		}
	}
	ANACTRL->ANA_CSR = temp_ANA;
}

void OPA1_TRIM()
{
	uint8_t CTRIMOLD = 0;
	uint8_t CTRIM = 0x1f;
	uint8_t direction = 0;
	unsigned char flag = 0;
	uint32_t mode = 0;
	uint32_t temp_ANA = 0;
	temp_ANA = ANACTRL->ANA_CSR;
	ANACTRL->ANA_CSR = 0x00000000;
	ANACTRL->OPA1_CSR = 0X4a000001;
	opa1_trim(CTRIM);
	mode = opa1_trim_FLAG();
	while (flag == 0)
	{
		opa1_trim(CTRIM);
		delay10us(20); // 200uS
		mode = opa1_trim_FLAG();
		switch (direction)
		{
		case 0:
		{
			if (mode == 0)
			{
				CTRIM--;
			}
			if ((mode == 1) || (CTRIM == 0))
			{
				CTRIMOLD = CTRIM;
				CTRIM = 0;
				direction = 1;
			}
			break;
		}
		case 1:
		{
			if (mode == 1)
			{
				CTRIM++;
			}
			if ((CTRIM == 0x1f) || (mode == 0))
			{
				flag = 1;
				if (CTRIMOLD == 0)
				{
					opa1_trim(CTRIM);
				}
				else
				{
					CTRIM = (CTRIM + CTRIMOLD) >> 1;
					opa1_trim(CTRIM);
				}
			}
			break;
		}
		default:
			opa1_trim(0);
			break;
		}
	}
	ANACTRL->ANA_CSR = temp_ANA;
}

void OPA2_TRIM()
{
	uint8_t CTRIMOLD = 0;
	uint8_t CTRIM = 0x1f;
	uint8_t direction = 0;
	unsigned char flag = 0;
	uint32_t mode = 0;
	uint32_t temp_ANA = 0;
	temp_ANA = ANACTRL->ANA_CSR;
	ANACTRL->ANA_CSR = 0x00000000;
	ANACTRL->OPA2_CSR = 0X4a000001;
	opa2_trim(CTRIM);
	mode = opa2_trim_FLAG();
	while (flag == 0)
	{
		opa2_trim(CTRIM);
		delay10us(20); // 200uS
		mode = opa2_trim_FLAG();
		switch (direction)
		{
		case 0:
		{
			if (mode == 0)
			{
				CTRIM--;
			}
			if ((mode == 1) || (CTRIM == 0))
			{
				CTRIMOLD = CTRIM;
				CTRIM = 0;
				direction = 1;
			}
			break;
		}
		case 1:
		{
			if (mode == 1)
			{
				CTRIM++;
			}
			if ((CTRIM == 0x1f) || (mode == 0))
			{
				flag = 1;
				if (CTRIMOLD == 0)
				{
					opa2_trim(CTRIM);
				}
				else
				{
					CTRIM = (CTRIM + CTRIMOLD) >> 1;
					opa2_trim(CTRIM);
				}
			}
			break;
		}
		default:
			opa2_trim(0);
			break;
		}
	}
	ANACTRL->ANA_CSR = temp_ANA;
}
void OPA3_TRIM()
{
	uint8_t CTRIMOLD = 0;
	uint8_t CTRIM = 0x1f;
	uint8_t direction = 0;
	unsigned char flag = 0;
	uint32_t mode = 0;
	uint32_t temp_ANA = 0;
	temp_ANA = ANACTRL->ANA_CSR;
	ANACTRL->ANA_CSR = 0x00000000;
	ANACTRL->OPA3_CSR = 0X4a000001;
	opa3_trim(CTRIM);
	mode = opa3_trim_FLAG();
	while (flag == 0)
	{
		opa3_trim(CTRIM);
		delay10us(20); // 200uS
		mode = opa3_trim_FLAG();
		switch (direction)
		{
		case 0:
		{
			if (mode == 0)
			{
				CTRIM--;
			}
			if ((mode == 1) || (CTRIM == 0))
			{
				CTRIMOLD = CTRIM;
				CTRIM = 0;
				direction = 1;
			}
			break;
		}
		case 1:
		{
			if (mode == 1)
			{
				CTRIM++;
			}
			if ((CTRIM == 0x1f) || (mode == 0))
			{
				flag = 1;
				if (CTRIMOLD == 0)
				{
					opa3_trim(CTRIM);
				}
				else
				{
					CTRIM = (CTRIM + CTRIMOLD) >> 1;
					opa3_trim(CTRIM);
				}
			}
			break;
		}
		default:
			opa3_trim(0);
			break;
		}
	}
	ANACTRL->ANA_CSR = temp_ANA;
}
