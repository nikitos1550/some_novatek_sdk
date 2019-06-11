#ifndef _USBOTG987_H_
#define _USBOTG987_H_

//#include    "constant.h"

typedef struct {
	UINT8       ucEPId;
	UINT8       ucFIFOId;
	UINT8       ucEPDir;
	UINT8       ucFIFODir;
	UINT8       ucEPAttr;
	UINT8       ucBlknums;
	UINT8       ucBlksize;
	UINT16      usEPMaxLength;
} USB_DEVICE_FIFOEPXCFG_INFO;

//JerryKo
#define USBHost_EP_NUM          8
#define USBHost_FIFO_NUM        4
// Block toggle number define
#define SINGLE_BLK          1
#define DOUBLE_BLK          2
#define TRIBLE_BLK          3
// FIFO direction define
#define USB_FIFO_OUT           0
#define USB_FIFO_IN            1
#define USB_FIFO_BIDIR        2
// Endpoint direction define
#define USB_EP_IN           0
#define USB_EP_OUT          1
// Block Size define
#define BLK512BYTE          1
#define BLK1024BYTE         2
//#define BLK64BYTE         1
//#define BLK128BYTE        2

// FIFO direction define
#define USBOTG_DIR_IN       0
#define USBOTG_DIR_OUT        1
// FIFO number define
#define USBOTG_FIFO0        0x00
#define USBOTG_FIFO1        0x01
#define USBOTG_FIFO2        0x02
#define USBOTG_FIFO3        0x03
#define USBOTG_CXFIFO        0x04

// Endpoint ID define
#define USBOTG_EP1          0x01
#define USBOTG_EP2          0x02
#define USBOTG_EP3          0x03
#define USBOTG_EP4          0x04
#define USBOTG_EP5          0x05
#define USBOTG_EP6          0x06
#define USBOTG_EP7          0x07
#define USBOTG_EP8          0x08

//---------------------------------------------------------------------
//    Set/Get all host register information
//---------------------------------------------------------------------
// Host Controller Capability Registers
#define REG_HC_CAPLENG                  1
//#define REG_HC_VERSION                2
#define REG_HC_HCSPARAMS                3
#define REG_HC_HCCPARAMS                4
#define REG_HC_HCSPPORTROUTE            5   // do not support
// Host Controller Operational Registers
#define REG_HC_USBCMMD                  6
#define REG_HC_USBSTS                   7
#define REG_HC_USBINTR                  8
#define REG_HC_FRAME_INDEX              9
#define REG_HC_CTRLDSSEGMENT            10  // do not support
#define REG_HC_PERD_BASEADDR            11
#define REG_HC_CURR_LISTADDR            12
#define REG_HC_CONFIGFLAG               13
#define REG_HC_PORTSTATUS               14

#define REG_HC_PHYSUSP                  15
#define REG_HC_EOF1TIME                 16
#define REG_HC_EOF2TIME                 17
#define REG_HC_ASYNCSCH_SLEEPTIME       18
/*
#define REG_HC_PORTNUMS                 43
#define REG_HC_INT_THRES                44
#define REG_HC_PARKCNTS                 45
#define REG_HC_FRAMELIST_SIZE           46
#define REG_HC_LINESTATUS               21
#define REG_HC_PORTTSTMODE              22
#define REG_HC_PROG_FRAMELIST           24
#define REG_HC_ASYNC_PARK               25
#define REG_HC_USBCMMD_SETBIT           30
#define REG_HC_USBCMMD_CLRBIT           31
#define REG_HC_PORTCTRL_SETBIT          32
#define REG_HC_PORTCTRL_CLRBIT          33
*/
//---------------------------------------------------------------------
//    Set/Get all otg register information
//---------------------------------------------------------------------
#define REG_OTG_CTRL_SETBIT             1
#define REG_OTG_CTRL_CLRBIT             2
#define REG_OTG_STATUS                  3
#define REG_OTG_CURR_ROLE               4
#define REG_OTG_CURR_ID                 5
#define REG_OTG_HOST_SPEED              6
//---------------------------------------------------------------------
//    Set/Get all device register information
//---------------------------------------------------------------------
#define REG_DEV_MAINCTRL_SETBIT         1
#define REG_DEV_MAINCTRL_CLRBIT         2
#define REG_DEV_MAINCTRL                3
#define REG_DEV_ADDR                    4
#define REG_DEV_AFTER_CFG               5
#define REG_DEV_TSTREG_SETBIT           6
#define REG_DEV_TSTREG_CLRBIT           7
#define REG_DEV_SOFFRAME_NUMS           8
#define REG_DEV_SOFMFRAME_NUMS          9
#define REG_DEV_SOFMARK_TIMER           10
#define REG_DEV_PHY_TSTSEL              11
#define REG_DEV_PHY_UNPLUG              12
#define REG_DEV_VENDOR_CTRL             13
#define REG_DEV_VENDOR_STATUS           14
#define REG_DEV_IDLE_CNT                15

