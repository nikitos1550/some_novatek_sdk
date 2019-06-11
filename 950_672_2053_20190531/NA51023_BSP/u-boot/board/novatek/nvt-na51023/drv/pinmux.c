/*
	Pinmux module driver.

	This file is the driver of Piumux module.

	@file		pinmux.c
	@ingroup
	@note		Nothing.

	Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#include <common.h>
#include <asm/io.h>
#include "pinmux_int.h"
#include <modelext/top.h>


#include <asm/arch/rcw_macro.h>
#ifdef IPC_ENABLE
#include <asm/arch/nvt_ipc_common.h>
#include "../cc/cc.h"
#define loc_multiCores()    {loc_cpu();cc_hwLockResource(CC_CORE_CA53_CORE2,CC_RESOURCE_TOP);}
#define unl_multiCores()    {cc_hwUnlockResource(CC_CORE_CA53_CORE2,CC_RESOURCE_TOP);unl_cpu();}
#else
#define loc_multiCores()
#define unl_multiCores()
#endif
#include <asm/arch/nvt_common.h>

extern int pwm_enable(int pwm_id);
extern int pwm_config(int pwm_id, int duty_ns, int period_ns);

static uint32_t mipi_lvds_sensor[8] = {0, 0, 0, 0, 0, 0, 0, 0};
typedef ER (*PINMUX_CONFIG_HDL)(uint32_t);
static uint32_t mi_display = 0;

static ER pinmux_config_sdio(uint32_t config);
static ER pinmux_config_sdio2(uint32_t config);
static ER pinmux_config_sdio3(uint32_t config);
static ER pinmux_config_spi(uint32_t config);
static ER pinmux_config_nand(uint32_t config);
static ER pinmux_config_sensor(uint32_t config);
static ER pinmux_config_sensor2(uint32_t config);
static ER pinmux_config_mipi_lvds(uint32_t config);
static ER pinmux_config_i2c(uint32_t config);
static ER pinmux_config_sif(uint32_t config);
static ER pinmux_config_uart(uint32_t config);
static ER pinmux_config_remote(uint32_t config);
static ER pinmux_config_pwm(uint32_t config);
static ER pinmux_config_audio(uint32_t config);
static ER pinmux_config_lcd(uint32_t config);
static ER pinmux_config_tv(uint32_t config);
static ER pinmux_config_eth(uint32_t config);

union T_TOP_REG1 top_reg1;
union T_TOP_REG2 top_reg2;
union T_TOP_REG3 top_reg3;
union T_TOP_REG4 top_reg4;
union T_TOP_REG5 top_reg5;
union T_TOP_REG6 top_reg6;
union T_TOP_REG7 top_reg7;
union T_TOP_REG8 top_reg8;
union T_TOP_REG9 top_reg9;
union T_TOP_REGCGPIO0 top_reg_cgpio0;
union T_TOP_REGPGPIO0 top_reg_pgpio0;
union T_TOP_REGSGPIO0 top_reg_sgpio0;
union T_TOP_REGLGPIO0 top_reg_lgpio0;
union T_TOP_REGDGPIO0 top_reg_dgpio0;
union T_TOP_REGHGPI0  top_reg_hgpi0;

static uint32_t disp_pinmux_config[] =
{
    PINMUX_DISPMUX_SEL_NONE,            // LCD
    PINMUX_DISPMUX_SEL_NONE,            // LCD2
    PINMUX_TV_HDMI_CFG_NORMAL,          // TV
    PINMUX_TV_HDMI_CFG_NORMAL           // HDMI
};

static PINMUX_CONFIG_HDL pinmux_config_hdl[] =
{
    pinmux_config_sdio,
    pinmux_config_sdio2,
    pinmux_config_sdio3,
    pinmux_config_nand,
    pinmux_config_sensor,
    pinmux_config_sensor2,
    pinmux_config_mipi_lvds,
    pinmux_config_i2c,
    pinmux_config_sif,
    pinmux_config_uart,
    pinmux_config_spi,
    pinmux_config_remote,
    pinmux_config_pwm,
    pinmux_config_audio,
    pinmux_config_lcd,
    pinmux_config_tv,
    pinmux_config_eth
};

/**
	Configure pinmux controller

	Configure pinmux controller by upper layer

	@param[in] top_config	Configuration table
	@return void
*/

ER pinmux_init(PIN_GROUP_CONFIG *top_config)
{
	uint32_t i;
	ER err;
	union T_TOP_REG0 local_top_reg0;
	union T_TOP_REG8 local_top_reg8;

	// Assume all PINMUX is GPIO
	top_reg1.Reg = 0;
	top_reg2.Reg = 0;
	top_reg3.Reg = 0;
	top_reg4.Reg = 0;
	top_reg5.Reg = 0;
	top_reg6.Reg = 0;
	top_reg7.Reg = 0;
	top_reg8.Reg = 0x01;
	top_reg9.Reg = 0;
	top_reg_cgpio0.Reg = 0xFFFFFFFF;
	top_reg_pgpio0.Reg = 0xFFFFFFFF;
	top_reg_sgpio0.Reg = 0xFFFFFFFF;
	top_reg_lgpio0.Reg = 0xFFFFFFFF;
	top_reg_hgpi0.Reg = 0xFFFFFFFF;
	local_top_reg0.Reg = TOP_GETREG(TOP_REG0_OFS);
	if (local_top_reg0.Bit.EJTAG_SEL) {
		top_reg1.Reg = 0x100000;
		top_reg_dgpio0.Reg = 0x00000083;
	} else
		top_reg_dgpio0.Reg = 0x000000FF;

	mi_display = 0;
	mipi_lvds_sensor[0] = mipi_lvds_sensor[1] = 0;

	for (i=0; i<=PIN_FUNC_ETH; i++) {
		if (top_config[i].pinFunction != i) {
			printf("top_config[%d].pinFunction context error\r\n", i);
			return E_CTX;
		}

		err = pinmux_config_hdl[i](top_config[i].config);
		if (err != E_OK) {
			printf("top_config[%d].config config error\r\n", i);
			return err;
		}
	}

	// keep ram/rom mapping
	local_top_reg8.Reg = TOP_GETREG(TOP_REG8_OFS);
	top_reg8.Bit.MIPS_APB_CH_EN = local_top_reg8.Bit.MIPS_APB_CH_EN;
	top_reg8.Bit.DSP_IOP_CH_EN = local_top_reg8.Bit.DSP_IOP_CH_EN;


	TOP_SETREG(TOP_REG1_OFS, top_reg1.Reg);
	TOP_SETREG(TOP_REG2_OFS, top_reg2.Reg);
	TOP_SETREG(TOP_REG3_OFS, top_reg3.Reg);
	TOP_SETREG(TOP_REG4_OFS, top_reg4.Reg);
	TOP_SETREG(TOP_REG5_OFS, top_reg5.Reg);
	TOP_SETREG(TOP_REG6_OFS, top_reg6.Reg);
	TOP_SETREG(TOP_REG7_OFS, top_reg7.Reg);
	TOP_SETREG(TOP_REG8_OFS, top_reg8.Reg);
	TOP_SETREG(TOP_REG9_OFS, top_reg9.Reg);

	TOP_SETREG(TOP_REGCGPIO0_OFS, top_reg_cgpio0.Reg);
	TOP_SETREG(TOP_REGPGPIO0_OFS, top_reg_pgpio0.Reg);
	TOP_SETREG(TOP_REGSGPIO0_OFS, top_reg_sgpio0.Reg);
	TOP_SETREG(TOP_REGLGPIO0_OFS, top_reg_lgpio0.Reg);
	TOP_SETREG(TOP_REGDGPIO0_OFS, top_reg_dgpio0.Reg);
	TOP_SETREG(TOP_REGHGPI0_OFS,  top_reg_hgpi0.Reg);

#if defined(_NVT_SDIO2_PAD_POWER_1800_)
	*(u32*) 0xC029001C |= 0x10;
#else
	*(u32*) 0xC029001C &= ~0x10;
#endif

	return E_OK;
}


/*-----------------------------------------------------------------------------*/
/* PINMUX Interface Functions							*/
/*-----------------------------------------------------------------------------*/
static ER pinmux_config_sdio(uint32_t config)
{
    if (config == PIN_SDIO_CFG_NONE)
    {
    }
    else
    {
        if (config & PIN_SDIO_CFG_2ND_PINMUX)
        {
            printf("SDIO does NOT have 2nd pinmux pad\r\n");
            return E_PAR;
        }

        if (config & PIN_SDIO_CFG_8BITS)
        {
            printf("SDIO does NOT support 8 bits\r\n");
            return E_PAR;
        }

        //pinmux conflict checking
        if (top_reg9.Bit.UART2 == UART2_ENUM_2ND_PINMUX)
        {
            printf("SDIO conflict with UART2_2\r\n");
            return E_PAR;
        }

        if (top_reg5.Bit.SPI == SPI_1ST_PINMUX)
        {
            printf("SDIO conflict with SPI_1 \r\n");
            return E_PAR;
        }

        top_reg1.Bit.SDIO_EXIST = SDIO_EXIST_EN;    // SDIO exist

        top_reg_cgpio0.Bit.CGPIO_12 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_13 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_14 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_15 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_16 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_17 = GPIO_ID_EMUM_FUNC;
    }

    return E_OK;
}

static ER pinmux_config_sdio2(uint32_t config)
{
    if (config == PIN_SDIO_CFG_NONE)
    {
    }
    else
    {
        if (config & PIN_SDIO_CFG_8BITS)
        {
            //top_reg1.Bit.SDIO2_BUS_WIDTH = SDIO2_BUS_WIDTH_8BITS;
             printf("SDIO2 does NOT support 8 bits\r\n");
            return E_PAR;
        }

        if (config & PIN_SDIO_CFG_2ND_PINMUX)
        {
            //top_reg1.Bit.SDIO2_MUX_SEL = SDIO2_MUX_2ND;
            printf("SDIO2 does NOT have 2nd pinmux pad\r\n");
            return E_PAR;
        }

        if (top_reg5.Bit.SPI3 == SPI3_2ND_PINMUX)
        {
            printf("SDIO2 conflict with SPI3_2 \r\n");
            return E_OBJ;
        }

	if (top_reg1.Bit.EXTROM_EXIST == EXTROM_EXIST_EN)
        {
            printf("SDIO2 conflict with BMC\r\n");
            return E_OBJ;
        }


        top_reg1.Bit.SDIO2_EXIST = SDIO2_EXIST_EN;  // SD CLK exist

        top_reg_cgpio0.Bit.CGPIO_18 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_19 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_20 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_21 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_22 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_23 = GPIO_ID_EMUM_FUNC;

    }

    return E_OK;
}

