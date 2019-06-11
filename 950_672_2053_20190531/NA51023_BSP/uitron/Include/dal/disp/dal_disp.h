#ifndef _DAL_DISP_H
#define _DAL_DISP_H
#include "Type.h"
#include "DispDef.h"
#include "top.h"

typedef enum {
    DAL_DISP_PDIR_RGB = 0,   				///< R...G...B...
    DAL_DISP_PDIR_RBG = 1,    				///< R...B...G...
} DAL_DISP_PDIR;

typedef enum {
    DAL_DISP_LCD_R = 0,      				///< Color begin from R
    DAL_DISP_LCD_G = 1,      				///< Color begin from G
    DAL_DISP_LCD_B = 2,       				///< Color begin from B
} DAL_DISP_PORDER;

typedef enum {
    DAL_DISP_ICST_CCIR601    = 0,           ///< ICST type 601
    DAL_DISP_ICST_ITU_BT_709 = 1,           ///< ICST type 709
} DAL_DISP_ICST;

typedef enum {
    DAL_DISP_DITHER_4BITS = 0,   			///< Dithering 4 valid bits.
    DAL_DISP_DITHER_5BITS = 1,   			///< Dithering 5 valid bits.
    DAL_DISP_DITHER_6BITS = 2,   			///< Dithering 6 valid bits.
    DAL_DISP_DITHER_7BITS = 3,   			///< Dithering 7 valid bits.
} DAL_DISP_DITHER;

typedef struct _DAL_DISP_LCD_CFG {
    PINMUX_LCDINIT  LcdMode;                ///< LCD Mode Select: Top.h PINMUX_LCDINIT
    float           fDCLK;                  ///< LCD Target Clock Rate
    unsigned int    uiHSyncTotalPeriod;     ///< Horizontal Total Period
    unsigned int    uiHSyncActivePeriod;    ///< Horizontal Active Width
    unsigned int    uiHSyncBackPorch;       ///< Horizontal Backporth
    unsigned int    uiVSyncTotalPeriod;     ///< Vertical Total Period
    unsigned int    uiVSyncActivePeriod;    ///< Vertical Active Width
    unsigned int    uiVSyncBackPorchOdd;    ///< Vertical Odd Lines Backporch
    unsigned int    uiVSyncBackPorchEven;   ///< Vertical Even Lines Backporch
    unsigned int    uiMaxOutWidth;          ///< Max Output Width
    unsigned int    uiMaxOutHeight;         ///< Max Output Height
    BOOL        bYCbCrFormat;           	///< TRUE for YUV(Ex: YUV/UPS051/UPS052),  FALSE for RGB: (Ex: RGB/RGBD)
    unsigned int    uiHSyncSYNCwidth;       ///< Horizontal Sync pulse Width
    unsigned int    uiVSyncSYNCwidth;       ///< Vertical Sync pulse Width
    ////////////////////////////////////////////////////////////////////////////
    PINMUX_LCD_SEL  pinmux_select_lcd;  	///< LCD Pinmux select : Top.h PINMUX_LCD_SEL
    BOOL        dithering[2];           	///< [0]: Dither Enable.   [1]: Dither FreeRun
    DAL_DISP_PDIR   pdir;                   ///< RGB Pixel Output Direction
    DAL_DISP_PORDER odd;                    ///< LCD Odd Lines pixel Start from R or G or B.
    DAL_DISP_PORDER even;                   ///< LCD Odd Lines pixel Start from R or G or B.
    BOOL        hsinv;                  	///< H-SYNC Invert
    BOOL        vsinv;                  	///< V-SYNC Invert
    BOOL        hvldinv;                	///< H-Valid Invert
    BOOL        vvldinv;                	///< V-Valid Invert
    BOOL        clkinv;                 	///< PIXEL CLOCK Invert
    BOOL        fieldinv;               	///< FIELD Invert

    BOOL        interlace;              	///< Output interlaced format
    BOOL        cex;                    	///< Cb/Cr Exchane
    BOOL        clamp;                  	///< YCC color clamp
    BOOL        tv_powerdown;           	///< Assert TV power down in panel open
    BOOL        YCex;                   	///< Y/C Exchange

    BOOL        HLPF;                   	///< Horizontal lowpass filter
    BOOL        subpixOdd[3];           	///< Sub-Pixel ON/OFF for Odd lines. [0]:R   [1]:G   [2]:B //only for RGB through mode
    BOOL        subpixEven[3];          	///< Sub-Pixel ON/OFF for Even lines.[0]:R   [1]:G   [2]:B //only for RGB through mode
    DAL_DISP_DITHER DitherBits[3];          ///< [0]:R dither bits. [1]:G dither bits. [2]:B dither bits. This is valid only if dithering[0]=TRUE.
    BOOL        clk1_2;                 	///< Clock 1/2

} DAL_DISP_LCD_CFG;