#define REG_DEV_CXFIFO_STATUS           16
#define REG_DEV_CXFIFO_BYTECNT          17
#define REG_DEV_CXFIFO_EMPTY            18
#define REG_DEV_CXFIFO_SETBIT           19
#define REG_DEV_CXFIFO_CLRBIT           20

#define REG_DEV_RX0BYTE                 21
#define REG_DEV_RX0BYTE_CLRBIT          22
#define REG_DEV_TX0BYTE                 23
#define REG_DEV_TX0BYTE_CLRBIT          24
#define REG_DEV_ISOSEQERR               25
#define REG_DEV_ISOSEQERR_CLRBIT        26
#define REG_DEV_ISOSEQABT               27
#define REG_DEV_ISOSEQABT_CLRBIT        28

#define REG_DEV_DMA_CFG                 30
#define REG_DEV_DMA_ADDR                31
#define REG_DEV_DMA_TARGET_FIFO         32
#define REG_DEV_DMA_START               33
#define REG_DEV_DMA_ABORT               34
#define REG_DEV_DMA_LENGTH              35
#define REG_DEV_DMA_DIR                 36
#define REG_DEV_DMA_SETUPCMD            37
#define REG_DEV_DMA_REMAIN_LENGTH       38
//---------------------------------------------------------------------
//    Set/Get all device FIFO register information
//---------------------------------------------------------------------
#define REG_FIFO_MAP                    1
#define REG_FIFO_MAP_EPNO               2
#define REG_FIFO_MAP_DIR                3
#define REG_FIFO_MAP_CLRALL             4
#define REG_FIFO_CFG                    5
#define REG_FIFO_CFG_CLRALL             6
#define REG_FIFO_RESET                  7
#define REG_FIFO_CLEAR                  8
#define REG_FIFO_BYTECNT                9
//---------------------------------------------------------------------
//    Set/Get all device EP register information
//---------------------------------------------------------------------
#define REG_EP_MAP                      1
#define REG_EP_MAP_CLRALL               2
#define REG_EP_STALL                    3
#define REG_EP_RSTTOGGLE                4
#define REG_EP_MAXPACKET_SIZE           5
#define REG_EP_HIGHBAND                 6
#define REG_EP_TX0BYTE                  7
//---------------------------------------------------------------------
//  0x10    HC USB Command Register
//---------------------------------------------------------------------
//#define USBHC_CMD_RUN                       0x00000001      // Run/Stop. When set to a "1", the host controller proceeds with execution of the schedule
//#define USBHC_CMD_RESET                     0x00000002      // Host controller reset. This control bit is used by the software to reset the host controller
//#define USBHC_CMD_FRLSIZE_1024              0               // 00: 1024 elements (4096 bytes, default value)
//#define USBHC_CMD_FRLSIZE_512               1               // 01: 512 elements (2048 bytes)
//#define USBHC_CMD_FRLSIZE_256               2               // 10: 256 elements (1024 bytes)
//#define USBHC_CMD_PSCH_EN                   0x00000010      // Periodic schedule enable. This bit controls whether the host controller skips the periodic schedule processing
//#define USBHC_CMD_ASCH_EN                   0x00000020      // Asynchronous schedule enable. This bit controls whether the host controller skips the asynchronous schedule processing
//#define USBHC_CMD_INT_OAAD                  0x00000040      // Interrupt on asynchronous advance doorbell. This bit is used as a doorbell by the software to instruct the host controller to issue an interrupt the next time it advances the asynchronous schedule
//#define USBHC_CMD_ASYNC_PK_EN               0x00000800      // Asynchronous schedule park mode enable. Software uses this bit to enable or disable the park mode. When this bit is set to "1", the park mode is enabled
//---------------------------------------------------------------------
//  0x14~0x18    HC USB Interrupt Status\Enable Register
//---------------------------------------------------------------------
//#define USBHC_INT_USBCOMPLT                 0x00000001      // The host controller sets this bit to "1" upon completion of an USB transaction
//#define USBHC_INT_USBERR                    0x00000002      // The host controller sets this bit to "1" when completion of an USB transaction results in an error
//#define USBHC_INT_PO_CHG_DET                0x00000004      // The host controller sets this bit to "1" when any port has a change bit transition from "0" to "1"
//#define USBHC_INT_FRL_ROL                   0x00000008      // The host controller sets this bit to "1" when the frame list index rolls over from its maximum value to zero
//#define USBHC_INT_H_SYSERR                  0x00000010      // The host controller sets this bit to "1" when a serious error occurs during host system access involving the host controller module
//#define USBHC_INT_OAA                       0x00000020      // This status bit indicates the assertion of the interrupt on async advance doorbell
//#define USBHC_STA_HALTED                    0x00001000      // This bit is "0" whenever the run/stop bit is "1". The host controller sets this bit to "1" after it has stopped executing as a result of the run/stop bit being set to 0
//#define USBHC_STA_RECLAMATION               0x00002000      // This is a read only status bit, which is used to detect an empty asynchronous schedule
//#define USBHC_STA_PSCH                      0x00004000      // Periodic schedule status. This bit reports the actual status of the periodic schedule
//#define USBHC_STA_ASCH                      0x00008000      // Asynchronous schedule status. This bit reports the actual status of the asynchronous schedule
//---------------------------------------------------------------------
//  0x30    HC Port Status and Control Register
//---------------------------------------------------------------------
/*
#define USBHC_PORTSTS_CONNECT               0x00000001      // Current connect status
#define USBHC_PORTSTS_CONNCHG               0x00000002      // Connect status change
#define USBHC_PORTSTS_POEN                  0x00000004      // Port enable/disable
#define USBHC_PORTSTS_POENCHG               0x00000008      // Port enable/disable change
#define USBHC_PORTSTS_PO_RESM               0x00000040      // Force port resume
#define USBHC_PORTSTS_PO_SUSP               0x00000080      // Fort suspend
#define USBHC_PORTSTS_PO_RESET              0x00000100      // port reset
#define USBHC_PORTTST_DISABLE               0               // 0000    Test mode is not enable
#define USBHC_PORTTST_J_STA                 1               // 0001    Test J-state
#define USBHC_PORTTST_K_STA                 2               // 0010    Test K-sate
#define USBHC_PORTTST_SE0_NAK               3               // 0011    Test SE0_NAK
#define USBHC_PORTTST_PACKET                4               // 0100    Test Packet
#define USBHC_PORTTST_ENABLE                5               // 0101    Test FORCE_ENABLE
#define USBHC_PORTSTS_HC_TST_PKDONE         0x00100000      // HC_TST_PKDONE
*/
//---------------------------------------------------------------------
//    0x40    HC Misc. Register
//---------------------------------------------------------------------
#define USBHC_HOSTPHY_SUSP                  0x00000040      // Host transceiver suspend mode
//---------------------------------------------------------------------
//    0x80    OTG Control/Status Register
//---------------------------------------------------------------------
#define USBOTG_CTRL_B_BUS_REQ               0x00000001      // B-device bus request
#define USBOTG_CTRL_B_HNP_EN                0x00000002      // Inform B-device that it has been enabled to perform HNP
#define USBOTG_CTRL_B_DSCHRG_VBUS           0x00000004      // B-device discharge Vbus
#define USBOTG_CTRL_A_BUS_REQ               0x00000010      // A-device bus request
#define USBOTG_CTRL_A_BUS_DROP              0x00000020      // A-device bus drop
#define USBOTG_CTRL_A_SET_B_HNP_EN          0x00000040      // Indicates to A-device that the HNP function of B-device had been enabled
#define USBOTG_CTRL_A_SRP_DET_EN            0x00000080      // A-device SRP detection enable
#define USBOTG_CTRL_A_SRP_RESP_MASK         0x00000100      // SRP response type
#define USBOTG_CTRL_A_SRP_RESP_VBUS         0x00000000      // 0: A-device responds to Vbus pulsing
#define USBOTG_CTRL_A_SRP_RESP_DATALINE     0x00000100      // 1: A-device responds to data-line pulsing
#define USBOTG_CTRL_ID_FLTSEL_MASK          0x00000200      // Select a timer to filter out noise of ID from UTMI+
#define USBOTG_CTRL_ID_FLTSEL_3MS           0x00000000      // 0: approximated to 3ms
#define USBOTG_CTRL_ID_FLTSEL_4MS           0x00000200      // 1: approximated to 4ms
#define USBOTG_CTRL_Vbus_FLTSEL_MASK        0x00000400      // Select a timer to filter out noise of Vbus_VLD from UTMI+
#define USBOTG_CTRL_Vbus_FLTSEL_135S        0x00000000      // 0: approximated to 135 s
#define USBOTG_CTRL_Vbus_FLTSEL_472S        0x00000400      // 1: approximated to 472 s
#define USBOTG_CTRL_HDISCON_FLTSEL_MASK     0x00000800      // Select a timer to filter out noise of HDISCON from UTMI+
#define USBOTG_CTRL_HDISCON_FLTSEL_135S     0x00000000      // 0: approximated to 135 s
#define USBOTG_CTRL_HDISCON_FLTSEL_270S     0x00000800      // 1: approximated to 270 s

