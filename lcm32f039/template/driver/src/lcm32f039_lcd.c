/**
 ******************************************************************************
 * @file    lcm32f039_lcd.c
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    2021-05-21
 * @brief   This file provides all the lcd emulation firmware functions.
 ******************************************************************************/
/**
 ******************************************************************************
 * @attention:
 *
 *
 *
 *
 ******************************************************************************/
#include "lcm32f039_lcd.h"
#include "lcm32f039_rcc.h"
/**
 *******************************************************************************
 ** LCD��ʼ�� /LCD initialization
 ******************************************************************************/
void LCD_Init(LCD_InitTypeDef *LCD_InitStruct)
{
    uint32_t tmpreg1, tmpreg2;

    tmpreg1 = LCD_InitStruct->LCD_Bias;

    tmpreg2 = ((LCD_InitStruct->LCD_Brs) | ((LCD_InitStruct->LCD_Fcen) << 2) |
               (LCD_InitStruct->LCD_Crs) | (LCD_InitStruct->LCD_Cclks));
    LCD->SCDIV = LCD_InitStruct->LCD_Scdiv;
    LCD->CON |= tmpreg1;
    LCD->RCON = tmpreg2;
}

/*--------------------------------------------------------------------------------------------
  LCD ʹ�� /LCD enable
  --------------------------------------------------------------------------------------------*/
void LCD_Cmd(FunctionalState NewState)
{

    if (NewState != DISABLE)
    {
        LCD->CON |= (0x00000080);
    }
    else
    {
        LCD->CON &= (~(0x00000080));
    }
}

/*--------------------------------------------------------------------------------------------
  LCD ģ�����ʹ��   /Enable the simulation test
  --------------------------------------------------------------------------------------------*/
void LCD_AnalogModeTestCmd(FunctionalState NewState)
{

    if (NewState != DISABLE)
    {
        LCD->CON |= (0x00000040);
    }
    else
    {
        LCD->CON &= (~(0x00000040));
    }
}

/*********************************************************************************
 ** \brief GPIO IO����Ϊģ�⹦��ģʽ  /The I/O mode is simulated
 ** \param [in]  enPort          IO Port��
 ** \param [in]  enPin           IO Pin��
 ******************************************************************************/
void LCD_GpioSet_COM(GPIO_TypeDef *GPIOx, uint16_t GPIO_PinSource, uint32_t COMx)
{
    uint32_t tmpreg;
    uint32_t tmp_1 = 0;
    uint32_t tmp_2 = 0;
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
    tmpreg = GPIOx->MODER;
    tmpreg &= ~(0x03 << (GPIO_PinSource * 2));
    tmpreg |= 3 << GPIO_PinSource * 2;
    GPIOx->MODER = tmpreg;

    if (GPIO_PinSource < 8)
    {
        tmp_1 = GPIOx->AFRL;
        tmp_1 &= ~(0x0F << (GPIO_PinSource * 4));
        tmp_1 |= (1 << (GPIO_PinSource * 4));
        GPIOx->AFRL = tmp_1;
    }
    else
    {
        tmp_2 = GPIOx->AFRH;
        tmp_2 &= ~(0x0F << ((GPIO_PinSource - 8) * 4));
        tmp_2 |= (1 << ((GPIO_PinSource - 8) * 4));
        GPIOx->AFRH = tmp_2;
    }
    if (COMx < COM4)
        LCD->PEH |= COMx << 4;
    else
        LCD->PEL |= COMx >> 4;
    LCD->CON |= (COMx << 16);
}
/*********************************************************************************
 ** \brief GPIO IO����Ϊģ�⹦��ģʽ /The I/O mode is simulated
 ** \param [in]  enPort          IO Port��
 ** \param [in]  enPin           IO Pin��
 ******************************************************************************/
