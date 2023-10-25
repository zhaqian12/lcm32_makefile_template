/**
  ******************************************************************************
  * @file      startup_lcm32f039.s
  * @author    MCD Application Team
  * @brief     LCM32F039 series vector table for GCC toolchain.
  *            This module performs:
  *                - Set the initial SP
  *                - Set the initial PC == Reset_Handler,
  *                - Set the vector table entries with the exceptions ISR address
  *                - Branches to main in the C library (which eventually
  *                  calls main()).
  *            After Reset the Cortex-M0 processor is in Thread mode,
  *            priority is Privileged, and the Stack is set to Main.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

  .syntax unified
  .cpu cortex-m0
  .fpu softvfp
  .thumb

.global g_pfnVectors
.global Default_Handler

/* start address for the initialization values of the .data section.
defined in linker script */
.word _sidata
/* start address for the .data section. defined in linker script */
.word _sdata
/* end address for the .data section. defined in linker script */
.word _edata
/* start address for the .bss section. defined in linker script */
.word _sbss
/* end address for the .bss section. defined in linker script */
.word _ebss

  .section .text.Reset_Handler
  .weak Reset_Handler
  .type Reset_Handler, %function
Reset_Handler:
  ldr   r0, =_estack
  mov   sp, r0          /* set stack pointer */
  
  /* Call the clock system initialization function.*/
  bl  SystemInit

/* Copy the data segment initializers from flash to SRAM */
  ldr r0, =_sdata
  ldr r1, =_edata
  ldr r2, =_sidata
  movs r3, #0
  b LoopCopyDataInit

CopyDataInit:
  ldr r4, [r2, r3]
  str r4, [r0, r3]
  adds r3, r3, #4

LoopCopyDataInit:
  adds r4, r0, r3
  cmp r4, r1
  bcc CopyDataInit
  
/* Zero fill the bss segment. */
  ldr r2, =_sbss
  ldr r4, =_ebss
  movs r3, #0
  b LoopFillZerobss

FillZerobss:
  str  r3, [r2]
  adds r2, r2, #4

LoopFillZerobss:
  cmp r2, r4
  bcc FillZerobss

/* Call static constructors */
  bl __libc_init_array
/* Call the application's entry point.*/
  bl main

LoopForever:
    b LoopForever


.size Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 *
 * @param  None
 * @retval : None
*/
    .section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b Infinite_Loop
  .size Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M0.  Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
******************************************************************************/
   .section .isr_vector,"a",%progbits
  .type g_pfnVectors, %object
  .size g_pfnVectors, .-g_pfnVectors


g_pfnVectors:
  .word  _estack
  .word  Reset_Handler
  .word  NMI_Handler
  .word  HardFault_Handler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  0
  .word  SVC_Handler
  .word  0
  .word  0
  .word  PendSV_Handler
  .word  SysTick_Handler
  .word  WWDG_Handler                   /* Window WatchDog              */
  .word  EXTI16_LVD_Handler             /* LVD                          */
  .word  WT_Handler                     /* WT                           */
  .word  EFLS_Handler                   /* EFLS                         */
  .word  CHIPCTRL_Handler               /* CHIPCTRL                     */
  .word  EXTI0_1_Handler                /* EXTI Line 0 and 1            */
  .word  EXTI2_3_Handler                /* EXTI Line 2 and 3            */
  .word  EXTI4_15_Handler               /* EXTI Line 4 to 15            */
  .word  LCD_ACMP0_2_Handler            /* LCD_ACMP0_2                  */
  .word  DMAC_CH0_Handler               /* DMA1 Channel 0               */
  .word  DMAC_CH1_2_Handler             /* DMA1 Channel 1 and Channel 2 */
  .word  DMAC_CH3_Handler               /* DMA1 Channel 3               */
  .word  ADC_Handler                    /* ADC                          */
  .word  TIM1_NON_CC_Handler            /* TIM1_NON_CC                  */
  .word  TIM1_CC_Handler                /* TIM1_CC                      */
  .word  TIM2_Handler                   /* TIM2                         */
  .word  TIM3_Handler                   /* TIM3                         */
  .word  DAC0_1_Handler                 /* DAC0_1                       */
  .word  TIM7_DAC3_5_PLC_Handler        /* TIM7_DAC3_5_PLC              */
  .word  TIM14_Handler                  /* TIM14                        */
  .word  TIM15_Handler                  /* TIM15                        */
  .word  TIM16_Handler                  /* TIM16                        */
  .word  TIM17_Handler                  /* TIM17                        */
  .word  I2C0_Handler                   /* I2C0                         */
  .word  I2C1RVE_Handler                /* I2C1rve                      */
  .word  SSP0_Handler                   /* SSP0                         */
  .word  SSP2_Handler                   /* SSP2                         */
  .word  UART0_Handler                  /* UART0                        */
  .word  UART1_Handler                  /* UART1                        */
  .word  UART3_Handler                  /* UART3                        */
  .word  CAN_Handler                    /* CAN                          */
  .word  DIV_Handler                    /* DIV                          */

