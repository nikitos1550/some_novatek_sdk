/**
    MIPI-DSI Controller global header

    MIPI-DSI Controller global header

    @file       dsi.h
    @ingroup    mIDrvDisp_DSI
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _DSI_H
#define _DSI_H

/**
    @addtogroup mIDrvDisp_DSI
*/
//@{

/**
    @name   Host to peripheral packet data types

*/
//@{
#define DATA_TYPE_SHORT_READ_NO_PARAM           0x06    ///< packet size short, no parameter
#define DATA_TYPE_GENERIC_SHORT_READ_2_PARAM    0x24    ///< By manufacturer
#define DATA_TYPE_SHORT_WRITE_NO_PARAM          0x05    ///< Short packet write packet size short, no parameter
#define DATA_TYPE_SHORT_WRITE_1_PARAM           0x15
#define DATA_TYPE_SET_MAX_RT_PKT_SIZE           0x37
#define DATA_TYPE_LONG_WRITE                    0x39
#define DATA_TYPE_PACKET_STREAM_RGB_565_PACKED  0x0E
#define DATA_TYPE_PACKET_STREAM_RGB_666_PACKED  0x1E
#define DATA_TYPE_PACKET_STREAM_RGB_666_LOOSELY 0x2E
#define DATA_TYPE_PACKET_STREAM_RGB_888_PACKED  0x3E
//@}

/**
    @name   Ultra low power state entry command pattern

    @note for dsi_setEscapeEntry()
*/
//@{
#define ULPS_ENTRY_CMD_LPDT                     0x87    ///< Low power data transmission
#define ULPS_ENTRY_CMD_ULPS                     0x78    ///< Enter ultra Low power state
#define ULPS_ENTRY_CMD_RESET                    0x46    ///< reset trigger
#define ULPS_ENTRY_CMD_TE                       0xBA    ///< Tearing effect
#define ULPS_ENTRY_CMD_ACK                      0x12    ///< Ack
//@}

/**
    @name   Display command Set (DCS)

*/
//@{
#define DCS_CMD_SW_RESET                        0x01    ///<Power for the display panel is on
#define DCS_CMD_EXIT_SLEEP_MODE                 0x11    ///<Power for the display panel is on
#define DCS_CMD_SET_DISPLAY_ON                  0x29    ///<Show the image on the display device
#define DCS_CMD_SET_ALL_PIX_ON                  0x23
#define DCS_CMD_SET_ALL_PIX_OFF                 0x22
#define DCS_CMD_MEMORY_ACCESS_CTRL              0x36    ///<Show the image on the display device
#define DCS_CMD_MEMORY_WRITE                    0x2C
#define DCS_CMD_MEMORY_WRITE_CONT               0x3C
#define DCS_CMD_COLUMN_ADDR_SET                 0x2A
#define DCS_CMD_GAMMA_SET                       0x26
#define DCS_CMD_NULL_PKT                        0x09
#define DCS_CMD_READ_ID0                        0x04
#define DCS_CMD_READ_PWR_MODE                   0x0A
#define DCS_CMD_READ_ID1                        0xDA
#define DCS_CMD_READ_ID2                        0xDB
#define DCS_CMD_READ_ID3                        0xDC
#define DCS_CMD_READ_STATUS                     0xF2
#define DCS_CMD_SET_TE_OFF                      0x34
#define DCS_CMD_SET_TE_ON                       0x35
//@}
/**
    DSI mode select

    @note for dsi_setConfig()
*/
typedef enum {
	DSI_MODE_MANUAL_MODE = 0,   ///< Manual Commands Only: Send CMD(s) and auto clear DSI_TX_EN
	DSI_MODE_AUTO_MODE1,        ///< Send single frame only and auto clear DSI_TX_EN at frame end (No commands)
	DSI_MODE_AUTO_MODE2,        ///< Auto Mode 2: FRM1 + FRM2 + ¡K
	DSI_MODE_AUTO_MODE3,        ///< Auto Mode 3: CMD(s) + FRM1 + CMD(s) + FRM2 + ...

	DSI_MODE_CNT,

	ENUM_DUMMY4WORD(DSI_MODESEL)
} DSI_MODESEL;

