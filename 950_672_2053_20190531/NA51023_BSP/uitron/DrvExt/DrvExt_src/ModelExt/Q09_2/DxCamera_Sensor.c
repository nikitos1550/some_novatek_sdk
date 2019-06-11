/**
    DxSensor

    Sensor common interface

    @file       DxSensor.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "Delay.h"
// sensor
#include "DxSensor.h"
#include "sensor_if.h"
#include "sensor_drv.h"
#include "IOCfg.h"
// Driver_Sys
#include "lvds.h"
#include "sif.h"

#define __MODULE__          DxDrv
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" // *=All, [mark]=CustomClass
#include "DebugModule.h"

typedef enum {
	POW_CTRL_TURN_ON = 0,
	POW_CTRL_TURN_OFF,
	POW_CTRL_SAVE_ON,
	POW_CTRL_SAVE_OFF,
} POW_CTRL_FLAG;

typedef SENSOR_DRV_TAB *(*SEN_GET_DRV)(void);
typedef SENSOR_INIT_OBJ(*SEN_GET_INI_OBJ)(SENSOR_ID id);
typedef BOOL (*SEN_PLUG_IN)(SENSOR_ID id);
typedef SEN_PLUG_INFO (*SEN_PLUG_INFO_FP)(SENSOR_ID id);
typedef void (*SEN_POWER_CTRL)(SENSOR_ID id, POW_CTRL_FLAG flag);

typedef struct {
	CHAR *name;
	SEN_GET_DRV drv_tab;
	SEN_GET_INI_OBJ drv_obj;
	SEN_POWER_CTRL pow_ctrl;
	SEN_PLUG_IN det_plug_in;
	SEN_PLUG_IN plug_in_cb;
	SEN_PLUG_INFO_FP det_plug_info;
} SEN_MAP_INFO;

static CHAR sensor_name[SENSOR_ID_MAX_NUM][30] = {
	_SENSOR1_,
	_SENSOR2_,
};

#if (defined(_CMOS_AR0237L_))
static SENSOR_INIT_OBJ sensor_get_obj_ar0237l(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;

	// set mclk change enable to notify sensor driver
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	// set sensor interface pin map (LVDS/CSI/...)
	init_obj.sen_2_serial_pin_map[0] = 0;
	init_obj.sen_2_serial_pin_map[1] = 1;
	init_obj.sen_2_serial_pin_map[2] = 2;
	init_obj.sen_2_serial_pin_map[3] = 3;
	return init_obj;
}
static void sen_power_ctrl_ar0237l(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	switch (flag) {
	case POW_CTRL_TURN_ON:
		// set clock information
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, ENABLE);

		// set power on sequence
		// reset pin
		UINT32 reset_pin = S_GPIO_4;
		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		gpio_clearPin(reset_pin);
		Delay_DelayMs(30);
		gpio_setPin(reset_pin);
		Delay_DelayMs(1);

		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, DISABLE);
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif

#if (defined(_CMOS_IMX291M_))
#if 0
static void csi_hw_rst_imx291cqc(SENSOR_ID sensor_id)
{
	debug_msg("~G csi_hw_rst_imx291cqc \r\n");
	gpio_setDir(S_GPIO_4, GPIO_DIR_OUTPUT);
	gpio_clearPin(S_GPIO_4);
	Delay_DelayMs(1);
	gpio_setPin(S_GPIO_4);
	Delay_DelayUs(10);
}
#endif

static SENSOR_INIT_OBJ sensor_get_obj_imx291m(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C1_1ST;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	//init_obj.interface_info.csi.fp_hw_rst = csi_hw_rst_imx291cqc;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;

	// set mclk change enable to notify sensor driver
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	// set sensor interface pin map (LVDS/CSI/...)
	init_obj.sen_2_serial_pin_map[0] = 0;
	init_obj.sen_2_serial_pin_map[1] = 1;
	init_obj.sen_2_serial_pin_map[2] = 2;
	init_obj.sen_2_serial_pin_map[3] = 3;
	return init_obj;
}
static void sen_power_ctrl_imx291m(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	switch (flag) {
	case POW_CTRL_TURN_ON:
		// set clock information
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, ENABLE);

		// set power on sequence

		gpio_setDir(GPIO_SENSOR_12VEN, GPIO_DIR_OUTPUT);
		gpio_setDir(GPIO_SENSOR_18VEN, GPIO_DIR_OUTPUT);
		gpio_setDir(GPIO_SENSOR_29VEN, GPIO_DIR_OUTPUT);
		gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
		gpio_setPin(GPIO_SENSOR_12VEN);
		Delay_DelayMs(10);
		gpio_setPin(GPIO_SENSOR_18VEN);
		Delay_DelayMs(10);
		gpio_setPin(GPIO_SENSOR_29VEN);
		Delay_DelayMs(20);
		gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
		gpio_setPin(GPIO_SENSOR_RESET);
		Delay_DelayMs(20);
		gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
		gpio_clearPin(GPIO_SENSOR_RESET);
		Delay_DelayMs(20);
		gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
		gpio_setPin(GPIO_SENSOR_RESET);
		Delay_DelayMs(20);
		gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
		gpio_clearPin(GPIO_SENSOR_RESET);
		Delay_DelayMs(1);
		gpio_setPin(GPIO_SENSOR_RESET);
		Delay_DelayMs(1);
		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, DISABLE);

		// set power off sequence
		gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
		gpio_clearPin(GPIO_SENSOR_RESET);
		Delay_DelayMs(1);
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif

#if (defined(_CMOS_IMX290M_))
#if 0
static void csi_hw_rst_imx290(SENSOR_ID sensor_id)
{
	debug_msg("~G csi_hw_rst_imx290 \r\n");
	gpio_setDir(S_GPIO_4, GPIO_DIR_OUTPUT);
	gpio_clearPin(S_GPIO_4);
	Delay_DelayMs(1);
	gpio_setPin(S_GPIO_4);
	Delay_DelayUs(10);
}
#endif

static SENSOR_INIT_OBJ sensor_get_obj_imx290m(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	//init_obj.interface_info.csi.fp_hw_rst = csi_hw_rst_imx290;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;

	// set mclk change enable to notify sensor driver
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	// set sensor interface pin map (LVDS/CSI/...)
	init_obj.sen_2_serial_pin_map[0] = 0;
	init_obj.sen_2_serial_pin_map[1] = 1;
	init_obj.sen_2_serial_pin_map[2] = 2;
	init_obj.sen_2_serial_pin_map[3] = 3;
	return init_obj;
}
static void sen_power_ctrl_imx290m(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	UINT32 reset_pin = S_GPIO_4;
	switch (flag) {
	case POW_CTRL_TURN_ON:
		// set clock information
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, ENABLE);

		// set power on sequence
		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		gpio_clearPin(reset_pin);
		Delay_DelayMs(1);
		gpio_setPin(reset_pin);
		Delay_DelayMs(1);

		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, DISABLE);

		// set power off sequence
		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		gpio_clearPin(reset_pin);
		Delay_DelayMs(1);

		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif


#if (defined(_CMOS_OV2715M_))
static SENSOR_INIT_OBJ sensor_get_obj_ov2715m(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;

	//init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
	switch (id) {
	case SENSOR_ID_1:
	default:
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
		break;

	case SENSOR_ID_2:
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C1_1ST;
		break;
	}

	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.option = SENSOR_OPTION_FLIP;

	// set mclk change enable to notify sensor driver
	// init_obj.chg_mclk_en = ENABLE;
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	// set sensor interface pin map (LVDS/CSI/...)
	//init_obj.sen_2_serial_pin_map[0] = 0;
	init_obj.sen_2_serial_pin_map[0] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[1] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[2] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[3] = SEN_IGNORE;

	switch (id) {
	case SENSOR_ID_1:
	default:
		init_obj.sen_2_serial_pin_map[0] = 0;
		break;

	case SENSOR_ID_2:
		init_obj.sen_2_serial_pin_map[2] = 0;
		break;
	}
	return init_obj;
}
static void sen_power_ctrl_ov2715m(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	UINT32 reset_pin = S_GPIO_4;

	switch (flag) {
	case POW_CTRL_TURN_ON:
		// set clock information
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, ENABLE);

		switch (id) {
		case SENSOR_ID_1:
		default:
			reset_pin = S_GPIO_4;
			break;

		case SENSOR_ID_2:
			reset_pin = S_GPIO_4;
			break;
		}

		//Power on sequence
		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		gpio_clearPin(reset_pin);
		Delay_DelayMs(1);
		gpio_setPin(reset_pin);
		Delay_DelayMs(1);

		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, DISABLE);
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif

#if (defined(_CMOS_OV2735M_))
static SENSOR_INIT_OBJ sensor_get_obj_ov2735m(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;

	//init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
	switch (id) {
	case SENSOR_ID_1:
	default:
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
		break;

	case SENSOR_ID_2:
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C1_1ST;
		break;
	}

	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.option = SENSOR_OPTION_FLIP | SENSOR_OPTION_MIRROR;

	// set mclk change enable to notify sensor driver
	// init_obj.chg_mclk_en = ENABLE;
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	// set sensor interface pin map (LVDS/CSI/...)
	//init_obj.sen_2_serial_pin_map[0] = 0;
	init_obj.sen_2_serial_pin_map[0] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[1] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[2] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[3] = SEN_IGNORE;

	switch (id) {
	case SENSOR_ID_1:
	default:
		init_obj.sen_2_serial_pin_map[0] = 0;
		break;

	case SENSOR_ID_2:
		init_obj.sen_2_serial_pin_map[2] = 0;
		break;
	}
	return init_obj;
}
static void sen_power_ctrl_ov2735m(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	switch (flag) {
	case POW_CTRL_TURN_ON:
		// set clock information
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, ENABLE);

		// set power on sequence
		UINT32 reset_pin = S_GPIO_4;
		switch (id) {
		case SENSOR_ID_1:
		default:
			reset_pin = S_GPIO_4;
			break;

		case SENSOR_ID_2:
			reset_pin = S_GPIO_4;
			break;
		}
		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		gpio_clearPin(reset_pin);
		Delay_DelayMs(1);
		gpio_setPin(reset_pin);
		Delay_DelayMs(1);

		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, DISABLE);
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif

#if (defined(_CMOS_OS05A10M_))
static SENSOR_INIT_OBJ sensor_get_obj_os05a10m(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.option = SENSOR_OPTION_FLIP;

	// set mclk change enable to notify sensor driver
	// init_obj.chg_mclk_en = ENABLE;
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	// set sensor interface pin map (LVDS/CSI/...)
	init_obj.sen_2_serial_pin_map[0] = 0;
	init_obj.sen_2_serial_pin_map[1] = 1;
	init_obj.sen_2_serial_pin_map[2] = 2;
	init_obj.sen_2_serial_pin_map[3] = 3;
	return init_obj;
}
static void sen_power_ctrl_os05a10m(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	switch (flag) {
	case POW_CTRL_TURN_ON:
		// set clock information
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, ENABLE);

		// set power on sequence
		UINT32 reset_pin = S_GPIO_4;
		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		gpio_clearPin(reset_pin);
		Delay_DelayMs(1);
		gpio_setPin(reset_pin);
		Delay_DelayMs(1);

		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, DISABLE);
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif

#if (defined(_CMOS_OS08A10M_))
static SENSOR_INIT_OBJ sensor_get_obj_os08a10m(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.option = SENSOR_OPTION_MIRROR;

	// set mclk change enable to notify sensor driver
	// init_obj.chg_mclk_en = ENABLE;
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	// set sensor interface pin map (LVDS/CSI/...)
	init_obj.sen_2_serial_pin_map[0] = 0;
	init_obj.sen_2_serial_pin_map[1] = 1;
	init_obj.sen_2_serial_pin_map[2] = 2;
	init_obj.sen_2_serial_pin_map[3] = 3;
	return init_obj;
}
static void sen_power_ctrl_os08a10m(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	switch (flag) {
	case POW_CTRL_TURN_ON:
		// set clock information
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, ENABLE);
		//DBG_ERR("MCLK Enable id=%d, flag=%d\r\n", id, flag);

		// set power on sequence
		UINT32 reset_pin = S_GPIO_4;
		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		gpio_clearPin(reset_pin);
		Delay_DelayMs(1);
		gpio_setPin(reset_pin);
		Delay_DelayMs(1);

		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, DISABLE);
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif

#if (defined(_CMOS_SC2235P_))
static SENSOR_INIT_OBJ sensor_get_obj_sc2235p(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C1_1ST;//for TP ; EVB SENSOR_I2C2_1ST
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.option = SENSOR_OPTION_NONE;

	// set mclk change enable to notify sensor driver
	// init_obj.chg_mclk_en = ENABLE;
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	// set sensor interface pin map (LVDS/CSI/...)
	init_obj.sen_2_serial_pin_map[0] = 0;
	init_obj.sen_2_serial_pin_map[1] = 1;
	init_obj.sen_2_serial_pin_map[2] = 2;
	init_obj.sen_2_serial_pin_map[3] = 3;
	return init_obj;
}
static void sen_power_ctrl_sc2235p(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	switch (flag) {
	case POW_CTRL_TURN_ON:
		// set clock information
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, ENABLE);
		//DBG_ERR("MCLK Enable id=%d, flag=%d\r\n", id, flag);

		// set power on sequence
		UINT32 reset_pin = S_GPIO_4;
		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		//gpio_clearPin(reset_pin);//Low
		//Delay_DelayMs(1);
		gpio_setPin(reset_pin);//high
		Delay_DelayMs(1);
		gpio_clearPin(reset_pin);//Low
		Delay_DelayMs(1);
		gpio_setPin(reset_pin);//high
		Delay_DelayMs(1);

		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, DISABLE);
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif

#if (defined(_CMOS_OV4689M_))
static SENSOR_INIT_OBJ sensor_get_obj_ov4689m(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.option = SENSOR_OPTION_FLIP;

	// set mclk change enable to notify sensor driver
	// init_obj.chg_mclk_en = ENABLE;
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	// set sensor interface pin map (LVDS/CSI/...)
	init_obj.sen_2_serial_pin_map[0] = 0;
	init_obj.sen_2_serial_pin_map[1] = 1;
	init_obj.sen_2_serial_pin_map[2] = 2;
	init_obj.sen_2_serial_pin_map[3] = 3;
	return init_obj;
}
static void sen_power_ctrl_ov4689m(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	switch (flag) {
	case POW_CTRL_TURN_ON:
		// set clock information
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, ENABLE);

		// set power on sequence
		UINT32 reset_pin = S_GPIO_4;
		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		gpio_clearPin(reset_pin);
		Delay_DelayMs(1);
		gpio_setPin(reset_pin);
		Delay_DelayMs(1);

		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, DISABLE);
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif

#if (defined(_CMOS_AR0237P_))
static SENSOR_INIT_OBJ sensor_get_obj_ar0237p(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.option = SENSOR_OPTION_NONE;

	// set mclk change enable to notify sensor driver
	// init_obj.chg_mclk_en = ENABLE;
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	return init_obj;
}
static void sen_power_ctrl_ar0237p(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	UINT32 reset_pin = S_GPIO_4;
	switch (flag) {
	case POW_CTRL_TURN_ON:
		// set power on sequence
		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		gpio_setPin(reset_pin);
		Delay_DelayMs(1);
		// set clock information
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, ENABLE);
		Delay_DelayMs(1);
		gpio_clearPin(reset_pin);
		Delay_DelayMs(1);
		gpio_setPin(reset_pin);
		Delay_DelayMs(20);
		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, DISABLE);
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif

#if (defined(_CMOS_AR0237IRP_))
static SENSOR_INIT_OBJ sensor_get_obj_ar0237irp(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.option = SENSOR_OPTION_NONE;

	// set mclk change enable to notify sensor driver
	// init_obj.chg_mclk_en = ENABLE;
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	return init_obj;
}
static void sen_power_ctrl_ar0237irp(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	UINT32 reset_pin = S_GPIO_4;

	switch (flag) {
	case POW_CTRL_TURN_ON:
		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		gpio_setPin(reset_pin);
		Delay_DelayMs(1);
		// set clock information
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, ENABLE);
		Delay_DelayMs(1);
		gpio_clearPin(reset_pin);
		Delay_DelayMs(1);
		gpio_setPin(reset_pin);
		Delay_DelayMs(20);
		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, DISABLE);
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif

#if (defined(_CMOS_AR0521M_))
static SENSOR_INIT_OBJ sensor_get_obj_ar0521m(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.option = SENSOR_OPTION_MIRROR | SENSOR_OPTION_FLIP;

	// set mclk change enable to notify sensor driver
	// init_obj.chg_mclk_en = ENABLE;
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	// set sensor interface pin map (LVDS/CSI/...)
	init_obj.sen_2_serial_pin_map[0] = 0;
	init_obj.sen_2_serial_pin_map[1] = 1;
	init_obj.sen_2_serial_pin_map[2] = 2;
	init_obj.sen_2_serial_pin_map[3] = 3;
	return init_obj;
}
static void sen_power_ctrl_ar0521m(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	UINT32 reset_pin = S_GPIO_4;
	switch (flag) {
	case POW_CTRL_TURN_ON:
		// set clock information
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, ENABLE);

		// set power on sequence
		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		gpio_clearPin(reset_pin);
		Delay_DelayMs(1);
		gpio_setPin(reset_pin);
		Delay_DelayMs(1);

		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, DISABLE);
		Delay_DelayMs(1);
		gpio_clearPin(reset_pin);
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif

#if (defined(_CMOS_AR0330P_)) // for TP-LINK
static SENSOR_INIT_OBJ sensor_get_obj_ar0330p(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C1_1ST;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.option = SENSOR_OPTION_MIRROR | SENSOR_OPTION_FLIP;;

	// set mclk change enable to notify sensor driver
	// init_obj.chg_mclk_en = ENABLE;
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	return init_obj;
}
static void sen_power_ctrl_ar0330p(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	UINT32 reset_pin = S_GPIO_4;
	switch (flag) {
	case POW_CTRL_TURN_ON:
		// set power on sequence
		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		gpio_setPin(reset_pin);
		Delay_DelayMs(1);
		// set clock information
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, ENABLE);
		Delay_DelayMs(1);
		gpio_clearPin(reset_pin);
		Delay_DelayMs(1);
		gpio_setPin(reset_pin);
		Delay_DelayMs(20);
		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, DISABLE);
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif

#if (defined(_CMOS_AR0330M_))
static SENSOR_INIT_OBJ sensor_get_obj_ar0330m(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.option = SENSOR_OPTION_MIRROR | SENSOR_OPTION_FLIP;

	// set mclk change enable to notify sensor driver
	// init_obj.chg_mclk_en = ENABLE;
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	// set sensor interface pin map (LVDS/CSI/...)
	init_obj.sen_2_serial_pin_map[0] = 0;
	init_obj.sen_2_serial_pin_map[1] = 1;
	init_obj.sen_2_serial_pin_map[2] = 2;
	init_obj.sen_2_serial_pin_map[3] = 3;
	return init_obj;
}

static void sen_power_ctrl_ar0330m(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	UINT32 reset_pin = S_GPIO_4;
	switch (flag) {
	case POW_CTRL_TURN_ON:
		// set clock information
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, ENABLE);

		// set power on sequence
		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		gpio_clearPin(reset_pin);
		Delay_DelayMs(1);
		gpio_setPin(reset_pin);
		Delay_DelayMs(1);

		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, DISABLE);
		Delay_DelayMs(1);
		gpio_clearPin(reset_pin);
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif

#if (defined(_CMOS_PS5250M_))
static SENSOR_INIT_OBJ sensor_get_obj_ps5250m(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.option = SENSOR_OPTION_MIRROR | SENSOR_OPTION_FLIP;

	// set mclk change enable to notify sensor driver
	// init_obj.chg_mclk_en = ENABLE;
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	// set sensor interface pin map (LVDS/CSI/...)
	init_obj.sen_2_serial_pin_map[0] = 0;
	init_obj.sen_2_serial_pin_map[1] = 1;
	init_obj.sen_2_serial_pin_map[2] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[3] = SEN_IGNORE;
	return init_obj;
}

static void sen_power_ctrl_ps5250m(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	UINT32 reset_pin = S_GPIO_4;
	switch (flag) {
	case POW_CTRL_TURN_ON:


		// set power on sequence
		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		gpio_setPin(reset_pin);
		Delay_DelayMs(1);
		gpio_clearPin(reset_pin);
		Delay_DelayMs(1);
		gpio_setPin(reset_pin);
		Delay_DelayMs(1);

		// set clock information
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, ENABLE);

		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, DISABLE);
		Delay_DelayMs(1);
		gpio_clearPin(reset_pin);
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif

#if (defined(_CMOS_SC4236M_))
static SENSOR_INIT_OBJ sensor_get_obj_sc4236m(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.option = SENSOR_OPTION_MIRROR | SENSOR_OPTION_FLIP;

	// set mclk change enable to notify sensor driver
	// init_obj.chg_mclk_en = ENABLE;
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	// set sensor interface pin map (LVDS/CSI/...)
	init_obj.sen_2_serial_pin_map[0] = 0;
	init_obj.sen_2_serial_pin_map[1] = 1;
	init_obj.sen_2_serial_pin_map[2] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[3] = SEN_IGNORE;
	return init_obj;
}

static void sen_power_ctrl_sc4236m(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	UINT32 reset_pin = S_GPIO_4;
	switch (flag) {
	case POW_CTRL_TURN_ON:
		// set power on sequence
		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		gpio_setPin(reset_pin);
		Delay_DelayMs(2); //>=1ms
		gpio_clearPin(reset_pin);
		Delay_DelayMs(2); //>=1ms
		gpio_setPin(reset_pin);
		Delay_DelayMs(2); //>=1ms

		// set clock information
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, ENABLE);

		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, DISABLE);
		Delay_DelayMs(2);
		gpio_clearPin(reset_pin);
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif

#if (defined(_CMOS_SC5035M_))
static SENSOR_INIT_OBJ sensor_get_obj_sc5035m(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.option = SENSOR_OPTION_NONE; //SENSOR_OPTION_MIRROR | SENSOR_OPTION_FLIP;

	// set mclk change enable to notify sensor driver
	// init_obj.chg_mclk_en = ENABLE;
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	// set sensor interface pin map (LVDS/CSI/...)
	init_obj.sen_2_serial_pin_map[0] = 0;
	init_obj.sen_2_serial_pin_map[1] = 1;
	init_obj.sen_2_serial_pin_map[2] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[3] = SEN_IGNORE;
	return init_obj;
}

static void sen_power_ctrl_sc5035m(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	UINT32 reset_pin = S_GPIO_4;
	switch (flag) {
	case POW_CTRL_TURN_ON:
		// set power on sequence
		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		gpio_setPin(reset_pin);
		Delay_DelayMs(2); //>=1ms
		gpio_clearPin(reset_pin);
		Delay_DelayMs(2); //>=1ms
		gpio_setPin(reset_pin);
		Delay_DelayMs(2); //>=1ms

		// set clock information
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, ENABLE);

		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, DISABLE);
		Delay_DelayMs(2);
		gpio_clearPin(reset_pin);
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif

#if (defined(_CMOS_SIEPATGEN_))
static SENSOR_INIT_OBJ sensor_get_obj_siepatgen(SENSOR_ID id)
{

	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};

	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_MCLK;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	return init_obj;
}

static void sen_power_ctrl_siepatgen(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	switch (flag) {
	case POW_CTRL_TURN_ON:

		if (id == SENSOR_ID_1) {
			sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, ENABLE);
		} else {
			sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK2, ENABLE);
		}

		break;

	case POW_CTRL_TURN_OFF:
		if (id == SENSOR_ID_1) {
			sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK, DISABLE);
		} else {
			sensor_if_get_pll_ctrl_obj()->set_clk_en(id, SEN_CLK_SIEMCLK2, DISABLE);
		}
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif

static SEN_MAP_INFO sensor_map_tab[SENSOR_ID_MAX_NUM + 1] = {
#if (defined(_CMOS_AR0237L_))
	{"CMOS_AR0237L", sensor_get_drv_tab_ar0237l, sensor_get_obj_ar0237l, sen_power_ctrl_ar0237l, NULL, NULL, NULL},
#endif
#if (defined(_CMOS_IMX291M_))
	{"CMOS_IMX291M", sensor_get_drv_tab_imx291m, sensor_get_obj_imx291m, sen_power_ctrl_imx291m, NULL, NULL, NULL},
#endif
#if (defined(_CMOS_IMX290M_))
	{"CMOS_IMX290M", sensor_get_drv_tab_imx290m, sensor_get_obj_imx290m, sen_power_ctrl_imx290m, NULL, NULL, NULL},
#endif
#if (defined(_CMOS_OV2715M_))
	{"CMOS_OV2715M", sensor_get_drv_tab_ov2715m, sensor_get_obj_ov2715m, sen_power_ctrl_ov2715m, NULL, NULL, NULL},
#endif
#if (defined(_CMOS_OV2735M_))
	{"CMOS_OV2735M", sensor_get_drv_tab_ov2735m, sensor_get_obj_ov2735m, sen_power_ctrl_ov2735m, NULL, NULL, NULL},
#endif
#if (defined(_CMOS_OS05A10M_))
	{"CMOS_OS05A10M", sensor_get_drv_tab_os05a10m, sensor_get_obj_os05a10m, sen_power_ctrl_os05a10m, NULL, NULL, NULL},
#endif
#if (defined(_CMOS_OS08A10M_))
	{"CMOS_OS08A10M", sensor_get_drv_tab_os08a10m, sensor_get_obj_os08a10m, sen_power_ctrl_os08a10m, NULL, NULL, NULL},
#endif
#if (defined(_CMOS_OV4689M_))
	{"CMOS_OV4689M", sensor_get_drv_tab_ov4689m, sensor_get_obj_ov4689m, sen_power_ctrl_ov4689m, NULL, NULL, NULL},
#endif
#if (defined(_CMOS_AR0237P_))
	{"CMOS_AR0237P", sensor_get_drv_tab_ar0237p, sensor_get_obj_ar0237p, sen_power_ctrl_ar0237p, NULL, NULL, NULL},
#endif
#if (defined(_CMOS_AR0237IRP_))
	{"CMOS_AR0237IRP", sensor_get_drv_tab_ar0237irp, sensor_get_obj_ar0237irp, sen_power_ctrl_ar0237irp, NULL, NULL, NULL},
#endif
#if (defined(_CMOS_AR0330M_))
	{"CMOS_AR0330M", sensor_get_drv_tab_ar0330m, sensor_get_obj_ar0330m, sen_power_ctrl_ar0330m, NULL, NULL, NULL},
#endif
#if (defined(_CMOS_AR0330P_))
	{"CMOS_AR0330P", sensor_get_drv_tab_ar0330p, sensor_get_obj_ar0330p, sen_power_ctrl_ar0330p, NULL, NULL, NULL},
#endif
#if (defined(_CMOS_AR0521M_))
	{"CMOS_AR0521M", sensor_get_drv_tab_ar0521m, sensor_get_obj_ar0521m, sen_power_ctrl_ar0521m, NULL, NULL, NULL},
#endif
#if (defined(_CMOS_PS5250M_))
	{"CMOS_PS5250M", sensor_get_drv_tab_ps5250m, sensor_get_obj_ps5250m, sen_power_ctrl_ps5250m, NULL, NULL, NULL},
#endif
#if (defined(_CMOS_SC4236M_))
	{"CMOS_SC4236M", sensor_get_drv_tab_sc4236m, sensor_get_obj_sc4236m, sen_power_ctrl_sc4236m, NULL, NULL, NULL},
#endif
#if (defined(_CMOS_SC5035M_))
	{"CMOS_SC5035M", sensor_get_drv_tab_sc5035m, sensor_get_obj_sc5035m, sen_power_ctrl_sc5035m, NULL, NULL, NULL},
#endif
#if (defined(_CMOS_SIEPATGEN_))
	{"CMOS_SIEPATGEN", sensor_get_drv_tab_siepatgen, sensor_get_obj_siepatgen, sen_power_ctrl_siepatgen, NULL, NULL, NULL},
#endif
	{"END", NULL, NULL, NULL, NULL, NULL, NULL},
};


#if 0
#endif
/************************************************************************/
/*                      common function                                 */
/************************************************************************/
static SEN_MAP_INFO *drv_sensor_search(CHAR *search_name)
{
	UINT32 i;
	SEN_MAP_INFO *info;

	DBG_IND("search drv name = %s\r\n", search_name);
	info = sensor_map_tab;

	for (i = 0; (strcmp(info[i].name, "END") != 0); i ++) {
		if (strcmp(info[i].name, search_name) == 0) {
			return &info[i];
		}
	}
	return NULL;
}

