/*
 * (C) Copyright 2002
 * Richard Jones, rjones@nexus-tech.net
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Boot support
 */
#include <common.h>
#include <command.h>
#include <s_record.h>
#include <net.h>
#include <ata.h>
#include <asm/io.h>
#include <part.h>
#include <exfat.h>
#include <fs.h>

int do_exfat_fsload (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	return do_load(cmdtp, flag, argc, argv, FS_TYPE_EXFAT);
}


U_BOOT_CMD(
	exfatload,	7,	0,	do_exfat_fsload,
	"load binary file from a dos filesystem",
	"<interface> [<dev[:part]> [<addr> [<filename> [bytes [pos]]]]]\n"
	"    - Load binary file 'filename' from 'dev' on 'interface'\n"
	"      to address 'addr' from dos filesystem.\n"
	"      'pos' gives the file position to start loading from.\n"
	"      If 'pos' is omitted, 0 is used. 'pos' requires 'bytes'.\n"
	"      'bytes' gives the size to load. If 'bytes' is 0 or omitted,\n"
	"      the load stops on end of file.\n"
	"      If either 'pos' or 'bytes' are not aligned to\n"
	"      ARCH_DMA_MINALIGN then a misaligned buffer warning will\n"
	"      be printed and performance will suffer for the load."
);

static int do_exfat_ls(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	return do_ls(cmdtp, flag, argc, argv, FS_TYPE_EXFAT);
}

U_BOOT_CMD(
	exfatls,	4,	1,	do_exfat_ls,
	"list files in a directory (default /)",
	"<interface> [<dev[:part]>] [directory]\n"
	"    - list files from 'dev' on 'interface' in a 'directory'"
);

static int do_exfat_fsinfo(cmd_tbl_t *cmdtp, int flag, int argc,
			 char * const argv[])
{
	int dev, part;
	block_dev_desc_t *dev_desc;
	disk_partition_t info;

	if (argc < 2) {
		printf("usage: exfatinfo <interface> [<dev[:part]>]\n");
		return 0;
	}

	part = get_device_and_partition(argv[1], argv[2], &dev_desc, &info, 1);
	if (part < 0)
		return 1;

	dev = dev_desc->dev;
	if (exfat_set_blk_dev(dev_desc, &info) != 0) {
		printf("\n** Unable to use %s %d:%d for fatinfo **\n",
			argv[1], dev, part);
		return 1;
	}
	return file_exfat_detectfs();
}

U_BOOT_CMD(
	exfatinfo,	3,	1,	do_exfat_fsinfo,
	"print information about filesystem",
	"<interface> [<dev[:part]>]\n"
	"    - print information about filesystem from 'dev' on 'interface'"
);

#ifdef CONFIG_EXFAT_WRITE
static int do_exfat_fswrite(cmd_tbl_t *cmdtp, int flag,
		int argc, char * const argv[])
{
	long size;
	unsigned long addr;
	unsigned long count;
	block_dev_desc_t *dev_desc = NULL;
	disk_partition_t info;
	int dev = 0;
	int part = 1;
	void *buf;

	if (argc < 5)
		return cmd_usage(cmdtp);

	part = get_device_and_partition(argv[1], argv[2], &dev_desc, &info, 1);
	if (part < 0)
		return 1;

	dev = dev_desc->dev;

	if (exfat_set_blk_dev(dev_desc, &info) != 0) {
		printf("\n** Unable to use %s %d:%d for exfatwrite **\n",
			argv[1], dev, part);
		return 1;
	}
	addr = simple_strtoul(argv[3], NULL, 16);
	count = simple_strtoul(argv[5], NULL, 16);

	buf = map_sysmem(addr, count);
	size = file_exfat_write(argv[4], buf, count);
	unmap_sysmem(buf);
	if (size == -1) {
		printf("\n** Unable to write \"%s\" from %s %d:%d **\n",
			argv[4], argv[1], dev, part);
		return 1;
	}

	printf("%ld bytes written\n", size);

	return 0;
}

U_BOOT_CMD(
	exfatwrite,	6,	0,	do_exfat_fswrite,
	"write file into a dos filesystem",
	"<interface> <dev[:part]> <addr> <filename> <bytes>\n"
	"    - write file 'filename' from the address 'addr' in RAM\n"
	"      to 'dev' on 'interface'"
);

