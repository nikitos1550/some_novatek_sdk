ECOS_GLOBAL_CFLAGS = -pipe -march=24kec -mtune=24kec -mdsp -EL -msoft-float -G0 -Wall -Winline -Wno-missing-braces -Woverloaded-virtual -Wpointer-arith -Wsign-compare -Wstrict-prototypes -Wundef -fno-exceptions -fno-common -fno-optimize-sibling-calls -fno-rtti -fno-strict-aliasing -fshort-wchar -O2 -ffunction-sections -fdata-sections
ECOS_GLOBAL_LDFLAGS = -EL -msoft-float -nostdlib -Wl,--gc-sections -Wl,-static
ECOS_COMMAND_PREFIX = mips-sde-elf-