BOOL drv_sensor_repalce_drv(SENSOR_ID id, CHAR *repalce_name)
{
	if (id >= SENSOR_ID_MAX_NUM) {
		DBG_ERR("id(%d) overflow\r\n");
		return FALSE;
	}
	DBG_IND("id = %d, repalce drv name = %s\r\n", id, repalce_name);

	if (drv_sensor_search(repalce_name) == NULL) {
		DBG_ERR("search sensor driver fail (%s)\r\n", repalce_name);
		return FALSE;
	}

	strncpy(&sensor_name[id][0], repalce_name, sizeof(sensor_name[id]));

	return TRUE;
}


SENSOR_MULTI_ID sensor_gp_map[SENSOR_ID_MAX_NUM] = {0}, sensor_subgp_map[SENSOR_ID_MAX_NUM] = {0};
ER drv_sensor_group_cfg(SENSOR_MULTI_ID *sensor_gp, SENSOR_MULTI_ID *sensor_subgp)
{
	UINT32 cnt, chk_cnt;

	for (cnt = 0; cnt < SENSOR_ID_MAX_NUM; cnt++) {
		if (sensor_gp == SENSOR_MULTI_ID_NONE) {
			continue;
		}
		for (chk_cnt = 0; chk_cnt < SENSOR_ID_MAX_NUM; chk_cnt++) {
			if (sensor_chk_multiid(chk_cnt, sensor_gp[cnt])) {
				sensor_gp_map[chk_cnt] |= sensor_gp[cnt];
			}
		}
	}
	for (cnt = 0; cnt < SENSOR_ID_MAX_NUM; cnt++) {
		if (sensor_subgp == SENSOR_MULTI_ID_NONE) {
			continue;
		}
		for (chk_cnt = 0; chk_cnt < SENSOR_ID_MAX_NUM; chk_cnt++) {
			if (sensor_chk_multiid(chk_cnt, sensor_subgp[cnt])) {
				sensor_subgp_map[chk_cnt] |= sensor_subgp[cnt];
			}
		}
	}

	return E_OK;
}

