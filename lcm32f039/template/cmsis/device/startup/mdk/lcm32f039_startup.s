;/**************************************************************************//**
; * @file     startup_LCM32F039.s
; * @brief    CMSIS Core Device Startup File for
; *           ARMCM0 Device Series
; * @version  V5.00
; * @date     02. March 2016
; ******************************************************************************/
;/*
; * Copyright (c) 2009-2016 ARM Limited. All rights reserved.
; *
; * SPDX-License-Identifier: Apache-2.0
; *
; * Licensed under the Apache License, Version 2.0 (the License); you may
; * not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; * www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an AS IS BASIS, WITHOUT
; * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; */

;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/
;*/@author ps by zhang.
;*/@ref https://blog.csdn.net/Hao_Zhen/article/details/84575295
; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>
;һ����Ա�ǩ����һ����ַ�������ĵ�ַ�������ĵ�ַ�����ߴ����(section)�ĵ�ַ������ĵ�ַһ��Ϊ��ʼ��ַ,


Stack_Size      EQU     0x00000400                        ;��ʾ��Ҫ����stack�Ĵ�СΪ1024 byte ��1KB���������仰����ִ�з���������൱��C�ꡣ// (R13)SP=0x20000000 + Stack_Size; ����������ջ
                                                                                                                                             ; (R13)SP=0x20000800 + Stack_Size; ����������ջ
                AREA    STACK, NOINIT, READWRITE, ALIGN=3 ;�����������ԣ����뷽ʽ
Stack_Mem       SPACE   Stack_Size                        ;space ָ�����ڷ�������ֽڵĿռ䣬����ʼ��Ϊ0���������ã������仰��ʾ��stace_mem��ʾ����ʼ��ַ���������Ĵ�СΪ1KB�ռ䱸�ò���ʼ��Ϊ0
__initial_sp                                              ;ջ��ָ��


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000800                       ;��ʾ��Ҫ����heap�Ĵ�СΪ3072 byte��3KB���������仰����ִ�з��������

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3 ;ָ���ô���ε����ԺͶ��뷽ʽ
__heap_base                                              ;�ѵ���ʼ��ַ
Heap_Mem        SPACE   Heap_Size                        ;ִ�пռ�������񣨱��ã�������ʼ��Ϊ0����ʼ��ַΪHeap_Mem����ĵ�ַ
__heap_limit                                             ;�ѵı߽�


                PRESERVE8                               ;The PRESERVE8 directive specifies that the current file preserves eight-byte alignment of the stack.
                                                        ;It sets the PRES8 build attribute to inform the linker.
                                                        
                THUMB                                   ;This option instructs the assembler to interpret instructions as Thumb instructions, using the UAL syntax. 
                                                        ;This is equivalent to a THUMB directive at the start of the source file.


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY          ;reset �Ƕ����֣�DATAָ�����������ݣ�������ָ�readonly�Ƿ���Ȩ��
                EXPORT  __Vectors                      ;����һ���ⲿ��ţ�������C�����е���
                EXPORT  __Vectors_End                  ;������� __Vector_End
                EXPORT  __Vectors_Size                 ;������� __Vector_Size