/*
    Structure for standard format of Panel Register Access
*/
typedef struct {
	UINT32  uiAddress;                      ///< Panel Register Address
	UINT32  uiValue;                        ///< Panel Register config value
} DAL_DISP_LCD_CMD;

/**
    Dal_Diap Layer Buffer Format

    This is used for dal_disp_setlayerbufferformat to specify the buffer format.
    This feature can be assigned for each display layer with different setting.
*/

typedef enum _DAL_DISP_BUFFORMAT {
	DAL_DISP_BUFFORMAT_PAL1,                 ///< Buffer uses format 1bit palette.
	DAL_DISP_BUFFORMAT_PAL2,                 ///< Buffer uses format 2bit palette.
	DAL_DISP_BUFFORMAT_PAL4,                 ///< Buffer uses format 4bit palette.
	DAL_DISP_BUFFORMAT_PAL8,                 ///< Buffer uses format 8bit palette.

	DAL_DISP_BUFFORMAT_YUV444,               ///< Buffer uses format YUV444.
	DAL_DISP_BUFFORMAT_YUV422,               ///< Buffer uses format YUV422.
	DAL_DISP_BUFFORMAT_YUV420,               ///< Buffer uses format YUV420.

	DAL_DISP_BUFFORMAT_ARGB4565,             ///< Buffer uses format ARGB 4565.
	DAL_DISP_BUFFORMAT_ARGB8565,             ///< Buffer uses format ARGB 8565.

	DAL_DISP_BUFFORMAT_YUV422PACK,           ///< Buffer uses format YUV422.
	DAL_DISP_BUFFORMAT_YUV420PACK,           ///< Buffer uses format YUV420.

	DAL_DISP_BUFFORMAT_ARGB8888,             ///< Buffer uses format ARGB 8888.

	DAL_DISP_BUFFORMAT_ARGB4444,             ///< Buffer uses format ARGB 4444.
	DAL_DISP_BUFFORMAT_ARGB1555,             ///< Buffer uses format ARGB 1555.

	ENUM_DUMMY4WORD(DAL_DISP_BUFFORMAT)
} DAL_DISP_BUFFORMAT;

/////////////////////////////////////////////////////////////////////////////////
/**
     @name DAL_DISP device id
*/
//@{
#define DAL_DISP_DOUT1                  0x00 ///< 1st device
#define DAL_DISP_DOUT2                  0x10 ///< 2nd device
#define DAL_DISP_DOUT_MASK              0xf0 ///< output device mask
//@}

/**
     @state param for get/set deivce
*/
//@{
#define DAL_DISP_DEVCTRL_MODE             0 ///< get MODE, set MODE
//@}

/////////////////////////////////////////////////////////////////////////////////

/**
     @name DAL_DISP layer id
*/
//@{
#define DAL_DISP_LAYER_OSD1             0x00 ///< OSD 1
#define DAL_DISP_LAYER_OSD2             0x01 ///< OSD 2
#define DAL_DISP_LAYER_VDO1             0x02 ///< video 1
#define DAL_DISP_LAYER_VDO2             0x03 ///< video 2
#define DAL_DISP_LAYER_ALL              0x0F
//@}

#define DAL_DISP_LAYER_NUM                 4 //total input layer

/**
     @state param for get/set layer
*/
//@{
#define DAL_DISP_LAYER_ENABLE           0x00 ///< enable/disable layer, value = 1/0
#define DAL_DISP_LAYER_COMPARE          0x01 ///< color compare,func = DAL_DISP_COMPARE_KEY_NEVER/EQUAL/LESS/GREATER
#define DAL_DISP_LAYER_COLORKEY         0x02 ///< do color key,value = YUV color
#define DAL_DISP_LAYER_BLEND            0x03 ///< blend ctrl,func = DAL_DISP_BLEND_DISABLE/CONSTALPHA/PIXELALPHA/R1
#define DAL_DISP_LAYER_ALPHA            0x04 ///< const alpha,value = 0x00~0xFF (DAL_DISP_BLEND_OPAQUE_000~DAL_DISP_BLEND_OPAQUE_100)
#define DAL_DISP_LAYER_FLIP             0x05 ///< Set Buffer Output Order such as Flip / 180 Rotation
//@}


/**
     @value of DAL_DISP_LAYER_COMPARE
*/
//@{
#define DAL_DISP_COMPARE_KEY_NEVER         0 ///< never key
#define DAL_DISP_COMPARE_KEY_LESS          1 ///< key if pixel color is less then color key
#define DAL_DISP_COMPARE_KEY_EQUAL         2 ///< key if pixel color is equal to color key
#define DAL_DISP_COMPARE_KEY_GREATER       3 ///< key if pixel color is grater then color key
//@}

