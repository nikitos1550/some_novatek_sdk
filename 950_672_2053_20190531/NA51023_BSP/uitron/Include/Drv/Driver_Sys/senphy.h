/*
    LVDS/MIPI-CSI/HiSPi Sensor PHY Configuration Driver Global header

    LVDS/MIPI-CSI/HiSPi Sensor PHY Configuration Driver Global header

    @file       senphy.h
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/

#ifndef _SENPHY_H
#define _SENPHY_H

#include "Type.h"


/**
    Sensor PHY configuration Selection

    This is used in senphy_setConfig() to specify which of the function is selected to assign new configuration.
*/
typedef enum {
	SENPHY_CONFIG_ID_ENO_DLY,       ///< (Clock) Lane Enable to Valid Output Delay Count in (60/80/120)MHz.

	SENPHY_CONFIG_ID_DLY_EN,        ///< Set new PHY Delay/DeSkew Function Enable/Disable.
	SENPHY_CONFIG_ID_DLY_CLK0,      ///< Clock Lane 0 analog Delay Unit select. Valid value range from 0x0 to 0x7.
	SENPHY_CONFIG_ID_DLY_CLK1,      ///< Clock Lane 1 analog Delay Unit select. Valid value range from 0x0 to 0x7.

	SENPHY_CONFIG_ID_DLY_DAT0,      ///< Data Lane 0 analog Delay Unit select.  Valid value range from 0x0 to 0x7.
	SENPHY_CONFIG_ID_DLY_DAT1,      ///< Data Lane 1 analog Delay Unit select.  Valid value range from 0x0 to 0x7.
	SENPHY_CONFIG_ID_DLY_DAT2,      ///< Data Lane 2 analog Delay Unit select.  Valid value range from 0x0 to 0x7.
	SENPHY_CONFIG_ID_DLY_DAT3,      ///< Data Lane 3 analog Delay Unit select.  Valid value range from 0x0 to 0x7.

	SENPHY_CONFIG_ID_IADJ,          ///< Analog Block Current Source adjustment.
									///< Set 0x0 is 20uA (Default value). Set 0x1 is 40uA. 0x2 is 30uA. 0x3 is 60uA.

	SENPHY_CONFIG_ID_INV_DAT0,      ///< Data Lane 0 pad pin P/N Swap Enable/Disable.
	SENPHY_CONFIG_ID_INV_DAT1,      ///< Data Lane 1 pad pin P/N Swap Enable/Disable.
	SENPHY_CONFIG_ID_INV_DAT2,      ///< Data Lane 2 pad pin P/N Swap Enable/Disable.
	SENPHY_CONFIG_ID_INV_DAT3,      ///< Data Lane 3 pad pin P/N Swap Enable/Disable.

	SENPHY_CONFIG_CK1_EN,			///< Clock lane 1 enable/disable. This shall be enabled during dual sensor application.

	SENPHY_CONFIG_ID_DBGMSG,        //   Sensor PHY Module Debug Message Enable/Disable. Default is disabled.
	ENUM_DUMMY4WORD(SENPHY_CONFIG_ID)
} SENPHY_CONFIG_ID;



extern ER       senphy_setConfig(SENPHY_CONFIG_ID ConfigID, UINT32 uiCfgValue);

#endif
