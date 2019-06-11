/**
    Header file for HDMI module

    Header file for HDMI module.
    The user can reference this section for the detail description of the each driver API usage
    and also the parameter descriptions and its limitations.
    The overall combinational usage flow is introduced in the application note document,
    and the user must reference to the application note for the driver usage flow.

    @file       hdmitx.h
    @ingroup    mIDrvDisp_HDMITx
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/

#ifndef  _HDMITX_H
#define  _HDMITX_H

#include "Type.h"
#include "Driver.h"

/**
    @addtogroup mIDrvDisp_HDMITx
*/
//@{


/**
    HDMI-Tx output Video Format ID

    This enumeration is used in hdmitx_setConfig(HDMI_CONFIG_ID_VIDEO) to specify the HDMI output video resolution,
    and also used in the display device object control code "SET_HDMIMODE" to specify the video mode.
    These definitions are from the TV standard CEA-861D and all of the output timngs such as frame rate/blanking/...
    are the same as the CEA-861D listed.
*/
typedef enum {
	HDMI_640X480P60         = 1,    ///< HDMI Video format is 640x480 & Progressive 60fps
	HDMI_720X480P60         = 2,    ///< HDMI Video format is 720x480 & Progressive 60fps & 4:3
	HDMI_720X480P60_16X9    = 3,    ///< HDMI Video format is 720x480 & Progressive 60fps & 16:9
	HDMI_1280X720P60        = 4,    ///< HDMI Video format is 1280x720 & Progressive 60fps
	HDMI_1920X1080I60       = 5,    ///< HDMI Video format is 1920x1080 & Interlaced 60fps
	HDMI_720X480I60         = 6,    ///< HDMI Video format is 720x480 & Interlaced 60fps
	HDMI_720X480I60_16X9    = 7,    ///< HDMI Video format is 720x480 & Interlaced 60fps & 16:9
	HDMI_720X240P60         = 8,    ///< HDMI Video format is 720x240 & Progressive 60fps
	HDMI_720X240P60_16X9    = 9,    ///< HDMI Video format is 720x240 & Progressive 60fps & 16:9
	HDMI_1440X480I60        = 10,   ///< HDMI Video format is 1440x480 & Interlaced 60fps
	HDMI_1440X480I60_16X9   = 11,   ///< HDMI Video format is 1440x480 & Interlaced 60fps & 16:9
	HDMI_1440X240P60        = 12,   ///< HDMI Video format is 1440x240 & Progressive 60fps
	HDMI_1440X240P60_16X9   = 13,   ///< HDMI Video format is 1440x240 & Progressive 60fps & 16:9
	HDMI_1440X480P60        = 14,   ///< HDMI Video format is 1440x480 & Progressive 60fps
	HDMI_1440X480P60_16X9   = 15,   ///< HDMI Video format is 1440x480 & Progressive 60fps & 16:9
	HDMI_1920X1080P60       = 16,   ///< HDMI Video format is 1920x1080 & Progressive 60fps
	HDMI_720X576P50         = 17,   ///< HDMI Video format is 720x576 & Progressive 50fps
	HDMI_720X576P50_16X9    = 18,   ///< HDMI Video format is 720x576 & Progressive 50fps & 16:9
	HDMI_1280X720P50        = 19,   ///< HDMI Video format is 1280x720 & Progressive 50fps
	HDMI_1920X1080I50       = 20,   ///< HDMI Video format is 1920x1080 & Interlaced 50fps
	HDMI_720X576I50         = 21,   ///< HDMI Video format is 720x576 & Interlaced 50fps
	HDMI_720X576I50_16X9    = 22,   ///< HDMI Video format is 720x576 & Interlaced 50fps & 16:9
	HDMI_720X288P50         = 23,   ///< HDMI Video format is 720x288 & Progressive 50fps
	HDMI_720X288P50_16X9    = 24,   ///< HDMI Video format is 720x288 & Progressive 50fps & 16:9
	HDMI_1440X576I50        = 25,   ///< HDMI Video format is 1440x576 & Interlaced 50fps
	HDMI_1440X576I50_16X9   = 26,   ///< HDMI Video format is 1440x576 & Interlaced 50fps & 16:9
	HDMI_1440X288P50        = 27,   ///< HDMI Video format is 1440x288 & Progressive 50fps
	HDMI_1440X288P50_16X9   = 28,   ///< HDMI Video format is 1440x288 & Progressive 50fps & 16:9
	HDMI_1440X576P50        = 29,   ///< HDMI Video format is 1440x576 & Progressive 50fps
	HDMI_1440X576P50_16X9   = 30,   ///< HDMI Video format is 1440x576 & Progressive 50fps & 16:9
	HDMI_1920X1080P50       = 31,   ///< HDMI Video format is 1920x1080 & Progressive 50fps
	HDMI_1920X1080P24       = 32,   ///< HDMI Video format is 1920x1080 & Progressive 24fps
	HDMI_1920X1080P25       = 33,   ///< HDMI Video format is 1920x1080 & Progressive 25fps
	HDMI_1920X1080P30       = 34,   ///< HDMI Video format is 1920x1080 & Progressive 30fps
	HDMI_1920X1080I50_VT1250 = 39,  ///< HDMI Video format is 1920x1080 & Interlaced  50fps & V-total is 1250 lines
	HDMI_1920X1080I100      = 40,   ///< HDMI Video format is 1920x1080 & Interlaced  100fps
	HDMI_1280X720P100       = 41,   ///< HDMI Video format is 1280X720  & Progressive 100fps
	HDMI_720X576P100        = 42,   ///< HDMI Video format is 720X576   & Progressive 100fps
	HDMI_720X576P100_16X9   = 43,   ///< HDMI Video format is 720X576   & Progressive 100fps & 16:9
	HDMI_720X576I100        = 44,   ///< HDMI Video format is 720X576  & Interlaced 100fps
	HDMI_720X576I100_16X9   = 45,   ///< HDMI Video format is 720X576  & Interlaced 100fps & 16:9
	HDMI_1920X1080I120      = 46,   ///< HDMI Video format is 1920X1080 & Interlaced 120fps
	HDMI_1280X720P120       = 47,   ///< HDMI Video format is 1280X720  & Progressive 120fps
	HDMI_720X480P120        = 48,   ///< HDMI Video format is 720X480   & Progressive 120fps
	HDMI_720X480P120_16X9   = 49,   ///< HDMI Video format is 720X480   & Progressive 120fps & 16:9
	HDMI_720X480I120        = 50,   ///< HDMI Video format is 720X480  & Interlaced 120fps
	HDMI_720X480I120_16X9   = 51,   ///< HDMI Video format is 720X480  & Interlaced 120fps & 16:9
	HDMI_720X576P200        = 52,   ///< HDMI Video format is 720X576  & Progressive 200fps
	HDMI_720X576P200_16X9   = 53,   ///< HDMI Video format is 720X576  & Progressive 200fps & 16:9
	HDMI_720X576I200        = 54,   ///< HDMI Video format is 720X576  & Interlaced 200fps
	HDMI_720X576I200_16X9   = 55,   ///< HDMI Video format is 720X576  & Interlaced 200fps & 16:9
	HDMI_720X480P240        = 56,   ///< HDMI Video format is 720X480  & Progressive 240fps
	HDMI_720X480P240_16X9   = 57,   ///< HDMI Video format is 720X480  & Progressive 240fps & 16:9
	HDMI_720X480I240        = 58,   ///< HDMI Video format is 720X480  & Interlaced 240fps
	HDMI_720X480I240_16X9   = 59,   ///< HDMI Video format is 720X480  & Interlaced 240fps & 16:9

	HDMI_VID_NO_CHANGE      = 0xFF, ///< HDMI Video format use current setting
	ENUM_DUMMY4WORD(HDMI_VIDEOID)
} HDMI_VIDEOID;

