#ifndef __USBOTGAllAPIS987_H
#define __USBOTGAllAPIS987_H

#include    "StrgDef.h"
#include     "usbotg987.h"
#include     "usbhost_EHCI_reg.h"

#define OTGC_Get(cmmd)                              usbotgc_getReg(cmmd)
#define OTGC_Set(cmmd,data)                       usbotgc_setReg(cmmd,data)
#define Host20_Get(cmmd)                          usbhostc_getReg(cmmd)
#define Host20_Set(cmmd,data)                     usbhostc_setReg(cmmd,data)
#define DEVC_Get(cmmd)                              usbdevc_getReg(cmmd)
#define DEVC_Set(cmmd,data)                       usbdevc_setReg(cmmd,data)
#define DEVFIFO_Get(cmmd,fifo_num)                  usbdevc_getFIFOReg(cmmd,fifo_num)
#define DEVFIFO_Set(cmmd,fifo_num,data)           usbdevc_setFIFOReg(cmmd,fifo_num,data)
#define DEVEP_Get(cmmd,ep_num,ep_dir)              usbdevc_getEPReg(cmmd,ep_num,ep_dir)
#define DEVEP_Set(cmmd,ep_num,ep_dir,data)        usbdevc_setEPReg(cmmd,ep_num,ep_dir,data)

/*
#define mwOTG20_Interrupt_Mask_Device_Set()       USBC_Set(REG_USB_INTDIS_GLOBAL, USB_INTTYP_DEVG)
#define mwOTG20_Interrupt_Mask_OTG_Set()          USBC_Set(REG_USB_INTDIS_GLOBAL, USB_INTTYP_OTG)
#define mwOTG20_Interrupt_Mask_HOST_Set()          USBC_Set(REG_USB_INTDIS_GLOBAL, USB_INTTYP_HOST)
#define mwOTG20_Interrupt_OutPut_High_Set()          USBC_Set(REG_USB_INT_POLARITY, USB_INT_POLARITY_ACTLOW)
#define mwOTG20_Interrupt_OutPut_High_Clr()          USBC_Set(REG_USB_INT_POLARITY, USB_INT_POLARITY_ACTHIGH)
*/
//-------------------------------------------------------------------------
// OTG APIs define
//-------------------------------------------------------------------------
//    0x80    OTG Control/Status Register
#define mdwOTGC_Control_CROLE_Rd()                  OTGC_Get(REG_OTG_CURR_ROLE)     //0:Host 1:Peripheral
#define mdwOTGC_Control_ID_Rd()                      OTGC_Get(REG_OTG_CURR_ID)       //0:A-Device 1:B-Device
#define mdwOTGC_Control_Rd()                      OTGC_Get(REG_OTG_STATUS)
#define mdwOTGC_Control_Speed_Rd()                  OTGC_Get(REG_OTG_HOST_SPEED)

#define mdwOTGC_Control_Get()                      OTGC_Get(REG_OTG_STATUS)
#define mdwOTGC_Control_Set(bit)                  OTGC_Set(REG_OTG_CTRL_SETBIT,bit)
#define mdwOTGC_Control_Clr(bit)                  OTGC_Set(REG_OTG_CTRL_CLRBIT,bit)

//~B Function
#define mdwOTGC_Control_B_BUS_REQ_Rd()              (mdwOTGC_Control_Get() & USBOTG_CTRL_B_BUS_REQ)
#define mdwOTGC_Control_B_BUS_REQ_Set()            mdwOTGC_Control_Set(USBOTG_CTRL_B_BUS_REQ)
#define mdwOTGC_Control_B_BUS_REQ_Clr()            mdwOTGC_Control_Clr(USBOTG_CTRL_B_BUS_REQ)
#define mdwOTGC_Control_B_HNP_EN_Rd()              (mdwOTGC_Control_Get() & USBOTG_CTRL_B_HNP_EN)
#define mdwOTGC_Control_B_HNP_EN_Set()              mdwOTGC_Control_Set(USBOTG_CTRL_B_HNP_EN)
#define mdwOTGC_Control_B_HNP_EN_Clr()            mdwOTGC_Control_Clr(USBOTG_CTRL_B_HNP_EN)
#define mdwOTGC_Control_B_DSCHG_VBUS_Rd()          (mdwOTGC_Control_Get() & USBOTG_CTRL_B_DSCHRG_VBUS)
#define mdwOTGC_Control_B_DSCHG_VBUS_Set()          mdwOTGC_Control_Set(USBOTG_CTRL_B_DSCHRG_VBUS)
#define mdwOTGC_Control_B_DSCHG_VBUS_Clr()           mdwOTGC_Control_Clr(USBOTG_CTRL_B_DSCHRG_VBUS)
//~A Function
#define mdwOTGC_Control_A_BUS_REQ_Rd()              (mdwOTGC_Control_Get() & USBOTG_CTRL_A_BUS_REQ)
#define mdwOTGC_Control_A_BUS_REQ_Set()              mdwOTGC_Control_Set(USBOTG_CTRL_A_BUS_REQ)
#define mdwOTGC_Control_A_BUS_REQ_Clr()              mdwOTGC_Control_Clr(USBOTG_CTRL_A_BUS_REQ)
#define mdwOTGC_Control_A_BUS_DROP_Rd()              (mdwOTGC_Control_Get() & USBOTG_CTRL_A_BUS_DROP)
#define mdwOTGC_Control_A_BUS_DROP_Set()          mdwOTGC_Control_Set(USBOTG_CTRL_A_BUS_DROP)
#define mdwOTGC_Control_A_BUS_DROP_Clr()          mdwOTGC_Control_Clr(USBOTG_CTRL_A_BUS_DROP)
#define mdwOTGC_Control_A_SET_B_HNP_EN_Rd()          (mdwOTGC_Control_Get() & USBOTG_CTRL_A_SET_B_HNP_EN)
#define mdwOTGC_Control_A_SET_B_HNP_EN_Set()      mdwOTGC_Control_Set(USBOTG_CTRL_A_SET_B_HNP_EN)
#define mdwOTGC_Control_A_SET_B_HNP_EN_Clr()      mdwOTGC_Control_Clr(USBOTG_CTRL_A_SET_B_HNP_EN)
#define mdwOTGC_Control_A_SRP_DET_EN_Rd()          (mdwOTGC_Control_Get() & USBOTG_CTRL_A_SRP_DET_EN)
#define mdwOTGC_Control_A_SRP_DET_EN_Set()          mdwOTGC_Control_Set(USBOTG_CTRL_A_SRP_DET_EN)
#define mdwOTGC_Control_A_SRP_DET_EN_Clr()          mdwOTGC_Control_Clr(USBOTG_CTRL_A_SRP_DET_EN)
#define mdwOTGC_Control_A_SRP_RESP_TYPE_Rd()      (mdwOTGC_Control_Get() & USBOTG_CTRL_A_SRP_RESP_MASK)
#define mdwOTGC_Control_A_SRP_RESP_TYPE_Set(b)      mdwOTGC_Control_Set(USBOTG_CTRL_A_SRP_RESP_MASK)
#define mdwOTGC_Control_A_SRP_RESP_TYPE_Clr()      mdwOTGC_Control_Clr(USBOTG_CTRL_A_SRP_RESP_MASK)
#define mdwOTGC_Control_VBUS_FLT_SEL_Rd()          (mdwOTGC_Control_Get() & USBOTG_CTRL_Vbus_FLTSEL_MASK)
#define mdwOTGC_Control_VBUS_FLT_SEL_Set()          mdwOTGC_Control_Set(USBOTG_CTRL_Vbus_FLTSEL_MASK)
#define mdwOTGC_Control_VBUS_FLT_SEL_Clr()          mdwOTGC_Control_Clr(USBOTG_CTRL_Vbus_FLTSEL_MASK)
//    #define mdwOTGC_Control_ForceID_Set()               mdwOTGC_Control_Set(USBOTG_CTRL_FORCE_ID)
//    #define mdwOTGC_Control_ForceID_Clr()               mdwOTGC_Control_Clr(USBOTG_CTRL_FORCE_ID)
#define mdwOTGC_Control_EOF_Legacy_Set()            mdwOTGC_Control_Set(USBOTG_CTRL_EOF_LEGACY)
#define mdwOTGC_Control_EOF_Legacy_Clr()            mdwOTGC_Control_Clr(USBOTG_CTRL_EOF_LEGACY)