//#define USBOTG_CTRL_FORCE_FS              0x00001000      // do not support
//#define USBOTG_CTRL_FORCE_HS              0x00004000      // do not support
//#define USBOTG_CTRL_PHY_RESET             0x00008000      // do not support

#define USBOTG_STAT_B_SESS_END              0x00010000      // B-device session end
#define USBOTG_STAT_B_SESS_VLD              0x00020000      // B-device session valid
#define USBOTG_STAT_A_SESS_VLD              0x00040000      // A-device session valid
#define USBOTG_STAT_A_VBUS_VLD              0x00080000      // A-device Vbus valid
#define USBOTG_STAT_CROLE_HOST              0               // 0: host
#define USBOTG_STAT_CROLE_DEV               1               // 1: device
#define USBOTG_STAT_ID_A_DEV                0               // 0: A-device
#define USBOTG_STAT_ID_B_DEV                1               // 1: B-device
#define USBOTG_STAT_HOST_SPD_FS             0               // 00: FS
#define USBOTG_STAT_HOST_SPD_LS             1               // 01: LS
#define USBOTG_STAT_HOST_SPD_HS             2               // 10: HS

//#define USBOTG_CTRL_FORCE_VBUSI             0x40000000      // Force VBUSI
//#define USBOTG_CTRL_FORCE_ID                0x80000000      // Force ID PIN (0: host, 1: device)
#define USBOTG_CTRL_EOF_LEGACY              0x80000000      // EOF legacy behavior(0: patched behavior, 1: legacy behavior)
//---------------------------------------------------------------------
//  0x84~0x88    OTG Interrupt status\enable Register
//---------------------------------------------------------------------
#define USBOTG_INT_B_SRP_DN                 0x00000001
#define USBOTG_INT_A_SRP_DET                0x00000010
#define USBOTG_INT_A_VBUS_ERR               0x00000020
#define USBOTG_INT_B_SESS_END               0x00000040
#define USBOTG_INT_RLCHG                    0x00000100
#define USBOTG_INT_IDCHG                    0x00000200
#define USBOTG_INT_OVC                      0x00000400
#define USBOTG_INT_BPLGRMV                  0x00000800
#define USBOTG_INT_APLGRMV                  0x00001000
//---------------------------------------------------------------------
//  0xC0~0xC4    Global Mask of HC/OTG/DEV Interrupt Mask\Status
//---------------------------------------------------------------------
//#define USB_INTTYP_GLOBAL                 0x00000001

