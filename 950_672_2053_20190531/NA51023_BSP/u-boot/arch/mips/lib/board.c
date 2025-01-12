/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <malloc.h>
#include <serial.h>
#include <stdio_dev.h>
#include <version.h>
#include <net.h>
#include <environment.h>
#include <nand.h>
#include <onenand_uboot.h>
#include <spi.h>
#include <mmc.h>
#include <spi_flash.h>
#include <asm/arch/nvt_common.h>

#ifdef CONFIG_NA51023
#include <asm/arch/na51023_time.h>
#endif

#ifdef CONFIG_BITBANGMII
#include <miiphy.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

ulong monitor_flash_len;

static char *failed = "*** failed ***\n";

int __board_early_init_f(void)
{
	/*
	 * Nothing to do in this dummy implementation
	 */
	return 0;
}
int board_early_init_f(void)
	__attribute__((weak, alias("__board_early_init_f")));

static int init_func_ram(void)
{
#ifdef	CONFIG_BOARD_TYPES
	int board_type = gd->board_type;
#else
	int board_type = 0;	/* use dummy arg */
#endif
	puts("DRAM:  ");

	gd->ram_size = initdram(board_type);
	if (gd->ram_size > 0) {
		print_size(gd->ram_size, "\n");
		return 0;
	}
	puts(failed);
	return 1;
}

static int display_banner(void)
{

	printf("\n\n%s\n\n", version_string);
	return 0;
}

#ifndef CONFIG_SYS_NO_FLASH
static void display_flash_config(ulong size)
{
	puts("Flash: ");
	print_size(size, "\n");
}
#endif

static int init_baudrate(void)
{
	gd->baudrate = getenv_ulong("baudrate", 10, CONFIG_BAUDRATE);
	return 0;
}


/*
 * Breath some life into the board...
 *
 * The first part of initialization is running from Flash memory;
 * its main purpose is to initialize the RAM so that we
 * can relocate the monitor code to RAM.
 */

/*
 * All attempts to come up with a "common" initialization sequence
 * that works for all boards and architectures failed: some of the
 * requirements are just _too_ different. To get rid of the resulting
 * mess of board dependend #ifdef'ed code we now make the whole
 * initialization sequence configurable to the user.
 *
 * The requirements for any new initalization function is simple: it
 * receives a pointer to the "global data" structure as it's only
 * argument, and returns an integer return code, where 0 means
 * "continue" and != 0 means "fatal error, hang the system".
 */
typedef int (init_fnc_t)(void);

init_fnc_t *init_sequence[] = {
	board_early_init_f,
	timer_init,
	env_init,		/* initialize environment */
	init_baudrate,		/* initialize baudrate settings */
	serial_init,		/* serial communications setup */
	console_init_f,
	display_banner,		/* say that we are here */
	checkboard,
	init_func_ram,
	NULL,
};