//#define mdwOTGC_Control_711MA_Phy_Issue_Rd()      (mdwFOTGPort(0x80)& BIT28)
//#define mdwOTGC_Control_711MA_Phy_Issue_Set()   (mdwFOTGPort(0x80) |=  BIT28)
//#define mdwOTGC_Control_711MA_Phy_Issue_Clr()   (mdwFOTGPort(0x80) &=  (~BIT28))

#define mdwOTGC_Control_B_SESS_END_Rd()              (mdwOTGC_Control_Get() & USBOTG_STAT_B_SESS_END)
#define mdwOTGC_Control_B_SESS_VLD_Rd()              (mdwOTGC_Control_Get() & USBOTG_STAT_B_SESS_VLD)
#define mdwOTGC_Control_A_SESS_VLD_Rd()              (mdwOTGC_Control_Get() & USBOTG_STAT_A_SESS_VLD)
#define mdwOTGC_Control_A_VBUS_VLD_Rd()              (mdwOTGC_Control_Get() & USBOTG_STAT_A_VBUS_VLD)

#define A_SRP_RESP_TYPE_VBUS                      USBOTG_CTRL_A_SRP_RESP_VBUS
#define A_SRP_RESP_TYPE_DATA_LINE                 USBOTG_CTRL_A_SRP_RESP_DATALINE

#define mwOTG20_Control_HOST_SPD_TYP_Rd()          mdwOTGC_Control_Speed_Rd()
#define mwOTG20_Control_A_BUS_REQ_Set()              mdwOTGC_Control_Set(USBOTG_CTRL_A_BUS_REQ)
#define mwOTG20_Control_Phy_Reset_Set()                        //usbotgc_setCtrl(USBOTG_CTRL_PHY_RESET)
#define mwOTG20_Control_Phy_Reset_Clr()                        //usbotgc_clrCtrl(USBOTG_CTRL_PHY_RESET)

//#define mwHost20_Control_ForceFullSpeed_Rd()      (mwHost20Port(0x80)& BIT12)
//#define mwHost20_Control_ForceFullSpeed_Set()        (mwHost20Bit_Set(0x80,BIT12))
//#define mwHost20_Control_ForceFullSpeed_Clr()        (mwHost20Bit_Clr(0x80,BIT12))
//#define mwHost20_Control_ForceHighSpeed_Rd()        (mwHost20Port(0x80)& BIT14)
//#define mwHost20_Control_ForceHighSpeed_Set()        (mwHost20Bit_Set(0x80,BIT14))
//#define mwHost20_Control_ForceHighSpeed_Clr()        (mwHost20Bit_Clr(0x80,BIT14))
//#define mwOTG20_Control_OTG_Reset_Set()                    (mwHost20Bit_Set(0x80,BIT24))
//#define mwOTG20_Control_OTG_Reset_Clr()                    (mwHost20Bit_Clr(0x80,BIT24))


//--------------------------------------------------------------------------
//  0x84~0x88     OTG Interrupt Enable & Status Register
//--------------------------------------------------------------------------
#define mdwOTGC_INT_Enable_Set(wValue)        usb_enaOtgInt(wValue)
#define mdwOTGC_INT_Enable_Clr(wValue)        usb_disOtgInt(wValue)

#define OTGC_INT_A_TYPE        (USBOTG_INT_A_SRP_DET|USBOTG_INT_A_VBUS_ERR|\
								USBOTG_INT_OVC|USBOTG_INT_RLCHG|USBOTG_INT_IDCHG|\
								USBOTG_INT_BPLGRMV|USBOTG_INT_APLGRMV)
#define OTGC_INT_B_TYPE     (USBOTG_INT_B_SRP_DN|USBOTG_INT_A_VBUS_ERR|\
							 USBOTG_INT_OVC|USBOTG_INT_RLCHG|USBOTG_INT_IDCHG)