#define USB_INTTYP_DEVG                     0x00000001
#define USB_INTTYP_OTG                      0x00000002
#define USB_INTTYP_HOST                     0x00000004

#define USB_INTTYP_DEV_G0                   0x00000001
#define USB_INTTYP_DEV_G1                   0x00000002
#define USB_INTTYP_DEV_G2                   0x00000004

#define USB_INT_POLARITY_ACTHIGH            0x00000000
#define USB_INT_POLARITY_ACTLOW             0x00000008
//---------------------------------------------------------------------
//  0x100    Device Main Control Register
//---------------------------------------------------------------------
#define USBDEV_MAINCTL_CAP_RMWAKUP          0x00000001      // Capability of remote wakeup
#define USBDEV_MAINCTL_HALF_SPEED           0x00000002      // Half speed enable
#define USBDEV_MAINCTL_GLINT_EN             0x00000004      // Global interrupt enable
#define USBDEV_MAINCTL_GOSUSP               0x00000008      // Go suspend
#define USBDEV_MAINCTL_SFRST                0x00000010      // Device software reset
#define USBDEV_MAINCTL_CHIP_EN              0x00000020      // Chip enable, enables write cycle of FIFO controller
#define USBDEV_MAINCTL_HIGHSPEED            0x00000040      // High speed status
#define USBDEV_MAINCTL_RISC51_INF           0x00000080      // RISC51 interface enable

