OUTPUT_FORMAT("elf32-tradlittlemips", "elf32-tradbigmips",
              "elf32-tradlittlemips")
OUTPUT_ARCH(mips:isa32)
ENTRY(_start_nvt)
SECTIONS
{
 . = 0x00000000;
 . = ALIGN(4);
        .text : {
                KEEP(arch/mips/cpu/mips32/start.o (.text.startup*))
                . = 0x300;
                KEEP(board/novatek/nvt-na51023/built-in.o(.data.bininfo*))
                *(.text*)
        }
 . = ALIGN(4);
 .rodata : {
  *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
 }
 . = ALIGN(4);
 .data : {
  *(.data*)
 }
 . = .;
 _gp = ALIGN(16) + 0x7ff0;
 .got : {
  *(.got)
 }
 num_got_entries = SIZEOF(.got) >> 2;
 . = ALIGN(4);
 .sdata : {
  *(.sdata*)
 }
 . = ALIGN(4);
 .u_boot_list : {
  KEEP(*(SORT(.u_boot_list*)));
 }
 . = ALIGN(4);
 __image_copy_end = .;
 __init_end = .;
 .rel.dyn : {
  __rel_dyn_start = .;
  *(.rel.dyn)
  __rel_dyn_end = .;
 }
 . = ALIGN(4);
 .BIN_END :
 {
   KEEP(board/novatek/nvt-na51023/built-in.o(.BinEnd));
 }
 _end = .;
 uboot_bin_size = _end - 0x03900000;
 .bss __rel_dyn_start (OVERLAY) : {
  __bss_start = .;
  *(.sbss.*)
  *(.bss.*)
  *(COMMON)
  . = ALIGN(4);
  __bss_end = .;
 }
 .dynsym _end : {
  *(.dynsym)
 }
 .dynbss : {
  *(.dynbss)
 }
 .dynstr : {
  *(.dynstr)
 }
 .dynamic : {
  *(.dynamic)
 }
 .plt : {
  *(.plt)
 }
 .interp : {
  *(.interp)
 }
 .gnu : {
  *(.gnu*)
 }
 .MIPS.stubs : {
  *(.MIPS.stubs)
 }
 .hash : {
  *(.hash)
 }
}
