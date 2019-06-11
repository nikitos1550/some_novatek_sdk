#ifndef _NVT_IVOT_PTYD_H
#define _NVT_IVOT_PTYD_H
#include "Platform.h"

extern int nvt_ivot_ptyd_open(void);
extern int nvt_ivot_ptyd_close(void);
extern void nvt_ivot_ptyd_install_id(void) _SECTION(".kercfg_text");

#endif