
#ifndef _GSENSOR_SC7A20_H
#define _GSENSOR_SC7A20_H
#include "Type.h"
#include "IOInit.h"
#include "pad.h"
#include "top.h"
#include "gpio.h"
#include "adc.h"
#include "pwm.h"

// SC7A20 I2C Addr
#define SC7A20_I2C_WR_ADDR		0x30
#define SC7A20_I2C_RD_ADDR		0x31

// BMA254 Reg Table
#define CHIP_ID_REG				0x0F
#define SW_RST_REG				0x14
#define OUT_X_L_REG				0x28
#define OUT_X_H_REG				0x29
#define OUT_Y_L_REG				0x2A
#define OUT_Y_H_REG				0x2B
#define OUT_Z_L_REG				0x2C
#define OUT_Z_H_REG				0x2D
#define PMU_RANGE_REG			0x0F
#define PMU_BW_REG				0x10

// For CHIP_ID_REG
#define CHIP_ID					0x11

// For SW_RST_REG
#define SW_RST					0xB6

// For PMU_RANGE_REG
#define PMU_RANGE_2G			0x03		/// +/- 2G
#define PMU_RANGE_4G			0x05		/// +/- 4G
#define PMU_RANGE_8G			0x08		/// +/- 8G
#define PMU_RANGE_16G			0x0C		/// +/-16G


// For PMU_BW_REG
#define PMU_BW_7_81				0x08		///    7.81 Hz
#define PMU_BW_15_63			0x09		///   15.63 Hz
#define PMU_BW_31_25			0x0A		///   31.25 Hz
#define PMU_BW_62_5				0x0B		///   62.5  Hz
#define PMU_BW_125				0x0C		///  125    Hz
#define PMU_BW_250				0x0D		///  250    Hz
#define PMU_BW_500				0x0E		///  500    Hz
#define PMU_BW_1000				0x0F		/// 1000    Hz

#endif

