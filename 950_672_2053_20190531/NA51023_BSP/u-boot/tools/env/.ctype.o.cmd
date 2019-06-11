cmd_tools/env/ctype.o := /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/bin/mipsel-24kec-linux-gnu-gcc -Wp,-MD,tools/env/.ctype.o.d -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer   -idirafterinclude -idirafter/home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include -idirafter /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/tools/env -DUSE_HOSTCC -DTEXT_BASE=  -c -o tools/env/ctype.o tools/env/ctype.c

source_tools/env/ctype.o := tools/env/ctype.c

deps_tools/env/ctype.o := \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/mipsel-24kec-linux-gnu/sysroot/usr/include/stdc-predef.h \
  tools/env/../../lib/ctype.c \
  include/linux/ctype.h \

tools/env/ctype.o: $(deps_tools/env/ctype.o)

$(deps_tools/env/ctype.o):
