/**
    Header file for USB device

    This file is the header file for USB device

    @file       usb_define.h
    @ingroup    mIDrvUSB_Device
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _USBDEFINE_H
#define _USBDEFINE_H

#include "Type.h"

/**
    @addtogroup mIDrvUSB_Device
*/
//@{


/**
    @name   Basic parameters
*/
//@{
#define USB_EP_NUM                      8       ///< max number of endpoint
#define USB_NUM_CONFIG                  1       ///< number of configration
#define USB_NUM_INTERFACE               1       ///< number of interface
#define USB_NUM_STRING_DESC             4       ///< number of string descriptor
#define USB_CONFIG_DESC_TOTAL_SIZE      20      ///< configuration descriptor total size
#define EP0_PACKET_SIZE                 64      ///< EP0 : control packet size
#define USB_WAITBUFEMPTY_LOOPCNT        10000   ///< loop count for waiting buffer empty
//@}


/**
    @name   Block number

    (OBSOLETE)
*/
//@{
#define SINGLE_BLK                      1       ///< Single block
#define DOUBLE_BLK                      2       ///< Double block
#define TRIPLE_BLK                      3       ///< triple block
//@}

/**
    @name   Endpoint/FIFO direction
*/
//@{
#define DIRECTION_IN                    0       ///< direction IN
#define DIRECTION_OUT                   1       ///< direction OUT
//@}

/**
    @name   USB Request Type
*/
//@{
#define USB_DEV_REQ_TYPE_STANDARD       0x00    ///< standard request
#define USB_DEV_REQ_TYPE_CLASS          0x01    ///< class specific request
#define USB_DEV_REQ_TYPE_VENDER         0x02    ///< vendor specific request
#define USB_DEV_REQ_TYPE_RESERVE        0x03    ///< reserved
#define USB_DEV_REQ_TYPE_UNSUPPORTED    0xff    ///< unsupported
//@}


/**
    @name   USB Standard Device Request
*/
//@{
#define USB_GET_STATUS                  0       ///< GetStatus request
#define USB_CLEAR_FEATURE               1       ///< ClearFeature request
#define USB_SET_FEATURE                 3       ///< SetFeature request
#define USB_SET_ADDRESS                 5       ///< SetAddress request
#define USB_GET_DESCRIPTOR              6       ///< GetDescriptor request
#define USB_SET_DESCRIPTOR              7       ///< SetDescriptor request
#define USB_GET_CONFIGURATION           8       ///< GetConfiguratoin request
#define USB_SET_CONFIGURATION           9       ///< SetConfiguratoin request
#define USB_GET_INTERFACE               10      ///< GetInterface request
#define USB_SET_INTERFACE               11      ///< SetInterface request
#define USB_SYNCH_FRAME                 12      ///< SynchFrame request
//@}


/**
    @name   device request
*/
//@{
#define USB_DEVICE_REQUEST_SIZE         8       ///< device request size
#define USB_DEVICE_TO_HOST              0x80    ///< device to host transfer
#define USB_HOST_TO_DEVICE              0x00    ///< host to device transfer
#define USB_DEVICE                      0x00    ///< request to device
#define USB_INTERFACE                   0x01    ///< request to interface
#define USB_ENDPOINT                    0x02    ///< request to endpoint
#define USB_CLASS                       0x20    ///< class request
#define USB_VENDOR                      0x40    ///< vendor request
#define USB_STANDARD                    0x00    ///< standard request
//@}


/**
    @name   descriptor size

    Descriptor size for class library to build descriptor array
*/
//@{
#define USB_DEV_LENGTH                  0x12    ///< device descriptor size
#define USB_CFG_LENGTH                  0x09    ///< config descriptor size
#define USB_IF_LENGTH                   0x09    ///< interface descriptor size
#define USB_BULK_EP_LENGTH              0x07    ///< bulk endpoint descriptor size
#define USB_AUDIO_AC_IF_LENGTH          0x09    ///< audio commander class specific interface descriptor size
#define USB_AUDIO_IT_IF_LENGTH          0x0C    ///< audio input terminal class specific interface descriptor size
#define USB_AUDIO_OT_IF_LENGTH          0x09    ///< audio output terminal class specific interface descriptor size
#define USB_AUDIO_AS_IF_LENGTH          0x07    ///< audio class specific AS interface descriptor size
#define USB_AUDIO_FT_IF_LENGTH          0x0B    ///< audio class specific format type interface descriptor size
#define USB_ISO_EP_LENGTH               0x09    ///< iso endpoint descriptor size
#define USB_AUDIO_CSEP_ISOC_LENGTH      0x07    ///< audio class specific endpoint descriptor size
#define USB_DEV_QUALI_LENGTH            0x0A    ///< device qualifier length */
//@}

