#ifndef LCM32F039_ACMP_H
#define LCM32F039_ACMP_H
#include "lcm32f039.h"
/******************** ANACTRL Register Struct Definition *****************/

typedef struct
{
	//	__IO uint32_t ACMP_CSR;
	//
	//	__IO uint32_t ACMP_MUXCR;
	//
	//	__IO uint32_t ACMP_EXTCFG;
	__IO uint32_t ACMP0_CSR; // 0x00
	__IO uint32_t ACMP1_CSR; // 0x00
	__IO uint32_t ACMP2_CSR; // 0x00
	__IM uint32_t RESERVED[37];
	__IO uint32_t ACMP0_MUXCR;
	__IO uint32_t ACMP1_MUXCR;
	__IO uint32_t ACMP2_MUXCR;
	__IO uint32_t ACMP0_EXTCFG;
	__IO uint32_t ACMP1_EXTCFG;
	__IO uint32_t ACMP2_EXTCFG;

} ACMP_TypeDef;

typedef struct
{
	uint32_t ACMP_P_Select;		//�Ƚ�����������ѡ��/Comparator positive-end input selection
	uint32_t ACMP_N_Select;		//�Ƚ�����������ѡ��/Comparator negative-end input selection
	uint32_t ACMP_Delay_time;	//����ӳ�����/Output delay Configuration
	uint32_t ACMP_Blanking;		//�Ƚ�������Դѡ��/Comparator blanking source selection
	uint32_t ACMP_Is;			//�Ƚ����������/Comparator output polarity
	FunctionalState ACMP_HYSEN; //����ʹ������/Enable configuration of hysteresis
	FunctionalState ACMP_FREN;	//������Ӧʹ��/Enable configuration of quick response
	FunctionalState ACMP_BLANKING;	//�������ʹ��/Enable Software blanking

	FunctionalState CMP_INTENMASK; //�Ƚ��������Ƚ��ж�ʹ������/Comparator continuous comparison interrupt enables masking
	FunctionalState CMP_SEQ_MODE;  //����ģʽʹ��/Enable configuration of Continuous
	uint32_t TRIG_MODE;			   //����ģʽ����������ⲿӲ������/Trigger mode: Triggered by software or external hardware
	uint32_t CHNL_CNT_TIME;		   //�Ƚ���ͨ���л�ʱ������ѡ��/The selection of comparator channel switching clock cycle

	uint32_t EXTSELECT;					  // Two sets of trigger sources enable control bits
	FunctionalState EXTEN_Signal_andmode; // Trigger signal logic 'or' or 'and'
	uint32_t EXTEN_MODE_SEL;			  //�ⲿ����ģʽѡ��/External trigger mode selection
	uint32_t EXTEN_EDGE_MODE;			  //�ⲿ�����źŷ�ʽ/External trigger signal mode
	uint32_t SYN_MODE;					  //�ⲿ����ͬ�����첽/Synchronization or asynchrony is triggered externally
	uint32_t TRIG_DELAY_TIME;			  //�ⲿ�����ӳ�ʱ��/External trigger delay clock
	uint32_t TRIG_SOURCE;				  //����Դѡ��/Trigger source selection
	uint32_t CP_EXTSP;					  //�����źż���ѡ��/Trigger signal polarity selection

} ACMP_InitTypeDef;

/*****************  Definition for ACMP  **************/
#define CP_NOBLANKING (0x0UL << 12)
#define CP_BLANK_TM1OC4 (0x1UL << 12)
#define CP_BLANK_TM3OC4 (0x2UL << 12)
#define CP_BLANK_TM15OC1 (0x4UL << 12)

#define CP_NODelay_time (0x0UL << 9)
#define CP_Delay_time1 (0x1UL << 9)
#define CP_Delay_time2 (0x2UL << 9)
#define CP_Delay_time3 (0x3UL << 9)

#define CP_IS_POS (0x0UL << 3)
#define CP_IS_REV (0x1UL << 3)
// ACMP0 P-N_SELECT
#define CP0_PS_CPP0EXT_INPUT (0x0UL << 4)
#define CP0_PS_DAC0_HALLMID (0x1UL << 4)

#define CP0_NS_DAC0OUT (0x0UL << 6)
#define CP0_NS_CPN0EXT_INPUT (0x1UL << 6)

// ACMP1 P-N_SELECT
#define CP1_PS_CPP1EXT_INPUT (0x0UL << 4)
#define CP1_PS_DAC0OUT  (0x1UL << 4)

#define CP1_NS_DAC0OUT (0x0UL << 6)
#define CP1_NS_CPN1EXT_INPUT (0x1UL << 6)

// ACMP2 P-N_SELECT

/***************** ACMP_MUXCR **************/
#define CP_TIRG_INASOFT (0x0UL << 28)
#define CP_TIRG_EXTEN (0x1UL << 28)

#define CHNL_CNT_TIME_64CLK (0x3FUL << 2)
#define CHNL_CNT_TIME_0CLK (0x0UL << 2)