/**
     @value of DAL_DISP_LAYER_BLEND
*/
//@{
#define DAL_DISP_BLEND_DISABLE          0x00 ///< blending disable
#define DAL_DISP_BLEND_CONSTALPHA       0x01 ///< blending by layer const alpha
#define DAL_DISP_BLEND_PIXELALPHA       0x02 ///< blending by layer pixel alpha
#define DAL_DISP_BLEND_R1               0x03 ///< (reserved)
//@}

/**
     @value of DAL_DISP_LAYER_ALPHA
*/
//@{
#define DAL_DISP_BLEND_OPAQUE_000       0x00 //Output = Under_Layer
#define DAL_DISP_BLEND_OPAQUE_012       0x20 //Output = This_Layer * 12% + Under_Layer * 88%
#define DAL_DISP_BLEND_OPAQUE_025       0x40 //Output = This_Layer * 25% + Under_Layer * 75%
#define DAL_DISP_BLEND_OPAQUE_050       0x80 //Output = This_Layer * 50% + Under_Layer * 50%
#define DAL_DISP_BLEND_OPAQUE_075       0xC0 //Output = This_Layer * 75% + Under_Layer * 25%
#define DAL_DISP_BLEND_OPAQUE_088       0xE0 //Output = This_Layer * 88% + Under_Layer * 12%
#define DAL_DISP_BLEND_OPAQUE_100       0xff //Output = This_Layer
//@}


/**
     @value of DAL_DISP_LAYER_FLIP
*/
//@{
#define DAL_DISP_FLIP_NORMAL       		0x0          ///< Normal output direction
#define DAL_DISP_FLIP_HRZ       		0x1          ///< Horizontal flip
#define DAL_DISP_FLIP_VTC       		0x2          ///< Vertical flip
#define DAL_DISP_FLIP_180       		0x3          ///< Rotate 180 degree
//@}
/**
     config lcd device.

     config lcd device.
     @note pCmdQueue is NULL would run LCD device default command.

     @param[in] lcd_cfg         init lcd parameter.
     @param[in] pCmdQueue       init lcd comand queue.
     @param[in] nCmd           	number of command.
     @return
         - @b E_OK:   Successed
         - @b others:  Failed
*/
extern INT32 dal_disp_lcd_cfg(DAL_DISP_LCD_CFG *lcd_cfg,DAL_DISP_LCD_CMD *pCmdQueue,UINT32 nCmd);

/**
     Initialize device.

     Initialize device.
     @note Nothing

     @param[in] id           device id (ex: DAL_DISP_DOUT1 or DAL_DISP_DOUT2).
     @return
         - @b E_OK:   Successed
         - @b others:  Failed
*/
extern INT32  dal_disp_init(UINT32 id);

/**
     Uninitialize device.

     Uninitialize device.
     @note Nothing

     @param[in] id           device id (ex: DAL_DISP_DOUT1 or DAL_DISP_DOUT2).
     @return
         - @b E_OK:   Successed
         - @b others:  Failed
*/
extern INT32  dal_disp_uninit(UINT32 id);

/**
     Open device.

     Open device.
     @note Nothing

     @param[in] id           device id (ex: DAL_DISP_DOUT1 or DAL_DISP_DOUT2).
     @param[in] disp         device object.
     @param[in] mode         device mode.(ex:DISP_LCDMODE/DISP_TVMODE/DISP_HDMIMODE)
     @return
         - @b E_OK:   Successed
         - @b others:  Failed
*/
extern INT32  dal_disp_open(UINT32 id, UINT32 disp, UINT32 mode);

/**
     Close device.

     Close device.
     @note Nothing

     @param[in] id           Device id (ex: DAL_DISP_DOUT1 or DAL_DISP_DOUT2).
     @return
         - @b E_OK:   Successed
         - @b others:  Failed
*/
extern INT32  dal_disp_close(UINT32 id);

/**
     Set control to device.

     Set control to device.
     @note Nothing

     @param[in] id           device id (ex: DAL_DISP_DOUT1 or DAL_DISP_DOUT2).
     @param[in] param        control state (ex:DAL_DISP_DEVCTRL_XXX)
     @param[in] value        control value
     @return
         - @b E_OK:   Successed
         - @b others:  Failed
*/
extern INT32  dal_disp_setdevice(UINT32 id, UINT32 param, UINT32 value);

