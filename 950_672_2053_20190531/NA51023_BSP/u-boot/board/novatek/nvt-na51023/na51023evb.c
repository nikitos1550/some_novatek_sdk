/**
    NVT evb board file
    To handle na51023 basic init.
    @file       na51023evb.c
    @ingroup
    @note
    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/


#include <common.h>
//#include <asm/mach-types.h>
#include <asm/arch/nvt_common.h>
extern void sdio_power_cycle(void);

phys_size_t initdram(int board_type)
{
	return CONFIG_SYS_SDRAM_SIZE;
}

int checkboard(void)
{
	printf("Welcome to uboot\n");
	return 0;
}

DECLARE_GLOBAL_DATA_PTR;
extern void wdt_init(void);
extern int nvt_mmc_init(int id);
extern int nvt_flash_mtdpart_config(char *p_cmdline, EMB_PARTITION *partition_ptr);
extern int na51023_eth_initialize(bd_t *bis);

const char *boardinfo[] = {
	"Board:"_CHIP_NAME_"EVB\n"
};

/**
 * @brief board_early_init_f
 *
 * @return 0
 */
int board_early_init_f(void)
{
	nvt_ipc_init();

#ifdef CONFIG_WATCHDOG
	wdt_init();
#endif

	return 0;
}

#ifdef CONFIG_MMC
int board_mmc_init(bd_t *bis)
{
	int ret = 0, i;

	for (i = 0; i < CONFIG_NVT_MMC_MAX_NUM; i++) {
		if((CONFIG_NVT_MMC_CHANNEL >> i) & 0x1) {
			ret = nvt_mmc_init(i);
			if(ret)
				break;
		}
	}

	return ret;
}
#endif

#if 0
int board_nand_init(bd_t *bis)
{
	return 0;
}
#endif
int board_eth_init(bd_t *bis)
{
    int rc = 0;
#ifdef CONFIG_ETHNET
    rc = na51023_eth_initialize(bis);
#endif
    return rc;
}