static ER pinmux_config_sdio3(uint32_t config)
{
    if (config == PIN_SDIO_CFG_NONE)
    {
    }
    else
    {
        if (config & PIN_SDIO_CFG_2ND_PINMUX)
        {
            printf("SDIO3 does NOT support 2ND_PINMUX \r\n");
            return E_PAR;
        }
        else
        {
            //pinmux conflict checking
            if (top_reg9.Bit.UART4 == UART4_ENUM_2ND_PINMUX)
            {
                printf("SDIO3 conflict with UART4_2\r\n");
                return E_OBJ;
            }

            if (config & PIN_SDIO_CFG_8BITS)
            {
                top_reg1.Bit.SDIO3_BUS_WIDTH = SDIO3_BUS_WIDTH_8BITS;

                top_reg_cgpio0.Bit.CGPIO_4 = GPIO_ID_EMUM_FUNC;
                top_reg_cgpio0.Bit.CGPIO_5 = GPIO_ID_EMUM_FUNC;
                top_reg_cgpio0.Bit.CGPIO_6 = GPIO_ID_EMUM_FUNC;
                top_reg_cgpio0.Bit.CGPIO_7 = GPIO_ID_EMUM_FUNC;
            }

            top_reg_cgpio0.Bit.CGPIO_0 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_1 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_2 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_3 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_8 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_9 = GPIO_ID_EMUM_FUNC;
            top_reg_cgpio0.Bit.CGPIO_10 = GPIO_ID_EMUM_FUNC;
        }

        top_reg1.Bit.SDIO3_EN = SDIO3_EN;  // SD3 pinmux enable

    }

    return E_OK;
}

static ER pinmux_config_nand(uint32_t config)
{
    if (config == PIN_NAND_CFG_NONE)
    {
    }
    //else if (config & (PIN_NAND_CFG_1CS|PIN_NAND_CFG_2CS))
    else if (config & (PIN_NAND_CFG_1CS|PIN_NAND_CFG_2CS | PIN_NAND_CFG_SPI_NAND | PIN_NAND_CFG_SPI_NOR))
    {
        if (top_reg1.Bit.SDIO3_EN == SDIO3_EN)
        {
            printf("NAND conflict with SDIO3_2\r\n");
            return E_OBJ;
        }

        if ((top_reg9.Bit.UART4 == UART4_ENUM_2ND_PINMUX))
        {
            printf("NAND conflict with UART4_2\r\n");
            return E_OBJ;
        }

        if (config & PIN_NAND_CFG_2CS)
        {
            printf("NA51023 not support PIN_NAND_CFG_2CS\r\n");
            return E_OBJ;
        }

		if (config & PIN_NAND_CFG_SPI_NAND) {
			if (config & PIN_NAND_CFG_SPI_NOR) {
				printf("conflict with SPI NOR\r\n");
				return E_OBJ;
			} else {
				top_reg1.Bit.SPI_EXIST = SPI_EXIST_EN;
			}
		} else {
			if (config & PIN_NAND_CFG_SPI_NAND) {
				printf("conflict with SPI NAND\r\n");
				return E_OBJ;
			} else {
				top_reg1.Bit.SPI_EXIST = SPI_EXIST_EN;
			}
		}

        top_reg_cgpio0.Bit.CGPIO_0 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_1 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_2 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_3 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_8 = GPIO_ID_EMUM_FUNC;
        top_reg_cgpio0.Bit.CGPIO_11 = GPIO_ID_EMUM_FUNC;
    }
    else
    {
        printf("invalid config: 0x%x\r\n", config);
        return E_PAR;
    }
    return E_OK;
}

