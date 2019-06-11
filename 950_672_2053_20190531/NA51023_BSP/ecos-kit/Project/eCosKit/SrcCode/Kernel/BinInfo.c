/*;   Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
 *;
 *;   @file       BinInfo.c
 *;   @ingroup
 *;
 *;   @brief      Binary file information
 *;               Binary file information
 *;
 *;   @version    V1.20.000
 *;   @author     Novatek
 *;   @date       2012/07/19
 */

#include "Type.h"
#include "PrjCfg.h"
#include "BinInfo.h"

extern char _section_01_addr[];
extern char ecos_bin_size[];

HEADINFO gBinInfo = {
	(UINT32)_section_01_addr, //<- fw CODE entry (4)
	{0}, ///<- reserved (4*19)
	"ecos-kit", //<- CHIP-NAME (8)
	"10000000", //<- version (8)
	"20100701", //<- releasedate (8)
	(UINT32)ecos_bin_size, //<- Bin File Length (4)
	0xffffffff, //<- Check Sum or CRC (4)
	0,///<- Length check for CRC (4)
	0,///<- reserved (4)
	0,///<- Bin flag (4)
	0,///<- Binary Tag for CRC (4)
};