/**
    @name   endpoint address
*/
//@{
#define USB_EP_IN_ADDRESS               0x80    ///< IN endpoint address
#define USB_EP_OUT_ADDRESS              0x00    ///< OUT endpoint address
#define USB_EP_EP1I_ADDRESS             (USB_EP_IN_ADDRESS  | 0x01) ///< IN EP1 address
#define USB_EP_EP1O_ADDRESS             (USB_EP_OUT_ADDRESS | 0x01) ///< OUT EP1 address
#define USB_EP_EP2I_ADDRESS             (USB_EP_IN_ADDRESS  | 0x02) ///< IN EP2 address
#define USB_EP_EP2O_ADDRESS             (USB_EP_OUT_ADDRESS | 0x02) ///< OUT EP2 address
#define USB_EP_EP3I_ADDRESS             (USB_EP_IN_ADDRESS  | 0x03) ///< IN EP3 address
#define USB_EP_EP3O_ADDRESS             (USB_EP_OUT_ADDRESS | 0x03) ///< OUT EP3 address
#define USB_EP_EP4I_ADDRESS             (USB_EP_IN_ADDRESS  | 0x04) ///< IN EP4 address
#define USB_EP_EP4O_ADDRESS             (USB_EP_OUT_ADDRESS | 0x04) ///< OUT EP4 address
#define USB_EP_EP5I_ADDRESS             (USB_EP_IN_ADDRESS  | 0x05) ///< IN EP5 address
#define USB_EP_EP5O_ADDRESS             (USB_EP_OUT_ADDRESS | 0x05) ///< OUT EP5 address
#define USB_EP_EP6I_ADDRESS             (USB_EP_IN_ADDRESS  | 0x06) ///< IN EP6 address
#define USB_EP_EP6O_ADDRESS             (USB_EP_OUT_ADDRESS | 0x06) ///< OUT EP6 address
#define USB_EP_EP7I_ADDRESS             (USB_EP_IN_ADDRESS  | 0x07) ///< IN EP7 address
#define USB_EP_EP7O_ADDRESS             (USB_EP_OUT_ADDRESS | 0x07) ///< OUT EP7 address
#define USB_EP_EP8I_ADDRESS             (USB_EP_IN_ADDRESS  | 0x08) ///< IN EP8 address
#define USB_EP_EP8O_ADDRESS             (USB_EP_OUT_ADDRESS | 0x08) ///< OUT EP8 address
#define USB_EP_EP9I_ADDRESS             (USB_EP_IN_ADDRESS  | 0x09) ///< IN EP9 address
#define USB_EP_EP9O_ADDRESS             (USB_EP_OUT_ADDRESS | 0x09) ///< OUT EP9 address
#define USB_EP_EP10I_ADDRESS            (USB_EP_IN_ADDRESS  | 0x0A) ///< IN EP10 address
#define USB_EP_EP10O_ADDRESS            (USB_EP_OUT_ADDRESS | 0x0A) ///< OUT EP10 address
#define USB_EP_EP11I_ADDRESS            (USB_EP_IN_ADDRESS  | 0x0B) ///< IN EP11 address
#define USB_EP_EP11O_ADDRESS            (USB_EP_OUT_ADDRESS | 0x0B) ///< OUT EP11 address
#define USB_EP_EP12I_ADDRESS            (USB_EP_IN_ADDRESS  | 0x0C) ///< IN EP12 address
#define USB_EP_EP12O_ADDRESS            (USB_EP_OUT_ADDRESS | 0x0C) ///< OUT EP12 address
#define USB_EP_EP13I_ADDRESS            (USB_EP_IN_ADDRESS  | 0x0D) ///< IN EP13 address
#define USB_EP_EP13O_ADDRESS            (USB_EP_OUT_ADDRESS | 0x0D) ///< OUT EP13 address
#define USB_EP_EP14I_ADDRESS            (USB_EP_IN_ADDRESS  | 0x0E) ///< IN EP14 address
#define USB_EP_EP14O_ADDRESS            (USB_EP_OUT_ADDRESS | 0x0E) ///< OUT EP14 address
#define USB_EP_EP15I_ADDRESS            (USB_EP_IN_ADDRESS  | 0x0F) ///< IN EP15 address
#define USB_EP_EP15O_ADDRESS            (USB_EP_OUT_ADDRESS | 0x0F) ///< OUT EP15 address
#define USB_EP_ADDRESS_MASK             (0x0F)                      ///< EP mask to get endpoint number (exclude IN/OUT)
//@}

