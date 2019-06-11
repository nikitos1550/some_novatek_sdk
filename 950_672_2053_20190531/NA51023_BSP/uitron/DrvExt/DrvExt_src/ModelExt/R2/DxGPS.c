#include "DxCfg.h"
#include "uart.h"
#include "GPS.h"
#include "DxGPS.h"
#include "Debug.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxGPS
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

static GPS_UART_OBJ UartObj = {
	.open       = uart3_open,
	.close      = uart3_close,
	.init       = uart3_init,
	.getString  = uart3_getString,
};

void DrvGPS_SetUartPort(void)
{
	GPS_SetUartObj(&UartObj);
}
