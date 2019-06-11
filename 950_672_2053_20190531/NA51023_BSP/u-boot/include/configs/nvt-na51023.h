/*
 * Copyright (C) 2016 Novatek Microelectronics Corp. All rights reserved.
 * Author: iVoT-IM <iVoT_MailGrp@novatek.com.tw>
 *
 * Configuration settings for the Novatek NA51023 SOC.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CONFIG_NA51023_H
#define __CONFIG_NA51023_H
#include <linux/sizes.h>

//#define DEBUG										1
#define CONFIG_NVT_FW_UPDATE_LED
//#define CONFIG_NVT_PWM

//#define CONFIG_FPGA_EMULATION								1
#define CONFIG_MIPS32									1	/* CPU core */
#define CONFIG_NA51023									1	/* SoC */
#define CONFIG_NVT_BOARD								1
//#define CONFIG_SKIP_LOWLEVEL_INIT							1	/* Do not define this, we need to map 0xc0000000 via TLB in lowlevel_init.s */
//#define CONFIG_NAND_TYPE_ONFI								0
#define CONFIG_NAND_TYPE_SPINAND							1
#define CONFIG_NAND_TYPE_SPINOR								2

#if defined(_FORCE_FLASH_DUAL_ON_)
#define CONFIG_FLASH_ONLY_DUAL
#endif

#define IPC_ENABLE                                      1
#if defined(_CLOCK_DRTC_)
#define CONFIG_NVT_DRTC
#endif

/* What is the oscillator's frequency in MHz? */
#ifdef CONFIG_FPGA_EMULATION
#define CONFIG_SYS_CLKIN								24
#else
#define CONFIG_SYS_CLKIN								240
#endif

//Original
#ifdef CONFIG_FPGA_EMULATION
#define CONFIG_CPU_MHZ									48 		/* pll setting for CPU core */
#else
#define CONFIG_CPU_MHZ									640
#endif
#define CONFIG_SYS_MIPS_TIMER_FREQ							((CONFIG_CPU_MHZ * 1000000) / 2)
#define CONFIG_SYS_HZ									1000

/*-----------------------------------------------------------------------
 * PWM Configuration
 */
//#define CONFIG_PWM
#ifdef CONFIG_PWM
#define CONFIG_PWM_PERIOD00   1000000 //ns
#define CONFIG_PWM_PERIOD01   1000000 //ns
#define CONFIG_PWM_PERIOD02   1000000 //ns
#define CONFIG_PWM_PERIOD03   1000000 //ns
#define CONFIG_PWM_PERIOD04   1000000 //ns
#define CONFIG_PWM_PERIOD05   1000000 //ns
#define CONFIG_PWM_PERIOD06   1000000 //ns
#define CONFIG_PWM_PERIOD07   1000000 //ns
#define CONFIG_PWM_PERIOD08   1000000 //ns
#define CONFIG_PWM_PERIOD09   1000000 //ns
#define CONFIG_PWM_PERIOD10   1000000 //ns
#define CONFIG_PWM_PERIOD11   1000000 //ns
#define CONFIG_PWM_PERIOD12   1000000 //ns
#define CONFIG_PWM_PERIOD13   1000000 //ns
#define CONFIG_PWM_PERIOD14   1000000 //ns
#define CONFIG_PWM_PERIOD15   1000000 //ns
#define CONFIG_PWM_DUTY00      500000  //ns
#define CONFIG_PWM_DUTY01      500000  //ns
#define CONFIG_PWM_DUTY02      500000  //ns
#define CONFIG_PWM_DUTY03      500000  //ns
#define CONFIG_PWM_DUTY04      500000  //ns
#define CONFIG_PWM_DUTY05      500000  //ns
#define CONFIG_PWM_DUTY06      500000  //ns
#define CONFIG_PWM_DUTY07      500000  //ns
#define CONFIG_PWM_DUTY08      500000  //ns
#define CONFIG_PWM_DUTY09      500000  //ns
#define CONFIG_PWM_DUTY10      500000  //ns
#define CONFIG_PWM_DUTY11      500000  //ns
#define CONFIG_PWM_DUTY12      500000  //ns
#define CONFIG_PWM_DUTY13      500000  //ns
#define CONFIG_PWM_DUTY14      500000  //ns
#define CONFIG_PWM_DUTY15      500000  //ns
#endif