/**
    HDMI-Tx input audio format

    This structure is used in hdmitx_setConfig(HDMI_CONFIG_ID_AUDIO) to specify the Audio Sampling Rate in HDMI audio stream.
    This definition would be used in the HDMI audio infoFrame. Only these three audio modes are maindatory modes in HDMI specification,
    so sampling rates less than 32KHz are not supported in HDMI audio.
    Besides, we must also note that the HDMI has only the Stereo Audio Channels. The Mono channel can only playback to left ot right channel
    on the HDMI audio. The audio channel number larger than two are not supported in current chip version.
*/
typedef enum {
	HDMI_AUDIO32KHZ,                ///< HDMI Audio sampling rate uses 32KHz.
	HDMI_AUDIO44_1KHZ,              ///< HDMI Audio sampling rate uses 44.1KHz.
	HDMI_AUDIO48KHZ,                ///< HDMI Audio sampling rate uses 48KHz.

	HDMI_AUDIO_NO_CHANGE  = 0xFF,   ///< Reserve the curretn HDMI audio sampling rate.
	ENUM_DUMMY4WORD(HDMI_AUDIOFMT)
} HDMI_AUDIOFMT;

/**
    HDMI-Tx Video Ability Structure

    This structure is used in hdmitx_getVideoAbility() to get the 3D abilities of the TV/Display.
*/
typedef struct {
	HDMI_VIDEOID    VideoID;        ///< The Video Format which TV supports.
	UINT32          Vdo3DAbility;   ///< The 3D format supported for this video ID. The format uses the "bit-wise OR" of HDMI_3DFMT.

} HDMI_VDOABI, *PHDMI_VDOABI;

