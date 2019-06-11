/**
    Header file for PMC module

    This file is the header file that define the API and data type for PMC module.

    @file       pmc.h
    @ingroup    mIDrvSys_PMC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _PMC_H
#define _PMC_H

#include "Driver.h"

#define PMC_MODULE_PD1      0   ///< Power domain 1 starting ID
#define PMC_MODULE_PD2      32  ///< Power domain 2 starting ID
#define PMC_MODULE_PD3      64  ///< Power domain 3 starting ID
#define PMC_MODULE_PD_NONE  96  ///< Power domain none

/**
    @addtogroup mIDrvSys_PMC
*/
//@{

/**
    PMC module ID

    PMC module ID.
*/
typedef enum {
	PMC_MODULE_DSP          = PMC_MODULE_PD1,       ///< DSP

	PMC_MODULE_CPU2         = PMC_MODULE_PD_NONE,   ///< CPU2
	PMC_MODULE_SIE          = PMC_MODULE_PD_NONE,   ///< SIE
	PMC_MODULE_SIE2         = PMC_MODULE_PD_NONE,   ///< SIE2
	PMC_MODULE_TGE          = PMC_MODULE_PD_NONE,   ///< TGE
	PMC_MODULE_IPE          = PMC_MODULE_PD_NONE,   ///< IPE
	PMC_MODULE_DIS          = PMC_MODULE_PD_NONE,   ///< DIS
	PMC_MODULE_IME          = PMC_MODULE_PD_NONE,   ///< IME
	PMC_MODULE_FDE          = PMC_MODULE_PD_NONE,   ///< FDE
	PMC_MODULE_ISE          = PMC_MODULE_PD_NONE,   ///< ISE
	PMC_MODULE_IFE          = PMC_MODULE_PD_NONE,   ///< IFE
	PMC_MODULE_DCE          = PMC_MODULE_PD_NONE,   ///< DEC
	PMC_MODULE_IFE2         = PMC_MODULE_PD_NONE,   ///< IFE2
	PMC_MODULE_TV           = PMC_MODULE_PD_NONE,   ///< TV
	PMC_MODULE_HDMI         = PMC_MODULE_PD_NONE,   ///< HDMI
	PMC_MODULE_AFFINE       = PMC_MODULE_PD_NONE,   ///< Affine
	PMC_MODULE_JPEG         = PMC_MODULE_PD_NONE,   ///< JPEG
	PMC_MODULE_GRAPHIC      = PMC_MODULE_PD_NONE,   ///< Graphic
	PMC_MODULE_GRAPHIC2     = PMC_MODULE_PD_NONE,   ///< Graphic2
	PMC_MODULE_I2C3         = PMC_MODULE_PD_NONE,   ///< I2C3

	PMC_MODULE_MIPI_CSI     = PMC_MODULE_PD_NONE,   ///< MIPI - CSI
	PMC_MODULE_MIPI_CSI2    = PMC_MODULE_PD_NONE,   ///< MIPI - CSI2
	PMC_MODULE_RDE          = PMC_MODULE_PD_NONE,   ///< RDE
	PMC_MODULE_DRE          = PMC_MODULE_PD_NONE,   ///< DRE
	PMC_MODULE_VX1          = PMC_MODULE_PD_NONE,   ///< VX1
	PMC_MODULE_SIE3         = PMC_MODULE_PD_NONE,   ///< SIE3
	PMC_MODULE_SIE4         = PMC_MODULE_PD_NONE,   ///< SIE4
	PMC_MODULE_RHE          = PMC_MODULE_PD_NONE,   ///< RHE
	PMC_MODULE_SL           = PMC_MODULE_PD_NONE,   ///< SUBLINK
	PMC_MODULE_SL_REG       = PMC_MODULE_PD_NONE,   ///< SUBLINK REG

	PMC_MODULE_H264         = PMC_MODULE_PD_NONE,   ///< H264
	PMC_MODULE_JPEG2        = PMC_MODULE_PD_NONE,   ///< JPEG2
	ENUM_DUMMY4WORD(PMC_MODULE)
} PMC_MODULE, *PPMC_MODULE;

#define PMC_MODULE_LVDS     PMC_MODULE_MIPI_CSI   ///< LVDS / HiSPi
#define PMC_MODULE_LVDS2    PMC_MODULE_MIPI_CSI2  ///< LVDS2 / HiSPi2


/**
    PMC module status

    PMC module status.
*/
typedef enum {
	PMC_PWRSTS_OFF,         ///< Power is OFF
	PMC_PWRSTS_ON,          ///< Power is ON

	ENUM_DUMMY4WORD(PMC_PWRSTS)
} PMC_PWRSTS, *PPMC_PWRSTS;

// PMC public APIs
extern PMC_PWRSTS  pmc_getPWRStatus(PMC_MODULE Module);

//@}

#endif