/**
    DSI pixel format select

    @note for dsi_setConfig()
*/
typedef enum {
	DSI_PIXEL_RGB_565 = 0,      ///< 16 bits / pixel
	DSI_PIXEL_RGB_666_PACKETED, ///< 18 bits / pixel, size should multiple of 9 bytes
	DSI_PIXEL_RGB_666_LOOSELY,  ///< 18 bits / pixel, size should multiple of 3 bytes
	DSI_PIXEL_RGB_888,          ///< 24 bits / pixel

	DSI_PIXEL_FMT_CNT,
	ENUM_DUMMY4WORD(DSI_PIXEL_FORMATSEL)
} DSI_PIXEL_FORMATSEL;


/**
    DSI pixel packet mode select

    @note for dsi_setConfig()
*/
typedef enum {
	DSI_PIXMODE_VIDEO_MODE = 0, ///< video   mode (without framebuffer)
	DSI_PIXMODE_COMMAND_MODE,   ///< command mode (with framebuffer)

	DSI_PIXMODE_CNT,
	ENUM_DUMMY4WORD(DSI_PIXPKT_MODESEL)
} DSI_PIXPKT_MODESEL;


/**
    DSI video packet type select

    @note for dsi_setConfig()
*/
typedef enum {
	DSI_VIDEOPKT_TYPE_SYNC_PULSE = 0,
	DSI_VIDEOPKT_TYPE_SYNC_EVENT,       ///< Burst mode

	ENUM_DUMMY4WORD(DSI_VIDEOPKT_TYPESEL)
} DSI_VIDEOPKT_TYPESEL;

/**
    DSI Lane select

    @note for dsi_setConfig()
*/
typedef enum {
	DSI_DATA_LANE_0 = 0,        ///< Data lane 0
	DSI_DATA_LANE_1,            ///< Data lane 1
	DSI_DATA_LANE_2,            ///< Data lane 2
	DSI_DATA_LANE_3,            ///< Data lane 3

	DSI_DATA_LANE_CNT,

	ENUM_DUMMY4WORD(DSI_LANESEL)
} DSI_LANESEL;


/**
    DSI CLK ULP select

    @note for dsi_setConfig()
*/
typedef enum {
	DSI_ULP_EXIT = 0,           ///< Clock lane exit ULPS
	DSI_ULP_ENTER = 1,          ///< Clock lane enter ULPS
	DSI_ULP_SEL_CNT,

	ENUM_DUMMY4WORD(DSI_ULP_SEL)
} DSI_ULP_SEL;


/**
    DSI escape control operation select

    @note for dsi_setConfig()
*/
typedef enum {
	DSI_ESCAPE_TRIGGER = 0,      ///< Escape command trigger
	DSI_ESCAPE_START,            ///< Escape command start procedure
	DSI_ESCAPE_STOP,             ///< Escape command stop procedure
	ENUM_DUMMY4WORD(DSI_ESC_OP)
} DSI_ESC_OP;


/**
    DSI escape control operation stop action selection

    @note for dsi_setConfig()
*/
typedef enum {
	DSI_SET_ESC_NOT_STOP = 0x0,         ///< Escape command not stop
	DSI_SET_ESC_STOP_WITH_ESC_CMD,      ///< Escape command stop but not send exit cmd
	DSI_SET_ESC_STOP_WITHOUT_ESC_CMD,   ///< Escape command stopp and send exit cmd

	ENUM_DUMMY4WORD(DSI_CFG_ESCAPE_CMD_STOP_TYPE)
} DSI_CFG_ESCAPE_CMD_STOP_TYPE;


typedef struct {
	UINT32  uiDataType;
	UINT32  uiVirtualChannel;

	UINT32  uiPacketType;
	UINT32  uiDCSCmd;
	UINT8    *uiParam;
	UINT32  uiParamCnt;
} DSI_CMD_CTRL_PARAM, *PDSI_CMD_CTRL_PARAM;