static ER pinmux_config_sensor(uint32_t config)
{
    uint32_t tmp;

    if (config == PIN_SENSOR_CFG_NONE)
    {
    }
    else
    {
        mipi_lvds_sensor[0] = 0;
        tmp = config & (PIN_SENSOR_CFG_8BITS|PIN_SENSOR_CFG_10BITS| \
		PIN_SENSOR_CFG_12BITS|PIN_SENSOR_CFG_MIPI|PIN_SENSOR_CFG_LVDS | \
		PIN_SENSOR_CFG_CCIR8BITS|PIN_SENSOR_CFG_CCIR16BITS|PIN_SENSOR_CFG_12BITS_2ND);
        switch (tmp)
        {
        case PIN_SENSOR_CFG_8BITS:
            top_reg3.Bit.SENSOR = SENSOR_ENUM_12BITS_1ST;
            top_reg_hgpi0.Bit.HSIGPI_2 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_3 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_4 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_5 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_6 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_7 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_8 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_9 = GPIO_ID_EMUM_GPIO;
            top_reg_sgpio0.Bit.SGPIO_1 = GPIO_ID_EMUM_FUNC; //PXCLK
            top_reg_sgpio0.Bit.SGPIO_2 = GPIO_ID_EMUM_FUNC; //VD
            top_reg_sgpio0.Bit.SGPIO_3 = GPIO_ID_EMUM_FUNC; //HD
            break;
        case PIN_SENSOR_CFG_10BITS:
            top_reg3.Bit.SENSOR = SENSOR_ENUM_12BITS_1ST;
            top_reg_hgpi0.Bit.HSIGPI_0 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_1 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_2 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_3 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_4 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_5 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_6 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_7 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_8 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_9 = GPIO_ID_EMUM_GPIO;
            top_reg_sgpio0.Bit.SGPIO_1 = GPIO_ID_EMUM_FUNC; //PXCLK
            top_reg_sgpio0.Bit.SGPIO_2 = GPIO_ID_EMUM_FUNC; //VD
            top_reg_sgpio0.Bit.SGPIO_3 = GPIO_ID_EMUM_FUNC; //HD
            break;
        case PIN_SENSOR_CFG_12BITS:
            top_reg3.Bit.SENSOR = SENSOR_ENUM_12BITS_1ST;
            top_reg_hgpi0.Bit.HSIGPI_0 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_1 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_2 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_3 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_4 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_5 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_6 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_7 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_8 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_9 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_10 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_11 = GPIO_ID_EMUM_GPIO;
            top_reg_sgpio0.Bit.SGPIO_1 = GPIO_ID_EMUM_FUNC; //PXCLK
            top_reg_sgpio0.Bit.SGPIO_2 = GPIO_ID_EMUM_FUNC; //VD
            top_reg_sgpio0.Bit.SGPIO_3 = GPIO_ID_EMUM_FUNC; //HD
            break;
        case PIN_SENSOR_CFG_12BITS_2ND:
            top_reg3.Bit.SENSOR = SENSOR_ENUM_12BITS_2ND;
            top_reg_hgpi0.Bit.HSIGPI_0 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_1 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_2 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_3 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_4 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_5 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_6 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_7 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_8 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_9 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_10 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_11 = GPIO_ID_EMUM_GPIO;
            top_reg_pgpio0.Bit.PGPIO_12 = GPIO_ID_EMUM_FUNC; //PXCLK
            top_reg_sgpio0.Bit.SGPIO_2 = GPIO_ID_EMUM_FUNC; //VD
            top_reg_sgpio0.Bit.SGPIO_3 = GPIO_ID_EMUM_FUNC; //HD
            break;
        case PIN_SENSOR_CFG_CCIR8BITS:
            top_reg3.Bit.SENSOR = SENSOR_ENUM_CCIR8BITS;
            top_reg_hgpi0.Bit.HSIGPI_2 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_3 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_4 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_5 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_6 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_7 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_8 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_9 = GPIO_ID_EMUM_GPIO;
            top_reg_sgpio0.Bit.SGPIO_1 = GPIO_ID_EMUM_FUNC; //PXCLK
            top_reg_sgpio0.Bit.SGPIO_2 = GPIO_ID_EMUM_FUNC; //VD
            top_reg_sgpio0.Bit.SGPIO_3 = GPIO_ID_EMUM_FUNC; //HD
            top_reg_sgpio0.Bit.SGPIO_4 = GPIO_ID_EMUM_FUNC; //Field
            break;
        case PIN_SENSOR_CFG_CCIR16BITS:
            top_reg3.Bit.SENSOR = SENSOR_ENUM_CCIR16BITS;
            //SN_Y0:Y7    CCIR-IN1
            top_reg_hgpi0.Bit.HSIGPI_2 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_3 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_4 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_5 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_6 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_7 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_8 = GPIO_ID_EMUM_GPIO;
            top_reg_hgpi0.Bit.HSIGPI_9 = GPIO_ID_EMUM_GPIO;
            //SN_C0:C7    CCIR-IN2
            top_reg_pgpio0.Bit.PGPIO_0 = GPIO_ID_EMUM_GPIO;
            top_reg_pgpio0.Bit.PGPIO_1 = GPIO_ID_EMUM_GPIO;
            top_reg_pgpio0.Bit.PGPIO_2 = GPIO_ID_EMUM_GPIO;
            top_reg_pgpio0.Bit.PGPIO_3 = GPIO_ID_EMUM_GPIO;
            top_reg_pgpio0.Bit.PGPIO_4 = GPIO_ID_EMUM_GPIO;
            top_reg_pgpio0.Bit.PGPIO_5 = GPIO_ID_EMUM_GPIO;
            top_reg_pgpio0.Bit.PGPIO_6 = GPIO_ID_EMUM_GPIO;
            top_reg_pgpio0.Bit.PGPIO_7 = GPIO_ID_EMUM_GPIO;

            top_reg_sgpio0.Bit.SGPIO_1 = GPIO_ID_EMUM_FUNC; //PXCLK
            top_reg_sgpio0.Bit.SGPIO_2 = GPIO_ID_EMUM_FUNC; //VD
            top_reg_sgpio0.Bit.SGPIO_3 = GPIO_ID_EMUM_FUNC; //HD
            top_reg_sgpio0.Bit.SGPIO_4 = GPIO_ID_EMUM_FUNC; //Field
            break;
        case PIN_SENSOR_CFG_MIPI:
            mipi_lvds_sensor[0] = PIN_SENSOR_CFG_MIPI;
            break;
        case PIN_SENSOR_CFG_LVDS:
            mipi_lvds_sensor[0] = PIN_SENSOR_CFG_LVDS;
            break;
        default:
            printf("no bus width assigned: 0x%x\r\n", config);
            return E_PAR;
        }

        // Don't need check I2C2_2, because VD/HD is checked first
        // When sensor is LVDS/MIPI and LVDS_VDHD is selected
        if ((mipi_lvds_sensor[0] != 0) && (config & PIN_SENSOR_CFG_LVDS_VDHD))
        {
            // Assing LVDS VD/HD when project layer select this config
            //top_reg3.Bit.VD_HD_SEL = VD_HD_SEL_ENUM_LVDS_VDHD;
            top_reg_sgpio0.Bit.SGPIO_2 = GPIO_ID_EMUM_FUNC; //VD
            top_reg_sgpio0.Bit.SGPIO_3 = GPIO_ID_EMUM_FUNC; //HD
        }


        if (config & PIN_SENSOR_CFG_SHUTTER0)
        {
            top_reg_sgpio0.Bit.SGPIO_12 = GPIO_ID_EMUM_FUNC;
            top_reg3.Bit.SHUTTER0_SEL = SHUTTER_ENUM_SHUTTER;
        }

        if (config & PIN_SENSOR_CFG_SHUTTER1)
        {
            top_reg_sgpio0.Bit.SGPIO_13 = GPIO_ID_EMUM_FUNC;
            top_reg3.Bit.SHUTTER1_SEL = SHUTTER_ENUM_SHUTTER;
        }

        if (config & PIN_SENSOR_CFG_MCLK)
        {
            top_reg_sgpio0.Bit.SGPIO_0 = GPIO_ID_EMUM_FUNC;
        }

        if (config & PIN_SENSOR_CFG_MCLK2)
        {
            top_reg_sgpio0.Bit.SGPIO_5 = GPIO_ID_EMUM_FUNC;
            top_reg3.Bit.SEN_MCLK2 = SENMCLK_SEL_ENUM_MCLK;
        }

        if (config & (PIN_SENSOR_CFG_MES0|PIN_SENSOR_CFG_MES0_2ND)) //?? check conflict
        {
            if (config & PIN_SENSOR_CFG_MES0_2ND)
            {
                top_reg_pgpio0.Bit.PGPIO_8 = GPIO_ID_EMUM_FUNC;
                top_reg3.Bit.ME_SHUT0_SEL = MES_SEL_ENUM_2ND_PINMUX;
            }
            else
            {
                top_reg_pgpio0.Bit.PGPIO_0 = GPIO_ID_EMUM_FUNC;
                top_reg3.Bit.ME_SHUT0_SEL = MES_SEL_ENUM_1ST_PINMUX;
            }
        }

        if (config & (PIN_SENSOR_CFG_MES1|PIN_SENSOR_CFG_MES1_2ND)) //?? check conflict
        {
            if (config & PIN_SENSOR_CFG_MES1_2ND)
            {
                top_reg_pgpio0.Bit.PGPIO_9 = GPIO_ID_EMUM_FUNC;
                top_reg3.Bit.ME_SHUT1_SEL = MES_SEL_ENUM_2ND_PINMUX;
            }
            else
            {
                top_reg_pgpio0.Bit.PGPIO_1 = GPIO_ID_EMUM_FUNC;
                top_reg3.Bit.ME_SHUT1_SEL = MES_SEL_ENUM_1ST_PINMUX;
            }
        }

        if (config & (PIN_SENSOR_CFG_MES2|PIN_SENSOR_CFG_MES2_2ND)) //?? check conflict
        {
            if (config & PIN_SENSOR_CFG_MES2_2ND)
            {
                top_reg_pgpio0.Bit.PGPIO_10 = GPIO_ID_EMUM_FUNC;
                top_reg3.Bit.ME2_SHUT0_SEL = MES_SEL_ENUM_2ND_PINMUX;
            }
            else
            {
                top_reg_pgpio0.Bit.PGPIO_2 = GPIO_ID_EMUM_FUNC;
                top_reg3.Bit.ME2_SHUT0_SEL = MES_SEL_ENUM_1ST_PINMUX;
            }
        }

        if (config & (PIN_SENSOR_CFG_MES3|PIN_SENSOR_CFG_MES3_2ND)) //?? check conflict
        {
            if (config & PIN_SENSOR_CFG_MES3_2ND)
            {
                top_reg_pgpio0.Bit.PGPIO_11 = GPIO_ID_EMUM_FUNC;
                top_reg3.Bit.ME2_SHUT1_SEL = MES_SEL_ENUM_2ND_PINMUX;
            }
            else
            {
                top_reg_pgpio0.Bit.PGPIO_3 = GPIO_ID_EMUM_FUNC;
                top_reg3.Bit.ME2_SHUT1_SEL = MES_SEL_ENUM_1ST_PINMUX;
            }
        }

        if (config & PIN_SENSOR_CFG_FLCTR)
        {
            top_reg_pgpio0.Bit.PGPIO_25 = GPIO_ID_EMUM_FUNC;
            top_reg3.Bit.FLCTR = FLCTR_SEL_ENUM_FLCTR;
        }

        if (config & PIN_SENSOR_CFG_STROBE)
        {
            top_reg_sgpio0.Bit.SGPIO_14 = GPIO_ID_EMUM_FUNC;
            top_reg3.Bit.STROBE_SEL = STROBE_ENUM_STROBE;
        }

        if (config & (PIN_SENSOR_CFG_SPCLK|PIN_SENSOR_CFG_SPCLK_2ND))
        {
            if (config & PIN_SENSOR_CFG_SPCLK_2ND)
            {
                if (top_reg7.Bit.PI_CNT3 == PICNT_ENUM_PICNT)
                {
                    printf("SP_CLK_2 conflict with PICNT3_1\r\n");
                    return E_OBJ;
                }
                if (top_reg6.Bit.AUDIO_MCLK == AUDIO_MCLK_MCLK)
                {
                    printf("SP_CLK_2 conflict with AUDIO MCLK\r\n");
                    return E_OBJ;
                }
                top_reg_pgpio0.Bit.PGPIO_19 = GPIO_ID_EMUM_FUNC;
                top_reg3.Bit.SP_CLK = SP_CLK_SEL_ENUM_SP_2_CLK;
            }
            else
            {
                //check PICNT4_1
                if (top_reg6.Bit.ETH == ETH_ID_ENUM_RMII)
                {
                    printf("SP_CLK conflict with ETH RMII\r\n");
                    return E_OBJ;
                }

                top_reg_lgpio0.Bit.LGPIO_20 = GPIO_ID_EMUM_FUNC;
                top_reg3.Bit.SP_CLK = SP_CLK_SEL_ENUM_SPCLK;
            }
        }

        if (config & (PIN_SENSOR_CFG_SP2CLK|PIN_SENSOR_CFG_SP2CLK_2ND))
        {
            if (config & PIN_SENSOR_CFG_SP2CLK_2ND)
            {
                if (top_reg5.Bit.SPI == SPI_2ND_PINMUX)
                {
                    printf("SP_CLK2_2 conflict with SPI_2\r\n");
                    return E_OBJ;
                }
                if (top_reg9.Bit.UART3 == UART3_ENUM_1ST_PINMUX)
                {
                    printf("SP_CLK2_2 conflict with UART3_1\r\n");
                    return E_OBJ;
                }
                if (top_reg5.Bit.SIFCH3 == SIFCH_ENUM_SIF)
                {
                    printf("SP_CLK2_2 conflict with SIF3\r\n");
                    return E_OBJ;
                }
                if (top_reg7.Bit.PWM12 == PWM_ENUM_PWM)
                {
                    printf("SP_CLK2_2 conflict with PWM12\r\n");
                    return E_OBJ;
                }
                top_reg_pgpio0.Bit.PGPIO_15 = GPIO_ID_EMUM_FUNC;
                top_reg6.Bit.SP2_CLK = SP2_CLK_SEL_ENUM_SP2_2_CLK;
            }
            else
            {
                if (top_reg6.Bit.REMOTE == REMOTE_ENUM_REMOTE)
                {
                    printf("SP_CLK2 conflict with REMOTE\r\n");
                    return E_OBJ;
                }

                if (top_reg7.Bit.PI_CNT == PICNT_ENUM_PICNT)
                {
                    printf("SP_CLK2 conflict with PICNT1_1\r\n");
                    return E_OBJ;
                }

                if (top_reg6.Bit.DIGITAL_MIC == DMIC_ENUM_DMIC)
                {
                    printf("SP_CLK2 conflict with DM_CLK\r\n");
                    return E_OBJ;
                }

                top_reg_pgpio0.Bit.PGPIO_24 = GPIO_ID_EMUM_FUNC;
                top_reg6.Bit.SP2_CLK = SP2_CLK_SEL_ENUM_SP2CLK;
            }
        }
        if (config & (PIN_SENSOR_CFG_LOCKN))
        {
            printf("LOCKN conflict with SPI2_1\r\n");
            return E_OBJ;
        }
    }

    return E_OK;
}

static ER pinmux_config_sensor2(uint32_t config)
{
    uint32_t tmp;

    if (config == PIN_SENSOR2_CFG_NONE)
    {
    }
    else
    {
        mipi_lvds_sensor[1] = 0;
        tmp = config & (PIN_SENSOR2_CFG_CCIR8BITS|PIN_SENSOR2_CFG_10BITS| \
		PIN_SENSOR2_CFG_CCIR16BITS|PIN_SENSOR2_CFG_MIPI|PIN_SENSOR2_CFG_LVDS);
        switch (tmp)
        {
        case PIN_SENSOR2_CFG_CCIR8BITS:
            top_reg_pgpio0.Bit.PGPIO_0 = GPIO_ID_EMUM_GPIO;
            top_reg_pgpio0.Bit.PGPIO_1 = GPIO_ID_EMUM_GPIO;
            top_reg_pgpio0.Bit.PGPIO_2 = GPIO_ID_EMUM_GPIO;
            top_reg_pgpio0.Bit.PGPIO_3 = GPIO_ID_EMUM_GPIO;
            top_reg_pgpio0.Bit.PGPIO_4 = GPIO_ID_EMUM_GPIO;
            top_reg_pgpio0.Bit.PGPIO_5 = GPIO_ID_EMUM_GPIO;
            top_reg_pgpio0.Bit.PGPIO_6 = GPIO_ID_EMUM_GPIO;
            top_reg_pgpio0.Bit.PGPIO_7 = GPIO_ID_EMUM_GPIO;

            top_reg_pgpio0.Bit.PGPIO_12 = GPIO_ID_EMUM_FUNC; //SN2_PXCLK
            top_reg_pgpio0.Bit.PGPIO_10 = GPIO_ID_EMUM_FUNC; // SN2_XVS
            top_reg_pgpio0.Bit.PGPIO_11 = GPIO_ID_EMUM_FUNC; // SN2_XHS

            top_reg3.Bit.SENSOR2 = SENSOR2_ENUM_CCIR8BITS;
            break;
        case PIN_SENSOR2_CFG_10BITS:
            if (top_reg3.Bit.SENSOR == SENSOR_ENUM_12BITS_1ST)
            {
                printf("SENSOR2 10 bit conflict with SENSOR 12 bit\r\n");
                return E_OBJ;
            }
            break;
        case PIN_SENSOR2_CFG_CCIR16BITS:
            if (top_reg3.Bit.SENSOR != SENSOR_ENUM_GPIO)
            {
                printf("SENSOR2 CCIR 16 conflict with SENSOR\r\n");
                return E_OBJ;
            }
            break;
        case PIN_SENSOR2_CFG_MIPI:
            mipi_lvds_sensor[1] = PIN_SENSOR2_CFG_MIPI;
            break;
        case PIN_SENSOR2_CFG_LVDS:
            mipi_lvds_sensor[1] = PIN_SENSOR2_CFG_LVDS;
            break;
        default:
            printf("no bus width assigned: 0x%x\r\n", config);
            break;
        }

        if ((mipi_lvds_sensor[1] != 0) && (config & PIN_SENSOR2_CFG_LVDS_VDHD))
        {
            // Assing LVDS VD/HD when project layer select this config
            top_reg_sgpio0.Bit.SGPIO_4 = GPIO_ID_EMUM_FUNC; //SN2_VD
            top_reg_sgpio0.Bit.SGPIO_5 = GPIO_ID_EMUM_FUNC; //SN2_HD
        }

    }

    return E_OK;
}