//  0x88    OTG Interrupt Enable Register
#define mdwOTGC_INT_Enable_BSRPDN_Set()                     mdwOTGC_INT_Enable_Set(USBOTG_INT_B_SRP_DN)
#define mdwOTGC_INT_Enable_ASRPDET_Set()                    mdwOTGC_INT_Enable_Set(USBOTG_INT_A_SRP_DET)
#define mdwOTGC_INT_Enable_AVBUSERR_Set()                   mdwOTGC_INT_Enable_Set(USBOTG_INT_A_VBUS_ERR)
#define mdwOTGC_INT_Enable_RLCHG_Set()                      mdwOTGC_INT_Enable_Set(USBOTG_INT_RLCHG)
#define mdwOTGC_INT_Enable_IDCHG_Set()                      mdwOTGC_INT_Enable_Set(USBOTG_INT_IDCHG)
#define mdwOTGC_INT_Enable_OVC_Set()                        mdwOTGC_INT_Enable_Set(USBOTG_INT_OVC)
#define mdwOTGC_INT_Enable_BPLGRMV_Set()                    mdwOTGC_INT_Enable_Set(USBOTG_INT_BPLGRMV)
#define mdwOTGC_INT_Enable_APLGRMV_Set()                    mdwOTGC_INT_Enable_Set(USBOTG_INT_APLGRMV)
#define mdwOTGC_INT_Enable_BSRPDN_Clr()                     mdwOTGC_INT_Enable_Clr(USBOTG_INT_B_SRP_DN)
#define mdwOTGC_INT_Enable_ASRPDET_Clr()                    mdwOTGC_INT_Enable_Clr(USBOTG_INT_A_SRP_DET)
#define mdwOTGC_INT_Enable_AVBUSERR_Clr()                   mdwOTGC_INT_Enable_Clr(USBOTG_INT_A_VBUS_ERR)
#define mdwOTGC_INT_Enable_RLCHG_Clr()                      mdwOTGC_INT_Enable_Clr(USBOTG_INT_RLCHG)
#define mdwOTGC_INT_Enable_IDCHG_Clr()                      mdwOTGC_INT_Enable_Clr(USBOTG_INT_IDCHG)
#define mdwOTGC_INT_Enable_OVC_Clr()                        mdwOTGC_INT_Enable_Clr(USBOTG_INT_OVC)
#define mdwOTGC_INT_Enable_BPLGRMV_Clr()                    mdwOTGC_INT_Enable_Clr(USBOTG_INT_BPLGRMV)
#define mdwOTGC_INT_Enable_APLGRMV_Clr()                    mdwOTGC_INT_Enable_Clr(USBOTG_INT_APLGRMV)

//-------------------------------------------------------------------------
// Host APIs define
//-------------------------------------------------------------------------
//  0x00    HC Capability Register
#define mwHost20_HCIVersion_Rd()                            usbehci_getHCIVERSION()
#define mwHost20_CapLength_Rd()                                usbehci_getCAPLENGTH()
//    0x04    HC Structural parameters
#define mwHost20_NumPorts_Rd()                                usbehci_getHCSPARAMS_NPorts()
//    0x08    HC Capability parameters
#define mbHost20_ProgrammableFrameListFlag_Rd()                usbehci_getHCCPARAMS_Prog_FRListFlag_En()
//  0x10    HC USB Command Register
#define mwHost20_USBCMD_Get()                                usbehci_getUSBCMD()
#define mwHost20_USBCMD_IntThreshold_Rd()                    usbehci_getUSBCMD_IntThreshold()
#define mbHost20_USBCMD_IntThreshold_Set(bValue)            usbehci_setUSBCMD_IntThreshold(bValue)
//----->Add  "Asynchronous schedule Park mode ENable"
//----->Add  "ASYNchronous schedule Park mode CouNT"
#define mbHost20_USBCMD_ParkMode_Rd()                       (mwHost20_USBCMD_Get() & EHCI_USBCMD_ASYNC_PK_EN)
#define mbHost20_USBCMD_ParkMode_Set()                         usbehci_setUSBCMD_AsyncSchPark_En(TRUE)
#define mbHost20_USBCMD_ParkMode_Clr()                        usbehci_setUSBCMD_AsyncSchPark_En(FALSE)
#define mbHost20_USBCMD_ParkMode_CNT_Rd()                   usbehci_getUSBCMD_AsyncSchParkCnt()
#define mbHost20_USBCMD_ParkMode_CNT_Set(bValue)             usbehci_setUSBCMD_AsyncSchParkCnt(bValue)
#define mbHost20_USBCMD_InterruptOnAsync_Rd()               (mwHost20_USBCMD_Get() & EHCI_USBCMD_INT_OAAD)
#define mbHost20_USBCMD_InterruptOnAsync_Set()                 usbehci_setUSBCMD_IntOnAsyncDoorbell_En(TRUE)
#define mbHost20_USBCMD_InterruptOnAsync_Clr()                usbehci_setUSBCMD_IntOnAsyncDoorbell_En(FALSE)
#define mbHost20_USBCMD_AsynchronousEnable_Rd()             (mwHost20_USBCMD_Get() & EHCI_USBCMD_ASCH_EN)
#define mbHost20_USBCMD_AsynchronousEnable_Set()             usbehci_setUSBCMD_AsyncSch_En(TRUE)
#define mbHost20_USBCMD_AsynchronousEnable_Clr()            usbehci_setUSBCMD_AsyncSch_En(FALSE)
#define mbHost20_USBCMD_PeriodicEnable_Rd()                 (mwHost20_USBCMD_Get() & EHCI_USBCMD_PSCH_EN)
#define mbHost20_USBCMD_PeriodicEnable_Set()                 usbehci_setUSBCMD_PeriSch_En(TRUE)
#define mbHost20_USBCMD_PeriodicEnable_Clr()                usbehci_setUSBCMD_PeriSch_En(FALSE)

