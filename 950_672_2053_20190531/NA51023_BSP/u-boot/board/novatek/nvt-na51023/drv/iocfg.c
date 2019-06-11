/*
	io module driver.

	This file is the driver of io module.

	@file		iocfg.c
	@ingroup
	@note		Nothing.

	Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#include <common.h>
#include <asm/io.h>
#include <modelext/gpio_info.h>
#include "iocfg_int.h"


static int pad_set_pull(u32 pin, PAD_PULL pulltype)
{
	u32 dw_ofs, bit_ofs, reg_data;

	if (pin > PAD_PIN_MAX)
		return E_NOEXS;

	if (pin == PAD_PIN_NOT_EXIST) {
		printf("Not Existed Pad Pin\n");
		return E_NOEXS;
	}

	bit_ofs = pin & 0x1F;
	dw_ofs = (pin >> 5);

	reg_data = PAD_GETREG(PAD_PUPD0_REG_OFS + (dw_ofs << 2));
	reg_data &= ~(3 << bit_ofs);
	reg_data |= (pulltype << bit_ofs);
	PAD_SETREG(PAD_PUPD0_REG_OFS + (dw_ofs << 2), reg_data);
	return E_OK;
}

static void gpio_set_dir(u32 pin, GPIO_DIR dir)
{
	u32 reg_data;
	u32 ofs = (pin >> 5) << 2;

	pin &= (32 - 1);

	reg_data = GPIO_GETREG(GPIO_STRG_DIR_REG_OFS + ofs);

	if (dir) {
		reg_data |= (1 << pin);    //output
	} else {
		reg_data &= ~(1 << pin);    //input
	}

	GPIO_SETREG(GPIO_STRG_DIR_REG_OFS + ofs, reg_data);
}

static void gpio_set_pin(u32 pin)
{
	u32 tmp;
	u32 ofs = (pin >> 5) << 2;

	pin &= (32 - 1);
	tmp = (1 << pin);

	GPIO_SETREG(GPIO_STRG_SET0_REG_OFS + ofs, tmp);
}

static void gpio_clear_pin(u32 pin)
{
	u32 tmp;
	u32 ofs = (pin >> 5) << 2;

	pin &= (32 - 1);
	tmp = (1 << pin);

	GPIO_SETREG(GPIO_STRG_CLR0_REG_OFS + ofs, tmp);
}

void sdio_power_cycle(void)
{
#ifdef CONFIG_SD_CARD1_POWER_PIN
	gpio_set_dir(CONFIG_SD_CARD1_POWER_PIN, GPIO_DIR_OUTPUT);
	if (CONFIG_SD_CARD1_ON_STATE)
		gpio_clear_pin(CONFIG_SD_CARD1_POWER_PIN);
	else
		gpio_set_pin(CONFIG_SD_CARD1_POWER_PIN);

	*(u32*) 0xC00100A0 |= 0x3F000;
	*(u32*) 0xC0030000 &= ~0xFF000000;
	*(u32*) 0xC0030004 &= ~0xF;
	*(u32*) 0xC0030000 |= 0x55000000;
	*(u32*) 0xC0030004 |= 0x5;
	gpio_set_dir(C_GPIO(12), GPIO_DIR_OUTPUT);
	gpio_set_dir(C_GPIO(13), GPIO_DIR_OUTPUT);
	gpio_set_dir(C_GPIO(14), GPIO_DIR_OUTPUT);
	gpio_set_dir(C_GPIO(15), GPIO_DIR_OUTPUT);
	gpio_set_dir(C_GPIO(16), GPIO_DIR_OUTPUT);
	gpio_set_dir(C_GPIO(17), GPIO_DIR_OUTPUT);
	gpio_clear_pin(C_GPIO(12));
	gpio_clear_pin(C_GPIO(13));
	gpio_clear_pin(C_GPIO(14));
	gpio_clear_pin(C_GPIO(15));
	gpio_clear_pin(C_GPIO(16));
	gpio_clear_pin(C_GPIO(17));
#endif

#ifdef CONFIG_SD_CARD2_POWER_PIN
	gpio_set_dir(CONFIG_SD_CARD2_POWER_PIN, GPIO_DIR_OUTPUT);
	if (CONFIG_SD_CARD2_ON_STATE)
		gpio_clear_pin(CONFIG_SD_CARD2_POWER_PIN);
	else
		gpio_set_pin(CONFIG_SD_CARD2_POWER_PIN);

	*(u32*) 0xC00100A0 |= 0xFC0000;
	*(u32*) 0xC0030004 &= ~0xFFF0;
	*(u32*) 0xC0030004 |= 0x5550;
	gpio_set_dir(C_GPIO(18), GPIO_DIR_OUTPUT);
	gpio_set_dir(C_GPIO(19), GPIO_DIR_OUTPUT);
	gpio_set_dir(C_GPIO(20), GPIO_DIR_OUTPUT);
	gpio_set_dir(C_GPIO(21), GPIO_DIR_OUTPUT);
	gpio_set_dir(C_GPIO(22), GPIO_DIR_OUTPUT);
	gpio_set_dir(C_GPIO(23), GPIO_DIR_OUTPUT);
	gpio_clear_pin(C_GPIO(18));
	gpio_clear_pin(C_GPIO(19));
	gpio_clear_pin(C_GPIO(20));
	gpio_clear_pin(C_GPIO(21));
	gpio_clear_pin(C_GPIO(22));
	gpio_clear_pin(C_GPIO(23));
#endif
}

void iocfg_init(GPIO_INIT_OBJ *gpio_map_initable, u32 gpio_count)
{
	u32 value;

	for (value = 0 ; value < gpio_count ; value++) {
		if (gpio_map_initable[value].GpioDir == GPIO_DIR_INPUT) {
			gpio_set_dir(gpio_map_initable[value].GpioPin, GPIO_DIR_INPUT);
			pad_set_pull(gpio_map_initable[value].PadPin, gpio_map_initable[value].PadDir);
		} else {
			gpio_set_dir(gpio_map_initable[value].GpioPin, GPIO_DIR_OUTPUT);
			if (gpio_map_initable[value].PadDir == GPIO_SET_OUTPUT_HI) {
				gpio_set_pin(gpio_map_initable[value].GpioPin);
			} else {
				gpio_clear_pin(gpio_map_initable[value].GpioPin);
			}
		}
	}
}
