/**
    Header file for TS MUXER/TS DEMUXER/HWCOPY Engine Integration module

    This file is the header file for TS MUXER/TS DEMUXER/HWCOPY Engine Integration module driver
    The user can reference this section for the detail description of the each driver API usage
    and also the parameter descriptions and its limitations.
    The overall combinational usage flow is introduced in the application note document,
    and the user must reference to the application note for the driver usage flow.

    @file       tse.h
    @ingroup    mIDrvMisc_TSE
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/

/**
    @addtogroup mIDrvMisc_TSE
*/
//@{

#ifndef _TSE_H
#define _TSE_H

#include "Type.h"
#include "Driver.h"




/**
    TSE module general configuration Selection

    This is used in tse_setConfig()/tse_getConfig() to specify which of the function is selected to assign new configuration.
*/
typedef enum {
	TSE_CFG_ID_FREQ,                ///< TSE operating frequency (unit: MHz). Context can be:
	///< - @b 0: 240 MHz
	///< - @b 1: 320 MHz
	///< - @b 2: Reserved
	///< - @b 3: PLL13

	TSMUX_CFG_ID_PAYLOADSIZE,       ///< Configure the TS packet payload size. Currently support only 184 or 176 Bytes only.
	///< The input value must use 184 or 176. Others no support.
	TSMUX_CFG_ID_SRC_ADDR,          ///< Muxing input buffer address. This input address can be byte aligned.
	TSMUX_CFG_ID_DST_ADDR,          ///< Muxing output buffer address. This output address can be 4-bytes aligned.
	TSMUX_CFG_ID_SRC_LENGTH,        ///< Muxing input bit stream length in bytes. Valid value from 0x1 to 0x3ffffff.

	TSMUX_CFG_ID_DST_BUFSIZE,       ///< Set the destination buffer size. If the destination buffer is not enough for source input,
	///< it would output warning messages as notifications.
	TSMUX_CFG_ID_MUXING_LEN,        ///< Get Last Mux output size.

	TSMUX_CFG_ID_SYNC_BYTE,         ///< TS Stream header's Field: Sync Byte.
	TSMUX_CFG_ID_CONTINUITY_CNT,    ///< TS Stream header's Field: continuity counter initial value. Valid value 0x0 ~ 0xF.
	TSMUX_CFG_ID_PID,               ///< TS Stream header's Field: 13-bits Packet ID. Valid value 0x0 ~ 0x1FFF.
	TSMUX_CFG_ID_TEI,               ///< TS Stream header's Field: Transport Error Indicator. Valid value 0 or 1.
	TSMUX_CFG_ID_TP,                ///< TS Stream header's Field: Transport Priority. Valid value 0 or 1.
	TSMUX_CFG_ID_SCRAMBLECTRL,      ///< TS Stream header's Field: Scrambling Control. Valid value 0~3. The value 0 means Not Scrambled.
	TSMUX_CFG_ID_START_INDICTOR,    ///< TS Stream header's Field: Payload Unit Start Indicator bit.

	TSMUX_CFG_ID_STUFF_VAL,         ///< Set Stuff values. Default value is 0x00.
	TSMUX_CFG_ID_ADAPT_FLAGS,       ///< Set 8bits adaptation field flags.

	TSMUX_CFG_ID_ACCMUXING_LEN,     ///< Get Accumulated Mux output size after last flush.

	TSMUX_CFG_ID_CURR_ADDR,         ///< Get muxing current buffer address. This current address can be 4-bytes aligned.

	TSMUX_CFG_ID_CON_CURR_CNT,      ///< Get continue current counter value.

	TSDEMUX_CFG_ID_SYNC_BYTE,           ///< Set SYNC Byte checking value. Default is 0x47.
	TSDEMUX_CFG_ID_ADAPTATION_FLAG,     ///< Set Adaptation Flag checking value. Default is 0x00.

	TSDEMUX_CFG_ID_PID0_ENABLE,         ///< Enable/Disable of the filtering for TS packet's payload with PID0.
	TSDEMUX_CFG_ID_PID1_ENABLE,         ///< Enable/Disable of the filtering for TS packet's payload with PID1.
	TSDEMUX_CFG_ID_PID2_ENABLE,         ///< Enable/Disable of the filtering for TS packet's payload with PID2.

	TSDEMUX_CFG_ID_PID0_VALUE,          ///< 13bits PID-0 value.
	TSDEMUX_CFG_ID_PID1_VALUE,          ///< 13bits PID-1 value.
	TSDEMUX_CFG_ID_PID2_VALUE,          ///< 13bits PID-2 value.

	TSDEMUX_CFG_ID_CONTINUITY0_MODE,    ///<
	TSDEMUX_CFG_ID_CONTINUITY1_MODE,    ///<
	TSDEMUX_CFG_ID_CONTINUITY2_MODE,    ///<

	TSDEMUX_CFG_ID_CONTINUITY0_VALUE,   ///<
	TSDEMUX_CFG_ID_CONTINUITY1_VALUE,   ///<
	TSDEMUX_CFG_ID_CONTINUITY2_VALUE,   ///<

	TSDEMUX_CFG_ID_CONTINUITY0_LAST,    ///< Get the last continuity counter value after previous operation done.
	TSDEMUX_CFG_ID_CONTINUITY1_LAST,    ///< Get the last continuity counter value after previous operation done.
	TSDEMUX_CFG_ID_CONTINUITY2_LAST,    ///< Get the last continuity counter value after previous operation done.

	TSDEMUX_CFG_ID_IN_ADDR,             ///< Set Input Start Address
	TSDEMUX_CFG_ID_IN_SIZE,             ///< Set Input Size

	TSDEMUX_CFG_ID_OUT0_ADDR,           ///< Set Out0 Start Address
	TSDEMUX_CFG_ID_OUT1_ADDR,           ///< Set Out1 Start Address
	TSDEMUX_CFG_ID_OUT2_ADDR,           ///< Set Out2 Start Address

	HWCOPY_CFG_ID_CMD,                  ///< Set hwcopy command
	HWCOPY_CFG_ID_CTEX,                 ///< Set constant value

	HWCOPY_CFG_ID_SRC_ADDR,             ///< Set Input Start Address
	HWCOPY_CFG_ID_DST_ADDR,             ///< Set Output Start Address
	HWCOPY_CFG_ID_SRC_LENGTH,           ///< Set Input Size

	TSE_CFG_ID_TOTAL,
	ENUM_DUMMY4WORD(TSE_CFG_ID)
} TSE_CFG_ID;

