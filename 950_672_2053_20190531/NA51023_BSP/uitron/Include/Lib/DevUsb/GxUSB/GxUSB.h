/**
    GxUSB module

    Detect USB whether it's plugged in or not.

    @file       GxUSB.h
    @ingroup    mIGxUSB

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _GXUSB_H
#define _GXUSB_H

#include "GxCommon.h"

/**
    @addtogroup mIGxUSB
*/
//@{


////////////////////////////////////////////////////////////////////////////////
//SxUSB
//USB Driver                Detect USB change

/**
     @name GxUSB callback event
*/
//@{
#define USB_CB_PLUG     0   ///< plugged-in event
#define USB_CB_UNPLUG   1   ///< unplugged event
#define USB_CB_CHARGE   2   ///< charging current event
//@}


/**
     @name GxUSB charging current
*/
//@{
#define USB_CHARGE_CURRENT_2P5MA        0///< currently can draw up to 2.5mA
#define USB_CHARGE_CURRENT_100MA      100///< currently can draw up to 100mA
#define USB_CHARGE_CURRENT_500MA      500///< currently can draw up to 500mA
#define USB_CHARGE_CURRENT_1500MA    1500///< currently can draw up to 1500mA
//@}

/**
    The prototype of callback function.
*/
typedef void (*FPUSBDETCB)(BOOL bPlugIn);

/**
    GxUSB Init.

    @param[in] DevID       Device id
    @param[in] bPowerOnSrc Set TRUE if USB has been plugged since system booting up.
*/
extern void GxUSB_Init(UINT32 DevID, BOOL bPowerOnSrc);

/**
    GxUSB Exif.
*/
extern void GxUSB_Exit(void);


/**
    USB detecting function
    @note This API should be registered to SxTimer for auto detecting USB connection.
*/
extern void GxUSB_DetConnect(void);

/**
    Get USB current status which is plugged in or unplugged.
    @return USB status.
*/
extern BOOL GxUSB_GetIsUSBPlug(void);

/**
    Register callback function for GxUSB.
    @param[in] fpUSBDetCB Callback function for USB detection.
*/
extern void GxUSB_RegCB(GX_CALLBACK_PTR fpUSBDetCB);


/**
    Get USB connection type.
    @return USB connection type, the result is updated by GxUSB_DetConnect() or GxUSB_UpdateConnectType().
*/
extern UINT32 GxUSB_GetConnectType(void);

/**
    Update USB connection type.
    Force to update connection type immediately.
*/
extern void GxUSB_UpdateConnectType(void);

/**
    Config charger type.
    @param[in] bIsStandardCharger Set TRUE for supporting standard chargers only, FLASE for non-standard ones. Default is TRUE.
*/
extern void GxUSB_SetChargerType(BOOL bIsStandardCharger);
//@}
#endif //_GXUSB_H