/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/

  .weak      NMI_Handler
  .thumb_set NMI_Handler,Default_Handler

  .weak      HardFault_Handler
  .thumb_set HardFault_Handler,Default_Handler

  .weak      SVC_Handler
  .thumb_set SVC_Handler,Default_Handler

  .weak      PendSV_Handler
  .thumb_set PendSV_Handler,Default_Handler

  .weak      SysTick_Handler
  .thumb_set SysTick_Handler,Default_Handler

  .weak      WWDG_Handler
  .thumb_set WWDG_Handler,Default_Handler

  .weak      EXTI16_LVD_Handler
  .thumb_set EXTI16_LVD_Handler,Default_Handler

  .weak      WT_Handler
  .thumb_set WT_Handler,Default_Handler

  .weak      EFLS_Handler
  .thumb_set EFLS_Handler,Default_Handler

  .weak      CHIPCTRL_Handler
  .thumb_set CHIPCTRL_Handler,Default_Handler

  .weak      EXTI0_1_Handler
  .thumb_set EXTI0_1_Handler,Default_Handler

  .weak      EXTI2_3_Handler
  .thumb_set EXTI2_3_Handler,Default_Handler

  .weak      EXTI4_15_Handler
  .thumb_set EXTI4_15_Handler,Default_Handler

  .weak      LCD_ACMP0_2_Handler
  .thumb_set LCD_ACMP0_2_Handler,Default_Handler

  .weak      DMAC_CH0_Handler
  .thumb_set DMAC_CH0_Handler,Default_Handler

  .weak      DMAC_CH1_2_Handler
  .thumb_set DMAC_CH1_2_Handler,Default_Handler

  .weak      DMAC_CH3_Handler
  .thumb_set DMAC_CH3_Handler,Default_Handler

  .weak      ADC_Handler
  .thumb_set ADC_Handler,Default_Handler

  .weak      TIM1_NON_CC_Handler
  .thumb_set TIM1_NON_CC_Handler,Default_Handler

  .weak      TIM1_CC_Handler
  .thumb_set TIM1_CC_Handler,Default_Handler

  .weak      TIM2_Handler
  .thumb_set TIM2_Handler,Default_Handler

  .weak      TIM3_Handler
  .thumb_set TIM3_Handler,Default_Handler

  .weak      DAC0_1_Handler
  .thumb_set DAC0_1_Handler,Default_Handler

  .weak      TIM7_DAC3_5_PLC_Handler
  .thumb_set TIM7_DAC3_5_PLC_Handler,Default_Handler

  .weak      TIM14_Handler
  .thumb_set TIM14_Handler,Default_Handler

  .weak      TIM15_Handler
  .thumb_set TIM15_Handler,Default_Handler

  .weak      TIM16_Handler
  .thumb_set TIM16_Handler,Default_Handler

  .weak      TIM17_Handler
  .thumb_set TIM17_Handler,Default_Handler

  .weak      I2C0_Handler
  .thumb_set I2C0_Handler,Default_Handler

  .weak      I2C1RVE_Handler
  .thumb_set I2C1RVE_Handler,Default_Handler

  .weak      SSP0_Handler
  .thumb_set SSP0_Handler,Default_Handler

  .weak      SSP2_Handler
  .thumb_set SSP2_Handler,Default_Handler

  .weak      UART0_Handler
  .thumb_set UART0_Handler,Default_Handler

  .weak      UART1_Handler
  .thumb_set UART1_Handler,Default_Handler

  .weak      UART3_Handler
  .thumb_set UART3_Handler,Default_Handler

  .weak      CAN_Handler
  .thumb_set CAN_Handler,Default_Handler

  .weak      DIV_Handler
  .thumb_set DIV_Handler,Default_Handler


