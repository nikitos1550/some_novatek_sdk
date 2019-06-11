cmd_tools/env/fw_env.o := /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/bin/mipsel-24kec-linux-gnu-gcc -Wp,-MD,tools/env/.fw_env.o.d -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer   -idirafterinclude -idirafter/home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include -idirafter /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/tools/env -DUSE_HOSTCC -DTEXT_BASE=  -c -o tools/env/fw_env.o tools/env/fw_env.c

source_tools/env/fw_env.o := tools/env/fw_env.c

deps_tools/env/fw_env.o := \
    $(wildcard include/config/file.h) \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/stdc-predef.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/errno.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/features.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/sys/cdefs.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/wordsize.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/sgidefs.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/gnu/stubs.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/gnu/stubs-o32_soft.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/errno.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/linux/errno.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/asm/errno.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/asm-generic/errno-base.h \
  include/env_flags.h \
    $(wildcard include/config/cmd/net.h) \
    $(wildcard include/config/env/flags/list/static.h) \
    $(wildcard include/config/env/overwrite.h) \
    $(wildcard include/config/overwrite/ethaddr/once.h) \
    $(wildcard include/config/cmd/env/flags.h) \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/fcntl.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/types.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/typesizes.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/fcntl.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/fcntl-linux.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/time.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/stat.h \
  include/linux/stringify.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/stdio.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/lib/gcc/mipsel-24kec-linux-gnu/4.9.3/include/stddef.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/libio.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/_G_config.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/wchar.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/lib/gcc/mipsel-24kec-linux-gnu/4.9.3/include/stdarg.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/stdio_lim.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/sys_errlist.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/stdio.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/stdlib.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/waitflags.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/waitstatus.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/endian.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/endian.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/byteswap.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/byteswap-16.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/sys/types.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/sys/select.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/select.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/sigset.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/time.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/sys/sysmacros.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/pthreadtypes.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/alloca.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/stdlib-bsearch.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/stdlib-float.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/string.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/xlocale.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/string.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/string2.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/sys/ioctl.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/ioctls.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/asm/ioctls.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/asm/ioctl.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/asm-generic/ioctl.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/ioctl-types.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/sys/ttydefaults.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/sys/stat.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/unistd.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/posix_opt.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/environments.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/confname.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/getopt.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/mtd/mtd-user.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/lib/gcc/mipsel-24kec-linux-gnu/4.9.3/include/stdint.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/stdint.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/wchar.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/mtd/mtd-abi.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/linux/types.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/asm/types.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/asm-generic/int-ll64.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/asm/bitsperlong.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/asm-generic/bitsperlong.h \
    $(wildcard include/config/64bit.h) \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/linux/posix_types.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/linux/stddef.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/asm/posix_types.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/asm/sgidefs.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/asm-generic/posix_types.h \
  tools/env/fw_env.h \
    $(wildcard include/config/baudrate.h) \
    $(wildcard include/config/bootdelay.h) \
    $(wildcard include/config/bootcommand.h) \
  include/config.h \
    $(wildcard include/config/sys/arch.h) \
    $(wildcard include/config/sys/cpu.h) \
    $(wildcard include/config/sys/board.h) \
    $(wildcard include/config/sys/vendor.h) \
    $(wildcard include/config/sys/soc.h) \
    $(wildcard include/config/boarddir.h) \
  include/config_cmd_defaults.h \
    $(wildcard include/config/cmd/defaults/h/.h) \
    $(wildcard include/config/cmd/bootm.h) \
    $(wildcard include/config/cmd/crc32.h) \
    $(wildcard include/config/cmd/exportenv.h) \
    $(wildcard include/config/cmd/go.h) \
    $(wildcard include/config/cmd/importenv.h) \
  include/config_defaults.h \
    $(wildcard include/config/defaults/h/.h) \
    $(wildcard include/config/bootm/linux.h) \
    $(wildcard include/config/bootm/netbsd.h) \
    $(wildcard include/config/bootm/plan9.h) \
    $(wildcard include/config/bootm/rtems.h) \
    $(wildcard include/config/bootm/vxworks.h) \
    $(wildcard include/config/gzip.h) \
    $(wildcard include/config/zlib.h) \
    $(wildcard include/config/partitions.h) \
  include/configs/nvt-na51023.h \
    $(wildcard include/config/na51023/h.h) \
    $(wildcard include/config/nvt/fw/update/led.h) \
    $(wildcard include/config/nvt/pwm.h) \
    $(wildcard include/config/fpga/emulation.h) \
    $(wildcard include/config/mips32.h) \
    $(wildcard include/config/na51023.h) \
    $(wildcard include/config/nvt/board.h) \
    $(wildcard include/config/skip/lowlevel/init.h) \
    $(wildcard include/config/nand/type/onfi.h) \
    $(wildcard include/config/nand/type/spinand.h) \
    $(wildcard include/config/nand/type/spinor.h) \
    $(wildcard include/config/flash/only/dual.h) \
    $(wildcard include/config/nvt/drtc.h) \
    $(wildcard include/config/sys/clkin.h) \
    $(wildcard include/config/cpu/mhz.h) \
    $(wildcard include/config/sys/mips/timer/freq.h) \
    $(wildcard include/config/sys/hz.h) \
    $(wildcard include/config/pwm.h) \
    $(wildcard include/config/pwm/period00.h) \
    $(wildcard include/config/pwm/period01.h) \
    $(wildcard include/config/pwm/period02.h) \
    $(wildcard include/config/pwm/period03.h) \
    $(wildcard include/config/pwm/period04.h) \
    $(wildcard include/config/pwm/period05.h) \
    $(wildcard include/config/pwm/period06.h) \
    $(wildcard include/config/pwm/period07.h) \
    $(wildcard include/config/pwm/period08.h) \
    $(wildcard include/config/pwm/period09.h) \
    $(wildcard include/config/pwm/period10.h) \
    $(wildcard include/config/pwm/period11.h) \
    $(wildcard include/config/pwm/period12.h) \
    $(wildcard include/config/pwm/period13.h) \
    $(wildcard include/config/pwm/period14.h) \
    $(wildcard include/config/pwm/period15.h) \
    $(wildcard include/config/pwm/duty00.h) \
    $(wildcard include/config/pwm/duty01.h) \
    $(wildcard include/config/pwm/duty02.h) \
    $(wildcard include/config/pwm/duty03.h) \
    $(wildcard include/config/pwm/duty04.h) \
    $(wildcard include/config/pwm/duty05.h) \
    $(wildcard include/config/pwm/duty06.h) \
    $(wildcard include/config/pwm/duty07.h) \
    $(wildcard include/config/pwm/duty08.h) \
    $(wildcard include/config/pwm/duty09.h) \
    $(wildcard include/config/pwm/duty10.h) \
    $(wildcard include/config/pwm/duty11.h) \
    $(wildcard include/config/pwm/duty12.h) \
    $(wildcard include/config/pwm/duty13.h) \
    $(wildcard include/config/pwm/duty14.h) \
    $(wildcard include/config/pwm/duty15.h) \
    $(wildcard include/config/sys/dcache/size.h) \
    $(wildcard include/config/sys/icache/size.h) \
    $(wildcard include/config/sys/cacheline/size.h) \
    $(wildcard include/config/sys/uart.h) \
    $(wildcard include/config/sys/baudrate/table.h) \
    $(wildcard include/config/na51023/gpio.h) \
    $(wildcard include/config/sys/sdram/base.h) \
    $(wildcard include/config/uboot/sdram/base.h) \
    $(wildcard include/config/sys/sdram/size.h) \
    $(wildcard include/config/uboot/sdram/size.h) \
    $(wildcard include/config/linux/sdram/base.h) \
    $(wildcard include/config/linux/sdram/size.h) \
    $(wildcard include/config/sys/init/sp/addr.h) \
    $(wildcard include/config/sys/malloc/len.h) \
    $(wildcard include/config/sys/monitor/base.h) \
    $(wildcard include/config/sys/text/base.h) \
    $(wildcard include/config/sys/monitor/len.h) \
    $(wildcard include/config/fixed/eth/parameter.h) \
    $(wildcard include/config/ethnet.h) \
    $(wildcard include/config/ethaddr.h) \
    $(wildcard include/config/ipaddr.h) \
    $(wildcard include/config/netmask.h) \
    $(wildcard include/config/serverip.h) \
    $(wildcard include/config/gatewayip.h) \
    $(wildcard include/config/hostname.h) \
    $(wildcard include/config/bootp/subnetmask.h) \
    $(wildcard include/config/bootp/gateway.h) \
    $(wildcard include/config/bootp/hostname.h) \
    $(wildcard include/config/bootp/bootpath.h) \
    $(wildcard include/config/bootp/bootfilesize.h) \
    $(wildcard include/config/phylib.h) \
    $(wildcard include/config/cmd/ping.h) \
    $(wildcard include/config/cmd/dhcp.h) \
    $(wildcard include/config/net/multi.h) \
    $(wildcard include/config/netconsole.h) \
    $(wildcard include/config/sys/longhelp.h) \
    $(wildcard include/config/auto/complete.h) \
    $(wildcard include/config/cmdline/editing.h) \
    $(wildcard include/config/sys/hush/parser.h) \
    $(wildcard include/config/sys/prompt/hush/ps2.h) \
    $(wildcard include/config/lzo.h) \
    $(wildcard include/config/bzip2.h) \
    $(wildcard include/config/lzma.h) \
    $(wildcard include/config/cmd/memory.h) \
    $(wildcard include/config/sys/prompt.h) \
    $(wildcard include/config/sys/cbsize.h) \
    $(wildcard include/config/sys/pbsize.h) \
    $(wildcard include/config/sys/maxargs.h) \
    $(wildcard include/config/sys/bootparams/len.h) \
    $(wildcard include/config/sys/load/addr.h) \
    $(wildcard include/config/sys/memtest/start.h) \
    $(wildcard include/config/sys/memtest/end.h) \
    $(wildcard include/config/memsize/in/bytes.h) \
    $(wildcard include/config/misc/init/r.h) \
    $(wildcard include/config/bootargs/common.h) \
    $(wildcard include/config/nvt/linux/autoload.h) \
    $(wildcard include/config/nvt/linux/autodetect.h) \
    $(wildcard include/config/nvt/bin/chksum/support.h) \
    $(wildcard include/config/nvt/linux/ramdisk/boot.h) \
    $(wildcard include/config/bootargs.h) \
    $(wildcard include/config/nvt/linux/spinand/boot.h) \
    $(wildcard include/config/cmd/ubi.h) \
    $(wildcard include/config/cmd/ubifs.h) \
    $(wildcard include/config/nvt/linux/spinor/boot.h) \
    $(wildcard include/config/cmd/nand/trimffs.h) \
    $(wildcard include/config/nvt/linux/sd/boot.h) \
    $(wildcard include/config/dsp1/freertos.h) \
    $(wildcard include/config/nvt/mmc.h) \
    $(wildcard include/config/nvt/load/linux/image.h) \
    $(wildcard include/config/sys/flash/base.h) \
    $(wildcard include/config/ramoops/log.h) \
    $(wildcard include/config/stacksize.h) \
    $(wildcard include/config/sys/init/sp/offset.h) \
    $(wildcard include/config/autoboot/keyed.h) \
    $(wildcard include/config/autoboot/stop/str.h) \
    $(wildcard include/config/autoboot/prompt.h) \
    $(wildcard include/config/zero/bootdelay/check.h) \
    $(wildcard include/config/nvt/wdt.h) \
    $(wildcard include/config/watchdog.h) \
    $(wildcard include/config/md5.h) \
    $(wildcard include/config/mmc.h) \
    $(wildcard include/config/cmd/mmc.h) \
    $(wildcard include/config/cmd/fat.h) \
    $(wildcard include/config/fs/fat.h) \
    $(wildcard include/config/cmd/exfat.h) \
    $(wildcard include/config/fs/exfat.h) \
    $(wildcard include/config/fs/ext4.h) \
    $(wildcard include/config/dos/partition.h) \
    $(wildcard include/config/generic/mmc.h) \
    $(wildcard include/config/sdhci.h) \
    $(wildcard include/config/sd/card1/power/pin.h) \
    $(wildcard include/config/sd/card1/on/state.h) \
    $(wildcard include/config/sd/card2/power/pin.h) \
    $(wildcard include/config/sd/card2/on/state.h) \
    $(wildcard include/config/support/emmc/boot.h) \
    $(wildcard include/config/nvt/mmc/channel.h) \
    $(wildcard include/config/nvt/mmc/max/num.h) \
    $(wildcard include/config/fat/write.h) \
    $(wildcard include/config/exfat/write.h) \
    $(wildcard include/config/nvt/nand/ecc.h) \
    $(wildcard include/config/nvt/spi/nand.h) \
    $(wildcard include/config/nvt/nand/type.h) \
    $(wildcard include/config/cmd/nand.h) \
    $(wildcard include/config/sys/max/nand/device.h) \
    $(wildcard include/config/nvt/spi/nor.h) \
    $(wildcard include/config/cmd/sf.h) \
    $(wildcard include/config/spi/flash.h) \
    $(wildcard include/config/mtd/device.h) \
    $(wildcard include/config/spi/flash/macronix.h) \
    $(wildcard include/config/spi/flash/winbond.h) \
    $(wildcard include/config/spi/flash/gigadevice.h) \
    $(wildcard include/config/spi/flash/stmicro.h) \
    $(wildcard include/config/spi/flash/eon.h) \
    $(wildcard include/config/sys/max/sf/device.h) \
    $(wildcard include/config/spi/flash/mtd.h) \
    $(wildcard include/config/sf/default/bus.h) \
    $(wildcard include/config/sf/default/cs.h) \
    $(wildcard include/config/sf/default/mode.h) \
    $(wildcard include/config/sf/default/speed.h) \
    $(wildcard include/config/env/sect/size.h) \
    $(wildcard include/config/rbtree.h) \
    $(wildcard include/config/mtd/partitions.h) \
    $(wildcard include/config/cmd/mtdparts.h) \
    $(wildcard include/config/timestamp.h) \
    $(wildcard include/config/sys/stbc/len.h) \
    $(wildcard include/config/sys/rom/len.h) \
    $(wildcard include/config/sys/no/flash.h) \
    $(wildcard include/config/cmd/imls.h) \
    $(wildcard include/config/modem/support.h) \
    $(wildcard include/config/cmd/saveenv.h) \
    $(wildcard include/config/env/is/in/nand.h) \
    $(wildcard include/config/env/size.h) \
    $(wildcard include/config/env/offset.h) \
    $(wildcard include/config/env/range.h) \
    $(wildcard include/config/env/is/in/spi/flash.h) \
    $(wildcard include/config/env/is/nowhere.h) \
    $(wildcard include/config/nvt/mmc/boot.h) \
    $(wildcard include/config/ramdisk/size.h) \
    $(wildcard include/config/image/format/legacy.h) \
  include/linux/sizes.h \
  include/configs/novatek/na51023_mips24k_fpga.h \
    $(wildcard include/config/model/nvt/fpga.h) \
    $(wildcard include/config/mem/base.h) \
    $(wildcard include/config/mem/size.h) \
    $(wildcard include/config/board/rev/addr.h) \
    $(wildcard include/config/board/rev/size.h) \
    $(wildcard include/config/nvt/uimage/size.h) \
    $(wildcard include/config/nvt/uimage/unzip/size.h) \
    $(wildcard include/config/nvt/itron/unzip/size.h) \
    $(wildcard include/config/dsp/sdram/base.h) \
    $(wildcard include/config/dsp/sdram/size.h) \
    $(wildcard include/config/dsp/boot/tmp/buf/addr.h) \
    $(wildcard include/config/dsp/boot/tmp/buf/size.h) \
    $(wildcard include/config/nvt/runfw/sdram/base.h) \
    $(wildcard include/config/ramdisk/sdram/base.h) \
    $(wildcard include/config/ramdisk/sdram/size.h) \
    $(wildcard include/config/itron/sdram/base.h) \
    $(wildcard include/config/itron/sdram/size.h) \
    $(wildcard include/config/nvt/runfw/sdram/size.h) \
    $(wildcard include/config/linux/sdram/start.h) \
    $(wildcard include/config/linux/unzip/sdram/end.h) \
    $(wildcard include/config/linux/sdram/memreserved/base.h) \
    $(wildcard include/config/linux/sdram/memreserved/size.h) \
    $(wildcard include/config/smem/sdram/base.h) \
    $(wildcard include/config/modelext/sdram/base.h) \
    $(wildcard include/config/modelext/sdram/size.h) \
    $(wildcard include/config/nvtipc/shared/mem/addr.h) \
    $(wildcard include/config/nvtipc/shared/mem/size.h) \
    $(wildcard include/config/smem/sdram/size.h) \
    $(wildcard include/config/modelext/spinand/base.h) \
    $(wildcard include/config/modelext/spinor/base.h) \
    $(wildcard include/config/sys/nand/base.h) \
    $(wildcard include/config/sys/hz/clock.h) \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include/asm/config.h \
    $(wildcard include/config/h/.h) \
    $(wildcard include/config/sys/generic/global/data.h) \
    $(wildcard include/config/lmb.h) \
    $(wildcard include/config/sys/boot/ramdisk/high.h) \
  include/config_fallbacks.h \
    $(wildcard include/config/fallbacks/h.h) \
    $(wildcard include/config/spl.h) \
    $(wildcard include/config/spl/pad/to.h) \
    $(wildcard include/config/spl/max/size.h) \
    $(wildcard include/config/cmd/ext4.h) \
    $(wildcard include/config/cmd/ext2.h) \
    $(wildcard include/config/cmd/ext4/write.h) \
    $(wildcard include/config/ext4/write.h) \
    $(wildcard include/config/cmd/ide.h) \
    $(wildcard include/config/cmd/sata.h) \
    $(wildcard include/config/cmd/scsi.h) \
    $(wildcard include/config/cmd/usb.h) \
    $(wildcard include/config/cmd/part.h) \
    $(wildcard include/config/cmd/gpt.h) \
    $(wildcard include/config/systemace.h) \
    $(wildcard include/config/sandbox.h) \
    $(wildcard include/config/partition/uuids.h) \
    $(wildcard include/config/efi/partition.h) \
    $(wildcard include/config/random/uuid.h) \
    $(wildcard include/config/cmd/uuid.h) \
    $(wildcard include/config/bootp/pxe.h) \
    $(wildcard include/config/lib/uuid.h) \
    $(wildcard include/config/lib/rand.h) \
    $(wildcard include/config/lib/hw/rand.h) \
    $(wildcard include/config/api.h) \
    $(wildcard include/config/lcd.h) \
    $(wildcard include/config/cmd/bmp.h) \
    $(wildcard include/config/fit/signature.h) \
    $(wildcard include/config/disable/image/legacy.h) \
  include/config_uncmd_spl.h \
    $(wildcard include/config/uncmd/spl/h//.h) \
    $(wildcard include/config/spl/build.h) \
    $(wildcard include/config/cmd/cdp.h) \
    $(wildcard include/config/cmd/dns.h) \
    $(wildcard include/config/cmd/link/local.h) \
    $(wildcard include/config/cmd/nfs.h) \
    $(wildcard include/config/cmd/rarp.h) \
    $(wildcard include/config/cmd/sntp.h) \
    $(wildcard include/config/cmd/tftpput.h) \
    $(wildcard include/config/cmd/tftpsrv.h) \
  include/aes.h \
  include/env_default.h \
    $(wildcard include/config/sys/redundand/environment.h) \
    $(wildcard include/config/env/callback/list/default.h) \
    $(wildcard include/config/env/flags/list/default.h) \
    $(wildcard include/config/rambootcommand.h) \
    $(wildcard include/config/nfsbootcommand.h) \
    $(wildcard include/config/loads/echo.h) \
    $(wildcard include/config/eth1addr.h) \
    $(wildcard include/config/eth2addr.h) \
    $(wildcard include/config/eth3addr.h) \
    $(wildcard include/config/eth4addr.h) \
    $(wildcard include/config/eth5addr.h) \
    $(wildcard include/config/ethprime.h) \
    $(wildcard include/config/sys/autoload.h) \
    $(wildcard include/config/preboot.h) \
    $(wildcard include/config/rootpath.h) \
    $(wildcard include/config/bootfile.h) \
    $(wildcard include/config/loadaddr.h) \
    $(wildcard include/config/clocks/in/mhz.h) \
    $(wildcard include/config/pci/bootdelay.h) \
    $(wildcard include/config/env/vars/uboot/config.h) \
    $(wildcard include/config/extra/env/settings.h) \
  include/env_callback.h \
    $(wildcard include/config/env/callback/list/static.h) \
    $(wildcard include/config/silent/console.h) \
    $(wildcard include/config/splashimage/guard.h) \
  include/linker_lists.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/search.h \

tools/env/fw_env.o: $(deps_tools/env/fw_env.o)

$(deps_tools/env/fw_env.o):
