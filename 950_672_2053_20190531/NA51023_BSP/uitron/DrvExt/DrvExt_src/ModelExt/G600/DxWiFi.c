#include "DxCfg.h"
#include "IOCfg.h"
#include "Debug.h"
#include "DxWiFi.h"
#include "SwTimer.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxWiFi
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
void DrvWiFi_PowerOn(void)
{
#if 0
	gpio_setPin(GPIO_WIFI_REG_ON);
	SwTimer_DelayMs(10); // need to delay 10 ms to wait for WiFi power ready
#endif
}

void DrvWiFi_PowerOff(void)
{
#if 0
	gpio_clearPin(GPIO_WIFI_REG_ON);
#endif
}