#define USBDEV_MAINCTL_DMARST               0x00000100      // DMA reset
#define USBDEV_MAINCTL_FORCE_FS             0x00000200      // Force the device to full-speed
#define USBDEV_MAINCTL_REMOTE_WAKEUP        0x00000400      // Remote Wakeup trigger signal
//---------------------------------------------------------------------
//    0x108    Device Test Register
//---------------------------------------------------------------------
#define USBDEV_TST_CLRFF                    0x00000001      // Clear FIFO
#define USBDEV_TST_LPCX                     0x00000002      // Loop back test for control endpoint
#define USBDEV_TST_CLREA                    0x00000004      // Clear external side address
#define USBDEV_TST_DISCRC                   0x00000008      // Disable CRC
#define USBDEV_TST_DISTOG                   0x00000010      // Disable toggle sequence
#define USBDEV_TST_MOD                      0x00000020      // Turn the test mode on
#define USBDEV_DISGENSOF                    0x00000040      // Disable generation of SOF
//---------------------------------------------------------------------
//  0x114    PHY Test Mode Selector Register
//---------------------------------------------------------------------
#define USBDEV_TSTSEL_UNPLUG                0x00000001      // set PHY in the non-driving mode
#define USBDEV_TSTSEL_JSTA                  0x00000002      // the D+/D- is set to the high speed J state
#define USBDEV_TSTSEL_KSTA                  0x00000004      // the D+/D- is set to the high speed K state
#define USBDEV_TSTSEL_SEN0NAK               0x00000008      // the D+/D- lines are set to the HS
#define USBDEV_TSTSEL_PKT                   0x00000010      // Test mode for packet
//---------------------------------------------------------------------
//    0x120    Device CX Configuration and FIFO Empty Status Register
//---------------------------------------------------------------------
#define USBDEV_CXCFG_CX_DONE                0x00000001      // Data transfer is done for CX
#define USBDEV_CXCFG_TST_PKDONE             0x00000002      // Data transfer is done for test packet
#define USBDEV_CXCFG_CX_STL                 0x00000004      // Stall CX
#define USBDEV_CXCFG_CX_CLR                 0x00000008      // Clear CX FIFO data
#define USBDEV_CXSTATUS_CXFULL              0x00000010      // CX FIFO is full
#define USBDEV_CXSTATUS_CXEMPTY             0x00000020      // CX FIFO is empty
//---------------------------------------------------------------------
//    0x130    Device Mask of Interrupt Group 0~2
//---------------------------------------------------------------------
#define USBDEV_INT_SHIFT_GROUP0                0
#define USBDEV_INT_SHIFT_GROUP1                6
#define USBDEV_INT_SHIFT_GROUP2                18
#define USBDEV_INT_MASK_GROUP0                (0x003F << USBDEV_INT_SHIFT_GROUP0)
#define USBDEV_INT_MASK_GROUP1                (0x0FFF << USBDEV_INT_SHIFT_GROUP1)
#define USBDEV_INT_MASK_GROUP2                (0x07FF << USBDEV_INT_SHIFT_GROUP2)

