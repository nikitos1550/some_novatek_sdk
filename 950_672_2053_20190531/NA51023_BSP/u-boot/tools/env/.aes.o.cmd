cmd_tools/env/aes.o := /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/bin/mipsel-24kec-linux-gnu-gcc -Wp,-MD,tools/env/.aes.o.d -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer   -idirafterinclude -idirafter/home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include -idirafter /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/tools/env -DUSE_HOSTCC -DTEXT_BASE=  -c -o tools/env/aes.o tools/env/aes.c

source_tools/env/aes.o := tools/env/aes.c

deps_tools/env/aes.o := \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/stdc-predef.h \
  tools/env/../../lib/aes.c \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/string.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/features.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/sys/cdefs.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/wordsize.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/sgidefs.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/gnu/stubs.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/gnu/stubs-o32_soft.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/lib/gcc/mipsel-24kec-linux-gnu/4.9.3/include/stddef.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/xlocale.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/string.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/bits/string2.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/stdlib.h \
  include/aes.h \

tools/env/aes.o: $(deps_tools/env/aes.o)

$(deps_tools/env/aes.o):
