/**
	NVT IOCFG driver
	To handle NVT IOCFG driver
	@ingroup
	@note
	Copyright Novatek Microelectronics Corp. 2016. All rights reserved.

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License version 2 as
	published by the Free Software Foundation.
*/
#ifndef _IOCFG_INT_H
#define _IOCFG_INT_H

#include <asm/arch/rcw_macro.h>
#include <asm/arch/IOAddress.h>

#define GPIO_GETREG(ofs)	INW(IOADDR_GPIO_REG_BASE+(ofs))
#define GPIO_SETREG(ofs, value)	OUTW(IOADDR_GPIO_REG_BASE +(ofs),(value))
#define PAD_GETREG(ofs)		INW(IOADDR_PAD_REG_BASE+(ofs))
#define PAD_SETREG(ofs, value)	OUTW(IOADDR_PAD_REG_BASE +(ofs),(value))

#define GPIO_SET_OUTPUT_HI      0x1
#define PAD_PUPD0_REG_OFS	0x00
#define GPIO_STRG_DIR_REG_OFS	0x20
#define GPIO_STRG_SET0_REG_OFS	0x40
#define GPIO_STRG_CLR0_REG_OFS	0x60
#define	PAD_C_GPIO_BASE 0
#define	PAD_S_GPIO_BASE 96
#define	PAD_P_GPIO_BASE	128
#define	PAD_L_GPIO_BASE	224
#define	PAD_D_GPIO_BASE	288

typedef enum {
	GPIO_DIR_INPUT =            0,      ///< GPIO is input direction
	GPIO_DIR_OUTPUT =           1,      ///< GPIO is output direction

	ENUM_DUMMY4WORD(GPIO_DIR)
} GPIO_DIR;

typedef enum {
	PAD_NONE         =          0x00,      ///< none of pull up/down
	PAD_PULLDOWN     =          0x01,      ///< pull down
	PAD_PULLUP       =          0x02,      ///< pull up
	PAD_KEEPER       =          0x03,      ///< keeper

	ENUM_DUMMY4WORD(PAD_PULL)
} PAD_PULL;

struct pad_pullupdown_pin {
	u16 pin;
	u8 pup;
	u8 bit_ofs;
};