/**
    HDMI-Tx 3D Format Type

    This is used in hdmitx_setConfig(HDMI_CONFIG_ID_3D_FMT) and "HDMI_VDOABI" to specify the HDMI 3D format.
*/
typedef enum {
	HDMI3D_NONE             = 0x00, ///< None of the 3D function.
	HDMI3D_SIDEBYSIDE_HALF  = 0x01, ///< 3D Format: Side-by-Side-half. This results in the half of the horizontal resolution.
	HDMI3D_TOPANDBOTTOM     = 0x02, ///< 3D Format: Top-and-Bottom. This results in the half of the vertical resolution.

	ENUM_DUMMY4WORD(HDMI_3DFMT)
} HDMI_3DFMT;

/**
    HDMI EDID flags

    Each bit represents TV device support this feature or not.
    These flags are used for hdmitx_getEdidInfo() to determine the TV supported audio/video formats.
    All the information below is from TV's EDID.
    This information is used in the hdmi display device object for function control information.

    @name   HDMI EDID parsing results flags
*/
//@{
#define HDMITX_DTD1080P60       0x0001  ///< Video as 1080P60 from EDID's Detailed Timing Descriptor.
#define HDMITX_DTD720P60        0x0002  ///< Video as  720P60 from EDID's Detailed Timing Descriptor.
#define HDMITX_DTD640X480P60    0x0004  ///< Video as  480P60 from EDID's Detailed Timing Descriptor.
#define HDMITX_DTD720X480P60    0x0008  ///< Video as  480P60 from EDID's Detailed Timing Descriptor.
#define HDMITX_DTD1080I60       0x0010  ///< Video as 1080I60 from EDID's Detailed Timing Descriptor.
#define HDMITX_DTD720X480I60    0x0020  ///< Video as  480I60 from EDID's Detailed Timing Descriptor.
#define HDMITX_DTD1080P50       0x0100  ///< Video as 1080P50 from EDID's Detailed Timing Descriptor.
#define HDMITX_DTD720P50        0x0200  ///< Video as  720P50 from EDID's Detailed Timing Descriptor.
#define HDMITX_DTD576P50        0x0400  ///< Video as  576P50 from EDID's Detailed Timing Descriptor.
#define HDMITX_DTD1080I50       0x1000  ///< Video as 1080I50 from EDID's Detailed Timing Descriptor.
#define HDMITX_DTD576I50        0x2000  ///< Video as  576I50 from EDID's Detailed Timing Descriptor.

#define HDMITX_TV_YCBCR444     0x10000  ///< TV Support Video format: YCbCr-444.
#define HDMITX_TV_YCBCR422     0x20000  ///< TV Support Video format: YCbCr-422.
#define HDMITX_TV_UNDERSCAN    0x40000  ///< TV Support UnderScan.
#define HDMITX_TV_HDMI         0x80000  ///< TV Support HDMI. If not, it is DVI device.
#define HDMITX_ADO_32KHZ      0x100000  ///< TV Support Audio Sampling Rate 32Khz.
#define HDMITX_ADO_44KHZ      0x200000  ///< TV Support Audio Sampling Rate 44.1Khz.
#define HDMITX_ADO_48KHZ      0x400000  ///< TV Support Audio Sampling Rate 48Khz.
#define HDMITX_ADO_MORE       0x800000  ///< TV Support Advanced Audio Format.
//@}




//
//  HDMI CEC related header
//