#define mbHost20_USBCMD_FrameListSize_Rd()                    usbehci_getUSBCMD_FRListSize()
#define mbHost20_USBCMD_FrameListSize_Set(bValue)             usbehci_setUSBCMD_FRListSize(bValue)
#define HOST20_USBCMD_FrameListSize_1024                    EHCI_USBCMD_FRLSIZE_1024
#define HOST20_USBCMD_FrameListSize_512                     EHCI_USBCMD_FRLSIZE_512
#define HOST20_USBCMD_FrameListSize_256                     EHCI_USBCMD_FRLSIZE_256
#define mbHost20_USBCMD_HCReset_Rd()                        (mwHost20_USBCMD_Get() & EHCI_USBCMD_RESET)
#define mbHost20_USBCMD_HCReset_Set()                        usbehci_setUSBCMD_Reset(TRUE)
#define mbHost20_USBCMD_RunStop_Rd()                        (mwHost20_USBCMD_Get() & EHCI_USBCMD_RUN)
#define mbHost20_USBCMD_RunStop_Set()                        usbehci_setUSBCMD_RunStop(TRUE)
#define mbHost20_USBCMD_RunStop_Clr()                        usbehci_setUSBCMD_RunStop(FALSE)
#define HOST20_Enable                                       0x01
#define HOST20_Disable                                      0x00
//  0x14    HC USB Status Register
#define mwHost20_USBSTS_Rd()                                usbehci_getUSBSTS()
#define mwHost20_USBSTS_Set(wValue)                            usbehci_clrUSBSTS(wValue)
#define mwHost20_USBSTS_Clr(wValue)                            usbehci_clrUSBSTS(wValue)
#define mwHost20_USBSTS_AsynchronousStatus_Rd()                (mwHost20_USBSTS_Rd() & EHCI_USBSTA_ASCH)
#define mwHost20_USBSTS_PeriodicStatus_Rd()                    (mwHost20_USBSTS_Rd() & EHCI_USBSTA_PSCH)
#define mwHost20_USBSTS_Reclamation_Rd()                    (mwHost20_USBSTS_Rd() & EHCI_USBSTA_RECLAMATION)
#define mwHost20_USBSTS_HCHalted_Rd()                        (mwHost20_USBSTS_Rd() & EHCI_USBSTA_HALTED)
#define mwHost20_USBSTS_IntOnAsyncAdvance_Rd()                (mwHost20_USBSTS_Rd() & EHCI_USBSTA_INT_OAA)
#define mwHost20_USBSTS_IntOnAsyncAdvance_Set()                mwHost20_USBSTS_Set(EHCI_USBSTA_INT_OAA)
#define mwHost20_USBSTS_SystemError_Rd()                    (mwHost20_USBSTS_Rd() & EHCI_USBSTA_H_SYSERR)
#define mwHost20_USBSTS_SystemError_Set()                    mwHost20_USBSTS_Set(EHCI_USBSTA_H_SYSERR)
#define mwHost20_USBSTS_FrameRollover_Rd()                    (mwHost20_USBSTS_Rd() & EHCI_USBSTA_FRL_ROL)
#define mwHost20_USBSTS_FrameRollover_Set()                    mwHost20_USBSTS_Set(EHCI_USBSTA_FRL_ROL)
#define mwHost20_USBSTS_PortChangeDetect_Rd()                (mwHost20_USBSTS_Rd() & EHCI_USBSTA_PO_CHG_DET)
#define mwHost20_USBSTS_PortChangeDetect_Set()                mwHost20_USBSTS_Set(EHCI_USBSTA_PO_CHG_DET)
#define mwHost20_USBSTS_USBError_Rd()                        (mwHost20_USBSTS_Rd() & EHCI_USBSTA_USBERR_INT)
#define mwHost20_USBSTS_USBError_Set()                        mwHost20_USBSTS_Set(EHCI_USBSTA_USBERR_INT)
#define mwHost20_USBSTS_CompletionOfTransaction_Rd()        (mwHost20_USBSTS_Rd() & EHCI_USBSTA_USB_INT)
#define mwHost20_USBSTS_CompletionOfTransaction_Set()        mwHost20_USBSTS_Set(EHCI_USBSTA_USB_INT)
//  0x18    HC USB Interrupt Enable Register
#define HOST20_USBINTR_IntOnAsyncAdvance                    EHCI_USBINT_OAA
#define HOST20_USBINTR_SystemError                          EHCI_USBINT_H_SYSERR
#define HOST20_USBINTR_FrameRollover                        EHCI_USBINT_FRL_ROL
#define HOST20_USBINTR_PortChangeDetect                     EHCI_USBINT_PO_CHG_DET
#define HOST20_USBINTR_USBError                             EHCI_USBINT_USBERR
#define HOST20_USBINTR_CompletionOfTransaction              EHCI_USBINT_USBCOMPLT

#define Host20_EnableInt(EnInt)                             usbehci_setUSBINTEnable(EnInt)
#define Host20_DisableInt(DisInt)                           usbehci_clrUSBINTEnable(DisInt)
#define mwHost20_USBINTR_Rd()                                usbehci_getUSBINTEnable()
#define mwHost20_USBINTR_Set(bValue)                        Host20_EnableInt(bValue)

#define mwHost20_USBINTR_IntOnAsyncAdvance_Rd()                (mwHost20_USBINTR_Rd() & HOST20_USBINTR_IntOnAsyncAdvance)
#define mwHost20_USBINTR_IntOnAsyncAdvance_Set()            Host20_EnableInt(HOST20_USBINTR_IntOnAsyncAdvance)
#define mwHost20_USBINTR_IntOnAsyncAdvance_Clr()            Host20_DisableInt(HOST20_USBINTR_IntOnAsyncAdvance)
#define mwHost20_USBINTR_SystemError_Rd()                    (mwHost20_USBINTR_Rd() & HOST20_USBINTR_SystemError)
#define mwHost20_USBINTR_SystemError_Set()                    Host20_EnableInt(HOST20_USBINTR_SystemError)
#define mwHost20_USBINTR_SystemError_Clr()                    Host20_DisableInt(HOST20_USBINTR_SystemError)
#define mwHost20_USBINTR_FrameRollover_Rd()                    (mwHost20_USBINTR_Rd() & HOST20_USBINTR_FrameRollover)
#define mwHost20_USBINTR_FrameRollover_Set()                Host20_EnableInt(HOST20_USBINTR_FrameRollover)
#define mwHost20_USBINTR_FrameRollover_Clr()                Host20_DisableInt(HOST20_USBINTR_FrameRollover)
#define mwHost20_USBINTR_PortChangeDetect_Rd()                (mwHost20_USBINTR_Rd() & HOST20_USBINTR_PortChangeDetect)
#define mwHost20_USBINTR_PortChangeDetect_Set()                Host20_EnableInt(HOST20_USBINTR_PortChangeDetect)
#define mwHost20_USBINTR_PortChangeDetect_Clr()                Host20_DisableInt(HOST20_USBINTR_PortChangeDetect)
#define mwHost20_USBINTR_USBError_Rd()                        (mwHost20_USBINTR_Rd() & HOST20_USBINTR_USBError)
#define mwHost20_USBINTR_USBError_Set()                        Host20_EnableInt(HOST20_USBINTR_USBError)
#define mwHost20_USBINTR_USBError_Clr()                        Host20_DisableInt(HOST20_USBINTR_USBError)
#define mwHost20_USBINTR_CompletionOfTransaction_Rd()        (mwHost20_USBINTR_Rd() & HOST20_USBINTR_CompletionOfTransaction)
#define mwHost20_USBINTR_CompletionOfTransaction_Set()        Host20_EnableInt(HOST20_USBINTR_CompletionOfTransaction)
#define mwHost20_USBINTR_CompletionOfTransaction_Clr()        Host20_DisableInt(HOST20_USBINTR_CompletionOfTransaction)

//  0x1c    HC Frame Index Register
#define mwHost20_FrameIndex_Rd()                            (usbehci_getFRINDEX() & 0x00001FFF)     //Only Read Bit0~Bit12(Skip Bit 13)
#define mwHost20_FrameIndex14Bit_Rd()                       usbehci_getFRINDEX()
#define mwHost20_FrameIndex_Set(wValue)                        usbehci_setFRINDEX(wValue)
//  0x24    HC Periodic Frame List Base Address Register
#define mwHost20_PeriodicBaseAddr_Rd()                        usbehci_getPeriFRListBase()
#define mwHost20_PeriodicBaseAddr_Set(wValue)                usbehci_setPeriFRListBase(wValue)
//  0x28    HC Current Asynchronous List Address Register
#define mwHost20_CurrentAsynchronousAddr_Rd()                usbehci_getAsyncListAddr()
#define mwHost20_CurrentAsynchronousAddr_Set(wValue)        usbehci_setAsyncListAddr(wValue)
//  0x30    HC Port Status and Control Register
#define mwHost20_PORTSC_Rd()                                usbehci_getPORTSC()
#define mwHost20_PORTSC_Set(bit)                            usbehci_setPORTSC(bit)
#define mwHost20_PORTSC_Clr(bit)                            usbehci_clrPORTSC(bit)