static ER pinmux_config_mipi_lvds(uint32_t config)
{

    if (config == PIN_MIPI_LVDS_CFG_NONE)
    {
    }
    else
    {
        // check clk lane 0
        if (config & PIN_MIPI_LVDS_CFG_CLK0)
        {
            if (top_reg3.Bit.SENSOR == SENSOR_ENUM_12BITS_1ST)
            {
                printf("MIPI CLK0 conflict with SENSOR 12 or SENSOR2 10 bits\r\n");
                return E_OBJ;
            }

            top_reg_hgpi0.Bit.HSIGPI_4 = GPIO_ID_EMUM_FUNC;
            top_reg_hgpi0.Bit.HSIGPI_5 = GPIO_ID_EMUM_FUNC;
        }

        // check clk lane 1
        if (config & PIN_MIPI_LVDS_CFG_CLK1)
        {
            top_reg_hgpi0.Bit.HSIGPI_10 = GPIO_ID_EMUM_FUNC;
            top_reg_hgpi0.Bit.HSIGPI_11 = GPIO_ID_EMUM_FUNC;
        }

        // check data lane 0
        if (config & PIN_MIPI_LVDS_CFG_DAT0)
        {
            if (top_reg3.Bit.SENSOR == SENSOR_ENUM_12BITS_1ST)
            {
                printf("MIPI D0 conflict with SENSOR 10/12 bits\r\n");
                return E_OBJ;
            }

            top_reg_hgpi0.Bit.HSIGPI_0 = GPIO_ID_EMUM_FUNC;
            top_reg_hgpi0.Bit.HSIGPI_1 = GPIO_ID_EMUM_FUNC;
        }

        if (config & (PIN_MIPI_LVDS_CFG_DAT1|PIN_MIPI_LVDS_CFG_DAT2|PIN_MIPI_LVDS_CFG_DAT3|PIN_MIPI_LVDS_CFG_DAT4))
        {
            if (top_reg3.Bit.SENSOR != SENSOR_ENUM_GPIO)
            {
                printf("MIPI D1~4, CK0 conflict with SENSOR\r\n");
                return E_OBJ;
            }

            // check data lane 1
            if (config & PIN_MIPI_LVDS_CFG_DAT1)
            {

                top_reg_hgpi0.Bit.HSIGPI_2 = GPIO_ID_EMUM_FUNC;
                top_reg_hgpi0.Bit.HSIGPI_3 = GPIO_ID_EMUM_FUNC;
            }

            // check data lane 2
            if (config & PIN_MIPI_LVDS_CFG_DAT2)
            {

                top_reg_hgpi0.Bit.HSIGPI_6 = GPIO_ID_EMUM_FUNC;
                top_reg_hgpi0.Bit.HSIGPI_7 = GPIO_ID_EMUM_FUNC;
            }

            // check data lane 3
            if (config & PIN_MIPI_LVDS_CFG_DAT3)
            {

                top_reg_hgpi0.Bit.HSIGPI_8 = GPIO_ID_EMUM_FUNC;
                top_reg_hgpi0.Bit.HSIGPI_9 = GPIO_ID_EMUM_FUNC;
            }

            // check data lane 4
            if (config & PIN_MIPI_LVDS_CFG_DAT4)
            {
                printf("NA51023 only support 2C4D\r\n");
                return E_OBJ;
            }
        }

        // check data lane 5~9
        if (config & (PIN_MIPI_LVDS_CFG_DAT5|PIN_MIPI_LVDS_CFG_DAT6|PIN_MIPI_LVDS_CFG_DAT7|PIN_MIPI_LVDS_CFG_DAT8|PIN_MIPI_LVDS_CFG_DAT9))
        {
            printf("NA51023 only support 2C4D\r\n");
            return E_OBJ;
        }
    }

    return E_OK;
}

static ER pinmux_config_i2c(uint32_t config)
{
    if (config == PIN_I2C_CFG_NONE)
    {
    }
    else
    {
        if (config & PIN_I2C_CFG_CH1)
        {
            if (top_reg5.Bit.SIFCH0 == SIFCH_ENUM_SIF)
            {
                printf("I2C1 conflict with SIF_CH0\r\n");
                return E_OBJ;
            }
            top_reg_sgpio0.Bit.SGPIO_7 = GPIO_ID_EMUM_FUNC;
            top_reg_sgpio0.Bit.SGPIO_8 = GPIO_ID_EMUM_FUNC;

            top_reg5.Bit.I2C = I2C_ENUM_I2C;
        }

        if (config & PIN_I2C_CFG_CH2)
        {
            if (top_reg5.Bit.SPI2 == SPI2_1ST_PINMUX)
            {
                printf("I2C2 conflict with SPI2_1\r\n");
                return E_OBJ;
            }
            if (top_reg5.Bit.SIFCH1 == SIFCH_ENUM_SIF)
            {
                printf("I2C2 conflict with SIF_CH1\r\n");
                return E_OBJ;
            }

            top_reg_sgpio0.Bit.SGPIO_10 = GPIO_ID_EMUM_FUNC;
            top_reg_sgpio0.Bit.SGPIO_11 = GPIO_ID_EMUM_FUNC;

            top_reg5.Bit.I2C2 = I2C_ENUM_I2C;
        }

        if (config & PIN_I2C_CFG_CH3)
        {
            top_reg_pgpio0.Bit.PGPIO_13 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_14 = GPIO_ID_EMUM_FUNC;

            top_reg5.Bit.I2C3 = I2C_ENUM_I2C;

        }

        if (config & PIN_I2C_CFG_CH2_2ND_PINMUX)
        {
            printf("NA51023 donot support I2C2_2\r\n");
        }
    }

    return E_OK;
}