/*-----------------------------------------------------------------------
 * RTC Default Date
 */
#define RTC_YEAR 2000
#define RTC_MONTH 1
#define RTC_DAY 1


/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE								32768
#define CONFIG_SYS_ICACHE_SIZE								32768
#define CONFIG_SYS_CACHELINE_SIZE							32

/*-----------------------------------------------------------------------
 * Serial console configuration
 */
#define CONSOLE_UART1	0
#define CONSOLE_UART2	1
#define CONSOLE_UART3	2
#define CONSOLE_UART4	3

#define CONFIG_SYS_UART									CONSOLE_UART2 // Select UARTx
#define CONFIG_SYS_BAUDRATE_TABLE							{ 9600, 19200, 38400, 57600, 115200 }
#define CONFIG_BAUDRATE									115200

/*
 * UART2_1 UART2_TX/RX
 * UART2_2 C_GPIO13/C_GPIO14
 * UART3_1 P_GPIO15/P_GPIO16
 * UART3_2 P_GPIO8/P_GPIO9
 * UART4_1 P_GPIO4/P_GPIO5
 * UART4_2 C_GPIO5/C_GPIO6
 */
#define PINMUX_CHANNEL_1	0
#define PINMUX_CHANNEL_2	1

#define UART_PINMUX_SEL	PINMUX_CHANNEL_1

/*-----------------------------------------------------------------------
 * NVT LED CONFIG
 *
 * LED GPIO selection
 * C_GPIO(x)
 * P_GPIO(x)
 * S_GPIO(x)
 * L_GPIO(x)
 * D_GPIO(x)
 * Duration Unit: ms
 */
#ifdef CONFIG_NVT_FW_UPDATE_LED
#ifdef CONFIG_NVT_PWM
#define PWM_SIGNAL_NORMAL 0
#define PWM_SIGNAL_INVERT 1
#define NVT_PWMLED (PWMID_0 | PWMID_2)
#define PWM_SIGNAL_TYPE PWM_SIGNAL_INVERT
#define	PWM_LED_ERASE 50
#define	PWM_LED_PROGRAM 5
#else
#define NVT_LED_PIN P_GPIO(0)
#define NVT_LED_ERASE_DURATION 30
#define NVT_LED_PROGRAM_DURATION 10
#endif
#endif


/* Enable GPIO Usage*/
/*#define CONFIG_NA51023_GPIO*/

/*-----------------------------------------------------------------------
 * To include nvt memory layout
 */
#include "novatek/na51023_mips24k_fpga.h"
#ifndef _CPU2_LINUX_
#define CONFIG_SYS_SDRAM_BASE								CONFIG_UBOOT_SDRAM_BASE
#define CONFIG_SYS_SDRAM_SIZE								CONFIG_UBOOT_SDRAM_SIZE
#else
#define CONFIG_SYS_SDRAM_BASE								CONFIG_LINUX_SDRAM_BASE
#define CONFIG_SYS_SDRAM_SIZE								CONFIG_LINUX_SDRAM_SIZE + CONFIG_UBOOT_SDRAM_SIZE
#endif

#define NVT_LINUX_BOOT_PARAM_ADDR							(CONFIG_LINUX_SDRAM_BASE + 0x100)

//#define CONFIG_SYS_INIT_SP_ADDR								(CONFIG_UBOOT_SDRAM_BASE + CONFIG_UBOOT_SDRAM_SIZE - 0x1000)
#define CONFIG_SYS_MALLOC_LEN								(3 * SZ_1M)
#define CONFIG_SYS_MONITOR_BASE								CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_MONITOR_LEN								SZ_512K

/*-----------------------------------------------------------------------
 * IP address configuration
 */
#define CONFIG_FIXED_ETH_PARAMETER
#define CONFIG_OVERWRITE_ETHADDR_ONCE

#if defined(_NVT_ETHERNET_EQOS_)
#define CONFIG_ETHNET
#define CONFIG_ETHADDR									0a:0b:0c:0d:0e:01
#define CONFIG_IPADDR									192.168.1.99	/* Target IP address */
#define CONFIG_NETMASK									255.255.255.0
#define CONFIG_SERVERIP									192.168.1.11	/* Server IP address */
#define CONFIG_GATEWAYIP								192.168.1.254
#define CONFIG_HOSTNAME									soclnx