#define USBDEV_INT_CX_SETUP                 0x00000001      // This bit will remain asserted until firmware starts to read data from control transfer FIFO of device
#define USBDEV_INT_CX_IN                    0x00000002      // Indicate that firmware should write data for control-read transfer to control transfer FIFO
#define USBDEV_INT_CX_OUT                   0x00000004      // Indicate the control transfer contains valid data for control-write transfers
#define USBDEV_INT_CX_COMEND                0x00000008      // Indicate the control transfer has entered status stage
#define USBDEV_INT_CX_COMFAIL               0x00000010      // Indicate the control transfer is abnormally terminated
#define USBDEV_INT_CX_COMABT                0x00000020      // Indicate a command abort event occurred
#define USBDEV_INT_F0_OUT                   0x00000040      // This bit becomes "1" when FIFO 0 is ready to be read
#define USBDEV_INT_F0_SPK                   0x00000080      // This bit becomes "1" when short packet data is received in FIFO 0
#define USBDEV_INT_F1_OUT                   0x00000100      // This bit becomes "1" when FIFO 1 is ready to be read
#define USBDEV_INT_F1_SPK                   0x00000200      // This bit becomes "1" when short packet data is received in FIFO 1
#define USBDEV_INT_F2_OUT                   0x00000400      // This bit becomes "1" when FIFO 2 is ready to be read
#define USBDEV_INT_F2_SPK                   0x00000800      // This bit becomes "1" when short packet data is received in FIFO 2
#define USBDEV_INT_F3_OUT                   0x00001000      // This bit becomes "1" when FIFO 3 is ready to be read
#define USBDEV_INT_F3_SPK                   0x00002000      // This bit becomes "1" when short packet data is received in FIFO 3
#define USBDEV_INT_F0_IN                    0x00004000      // "1": FIFO 0 is ready to be written
#define USBDEV_INT_F1_IN                    0x00008000      // "1": FIFO 1 is ready to be written
#define USBDEV_INT_F2_IN                    0x00010000      // "1": FIFO 2 is ready to be written
#define USBDEV_INT_F3_IN                    0x00020000      // "1": FIFO 3 is ready to be written
#define USBDEV_INT_USBRST                   0x00040000      // When device detects USB bus reset from host, this bit will be set
#define USBDEV_INT_SUSP                     0x00080000      // When the USB bus remains in an idle state for over 3 ms, this bit will be set
#define USBDEV_INT_RESM                     0x00100000      // When the device detects a resume event from host, this bit will be set
#define USBDEV_INT_ISOSEQ_ERR               0x00200000      // When a device detects a DATA PID sequence error of an isochronous transaction in high bandwidth, this bit will be set
#define USBDEV_INT_ISOSEQ_ABT               0x00400000      // When a device detects an incomplete DATA PID sequence during a microframe, this bit will be set
#define USBDEV_INT_TX0BYTE                  0x00800000      // Transferred zero-length data packet interrupt
#define USBDEV_INT_RX0BYTE                  0x01000000      // Received zero-length data packet interrupt
#define USBDEV_INT_DMA_CMPLT                0x02000000      // DMA completion interrupt
#define USBDEV_INT_DMA_ERROR                0x04000000      // DMA error interrupt
#define USBDEV_INT_DEVIDLE                  0x08000000      // Device is in idle state
#define USBDEV_INT_DEVWAKEUP                0x10000000      // Device wakeup by Vbus
//---------------------------------------------------------------------
//  0x160    Device IN Endpoint 1 MaxPacketSize Register
//---------------------------------------------------------------------
#define USBDEV_INEP_TXNUM_HIGHBW_NONE       1               // 0 0, 0 1: Indicate endpoint 1 is not in high bandwidth
#define USBDEV_INEP_TXNUM_HIGHBW_TWO        2               // 1 0: Two transactions per microframe
#define USBDEV_INEP_TXNUM_HIGHBW_THREE      3               // 1 1: Three transactions per microframe
//---------------------------------------------------------------------
//  0x1A8    Device FIFO Map Register
//---------------------------------------------------------------------
#define USBDEV_FIFO_DIR_OUT                 0               // out
#define USBDEV_FIFO_DIR_IN                  1               // in
#define USBDEV_FIFO_DIR_BIDIR               2               // bi-direction
#define USBDEV_FIFO_DIR_NONE                3               // not allowed
//---------------------------------------------------------------------
//  0x1AC    Device FIFO Config Register
//---------------------------------------------------------------------
#define USBDEV_FIFO_BLK_TYP_MASK            0x00000003      // Transfer type of FIFO
#define USBDEV_FIFO_BLK_TYP_ISO             0x00000001      // TYP_F0 = 01: Isochronous type
#define USBDEV_FIFO_BLK_TYP_BULK            0x00000002      // TYP_F0 = 10: Bulk type
#define USBDEV_FIFO_BLK_TYP_INT             0x00000003      // TYP_F0 = 11: interrupt type
#define USBDEV_FIFO_BLKNO_MASK              0x0000000C      // Block number of FIFO
#define USBDEV_FIFO_BLKNO_SINGLE            0x00000000      // BLKNUM_F0 = 00: Single block
#define USBDEV_FIFO_BLKNO_DOUBLE            0x00000004      // BLKNUM_F0 = 01: Double blocks
#define USBDEV_FIFO_BLKNO_TRIPLE            0x00000008      // BLKNUM_F0 = 10: Triple blocks
#define USBDEV_FIFO_BLKSZ_MASK              0x00000010      // Block size of FIFO
#define USBDEV_FIFO_BLKSZ_512               0x00000000      // BLKSIZE_F0 = 0: For transferring packets whose maximum packet size is smaller than or equal to 512 bytes
#define USBDEV_FIFO_BLKSZ_1024              0x00000010      // BLKSIZE_F0 = 1: For transferring packets whose maximum packet size is smaller than or equal to 1024 bytes and greater than 512 bytes
#define USBDEV_FIFO_ENABLE                  0x00000020      // Enable FIFO
//---------------------------------------------------------------------
//  0x1C0    Device DMA Target FIFO Number Register
//---------------------------------------------------------------------
#define USBDEV_DMA2FIFO_NONE                 0x00000000
#define USBDEV_DMA2FIFO0                    0x00000001      // Accessing FIFO 0
#define USBDEV_DMA2FIFO1                    0x00000002      // Accessing FIFO 1
#define USBDEV_DMA2FIFO2                    0x00000004      // Accessing FIFO 2
#define USBDEV_DMA2FIFO3                    0x00000008      // Accessing FIFO 3
#define USBDEV_DMA2CXFIFO                    0x00000010      // Accessing control transfer FIFO