__Vectors       DCD     __initial_sp                ; Top of Stack     ;DCD��һ��αָ����߱�������ô����������������ɻ����룬DCD����word�洢�ռ�  //MSP
                DCD     Reset_Handler               ; Reset Handler    ;��λ����
                DCD     NMI_Handler                 ; NMI Handler      ;�����ж�������
                DCD     HardFault_Handler           ; Hard Fault Handler
                DCD     0                           ; Reserved
                DCD     0                           ; Reserved
                DCD     0                           ; Reserved
                DCD     0                           ; Reserved
                DCD     0                           ; Reserved
                DCD     0                           ; Reserved
                DCD     0                           ; Reserved
                DCD     SVC_Handler                 ; SVCall Handler
                DCD     0                           ; Reserved
                DCD     0                           ; Reserved
                DCD     PendSV_Handler              ; PendSV Handler
                DCD     SysTick_Handler             ; SysTick Handler

                ; External Interrupts               ;�����ж�
                DCD     WWDG_Handler                ; 00 : WWDG                 Handler
				DCD     EXTI16_LVD_Handler          ; 01 : LVD                  Handler
                DCD     WT_Handler                  ; 02 : WT                   Handler
                DCD     EFLS_Handler                ; 03 : EFLS                 Handler
                DCD     CHIPCTRL_Handler            ; 04 : CHIPCTRL             Handler
                DCD     EXTI0_1_Handler             ; 05 : EXTI0_1              Handler
                DCD     EXTI2_3_Handler             ; 06 : EXTI2_3              Handler
                DCD     EXTI4_15_Handler            ; 07 : EXTI4_15             Handler
                DCD     LCD_ACMP0_2_Handler         ; 08 : LCD_ACMP0_2          Handler
                DCD     DMAC_CH0_Handler            ; 09 : DMAC_CH0             Handler
                DCD     DMAC_CH1_2_Handler          ; 10 : DMAC_CH1_2           Handler
                DCD     DMAC_CH3_Handler            ; 11 : DMAC_CH3             Handler
                DCD     ADC_Handler                 ; 12 : ADC                  Handler
                DCD     TIM1_NON_CC_Handler         ; 13 : TIM1_NON_CC          Handler
                DCD     TIM1_CC_Handler             ; 14 : TIM1_CC              Handler
                DCD     TIM2_Handler                ; 15 : TIM2                 Handler
                DCD     TIM3_Handler                ; 16 : TIM3                 Handler
                DCD     DAC0_1_Handler              ; 17 : TDAC0_1              Handler
                DCD     TIM7_DAC3_5_PLC_Handler     ; 18 : TIM7_DAC3_5_PLC      Handler
                DCD     TIM14_Handler               ; 19 : TIM14                Handler
                DCD     TIM15_Handler               ; 20 : TIM15                Handler
                DCD     TIM16_Handler               ; 21 : TIM16                Handler
                DCD     TIM17_Handler               ; 22 : TIM17                Handler
                DCD     I2C0_Handler                ; 23 : I2C0                 Handler 
                DCD     I2C1RVE_Handler             ; 24 : I2C1rve              Handler					
                DCD     SSP0_Handler                ; 25 : SSP0                 Handler 
                DCD     SSP2_Handler                ; 26 : ssp2                 Handler					
                DCD     UART0_Handler               ; 27 : UART0                Handler
                DCD     UART1_Handler               ; 28 : UART1                Handler
                DCD     UART3_Handler               ; 29 : UART3                Handler
                DCD     CAN_Handler                 ; 30 : CAN                  Handler
                DCD     DIV_Handler                 ; 31 : DIV                  Handler

__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors   ;�����ж�������ռ��С

                AREA    |.text|, CODE, READONLY     ;�涨����Ϊ*.text�Ķοռ�Ϊ����Σ���˫|��������Ϊ�˷�ֹ���������ֿ�ͷ������Ȩ��ΪRO


; Reset Handler

Reset_Handler   PROC                                ;PROC��ͬ��FUNCTION������ָ��һ�������Ŀ�ʼ
				EXPORT  Reset_Handler      [WEAK]   ;weak��������ⲿ������Ҫ��д��������������ʾͬ�⸲д����仰�ĺ�����˵��Reset_Handler�ĺ�������Ϊ������д��ȫ�ֺ�����
                IMPORT  SystemInit                      ;IMPORT��ʾ.��Ҫ.����һ���ⲿ������һ����˵�����ⲿC�ļ��ж����˵ĺ���
                IMPORT  __main                          ;����__main �������Ҳ����ģ���Ϊ���Ǳ��빤�����Լ������ģ����ú�������C�����е�main()���������ú�������ִ�й���������mian();
                LDR     R0, =SystemInit                 
                                                        ;LDRαָ���systeminit������ַ�ĵ�ַ������ص�R0�Ĵ������ᱻ�滻��MOV R0, #SystemInit
                BLX     R0                              ;��תָ�����תǰ�ȱ�����תǰ��һ��Ҫִ�е�ָ�LR��Ȼ����ת�� systeminit ����
                LDR     R0, =__main                     ;LDRαָ���__main ������ַ���ص�R0�Ĵ���
                BX      R0                              ;��תָ���ת��__main ���������ϸ�ؽ������__main���ȸ���rom���rw-data��ram���ٽ�PC��λ��c��main����ָ�룩
                ENDP                                    ;����һ�������Ľ�������ENDFUNC������ͬ


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC                                ;���庯���Ŀ�ʼ
                EXPORT  NMI_Handler                 [WEAK];EXPORT������C���Ե���
                B       .                           ;B. ��ʾ��ת����ǰָ���ʲôҲ��ִ��
                ENDP