/***************** ACMP_EXTCFG **************/
#define CP_EXTEN_TRIG_DIS (0x0UL << 0)
#define CP_EXTEN_RISE (0x1UL << 0)
#define CP_EXTEN_FALL (0x2UL << 0)
#define CP_EXTEN_RISEANDFALL (0x3UL << 0)

#define CP_Edge_model (0x0UL << 2)
#define CP_level_model (0x1UL << 2)

#define TRIG_DELAY_TIME_128CLK (0x3fUL << 4)
#define TRIG_DELAY_TIME_0CLK (0x0UL << 4)

#define TRIG_TIM1_CC1 (0x0UL << 12)
#define TRIG_TIM1_CC2 (0x1UL << 12)
#define TRIG_TIM2_TRGO (0x2UL << 12)
#define TRIG_TIM1_EXTI11 (0x3UL << 12)
#define TRIG_TIM1_CC3 (0x0UL << 14)
#define TRIG_TIM1_CC4 (0x1UL << 14)
#define TRIG_TIM1_EXTI3 (0x2UL << 14)
#define TRIG_TIM1_EXTI1 (0x3UL << 14)

#define CP_SYN_MODE (0x0UL << 21)
#define CP_ASYN_MODE (0x1UL << 21)

#define CP_EXTSP_POS (0x0UL << 22)
#define CP_EXTSP_NOS (0x1UL << 22)

#define ENABLE_EXTSELG1 (0x2UL << 27)
#define ENABLE_EXTSELG0 (0x1UL << 27)
/************************************HALL_CR***************************************/
#define HALL_IP1_PA0 (0x0UL)
#define HALL_IP1_PA3 (0x1UL)

#define HALL_IP2_PA1 (0x0UL)
#define HALL_IP2_PA6 (0x1UL)

#define HALL_IP3_PA2 (0x0UL)
#define HALL_IP3_PA7 (0x1UL)
/***************** Bit Definition for ANACTRL_ACMPX_CSR Register **************/
#define ANACTRL_ACMPX_CSR_CP_OUT_Pos 28
#define ANACTRL_ACMPX_CSR_CP_OUT_Msk (0x1UL << ANACTRL_ACMPX_CSR_CP_OUT_Pos)
#define ANACTRL_ACMPX_CSR_CP_PF_Pos 16
#define ANACTRL_ACMPX_CSR_CP_PF_Msk (0x1FUL << ANACTRL_ACMPX_CSR_CP_PF_Pos)
#define ANACTRL_ACMPX_CSR_CP_BLANKING_Pos 12
#define ANACTRL_ACMPX_CSR_CP_BLANKING_Msk (0x7UL << ANACTRL_ACMPX_CSR_CP_BLANKING_Pos)
#define ANACTRL_ACMPX_CSR_CP_BLANKING_DIS (0x0UL << ANACTRL_ACMPX_CSR_CP_BLANKING_Pos)
#define ANACTRL_ACMPX_CSR_CP_BLANKING_TIM1_OC4 (0x1UL << ANACTRL_ACMPX_CSR_CP_BLANKING_Pos)
#define ANACTRL_ACMPX_CSR_CP_BLANKING_TIM2_OC4 (0x2UL << ANACTRL_ACMPX_CSR_CP_BLANKING_Pos)
#define ANACTRL_ACMPX_CSR_CP_BLANKING_TIM15_OC2 (0x3UL << ANACTRL_ACMPX_CSR_CP_BLANKING_Pos)
#define ANACTRL_ACMPX_CSR_CP_BLANKING_TIM1_OC3 (0x4UL << ANACTRL_ACMPX_CSR_CP_BLANKING_Pos)
#define ANACTRL_ACMPX_CSR_CP_BLANKING_TIM2_OC3 (0x5UL << ANACTRL_ACMPX_CSR_CP_BLANKING_Pos)
#define ANACTRL_ACMPX_CSR_CP_BLANKING_TIM15_OC1 (0x6UL << ANACTRL_ACMPX_CSR_CP_BLANKING_Pos)
#define ANACTRL_ACMPX_CSR_CP_BLANKING_TIM3_OC3 (0x7UL << ANACTRL_ACMPX_CSR_CP_BLANKING_Pos)
#define ANACTRL_ACMPX_CSR_CP_DAC_Pos 11
#define ANACTRL_ACMPX_CSR_CP_DAC_Msk (0x1UL << ANACTRL_ACMPX_CSR_CP_DAC_Pos)
#define ANACTRL_ACMPX_CSR_CP_DLY_Pos 9
#define ANACTRL_ACMPX_CSR_CP_DLY_Msk (0x3UL << ANACTRL_ACMPX_CSR_CP_DLY_Pos)
#define ANACTRL_ACMPX_CSR_CP_FS_Pos 8
#define ANACTRL_ACMPX_CSR_CP_FS_Msk (0x1UL << ANACTRL_ACMPX_CSR_CP_FS_Pos)
#define ANACTRL_ACMPX_CSR_CP_NS_Pos 6
#define ANACTRL_ACMPX_CSR_CP_NS_Msk (0x1UL << ANACTRL_ACMPX_CSR_CP_NS_Pos)
#define ANACTRL_ACMPX_CSR_CP_PS_Pos 4
#define ANACTRL_ACMPX_CSR_CP_PS_Msk (0x1UL << ANACTRL_ACMPX_CSR_CP_PS_Pos)
#define ANACTRL_ACMPX_CSR_CP_IS_Pos 3
#define ANACTRL_ACMPX_CSR_CP_IS_Msk (0x1UL << ANACTRL_ACMPX_CSR_CP_IS_Pos)
#define ANACTRL_ACMPX_CSR_CP_HYSEN_Pos 2
#define ANACTRL_ACMPX_CSR_CP_HYSEN_Msk (0x1UL << ANACTRL_ACMPX_CSR_CP_HYSEN_Pos)
#define ANACTRL_ACMPX_CSR_CP_FREN_Pos 1
#define ANACTRL_ACMPX_CSR_CP_FREN_Msk (0x1UL << ANACTRL_ACMPX_CSR_CP_FREN_Pos)
#define ANACTRL_ACMPX_CSR_CP_EN_Pos 0
#define ANACTRL_ACMPX_CSR_CP_EN_Msk (0x1UL << ANACTRL_ACMPX_CSR_CP_EN_Pos)