BOOL drv_sensor_first_cfg(void)
{
	UINT32 cnt_id;
	SEN_MAP_INFO *sen_map_info;
	ER rt = E_OK;

	for (cnt_id = 0; cnt_id < SENSOR_ID_MAX_NUM; cnt_id++) {
		if (sensor_debug_item[cnt_id] & SENSOR_DEBUG_MODELEXT) {
			debug_msg("^C%s\r\n", __func__);
		}
	}

	// Check that no sensor is open
	for (cnt_id = 0; cnt_id < SENSOR_ID_MAX_NUM; cnt_id++) {
		if (sensor_is_open(cnt_id)) {
			DBG_ERR("sensor id %d is open\r\n", cnt_id);
			return FALSE;
		}
	}

	// set group info.
	sensor_set_group_info(sensor_gp_map, sensor_subgp_map);

	// set mclk info for SIE MCLK
	for (cnt_id = 0; cnt_id < SENSOR_ID_MAX_NUM; cnt_id++) {
		sen_map_info = drv_sensor_search(sensor_name[cnt_id]);

		if ((strcmp(sensor_name[cnt_id], "CMOS_OFF") != 0) && (sen_map_info != NULL)) {
			rt = sensor_cfg_sie_mclk(cnt_id, sen_map_info->drv_obj(cnt_id), sen_map_info->drv_tab());
			if (rt != E_OK) {
				DBG_ERR("id %d sensor_cfg_sie_mclk error %d\r\n", cnt_id, rt);
			}
			rt = sensor_cfg_sie_max_clk(cnt_id, sen_map_info->drv_obj(cnt_id), sen_map_info->drv_tab());
			if (rt != E_OK) {
				DBG_ERR("id %d sensor_cfg_sie_clk error %d\r\n", cnt_id, rt);
			}
		}
	}

	// set pll clk
	rt = sensor_cfg_pll_clk();
	if (rt != E_OK) {
		DBG_ERR("sensor_cfg_pll_clk %d\r\n", rt);
	}

	return FALSE;
}