typedef struct {
	UINT32                  uiCmdNo;
	BOOL                    ubBTAEN;
	BOOL                    ubBTAOnly;
	UINT32                  uiSramOfs;
	BOOL                    ubEOTEN;
	PDSI_CMD_CTRL_PARAM     pDSICmdCtx;
} DSI_CMD_RW_CTRL_PARAM, *PDSI_CMD_RW_CTRL_PARAM;

typedef enum {
	DSI_CONFIG_ID_MODE,         //< Configure DSI mode
	//< @note for DSI_CFG_MODE_SEL
	//< Context can be any of:
	//< - @b DSI_MODE_MANUAL_MODE  : Manual Commands Only: Send CMD(s) and auto clear DSI_TX_EN
	//< - @b DSI_MODE_AUTO_MODE1   : Send single frame only and auto clear DSI_TX_EN at frame end (No commands)
	//< - @b DSI_MODE_AUTO_MODE2   : Auto Mode 2: FRM1 + FRM2 + ...
	//< - @b DSI_MODE_AUTO_MODE3   : Auto Mode 3: CMD(s) + FRM1 + CMD(s) + FRM2 + ...

	DSI_CONFIG_ID_PIXEL_FMT,
	DSI_CONFIG_ID_PIXPKT_MODE,
	DSI_CONFIG_ID_VDOPKT_TYPE,
	DSI_CONFIG_ID_ECC_CHK_EN,
	DSI_CONFIG_ID_FRMEND_BTA_EN,
	DSI_CONFIG_ID_EOT_PKT_EN,
	DSI_CONFIG_ID_BLANK_CTRL,
	DSI_CONFIG_ID_INTER_PKT_LP,
	DSI_CONFIG_ID_CLK_LP_CTRL,
	DSI_CONFIG_ID_SYNC_EN,
	DSI_CONFIG_ID_SYNC_SRC,
	DSI_CONFIG_ID_SYNC_WITH_SETTEON,
	DSI_CONFIG_ID_SYNC_WITH_SETTEON_RTY,
	DSI_CONFIG_ID_RGB_SWAP,
	DSI_CONFIG_ID_RGB_BITFLIP,
	DSI_CONFIG_ID_DATALANE_NO,

	DSI_CONFIG_ID_PIXPKT_PH_DT,
	DSI_CONFIG_ID_PIXPKT_PH_VC,
	DSI_CONFIG_ID_DCS_CT0,
	DSI_CONFIG_ID_DCS_CT1,

	DSI_CONFIG_ID_SYNCEVT_SLICE_NO,
	DSI_CONFIG_ID_SYNCEVT_NULL_LEN,

	DSI_CONFIG_ID_VSA,
	DSI_CONFIG_ID_VTOTAL,

	DSI_CONFIG_ID_VVALID_START,
	DSI_CONFIG_ID_VVALID_END,

	DSI_CONFIG_ID_HSA,
	DSI_CONFIG_ID_BLLP,

	DSI_CONFIG_ID_HBP,
	DSI_CONFIG_ID_HFP,

	DSI_CONFIG_ID_HACT,

	DSI_CONFIG_ID_TLPX,
	DSI_CONFIG_ID_BTA_TA_GO,
	DSI_CONFIG_ID_BTA_TA_SURE,
	DSI_CONFIG_ID_BTA_TA_GET,
	DSI_CONFIG_ID_THS_PREPARE,
	DSI_CONFIG_ID_THS_ZERO,
	DSI_CONFIG_ID_THS_TRAIL,
	DSI_CONFIG_ID_THS_EXIT,
	DSI_CONFIG_ID_TWAKEUP,
	DSI_CONFIG_ID_TCLK_PREPARE,
	DSI_CONFIG_ID_TCLK_ZERO,
	DSI_CONFIG_ID_TCLK_POST,
	DSI_CONFIG_ID_TCLK_PRE,
	DSI_CONFIG_ID_TCLK_TRAIL,

	DSI_CONFIG_ID_BTA_TMOUT_VAL,
	DSI_CONFIG_ID_BTA_HANDSK_TMOUT_VAL,

	DSI_CONFIG_ID_SYNC_POL,
	DSI_CONFIG_ID_SYNC_SEL,
	DSI_CONFIG_ID_SYNC_DLY_CNT,
	DSI_CONFIG_ID_TE_BTA_INTERVAL,

	DSI_CONFIG_ID_CLK_PHASE_OFS,
	DSI_CONFIG_ID_DAT0_PHASE_OFS,
	DSI_CONFIG_ID_DAT1_PHASE_OFS,
	DSI_CONFIG_ID_PHASE_DELAY_ENABLE_OFS,

	//DSI_CFG_ID_FREQ
	DSI_CONFIG_ID_FREQ,         ///< DSI module target clock (Unit: MHz)
	DSI_CONFIG_ID_IDEHVALID,    ///< DSI module of IDE HVALID setting

	DSI_CONFIG_ID_LANSEL_D0,
	DSI_CONFIG_ID_LANSEL_D1,
	DSI_CONFIG_ID_LANSEL_D2,
	DSI_CONFIG_ID_LANSEL_D3,
	ENUM_DUMMY4WORD(DSI_CONFIG_ID)
} DSI_CONFIG_ID;