/*-----------------------------------------------------------------------
 * BOOTP options
 */
#define CONFIG_BOOTP_SUBNETMASK
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_BOOTFILESIZE

/*-----------------------------------------------------------------------
 * Others
 */
#define CONFIG_PHYLIB
#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_NET
//#define CONFIG_NET_MULTI
//#define CONFIG_NETCONSOLE
#endif /* _NVT_ETHERNET_EQOS_ */
/*-----------------------------------------------------------------------
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP	/* undef to save memory */
#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "

/* Configure Linux kernel compression type support selection */
#define CONFIG_LZO
//#define CONFIG_BZIP2
#define CONFIG_LZMA

#define CONFIG_CMD_MEMORY

#define CONFIG_SYS_PROMPT								"NA51023> "	/* Monitor Command Prompt */
#define CONFIG_SYS_CBSIZE								1024			/* Console I/O Buffer Size */
#define CONFIG_SYS_PBSIZE 								(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)  /* Print Buffer Size */
#define CONFIG_SYS_MAXARGS								64			/* max number of command args */

#define CONFIG_SYS_BOOTPARAMS_LEN							4096

#define CONFIG_SYS_LOAD_ADDR								0 //not used currently	/* default linux kernel load address (uImage)*/

#define CONFIG_SYS_MEMTEST_START							0 //not used currently
#define CONFIG_SYS_MEMTEST_END								0 //not used currently

#undef CONFIG_MEMSIZE_IN_BYTES

#define CONFIG_MISC_INIT_R


/*-----------------------------------------------------------------------
 * NVT related setting
 *
 */
#define CONFIG_BOOTARGS_COMMON								"earlyprintk console=ttyS0,115200 rootwait "

/* NVT boot related setting */
#define CONFIG_NVT_LINUX_AUTOLOAD						/* Support for uboot autoboot (loading and boot FWxxxxxT.bin[for RAMDISK] or FWxxxxxA.bin[for mtd device]) */
#ifdef CONFIG_NVT_LINUX_AUTOLOAD
    #define CONFIG_NVT_LINUX_AUTODETECT						/* Support for detect FWxxxxxA.bin/FWxxxxxT.bin automatically. (Only working on mtd device boot method) */
	#define CONFIG_NVT_BIN_CHKSUM_SUPPORT					/* This option will check rootfs/uboot checksum info. during update image flow */
	#if defined(_NVT_ROOTFS_TYPE_RAMDISK_)
		/* the ramdisk dram base/size will be defined in itron modelext info. */
		#define CONFIG_NVT_LINUX_RAMDISK_BOOT 				/* Loading ramdisk image rootfs.bin from SD card */
		#define CONFIG_BOOTARGS 		CONFIG_BOOTARGS_COMMON "root=/dev/ram0 rootfstype=ramfs rdinit=/linuxrc "
	#elif defined(_NVT_ROOTFS_TYPE_NAND_UBI_)					/* UBIFS rootfs boot */
		#define CONFIG_NVT_LINUX_SPINAND_BOOT
		#define CONFIG_BOOTARGS			CONFIG_BOOTARGS_COMMON "root=ubi0:rootfs rootfstype=ubifs rw ubi.fm_autoconvert=1 init=/linuxrc "
		#define CONFIG_CMD_UBI						/* UBI-formated MTD partition support */
		#define CONFIG_CMD_UBIFS					/* Read-only UBI volume operations */
	#elif defined(_NVT_ROOTFS_TYPE_NOR_SQUASH_)				/* SquashFs rootfs boot */
		#define CONFIG_NVT_LINUX_SPINOR_BOOT
		#define CONFIG_BOOTARGS			CONFIG_BOOTARGS_COMMON "rootfstype=squashfs ro "
	#elif defined(_NVT_ROOTFS_TYPE_NOR_JFFS2_)				/* JFFS2 rootfs boot */
		#define CONFIG_NVT_LINUX_SPINOR_BOOT
		#define CONFIG_BOOTARGS			CONFIG_BOOTARGS_COMMON "rootfstype=jffs2 rw rootwait "
	#elif defined(_NVT_ROOTFS_TYPE_NAND_JFFS2_)				/* JFFS2 rootfs boot */
		#define CONFIG_NVT_LINUX_SPINAND_BOOT				/* Boot from spinand or spinor (Support FWxxxxxA.bin update all-in-one) */
		#define CONFIG_CMD_NAND_TRIMFFS
		#define CONFIG_BOOTARGS			CONFIG_BOOTARGS_COMMON "rootfstype=jffs2 rw rootwait "
	#else
		#define CONFIG_NVT_LINUX_SD_BOOT				/* To handle RAW SD boot (e.g. itron.bin, uImage.bin, uboot.bin...) itron.bin u-boot.bin dsp.bin dsp2.bin must be not compressed.*/
		#define CONFIG_BOOTARGS 		CONFIG_BOOTARGS_COMMON "root=/dev/mmcblk0p2 noinitrd rootfstype=ext3 init=/linuxrc "
	#endif /* _NVT_ROOTFS_TYPE_ */