BOOL drv_sensor_init(SENSOR_ID id, SENSOR_HANDLE *handle)
{
	SEN_MAP_INFO *sensor_info = drv_sensor_search(sensor_name[id]);

	if (sensor_debug_item[id] & SENSOR_DEBUG_MODELEXT) {
		debug_msg("^C%s id %d\r\n", __func__, id);
	}

	if (sensor_info == NULL) {
		DBG_ERR("driver get fail\r\n");
		return FALSE;
	}

	if ((sensor_info->drv_tab == NULL) || (sensor_info->drv_obj == NULL)) {
		DBG_ERR("fp NULL fail\r\n");
		return FALSE;
	}

	if (handle != NULL) {
		handle->drv_tab = sensor_info->drv_tab();
		handle->ini_obj = sensor_info->drv_obj(id);
		if (handle->drv_tab == NULL) {
			DBG_ERR("input parame error 0x%.8x\r\n", handle->drv_tab);
		} else {
			return TRUE;
		}
	} else {
		DBG_ERR("input parame error 0x%.8x\r\n", handle);
	}

	return FALSE;
}

void drv_sensor_power_turn_on(SENSOR_ID id)
{
	SEN_MAP_INFO *sensor_info = drv_sensor_search(sensor_name[id]);

	if (sensor_debug_item[id] & SENSOR_DEBUG_MODELEXT) {
		debug_msg("^C%s id %d\r\n", __func__, id);
	}

	if (sensor_info == NULL) {
		DBG_ERR("driver get fail\r\n");
		return;
	}

	if (sensor_info->pow_ctrl == NULL) {
		DBG_IND("driver NULL\r\n");
		return;
	}

	if (sensor_chk_power_on(id)) {
		sensor_info->pow_ctrl(id, POW_CTRL_TURN_ON);
	}
}

