/**
    Header file for USB device descriptor

    This file is the header file for USB device descriptor

    @file       usb_desc.h
    @ingroup    mIDrvUSB_Device
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _USBDESC_H
#define _USBDESC_H

/**
    @addtogroup mIDrvUSB_Device
*/
//@{


/**
    Device Descriptor

    @note For pDevDesc of USB_MNG
*/
typedef  struct Device_Descriptor {
	UINT8       bLength;                ///< size of Device Descriptor
	UINT8       bDescriptorType;        ///< Device Dscriptor type
	UINT16      bcdUSB;                 ///< number of USB specifications
	UINT8       bDeviceClass;           ///< class code
	UINT8       bDeviceSubClass;        ///< sub class code
	UINT8       bDeviceProtocol;        ///< protocol code
	UINT8       bMaxPacketSize0;        ///< max packt size of endpoint0
	UINT16      idVendor;               ///< Vendor id
	UINT16      idProduct;              ///< Protocol id
	UINT16      bcdDevice;              ///< Device nmber
	UINT8       iManufacturer;          ///< index of string Desc(maker)
	UINT8       iProduct;               ///< index of string Desc(products)
	UINT8       iSerialNumber;          ///< index of string Desc(serial number)
	UINT8       bNumConfigurations;     ///< number for configration
} USB_DEVICE_DESC;

/**
    Configuration Descriptor

    @note For pConfigDesc/pConfigOthersDesc/pConfigDescHS/pConfigDescFS/pConfigDescFSOther/pConfigDescHSOther of USB_MNG
*/
typedef  struct Configuration_Descriptor {
	UINT8       bLength;                ///< size of Configuration Descriptor
	UINT8       bDescriptorType;        ///< Configuration Descriptor type
	UINT16      wTotalLength;           ///< all length of data
	UINT8       bNumInterfaces;         ///< number of interface
	UINT8       bConfigurationValue;    ///< value of argument
	UINT8       iConfiguration;         ///< index of string Descriptor
	UINT8       bmAttributes;           ///< characteristic of composition
	UINT8       MaxPower;               ///< max power consumption
} USB_CONFIG_DESC;

/**
    String Descriptor

    @note For pStringDesc of USB_MNG
*/
typedef  struct String_Descriptor {
	UINT8       bLength;                ///< size of String Descriptor
	UINT8       bDescriptorType;        ///< String Descriptor type
	UINT8       bString[254];           ///< UNICODE stirng
} USB_STRING_DESC;


/**
    @name   USB device release number

    @note For usb_SetDevDescUSBVerRelNum()
*/
//@{
#define USB_HS_DEV_RELEASE_NUMBER       0x0200  ///< spec. release number
#define USB_FS_DEV_RELEASE_NUMBER       0x0110  ///< spec. release number
//@}


//@}

#endif  //_USBDESC_H

