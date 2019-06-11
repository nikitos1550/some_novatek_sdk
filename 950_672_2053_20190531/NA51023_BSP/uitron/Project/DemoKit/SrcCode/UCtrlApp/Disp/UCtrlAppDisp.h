#ifndef _UCTRLAPP_DISP_H
#define _UCTRLAPP_DISP_H
#include "Type.h"


/////////////////////////////////////////////////////////////////////////////////
/**
     @name NVTDISP device id
*/
//@{
#define NVTDISP_DOUT1                  0x00 ///< 1st device
#define NVTDISP_DOUT2                  0x10 ///< 2nd device
#define NVTDISP_DOUT_NUM               2 	///< total device
//@}

/**
     @state param for get/set deivce
*/
//@{
#define NVTDISP_DEVCTRL_MODE             0 ///< get MODE, set MODE
//@}

/////////////////////////////////////////////////////////////////////////////////

/**
     @name NVTDISP layer id
*/
//@{
#define NVTDISP_LYR_OSD1             0x00 ///< OSD 1
#define NVTDISP_LYR_OSD2             0x01 ///< OSD 2
#define NVTDISP_LYR_VDO1             0x02 ///< video 1
#define NVTDISP_LYR_VDO2             0x03 ///< video 2
#define NVTDISP_LYR_ALL              0x0F
//@}

#define NVTDISP_LYR_NUM                 4 //total input layer

/**
    Dal_Diap Layer Buffer Format

    This is used for NVTDISP_setlayerbufferformat to specify the buffer format.
    This feature can be assigned for each display layer with different setting.
*/

typedef enum _NVTDISP_BUFFORMAT {
	NVTDISP_BUFFORMAT_PAL1,                 ///< Buffer uses format 1bit palette.
	NVTDISP_BUFFORMAT_PAL2,                 ///< Buffer uses format 2bit palette.
	NVTDISP_BUFFORMAT_PAL4,                 ///< Buffer uses format 4bit palette.
	NVTDISP_BUFFORMAT_PAL8,                 ///< Buffer uses format 8bit palette.

	NVTDISP_BUFFORMAT_YUV444,               ///< Buffer uses format YUV444.
	NVTDISP_BUFFORMAT_YUV422,               ///< Buffer uses format YUV422.
	NVTDISP_BUFFORMAT_YUV420,               ///< Buffer uses format YUV420.

	NVTDISP_BUFFORMAT_ARGB4565,             ///< Buffer uses format ARGB 4565.
	NVTDISP_BUFFORMAT_ARGB8565,             ///< Buffer uses format ARGB 8565.

	NVTDISP_BUFFORMAT_YUV422PACK,           ///< Buffer uses format YUV422.
	NVTDISP_BUFFORMAT_YUV420PACK,           ///< Buffer uses format YUV420.

	NVTDISP_BUFFORMAT_ARGB8888,             ///< Buffer uses format ARGB 8888.

	NVTDISP_BUFFORMAT_ARGB4444,             ///< Buffer uses format ARGB 4444.
	NVTDISP_BUFFORMAT_ARGB1555,             ///< Buffer uses format ARGB 1555.

	ENUM_DUMMY4WORD(NVTDISP_BUFFORMAT)
} NVTDISP_BUFFORMAT;

typedef struct _DispMemSetting{
    UINT32  addr;
    UINT32  size;
} DispMemSetting;


typedef struct _DispBufAddrSetting{
    UINT32  addr1;
    UINT32  addr2;
    UINT32  addr3;
    UINT32  wait;
} DispBufAddrSetting;


typedef struct _DispBufFmtSetting{
	NVTDISP_BUFFORMAT 	format;
	UINT32 				w;
	UINT32 				h;
	UINT32 				loff;
} DispBufFmtSetting;

typedef struct _DispScaleWinSetting{
	UINT32 	x;
	UINT32 	y;
	UINT32 	w;
	UINT32 	h;
} DispScaleWinSetting;


void UCtrlAppDisp_InstallCmd(void);
#endif //_UCTRLAPP_DISP_H