void LCD_GpioSet_SEG(GPIO_TypeDef *GPIOx, uint16_t GPIO_PinSource, uint32_t SEGx)
{
    uint32_t tmpreg;
    uint32_t tmp_1 = 0;
    uint32_t tmp_2 = 0;
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
    tmpreg = GPIOx->MODER;
    tmpreg &= ~(0x03 << (GPIO_PinSource * 2));
    tmpreg |= 3 << GPIO_PinSource * 2;
    GPIOx->MODER = tmpreg;
    if (SEGx != SEG29)
    {
        if (GPIO_PinSource < 8)
        {
            tmp_1 = GPIOx->AFRL;
            tmp_1 &= ~(0x0F << (GPIO_PinSource * 4));
            tmp_1 |= (1 << (GPIO_PinSource * 4));
            GPIOx->AFRL = tmp_1;
        }
        else
        {
            tmp_2 = GPIOx->AFRH;
            tmp_2 &= ~(0x0F << ((GPIO_PinSource - 8) * 4));
            tmp_2 |= (1 << ((GPIO_PinSource - 8) * 4));
            GPIOx->AFRH = tmp_2;
        }
    }
    else
    {
        tmp_1 = GPIOx->AFRL;
        tmp_1 &= ~(0x0F << 16);
        tmp_1 |= (2 << 16);
        GPIOx->AFRL = tmp_1;
    }
    if (SEGx < SEG32)
        LCD->PEL |= SEGx;
    else
        LCD->PEH |= SEGx & 0x00000003;
}
/*--------------------------------------------------------------------------------------------
  LCD ��˸��ʼ��  /Flicker initialization
  TKFSx:��˸Ƶ��ѡ��  /Flicker frequency selection
  TKMODx:��˸ģʽѡ��  /Flicker mode selection
  --------------------------------------------------------------------------------------------*/
void LCDTK_Init(uint16_t TKFSx, uint16_t TKMODx)
{
    assert_param(IS_LCD_TK_TKFS(TKFSx));
    assert_param(IS_LCD_TK_TKMOD(TKMODx));
    LCD->TK_CON |= TKFSx | TKMODx;
}
/*--------------------------------------------------------------------------------------------
  LCD ��˸ʹ��  /Flicker enable 
  --------------------------------------------------------------------------------------------*/
void LCDTK_Cmd(FunctionalState NewState)
{

    if (NewState != DISABLE)
        LCD->TK_CON |= (0x00000080);
    else
        LCD->TK_CON &= (~(0x00000080));
}
/*********************************************************************************
LCDѡ����˸��SEG  /LCD Select the blinking SEG
 ******************************************************************************/
void LCDTK_SEG_Cmd(uint32_t SEGx, FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
        if (SEGx < SEG32)
            LCD->TK_PEL |= SEGx;
        else
            LCD->TK_PEH |= (SEGx & 0x00000003);
    }
    else
    {
        if (SEGx < SEG32)
            LCD->TK_PEL &= ~SEGx;
        else
            LCD->TK_PEH &= ~(SEGx & 0x00000003);
    }
}
/*********************************************************************************
LCDѡ����˸��COM  /LCD Select the blinking COM
 ******************************************************************************/
void LCDTK_COM_Cmd(uint32_t COMx, FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
        if (COMx < COM4)
            LCD->TK_PEH |= COMx << 4;
        else
            LCD->TK_PEL |= COMx >> 4;
    }
    else
    {
        if (COMx < COM4)
            LCD->TK_PEH &= ~(COMx << 4);
        else
            LCD->TK_PEL &= ~(COMx >> 4);
    }
}
/*---------------------------------------------------------------------------
 LCD��˸�ж�����  /LCD blinking interrupts configuration
  --------------------------------------------------------------------------*/
void LCDTK_ITConfig(uint32_t LCD_FLAG, FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
        LCD->TK_CON |= 0x00000002; // TKIE����˸�ж�ʹ��λ;
    }
    else
    {
        LCD->TK_CON &= 0xFFFFFFFD;
    }
}
/*---------------------------------------------------------------------------
LCD��˸�ж������־λ  /LCD blinking interrupt clear flag
  --------------------------------------------------------------------------*/
void LCDTK_ClearFlag(uint32_t LCD_FLAG)
{
    LCD->TK_CON &= 0xFFFFFFFE;
}