HardFault_Handler  PROC
                EXPORT  HardFault_Handler           [WEAK]
                BX      LR
				ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                 [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler              [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler             [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  WWDG_Handler              [WEAK]
				EXPORT  EXTI16_LVD_Handler        [WEAK]
                EXPORT  WT_Handler                [WEAK]
                EXPORT  EFLS_Handler              [WEAK]
                EXPORT  CHIPCTRL_Handler          [WEAK]
                EXPORT  EXTI0_1_Handler           [WEAK]
                EXPORT  EXTI2_3_Handler           [WEAK]
                EXPORT  EXTI4_15_Handler          [WEAK]
                EXPORT  LCD_ACMP0_2_Handler       [WEAK]
                EXPORT  DMAC_CH0_Handler          [WEAK]
                EXPORT  DMAC_CH1_2_Handler        [WEAK]
                EXPORT  DMAC_CH3_Handler          [WEAK]
                EXPORT  ADC_Handler               [WEAK]
                EXPORT  TIM1_NON_CC_Handler       [WEAK]
                EXPORT  TIM1_CC_Handler           [WEAK]
                EXPORT  TIM2_Handler              [WEAK]
                EXPORT  TIM3_Handler              [WEAK]
                EXPORT  DAC0_1_Handler            [WEAK]
                EXPORT  TIM7_DAC3_5_PLC_Handler   [WEAK]
                EXPORT  TIM14_Handler             [WEAK]
                EXPORT  TIM15_Handler             [WEAK]
                EXPORT  TIM16_Handler             [WEAK]
                EXPORT  TIM17_Handler             [WEAK]
                EXPORT  I2C0_Handler              [WEAK]
				EXPORT  I2C1RVE_Handler           [WEAK]
                EXPORT  SSP0_Handler              [WEAK]
				EXPORT  SSP2_Handler              [WEAK]
                EXPORT  UART0_Handler             [WEAK]
                EXPORT  UART1_Handler             [WEAK]
                EXPORT  UART3_Handler             [WEAK]
                EXPORT  CAN_Handler               [WEAK]
                EXPORT  DIV_Handler               [WEAK]
                

WWDG_Handler
EXTI16_LVD_Handler
WT_Handler
EFLS_Handler
CHIPCTRL_Handler
EXTI0_1_Handler
EXTI2_3_Handler
EXTI4_15_Handler
LCD_ACMP0_2_Handler
DMAC_CH0_Handler
DMAC_CH1_2_Handler
DMAC_CH3_Handler
ADC_Handler
TIM1_NON_CC_Handler
TIM1_CC_Handler
TIM2_Handler
TIM3_Handler
DAC0_1_Handler
TIM7_DAC3_5_PLC_Handler
TIM14_Handler
TIM15_Handler
TIM16_Handler
TIM17_Handler
I2C0_Handler
I2C1RVE_Handler
SSP0_Handler
SSP2_Handler
UART0_Handler
UART1_Handler
UART3_Handler
CAN_Handler
DIV_Handler
                B       .

                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

__user_initial_stackheap PROC
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP

                ALIGN

                ENDIF


                END

	ALIGN
    EXPORT  OS_CPU_SR_Save                                      ; Functions declared in this file
    EXPORT  OS_CPU_SR_Restore





;One memory region is for the stack and the other is for the heap. The size of the heap region can be zero. The stack region can be in allocated memory or inherited from the execution environment.
;To use the two-region model rather than the default single-region model, use either:
;IMPORT __use_two_region_memory from assembly language
;#pragma import(__use_two_region_memory) from C.



