#define mwHost20_PORTSC_LineStatus_Rd()                        usbehci_getPORTSC_LINESTS()
#define mwHost20_Misc_LineStatus_Rd()                        usbehci_getPORTSC_LINESTS()
#define mwHost20_PORTSC_PortReset_Rd()                        (mwHost20_PORTSC_Rd() & EHCI_PORTSC_PO_RESET)
#define mwHost20_PORTSC_PortReset_Set()                        mwHost20_PORTSC_Set(EHCI_PORTSC_PO_RESET)
#define mwHost20_PORTSC_PortReset_Clr()                        mwHost20_PORTSC_Clr(EHCI_PORTSC_PO_RESET)
#define mwHost20_PORTSC_ForceSuspend_Rd()                    (mwHost20_PORTSC_Rd() & EHCI_PORTSC_PO_SUSP)
#define mwHost20_PORTSC_ForceSuspend_Set()                    mwHost20_PORTSC_Set(EHCI_PORTSC_PO_SUSP)
#define mwHost20_PORTSC_ForceResume_Rd()                    (mwHost20_PORTSC_Rd() & EHCI_PORTSC_F_PO_RESM)
#define mwHost20_PORTSC_ForceResume_Set()                    mwHost20_PORTSC_Set(EHCI_PORTSC_F_PO_RESM)
#define mwHost20_PORTSC_ForceResume_Clr()                    mwHost20_PORTSC_Clr(EHCI_PORTSC_F_PO_RESM)
#define mwHost20_PORTSC_EnableDisableChange_Rd()            (mwHost20_PORTSC_Rd() & EHCI_PORTSC_PO_EN_CHG)
#define mwHost20_PORTSC_EnableDisableChange_Set()            mwHost20_PORTSC_Set(EHCI_PORTSC_PO_EN_CHG)
#define mwHost20_PORTSC_EnableDisable_Rd()                    (mwHost20_PORTSC_Rd() & EHCI_PORTSC_PO_EN)
#define mwHost20_PORTSC_EnableDisable_Set()                    mwHost20_PORTSC_Set(EHCI_PORTSC_PO_EN)
#define mwHost20_PORTSC_EnableDisable_Clr()                    mwHost20_PORTSC_Clr(EHCI_PORTSC_PO_EN)
#define mwHost20_PORTSC_ConnectChange_Rd()                    (mwHost20_PORTSC_Rd() & EHCI_PORTSC_CONN_CHG)
#define mwHost20_PORTSC_ConnectChange_Set()                    mwHost20_PORTSC_Set(EHCI_PORTSC_CONN_CHG)
#define mwHost20_PORTSC_ConnectStatus_Rd()                    (mwHost20_PORTSC_Rd() & EHCI_PORTSC_CONN_STS)
#define mwHost20_PORTSC_PortTest_TestPkt()                  (mwHost20_PORTSC_Rd() & (EHCI_PORTSC_PORTTST_PACKET<<16))
#define mwHost20_PORTSC_PortTest_TestPktDone()              (mwHost20_PORTSC_Rd() & EHCI_PORTSC_PORTTST_PKDONE)
//    0x40    HC Misc. Register
#define mwHost20_Misc_AsynSleepTime_Set(bValue)                  Host20_Set(REG_HC_ASYNCSCH_SLEEPTIME,bValue)
#define mwHost20_Misc_EOF1Time_Set(bValue)                    Host20_Set(REG_HC_EOF1TIME,bValue)
#define mwHost20_Misc_EOF2Time_Set(bValue)                    Host20_Set(REG_HC_EOF2TIME,bValue)
#define mwHost20_Misc_Physuspend_Rd()                        Host20_Get(REG_HC_PHYSUSP)
#define mwHost20_Misc_Physuspend_Set()                        Host20_Set(REG_HC_PHYSUSP,TRUE)
#define mwHost20_Misc_Physuspend_Clr()                        Host20_Set(REG_HC_PHYSUSP,FALSE)

//-------------------------------------------------------------------------
// Device APIs define
//-------------------------------------------------------------------------
//  0x100    Device Main Control Register
#define mwPeri20_Control_Set(setBit)                        DEVC_Set(REG_DEV_MAINCTRL_SETBIT,setBit)
#define mwPeri20_Control_Clr(clrBit)                        DEVC_Set(REG_DEV_MAINCTRL_CLRBIT,clrBit)
#define mwPeri20_Control_Get()                              DEVC_Get(REG_DEV_MAINCTRL)

#define mwPeri20_Control_ChipEnable_Set()                    mwPeri20_Control_Set(USBDEV_MAINCTL_CHIP_EN)
#define mwPeri20_Control_HALFSPEEDEnable_Set()                mwPeri20_Control_Set(USBDEV_MAINCTL_HALF_SPEED)
#define mwPeri20_Control_HALFSPEEDEnable_Clr()                mwPeri20_Control_Clr(USBDEV_MAINCTL_HALF_SPEED)
#define mUsbRmWkupST()                (mwPeri20_Control_Get() & USBDEV_MAINCTL_CAP_RMWAKUP)
#define mUsbRmWkupSet()                mwPeri20_Control_Set(USBDEV_MAINCTL_CAP_RMWAKUP)
#define mUsbRmWkupClr()                mwPeri20_Control_Clr(USBDEV_MAINCTL_CAP_RMWAKUP)
#define mUsbTstHalfSpeedEn()        mwPeri20_Control_Set(USBDEV_MAINCTL_HALF_SPEED)
#define mUsbTstHalfSpeedDis()        mwPeri20_Control_Clr(USBDEV_MAINCTL_HALF_SPEED)
#define mUsbGlobIntEnRd()            (mwPeri20_Control_Get() & USBDEV_MAINCTL_GLINT_EN)
#define mUsbGlobIntEnSet()            mwPeri20_Control_Set(USBDEV_MAINCTL_GLINT_EN)
#define mUsbGlobIntDis()            mwPeri20_Control_Clr(USBDEV_MAINCTL_GLINT_EN)
#define mUsbGoSuspend()                mwPeri20_Control_Set(USBDEV_MAINCTL_GOSUSP)
#define mUsbReleaseSuspend()        mwPeri20_Control_Clr(USBDEV_MAINCTL_GOSUSP)
#define mUsbSoftRstSet()            mwPeri20_Control_Set(USBDEV_MAINCTL_SFRST)
#define mUsbSoftRstClr()            mwPeri20_Control_Clr(USBDEV_MAINCTL_SFRST)
#define mUsbChipEnSet()                mwPeri20_Control_Set(USBDEV_MAINCTL_CHIP_EN)
#define mUsbOTGHighSpeedST()        (mwPeri20_Control_Get() & USBDEV_MAINCTL_HIGHSPEED)
#define mUsbDMARst()                mwPeri20_Control_Set(USBDEV_MAINCTL_DMARST)
#define mUsbForceToFullSpeed()      mwPeri20_Control_Set(USBDEV_MAINCTL_FORCE_FS)
#define mUsbRmWkupEnable()            mwPeri20_Control_Set(USBDEV_MAINCTL_REMOTE_WAKEUP)