/**
    @name   endpoint attribute
*/
//@{
#define USB_EP_ATR_CONTROL              0x00    ///< transfer mode : control
#define USB_EP_ATR_ISO                  0x01    ///< transfer mode : isochronous
#define USB_EP_ATR_BULK                 0x02    ///< transfer mode : bulk
#define USB_EP_ATR_INTERRUPT            0x03    ///< transfer mode : interrupt
#define USB_EP_ATR_ASYNC                0x04    ///< synchornization type : asynchornization
#define USB_EP_ATR_ADAPTIVE             0x08    ///< synchornization type : adaptive
#define USB_EP_ATR_SYNC                 0x0C    ///< synchornization type : synchornization
//@}

/**
    @name   bInterval value
*/
//@{
#define USB_EP_CONTROL_INTERVAL                 0x00    ///< control interval
#define USB_EP_BULK_INTERVAL                    0x00    ///< bulk interval
#define USB_EP_INT_INTERVAL                     0x01    ///< interrupt interval
#define USB_EP_ISO_INTERVAL                     0x01    ///< isochronous interval
#define USB_EP_INT_CFGA_IF0ALT0_INTERVAL        0x01    ///< interrupt CFGA_IF0ALT0 interval
#define USB_EP_INT_CFGA_IF0ALT1_INTERVAL        0x02    ///< interrupt CFGA_IF0ALT1 interval
#define USB_EP_INT_CFGA_IF0ALT2_INTERVAL        0x03    ///< interrupt CFGA_IF0ALT2 interval
#define USB_EP_INT_CFGA_IF0ALT3_INTERVAL        0x04    ///< interrupt CFGA_IF0ALT3 interval
#define USB_EP_INT_CFGB_IF1ALT0_INTERVAL        0x05    ///< interrupt CFGA_IF1ALT0 interval
//@}

/**
    @name   USB Feature Select
*/
//@{
#define USB_DEVICE_REMOTE_WAKEUP                1       ///< remote wake up
#define USB_ENDPOINT_STALL                      0       ///< endpoint stall
//@}

/**
    @name   USB descriptor type
*/
//@{
#define USB_DEVICE_DESCRIPTOR_TYPE              1       ///< device descriptor
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       2       ///< configuraton descriptor
#define USB_STRING_DESCRIPTOR_TYPE              3       ///< string descriptor
#define USB_INTERFACE_DESCRIPTOR_TYPE           4       ///< interface descriptor
#define USB_ENDPOINT_DESCRIPTOR_TYPE            5       ///< endpoint descriptor
#define USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE    6       ///< Device_Qualifier descriptor
#define USB_OTHER_SPEED_CONFIGURATION           7       ///< Other_Speed_Configuration
#define USB_BOS_DESCRIPTOR_TYPE                 15      ///< Binary Device Object Store (BOS) descriptor
//@}

/**
    @name   config descriptor definitions
*/
//@{
#define USB_DEVDESC_ATB_BUS_POWER               0x80    ///< bus power
#define USB_DEVDESC_ATB_SELF_POWER              0x40    ///< self power
#define USB_DEVDESC_ATB_RMT_WAKEUP              0x20    ///< remote wake up
//@}