#if 0
typedef enum {
	DSI_CFG_MODE_CMD_ONLY,
	DSI_CFG_MODE_SINGLE_FRM,
	DSI_CFG_MODE_MULTI_FRM,
	DSI_CFG_MODE_MULTI_CMD_FRM,

	ENUM_DUMMY4WORD(DSI_CFG_MODE_SEL)
} DSI_CFG_MODE_SEL;


typedef enum {
	DSI_CFG_PIXEL_FMT_RGB565,
	DSI_CFG_PIXEL_FMT_RGB666_PACKED,
	DSI_CFG_PIXEL_FMT_RGB666_LOOSELY,
	DSI_CFG_PIXEL_FMT_RGB888,

	ENUM_DUMMY4WORD(DSI_CFG_PIXEL_FMT_SEL)
} DSI_CFG_PIXEL_FMT_SEL;


typedef enum {
	DSI_CFG_PIXPKT_VIDEO_MODE,
	DSI_CFG_PIXPKT_COMMAND_MODE,

	ENUM_DUMMY4WORD(DSI_CFG_PIXPKT_MODE_SEL)
} DSI_CFG_PIXPKT_MODE_SEL;


typedef enum {
	DSI_CFG_VDOPKT_SYNCPULSE_TYPE,
	DSI_CFG_VDOPKT_SYNCEVENT_TYPE,

	ENUM_DUMMY4WORD(DSI_CFG_VDOPKT_TYPE_SEL)
} DSI_CFG_VDOPKT_TYPE_SEL;
#endif

// -----------------------------------------------------------------------------
// DSI Command RW control (0x2C)
// -----------------------------------------------------------------------------
/**
    DSI command RW control configuration

    @note for dsi_setCmdRWCtrl()
*/
typedef enum {
	DSI_SET_CMD_NUMBER = 0x0,           ///< The number of (DSI_CMD_NUMBER+1) commands would be sent in DSI_MODE= 0/3/4.
	///< Context is
	///< - @b UINT32 : 1 - 8
	DSI_SET_BTA_EN,                     ///< Enable the Bus Turn Around (BTA) process
	///< after the final commands are sent.
	///< The READ command should put at the final commands,
	///< and the data would be read back after BTA.
	///< This field is valid for DSI_MODE = 0/3/4.
	///< Context is
	///< - @b UINT32 : 1 : Enable
	///< - @b UINT32 : 0 : Disable
	DSI_SET_BTA_ONLY,                   ///< Context is
	///< - @b UINT32 : 1 : Issue BTA only
	///< - @b UINT32 : 0 : Disable
	DSI_SET_SRAM_READ_OFS,              ///< Context is
	///< - @b UINT32 : Sram offset 0~255 (256 bytes total)
	DSI_GET_SRAM_READ_CNT,              ///< Context is
	///< - @b UINT32*: Sram offset 0~256


	DSI_RW_CMD_CTRL_CNT,
	ENUM_DUMMY4WORD(DSI_CFG_CMD_RW_CTRL)
} DSI_CFG_CMD_RW_CTRL;