//JerryKo
#if 0
//    0x104    Device Address Register
#define mUsbDevAddrSet(Value)        DEVC_Set(REG_DEV_ADDR,Value)
#define mUsbCfgST()                    DEVC_Get(REG_DEV_AFTER_CFG)
#define mUsbCfgSet()                DEVC_Set(REG_DEV_AFTER_CFG,TRUE)
#define mUsbCfgClr()                DEVC_Set(REG_DEV_AFTER_CFG,FALSE)
//    0x108    Device Test Register
#define mwPeri20_TST_Set(setBit)    DEVC_Set(REG_DEV_TSTREG_SETBIT,setBit)
#define mwPeri20_TST_Clr(clrBit)    DEVC_Set(REG_DEV_TSTREG_CLRBIT,clrBit)

#define mUsbClrAllFIFOSet()            mwPeri20_TST_Set(USBDEV_TST_CLRFF)
//#define mUsbClrAllFIFOClr()        mwPeri20_TST_Clr(USBDEV_TST_CLRFF)    // HW auto clear
#define mUsbTstLoopBackSet()        mwPeri20_TST_Set(USBDEV_TST_LPCX)
#define mUsbTstClrEASet()           mwPeri20_TST_Set(USBDEV_TST_CLREA)
//    0x10C    Device SOF Frame Number Register
#define mUsbFrameNo()                DEVC_Get(REG_DEV_SOFFRAME_NUMS)
#define mUsbMicroFrameNo()            DEVC_Get(REG_DEV_SOFMFRAME_NUMS)
//    0x110    Device SOF Mask Timer Register
#define mUsbSOFMaskHS()                DEVC_Set(REG_DEV_SOFMARK_TIMER,0x44c)
#define mUsbSOFMaskFS()                DEVC_Set(REG_DEV_SOFMARK_TIMER,0x2710)
//  0x114    PHY Test Mode Selector Register
#define mUsbTsMdWr(item)            DEVC_Set(REG_DEV_PHY_TSTSEL,item)
#define mUsbUnPLGClr()                DEVC_Set(REG_DEV_PHY_UNPLUG,FALSE)
#define mUsbUnPLGSet()                DEVC_Set(REG_DEV_PHY_UNPLUG,TRUE)
//    0x120    Device CX Configuration and FIFO Empty Status Register
#define mwPeri20_CX_Set(setBit)     DEVC_Set(REG_DEV_CXFIFO_SETBIT,setBit)
#define mwPeri20_CX_Clr(clrBit)     DEVC_Set(REG_DEV_CXFIFO_CLRBIT,clrBit)
#define mwPeri20_CX_Get()           DEVC_Get(REG_DEV_CXFIFO_STATUS)

#define mUsbEP0DoneSet()            mwPeri20_CX_Set(USBDEV_CXCFG_CX_DONE)
#define mUsbTsPkDoneSet()            mwPeri20_CX_Set(USBDEV_CXCFG_TST_PKDONE)
#define mUsbEP0StallSet()            mwPeri20_CX_Set(USBDEV_CXCFG_CX_STL)
#define mUsbCxFClr()                mwPeri20_CX_Set(USBDEV_CXCFG_CX_CLR)
#define mUsbCxFFull()                (mwPeri20_CX_Get() & USBDEV_CXSTATUS_CXFULL)
#define mUsbCxFEmpty()                (mwPeri20_CX_Get() & USBDEV_CXSTATUS_CXEMPTY)
#define mUsbCxFDone()               (mwPeri20_CX_Get() & USBDEV_CXCFG_CX_DONE)
#define mUsbCxFByteCnt()            DEVC_Get(REG_DEV_CXFIFO_BYTECNT)
//#define mUsbEP0DoneClr()          (wFOTGPeri_Port(0x120) &= ~BIT0)    // ?? hw clear ?

//    0x124    Device Idle Counter Register
#define mUsbIdleCnt(time)            DEVC_Set(REG_DEV_IDLE_CNT,time)


//--------------------------------------------------------------------------
//    0x130~0x14C    Device Interrupt mask & status Group 0~2 Register
//--------------------------------------------------------------------------
#define mUsbDevIntRd()                usb_getDevIntStatus()
#define mUsbDevIntClr(wValue)          usb_clrDevIntStatus(wValue)
#define mUsbDevIntEna(wValue)          usb_enaDevInt(wValue)
#define mUsbDevIntDis(wValue)          usb_disDevInt(wValue)

//  0x134    Device Mask of Interrupt Source Group 0 Register
#define mUsbIntEP0SetupDis()        mUsbDevIntDis(USBDEV_INT_CX_SETUP)
#define mUsbIntEP0InDis()            mUsbDevIntDis(USBDEV_INT_CX_IN)
#define mUsbIntEP0OutDis()            mUsbDevIntDis(USBDEV_INT_CX_OUT)
#define mUsbIntEP0EndDis()            mUsbDevIntDis(USBDEV_INT_CX_COMEND)
#define mUsbIntEP0FailDis()            mUsbDevIntDis(USBDEV_INT_CX_COMFAIL)
#define mUsbIntEP0AbtDis()            mUsbDevIntDis(USBDEV_INT_CX_COMABT)
#define mUsbIntEP0SetupEn()         mUsbDevIntEna(USBDEV_INT_CX_SETUP)
#define mUsbIntEP0InEn()            mUsbDevIntEna(USBDEV_INT_CX_IN)
#define mUsbIntEP0OutEn()            mUsbDevIntEna(USBDEV_INT_CX_OUT)
#define mUsbIntEP0EndEn()            mUsbDevIntEna(USBDEV_INT_CX_COMEND)
#define mUsbIntEP0FailEn()            mUsbDevIntEna(USBDEV_INT_CX_COMFAIL)
#define mUsbIntEP0AbtEn()            mUsbDevIntEna(USBDEV_INT_CX_COMABT)