#define PAD_PIN_NOT_EXIST       (64)                        // For backward compatible
//@{
typedef enum {

	//C_GPIO group
	PAD_PIN_CGPIO0     =     (PAD_C_GPIO_BASE + 0) ,    ///< C_GPIO_0
	PAD_PIN_CGPIO1     =     (PAD_C_GPIO_BASE + 2) ,    ///< C_GPIO_1
	PAD_PIN_CGPIO2     =     (PAD_C_GPIO_BASE + 4) ,   	///< C_GPIO_2
	PAD_PIN_CGPIO3     =     (PAD_C_GPIO_BASE + 6) ,    ///< C_GPIO_3
	PAD_PIN_CGPIO4     =     (PAD_C_GPIO_BASE + 8) ,    ///< C_GPIO_4
	PAD_PIN_CGPIO5     =     (PAD_C_GPIO_BASE + 10),    ///< C_GPIO_5
	PAD_PIN_CGPIO6     =     (PAD_C_GPIO_BASE + 12),    ///< C_GPIO_6
	PAD_PIN_CGPIO7     =     (PAD_C_GPIO_BASE + 14),    ///< C_GPIO_7
	PAD_PIN_CGPIO8     =     (PAD_C_GPIO_BASE + 16),    ///< C_GPIO_8
	PAD_PIN_CGPIO9     =     (PAD_C_GPIO_BASE + 18),    ///< C_GPIO_9
	PAD_PIN_CGPIO10    =     (PAD_C_GPIO_BASE + 20),    ///< C_GPIO_10
	PAD_PIN_CGPIO11    =     (PAD_C_GPIO_BASE + 22),    ///< C_GPIO_11
	PAD_PIN_CGPIO12    =     (PAD_C_GPIO_BASE + 24),    ///< C_GPIO_12
	PAD_PIN_CGPIO13    =     (PAD_C_GPIO_BASE + 26),    ///< C_GPIO_13
	PAD_PIN_CGPIO14    =     (PAD_C_GPIO_BASE + 28),    ///< C_GPIO_14
	PAD_PIN_CGPIO15    =     (PAD_C_GPIO_BASE + 30),    ///< C_GPIO_15
	PAD_PIN_CGPIO16    =     (PAD_C_GPIO_BASE + 32),    ///< C_GPIO_16
	PAD_PIN_CGPIO17    =     (PAD_C_GPIO_BASE + 34),    ///< C_GPIO_17
	PAD_PIN_CGPIO18    =     (PAD_C_GPIO_BASE + 36),    ///< C_GPIO_18
	PAD_PIN_CGPIO19    =     (PAD_C_GPIO_BASE + 38),    ///< C_GPIO_19
	PAD_PIN_CGPIO20    =     (PAD_C_GPIO_BASE + 40),    ///< C_GPIO_20
	PAD_PIN_CGPIO21    =     (PAD_C_GPIO_BASE + 42),    ///< C_GPIO_21
	PAD_PIN_CGPIO22    =     (PAD_C_GPIO_BASE + 44),    ///< C_GPIO_22
	PAD_PIN_CGPIO23    =     (PAD_C_GPIO_BASE + 46),    ///< C_GPIO_23

	//S_GPIO group
	PAD_PIN_SGPIO0      =    (PAD_S_GPIO_BASE + 0) ,    ///< S_GPIO_0
	PAD_PIN_SGPIO1      =    (PAD_S_GPIO_BASE + 2) ,    ///< S_GPIO_1
	PAD_PIN_SGPIO2      =    (PAD_S_GPIO_BASE + 4) ,    ///< S_GPIO_2
	PAD_PIN_SGPIO3      =    (PAD_S_GPIO_BASE + 6) ,    ///< S_GPIO_3
	PAD_PIN_SGPIO4      =    (PAD_S_GPIO_BASE + 8) ,    ///< S_GPIO_4
	PAD_PIN_SGPIO5      =    (PAD_S_GPIO_BASE + 10),    ///< S_GPIO_5
	PAD_PIN_SGPIO6      =    (PAD_S_GPIO_BASE + 12),    ///< S_GPIO_6
	PAD_PIN_SGPIO7      =    (PAD_S_GPIO_BASE + 14),    ///< S_GPIO_7
	PAD_PIN_SGPIO8      =    (PAD_S_GPIO_BASE + 16),    ///< S_GPIO_8
	PAD_PIN_SGPIO9      =    (PAD_S_GPIO_BASE + 18),    ///< S_GPIO_9
	PAD_PIN_SGPIO10     =    (PAD_S_GPIO_BASE + 20),    ///< S_GPIO_10
	PAD_PIN_SGPIO11     =    (PAD_S_GPIO_BASE + 22),    ///< S_GPIO_11
	PAD_PIN_SGPIO12     =    (PAD_S_GPIO_BASE + 24),    ///< S_GPIO_12
	PAD_PIN_SGPIO13     =    (PAD_S_GPIO_BASE + 26),    ///< S_GPIO_13
	PAD_PIN_SGPIO14     =    (PAD_S_GPIO_BASE + 28),    ///< S_GPIO_14

	//P_GPIO group
	PAD_PIN_PGPIO0      =    (PAD_P_GPIO_BASE + 0) ,    ///< P_GPIO_0
	PAD_PIN_PGPIO1      =    (PAD_P_GPIO_BASE + 2) ,    ///< P_GPIO_1
	PAD_PIN_PGPIO2      =    (PAD_P_GPIO_BASE + 4) ,    ///< P_GPIO_2
	PAD_PIN_PGPIO3      =    (PAD_P_GPIO_BASE + 6) ,    ///< P_GPIO_3
	PAD_PIN_PGPIO4      =    (PAD_P_GPIO_BASE + 8) ,    ///< P_GPIO_4
	PAD_PIN_PGPIO5      =    (PAD_P_GPIO_BASE + 10) ,   ///< P_GPIO_5
	PAD_PIN_PGPIO6      =    (PAD_P_GPIO_BASE + 12) ,   ///< P_GPIO_6
	PAD_PIN_PGPIO7      =    (PAD_P_GPIO_BASE + 14) ,   ///< P_GPIO_7
	PAD_PIN_PGPIO8      =    (PAD_P_GPIO_BASE + 16) ,   ///< P_GPIO_8
	PAD_PIN_PGPIO9      =    (PAD_P_GPIO_BASE + 18) ,   ///< P_GPIO_9
	PAD_PIN_PGPIO10     =    (PAD_P_GPIO_BASE + 20),    ///< P_GPIO_10
	PAD_PIN_PGPIO11     =    (PAD_P_GPIO_BASE + 22),    ///< P_GPIO_11
	PAD_PIN_PGPIO12     =    (PAD_P_GPIO_BASE + 24),    ///< P_GPIO_12
	PAD_PIN_PGPIO13     =    (PAD_P_GPIO_BASE + 26),    ///< P_GPIO_13
	PAD_PIN_PGPIO14     =    (PAD_P_GPIO_BASE + 28),    ///< P_GPIO_14
	PAD_PIN_PGPIO15     =    (PAD_P_GPIO_BASE + 30),    ///< P_GPIO_15
	PAD_PIN_PGPIO16     =    (PAD_P_GPIO_BASE + 32),    ///< P_GPIO_16
	PAD_PIN_PGPIO17     =    (PAD_P_GPIO_BASE + 34),    ///< P_GPIO_17
	PAD_PIN_PGPIO18     =    (PAD_P_GPIO_BASE + 36),    ///< P_GPIO_18
	PAD_PIN_PGPIO19     =    (PAD_P_GPIO_BASE + 38),    ///< P_GPIO_19
	PAD_PIN_PGPIO20     =    (PAD_P_GPIO_BASE + 40),    ///< P_GPIO_20
	PAD_PIN_PGPIO21     =    (PAD_P_GPIO_BASE + 42),    ///< P_GPIO_21
	PAD_PIN_PGPIO22     =    (PAD_P_GPIO_BASE + 44),    ///< P_GPIO_22
	PAD_PIN_PGPIO23     =    (PAD_P_GPIO_BASE + 46),    ///< P_GPIO_23
	PAD_PIN_PGPIO24     =    (PAD_P_GPIO_BASE + 48),    ///< P_GPIO_24
	PAD_PIN_PGPIO25     =    (PAD_P_GPIO_BASE + 50),    ///< P_GPIO_25
	PAD_PIN_PGPIO26     =    (PAD_P_GPIO_BASE + 52),    ///< P_GPIO_26
	PAD_PIN_PGPIO27     =    (PAD_P_GPIO_BASE + 54),    ///< P_GPIO_27
	PAD_PIN_PGPIO28     =    (PAD_P_GPIO_BASE + 56),    ///< P_GPIO_28

	//L_GPIO group
	PAD_PIN_LGPIO0      =    (PAD_L_GPIO_BASE + 0) ,     ///< L_GPIO_0
	PAD_PIN_LGPIO1      =    (PAD_L_GPIO_BASE + 2) ,     ///< L_GPIO_1
	PAD_PIN_LGPIO2      =    (PAD_L_GPIO_BASE + 4) ,     ///< L_GPIO_2
	PAD_PIN_LGPIO3      =    (PAD_L_GPIO_BASE + 6) ,     ///< L_GPIO_3
	PAD_PIN_LGPIO4      =    (PAD_L_GPIO_BASE + 8) ,     ///< L_GPIO_4
	PAD_PIN_LGPIO5      =    (PAD_L_GPIO_BASE + 10),     ///< L_GPIO_5
	PAD_PIN_LGPIO6      =    (PAD_L_GPIO_BASE + 12),     ///< L_GPIO_6
	PAD_PIN_LGPIO7      =    (PAD_L_GPIO_BASE + 14),     ///< L_GPIO_7
	PAD_PIN_LGPIO8      =    (PAD_L_GPIO_BASE + 16),     ///< L_GPIO_8
	PAD_PIN_LGPIO9      =    (PAD_L_GPIO_BASE + 18),     ///< L_GPIO_9
	PAD_PIN_LGPIO10     =    (PAD_L_GPIO_BASE + 20),     ///< L_GPIO_10
	PAD_PIN_LGPIO11     =    (PAD_L_GPIO_BASE + 22),     ///< L_GPIO_11
	PAD_PIN_LGPIO12     =    (PAD_L_GPIO_BASE + 24),     ///< L_GPIO_12
	PAD_PIN_LGPIO13     =    (PAD_L_GPIO_BASE + 26),     ///< L_GPIO_13
	PAD_PIN_LGPIO14     =    (PAD_L_GPIO_BASE + 28),     ///< L_GPIO_14
	PAD_PIN_LGPIO15     =    (PAD_L_GPIO_BASE + 30),     ///< L_GPIO_15
	PAD_PIN_LGPIO16     =    (PAD_L_GPIO_BASE + 32),     ///< L_GPIO_16
	PAD_PIN_LGPIO17     =    (PAD_L_GPIO_BASE + 34),     ///< L_GPIO_17
	PAD_PIN_LGPIO18     =    (PAD_L_GPIO_BASE + 36),     ///< L_GPIO_18
	PAD_PIN_LGPIO19     =    (PAD_L_GPIO_BASE + 38),     ///< L_GPIO_19
	PAD_PIN_LGPIO20     =    (PAD_L_GPIO_BASE + 40),     ///< L_GPIO_20
	PAD_PIN_LGPIO21     =    (PAD_L_GPIO_BASE + 42),     ///< L_GPIO_21
	PAD_PIN_LGPIO22     =    (PAD_L_GPIO_BASE + 44),     ///< L_GPIO_22
	PAD_PIN_LGPIO23     =    (PAD_L_GPIO_BASE + 46),     ///< L_GPIO_23

	//D_GPIO group
	PAD_PIN_DGPIO0      =    (PAD_D_GPIO_BASE + 0) ,     ///< D_GPIO_0
	PAD_PIN_DGPIO1      =    (PAD_D_GPIO_BASE + 2) ,     ///< D_GPIO_1
	PAD_PIN_DGPIO2      =    (PAD_D_GPIO_BASE + 4) ,     ///< D_GPIO_2
	PAD_PIN_DGPIO3      =    (PAD_D_GPIO_BASE + 6) ,     ///< D_GPIO_3
	PAD_PIN_DGPIO4      =    (PAD_D_GPIO_BASE + 8) ,     ///< D_GPIO_4
	PAD_PIN_DGPIO5      =    (PAD_D_GPIO_BASE + 10),     ///< D_GPIO_5
	PAD_PIN_DGPIO6      =    (PAD_D_GPIO_BASE + 12),     ///< D_GPIO_6
	PAD_PIN_DGPIO7      =    (PAD_D_GPIO_BASE + 14),     ///< D_GPIO_7
	PAD_PIN_SYSRST      =    (PAD_D_GPIO_BASE + 16),     ///< SYS_RST

	PAD_PIN_MAX 		= 	 PAD_PIN_SYSRST,
	ENUM_DUMMY4WORD(PAD_PIN)
}PAD_PIN;

#endif