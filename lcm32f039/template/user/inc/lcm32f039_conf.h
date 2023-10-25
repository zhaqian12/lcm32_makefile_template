/**
  ******************************************************************************
  * @file    Project/LCM32F039_StdPeriph_Templates/LCM32f039_conf.h 
  * @author  MCD Application Team
  * @version V0.4.44
  * @date    14-04-2021
  * @brief   Library configuration file.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCM32F039_CONF_H
#define __LCM32F039_CONF_H

// #define ACMP_MODULE_ENABLED
// #define ADC_MODULE_ENABLED
// #define CRC_MODULE_ENABLED
// #define DAC_MODULE_ENABLED
// #define DBGMCU_MODULE_ENABLED
// #define DIV_MODULE_ENABLED
#define DMA_MODULE_ENABLED
// #define EEPROM_MODULE_ENABLED
// #define EXTI_MODULE_ENABLED
// #define FLASH_MODULE_ENABLED
#define GPIO_MODULE_ENABLED
// #define I2C_MODULE_ENABLED
// #define IWDG_MODULE_ENABLED
// #define LCD_MODULE_ENABLED
// #define OPA_MODULE_ENABLED
#define PWR_MODULE_ENABLED
#define RCC_MODULE_ENABLED
// #define SSP_MODULE_ENABLED
// #define TIM_MODULE_ENABLED
#define UART_MODULE_ENABLED
// #define WT_MODULE_ENABLED

#ifdef ACMP_MODULE_ENABLED
#include "lcm32f039_acmp.h"
#endif
#ifdef ADC_MODULE_ENABLED
#include "lcm32f039_adc.h"
#endif
#ifdef CRC_MODULE_ENABLED
#include "lcm32f039_crc.h"
#endif
#ifdef DAC_MODULE_ENABLED
#include "lcm32f039_dac.h"
#endif
#ifdef DBGMCU_MODULE_ENABLED
#include "lcm32f039_dbgmcu.h"
#endif
#ifdef DIV_MODULE_ENABLED
#include "lcm32f039_div.h"
#endif
#ifdef DMA_MODULE_ENABLED
#include "lcm32f039_dma.h"
#endif
#ifdef EEPROM_MODULE_ENABLED
#include "lcm32f039_eeprom.h"
#endif
#ifdef EXTI_MODULE_ENABLED
#include "lcm32f039_exti.h"
#endif
#ifdef FLASH_MODULE_ENABLED
#include "lcm32f039_flash.h"
#endif
#ifdef GPIO_MODULE_ENABLED
#include "lcm32f039_gpio.h"
#endif
#ifdef I2C_MODULE_ENABLED
#include "lcm32f039_i2c.h"
#endif
#ifdef IWDG_MODULE_ENABLED
#include "lcm32f039_iwdg.h"
#endif
#ifdef LCD_MODULE_ENABLED
#include "lcm32f039_lcd.h"
#endif
#ifdef OPA_MODULE_ENABLED
#include "lcm32f039_opa.h"
#endif
#ifdef PWR_MODULE_ENABLED
#include "lcm32f039_pwr.h"
#endif
#ifdef RCC_MODULE_ENABLED
#include "lcm32f039_rcc.h"
#endif
#ifdef SSP_MODULE_ENABLED
#include "lcm32f039_ssp.h"
#endif
#ifdef TIM_MODULE_ENABLED
#include "lcm32f039_tim.h"
#endif
#ifdef UART_MODULE_ENABLED
#include "lcm32f039_uart.h"
#endif
#ifdef WT_MODULE_ENABLED
#include "lcm32f039_wt.h"
#endif

/* Exported macro ------------------------------------------------------------*/
#ifdef USE_FULL_ASSERT

//����ʱ���Էſ�����궨�壬��ʱ���ֻ��һ�����Ӵ���˵������У���ǳɹ��ģ�����Ϊʲô��һ������
//ԭ��μ���http://www.openedv.com/forum.php?mod=viewthread&tid=233944&page=1&extra=
//  https://www.cnblogs.com/vivohan/p/8470680.html
#define uint8_t unsigned char
#define uint32_t unsigned int
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
#define assert_param(expr) ((expr) ? ((void)0) : (assert_failed((uint8_t *)__FILE__, __LINE__)))
/* Exported functions ------------------------------------------------------- */
void assert_failed(uint8_t *file, uint32_t line);

#undef uint8_t
#undef uint32_t
#else
#define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif /* __LCM32F039_CONF_H */

/************************ (C) COPYRIGHT LCMicroelectronics *****END OF FILE****/