//  0x138    Device Mask of Interrupt Source Group 1 Register
#define mUsbIntFIFO0_3OUTDis()        mUsbDevIntDis(USBDEV_INT_F0_OUT|USBDEV_INT_F0_SPK|\
		USBDEV_INT_F1_OUT|USBDEV_INT_F1_SPK|\
		USBDEV_INT_F2_OUT|USBDEV_INT_F2_SPK|\
		USBDEV_INT_F3_OUT|USBDEV_INT_F3_SPK)

#define mUsbIntFIFO0_3INDis()        mUsbDevIntDis(USBDEV_INT_F0_IN|USBDEV_INT_F1_IN|\
		USBDEV_INT_F2_IN|USBDEV_INT_F3_IN)

#define mUsbIntF0OUTEn()            mUsbDevIntEna(USBDEV_INT_F0_OUT|USBDEV_INT_F0_SPK)
#define mUsbIntF0OUTDis()            mUsbDevIntDis(USBDEV_INT_F0_OUT|USBDEV_INT_F0_SPK)
#define mUsbIntF1OUTEn()            mUsbDevIntEna(USBDEV_INT_F1_OUT|USBDEV_INT_F1_SPK)
#define mUsbIntF1OUTDis()            mUsbDevIntDis(USBDEV_INT_F1_OUT|USBDEV_INT_F1_SPK)
#define mUsbIntF2OUTEn()            mUsbDevIntEna(USBDEV_INT_F2_OUT|USBDEV_INT_F2_SPK)
#define mUsbIntF2OUTDis()            mUsbDevIntDis(USBDEV_INT_F2_OUT|USBDEV_INT_F2_SPK)
#define mUsbIntF3OUTEn()            mUsbDevIntEna(USBDEV_INT_F3_OUT|USBDEV_INT_F3_SPK)
#define mUsbIntF3OUTDis()            mUsbDevIntDis(USBDEV_INT_F3_OUT|USBDEV_INT_F3_SPK)
#define mUsbIntF0INEn()                mUsbDevIntEna(USBDEV_INT_F0_IN)
#define mUsbIntF0INDis()            mUsbDevIntDis(USBDEV_INT_F0_IN)
#define mUsbIntF2INEn()                mUsbDevIntEna(USBDEV_INT_F2_IN)
#define mUsbIntF2INDis()            mUsbDevIntDis(USBDEV_INT_F2_IN)

//  0x13C    Device Mask of Interrupt Source Group 2 Register
#define mUsbIntSuspDis()            mUsbDevIntDis(USBDEV_INT_SUSP)
#define mUsbIntDmaErrDis()            mUsbDevIntDis(USBDEV_INT_DMA_ERROR)
#define mUsbIntDmaFinishDis()        mUsbDevIntDis(USBDEV_INT_DMA_CMPLT)
#define mUsbIntSuspEn()                mUsbDevIntEna(USBDEV_INT_SUSP)
#define mUsbIntDmaErrEn()            mUsbDevIntEna(USBDEV_INT_DMA_ERROR)
#define mUsbIntDmaFinishEn()        mUsbDevIntEna(USBDEV_INT_DMA_CMPLT)

//  0x140    Device Interrupt Group Register
//  0x144    Device Interrupt Source Group 0 Register
//  0x148    Device Interrupt Source Group 1 Register
//  0x14C    Device Interrupt Source Group 2 Register
#define mUsbIntBusRstClr()            mUsbDevIntClr(USBDEV_INT_USBRST)
#define mUsbIntSuspClr()            mUsbDevIntClr(USBDEV_INT_SUSP)
#define mUsbIntResmClr()            mUsbDevIntClr(USBDEV_INT_RESM)
#define mUsbIntIsoSeqErrClr()           mUsbDevIntClr(USBDEV_INT_ISOSEQ_ERR)
#define mUsbIntIsoSeqAbortClr()        mUsbDevIntClr(USBDEV_INT_ISOSEQ_ABT)
#define mUsbIntTX0ByteClr()            mUsbDevIntClr(USBDEV_INT_TX0BYTE)
#define mUsbIntRX0ByteClr()            mUsbDevIntClr(USBDEV_INT_RX0BYTE)
#define mUsbIntDmaFinishClr()        mUsbDevIntClr(USBDEV_INT_DMA_CMPLT)
#define mUsbIntDmaErrClr()            mUsbDevIntClr(USBDEV_INT_DMA_ERROR)
#define mUsbIntDmaFinishRd()        (mUsbDevIntRd() & USBDEV_INT_DMA_CMPLT)
#define mUsbIntDmaFinish()            (mUsbDevIntRd() & USBDEV_INT_DMA_CMPLT)
#define mUsbIntDmaErr()                (mUsbDevIntRd() & USBDEV_INT_DMA_ERROR)
//------------------------------------------------------------------------------------


