/**
    DSP Utility

    For start and close Core - DSP

    @file       GxDSP.h
    @ingroup    mIGXDSP

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/

#ifndef _GXDSP_H
#define _GXDSP_H

/**
    @addtogroup mIGXDSP
*/
//@{

/**
    DSP core ID.
*/
typedef enum _DSP_CORE_ID {
	DSP_CORE_ID_1 = 0,                       ///< DSP core 1
	DSP_CORE_ID_2,                           ///< DSP core 2
	DSP_CORE_ID_MAX_NUM,
	ENUM_DUMMY4WORD(DSP_CORE_ID)
} DSP_CORE_ID;

/**
    DSP memory pool.
*/
typedef struct _GXDSP_OPEN {
	UINT32 uiAddr; ///< DSP start of memory region
	UINT32 uiSize; ///< DSP size of memory region
} GXDSP_OPEN, *PGXDSP_OPEN;

/**
    DSP bin information.
*/
typedef struct _DSP_BIN_INFO {
	UINT32  Offset;     ///< original bin position in file
	UINT32  Size;       ///< size of original bin
	UINT32  TargetAddr; ///< target address in DRAM( physical address)
	UINT32  CheckSum;   ///< checksum of original
} DSP_BIN_INFO;

/**
    DSP header information.
*/
typedef struct _DSP_FW_HEADER {
	UINT32        uiFourCC; ///< 'D','S','P','1' for DSP1 and 'D','S','P','2' for DSP2
	UINT32        uiSize;   ///< sizeof(DSP_FW_HEADER)
	DSP_BIN_INFO  BLoader;  ///< boot loader bin info
	DSP_BIN_INFO  ProgInt;  ///< ProgInt bin info
	DSP_BIN_INFO  ISDM;     ///< ISDM bin info
	DSP_BIN_INFO  IVDM;     ///< IVDM bin info
	DSP_BIN_INFO  ProgExt;  ///< ProgExt bin info
	DSP_BIN_INFO  DataExt;  ///< DataExt bin info
	UINT32  TotalSize;      ///< total all in one bin size including header size
	UINT32  EmbMemBlkSize;  ///< EMBMEM_BLK_SIZE
	CHAR    BootVer[20];    ///< DSP boot code version
	CHAR    MainVer[20];    ///< DSP main code version
} DSP_FW_HEADER;

/**
    Initialize GxDSP lib.

    @return
        - @b TRUE:  Succeed.
        - @b FALSE:  Failed.
*/
extern BOOL GxDSP_Init(void);

/**
    Start DSP firmware.

    @param[in] pOpen DSP memory pool: #_GXDSP_OPEN.
    @return
        - @b TRUE:  Succeed.
        - @b FALSE:  Failed.
*/
extern BOOL GxDSP_Open(GXDSP_OPEN *pOpen);

/**
    Close DSP module.

    @return
        - @b TRUE:  Succeed.
        - @b FALSE:  Failed.
*/
extern BOOL GxDSP_Close(void);

/**
    Check DSP firmware.

    @param[in] uiAddr Firmware address.
    @return
        - @b TRUE:  Succeed.
        - @b FALSE:  Failed.
*/
extern BOOL GxDSP_ChkValid(UINT32 uiAddr);

/**
    Check DSP Support.

    @return
        - @b TRUE:  IC support DSP.
        - @b FALSE:  IC doesn't support DSP.
*/
extern BOOL GxDSP_IsSupport(void);
//@}
#endif //_GXDSP_H
