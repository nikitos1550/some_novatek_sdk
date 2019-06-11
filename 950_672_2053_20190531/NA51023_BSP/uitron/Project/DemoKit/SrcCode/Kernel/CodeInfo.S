# 1 "/home/nvt03650/00_NA51023_BSP/uitron/Project/DemoKit/SrcCode/Kernel/CodeInfo.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/home/nvt03650/00_NA51023_BSP/uitron/Project/DemoKit/SrcCode/Kernel/CodeInfo.S"
# 32 "/home/nvt03650/00_NA51023_BSP/uitron/Project/DemoKit/SrcCode/Kernel/CodeInfo.S"
 .global _image_general_zi_zi_base
 .global _image_general_zi_zi_limit
 .global _section_01_addr
 .global _section_01_size
 .global _section_02_addr
 .global _section_02_size
 .global _section_03_addr
 .global _section_03_size
 .global _section_04_addr
 .global _section_04_size
 .global _section_05_addr
 .global _section_05_size
 .global _section_06_addr
 .global _section_06_size
 .global _section_07_addr
 .global _section_07_size
 .global _section_08_addr
 .global _section_08_size
 .global _section_09_addr
 .global _section_09_size
 .global _section_10_addr
 .global _section_10_size

 .data
 .org 0x00
 .ascii "CODEINFO        "
 .org 0x10
 .word _image_general_zi_zi_base
 .word _image_general_zi_zi_limit
 .org 0x18
 .word _section_01_addr
 .word _section_01_size
 .word _section_02_addr
 .word _section_02_size
 .word _section_03_addr
 .word _section_03_size
 .word _section_04_addr
 .word _section_04_size
 .word _section_05_addr
 .word _section_05_size
 .word _section_06_addr
 .word _section_06_size
 .word _section_07_addr
 .word _section_07_size
 .word _section_08_addr
 .word _section_08_size
 .word _section_09_addr
 .word _section_09_size
 .word _section_10_addr
 .word _section_10_size
 .word 0xffffffff