/**
    HDMI CEC configuration identifications

    These definitions are used in the hdmicec_setConfig() and hdmicec_getConfig() to configure the CEC functionalities.
*/
typedef enum {
	CEC_CONFIG_ID_FREECNT1,         ///< Configure the Free Count-1 in the CEC Standard. Unit in CEC bit time.
	///< The free-count-1 is previous command sent successfully and want to send another command,
	///< the initiator must wait this duration. Default value is 0x7.
	CEC_CONFIG_ID_FREECNT2,         ///< Configure the Free Count-2 in the CEC Standard. Unit in CEC bit time.
	///< The free-count-2 is the new initiator must wait the bus idle time duration,
	///< and then be allowed to send command. Default value is 0x5.
	CEC_CONFIG_ID_FREECNT3,         ///< Configure the Free Count-3 in the CEC Standard. Unit in CEC bit time.
	///< The free-count-3 is when previous attemp to send failed, the initiator
	///< can wait this duration for the next bus idle time. Default value is 0x3.
	CEC_CONFIG_ID_RETRY_CNT,        ///< Configure the Re-Try times after command transmit failed.

	CEC_CONFIG_ID_TX_TRIG,          ///< Trigger the CEC command transmit. The interrupt status "CEC_TXFRM" would be triggered after transmitting done.
	CEC_CONFIG_ID_LOCAL_ADDR,       ///< Configure the CEC Local Address. The CEC controller would receive the CEC bus command's destination address matched with the local address.
	CEC_CONFIG_ID_SRC_ADDR,         ///< Configure the CEC source address. After CEC command is sent, the source address is used to notify the destination device where the command is from.
	CEC_CONFIG_ID_DEST_ADDR,        ///< Configure the CEC destination address. This is used to assign the command destination device address.
	CEC_CONFIG_ID_TXRISE_OFS,       ///< Configure the CEC command transmit timing Rise Offset. The value of 128 would be the standard value in the CEC standard. Unit in 10us.
	CEC_CONFIG_ID_TXFALL_OFS,       ///< Configure the CEC command transmit timing Fall Offset. The value of 128 would be the standard value in the CEC standard. Unit in 10us.

	CEC_CONFIG_ID_RXRISE_OFS,       ///< Configure the CEC command receive timing Rise Offset. The value of 20 would be the standard value in the CEC standard. Unit in 10us.
	CEC_CONFIG_ID_RXFALL_OFS,       ///< Configure the CEC command receive timing Fall Offset. The value of 35 would be the standard value in the CEC standard. Unit in 10us.
	CEC_CONFIG_ID_RXSTART_OFS,      ///< Configure the CEC command receive timing Start Bit Offset. The value of 20 would be the standard value in the CEC standard. Unit in 10us.
	CEC_CONFIG_ID_RXSMPL_OFS,       ///< Configure the CEC command receive sample timing Offset. The value of 128 would be the standard value in the CEC standard. Unit in 10us.

	CEC_CONFIG_ID_DEGLITCH,         ///< Configure the De-Glitch Time. Unit in 10us.
	CEC_CONFIG_ID_ERROR_LOW,        ///< Please set this value to 0x168 according to the CEC standard.
	CEC_CONFIG_ID_NACK_CTRL,        ///< If the NACK_CTRL is enabled, the CEC controller would NACK the in-coming command and cause the Tx re-transmit.
	CEC_CONFIG_ID_ARBI_RANGE,       ///< Configure the CEC arbitration lost judgement delay. The selection value would be "(uiConfig)x20" us. Default value is 3(60us). Valid Range from 0x0 to 0xF.

	CEC_CONFIG_ID_PHY_ADDR,         ///< Set the CEC physical address, this address is from the TV EDID information.

	ENUM_DUMMY4WORD(CEC_CONFIG_ID)
} CEC_CONFIG_ID;