static int do_exfat_fsrm(cmd_tbl_t *cmdtp, int flag,
		int argc, char * const argv[])
{

    block_dev_desc_t *dev_desc = NULL;
	disk_partition_t info;
	int dev = 0;
	int part = 1;

	if (argc != 4)
		return cmd_usage(cmdtp);

	part = get_device_and_partition(argv[1], argv[2], &dev_desc, &info, 1);
	if (part < 0)
		return 1;

	dev = dev_desc->dev;

	if (exfat_set_blk_dev(dev_desc, &info) != 0) {
		printf("\n** Unable to use %s %d:%d for exfatrm **\n",
			argv[1], dev, part);
		return 1;
	}

	if (-1 == file_exfat_rm(argv[3])) {
		printf("\n** Unable to remove \"%s\" from %s %d:%d **\n",
			argv[3], argv[1], dev, part);
		return 1;
	}

	printf("%s is removed\n", argv[3]);

	return 0;
}

U_BOOT_CMD(
	exfatrm,	4,	0,	do_exfat_fsrm,
	"remove file from a dos filesystem by exFAT",
	"<interface> <dev[:part]> <filename>\n"
	"    - remove file 'filename' from 'dev' on 'interface'\n"
);

static int do_exfat_fsrename(cmd_tbl_t *cmdtp, int flag,
		int argc, char * const argv[])
{
	block_dev_desc_t *dev_desc = NULL;
	disk_partition_t info;
	int dev = 0;
	int part = 1;

	if (argc != 5)
		return cmd_usage(cmdtp);

	part = get_device_and_partition(argv[1], argv[2], &dev_desc, &info, 1);
	if (part < 0)
		return 1;

	dev = dev_desc->dev;

	if (exfat_set_blk_dev(dev_desc, &info) != 0) {
		printf("\n** Unable to use %s %d:%d for exfatrename **\n",
			argv[1], dev, part);
		return 1;
	}

	if (-1 == file_exfat_rename(argv[3], argv[4])) {
		printf("\n** Unable to rename \"%s\" to \"%s\" from %s %d:%d **\n",
			argv[3], argv[4], argv[1], dev, part);
		return 1;
	}

    printf("%s is renamed to %s\n", argv[3], argv[4]);

	return 0;
}

U_BOOT_CMD(
	exfatrename,	5,	0,	do_exfat_fsrename,
	"write file into a dos filesystem by exFAT",
	"<interface> <dev[:part]> <oldname> <newname>\n"
	"    - rename file 'oldname' to 'newname'\n"
	"      to 'dev' on 'interface'"
);

static int do_write_wrapper(cmd_tbl_t *cmdtp, int flag, int argc,
				char * const argv[])
{
	return do_write(cmdtp, flag, argc, argv, FS_TYPE_ANY);
}

U_BOOT_CMD(
	write,	6,	0,	do_write_wrapper,
	"write file into a dos filesystem",
	"<interface> <dev[:part]> <addr> <filename> <bytes>\n"
	"    - write file 'filename' from the address 'addr' in RAM\n"
	"      to 'dev' on 'interface'"
);

static int do_rm_wrapper(cmd_tbl_t *cmdtp, int flag, int argc,
				char * const argv[])
{
	return do_rm(cmdtp, flag, argc, argv, FS_TYPE_ANY);
}

U_BOOT_CMD(
	rm,	4,	0,	do_rm_wrapper,
	"remove file from a dos filesystem by exFAT",
	"<interface> <dev[:part]> <filename>\n"
	"    - remove file 'filename' from 'dev' on 'interface'\n"
);

static int do_rename_wrapper(cmd_tbl_t *cmdtp, int flag, int argc,
				char * const argv[])
{
	return do_rename(cmdtp, flag, argc, argv, FS_TYPE_ANY);
}

U_BOOT_CMD(
	rename,	5,	0,	do_rename_wrapper,
	"write file into a dos filesystem by exFAT",
	"<interface> <dev[:part]> <oldname> <newname>\n"
	"    - rename file 'oldname' to 'newname'\n"
	"      to 'dev' on 'interface'"
);
#endif