static ER pinmux_config_sif(uint32_t config)
{
    if (config == PIN_SIF_CFG_NONE)
    {
    }
    else
    {
        if (config & PIN_SIF_CFG_CH0)
        {
            if (top_reg5.Bit.I2C == I2C_ENUM_I2C)
            {
                printf("SIF_CH0 conflict with I2C1\r\n");
                return E_OBJ;
            }

            top_reg_sgpio0.Bit.SGPIO_6 = GPIO_ID_EMUM_FUNC;
            top_reg_sgpio0.Bit.SGPIO_7 = GPIO_ID_EMUM_FUNC;
            top_reg_sgpio0.Bit.SGPIO_8 = GPIO_ID_EMUM_FUNC;

            top_reg5.Bit.SIFCH0 = SIFCH_ENUM_SIF;
        }

        if (config & (PIN_SIF_CFG_CH1|PIN_SIF_CFG_CH1_2ND_PINMUX))
        {
            if (top_reg5.Bit.SPI2 == SPI2_1ST_PINMUX)
            {
                printf("SIF_CH1 conflict with SPI2_1\r\n");
                return E_OBJ;
            }

            if (top_reg5.Bit.I2C2 == I2C_ENUM_I2C)
            {
                printf("SIF_CH1 conflict with I2C2\r\n");
                return E_OBJ;
            }

            if (config & PIN_SIF_CFG_CH1_2ND_PINMUX)
            {
                printf("SIF_CH1 no 2ND PINMUX\r\n");
                return E_OBJ;
            }
            else
            {
                top_reg_sgpio0.Bit.SGPIO_9 = GPIO_ID_EMUM_FUNC;
                top_reg_sgpio0.Bit.SGPIO_10 = GPIO_ID_EMUM_FUNC;
                top_reg_sgpio0.Bit.SGPIO_11 = GPIO_ID_EMUM_FUNC;
            }

            top_reg5.Bit.SIFCH1 = SIFCH_ENUM_SIF;
        }

        if (config & PIN_SIF_CFG_CH2)
        {
            top_reg_lgpio0.Bit.LGPIO_21 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_22 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_23 = GPIO_ID_EMUM_FUNC;

            top_reg5.Bit.SIFCH2 = SIFCH_ENUM_SIF;
        }

        if (config & PIN_SIF_CFG_CH3)
        {
            if (top_reg7.Bit.PWM12 == PWM_ENUM_PWM)
            {
                printf("SIF_CH3 conflict with to PWM12: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg7.Bit.PWM13 == PWM_ENUM_PWM)
            {
                printf("SIF_CH3 conflict with to PWM13: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg7.Bit.PWM14 == PWM_ENUM_PWM)
            {
                printf("SIF_CH3 conflict with to PWM14: 0x%x\r\n", config);
                return E_OBJ;
            }

            if (top_reg5.Bit.SPI == SPI_2ND_PINMUX)
            {
                printf("SIF_CH3 conflict with SPI_2\r\n");
                return E_OBJ;
            }

            if (top_reg9.Bit.UART3 == UART3_ENUM_1ST_PINMUX)
            {
                printf("SIF_CH3 conflict with UART3_1\r\n");
                return E_OBJ;
            }

            if (top_reg6.Bit.SP2_CLK == SP2_CLK_SEL_ENUM_SP2_2_CLK)
            {
                printf("SIF_CH3 conflict with SPCLK_2_2\r\n");
                return E_OBJ;
            }

            top_reg_pgpio0.Bit.PGPIO_15 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_16 = GPIO_ID_EMUM_FUNC;
            top_reg_pgpio0.Bit.PGPIO_17 = GPIO_ID_EMUM_FUNC;

            top_reg5.Bit.SIFCH3 = SIFCH_ENUM_SIF;
        }
    }
    return E_OK;
}

static ER pinmux_config_uart(uint32_t config)
{
    if (config == PIN_UART_CFG_NONE)
    {
    }
    else if (config & (PIN_UART_CFG_CH1|PIN_UART_CFG_CH2|PIN_UART_CFG_CH3|PIN_UART_CFG_CH4))
    {
        if (config & (PIN_UART_CFG_CH1|PIN_UART_CFG_CH1_TX))
        {
            top_reg_pgpio0.Bit.PGPIO_26 = GPIO_ID_EMUM_FUNC;

            if (config & (PIN_UART_CFG_CH1))
            {
                top_reg_pgpio0.Bit.PGPIO_27 = GPIO_ID_EMUM_FUNC;
            }
            top_reg9.Bit.UART = UART_ENUM_UART;
        }

        if (config & PIN_UART_CFG_CH2)
        {
            if (config & PIN_UART_CFG_CH2_2ND)
            {
                if (top_reg1.Bit.SDIO_EXIST == SDIO_EXIST_EN)
                {
                    printf("UART2_2 conflict with to SDIO: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (top_reg5.Bit.SPI3 == SPI3_1ST_PINMUX)
                {
                    printf("UART2_2 conflict with to SPI3_1: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (config & PIN_UART_CFG_CH2_CTSRTS)
                {
                    top_reg_cgpio0.Bit.CGPIO_15 = GPIO_ID_EMUM_FUNC;
                    top_reg_cgpio0.Bit.CGPIO_16 = GPIO_ID_EMUM_FUNC;

                    top_reg9.Bit.UART2_CTSRTS = UART_CTSRTS_PINMUX;
                }

                top_reg_cgpio0.Bit.CGPIO_13 = GPIO_ID_EMUM_FUNC;
                top_reg_cgpio0.Bit.CGPIO_14 = GPIO_ID_EMUM_FUNC;

                top_reg9.Bit.UART2 = UART2_ENUM_2ND_PINMUX;
            }
            else
            {
                if (top_reg6.Bit.AUDIO == AUDIO_ENUM_I2S)
                {
                    printf("UART2 conflict with to AUDIO: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (config & PIN_UART_CFG_CH2_CTSRTS)
                {
                    if ((top_reg7.Bit.PI_CNT == PICNT_ENUM_PICNT)||(top_reg7.Bit.PI_CNT2 == PICNT_ENUM_PICNT))
                    {
                        printf("UART2 conflict with to PICNT_1 or PICNT2_1: 0x%x\r\n", config);
                        return E_OBJ;
                    }
                    top_reg_pgpio0.Bit.PGPIO_22 = GPIO_ID_EMUM_FUNC;
                    top_reg_pgpio0.Bit.PGPIO_23 = GPIO_ID_EMUM_FUNC;

                    top_reg9.Bit.UART2_CTSRTS = UART_CTSRTS_PINMUX;
                }

                top_reg_pgpio0.Bit.PGPIO_20 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio0.Bit.PGPIO_21 = GPIO_ID_EMUM_FUNC;

                top_reg9.Bit.UART2 = UART2_ENUM_1ST_PINMUX;

            }
        }

        if (config & PIN_UART_CFG_CH3)
        {
            if (config & PIN_UART_CFG_CH3_2ND)
            {
                if (top_reg7.Bit.PWM8 == PWM_ENUM_PWM)
                {
                    printf("UART3 conflict with to PWM8: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (top_reg7.Bit.PWM9 == PWM_ENUM_PWM)
                {
                    printf("UART3 conflict with to PWM9: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (config & PIN_UART_CFG_CH3_CTSRTS)
                {
                    if (top_reg7.Bit.PWM10 == PWM_ENUM_PWM)
                    {
                        printf("UART3 conflict with to PWM10: 0x%x\r\n", config);
                        return E_OBJ;
                    }
                    if (top_reg7.Bit.PWM11 == PWM_ENUM_PWM)
                    {
                        printf("UART3 conflict with to PWM11: 0x%x\r\n", config);
                        return E_OBJ;
                    }

                    top_reg_pgpio0.Bit.PGPIO_10 = GPIO_ID_EMUM_FUNC;
                    top_reg_pgpio0.Bit.PGPIO_11 = GPIO_ID_EMUM_FUNC;

                    top_reg9.Bit.UART3_CTSRTS = UART_CTSRTS_PINMUX;
                }
                top_reg_pgpio0.Bit.PGPIO_8 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio0.Bit.PGPIO_9 = GPIO_ID_EMUM_FUNC;

                top_reg9.Bit.UART3 = UART3_ENUM_2ND_PINMUX;
            }
            else
            {
                if (top_reg5.Bit.SPI3 == SPI3_1ST_PINMUX)
                {
                    printf("UART3 conflict with to SPI3_1: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if ((top_reg5.Bit.SIFCH2 == SIFCH_ENUM_SIF)||(top_reg5.Bit.SIFCH3 == SIFCH_ENUM_SIF))
                {
                    printf("UART3 conflict with to SIF2 or SIF3: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (top_reg6.Bit.SP2_CLK == SP2_CLK_SEL_ENUM_SP2_2_CLK)
                {
                    printf("UART3 conflict with to SP_CLK2_2: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (config & PIN_UART_CFG_CH3_CTSRTS)
                {
                    if (top_reg6.Bit.ETH != ETH_ID_ENUM_GPIO)
                    {
                        printf("UART3 conflict with to ETH: 0x%x\r\n", config);
                        return E_OBJ;
                    }

                    top_reg_pgpio0.Bit.PGPIO_17 = GPIO_ID_EMUM_FUNC;
                    top_reg_pgpio0.Bit.PGPIO_18 = GPIO_ID_EMUM_FUNC;

                    top_reg9.Bit.UART3_CTSRTS = UART_CTSRTS_PINMUX;
                }

                top_reg_pgpio0.Bit.PGPIO_15 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio0.Bit.PGPIO_16 = GPIO_ID_EMUM_FUNC;

                top_reg9.Bit.UART3 = UART3_ENUM_1ST_PINMUX;

            }
        }

        if (config & PIN_UART_CFG_CH4)
        {
            if (config & PIN_UART_CFG_CH4_2ND)
            {
                if (top_reg1.Bit.SDIO3_EN == SDIO3_EN)
                {
                    printf("UART4_2 conflict with to SDIO3: 0x%x\r\n", config);
                    return E_OBJ;
                }

                if (config & PIN_UART_CFG_CH4_CTSRTS)
                {
                    top_reg_cgpio0.Bit.CGPIO_7 = GPIO_ID_EMUM_FUNC;
                    top_reg_cgpio0.Bit.CGPIO_8 = GPIO_ID_EMUM_FUNC;

                    top_reg9.Bit.UART4_CTSRTS = UART_CTSRTS_PINMUX;
                }

                top_reg_cgpio0.Bit.CGPIO_5 = GPIO_ID_EMUM_FUNC;
                top_reg_cgpio0.Bit.CGPIO_6 = GPIO_ID_EMUM_FUNC;

                top_reg9.Bit.UART4 = UART4_ENUM_2ND_PINMUX;

            }
            else
            {
                if (top_reg7.Bit.PWM4 == PWM_ENUM_PWM)
                {
                    printf("UART4 conflict with to PWM4: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (top_reg7.Bit.PWM5 == PWM_ENUM_PWM)
                {
                    printf("UART4 conflict with to PWM5: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (config & PIN_UART_CFG_CH4_CTSRTS)
                {
                    if (top_reg7.Bit.PWM6 == PWM_ENUM_PWM)
                    {
                        printf("UART4 conflict with to PWM6: 0x%x\r\n", config);
                        return E_OBJ;
                    }
                    if (top_reg7.Bit.PWM7 == PWM_ENUM_PWM)
                    {
                        printf("UART4 conflict with to PWM7: 0x%x\r\n", config);
                        return E_OBJ;
                    }
                    top_reg_pgpio0.Bit.PGPIO_6 = GPIO_ID_EMUM_FUNC;
                    top_reg_pgpio0.Bit.PGPIO_7 = GPIO_ID_EMUM_FUNC;

                    top_reg9.Bit.UART4_CTSRTS = UART_CTSRTS_PINMUX;
                }
                top_reg_pgpio0.Bit.PGPIO_4 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio0.Bit.PGPIO_5 = GPIO_ID_EMUM_FUNC;

                top_reg9.Bit.UART4 = UART4_ENUM_1ST_PINMUX;
            }
        }
    }
    else
    {
        printf("invalid config: 0x%x\r\n", config);
        return E_PAR;
    }

    return E_OK;
}

static ER pinmux_config_spi(uint32_t config)
{
    if (config == PIN_SPI_CFG_NONE)
    {
    }
    else
    {
        if (config & (PIN_SPI_CFG_CH1|PIN_SPI_CFG_CH1_2ND_PINMUX|PIN_SPI_CFG_CH1_2BITS|PIN_SPI_CFG_CH1_4BITS))
        {
            if (config & PIN_SPI_CFG_CH1_2ND_PINMUX)
            {
                if (config & PIN_SPI_CFG_CH1_4BITS)
                {
                    printf("SPI not support 4Bit mode\r\n");
                    return E_OBJ;
                }

                if (top_reg9.Bit.UART3 == UART3_ENUM_1ST_PINMUX)
                {
                    printf("SPI_2 conflict with UART3_1\r\n");
                    return E_OBJ;
                }

                if (top_reg5.Bit.SIFCH3 == SIFCH_ENUM_SIF)
                {
                    printf("SPI_2 conflict with SIF_CH3\r\n");
                    return E_OBJ;
                }

                if (top_reg7.Bit.PWM12 == PWM_ENUM_PWM)
                {
                    printf("SPI_2 conflict with to PWM12: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (top_reg7.Bit.PWM13 == PWM_ENUM_PWM)
                {
                    printf("SPI_2 conflict with to PWM13: 0x%x\r\n", config);
                    return E_OBJ;
                }
                if (top_reg7.Bit.PWM14 == PWM_ENUM_PWM)
                {
                    printf("SPI_2 conflict with to PWM14: 0x%x\r\n", config);
                    return E_OBJ;
                }

                if (top_reg6.Bit.SP2_CLK == SP2_CLK_SEL_ENUM_SP2_2_CLK)
                {
                    printf("SPI_2 conflict with SPCLK_2_2\r\n");
                    return E_OBJ;
                }

                if (config & PIN_SPI_CFG_CH1_2BITS)
                {
                    top_reg_pgpio0.Bit.PGPIO_18 = GPIO_ID_EMUM_FUNC;
                    top_reg5.Bit.SPI_DAT = SPI_DAT_ENUM_2Bit;
                }

                top_reg_pgpio0.Bit.PGPIO_15 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio0.Bit.PGPIO_16 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio0.Bit.PGPIO_17 = GPIO_ID_EMUM_FUNC;

                top_reg5.Bit.SPI = SPI_2ND_PINMUX;

            }
            else
            {
                if (config & PIN_SPI_CFG_CH1_4BITS)
                {
                    printf("SPI not support 4Bit mode\r\n");
                    return E_OBJ;
                }

                if (top_reg1.Bit.SDIO_EXIST == SDIO_EXIST_EN)
                {
                    printf("SPI_1 conflict with SDIO\r\n");
                    return E_OBJ;
                }

                if (top_reg9.Bit.UART2 == UART2_ENUM_2ND_PINMUX)
                {
                    printf("SPI_1 conflict with UART2_2\r\n");
                    return E_OBJ;
                }

                if (config & PIN_SPI_CFG_CH1_2BITS)
                {
                    top_reg_cgpio0.Bit.CGPIO_14 = GPIO_ID_EMUM_FUNC;
                    top_reg5.Bit.SPI_DAT = SPI_DAT_ENUM_2Bit;
                }

                top_reg5.Bit.SPI = SPI_1ST_PINMUX;
                top_reg_cgpio0.Bit.CGPIO_12 = GPIO_ID_EMUM_FUNC;
                top_reg_cgpio0.Bit.CGPIO_13 = GPIO_ID_EMUM_FUNC;
                top_reg_cgpio0.Bit.CGPIO_15 = GPIO_ID_EMUM_FUNC;
            }
        }

        if (config & (PIN_SPI_CFG_CH2|PIN_SPI_CFG_CH2_2ND_PINMUX|PIN_SPI_CFG_CH2_2BITS))
        {
            if (config & PIN_SPI_CFG_CH2_2ND_PINMUX)
            {
                if ((top_reg7.Bit.PWM8 == PWM_ENUM_PWM) ||
                    (top_reg3.Bit.ME_SHUT0_SEL == MES_SEL_ENUM_2ND_PINMUX) )
                {
                    printf("SPI2_2 conflict with PWM8 or ME_2_SHUT0\r\n");
                    return E_OBJ;
                }
                else if ((top_reg7.Bit.PWM9 == PWM_ENUM_PWM) || (top_reg3.Bit.ME_SHUT1_SEL == MES_SEL_ENUM_2ND_PINMUX))
                {
                    printf("SPI2_2 conflict with PWM9 or ME_2_SHUT1\r\n");
                    return E_OBJ;
                }
                else if ((top_reg7.Bit.PWM10 == PWM_ENUM_PWM) ||
                    (top_reg3.Bit.ME2_SHUT0_SEL == MES_SEL_ENUM_2ND_PINMUX) )
                {
                    printf("SPI2_2 conflict with PWM10 or ME2_2_SHUT0\r\n");
                    return E_OBJ;
                }
                else if ((config & PIN_SPI_CFG_CH2_2BITS) &&
                        ((top_reg7.Bit.PWM11 == PWM_ENUM_PWM)||(top_reg3.Bit.ME2_SHUT1_SEL == MES_SEL_ENUM_2ND_PINMUX)))
                {
                    printf("SPI2_2 conflict with PWM11 or ME2_2_SHUT1\r\n");
                    return E_OBJ;
                }

                if (top_reg9.Bit.UART3 == UART3_ENUM_2ND_PINMUX)
                {
                    printf("SPI2_2 conflict with UART3_2\r\n");
                    return E_OBJ;
                }

                if (config & PIN_SPI_CFG_CH2_2BITS)
                {
                    top_reg_pgpio0.Bit.PGPIO_11 = GPIO_ID_EMUM_FUNC;

                    top_reg5.Bit.SPI2_DAT = SPI_DAT_ENUM_2Bit;
                }

                top_reg_pgpio0.Bit.PGPIO_8 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio0.Bit.PGPIO_9 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio0.Bit.PGPIO_10 = GPIO_ID_EMUM_FUNC;

                top_reg5.Bit.SPI2 = SPI2_2ND_PINMUX;
            }
            else
            {
                if (top_reg5.Bit.I2C2 == I2C_ENUM_I2C)
                {
                    printf("SIF_CH1 conflict with I2C2\r\n");
                    return E_OBJ;
                }

                if (top_reg5.Bit.SIFCH1 == SIFCH_ENUM_SIF)
                {
                    printf("I2C2 conflict with SIF_CH1\r\n");
                    return E_OBJ;
                }

                if (config & PIN_SPI_CFG_CH2_2BITS)
                {
                    top_reg_sgpio0.Bit.SGPIO_12 = GPIO_ID_EMUM_FUNC;

                    top_reg5.Bit.SPI2_DAT = SPI_DAT_ENUM_2Bit;
                }

                top_reg_sgpio0.Bit.SGPIO_9 = GPIO_ID_EMUM_FUNC;
                top_reg_sgpio0.Bit.SGPIO_10 = GPIO_ID_EMUM_FUNC;
                top_reg_sgpio0.Bit.SGPIO_11 = GPIO_ID_EMUM_FUNC;

                top_reg5.Bit.SPI2 = SPI2_1ST_PINMUX;

            }
        }

        if (config & (PIN_SPI_CFG_CH3|PIN_SPI_CFG_CH3_2ND_PINMUX|PIN_SPI_CFG_CH3_2BITS|PIN_SPI_CFG_CH3_RDY))
        {
            if (config & PIN_SPI_CFG_CH3_2ND_PINMUX)
            {
                if (top_reg1.Bit.SDIO2_EXIST == SDIO2_EXIST_EN)
                {
                    printf("SPI3_2 conflict with SDIO2\r\n");
                    return E_OBJ;
                }
                else if (top_reg1.Bit.EXTROM_EXIST == EXTROM_EXIST_EN)
                {
                    printf("SPI3_2 conflict with BMC\r\n");
                    return E_OBJ;
                }

                if (config & PIN_SPI_CFG_CH3_RDY)
                {
                    top_reg_cgpio0.Bit.CGPIO_22 = GPIO_ID_EMUM_FUNC;

                    top_reg5.Bit.SPI3_RDY = SPI3_RDY_2ND_PINMUX;
                }

                if (config & PIN_SPI_CFG_CH3_2BITS)
                {
                    top_reg_cgpio0.Bit.CGPIO_20 = GPIO_ID_EMUM_FUNC;

                    top_reg5.Bit.SPI3_DAT = SPI_DAT_ENUM_2Bit;
                }

                top_reg_cgpio0.Bit.CGPIO_18 = GPIO_ID_EMUM_FUNC;
                top_reg_cgpio0.Bit.CGPIO_19 = GPIO_ID_EMUM_FUNC;
                top_reg_cgpio0.Bit.CGPIO_21 = GPIO_ID_EMUM_FUNC;

                top_reg5.Bit.SPI3 = SPI3_2ND_PINMUX;
            }
            else
            {
                if (top_reg9.Bit.UART2 == UART2_ENUM_2ND_PINMUX)
                {
                    printf("SPI3_1 conflict with UART2_2\r\n");
                    return E_OBJ;
                }
                else if (top_reg6.Bit.AUDIO == AUDIO_ENUM_I2S)
                {
                    printf("SPI3_1 conflict with AUDIO\r\n");
                    return E_OBJ;
                }

                if (config & PIN_SPI_CFG_CH3_RDY)
                {
                    if (top_reg7.Bit.PWM15 == PWM_ENUM_PWM)
                    {
                        printf("SPI3_1_RDY conflict with PWM15\r\n");
                        return E_OBJ;
                    }

                    top_reg_pgpio0.Bit.PGPIO_25 = GPIO_ID_EMUM_FUNC;
                    top_reg5.Bit.SPI3_RDY = SPI3_RDY_1ST_PINMUX;
                }

                if (config & PIN_SPI_CFG_CH3_2BITS)
                {
                    if (top_reg7.Bit.PI_CNT2 == PICNT_ENUM_PICNT)
                    {
                        printf("SPI3_1 2bit mode conflict with PICNT2_1\r\n");
                        return E_OBJ;
                    }

                    top_reg_pgpio0.Bit.PGPIO_23 = GPIO_ID_EMUM_FUNC;
                    top_reg5.Bit.SPI3_DAT = SPI_DAT_ENUM_2Bit;
                }

                top_reg_pgpio0.Bit.PGPIO_20 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio0.Bit.PGPIO_21 = GPIO_ID_EMUM_FUNC;
                top_reg_pgpio0.Bit.PGPIO_22 = GPIO_ID_EMUM_FUNC;

                top_reg5.Bit.SPI3 = SPI3_1ST_PINMUX;

            }
        }

        if (config & (PIN_SPI_CFG_CH4|PIN_SPI_CFG_CH4_2ND_PINMUX|PIN_SPI_CFG_CH4_2BITS|PIN_SPI_CFG_CH4_RDY))
        {
            printf("NA51023 donot support SPI4\r\n");
        }

        if (config & (PIN_SPI_CFG_CH5|PIN_SPI_CFG_CH5_2ND_PINMUX|PIN_SPI_CFG_CH5_2BITS))
        {
            printf("NA51023 donot support SPI5\r\n");
        }
    }

    return E_OK;
}

static ER pinmux_config_remote(uint32_t config)
{
    if (config == PIN_REMOTE_CFG_NONE)
    {
    }
    else if (config & PIN_REMOTE_CFG_CH1)
    {
        if (top_reg7.Bit.PI_CNT == PICNT_ENUM_PICNT)
        {
            printf("REMOTE conflict with to CCNT1_1: 0x%x\r\n", config);
            return E_OBJ;
        }
        if (top_reg6.Bit.SP2_CLK == SP2_CLK_SEL_ENUM_SP2CLK)
        {
            printf("REMOTE conflict with to SP_CLK2_1: 0x%x\r\n", config);
            return E_OBJ;
        }

        if (top_reg6.Bit.DIGITAL_MIC == DMIC_ENUM_DMIC)
        {
            printf("REMOTE conflict with to DM_CLK: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_24 = GPIO_ID_EMUM_FUNC;

        top_reg6.Bit.REMOTE = REMOTE_ENUM_REMOTE;
    }
    else
    {
        printf("invalid config: 0x%x\r\n", config);
        return E_PAR;
    }

    return E_OK;
}

static ER pinmux_config_pwm(uint32_t config)
{
    if (config & PIN_PWM_CFG_PWM0)
    {
        if (top_reg3.Bit.ME_SHUT0_SEL == MES_SEL_ENUM_1ST_PINMUX)
        {
            printf("PWM0 conflict with to MES_SHUT0: 0x%x\r\n", config);
            return E_OBJ;
        }
        top_reg_pgpio0.Bit.PGPIO_0 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM0 = PWM_ENUM_PWM;
#ifdef CONFIG_PWM
        pwm_config(0, CONFIG_PWM_DUTY00, CONFIG_PWM_PERIOD00);
        pwm_enable(0);
#endif
    }
    if (config & PIN_PWM_CFG_PWM1)
    {
        if (top_reg3.Bit.ME_SHUT1_SEL == MES_SEL_ENUM_1ST_PINMUX)
        {
            printf("PWM1 conflict with to MES_SHUT1: 0x%x\r\n", config);
            return E_OBJ;
        }
        top_reg_pgpio0.Bit.PGPIO_1 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM1 = PWM_ENUM_PWM;
#ifdef CONFIG_PWM
        pwm_config(1, CONFIG_PWM_DUTY01, CONFIG_PWM_PERIOD01);
        pwm_enable(1);
#endif
    }
    if (config & PIN_PWM_CFG_PWM2)
    {
        if (top_reg3.Bit.ME2_SHUT0_SEL == MES_SEL_ENUM_1ST_PINMUX)
        {
            printf("PWM2 conflict with to MES2_SHUT0: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_2 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM2 = PWM_ENUM_PWM;
#ifdef CONFIG_PWM
        pwm_config(2, CONFIG_PWM_DUTY02, CONFIG_PWM_PERIOD02);
        pwm_enable(2);
#endif
    }
    if (config & PIN_PWM_CFG_PWM3)
    {
        if (top_reg3.Bit.ME2_SHUT1_SEL == MES_SEL_ENUM_1ST_PINMUX)
        {
            printf("PWM3 conflict with to MES2_SHUT1: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_3 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM3 = PWM_ENUM_PWM;
#ifdef CONFIG_PWM
        pwm_config(3, CONFIG_PWM_DUTY03, CONFIG_PWM_PERIOD03);
        pwm_enable(3);
#endif
    }
    if (config & PIN_PWM_CFG_PWM4)
    {
        if (top_reg9.Bit.UART4 == UART4_ENUM_1ST_PINMUX)
        {
            printf("PWM4 conflict with to UART4_1: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_4 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM4 = PWM_ENUM_PWM;
#ifdef CONFIG_PWM
        pwm_config(4, CONFIG_PWM_DUTY04, CONFIG_PWM_PERIOD04);
        pwm_enable(4);
#endif
    }
    if (config & PIN_PWM_CFG_PWM5)
    {
        if (top_reg9.Bit.UART4 == UART4_ENUM_1ST_PINMUX)
        {
            printf("PWM5 conflict with to UART4_1: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_5 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM5 = PWM_ENUM_PWM;
#ifdef CONFIG_PWM
        pwm_config(5, CONFIG_PWM_DUTY05, CONFIG_PWM_PERIOD05);
        pwm_enable(5);
#endif
    }
    if (config & PIN_PWM_CFG_PWM6)
    {
        if ((top_reg9.Bit.UART4 == UART4_ENUM_1ST_PINMUX) && (top_reg9.Bit.UART4_CTSRTS == UART_CTSRTS_PINMUX))
        {
            printf("PWM6 conflict with to UART4_1: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_6 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM6 = PWM_ENUM_PWM;
#ifdef CONFIG_PWM
        pwm_config(6, CONFIG_PWM_DUTY06, CONFIG_PWM_PERIOD06);
        pwm_enable(6);
#endif
    }
    if (config & PIN_PWM_CFG_PWM7)
    {
        if ((top_reg9.Bit.UART4 == UART4_ENUM_1ST_PINMUX) && (top_reg9.Bit.UART4_CTSRTS == UART_CTSRTS_PINMUX))
        {
            printf("PWM7 conflict with to UART4_1: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_7 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM7 = PWM_ENUM_PWM;
#ifdef CONFIG_PWM
        pwm_config(7, CONFIG_PWM_DUTY07, CONFIG_PWM_PERIOD07);
        pwm_enable(7);
#endif
    }
    if (config & PIN_PWM_CFG_PWM8)
    {
        if (top_reg9.Bit.UART3 == UART3_ENUM_2ND_PINMUX)
        {
            printf("PWM8 conflict with to UART3_2: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_8 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM8 = PWM_ENUM_PWM;
#ifdef CONFIG_PWM
        pwm_config(8, CONFIG_PWM_DUTY08, CONFIG_PWM_PERIOD08);
        pwm_enable(8);
#endif
    }
    if (config & PIN_PWM_CFG_PWM9)
    {
        if (top_reg9.Bit.UART3 == UART3_ENUM_2ND_PINMUX)
        {
            printf("PWM9 conflict with to UART3_2: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_9 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM9 = PWM_ENUM_PWM;
#ifdef CONFIG_PWM
        pwm_config(9, CONFIG_PWM_DUTY09, CONFIG_PWM_PERIOD09);
        pwm_enable(9);
#endif
    }
    if (config & PIN_PWM_CFG_PWM10)
    {
        if ((top_reg9.Bit.UART3 == UART3_ENUM_2ND_PINMUX) && (top_reg9.Bit.UART3_CTSRTS == UART_CTSRTS_PINMUX))
        {
            printf("PWM10 conflict with to UART3_2: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_10 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM10 = PWM_ENUM_PWM;
#ifdef CONFIG_PWM
        pwm_config(10, CONFIG_PWM_DUTY10, CONFIG_PWM_PERIOD10);
        pwm_enable(10);
#endif
    }
    if (config & PIN_PWM_CFG_PWM11)
    {
        if ((top_reg9.Bit.UART3 == UART3_ENUM_2ND_PINMUX) && (top_reg9.Bit.UART3_CTSRTS == UART_CTSRTS_PINMUX))
        {
            printf("PWM11 conflict with to UART3_2: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_11 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM11 = PWM_ENUM_PWM;
#ifdef CONFIG_PWM
        pwm_config(11, CONFIG_PWM_DUTY11, CONFIG_PWM_PERIOD11);
        pwm_enable(11);
#endif
    }
    if (config & PIN_PWM_CFG_PWM12)
    {
        if (top_reg5.Bit.SPI == SPI_2ND_PINMUX)
        {
            printf("PWM12 conflict with to SPI_2: 0x%x\r\n", config);
            return E_OBJ;
        }

        if (top_reg9.Bit.UART3 == UART3_ENUM_1ST_PINMUX)
        {
            printf("PWM12 conflict with to UART3_1: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_15 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM12 = PWM_ENUM_PWM;
#ifdef CONFIG_PWM
        pwm_config(12, CONFIG_PWM_DUTY12, CONFIG_PWM_PERIOD12);
        pwm_enable(12);
#endif
    }
    if (config & PIN_PWM_CFG_PWM13)
    {
        if (top_reg5.Bit.SPI == SPI_2ND_PINMUX)
        {
            printf("PWM13 conflict with to SPI_2: 0x%x\r\n", config);
            return E_OBJ;
        }

        if (top_reg9.Bit.UART3 == UART3_ENUM_1ST_PINMUX)
        {
            printf("PWM13 conflict with to UART3_1: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_16 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM13 = PWM_ENUM_PWM;
#ifdef CONFIG_PWM
        pwm_config(13, CONFIG_PWM_DUTY13, CONFIG_PWM_PERIOD13);
        pwm_enable(13);
#endif
    }
    if (config & PIN_PWM_CFG_PWM14)
    {
        if (top_reg5.Bit.SPI == SPI_2ND_PINMUX)
        {
            printf("PWM14 conflict with to SPI_2: 0x%x\r\n", config);
            return E_OBJ;
        }

        if ((top_reg9.Bit.UART3 == UART3_ENUM_1ST_PINMUX) && (top_reg9.Bit.UART3_CTSRTS == UART_CTSRTS_PINMUX))
        {
            printf("PWM14 conflict with to UART3_1: 0x%x\r\n", config);
            return E_OBJ;
        }


        top_reg_pgpio0.Bit.PGPIO_17 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM14 = PWM_ENUM_PWM;
#ifdef CONFIG_PWM
        pwm_config(14, CONFIG_PWM_DUTY14, CONFIG_PWM_PERIOD14);
        pwm_enable(14);
#endif
    }
    if (config & PIN_PWM_CFG_PWM15)
    {
        if (top_reg5.Bit.SPI3_RDY == SPI3_RDY_1ST_PINMUX)
        {
            printf("PWM15 conflict with to SPI3_1_RDY: 0x%x\r\n", config);
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_25 = GPIO_ID_EMUM_FUNC;

        top_reg7.Bit.PWM15 = PWM_ENUM_PWM;
#ifdef CONFIG_PWM
        pwm_config(15, CONFIG_PWM_DUTY15, CONFIG_PWM_PERIOD15);
        pwm_enable(15);
#endif
    }

    if (config & (PIN_PWM_CFG_CCNT|PIN_PWM_CFG_CCNT_2ND))
    {
        if (config & PIN_PWM_CFG_CCNT_2ND)
        {
            // without check LCD2?

            // without check LCD?

            // ETH
            if (top_reg6.Bit.ETH != ETH_ID_ENUM_GPIO)
            {
                printf("PICNT conflict with ETH\r\n");
                return E_OBJ;
            }

            top_reg_lgpio0.Bit.LGPIO_0 = GPIO_ID_EMUM_FUNC;
            top_reg7.Bit.PI_CNT = PICNT_ENUM_PICNT2;
        }
        else
        {
            if (top_reg6.Bit.REMOTE == REMOTE_ENUM_REMOTE)
            {
                printf("PICNT conflict with REMOTE\r\n");
                return E_OBJ;
            }

            if (top_reg6.Bit.SP2_CLK == SP2_CLK_SEL_ENUM_SP2CLK)
            {
                printf("PICNT conflict with to SP_CLK2_1: 0x%x\r\n", config);
                return E_OBJ;
            }

            top_reg_pgpio0.Bit.PGPIO_24 = GPIO_ID_EMUM_FUNC;

            top_reg7.Bit.PI_CNT = PICNT_ENUM_PICNT;
        }
    }
    if (config & (PIN_PWM_CFG_CCNT2|PIN_PWM_CFG_CCNT2_2ND))
    {
        if (config & PIN_PWM_CFG_CCNT2_2ND)
        {
            // without check LCD2?

            // without check LCD?

            // ETH
            if (top_reg6.Bit.ETH != ETH_ID_ENUM_GPIO)
            {
                printf("PICNT2 conflict with ETH\r\n");
                return E_OBJ;
            }

            top_reg_lgpio0.Bit.LGPIO_12 = GPIO_ID_EMUM_FUNC;
            top_reg7.Bit.PI_CNT2 = PICNT_ENUM_PICNT2;
        }
        else
        {
            if ((top_reg9.Bit.UART2 == UART2_ENUM_1ST_PINMUX) &&
                (top_reg9.Bit.UART2_CTSRTS == UART_CTSRTS_PINMUX))
            {
                printf("PICNT2 conflict with UART2_1\r\n");
                return E_OBJ;
            }
            if (top_reg6.Bit.AUDIO == AUDIO_ENUM_I2S)
            {
                printf("PICNT2 conflict with AUDIO\r\n");
                return E_OBJ;
            }
            top_reg_pgpio0.Bit.PGPIO_23 = GPIO_ID_EMUM_FUNC;

            top_reg7.Bit.PI_CNT2 = PICNT_ENUM_PICNT;
        }
    }
    if (config & (PIN_PWM_CFG_CCNT3|PIN_PWM_CFG_CCNT3_2ND))
    {
        if (config & PIN_PWM_CFG_CCNT3_2ND)
        {
            // without check LCD2?

            // without check LCD?

            // ETH
            if (top_reg6.Bit.ETH != ETH_ID_ENUM_GPIO)
            {
                printf("PICNT3 conflict with ETH\r\n");
                return E_OBJ;
            }

            top_reg_lgpio0.Bit.LGPIO_17 = GPIO_ID_EMUM_FUNC;
            top_reg7.Bit.PI_CNT3 = PICNT_ENUM_PICNT2;
        }
        else
        {
            if (top_reg6.Bit.AUDIO == AUDIO_ENUM_I2S)
            {
                printf("PICNT3 conflict with to AUDIO: 0x%x\r\n", config);
                return E_OBJ;
            }
            if (top_reg3.Bit.SP_CLK != SP_CLK_SEL_ENUM_SP_2_CLK)
            {
                printf("PICNT3 conflict with to SP_CLK_2: 0x%x\r\n", config);
                return E_OBJ;
            }

            top_reg_pgpio0.Bit.PGPIO_19 = GPIO_ID_EMUM_FUNC;

            top_reg7.Bit.PI_CNT3 = PICNT_ENUM_PICNT;
        }
    }
    if (config & (PIN_PWM_CFG_CCNT4|PIN_PWM_CFG_CCNT4_2ND))
    {
        printf("NA51023 donot support CCNT4 \r\n");
    }

    return E_OK;
}

static ER pinmux_config_audio(uint32_t config)
{
    if (config & (PIN_AUDIO_CFG_I2S))
    {
        if (top_reg9.Bit.UART2 == UART2_ENUM_1ST_PINMUX)
        {
            printf("AUDIO conflict with UART2_1\r\n");
            return E_OBJ;
        }
        if (top_reg7.Bit.PI_CNT2 == PICNT_ENUM_PICNT)
        {
            printf("AUDIO conflict with PICNT2_1\r\n");
            return E_OBJ;
        }
        if (top_reg5.Bit.SPI3 == SPI3_1ST_PINMUX)
        {
            printf("AUDIO conflict with SPI3_1\r\n");
            return E_OBJ;
        }
        top_reg_pgpio0.Bit.PGPIO_20 = GPIO_ID_EMUM_FUNC;
        top_reg_pgpio0.Bit.PGPIO_21 = GPIO_ID_EMUM_FUNC;
        top_reg_pgpio0.Bit.PGPIO_22 = GPIO_ID_EMUM_FUNC;
        top_reg_pgpio0.Bit.PGPIO_23 = GPIO_ID_EMUM_FUNC;

        top_reg6.Bit.AUDIO = AUDIO_ENUM_I2S;

    }

    if (config & (PIN_AUDIO_CFG_MCLK))
    {
        if (top_reg3.Bit.SP_CLK == SP_CLK_SEL_ENUM_SP_2_CLK)
        {
            printf("AUDIO MCLK conflict with SP_CLK_2\r\n");
            return E_OBJ;
        }
        if (top_reg7.Bit.PI_CNT3 == PICNT_ENUM_PICNT)
        {
            printf("AUDIO MCLK conflict with LOCKN\r\n");
            return E_OBJ;
        }
        top_reg_pgpio0.Bit.PGPIO_19 = GPIO_ID_EMUM_FUNC;

        top_reg6.Bit.AUDIO_MCLK = AUDIO_MCLK_MCLK;
    }

    if (config & (PIN_AUDIO_CFG_DMIC))
    {
        if (top_reg6.Bit.SP2_CLK == SP2_CLK_SEL_ENUM_SP2CLK)
        {
            printf("AUDIO DMIC conflict with SP_CLK2_1\r\n");
            return E_OBJ;
        }

        if (top_reg6.Bit.REMOTE == REMOTE_ENUM_REMOTE)
        {
            printf("AUDIO DMIC conflict with REMOTE\r\n");
            return E_OBJ;
        }

        if (top_reg7.Bit.PI_CNT == PICNT_ENUM_PICNT)
        {
            printf("AUDIO DMIC conflict with PICNT_1\r\n");
            return E_OBJ;
        }

        if (top_reg3.Bit.FLCTR == FLCTR_SEL_ENUM_FLCTR)
        {
            printf("AUDIO DMIC conflict with Flash Control\r\n");
            return E_OBJ;
        }

        if (top_reg7.Bit.PWM15 == PWM_ENUM_PWM)
        {
            printf("AUDIO DMIC DMDAT conflict with PWM15\r\n");
            return E_OBJ;
        }

        top_reg_pgpio0.Bit.PGPIO_24 = GPIO_ID_EMUM_FUNC;
        top_reg_pgpio0.Bit.PGPIO_25 = GPIO_ID_EMUM_FUNC;

        top_reg6.Bit.DIGITAL_MIC = DMIC_ENUM_DMIC;
    }

    return E_OK;
}

static ER pinmux_config_lcd(uint32_t config)
{
    uint32_t tmp;

    tmp = config & PINMUX_DISPMUX_SEL_MASK;
    if (tmp == PINMUX_DISPMUX_SEL_MASK)
    {
        printf("invalid locate: 0x%x\r\n", config);
        return E_PAR;
    }

    disp_pinmux_config[PINMUX_FUNC_ID_LCD] = config;

    return E_OK;
}

static ER pinmux_config_tv(uint32_t config)
{
    uint32_t tmp;

    tmp = config & PINMUX_TV_HDMI_CFG_MASK;
    if ((tmp != PINMUX_TV_HDMI_CFG_NORMAL) &&
        (tmp != PINMUX_TV_HDMI_CFG_PINMUX_ON))
    {
        printf("invalid config: 0x%x\r\n", config);
        return E_PAR;
    }

    disp_pinmux_config[PINMUX_FUNC_ID_TV] = config;

    return E_OK;
}

static ER pinmux_config_eth(uint32_t config)
{
    uint32_t uiLCDMode;

    if (config == PIN_ETH_CFG_NONE)
    {
    }
    else if (config & PIN_ETH_CFG_INTERANL)
    {
        top_reg_dgpio0.Bit.DGPIO_0 = GPIO_ID_EMUM_FUNC;
        top_reg_dgpio0.Bit.DGPIO_1 = GPIO_ID_EMUM_FUNC;
    }
    else if (config & (PIN_ETH_CFG_MII|PIN_ETH_CFG_RMII|PIN_ETH_CFG_REVMII))
    {
        // check MDIO/MDC
        if (top_reg5.Bit.SPI3 == SPI3_1ST_PINMUX)
        {
            printf("ETH conflict with SPI3_1\r\n");
            return E_OBJ;
        }
        if ((top_reg9.Bit.UART3 == UART3_ENUM_1ST_PINMUX) &&
            (top_reg9.Bit.UART3_CTSRTS == UART_CTSRTS_PINMUX))
        {
            printf("ETH conflict with UART3_1\r\n");
            return E_OBJ;
        }
        if (top_reg5.Bit.SIFCH2 == SIFCH_ENUM_SIF)
        {
            printf("ETH conflict with SIF2\r\n");
            return E_OBJ;
        }
        if (top_reg5.Bit.SIFCH3 == SIFCH_ENUM_SIF)
        {
            printf("ETH conflict with SIF3\r\n");
            return E_OBJ;
        }
        if ((config & PIN_ETH_CFG_MII) || (config & PIN_ETH_CFG_REVMII))
        {
            if (disp_pinmux_config[PINMUX_FUNC_ID_LCD2] & PINMUX_DISPMUX_SEL_LCD2)
            {
                printf("ETH conflict with LCD2\r\n");
                return E_OBJ;
            }
            if (disp_pinmux_config[PINMUX_FUNC_ID_LCD] & PINMUX_DISPMUX_SEL_LCD)
            {
                uiLCDMode = disp_pinmux_config[PINMUX_FUNC_ID_LCD] & ~(PINMUX_DISPMUX_SEL_LCD|PINMUX_LCDMODE_AUTO_PINMUX);
                if ((uiLCDMode == PINMUX_LCDMODE_RGB_PARALL)||(uiLCDMode == PINMUX_LCDMODE_CCIR601)||
                    ((uiLCDMode >= PINMUX_LCDMODE_RGB_PARALL666) && (uiLCDMode <= PINMUX_LCDMODE_RGB_PARALL_DELTA))||
                    ((uiLCDMode >= PINMUX_LCDMODE_MI_FMT0) && (uiLCDMode <= PINMUX_LCDMODE_MI_SERIAL_SEP)))
                {
                    printf("ETH conflict with LCD\r\n");
                    return E_OBJ;
                }
            }
            if (top_reg7.Bit.PI_CNT == PICNT_ENUM_PICNT2)
            {
                printf("ETH conflict with PICNT_2\r\n");
                return E_OBJ;
            }
            if (top_reg7.Bit.PI_CNT2 == PICNT_ENUM_PICNT2)
            {
                printf("ETH conflict with PICNT2_2\r\n");
                return E_OBJ;
            }
            if (top_reg7.Bit.PI_CNT3 == PICNT_ENUM_PICNT2)
            {
                printf("ETH conflict with PICNT3_2\r\n");
                return E_OBJ;
            }

            if (top_reg3.Bit.SP_CLK == SP_CLK_SEL_ENUM_SP_2_CLK)
            {
                printf("ETH conflict with SP_CLK_2\r\n");
                return E_OBJ;
            }
            top_reg_lgpio0.Bit.LGPIO_0 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_1 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_2 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_3 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_4 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_5 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_6 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_7 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_8 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_12 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_13 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_14 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_15 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_16 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_17 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_18 = GPIO_ID_EMUM_FUNC;

            if (config & PIN_ETH_CFG_MII)
            {
                top_reg_lgpio0.Bit.LGPIO_11 = GPIO_ID_EMUM_GPIO;   // Ethernet PHY reset pin
                top_reg_lgpio0.Bit.LGPIO_19 = GPIO_ID_EMUM_FUNC;
                top_reg6.Bit.ETH = ETH_ID_ENUM_MII;
            }
            else
                top_reg6.Bit.ETH = ETH_ID_ENUM_REVMII;
        }
        else if (config & PIN_ETH_CFG_RMII)
        {
            if (disp_pinmux_config[PINMUX_FUNC_ID_LCD2] & PINMUX_DISPMUX_SEL_LCD2)
            {
                printf("ETH conflict with LCD2\r\n");
                return E_OBJ;
            }
            if (disp_pinmux_config[PINMUX_FUNC_ID_LCD] & PINMUX_DISPMUX_SEL_LCD)
            {
                uiLCDMode = disp_pinmux_config[PINMUX_FUNC_ID_LCD] & ~(PINMUX_DISPMUX_SEL_LCD|PINMUX_LCDMODE_AUTO_PINMUX);
                if ((uiLCDMode == PINMUX_LCDMODE_RGB_PARALL)||(uiLCDMode == PINMUX_LCDMODE_CCIR601)||
                    ((uiLCDMode >= PINMUX_LCDMODE_RGB_PARALL666) && (uiLCDMode <= PINMUX_LCDMODE_RGB_PARALL_DELTA))||
                    ((uiLCDMode >= PINMUX_LCDMODE_MI_FMT0) && (uiLCDMode <= PINMUX_LCDMODE_MI_SERIAL_SEP)))
                {
                    printf("ETH conflict with LCD\r\n");
                    return E_OBJ;
                }
            }

            top_reg_lgpio0.Bit.LGPIO_0 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_1 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_4 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_7 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_11 = GPIO_ID_EMUM_GPIO;   // Ethernet PHY reset pin
            top_reg_lgpio0.Bit.LGPIO_12 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_13 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_17 = GPIO_ID_EMUM_FUNC;
            top_reg_lgpio0.Bit.LGPIO_19 = GPIO_ID_EMUM_FUNC;   // Clock output for PHY (25MHz)
            top_reg_lgpio0.Bit.LGPIO_20 = GPIO_ID_EMUM_FUNC;

            top_reg6.Bit.ETH = ETH_ID_ENUM_RMII;
        }

        // MDC/MDIO
        top_reg_lgpio0.Bit.LGPIO_9 = GPIO_ID_EMUM_FUNC;
        top_reg_lgpio0.Bit.LGPIO_10 = GPIO_ID_EMUM_FUNC;
    }
    return E_OK;
}