#define ANACTRL_ACMPX_MUXCR_CPOUT_PF_Pos 8
#define ANACTRL_ACMPX_MUXCR_CPOUT_PF_Msk (0xFF << ANACTRL_ACMPX_MUXCR_CPOUT_PF_Pos)
/***************** Bit Definition for ANACTRL_KEY Register **************/
#define ANACTRL_KEY_Pos 0
#define ANACTRL_KEY_Msk (0xFFFFFFFFUL << ANACTRL_KEY_Pos)
#define ANACTRL_KEY_VALUE 0xA98BC56D

/***************** ANACTRL Declaration **************/
// ANACTRL_ADDR_STR       (0x40017C00)
#define ACMP ((ACMP_TypeDef *)(0x40017C00 + 0x20))
//#define ACMP1 ((ACMP_TypeDef *) (0x40017C00 + 0x24))
//#define ACMP2 ((ACMP_TypeDef *) (0x40017C00 + 0x28))

/***************** ANACTRL Function Definition **************/

void ACMP_DeInit(void);
void ACMP0_Init(ACMP_InitTypeDef *ACMP_InitStruct);
void ACMP1_Init(ACMP_InitTypeDef *ACMP_InitStruct);
void ACMP2_Init(ACMP_InitTypeDef *ACMP_InitStruct);
void ACMP0_StructInit(ACMP_InitTypeDef *ACMP_InitStruct);
void ACMP1_StructInit(ACMP_InitTypeDef *ACMP_InitStruct);
void ACMP2_StructInit(ACMP_InitTypeDef *ACMP_InitStruct);
void ACMP0_Cmd(FunctionalState NewState);
void ACMP1_Cmd(FunctionalState NewState);
void ACMP2_Cmd(FunctionalState NewState);
void ACMP0_FS_Cmd(FunctionalState NewState);												   //�˲�ʹ��/Enable configuration of filtering
void ACMP0_HYSEN_Cmd(FunctionalState NewState);												   //����ʹ��/Enable configuration of hysteresis
void ACMP0_FREN_Cmd(FunctionalState NewState);												   //������Ӧʹ��/Enable configuration of quick response
void ACMP0_SEQ_INPUT_SELECT(FunctionalState NewState, uint8_t pseledata, uint8_t nselectdata); //�ⲿIO����ѡ��/External I/O input selection
void ACMP1_SEQ_INPUT_SELECT(FunctionalState NewState, uint8_t pseledata, uint8_t nselectdata);
void ACMP2_SEQ_INPUT_SELECT(FunctionalState NewState, uint8_t pseledata, uint8_t nselectdata);
uint32_t GET_ACMP0_SEQ_RESLUT(void);
void SATRT_OFACMP0(void);
void SATRT_OFACMP1(void);
void SATRT_OFACMP2(void);
void ACMP0_Trim(void);
void ACMP1_Trim(void);
void ACMP2_Trim(void);
uint32_t get_acmp0_trim(void);
uint32_t get_acmp1_trim(void);
uint32_t get_acmp2_trim(void);

/***************hallmid*******************************/
void Hallmid_IP1_Config(uint32_t HALL_IP1, FunctionalState NewState);
void Hallmid_IP2_Config(uint32_t HALL_IP2, FunctionalState NewState);
void Hallmid_IP3_Config(uint32_t HALL_IP3, FunctionalState NewState);
void Hallmid_Cmd(FunctionalState NewState);
#endif // __ANACTRL_H__