/**
    @name   USB request-bmRequestType : RECIPIENT type
*/
//@{
#define USB_RECIPIENT                           0x1F    ///< recipient mask
#define USB_RECIPIENT_DEVICE                    0x00    ///< recipient device
#define USB_RECIPIENT_INTERFACE                 0x01    ///< recipient interface
#define USB_RECIPIENT_ENDPOINT                  0x02    ///< recipient endpoint
//@}

/**
    @name   USB defined Feature select
*/
//@{
#define USB_FEATURE_ENDPOINT_STALL              0x0000  ///< feature select stall
#define USB_FEATURE_REMOTE_WAKEUP               0x0001  ///< feature select remote wakeup
#define USB_FEATURE_POWER_D0                    0x0002  ///< feature select POWER_D0
#define USB_FEATURE_POWER_D1                    0x0003  ///< feature select POWER_D1
#define USB_FEATURE_POWER_D2                    0x0004  ///< feature select POWER_D2
#define USB_FEATURE_POWER_D3                    0x0005  ///< feature select POWER_D3
//@}

/**
    @name   USB request and masks
*/
//@{
#define USB_REQUEST_TYPE_MASK                   0x60    ///< request type mask
#define USB_STANDARD_REQUEST                    0x00    ///< standard request
#define USB_CLASS_REQUEST                       0x20    ///< class request
#define USB_VENDOR_REQUEST                      0x40    ///< vendor request
#define USB_VENDOR_D2H_MASK                     0x80    ///< vendor d2h mask
#define USB_REQUEST_MASK                        0x0F    ///< request mask
#define DEVICE_ADDRESS_MASK                     0x7F    ///< device address mask
#define USB_ENDPOINT_DIRECTION_MASK             0x80    ///< endpoint direction mask
//@}

/**
    @name   Endpoint number
*/
//@{
#define EP0                             0x00    ///< Endpoint 0
#define EP1                             0x01    ///< Endpoint 1
#define EP2                             0x02    ///< Endpoint 2
#define EP3                             0x03    ///< Endpoint 3
#define EP4                             0x04    ///< Endpoint 4
#define EP5                             0x05    ///< Endpoint 5
#define EP6                             0x06    ///< Endpoint 6
#define EP7                             0x07    ///< Endpoint 7
#define EP8                             0x08    ///< Endpoint 8
//@}

/**
    @name   FIFO number

    (OBSOLETE)
*/
//@{
#define FIFO0                           0x0     ///< FIFO 0
#define FIFO1                           0x1     ///< FIFO 1
#define FIFO2                           0x2     ///< FIFO 2
#define FIFO3                           0x3     ///< FIFO 3
//@}


/**
    USB type

    @note For USBType of USB_MNG
*/
typedef enum {
	USB_NONE,                                   ///< USB type not set
	USB_MSD,                                    ///< USB type is MSDC
	USB_SIDC,                                   ///< USB type is SIDC
	USB_PCC,                                    ///< USB type is PCC
	USB_TEST,                                   ///< USB type is test class

	ENUM_DUMMY4WORD(USB_DEV_TYPE)
} USB_DEV_TYPE;