#endif /* CONFIG_NVT_LINUX_AUTOLOAD */

#if defined(_DSP1_FREERTOS_)
#define CONFIG_DSP1_FREERTOS
#endif


#define CONFIG_NVT_MMC								/* Support MMC */
//#define CONFIG_NVT_LOAD_LINUX_IMAGE							/* Enable this option should sync with iTron; To do linux bin loading and boot */

/*-----------------------------------------------------------------------
 * Physical memory map
 *
 */
#define CONFIG_SYS_FLASH_BASE								0x0 /* not used currently */
//#define CONFIG_RAMOOPS_LOG

/*-----------------------------------------------------------------------
 * Stack sizes
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE								(256 * 1024)	/* regular stack */
//#define CONFIG_SYS_INIT_SP_OFFSET	(CONFIG_SYS_TEXT_BASE + 0x00200000 - CONFIG_SYS_SDRAM_BASE) //here must avoid overlaping uItron memory area. we use address of u-boot area + 0x200000

/*-----------------------------------------------------------------------
 * environment setting
 */

#define CONFIG_BOOTDELAY								0//1	/* autoboot after 0 seconds */
//#define CONFIG_AUTOBOOT_KEYED								1
//#define CONFIG_AUTOBOOT_STOP_STR							" "
//#define CONFIG_AUTOBOOT_PROMPT							"Press SPACE to abort autoboot in %d seconds\n", bootdelay
//#define CONFIG_ZERO_BOOTDELAY_CHECK

/* WATCHDOG*/
/*#define CONFIG_NVT_WDT*/
#ifdef CONFIG_NVT_WDT
#define CONFIG_WATCHDOG
#endif

/* CRYPTO */
#define CONFIG_MD5

/* MMC */
#ifdef CONFIG_NVT_MMC
#define CONFIG_MMC
#define CONFIG_CMD_MMC
#define CONFIG_CMD_FAT
#define CONFIG_FS_FAT
#define CONFIG_CMD_EXFAT
#define CONFIG_FS_EXFAT
//#define CONFIG_FS_EXT4
#define CONFIG_DOS_PARTITION
#define CONFIG_GENERIC_MMC
#define CONFIG_SDHCI
/*#define CONFIG_SD_CARD1_POWER_PIN			P_GPIO(0)
#define CONFIG_SD_CARD1_ON_STATE			0
#define CONFIG_SD_CARD2_POWER_PIN			P_GPIO(1)
#define CONFIG_SD_CARD2_ON_STATE			0*/
//#define CONFIG_SUPPORT_EMMC_BOOT
#define CONFIG_NVT_MMC_CHANNEL								1
#define CONFIG_NVT_MMC_MAX_NUM								3
/* NVT MMC Channel Matrix
SDIO   BIT 2  1  0
1          0  0  1
2          0  1  0
3          1  0  0

example:
if enabling SDIO 1 and 3, CONFIG_NVT_MMC_CHANNEL would be BIT[1, 0, 1], which is 5 */
#endif

/* FAT */
#if defined(CONFIG_FS_FAT)
#define CONFIG_FAT_WRITE
#endif

