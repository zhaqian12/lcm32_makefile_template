/**
  ******************************************************************************
  * @file    lcm32f039_div.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    2021-05-21
  * @brief   This file provides all the div emulation firmware functions.
  ******************************************************************************/
/**
  ******************************************************************************
  * @attention:
  * 
  * 
  * 
  * 
  ******************************************************************************/

#include "lcm32f039_div.h"

long long Fun_64_DIV_32(long long dividend, int divisor, DIV_Result *result)
{
	D_Typedef DID;
	DID.Dividend64 = dividend;
	DIV0->DIV64_CTRL = 1 << 24;//被除数64位
	DIV0->DIV64_ALO = DID.D.LOW32;
	DIV0->DIV64_AHI = DID.D.HIGH32;
	DIV0->DIV64_B = divisor;
	result->result64.D.LOW32 = DIV0->DIV64_QUOTLO;
	result->result64.D.HIGH32 = DIV0->DIV64_QUOTHI;
	result->remainder64 = DIV0->DIV64_REM;
	DIV0->DIV64_CTRL_b.PROCCEED = SET; //写1清0
	
	return (long long)result->result64.Dividend64;
}
long long Fun_32_DIV_32(int dividend, int divisor, DIV_Result *result)
{
	DIV0->DIV64_CTRL = 1 << 24 | 1 << 8;//被除数32位
	DIV0->DIV64_ALO = dividend;
	DIV0->DIV64_B = divisor;
	result->result64.D.LOW32 = DIV0->DIV64_QUOTLO;
	result->result64.D.HIGH32 = DIV0->DIV64_QUOTHI;
	result->remainder64 = DIV0->DIV64_REM;
	DIV0->DIV64_CTRL_b.PROCCEED = SET; //写1清0
	
	return (long long)result->result64.Dividend64;
}


int Fun_32_DIV_16(int dividend, int divisor, result_t *result)
{
	DIV1->DIV32_CTRL_b.DIV16 = 0; //被除数32位
	DIV1->DIV32_ALO = dividend;
	DIV1->DIV32_B = divisor;
	result->result32 = DIV1->DIV32_QUOTLO;
	result->remainder32 = DIV1->DIV32_REM;
	DIV1->DIV32_CTRL_b.PROCCEED = SET; //写1清0
	
	return result->result32;
}

int Fun_16_DIV_16(int dividend, int divisor, result_t *result)
{
	DIV1->DIV32_CTRL_b.DIV16 = 1; //被除数16位
	DIV1->DIV32_ALO = dividend;
	DIV1->DIV32_B = divisor;
	result->result32 = DIV1->DIV32_QUOTLO;
	result->remainder32 = DIV1->DIV32_REM;
	DIV1->DIV32_CTRL_b.PROCCEED = SET; //写1清0
	
	return result->result32;
}