void board_init_f(ulong bootflag)
{
	gd_t gd_data, *id;
	bd_t *bd;
	init_fnc_t **init_fnc_ptr;
	ulong addr, addr_sp, len;
	ulong *s;

	/* Pointer is writable since we allocated a register for it.
	 */
	gd = &gd_data;
	/* compiler optimization barrier needed for GCC >= 3.4 */
	__asm__ __volatile__("" : : : "memory");

	memset((void *)gd, 0, sizeof(gd_t));

	for (init_fnc_ptr = init_sequence; *init_fnc_ptr; ++init_fnc_ptr) {
		if ((*init_fnc_ptr)() != 0)
			hang();
	}

	/*
	 * Now that we have DRAM mapped and working, we can
	 * relocate the code and continue running from DRAM.
	 */
	addr = CONFIG_SYS_SDRAM_BASE + gd->ram_size;
	/* We can reserve some RAM "on top" here.
	 */

	/* round down to next 4 kB limit.
	 */
	addr &= ~(4096 - 1);
	debug("Top of RAM usable for U-Boot at: %08lx\n", addr);

	/* Reserve memory for U-Boot code, data & bss
	 * round down to next 16 kB limit
	 */
	len = bss_end() - CONFIG_SYS_MONITOR_BASE;
	addr -= len;
	addr &= ~(16 * 1024 - 1);

	debug("Reserving %ldk for U-Boot at: %08lx\n", len >> 10, addr);

	 /* Reserve memory for malloc() arena.
	 */
	addr_sp = addr - TOTAL_MALLOC_LEN;
	debug("Reserving %dk for malloc() at: %08lx\n",
			TOTAL_MALLOC_LEN >> 10, addr_sp);

	/*
	 * (permanently) allocate a Board Info struct
	 * and a permanent copy of the "global" data
	 */
	addr_sp -= sizeof(bd_t);
	bd = (bd_t *)addr_sp;
	gd->bd = bd;
	debug("Reserving %zu Bytes for Board Info at: %08lx\n",
			sizeof(bd_t), addr_sp);

	addr_sp -= sizeof(gd_t);
	id = (gd_t *)addr_sp;
	debug("Reserving %zu Bytes for Global Data at: %08lx\n",
			sizeof(gd_t), addr_sp);

	/* Reserve memory for boot params.
	 */
	addr_sp -= CONFIG_SYS_BOOTPARAMS_LEN;
	bd->bi_boot_params = addr_sp;
	debug("Reserving %dk for boot params() at: %08lx\n",
			CONFIG_SYS_BOOTPARAMS_LEN >> 10, addr_sp);

	/*
	 * Finally, we set up a new (bigger) stack.
	 *
	 * Leave some safety gap for SP, force alignment on 16 byte boundary
	 * Clear initial stack frame
	 */
	addr_sp -= 16;
	addr_sp &= ~0xF;
	s = (ulong *)addr_sp;
	*s-- = 0;
	*s-- = 0;
	addr_sp = (ulong)s;
	debug("Stack Pointer at: %08lx\n", addr_sp);

	/*
	 * Save local variables to board info struct
	 */
	bd->bi_memstart	= CONFIG_SYS_SDRAM_BASE;	/* start of DRAM */
	bd->bi_memsize	= gd->ram_size;		/* size of DRAM in bytes */

	memcpy(id, (void *)gd, sizeof(gd_t));

	relocate_code (addr_sp, id, addr);

	/* NOTREACHED - relocate_code() does not return */
}
#ifdef CONFIG_CMD_SF
/* Probe the SPI flash */

struct spi_flash *board_init_spi_flash = NULL;

int initr_spiflash(void)
{
	puts("SPI:   ");
	board_init_spi_flash = spi_flash_probe(
			CONFIG_SF_DEFAULT_BUS,
			CONFIG_SF_DEFAULT_CS,
			CONFIG_SF_DEFAULT_SPEED,
			CONFIG_SF_DEFAULT_MODE);
	return 0;
}
#endif

/*
 * This is the next part if the initialization sequence: we are now
 * running from RAM and have a "normal" C environment, i. e. global
 * data can be written, BSS has been cleared, the stack size in not
 * that critical any more, etc.
 */

