// eCos memory layout - Mon Apr 09 14:17:14 2001

// This is a generated file - do not edit

#ifndef __ASSEMBLER__
#include <cyg/infra/cyg_type.h>
#include <pkgconf/hal_mips_nvtdsc_nt96660.h>
#include <stddef.h>
#endif

extern char CYG_LABEL_NAME (__ebase);
extern char CYG_LABEL_NAME (_section_01_addr) [];
extern char CYG_LABEL_NAME (_end) [];
extern char CYG_LABEL_NAME (__heap1) [];
extern char CYG_LABEL_NAME (__heap1_end) [];
#define CYGMEM_REGION_ram ((size_t) CYG_LABEL_NAME (_section_01_addr))
#define CYGMEM_REGION_ram_SIZE ((size_t) CYG_LABEL_NAME (_end) - CYGMEM_REGION_ram)
#define CYGMEM_REGION_ram_ATTR (CYGMEM_REGION_ATTR_R | CYGMEM_REGION_ATTR_W)
#define CYGMEM_SECTION_heap1 (CYG_LABEL_NAME (__heap1))
#define CYGMEM_SECTION_heap1_SIZE ((size_t) CYG_LABEL_NAME (__heap1_end) - (size_t) CYG_LABEL_NAME (__heap1))
