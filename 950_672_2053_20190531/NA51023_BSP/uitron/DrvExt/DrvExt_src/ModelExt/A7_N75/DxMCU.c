/**
    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.

    @file       DxMCU.c
    @ingroup    mIDxMCU

    @brief      MCU control

    @date       2013/04/30
*/

#include "DxMCU.h"
#include "uart.h"


#define TEST        DISABLE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxMCU
#define __DBGLVL__          2 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

//========================================================

static UINT32 uart_baud = UART_BAUDRATE_57600;
static UINT32 uart_length = UART_LEN_L8_S1;

UINT32 DxMCU_Setcfgs(DXMCU_CFG_ID CfgID, UINT32 Param1) // Set Config Setting
{
	switch (CfgID) {
	case DXMCU_CFG_UART_BAUDRATE:
		uart_baud = Param1;
		break;
	case DXMCU_CFG_UART_LENGTH:
		uart_length = Param1;
		break;
	default:
		break;
	}
	return DX_OK;
}

UINT32 DxMCU_Open(void) // Common Constructor
{
#if (TEST ==ENABLE)
	CHAR    RecSentence[180];
	CHAR    uiLen;
#endif
	DBG_FUNC_BEGIN("\r\n");

	uart2_open();
	while ((uart2_checkIntStatus() & UART_INT_STATUS_TX_EMPTY) == 0) {
		;
	}
	uart2_init(uart_baud, uart_length, UART_PARITY_NONE);


#if (TEST ==ENABLE)
	uart2_putString("Send Message to PC\r\n");
#endif

	return DX_OK;
}

UINT32 DxMCU_Get(CHAR *pChar)
{
	//#NT#2017/01/10#HM Tseng -begin
	//#NT#abort to exit revc task
	if (!pChar) {
		uart2_abortGetChar();
		return 0;
	}
	//#NT#2017/01/10#HM Tseng -end
	return uart2_getChar(pChar);
}

UINT32 DxMCU_Put(CHAR *pChar, UINT32 len)
{
	UINT32 i = 0;
	for (i = 0; i < len; i++) {
		//DBGH(*(pChar+i));
		uart2_putChar(*(pChar + i));
	}
	return DX_OK;
}

UINT32 DxMCU_Close(void) // Common Destructor
{
	DBG_FUNC_BEGIN("\r\n");
	uart2_close();
	return DX_OK;
}