void board_init_r(gd_t *id, ulong dest_addr)
{
#ifndef CONFIG_SYS_NO_FLASH
	ulong size;
#endif
	bd_t *bd;

	gd = id;
	gd->flags |= GD_FLG_RELOC;	/* tell others: relocation done */

	debug("Now running in RAM - U-Boot at: %08lx\n", dest_addr);

	gd->reloc_off = dest_addr - CONFIG_SYS_MONITOR_BASE;

	TM0Init();
	monitor_flash_len = image_copy_end() - dest_addr;

	serial_initialize();

	bd = gd->bd;
	/* The Malloc area is immediately below the monitor copy in DRAM */
	mem_malloc_init(CONFIG_SYS_MONITOR_BASE + gd->reloc_off -
			TOTAL_MALLOC_LEN, TOTAL_MALLOC_LEN);

#ifndef CONFIG_SYS_NO_FLASH
	/* configure available FLASH banks */
	size = flash_init();
	display_flash_config(size);
	bd->bi_flashstart = CONFIG_SYS_FLASH_BASE;
	bd->bi_flashsize = size;
#if CONFIG_SYS_MONITOR_BASE == CONFIG_SYS_FLASH_BASE
	bd->bi_flashoffset = monitor_flash_len;	/* reserved area for U-Boot */
#else
	bd->bi_flashoffset = 0;
#endif
#else
	bd->bi_flashstart = 0;
	bd->bi_flashsize = 0;
	bd->bi_flashoffset = 0;
#endif

#ifdef CONFIG_CMD_NAND
	puts("NAND:  ");
	nand_init();		/* go init the NAND */
#endif

#if defined(CONFIG_CMD_ONENAND)
	onenand_init();
#endif

#ifdef CONFIG_GENERIC_MMC
	puts("MMC:   ");
	mmc_initialize(gd->bd);
#endif

	/* relocate environment function pointers etc. */
	env_relocate();

#if defined(CONFIG_PCI)
	/*
	 * Do pci configuration
	 */
	pci_init();
#endif

/** leave this here (after malloc(), environment and PCI are working) **/
	/* Initialize stdio devices */
	stdio_init();

	jumptable_init();

	/* Initialize the console (after the relocation and devices init) */
	console_init_r();
/** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **/

	/* Initialize from environment */
	load_addr = getenv_ulong("loadaddr", 16, load_addr);

#ifdef CONFIG_CMD_SPI
	puts("SPI:   ");
	spi_init();		/* go init the SPI */
	puts("ready\n");
#endif

#ifdef CONFIG_CMD_SF
	initr_spiflash();
#endif

#if defined(CONFIG_MISC_INIT_R)
	/* miscellaneous platform dependent initialisations */
	misc_init_r();
#endif

#ifdef CONFIG_BITBANGMII
	bb_miiphy_init();
#endif


/* Ethernet will be initialized by console command */
/*
#if defined(CONFIG_CMD_NET)
	puts("Net:   ");
	eth_initialize(gd->bd);
#endif
*/

	/* main_loop() can return to retry autoboot, if so just run it again. */
	for (;;)
		main_loop();

	/* NOTREACHED - no way out of command loop except booting */
}

void _machine_restart(void)
{
/* rtc power alarm */
#if 0
	uint32_t time, reg, day, sec;

	*(volatile unsigned int *) 0xC0090050 |=0x2;
	while(1) {
		reg = *(volatile unsigned int *) 0xC0090058;
		if (reg&0x2)
			break;
	}

	*(volatile unsigned int *) 0xC0060018 = 0x8;
	while(1) {
		reg = *(volatile unsigned int *) 0xC0060018;
		if (!(reg&0x8))
			break;
	}

	while(1) {
		time = *(volatile unsigned int *) 0xC0060000;
		sec = time & 0x3F;
		if(sec < 0x3A) {
			time += 0x2;
			break;
		}
	}

	day = *(volatile unsigned int *) 0xC0060004;
	day &= 0x1F;

	*(volatile unsigned int *) 0xC0060010 = 0xC0;
	*(volatile unsigned int *) 0xC0060020 = time;
	*(volatile unsigned int *) 0xC0060020 |= day << 20;
	*(volatile unsigned int *) 0xC0060010 |= 0x4;

	while(1) {
		reg = *(volatile unsigned int *) 0xC0060010;
		if (!(reg&0x4))
			break;
	}

	*(volatile unsigned int *) 0xC0060018 = 0x2;
#else /* wdt reset*/
	uint32_t reg;
    
	puts("system reset");
	//set clk
	reg = nvt_readl(0xc0020084);
	reg |= 0x1 << 17;
	nvt_writel(reg, 0xc0020084);

	reg = nvt_readl(0xc0020074);
	reg |= 0x1 << 17;
	nvt_writel(reg, 0xc0020074);

	// set sw reset mode
	nvt_writel(0x5a960112, 0xc0050000);

	udelay(80);

	// set timeout now and do external reset
	nvt_writel(0x5a960113, 0xc0050000);
#endif

	while(1);
}