/**
    USB event definition

    @note For fpEvent_CB() of USB_MNG
*/
typedef enum {
	USB_EVENT_NONE,
	USB_EVENT_IN0,                              ///< USB EP0 TX
	USB_EVENT_OUT0,                             ///< USB EP0 RX
	USB_EVENT_SETUP,                            ///< USB EP0 SETUP
	USB_EVENT_BULKIN1,                          ///< USB EP1 BULK TX
	USB_EVENT_FIFO0_IN = USB_EVENT_BULKIN1,     ///< FIFO0 IN event
	USB_EVENT_EP1_TX = USB_EVENT_BULKIN1,       ///< USB (IN)EP1 TX
	USB_EVENT_BULKOUT2,                         ///< USB EP2 BULK RX
	USB_EVENT_FIFO0_OUT = USB_EVENT_BULKOUT2,   ///< FIFO0 OUT event
	USB_EVENT_EP2_RX = USB_EVENT_BULKOUT2,      ///< USB (OUT)EP2 RX
	USB_EVENT_INT3,                             ///< USB EP3 INT TX
	USB_EVENT_FIFO2_IN = USB_EVENT_INT3,        ///< FIFO2 IN event
	USB_EVENT_EP3_TX = USB_EVENT_INT3,          ///< USB (IN)EP3 TX
	USB_EVENT_ISOIN4,                           ///< USB EP4 ISO IN
	USB_EVENT_EP4_RX = USB_EVENT_ISOIN4,        ///< USB (OUT)EP4 RX
	USB_EVENT_EP1TX0BYTE,                       ///< USB EP1 TX 0BYTE
	USB_EVENT_EP3TX0BYTE,                       ///< USB EP3 TX 0BYTE
	USB_EVENT_CLRFEATURE,                       ///< USB Clear Feature
	USB_EVENT_RESET,                            ///< USB Reset
	USB_EVENT_SETADDRESSCMD,                    ///< USB set address from host
	USB_EVENT_CONNECT,                          ///< USB connect to host
	USB_EVENT_DISCONNECT,                       ///< USB disconnect from host
	USB_EVENT_SET_CONFIG,                       ///< USB set configuration
	USB_EVENT_SUSPEND,                          ///< USB suspend event
	USB_EVENT_RESUME,                           ///< USB resume event
	USB_EVENT_EP1_RX,                           ///< USB (OUT)EP1 RX
	USB_EVENT_EP2_TX,                           ///< USB (IN)EP2 TX
	USB_EVENT_EP3_RX,                           ///< USB (OUT)EP3 RX
	USB_EVENT_EP4_TX,                           ///< USB (IN)EP4 TX

	USB_EVENT_EP5_TX,                           ///< USB (IN)EP5 TX
	USB_EVENT_EP6_TX,                           ///< USB (IN)EP6 TX
	USB_EVENT_EP7_TX,                           ///< USB (IN)EP6 TX
	USB_EVENT_EP8_TX,                           ///< USB (IN)EP6 TX
	USB_EVENT_EP9_TX,                           ///< USB (IN)EP6 TX
	USB_EVENT_EP10_TX,                          ///< USB (IN)EP6 TX
	USB_EVENT_EP11_TX,                          ///< USB (IN)EP6 TX
	USB_EVENT_EP12_TX,                          ///< USB (IN)EP6 TX
	USB_EVENT_EP13_TX,                          ///< USB (IN)EP6 TX
	USB_EVENT_EP14_TX,                          ///< USB (IN)EP6 TX
	USB_EVENT_EP15_TX,                          ///< USB (IN)EP6 TX

	USB_EVENT_EP5_RX,                           ///< USB (OUT)EP3 RX
	USB_EVENT_EP6_RX,                           ///< USB (OUT)EP3 RX
	USB_EVENT_EP7_RX,                           ///< USB (OUT)EP3 RX
	USB_EVENT_EP8_RX,                           ///< USB (OUT)EP3 RX
	USB_EVENT_EP9_RX,                           ///< USB (OUT)EP3 RX
	USB_EVENT_EP10_RX,                          ///< USB (OUT)EP3 RX
	USB_EVENT_EP11_RX,                          ///< USB (OUT)EP3 RX
	USB_EVENT_EP12_RX,                          ///< USB (OUT)EP3 RX
	USB_EVENT_EP13_RX,                          ///< USB (OUT)EP3 RX
	USB_EVENT_EP14_RX,                          ///< USB (OUT)EP3 RX
	USB_EVENT_EP15_RX,                          ///< USB (OUT)EP3 RX

	USB_EVENT_FIFO1_IN = 0x20000000,            ///< FIFO1 IN
	USB_EVENT_FIFO3_IN,                         ///< FIFO3 IN
	USB_EVENT_FIFO1_OUT,                        ///< FIFO1 OUT
	USB_EVENT_FIFO2_OUT,                        ///< FIFO2 OUT
	USB_EVENT_FIFO3_OUT,                        ///< FIFO3 OUT

	ENUM_DUMMY4WORD(USB_EVENT)
} USB_EVENT;

//@}

#endif /* _USBDEFINE_H */

