#include "PinmuxCfg.h"
#include "top.h"
#include "DrvExt.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#if 0
const PIN_GROUP_CONFIG vTopConfig[] = {
	// Module name              pinmux config

	//
	// Storage configuration
	//
	{PIN_FUNC_SDIO,             PIN_SDIO_CFG_4BITS},
	{PIN_FUNC_SDIO2,            PIN_SDIO_CFG_4BITS},
	{PIN_FUNC_SDIO3,            PIN_SDIO_CFG_4BITS | PIN_SDIO_CFG_1ST_PINMUX},
	{PIN_FUNC_NAND,             PIN_NAND_CFG_NONE},

	//
	// Sensor configuration
	//
	{PIN_FUNC_SENSOR,           PIN_SENSOR_CFG_MIPI},
	{PIN_FUNC_SENSOR2,          PIN_SENSOR2_CFG_NONE},
	{PIN_FUNC_MIPI_LVDS,        PIN_MIPI_LVDS_CFG_CLK0 | PIN_MIPI_LVDS_CFG_DAT0 | PIN_MIPI_LVDS_CFG_DAT1 | PIN_MIPI_LVDS_CFG_DAT2 | PIN_MIPI_LVDS_CFG_DAT3},

	//
	// Serial interface configuration
	//
	{PIN_FUNC_I2C,              PIN_I2C_CFG_CH1},
	{PIN_FUNC_SIF,              PIN_SIF_CFG_CH1},


	{PIN_FUNC_UART,             PIN_UART_CFG_CH1 | PIN_UART_CFG_CH2},
	{PIN_FUNC_SPI,              PIN_SPI_CFG_NONE},
	{PIN_FUNC_REMOTE,           PIN_REMOTE_CFG_NONE},

	//
	// PWM configuration
	//
	{PIN_FUNC_PWM,              PIN_PWM_CFG_NONE},

	//
	// AUDIO configuration
	//
	{PIN_FUNC_AUDIO,            PIN_AUDIO_CFG_NONE},

	//
	// Display configuration
	//
	{PIN_FUNC_LCD,              PINMUX_DISPMUX_SEL_LCD | PINMUX_LCDMODE_RGB_SERIAL},
	{PIN_FUNC_TV,               PINMUX_TV_HDMI_CFG_NORMAL},

	{PIN_FUNC_ETH,              PIN_ETH_CFG_NONE}
};
#endif
void IO_InitPinmux(void)
{
	MODELEXT_HEADER *header;
	PIN_GROUP_CONFIG *vTopConfig;

	vTopConfig = (PIN_GROUP_CONFIG *)Dx_GetModelExtCfg(MODELEXT_TYPE_PINMUX_CFG, &header);
	if (!vTopConfig || !header) {
		DBG_FATAL("Modelext: pinmux is null\n");
		return;
	}

#if defined(_CPU2_LINUX_)
	pinmux_init_for_dualcore((PIN_GROUP_CONFIG *)vTopConfig, TRUE);
#else
	pinmux_init((PIN_GROUP_CONFIG *)vTopConfig);
#endif

}