/*-------------------------------------------------
** USBOTG API
---------------------------------------------------*/
// USB OTG host APIs
extern  void    usbhostc_setReg(UINT32 uiCmmdType, UINT32 uiSetData);
extern  UINT32  usbhostc_getReg(UINT32 uiGetInfo);
// USB OTG controller APIs
extern  void    usbotgc_setReg(UINT32 uiCmmdType, UINT32 uiSetData);
extern  UINT32  usbotgc_getReg(UINT32 uiGetInfo);
// USB OTG device APIs
extern  void    usbdevc_setReg(UINT32 uiCmmdType, UINT32 uiSetData);
extern  UINT32  usbdevc_getReg(UINT32 uiGetInfo);
extern  void    usbdevc_setFIFOReg(UINT32 uiCmmdType, UINT32 uiFIFOId, UINT32 uiSetData);
extern  UINT32  usbdevc_getFIFOReg(UINT32 uiGetInfo, UINT32 uiFIFOId);
extern  void    usbdevc_setEPReg(UINT32 uiCmmdType, UINT32 uiEPId, UINT32 uiEPDir, UINT32 uiSetData);
extern  UINT32  usbdevc_getEPReg(UINT32 uiGetInfo, UINT32 uiEPId, UINT32 uiEPDir);

extern  void    usb_intisr(void);
extern  void    usb_setOtgIntCB(FP pCBIsr);
extern  void    usb_setDevIntCB(FP pCBIsr);
extern  void    usb_setHostIntCB(FP pCBIsr);