/**
    HDMI configuration identifications

    These definitions are used in the hdmitx_setConfig() and hdmitx_getConfig() to configure the HDMI functionalities.
*/
typedef enum {
	HDMI_CONFIG_ID_VIDEO,           ///< Specify the HDMI video format. The input parameter must use one of the HDMI_VIDEOID definition.
	HDMI_CONFIG_ID_AUDIO,           ///< Specify the HDMI audio format. The input parameter must use one of the HDMI_AUDIO32KHZ / HDMI_AUDIO44_1KHZ / HDMI_AUDIO48KHZ.
	HDMI_CONFIG_ID_OE,              ///< Configure HDMI output enable. When setting to disable, the output pad power would be close together for power saving.

	HDMI_CONFIG_ID_AV_MUTE,         ///< Set the HDMI AV_MUTE flag in the general control packet in the HDMI standard. This would mute both the HDMI video and audio.
	HDMI_CONFIG_ID_AUDIO_MUTE,      ///< Set the Audio Mute Enable/Disable.
	HDMI_CONFIG_ID_AUDIO_STREAM_EN, ///< Enable/Disable the HDMI controller internal audio stream.
	HDMI_CONFIG_ID_SOURCE,          ///< The HDMI can be selected from IDE/IDE2 by the definition "HDMI_SOURCE".
	HDMI_CONFIG_ID_3D_FMT,          ///< This can be used to notify the TV/Display which of the 3D content in the HDMI streaming. Use the "HDMI_3DFMT" as input parameter.

	HDMI_CONFIG_ID_FORCE_RGB,       ///< Set ENABLE would force the HDMI output as RGB format. If set to DISABLE, the HDMI would output RGB or YUV format accrording to the EDID information.
	HDMI_CONFIG_ID_FORCE_UNDERSCAN, ///< Set ENABLE would force the HDMI output as UnderScan. If set to DISABLE, the HDMI would output UnderScan or OverScan accrording to the EDID information.
	HDMI_CONFIG_ID_VERIFY_VIDEOID,  ///< Enable/Disable the HDMI driver to verify video ID is supported by the TV/Display. If no support, the driver would map the video format to HDMI basic format.

	HDMI_CONFIG_ID_MANUFACTURER_ID, ///< Get TV/Display Manufacturer-id from EDID information. This definition is for hdmitx_getConfig() only.

	HDMI_CONFIG_ID_SWING_CTRL,      ///< Set HDMI Output Swing Control Setting value. Please use HDMI_SWING_CTRL as input parameter.
	HDMI_CONFIG_ID_SEL_BGR,         ///< Get Current select BGR Setting value. This can be used in hdmitx_getConfig() only.
	HDMI_CONFIG_ID_REG_BGR_EN,      ///< Get Current HDMI PHY REG_BGR_EN Value. This can be used in hdmitx_getConfig() only.
	HDMI_CONFIG_ID_BGR_CTRL,        ///< Set HDMI PHY Output Voltage adjustment. Default value is 0x4. Valid range is 0x0 to 0x7.
	HDMI_CONFIG_ID_BGR_TEST,        ///< Get Current HDMI PHY BGR TEST Value. This can be used in hdmitx_getConfig() only.
	HDMI_CONFIG_ID_CLKDETECT,       ///< Get HDMI Clock Detect Enable. This can be used in hdmitx_getConfig() only.
	HDMI_CONFIG_ID_PHY_PDB,         ///< Get HDMI PHY PDB Bit Status. This can be used in hdmitx_getConfig() only.
	HDMI_CONFIG_ID_TERM_EN,         ///< Get HDMI Internal Source Terminator Enable. This can be used in hdmitx_getConfig() only.
	HDMI_CONFIG_ID_BW_CTRL,         ///< Get HDMI PHY PLL bandwidth control setup value. This can be used in hdmitx_getConfig() only.
	HDMI_CONFIG_ID_CKSW_CTRL,       ///< Get HDMI PHY CKSW Control setup value. This can be used in hdmitx_getConfig() only.
	HDMI_CONFIG_ID_PHYREG_PD,       ///< Get HDMI PHY REG_PD setup value. This can be used in hdmitx_getConfig() only.
	HDMI_CONFIG_ID_RXSENSE,         ///< Get HDMI Receiver Sense. The return value TRUE means the Rx terminator is ON. This can be used in hdmitx_getConfig() only.

	HDMI_CONFIG_ID_HWDDC,           ///< Select the DDC Channel is using HW I2C controller or SW GPIO control. Default value is TRUE (HW I2C Control).
	///< Input Parameter TRUE is HW I2C controller and FALSE is SW GPIO control and the user must assign the GPIO pin locations.
	HDMI_CONFIG_ID_SWDDC_SCL,       ///< Assign SW DDC SCL pin's GPIO location. This is valid only if HDMI_CONFIG_ID_HWDDC set to FALSE.
	HDMI_CONFIG_ID_SWDDC_SDA,       ///< Assign SW DDC SDA pin's GPIO location. This is valid only if HDMI_CONFIG_ID_HWDDC set to FALSE.

	ENUM_DUMMY4WORD(HDMI_CONFIG_ID)
} HDMI_CONFIG_ID;