/**
     Get control to device.

     Get control to device.
     @note Nothing

     @param[in] id           device id (ex: DAL_DISP_DOUT1 or DAL_DISP_DOUT2).
     @param[in] param        control state (ex:DAL_DISP_DEVCTRL_XXX)
     @return                 control value
*/
extern UINT32 dal_disp_getdevice(UINT32 id, UINT32 param);

/**
     Get device size of by current opened device mode.

     Get device size of by current opened device mode.
     @note Nothing

     @param[in] id           device id (ex: DAL_DISP_DOUT1 or DAL_DISP_DOUT2).
     @return                 device size
*/
extern ISIZE  dal_disp_getdevicesize(UINT32 id);

/**
     Set control to layer.

     Set control to layer.
     @note Nothing

     @param[in] id           device id (ex: DAL_DISP_DOUT1 or DAL_DISP_DOUT2).
     @param[in] layer        layer id (ex: DAL_DISP_LAYER_OSD1/OSD2/VDO1/VDO2).
     @param[in] param        control state (ex:DAL_DISP_LAYER_XXX)
     @param[in] value        control value (depend on control state)
     @return
         - @b E_OK:   Successed
         - @b others:  Failed
*/
extern INT32  dal_disp_setlayer(UINT32 id, UINT32 layer, UINT32 param, UINT32 value);

/**
     Set control to layer.

     Set control to layer.
     @note Nothing

     @param[in] id           device id (ex: DAL_DISP_DOUT1 or DAL_DISP_DOUT2).
     @param[in] layer        layer id (ex: DAL_DISP_LAYER_OSD1/OSD2/VDO1/VDO2).
     @param[in] param        control state (ex:DAL_DISP_LAYER_XXX)
     @param[out] value       control value (depend on control state)
     @return
         - @b E_OK:   Successed
         - @b others:  Failed

*/
extern INT32 dal_disp_getlayer(UINT32 id, UINT32 layer, UINT32 param,UINT32 *value);

/**
     Set destination window of layer.

     Set destination window of display layer.
     @note Destination window is an effective rectangle on device screen.
           User need to call dal_disp_setlayerbufferaddr to present visual effect on screen.

     @param[in] id           device id (ex: DAL_DISP_DOUT1 or DAL_DISP_DOUT2).
     @param[in] layer        layer id (ex: DAL_DISP_LAYER_OSD1/OSD2/VDO1/VDO2).
     @param[in] x            left of window (base on device physical coordinate)
     @param[in] y            top of window (base on device physical coordinate)
     @param[in] w            width of window (base on device physical coordinate)
     @param[in] h            height of window (base on device physical coordinate)
     @return
         - @b E_OK:   Successed
         - @b others:  Failed
*/
extern INT32  dal_disp_setlayerscalewindow(UINT32 id, UINT32 layer, UINT32 x, UINT32 y, UINT32 w, UINT32 h);

/**
     Set buffer format of layer.

     Set buffer format of layer.
     @note User need to call dal_disp_setlayerbufferaddr() to present visual effect on screen.

     @param[in] id           device id (ex: DAL_DISP_DOUT1 or DAL_DISP_DOUT2).
     @param[in] layer        layer id (ex: DAL_DISP_LAYER_OSD1/OSD2/VDO1/VDO2).
     @param[in] format       buffer format(enum of DAL_DISP_BUFFORMAT)
     @param[in] w            buffer width
     @param[in] h            buffer height
     @return
         - @b E_OK:   Successed
         - @b others:  Failed
*/
extern INT32  dal_disp_setlayerbufferformat(UINT32 id, UINT32 layer, DAL_DISP_BUFFORMAT format, UINT32 w, UINT32 h, UINT32 loff);

/**
     Set buffer address of layer.

     Set buffer address of layer.
     @note different format,addr would different

     @param[in] id           device id (ex: DAL_DISP_DOUT1 or DAL_DISP_DOUT2).
     @param[in] layer        layer id (ex: DAL_DISP_LAYER_OSD1/OSD2/VDO1/VDO2).
     @param[in] addr1        addr1 (YUV of Y)
     @param[in] addr2        addr2 (YUV pack of UV)
     @param[in] addr3        addr3 (reserved)
     @param[in] wait         wait vd
     @return
         - @b E_OK:   Successed
         - @b others:  Failed
*/
extern INT32  dal_disp_setlayerbufferaddr(UINT32 id, UINT32 layer, UINT32 addr1, UINT32 addr2,UINT32 addr3, UINT32 wait);

/**
     Wait VD of display device.

     Wait VD of display device.
     @note Nothing.

     @param[in] id           device id (ex: DAL_DISP_DOUT1 or DAL_DISP_DOUT2).
     @return
         - @b E_OK:   Successed
         - @b others:  Failed
*/
extern INT32  dal_disp_waitvd(UINT32 id);



#endif