void drv_sensor_power_turn_off(SENSOR_ID id)
{
	SEN_MAP_INFO *sensor_info = drv_sensor_search(sensor_name[id]);

	if (sensor_debug_item[id] & SENSOR_DEBUG_MODELEXT) {
		debug_msg("^C%s id %d\r\n", __func__, id);
	}

	if (sensor_info == NULL) {
		DBG_ERR("driver get fail\r\n");
		return;
	}

	if (sensor_info->pow_ctrl == NULL) {
		DBG_IND("driver NULL\r\n");
		return;
	}

	if (sensor_chk_power_off(id)) {
		sensor_info->pow_ctrl(id, POW_CTRL_TURN_OFF);
	}
}

void drv_sensor_power_save_on(SENSOR_ID id)
{
	SEN_MAP_INFO *sensor_info = drv_sensor_search(sensor_name[id]);

	if (sensor_debug_item[id] & SENSOR_DEBUG_MODELEXT) {
		debug_msg("^C%s id %d\r\n", __func__, id);
	}

	if (sensor_info == NULL) {
		DBG_ERR("driver get fail\r\n");
		return;
	}

	if (sensor_info->pow_ctrl == NULL) {
		DBG_IND("driver NULL\r\n");
		return;
	}

	if (sensor_chk_power_save_on(id)) {
		sensor_info->pow_ctrl(id, POW_CTRL_SAVE_ON);
	}
}