typedef enum {
	TSDEMUX_STATUS_OK               = 0x00000000,
	TSDEMUX_STATUS_NO_OPEN          = 0x00000010,

	TSDEMUX_STATUS_CONTINUITY_ERR0  = 0x00000100,
	TSDEMUX_STATUS_TEI0             = 0x00000200,
	TSDEMUX_STATUS_SYNC_ERR0        = 0x00000400,
	TSDEMUX_STATUS_ADAPFLG_ERR0     = 0x00000800,

	TSDEMUX_STATUS_CONTINUITY_ERR1  = 0x00010000,
	TSDEMUX_STATUS_TEI1             = 0x00020000,
	TSDEMUX_STATUS_SYNC_ERR1        = 0x00040000,
	TSDEMUX_STATUS_ADAPFLG_ERR1     = 0x00080000,

	TSDEMUX_STATUS_CONTINUITY_ERR2  = 0x01000000,
	TSDEMUX_STATUS_TEI2             = 0x02000000,
	TSDEMUX_STATUS_SYNC_ERR2        = 0x04000000,
	TSDEMUX_STATUS_ADAPFLG_ERR2     = 0x08000000,

	TSDEMUX_STATUS_SIZE_ERR         = 0x10000000,

	TSDEMUX_STATUS_ALL              = 0x0F0F0F00,
	ENUM_DUMMY4WORD(TSDEMUX_STATUS)
} TSDEMUX_STATUS;