extern  UINT32  usb_getOtgIntStatus(void);
extern  UINT32  usb_getHostIntStatus(void);
extern  UINT32  usb_getDevIntStatus(void);
extern  void    usb_clrOtgIntStatus(UINT32);
extern  void    usb_clrHostIntStatus(UINT32);
extern  void    usb_clrDevIntStatus(UINT32);

extern    void     usb_enaGlobalInt(UINT32 uiEnableINT);
extern    void     usb_disGlobalInt(UINT32 uiDisableINT);
extern    void     usb_setGlobalIntType(UINT32 uiINTType);
extern    void     usb_enaOtgInt(UINT32 uiEnableINT);
extern    void     usb_disOtgInt(UINT32 uiDisableINT);
extern    void     usb_enaDevInt(UINT32 uiEnableINT);
extern    void     usb_disDevInt(UINT32 uiDisableINT);

extern  void    usb_FIFORd(UINT32 uiFIFONum, UINT8 *pu8Buffer, UINT32 uiNum);
extern  void    usb_FIFOWr(UINT32 uiFIFONum, UINT8 *pu8Buffer, UINT32 uiNum);
extern  void    usb_rwFIFODMAReq(UINT32 uiFIFOId, UINT32 uiDMADir, UINT8 *pu8Buffer, UINT32 uiNums);
extern  UINT32  usb_rwFIFOwaitDMAInt(UINT32 uiFIFOId);
extern  UINT32  usbdev_FIFO_EPxCfg(USB_DEVICE_FIFOEPXCFG_INFO *pEPCfgInfo);
extern  void    usbdev_clrAllEPxFIFOxReg(void);
extern  void    usbdev_clrAllEPx(void);
extern  void    usbdev_readSetupCmd(UINT32 *pCmd0, UINT32 *pCmd1);
extern  UINT32  usbdev_chkEPValid(UINT32 u8ep_n, UINT32 bdir);
//-----------------------------------------------------------------------------

#endif  // _USBOTG987_H_