void drv_sensor_power_save_off(SENSOR_ID id)
{
	SEN_MAP_INFO *sensor_info = drv_sensor_search(sensor_name[id]);

	if (sensor_debug_item[id] & SENSOR_DEBUG_MODELEXT) {
		debug_msg("^C%s id %d\r\n", __func__, id);
	}

	if (sensor_info == NULL) {
		DBG_ERR("driver get fail\r\n");
		return;
	}

	if (sensor_info->pow_ctrl == NULL) {
		DBG_IND("driver NULL\r\n");
		return;
	}

	if (sensor_chk_power_save_off(id)) {
		sensor_info->pow_ctrl(id, POW_CTRL_SAVE_OFF);
	}
}

BOOL drv_sensor_det_plug_in(SENSOR_ID id)
{
	SEN_MAP_INFO *sensor_info = drv_sensor_search(sensor_name[id]);

	if (sensor_debug_item[id] & SENSOR_DEBUG_MODELEXT) {
		debug_msg("^C%s id %d\r\n", __func__, id);
	}

	if (sensor_info == NULL) {
		DBG_ERR("driver get fail\r\n");
		return FALSE;
	}

	if (sensor_info->det_plug_in == NULL) {
		DBG_IND("driver NULL\r\n");
		return FALSE;
	}

	return sensor_info->det_plug_in(id);
}