/**
    HwCopy operation cmmand

    Select which HwCopy operation command for HwCopy engine operation.

*/
typedef enum {
	HWCOPY_LINEAR_SET,            //0x0        ///< linear set constant to addressed memory
	HWCOPY_BLOCK_SET,          //0x1        ///< block set constant to addressed memory
	HWCOPY_LINEAR_COPY,         //0x2        ///< linear copy to addressed memory
	HWCOPY_BLOCK_COPY,          //0x3        ///< block copy to addressed memory


	ENUM_DUMMY4WORD(HWCOPY_CMD)
} HWCOPY_CMD;


/**
    HwCopy Memory identifier

    Select Memory A/B.

    @note For HWCOPY_MEM
*/
typedef enum {
	HWCOPY_MEM_ID_A,                          ///< Memory A
	HWCOPY_MEM_ID_B,                          ///< Memory B

	ENUM_DUMMY4WORD(HWCOPY_MEM_ID)
} HWCOPY_MEM_ID;


/**
    HwCopy Memory structure

    Description of HwCopy memory

    @note For HWCOPY_REQUEST
*/
typedef struct HWCOPY_MEM {
	HWCOPY_MEM_ID memID;            ///< Memory identifier
	UINT32 uiAddress;               ///< Memory DRAM address (unit: byte)
	UINT32 uiLineoffset;            ///< Memory lineoffset (unit: byte) (Must be 4 byte alignment)
	UINT32 uiWidth;                 ///< Memory width (unit: byte)
	UINT32 uiHeight;                ///< Memory height (unit: line)

	struct HWCOPY_MEM *pNext;         ///< Link to next Memory. (Fill NULL if this is last item)
} HWCOPY_MEM, *PHWCOPY_MEM;

/**
    HwCopy constant text description

    Description for each constant text

    @note For HWCOPY_REQUEST
*/
typedef struct HWCOPY_CTEX {
	UINT32 ctex;                    ///<  context
	UINT32 datalenth;               ///< Data length of linear set and copy (unit: byte)

} HWCOPY_CTEX, *PHWCOPY_CTEX;


/**
    HwCopy request structure

    Description of hwcopy request

    @note For hwcopy_request()
*/
typedef struct {
	DRV_VER_INFO verInfo;           ///< Driver version
	PHWCOPY_MEM pMemoryDescript;       ///< Descriptions of Memories for command
	HWCOPY_CTEX pCtexDescript;
	HWCOPY_CMD command;

} HWCOPY_REQUEST, *PHWCOPY_REQUEST;

/*
    HWCOPY_MODE_ENUM
*/
typedef enum {
	HWCOPY_MODE_SET_LINEAR,
	HWCOPY_MODE_COPY_LINEAR,
	HWCOPY_MODE_SET_BLOCK,
	HWCOPY_MODE_COPY_BLOCK,

	ENUM_DUMMY4WORD(OP_MODE_ENUM)
} HWCOPY_MODE_ENUM;

/*
    TSE_MODE
*/
typedef enum {
	TSE_MODE_TSMUX,
	TSE_MODE_TSDEMUX,
	TSE_MODE_HWCOPY,

	ENUM_DUMMY4WORD(TSE_MODE_ENUM)
} TSE_MODE_NUM;

//
//  Extern APIs
//
extern ER       tse_open(void);
extern ER       tse_close(void);
extern BOOL     tse_isOpened(void);

extern ER       tse_start(BOOL bWait, TSE_MODE_NUM OP_MODE);
extern ER       tse_waitDone(void);
extern ER       tse_getOutSize(UINT32 *pOutSize0, UINT32 *pOutSize1, UINT32 *pOutSize2);

extern ER       tse_setConfig(TSE_CFG_ID CfgID, UINT32 uiCfgValue);
extern UINT32   tse_getConfig(TSE_CFG_ID CfgID);

//@}
#endif