/**
    HDMI Video Source

    This is used as the input parameter in the hdmitx_setConfig(HDMI_CONFIG_ID_SOURCE) to select the HDMI video source.
*/
typedef enum {
	HDMI_SOURCE_IDE,                ///< Select HDMI video stream source from IDE.
	HDMI_SOURCE_IDE2,               ///< Select HDMI video stream source from IDE2.

	ENUM_DUMMY4WORD(HDMI_SOURCE)
} HDMI_SOURCE;


/**
    HDMI Output Swing Control

    This is used in hdmitx_setConfig(HDMI_CONFIG_ID_SWING_CTRL) to adjust the HDMI Output swing.
*/
typedef enum {
	HDMI_SWING_CTRL_P09,                ///< Select HDMI Output Swing Control +9%.
	HDMI_SWING_CTRL_DEFAULT,            ///< Select HDMI Output Swing Control Default Value.
	HDMI_SWING_CTRL_N09,                ///< Select HDMI Output Swing Control -9%.
	HDMI_SWING_CTRL_N18,                ///< Select HDMI Output Swing Control -18%.

	ENUM_DUMMY4WORD(HDMI_SWING_CTRL)
} HDMI_SWING_CTRL;

/**
    HDMI Interrupt identifications

    Some of the definitions are used in HDMI-CEC ISR callback identifications.
*/
typedef enum {
	CEC_RXFRM       = 0x00100000,   ///< CEC Receive a command.
	CEC_RXPOLL      = 0x00200000,   ///< CEC Receive a polling command.
	CEC_TXFRM       = 0x00400000,   ///< CEC Transmit a command successfully.
	CEC_RETRY       = 0x00800000,   ///< CEC Transmit a command fail.
	CEC_AL          = 0x01000000,   ///< CEC Transmit a command fail with arbitration lost condition.
	CEC_RXTIMEOUT   = 0x02000000,   ///< CEC Receive command with timeout error occured.
	CEC_ERROR       = 0x04000000,   ///< CEC Bus error.
	CEC_INTALL      = 0x07F00000,   ///< The bit mask of all the CEC interrupt status.


	OVER_RUN        = 0x00000001,   ///< Reserved for internal usage.
	RSEN            = 0x00000002,   ///< Reserved for internal usage.
	HPD             = 0x00000004,   ///< Reserved for internal usage.
	VSYNC_REC       = 0x00000008,   ///< Reserved for internal usage.
	PIXCLK_STBL     = 0x00000010,   ///< Reserved for internal usage.
	ACR_OVR         = 0x00000020,   ///< Reserved for internal usage.
	CTS_CHG         = 0x00000040,   ///< Reserved for internal usage.
	GCP_CHG_MUTE    = 0x00000080,   ///< Reserved for internal usage.
	HDMI_INTALL     = 0x000000FF,   ///< Reserved for internal usage.
	ENUM_DUMMY4WORD(HDMI_INT_TYPE)
} HDMI_INT_TYPE;



//
//  HDMI Extern API Definitions
//

extern ER       hdmitx_open(void);
extern ER       hdmitx_close(void);
extern BOOL     hdmitx_isOpened(void);

extern void     hdmitx_setConfig(HDMI_CONFIG_ID ConfigID, UINT32 uiConfig);
extern UINT32   hdmitx_getConfig(HDMI_CONFIG_ID ConfigID);

extern void     hdmitx_checkVideoStable(void);
extern BOOL     hdmitx_checkHotplug(void);

extern UINT32   hdmitx_getEdidInfo(void);
extern void     hdmitx_verifyVideoID(HDMI_VIDEOID *VID_Code);
extern void     hdmitx_dumpInfo(void);
extern void     hdmitx_getVideoAbility(UINT32 *pLen, PHDMI_VDOABI pVdoAbility);
extern BOOL     hdmitx_checkDDC(void);


//
//  CEC Extern API Definitions
//

extern ER       hdmicec_open(void);
extern ER       hdmicec_close(void);
extern void     hdmicec_hookCallback(DRV_CB IsrCB);

extern void     hdmicec_setConfig(CEC_CONFIG_ID ConfigID, UINT32 uiConfig);
extern UINT32   hdmicec_getConfig(CEC_CONFIG_ID ConfigID);

extern void     hdmicec_setTxData(UINT32 uiTxDataSize, UINT32 uiDataBufAddr);
extern void     hdmicec_getRxData(UINT32 *puiRxDataSize, UINT32 uiDataBufAddr);

//@}

#endif