#ifdef CONFIG_NVT_LINUX_AUTODETECT
static int nvt_handle_fw_abin(void)
{
	int ret = 0;
	char cmdline[1024] = {0};
	char buf[1024] = {0};

	/* To handle firmware update */
	ret = nvt_fw_update(false);
	if (ret < 0) {
		switch (ret) {
		case ERR_NVT_UPDATE_FAILED:
			printf("%sUpdate fail %s\r\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
			return -1;
			break;
		case ERR_NVT_UPDATE_OPENFAILED:
			printf("Open SD fail:%s No SD device? %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
			break;
		case ERR_NVT_UPDATE_READ_FAILED:
			printf("%sRead SD fail %s\r\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
			return -1;
			break;
		case ERR_NVT_UPDATE_NO_NEED:
			printf("%sNo need to update (%s) %s\r\n", ANSI_COLOR_YELLOW, get_nvt_bin_name(NVT_BIN_NAME_TYPE_FW), ANSI_COLOR_RESET);
			break;
		default:
			break;
		}
		ret = nvt_modelext_init();
		if (ret < 0) {
			printf("modelext init fail\n");
		}
	} else
		printf("%sUpdate successfully %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
	/*
	 * To handle bootargs expanding for the kernel /proc/cmdline and uboot mtdids env setting
	 */
	sprintf(buf,"%s ",getenv("bootargs"));
	strcat(cmdline, buf);
	ret = nvt_flash_mtdpart_config((char *)cmdline, NULL);
	if (ret < 0)
		return ret;

	setenv("bootargs",cmdline);

	return 0;
}

static int nvt_handle_fw_tbin(void)
{
	int ret = 0;

	ret = nvt_fw_load_tbin();
	if (ret < 0) {
		switch (ret) {
		case ERR_NVT_UPDATE_OPENFAILED:
			printf("Open SD fail:%s No SD device? (%s) %s\r\n", ANSI_COLOR_YELLOW, get_nvt_bin_name(NVT_BIN_NAME_TYPE_RUNFW), ANSI_COLOR_RESET);
			break;
		case ERR_NVT_UPDATE_NO_NEED:
		case ERR_NVT_UPDATE_READ_FAILED:
			printf("%sRead SD fail (%s) %s\r\n", ANSI_COLOR_RED, get_nvt_bin_name(NVT_BIN_NAME_TYPE_RUNFW), ANSI_COLOR_RESET);
			return -1;
			break;
		default:
			break;
		}
	} else
		printf("%sLoad successfully %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);

	ret = nvt_modelext_init();
	if (ret < 0) {
		printf("modelext init fail\n");
		return ret;
	}

	return 0;
}

static int nvt_handle_fw_update_fw(void)
{
	int ret = 0;
	unsigned long val = 0;

	printf("%sStarting to update firmware runtime %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
	/* To handle firmware update */
	ret = nvt_fw_update(true);
	if (ret < 0) {
		printf("%sUpdate fail %s\r\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
		val = (nvt_readl((ulong)nvt_binfo_comm_uboot_boot_func) & ~COMM_UBOOT_BOOT_FUNC_BOOT_DONE_MASK) | COMM_UBOOT_BOOT_FUNC_BOOT_NG;
		nvt_writel(val, (ulong)nvt_binfo_comm_uboot_boot_func);
		flush_dcache_range((u32)nvt_binfo_comm_uboot_boot_func, (u32)(nvt_binfo_comm_uboot_boot_func + sizeof(u32)));
		return -1;
	} else {
		printf("%sUpdate firmware successfully %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);

		if(nvt_check_should_rename_updfw()){
			if(nvt_rename_updfw(1))
			{
				printf("fail to rename fw update fw\r\n");
				return -1;
			}
			if(nvt_delete_delfw())
			{
				printf("fail to create delete file\r\n");
				return -1;
			}
		}

		// Update finished.
		val = (nvt_readl((ulong)nvt_binfo_comm_uboot_boot_func) & ~COMM_UBOOT_BOOT_FUNC_BOOT_DONE_MASK) | COMM_UBOOT_BOOT_FUNC_BOOT_DONE;
		nvt_writel(val, (ulong)nvt_binfo_comm_uboot_boot_func);
		flush_dcache_range((u32)nvt_binfo_comm_uboot_boot_func, (u32)(nvt_binfo_comm_uboot_boot_func + sizeof(u32)));
		while(1) {
			// Waiting for itron trigger reboot.
			printf(".");
			mdelay(1000);
		}
	}

	return 0;
}

static int nvt_handle_update_fw_by_usb_eth(void)
{
	int ret = 0;
	char cmdline[1024] = {0};
	char buf[1024] = {0};

	printf("%sStarting to update firmware from USB/ETH%s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
	/* To handle firmware update */
	ret = nvt_fw_update(true);
	if (ret < 0) {
		printf("%sUpdate fail %s\r\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
		return -1;
	} else {
		printf("%sUpdate firmware successfully %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
	}

	/*
	 * To handle bootargs expanding for the kernel /proc/cmdline and uboot mtdids env setting
	 * Continue to boot
	 */
	sprintf(buf,"%s ",getenv("bootargs"));
	strcat(cmdline, buf);
	ret = nvt_flash_mtdpart_config((char *)cmdline, NULL);
	if (ret < 0)
		return ret;

	setenv("bootargs",cmdline);
	return 0;
}

static int nvt_handle_fw_auto(void)
{
	int ret = 0;
#ifndef CONFIG_NVT_LINUX_RAMDISK_BOOT
	char cmdline[1024] = {0};
	char buf[1024] = {0};
#endif
	unsigned long boot_reason = nvt_readl((ulong)nvt_binfo_comm_uboot_boot_func) & COMM_UBOOT_BOOT_FUNC_BOOT_REASON_MASK;

	if (boot_reason == COMM_UBOOT_BOOT_FUNC_BOOT_UPDFIRM) {
		// Update by itron: firmware will be loaded by itron.
		ret = nvt_handle_fw_update_fw();
		if (ret < 0)
			return ret;
	} else if ((boot_reason == COMM_UBOOT_BOOT_FUNC_BOOT_UPD_FRM_USB) || \
			(boot_reason == COMM_UBOOT_BOOT_FUNC_BOOT_UPD_FRM_ETH)){
		// Update by USB/ETH: firmware will be loaded by loader.
		ret = nvt_handle_update_fw_by_usb_eth();
		if (ret < 0)
			return ret;
	} else if (nvt_detect_fw_tbin()) {
		ret = nvt_handle_fw_tbin();
		if (ret < 0)
			return ret;

		if (nvt_get_cpu_os_type() == CPU2_OS_TYPE_ECOS)
			return 0;
		#ifndef CONFIG_NVT_LINUX_RAMDISK_BOOT
		/*
		 * To handle bootargs expanding for the kernel /proc/cmdline and uboot mtdids env setting
		 */
		//sprintf(buf,"%s ",getenv("bootargs"));
		sprintf(buf,"%s ",CONFIG_BOOTARGS);
		strcat(cmdline, buf);
		ret = nvt_flash_mtdpart_config((char *)cmdline, NULL);
		if (ret < 0)
			return ret;

		setenv("bootargs",cmdline);
		#endif /* CONFIG_NVT_LINUX_RAMDISK_BOOT */
	} else {
		ret = nvt_handle_fw_abin();
		if (ret < 0)
			return ret;
	}

	return 0;
}
#endif /* CONFIG_NVT_LINUX_AUTODETECT */

/**
 * @brief misc_init_r - To do nvt update and board init.
 *
 * @return 0
 */
int misc_init_r(void)
{
	int ret = 0;

	printf("%sFirmware name: %s %s %s %s \n", ANSI_COLOR_YELLOW,
						  get_nvt_bin_name(NVT_BIN_NAME_TYPE_FW),
						  get_nvt_bin_name(NVT_BIN_NAME_TYPE_RUNFW),
						  get_nvt_bin_name(NVT_BIN_NAME_TYPE_MODELEXT),
						  ANSI_COLOR_RESET);
#ifdef CONFIG_NVT_LINUX_AUTOLOAD
	/* Get boot parameters from loader or itron */
	nvt_modelext_bininfo_init();
#ifdef CONFIG_NVT_LINUX_AUTODETECT
	ret = nvt_handle_fw_auto();
	if (ret < 0)
		return ret;
#else /* !CONFIG_NVT_LINUX_AUTODETECT */
#if (defined(CONFIG_NVT_LINUX_SPINAND_BOOT) || defined(CONFIG_NVT_LINUX_SPINOR_BOOT))
	ret = nvt_handle_fw_abin();
	if (ret < 0)
		return ret;
#else /* !CONFIG_NVT_LINUX_SPINAND_BOOT */
	ret = nvt_handle_fw_tbin();
	if (ret < 0)
		return ret;
#endif /* CONFIG_NVT_LINUX_SPINAND_BOOT */
#endif /* CONFIG_NVT_LINUX_AUTODETECT */
#else /* !CONFIG_NVT_LINUX_AUTOLOAD */
	/* FIXME: To do customized boot */
	ret = nvt_modelext_init();
	if (ret < 0) {
		printf("modelext init fail\n");
		return ret;
	}
#endif /* CONFIG_NVT_LINUX_AUTOLOAD */

	/* Why to do this again?
	 * This is because modelext maybe updated, we should init again here.
	 */

	sdio_power_cycle();

	ret = nvt_board_init();
	if (ret < 0) {
		printf("board init fail\n");
		return ret;
	}

	return 0;
}

/*
 * dram init.
 */
//void dram_init_banksize(void)
//{
//	gd->bd->bi_memstart = PHYS_SDRAM_1;
//	gd->bd->bi_memsize = PHYS_SDRAM_1_SIZE;
//}

/*
 * get_board_rev() - get board revision
 */
u32 get_board_rev(void)
{
	return 0;
}

int board_late_init(void)
{
	return 0;
}