BOOL drv_sensor_plug_in_cb(SENSOR_ID id)
{
	SEN_MAP_INFO *sensor_info = drv_sensor_search(sensor_name[id]);

	if (sensor_debug_item[id] & SENSOR_DEBUG_MODELEXT) {
		debug_msg("^C%s id %d\r\n", __func__, id);
	}

	if (sensor_info == NULL) {
		DBG_ERR("driver get fail\r\n");
		return FALSE;
	}

	if (sensor_info->plug_in_cb == NULL) {
		DBG_IND("driver NULL\r\n");
		return FALSE;
	}

	return sensor_info->plug_in_cb(id);
}

SEN_PLUG_INFO drv_sensor_det_plug_info(SENSOR_ID id)
{
	SEN_MAP_INFO *sen_map_info = drv_sensor_search(sensor_name[id]);
	SEN_PLUG_INFO plug_info;

	plug_info.resolution.w = 1920;
	plug_info.resolution.h = 1080;
	plug_info.fps = 30;

	if (sensor_debug_item[id] & SENSOR_DEBUG_MODELEXT) {
		debug_msg("^C%s id %d\r\n", __func__, id);
	}

	if (sen_map_info == NULL) {
		DBG_ERR("driver get fail\r\n");
		return plug_info;
	}

	if (sen_map_info->det_plug_info == NULL) {
		DBG_IND("driver NULL\r\n");
		return plug_info;
	}

	return sen_map_info->det_plug_info(id);
}