// -----------------------------------------------------------------------------
// DSI Command Register 0,1 (0x30,0x34)
// -----------------------------------------------------------------------------
/**
    DSI packet type

    @note for dsi_setCmdRegister()
*/
//@{
typedef enum {
	DSI_SHORT_PACKET = 0x0,             ///< This DSI short packet
	DSI_LONG_PACKET,                    ///< This DSI long packet

	DSI_PT_CNT,
	ENUM_DUMMY4WORD(DSI_PACKET_TYPE)
} DSI_PACKET_TYPE;
//@}

/**
    DSI command register set

    @note for dsi_setCmdRegister()
*/
typedef enum {
	DSI_CMD_REG0 = 0x0,
	DSI_CMD_REG1,
	DSI_CMD_REG2,
	DSI_CMD_REG3,
	DSI_CMD_REG4,
	DSI_CMD_REG5,
	DSI_CMD_REG6,
	DSI_CMD_REG7,

	DSI_CMD_SET_CNT,
	ENUM_DUMMY4WORD(DSI_CMD_REG_NUM)
} DSI_CMD_REG_NUM;


/**
    DSI command register configuration

    @note for dsi_setCmdRegister()
*/
typedef enum {
	DSI_SET_CMD_DT = 0x0,                   // The command n Data Type in the Data Identification (DI) field of the DSI packet header.
	DSI_SET_CMD_VC,                         // The virtual channel ID for the command n
	DSI_SET_CMD_WC,                         // If the command n is the Short Packet, this field is the command data for this short packet command.
	// If the command n is the Long Packet, this field is the length for this long packet command.
	// The long packet command data is stored in the SRAM in serially by commands. The command data n is from the SRAM address offset 0.

	DSI_SET_CMD_DATA = DSI_SET_CMD_WC,
	DSI_SET_CMD_PT,                         // The Packet Type of the command n.
	// 0: Short Packet
	// 1: Long Packet
	ENUM_DUMMY4WORD(DSI_CFG_CMD_REG)
} DSI_CFG_CMD_REG;

extern ER       dsi_open(void);
extern ER       dsi_close(void);
extern BOOL     dsi_isOpened(void);
extern ER       dsi_setTXEN(BOOL ubEN, BOOL ubWait);
extern ER       dsi_waitTXDone(void);
extern ER       dsi_waitFrameEnd(void);
extern ER       dsi_issueBTA(void);
extern ER       dsi_ULPSTrigger(void);
extern UINT32   dsi_getErrorReport(void);
extern ER       dsi_setLPSClockSel(DSI_ULP_SEL uiULPSel);


extern ER       dsi_setHSDcsCommand(PDSI_CMD_RW_CTRL_PARAM pDcsCtx);
extern ER       dsi_setConfig(DSI_CONFIG_ID CfgID, UINT32 uiConfig);
extern ER       dsi_setCmdRWCtrl(DSI_CFG_CMD_RW_CTRL uiCmd, UINT32 param);
extern ER       dsi_setLPDcsCommand(DSI_LANESEL uiDataLane, UINT32 uiLPCmd);
extern ER       dsi_setEscapeEntry(DSI_LANESEL uiDataLane, UINT32 uiEntryCmd, BOOL bStop);
extern ER       dsi_setEscapeControl(DSI_LANESEL uiDataLane, DSI_ESC_OP uiEscOp, BOOL ubEN);
extern ER       dsi_setCmdRegister(DSI_CMD_REG_NUM uiCmdRegNo, DSI_CFG_CMD_REG uiCmdReg, UINT32 param);
extern ER       dsi_setEscapeTransmission(DSI_LANESEL uiDataLane, UINT32 uiCmd, DSI_CFG_ESCAPE_CMD_STOP_TYPE exit);
extern BOOL     dsi_getPhaseDelayInfo(UINT32 *puiClkPhase, UINT32 *puiD0Phase, UINT32 *puiD1Phase);

extern UINT32   dsi_getConfig(DSI_CONFIG_ID ConfigID);




//@}

#endif