//  0x150    Device Receive Zero-Length Data Packet Register
#define mUsbIntRX0ByteRd()                DEVC_Get(REG_DEV_RX0BYTE_CLRBIT)
#define mUsbIntRX0ByteSetClr(ClrData)   DEVC_Set(REG_DEV_RX0BYTE_CLRBIT,ClrData)
//  0x154    Device Transfer Zero-Length Data Packet Register
#define mUsbIntTX0ByteRd()                DEVC_Get(REG_DEV_TX0BYTE_CLRBIT)
#define mUsbIntTX0ByteSetClr(ClrData)    DEVC_Set(REG_DEV_TX0BYTE_CLRBIT,ClrData)
//  0x158    Device Isochronous Sequential Error/Abort Register
#define mUsbIntIsoSeqErrRd()            DEVC_Get(REG_DEV_ISOSEQERR)
#define mUsbIntIsoSeqErrSetClr(data)    DEVC_Set(REG_DEV_ISOSEQERR,data)
#define mUsbIntIsoSeqAbortRd()            DEVC_Get(REG_DEV_ISOSEQABT)
#define mUsbIntIsoSeqAbortSetClr(data)    DEVC_Set(REG_DEV_ISOSEQABT,data)
//  0x160~0x17C    Device IN Endpoint 1 MaxPacketSize Register
#define mUsbEPinHighBandSet(EPn,dir,size)   DEVEP_Set(REG_EP_HIGHBAND, EPn,dir,size)
#define mUsbEPMxPtSz(EPn,dir,size)    DEVEP_Set(REG_EP_MAXPACKET_SIZE, EPn,dir,size)
#define mUsbEPMxPtSzClr(EPn,dir)    DEVEP_Set(REG_EP_MAXPACKET_SIZE, EPn,dir,0)
#define mUsbEPinMxPtSz(EPn)            DEVEP_Get(REG_EP_MAXPACKET_SIZE, EPn, USBOTG_DIR_IN)
#define mUsbEPinStallST(EPn)        DEVEP_Get(REG_EP_STALL, EPn, USBOTG_DIR_IN)
#define mUsbEPinStallClr(EPn)        DEVEP_Set(REG_EP_STALL, EPn, USBOTG_DIR_IN, FALSE)
#define mUsbEPinStallSet(EPn)        DEVEP_Set(REG_EP_STALL, EPn, USBOTG_DIR_IN, TRUE)
#define mUsbEPinRsTgClr(EPn)        DEVEP_Set(REG_EP_RSTTOGGLE, EPn, USBOTG_DIR_IN, FALSE)
#define mUsbEPinRsTgSet(EPn)        DEVEP_Set(REG_EP_RSTTOGGLE, EPn, USBOTG_DIR_IN, TRUE)
//  0x180~0x19C    Device OUT Endpoint 1~8 MaxPacketSize Register
#define mUsbEPoutMxPtSz(EPn)        DEVEP_Get(REG_EP_MAXPACKET_SIZE, EPn, USBOTG_DIR_OUT)
#define mUsbEPoutStallST(EPn)        DEVEP_Get(REG_EP_STALL, EPn, USBOTG_DIR_OUT)
#define mUsbEPoutStallClr(EPn)        DEVEP_Set(REG_EP_STALL, EPn, USBOTG_DIR_OUT, FALSE)
#define mUsbEPoutStallSet(EPn)        DEVEP_Set(REG_EP_STALL, EPn, USBOTG_DIR_OUT, TRUE)
#define mUsbEPoutRsTgClr(EPn)        DEVEP_Set(REG_EP_RSTTOGGLE, EPn, USBOTG_DIR_OUT, FALSE)
#define mUsbEPoutRsTgSet(EPn)        DEVEP_Set(REG_EP_RSTTOGGLE, EPn, USBOTG_DIR_OUT, TRUE)
//  0x1A0    Device Endpoint 1~4 Map Register
#define mUsbEPMap(EPn, MAP)            DEVEP_Set(REG_EP_MAP,EPn,0,MAP)
#define mUsbEPMapRd(EPn)            DEVEP_Get(REG_EP_MAP,EPn,0)
#define mUsbEPMapAllClr()            DEVEP_Set(REG_EP_MAP_CLRALL, 0,0,0)
//  0x1A8    Device FIFO Map Register
#define mUsbFIFOMap(FIFOn, MAP)        DEVFIFO_Set(REG_FIFO_MAP,FIFOn, MAP)
#define mUsbFIFOMapRd(FIFOn)        DEVFIFO_Get(REG_FIFO_MAP,FIFOn)
#define mUsbFIFOMapAllClr()            DEVFIFO_Set(REG_FIFO_MAP_CLRALL,0,0)
//  0x1AC    Device FIFO Configuration
#define mUsbFIFOConfig(FIFOn, cfg)    DEVFIFO_Set(REG_FIFO_CFG,FIFOn, cfg)
#define mUsbFIFOConfigRd(FIFOn)        DEVFIFO_Get(REG_FIFO_CFG,FIFOn)
#define mUsbFIFOConfigAllClr()        DEVFIFO_Set(REG_FIFO_CFG_CLRALL,0,0)
#define FIFOEnBit                    USBDEV_FIFO_ENABLE
//  0x1B0    Device FIFO 0 Instruction and Byte Count Register
#define mUsbFIFOOutByteCount(fifo_num)    DEVFIFO_Get(REG_FIFO_BYTECNT,fifo_num)
#define mUsbFIFODone(fifo_num)            DEVFIFO_Set(REG_FIFO_RESET,fifo_num,0)
#define mUsbFIFOClr(fifo_num)           DEVFIFO_Set(REG_FIFO_CLEAR,fifo_num,0)


#define FOTG200_DMA2FIFO_Non         USBDEV_DMA2FIFO_NONE
#define FOTG200_DMA2FIFO0             USBDEV_DMA2FIFO0
#define FOTG200_DMA2FIFO1             USBDEV_DMA2FIFO1
#define FOTG200_DMA2FIFO2             USBDEV_DMA2FIFO2
#define FOTG200_DMA2FIFO3             USBDEV_DMA2FIFO3
#define FOTG200_DMA2CxFIFO             USBDEV_DMA2CXFIFO

//  0x1C0    Device DMA Target FIFO Number Register
#define mUsbDMA2FIFOSel(sel)        DEVC_Set(REG_DEV_DMA_TARGET_FIFO,sel)
#define mUsbDMA2FIFORd()            DEVC_Get(REG_DEV_DMA_TARGET_FIFO)
//  0x1C8    Device DMA Controller Parameter Setting 1 Register
#define mUsbDmaConfig(len,Dir)        DEVC_Set(REG_DEV_DMA_LENGTH,len);DEVC_Set(REG_DEV_DMA_DIR,Dir)
#define mUsbDmaLenRd()                DEVC_Get(REG_DEV_DMA_LENGTH)
#define mUsbDmaConfigRd()            DEVC_Get(REG_DEV_DMA_CFG)
#define mUsbDmaConfigSet(set)        DEVC_Set(REG_DEV_DMA_CFG,set)
#define mUsbDmaStart()                DEVC_Set(REG_DEV_DMA_START,TRUE)
#define mUsbDmaStop()                DEVC_Set(REG_DEV_DMA_START,FALSE)
#define mUsbDmaAbort()                DEVC_Set(REG_DEV_DMA_ABORT,TRUE)
//  0x1CC    Device DMA Controller Parameter Setting 2 Register
#define mUsbDmaAddr(addr)            DEVC_Set(REG_DEV_DMA_ADDR,addr)
#define mUsbDmaAddrRd()                DEVC_Get(REG_DEV_DMA_ADDR)
//  0x1D0    Device DMA Controller Parameter Setting 3 Register
#define mUsbEP0CmdDataRdDWord()        DEVC_Get(REG_DEV_DMA_SETUPCMD)
//  0x1D4    DMA Controller Status Register
#define mUsbDmaReaminLength()        DEVC_Get(REG_DEV_DMA_REMAIN_LENGTH)

#endif    // JerryKo mark end

//JerryKo
extern PSTRG_TAB usbhmsdc_getStorageObject(void);
extern int USBHostStateChange(void);
extern UINT8 USB_DetectWriteProtected(void);//2008/11/04 fixbug 2: write-protected USB disk
extern BOOL MSDCCardReady(void);


#define USBHOST_PLUG_STATE              1
#define USBHOST_UNPLUG_STATE            0

#endif  // __USBOTGAllAPIS987_H
