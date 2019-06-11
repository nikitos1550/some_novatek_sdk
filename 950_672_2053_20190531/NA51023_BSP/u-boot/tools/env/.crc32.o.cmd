cmd_tools/env/crc32.o := /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/bin/mipsel-24kec-linux-gnu-gcc -Wp,-MD,tools/env/.crc32.o.d -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer   -idirafterinclude -idirafter/home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include -idirafter /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/tools/env -DUSE_HOSTCC -DTEXT_BASE=  -c -o tools/env/crc32.o tools/env/crc32.c

source_tools/env/crc32.o := tools/env/crc32.c

deps_tools/env/crc32.o := \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/stdc-predef.h \
  tools/env/../../lib/crc32.c \
    $(wildcard include/config/hw/watchdog.h) \
    $(wildcard include/config/watchdog.h) \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/arpa/inet.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/features.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/sys/cdefs.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/wordsize.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/sgidefs.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/gnu/stubs.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/gnu/stubs-o32_soft.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/netinet/in.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/lib/gcc/mipsel-24kec-linux-gnu/4.9.3/include/stdint.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/stdint.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/wchar.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/sys/socket.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/sys/uio.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/sys/types.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/types.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/typesizes.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/time.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/lib/gcc/mipsel-24kec-linux-gnu/4.9.3/include/stddef.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/endian.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/endian.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/byteswap.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/byteswap-16.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/sys/select.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/select.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/sigset.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/time.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/sys/sysmacros.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/pthreadtypes.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/uio.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/socket.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/socket_type.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/sockaddr.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/asm/socket.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/asm/sockios.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/asm/ioctl.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/asm-generic/ioctl.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/in.h \
  include/compiler.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/errno.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/errno.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/linux/errno.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/asm/errno.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/asm-generic/errno-base.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/stdlib.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/waitflags.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/waitstatus.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/alloca.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/stdlib-bsearch.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/stdlib-float.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/stdio.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/libio.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/_G_config.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/wchar.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/lib/gcc/mipsel-24kec-linux-gnu/4.9.3/include/stdarg.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/stdio_lim.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/sys_errlist.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/stdio.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/string.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/xlocale.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/string.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/string2.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/sys/mman.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/mman.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/mman-linux.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/fcntl.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/fcntl.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/fcntl-linux.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/stat.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/byteswap.h \
  include/u-boot/crc.h \
  include/u-boot/zlib.h \

tools/env/crc32.o: $(deps_tools/env/crc32.o)

$(deps_tools/env/crc32.o):