/* EXFAT */
#if defined(CONFIG_FS_EXFAT)
#define CONFIG_EXFAT_WRITE
#endif

#define NVT_FLASH_SOURCE_CLK								480000000
#if defined(_EMBMEM_SPI_NAND_)
/*
 *	NANDCTRL_SPIFLASH_USE_INTERNAL_RS_ECC
 *	NANDCTRL_SPIFLASH_USE_ONDIE_ECC
*/
#define CONFIG_NVT_NAND_ECC								NANDCTRL_SPIFLASH_USE_ONDIE_ECC
#define CONFIG_NVT_SPI_NAND
#define CONFIG_NVT_NAND_TYPE								CONFIG_NAND_TYPE_SPINAND
#define CONFIG_CMD_NAND
#define CONFIG_SYS_MAX_NAND_DEVICE							1
#define CONFIG_CMD_NAND_TRIMFFS
#elif defined(_EMBMEM_SPI_NOR_)
#define CONFIG_NVT_SPI_NOR
#define CONFIG_NVT_NAND_TYPE								CONFIG_NAND_TYPE_SPINOR
#define CONFIG_CMD_SF
#define CONFIG_SPI_FLASH
#define CONFIG_MTD_DEVICE
#define CONFIG_SPI_FLASH_MACRONIX
#define CONFIG_SPI_FLASH_WINBOND
#define CONFIG_SPI_FLASH_GIGADEVICE
#define CONFIG_SPI_FLASH_STMICRO
#define CONFIG_SPI_FLASH_EON
#define CONFIG_SYS_MAX_SF_DEVICE							1
#define CONFIG_SPI_FLASH_MTD
#define CONFIG_SF_DEFAULT_BUS								0
#define CONFIG_SF_DEFAULT_CS								0
#define CONFIG_SF_DEFAULT_MODE								0
#define CONFIG_SF_DEFAULT_SPEED								48000000
#define CONFIG_ENV_SECT_SIZE							    (64 << 10)      	/* Define the SPI flash's sector size */
#endif

#ifdef CONFIG_CMD_UBIFS
#define CONFIG_LZO
#define CONFIG_RBTREE
#endif
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define CONFIG_CMD_MTDPARTS

#define CONFIG_TIMESTAMP								/* Print image info with timestamp */

#define CONFIG_SYS_STBC_LEN								(64 << 10)
#define CONFIG_SYS_ROM_LEN								(32 << 10)

/*
 * We typically do not contain NOR flash.  In the cases where we do, we
 * undefine this later.
 */
#define CONFIG_SYS_NO_FLASH
#undef  CONFIG_CMD_IMLS
#undef  CONFIG_SYS_HUSH_PARSER
#undef  CONFIG_MODEM_SUPPORT

#if defined(_NVT_UBOOT_ENV_IN_STORG_SUPPORT_NAND_)
#define CONFIG_CMD_SAVEENV
#define CONFIG_ENV_IS_IN_NAND								1
#define CONFIG_ENV_SIZE									(128 << 10)			/* Unit: Block size: 128 KiB */
#define CONFIG_ENV_OFFSET								0x00B00000			/* Defined by iTron PartitionInfo.c */
#define CONFIG_ENV_RANGE								2 * CONFIG_ENV_SIZE /* Defined by iTron PartitionInfo.c */
#elif defined(_NVT_UBOOT_ENV_IN_STORG_SUPPORT_NOR_)
#define CONFIG_CMD_SAVEENV
#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_SIZE									0x00010000      	/* Sync to PartitionInfo.c */
#define CONFIG_ENV_OFFSET								0x00630000      	/* It must be aligned to an erase secrote boundary */
#else
#define CONFIG_ENV_IS_NOWHERE 1
#define CONFIG_ENV_SIZE			                        (8 << 10)
#endif

#if defined(CONFIG_NVT_MMC_BOOT)
#define CONFIG_RAMDISK_SIZE								(20 * SZ_1M)
#define CONFIG_IMAGE_FORMAT_LEGACY
#define CONFIG_BOOTCOMMAND								"nvt_boot_emmc;nvt_bootm"
#else
#define CONFIG_BOOTCOMMAND								"nvt_boot;nvt_bootm"
#endif
#endif /* __CONFIG_NA51023_H */
