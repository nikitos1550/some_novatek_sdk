/* Linker script for malta
 *
 * Version: Sourcery G++ Lite 4.3-221
 * Support: https://support.codesourcery.com/GNUToolchain/
 *
 * Copyright (c) 2007, 2008, 2009 CodeSourcery, Inc.
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions.  No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 * */

/* Rule:
 * 1. to use .o in project layer just type the .o name with file extension. (UiFlow.o)
 * 2. to use .o in library layer have to add prefix module name. (DemoKit_Data/Release/ObjectLds/DscSystem/SysTask.o)
 * */

OUTPUT_ARCH(mips)
ENTRY(_start)

SECTIONS
{
    .SPECIAL_RO 0x00101000:
    {
    	_section_01_addr = ABSOLUTE(.);
        KEEP(DemoKit_Data/Release/ObjectCode/BinInfo.o(.data*));
    }
    .EXCEPTION_TABLE 0x00101000 + 0x180:
    {
        KEEP(../../ThirdParty/eCos/lib/vectors.o(.other_vector*));
    }
    .VECTOR_TABLE 0x00101000 + 0x200:
    {
        __ebase = ABSOLUTE(.);
        __start_vector_table = ABSOLUTE(.);
        KEEP(../../ThirdParty/eCos/lib/vectors.o(.interrupt_vector*));
        KEEP(../../ARC_eCos/Common/Release/Startup.a(.text.isr*));
        __end_vector_table = ABSOLUTE(.);
    }
    .CODE_INFO 0x00101000 + 0x2F0:
    {
       	_code_info_start_base = ABSOLUTE(.);
        KEEP(DemoKit_Data/Release/ObjectCode/CodeInfo.o(.data*));
        _code_info_end_base = ABSOLUTE(.);
    }
    .START_UP 0x00101000 + 0x400:
    {
        _load_entry_start_base = ABSOLUTE(.);
        KEEP(../../ThirdParty/eCos/lib/vectors.o(.text.start*));
        _load_entry_end_base = ABSOLUTE(.);
    }
    .MODELEXT_INFO 0x00101000 + 0x1000:
    {
    	_section_modelext_info_addr = ABSOLUTE(.);
    	KEEP(DemoKit_Data/Release/ObjectLds/MODELEXT_X1/modelext_info.o (modelext_header.modelext_info));
    	KEEP(*(modelext_data.modelext_info));
    	KEEP(DemoKit_Data/Release/ObjectLds/MODELEXT_X1/bin_info.o (modelext_header.bin_info));
    	KEEP(*(modelext_data.bin_info));
    	KEEP(DemoKit_Data/Release/ObjectLds/MODELEXT_X1/pinmux_cfg.o (modelext_header.pinmux_cfg));
    	KEEP(*(modelext_data.pinmux_cfg));
    	KEEP(DemoKit_Data/Release/ObjectLds/MODELEXT_X1/intdir_cfg.o (modelext_header.intdir_cfg));
    	KEEP(*(modelext_data.intdir_cfg));
    	KEEP(DemoKit_Data/Release/ObjectLds/MODELEXT_X1/emb_partition_info.o (modelext_header.emb_partition_info));
    	KEEP(*(modelext_data.emb_partition_info));
    	KEEP(DemoKit_Data/Release/ObjectLds/MODELEXT_X1/gpio_info.o (modelext_header.gpio_info));
    	KEEP(*(modelext_data.gpio_info));
    	KEEP(DemoKit_Data/Release/ObjectLds/MODELEXT_X1/dram_partition_info.o (modelext_header.dram_partition_info));
    	KEEP(*(modelext_data.dram_partition_info));
    	KEEP(DemoKit_Data/Release/ObjectLds/MODELEXT_X1/model_cfg.o (modelext_header.model_cfg));
    	KEEP(*(modelext_data.model_cfg));
    	_section_modelext_info_size = ABSOLUTE(.) - _section_modelext_info_addr;
    }
    . = ALIGN(4);
    .DOCHG_DMA_CLK :
    {
        _change_dma_clock_text_start = .;
		    KEEP(DemoKit_Data/Release/ObjectLds/Driver_Sys/clock_power_down.o(.chgdma_text*));
		    *(.chgdma_text*) /* collect functions used in change DMA flow */
		    _change_dma_clock_text_end = .;
        _change_dma_clock_data_start = .;
        KEEP(*(.chgdma_data*));
		    *(.chgdma_data*) /* collect variables used in change DMA flow */
		    _change_dma_clock_data_end = .;
    }
    _stext = ABSOLUTE(.);
    /* all global C++ constructors */
    . = ALIGN(4);

       .ctors ALIGN (0x4) : { . = .; __CTOR_LIST__ = .; PROVIDE (__CTOR_LIST__ = .); KEEP (*(SORT(.ctors.*))) KEEP (*(.ctors*)) __CTOR_END__ = .; PROVIDE (__CTOR_END__ = .); }
       .dtors ALIGN (0x4) : { . = .; __DTOR_LIST__ = .; PROVIDE (__DTOR_LIST__ = .); KEEP (*(SORT(.dtors.*))) KEEP (*(.dtors*)) __DTOR_END__ = .; PROVIDE (__DTOR_END__ = .); }
       .devtab ALIGN (0x4) : { . = .; KEEP(*( SORT (.ecos.table.*))) ; }

    /* kernel common */
    . = ALIGN(4);
    .SECTION_01 :
    {
        /*============================ before os start ===============================  */
        *(.kercfg_data*) /*collect install ID process of all modules*/
        *(.kercfg_text*) /*collect install ID process of all modules*/
    }

    /* device common */
    . = ALIGN(4);
    .SECTION_02 :
    {
    	_section_01_size = ABSOLUTE(.) - _section_01_addr;
        _section_02_addr = ABSOLUTE(.);
    }

    /* device photo */
    . = ALIGN(4);
    .SECTION_03 :
    {
    	_section_02_size = ABSOLUTE(.) - _section_02_addr;
        _section_03_addr = ABSOLUTE(.);
    }

    /* device movie */
    . = ALIGN(4);
    .SECTION_04 :
    {
    	_section_03_size = ABSOLUTE(.) - _section_03_addr;
        _section_04_addr = ABSOLUTE(.);
    }

    /* device playback */
    . = ALIGN(4);
    .SECTION_05 :
    {
    	_section_04_size = ABSOLUTE(.) - _section_04_addr;
        _section_05_addr = ABSOLUTE(.);
    }

    /* application common */
    . = ALIGN(4);
    .SECTION_06 :
    {
    	_section_05_size = ABSOLUTE(.) - _section_05_addr;
        _section_06_addr = ABSOLUTE(.);
    }

    /* application photo */
    . = ALIGN(4);
    .SECTION_07 :
    {
    	_section_06_size = ABSOLUTE(.) - _section_06_addr;
        _section_07_addr = ABSOLUTE(.);
    }

    /* application movie */
    . = ALIGN(4);
    .SECTION_08 :
    {
    	_section_07_size = ABSOLUTE(.) - _section_07_addr;
        _section_08_addr = ABSOLUTE(.);
    }

    /* application playback */
    . = ALIGN(4);
    .SECTION_09 :
    {
    	_section_08_size = ABSOLUTE(.) - _section_08_addr;
        _section_09_addr = ABSOLUTE(.);
    }

    /* others */
    . = ALIGN(4);
    .SECTION_10 :
    {
    	_section_09_size = ABSOLUTE(.) - _section_09_addr;
	KEEP (*(.nvtversioninfo.begin*))
        _section_10_addr = ABSOLUTE(.);
	_section_verinfostart = ABSOLUTE(.);
        KEEP(*(.version.info.*))
	_section_verinfoend = ABSOLUTE(.);
        KEEP (*(.nvtversioninfo.end*))
        *(.text*)
        *(.got*)
        *(.rodata*)
        *(.data*)
        *(.data1*)
        *(.rodata1*)
        *(.reginfo*)
        *(.rel.dyn*)
        *(.sdata*) /* __fdlib_version in libm.a */
    }

    _section_10_size = ABSOLUTE(.) - _section_10_addr;

    . = ALIGN(4);
    .init ALIGN (0x4) : { . = .; KEEP (*(.init*)) }
    .text ALIGN (0x4) : { _stext = .; _ftext = . ; *(.text*) *(.text.*) *(.stub) *(.gnu.warning) *(.gnu.linkonce.t*) *(.mips16.fn.*) *(.mips16.call.*) } _etext = .; PROVIDE (etext = .);

    .fini ALIGN (0x4) : { . = .; KEEP (*(.fini*)) }
    .rodata ALIGN (0x8) : { . = .; *(.rodata*) *(.rodata.*) *(.gnu.linkonce.r*) }
    .rodata1 ALIGN (0x8) : { . = .; *(.rodata1*) *(.rodata1.*) }
    .data ALIGN (0x8) : { __ram_data_start = ABSOLUTE (.); _fdata = . ; *(.data*) *(.data.*) *(.gnu.linkonce.d*) *( .2ram.*) . = ALIGN (8); SORT(CONSTRUCTORS) } __rom_data_start = LOADADDR(.data);
    .data1 ALIGN (0x8) : { . = .; *(.data1*) *(.data1.*) }
    .eh_frame_hdr ALIGN (0x4) : { . = .; __EH_FRAME_BEGIN__ = .; KEEP(*(.eh_frame_hdr*)) __FRAME_END__ = .; . = . + 8; }
    .eh_frame ALIGN (0x4) : { . = .; __EH_FRAME_BEGIN__ = .; KEEP(*(.eh_frame*)) __FRAME_END__ = .; . = . + 8; }
    .gcc_except_table ALIGN (0x4) : { . = .; KEEP(*(.gcc_except_table*)) *(.gcc_except_table.*) }

    _gp = ALIGN(16) + 0x7ff0; .got ALIGN (0x4) : { . = .; *(.got.plt) *(.got*) }
    .dynamic ALIGN (0x4) : { . = .; *(.dynamic*) }
    .sdata ALIGN (0x4) : { . = .; *(.sdata*) *(.sdata.*) *(.gnu.linkonce.s*) }
    .lit8 ALIGN (0x8) : { . = .; *(.lit8*) }
    .lit4 : AT ((LOADADDR (.lit8) + SIZEOF (.lit8) + 0x40 - 1) & ~ (0x40 - 1)) { . = .; *(.lit4) } __ram_data_end = .; _edata = . ; PROVIDE (edata = .);

    /* collect zero initialized global data (mark "B" in sym file) */
    /* collect static local data (mark "b" in sym file) */
    _section_zi_addr = ABSOLUTE(.);
    _image_general_zi_zi_base = ABSOLUTE(.);

    __bss_start = .; _fbss = .; .sbss ALIGN (0x8) : { . = .; *(.dynsbss*) *(.sbss) *(.sbss.*) *(.scommon*) *(.gnu.linkonce.sb.*) }
    .bss ALIGN (0x8) : { *(.dynbss*) *(.bss*) *(.bss.*) *(COMMON) *(.gnu.linkonce.b.*) } __bss_end = .;

    /* NOTE:
    if using -fcommon compiler option, un-initialized global data will be isolated to .common section.
    if using -fno-common compiler option, un-initialized global data will be merged to .bss section.
    memory between _image_general_zi_zi_base and _image_general_zi_zi_limit will be fill to zero before _main().
    */
    . = ALIGN(4);
    .common : {
       __common_base = ABSOLUTE(.);
       /* collect "small" un-initialized global data (mark "C" in sym file) */
       *(.scommon*)
       /* collect "large" un-initialized global data (mark "B" in sym file) */
       *(COMMON)
       __common_limit = ABSOLUTE(.);
    }
    /* _image_general_zi_zi_limit = ABSOLUTE(.); */

    . = ALIGN(16);

    /* heap area start for eCos */
    __heap1 = ALIGN(16);	/* All rest of RAM reserved for heap. !!!TODO!!!manage kernel memory pool and stack */
    __heap1_end = . + 0x00300000;   /* reserved 3MB for ecos heap memory, 3MB is enough for EVB ecos network using
                                     , this value is project dependent should be adjusted by project*/
    . = __heap1_end;
    _ecos_mem_end = .;
    _ecos_mem_size = . - 0x00101000;
    _ecos_mem_sizembit = _ecos_mem_size / (1024*1024) * 8;

    /* end of code and eCos stuff */
    _end = .; PROVIDE (end = .);

    /* stack area start for RTOS tasks */
    _image_general_zi_zi_limit = ABSOLUTE(.);
    _section_all_size = ABSOLUTE(.) - _section_01_addr;
    _stack_start = ALIGN(16);
}

EXTERN(IPL_SetDZoomFCB IPL_SetSleepFCB IPL_SetWakeupFCB IPL_GetCapRawFCB IPL_SetPauseDMAFCB IPL_SetResumeDMAFCB IPL_SetImgInOutFCB IPL_SetVAFCB IPL_GetIMEHalIdxFCB IPL_GetIPLInfoFCB IPL_TrigFCB IPL_SetTrigInfoFCB IPL_Stream2PauseFCB IPL_Pause2StreamFCB DCE_D2D_CB IPE_D2D_CB IFE_D2D_CB IFE2_D2D_CB IME_D2D_CB DRE_D2D_CB RHEIME_D2D_IFECB RHEIME_D2D_DCECB RHEIME_D2D_IPECB RHEIME_D2D_IMECB RHEIME_D2D_RHECB IPL_GetCtrlFlowFCB RHEIME_D2D_EXTFUNC_RHE_CB)
EXTERN(cyg_sdio_devtab_entry sdio_initial rltk819x_wlan0_netdev0 rltk819x_wlan_init wlan0_main)
EXTERN($SYMBOL_PLUG)
EXTERN(
NVTFD_LIBRARY_VERSION_INFO
SceneRender_LIBRARY_VERSION_INFO
NvtCv_LIBRARY_VERSION_INFO
PD_LIBRARY_VERSION_INFO
SensorHDR_LIBRARY_VERSION_INFO
BC_LIBRARY_VERSION_INFO
ADAS_Apps_LIBRARY_VERSION_INFO
DIS_FW_LIBRARY_VERSION_INFO
AutoVP_LIBRARY_VERSION_INFO
DDD_LIBRARY_VERSION_INFO
FD_LIBRARY_VERSION_INFO
SnG_LIBRARY_VERSION_INFO
LDWS_LIBRARY_VERSION_INFO
TSR_LIBRARY_VERSION_INFO
CvHost_LIBRARY_VERSION_INFO
FTG_LIBRARY_VERSION_INFO
NvtOT_LIBRARY_VERSION_INFO
FCWS_LIBRARY_VERSION_INFO
DIS_HW_LIBRARY_VERSION_INFO
Playback_LIBRARY_VERSION_INFO
LviewNvt_LIBRARY_VERSION_INFO
ImageUnit_VdoDec_LIBRARY_VERSION_INFO
ImageUnit_Play_LIBRARY_VERSION_INFO
NMediaRecVdoEnc_LIBRARY_VERSION_INFO
NMediaRecFileOut_LIBRARY_VERSION_INFO
ImageApp_UsbMovie_LIBRARY_VERSION_INFO
ImageUnit_StreamReceiver_LIBRARY_VERSION_INFO
ImageUnit_AudIn_LIBRARY_VERSION_INFO
ImageUnit_BsMux_LIBRARY_VERSION_INFO
CurlNvt_LIBRARY_VERSION_INFO
RtspNvt_LIBRARY_VERSION_INFO
ImageUnit_ImagePProc_LIBRARY_VERSION_INFO
HfsNvt_LIBRARY_VERSION_INFO
ImageUnit_NetHTTP_LIBRARY_VERSION_INFO
ImageUnit_VdoIn_LIBRARY_VERSION_INFO
ImageUnit_AudEnc_LIBRARY_VERSION_INFO
ImageUnit_Dummy_LIBRARY_VERSION_INFO
ImageApp_Photo_LIBRARY_VERSION_INFO
MovieInterface_LIBRARY_VERSION_INFO
ImageApp_IPCam_LIBRARY_VERSION_INFO
ImageUnit_VdoEnc_LIBRARY_VERSION_INFO
ImageUnit_ImgTrans_LIBRARY_VERSION_INFO
NvtUser_LIBRARY_VERSION_INFO
ImageUnit_Demux_LIBRARY_VERSION_INFO
ImageApp_MovieMulti_LIBRARY_VERSION_INFO
ImageUnit_UsbUVAC_LIBRARY_VERSION_INFO
ImageUnit_Switch_LIBRARY_VERSION_INFO
ImageStream_LIBRARY_VERSION_INFO
NMediaRecBsMux_LIBRARY_VERSION_INFO
ImageUnit_MediaPlay_LIBRARY_VERSION_INFO
ImageUnit_Mux_LIBRARY_VERSION_INFO
AppControl_LIBRARY_VERSION_INFO
VControl_LIBRARY_VERSION_INFO
SMediaPlay_LIBRARY_VERSION_INFO
ImageUnit_NetRTSP_LIBRARY_VERSION_INFO
StreamReceiver_LIBRARY_VERSION_INFO
ImageUnit_FileOut_LIBRARY_VERSION_INFO
ImageUnit_UserProc_LIBRARY_VERSION_INFO
NMediaRecAudEnc_LIBRARY_VERSION_INFO
ImageApp_MoviePlay_LIBRARY_VERSION_INFO
ImageApp_Play_LIBRARY_VERSION_INFO
videosprite_LIBRARY_VERSION_INFO
NvtMediaInterface_LIBRARY_VERSION_INFO
ImageUnit_AudOut_LIBRARY_VERSION_INFO
ImageUnit_BsDemux_LIBRARY_VERSION_INFO
nvtevent_LIBRARY_VERSION_INFO
ImageUnit_VdoOut_LIBRARY_VERSION_INFO
ImageUnit_AudDec_LIBRARY_VERSION_INFO
NvtAec_LIBRARY_VERSION_INFO
UIControl_LIBRARY_VERSION_INFO
NMediaRecImgCap_LIBRARY_VERSION_INFO
SMediaRec_LIBRARY_VERSION_INFO
ImageUnit_FileIn_LIBRARY_VERSION_INFO
ImageUnit_StreamSender_LIBRARY_VERSION_INFO
Kernel_LIBRARY_VERSION_INFO
Startup_LIBRARY_VERSION_INFO
uToken_LIBRARY_VERSION_INFO
Nvtver_LIBRARY_VERSION_INFO
ExamSrv_LIBRARY_VERSION_INFO
LibC_LIBRARY_VERSION_INFO
OS_LIBRARY_VERSION_INFO
Utility_LIBRARY_VERSION_INFO
Debug_LIBRARY_VERSION_INFO
Ramdisk_LIBRARY_VERSION_INFO
Ker_dsp_LIBRARY_VERSION_INFO
NvtIPC_LIBRARY_VERSION_INFO
sensor_LIBRARY_VERSION_INFO
Dispobj_LIBRARY_VERSION_INFO
DxSys_LIBRARY_VERSION_INFO
Audio_ExtAC108_LIBRARY_VERSION_INFO
SpiFlash_LIBRARY_VERSION_INFO
Disprot_LIBRARY_VERSION_INFO
Audio_Common_LIBRARY_VERSION_INFO
Driver_Sys_LIBRARY_VERSION_INFO
Audio_ExtWM8978_LIBRARY_VERSION_INFO
Driver_Codec_LIBRARY_VERSION_INFO
DbgUt_LIBRARY_VERSION_INFO
Driver_IPP_LIBRARY_VERSION_INFO
Ker_cdc_LIBRARY_VERSION_INFO
Dx_LIBRARY_VERSION_INFO
Audio_ExtEMU_LIBRARY_VERSION_INFO
Ker_pkg_LIBRARY_VERSION_INFO
exif_LIBRARY_VERSION_INFO
dal_jpeg_LIBRARY_VERSION_INFO
dal_h265_LIBRARY_VERSION_INFO
jpegbrc_default_LIBRARY_VERSION_INFO
dal_h264_LIBRARY_VERSION_INFO
CMOS_IMX307M_LIBRARY_VERSION_INFO
CMOS_OFF_LIBRARY_VERSION_INFO
CMOS_OFF_LIBRARY_VERSION_INFO
CMOS_OFF_LIBRARY_VERSION_INFO
CHIPCFG_LIBRARY_VERSION_INFO
AVFile_MakerMov_LIBRARY_VERSION_INFO
AAC_API_LIBRARY_VERSION_INFO
CEC_LIBRARY_VERSION_INFO
JSONparser_LIBRARY_VERSION_INFO
FwSrv_LIBRARY_VERSION_INFO
NvtAnr_LIBRARY_VERSION_INFO
FontConv_LIBRARY_VERSION_INFO
AudioCodec_uLaw_LIBRARY_VERSION_INFO
PstoreIpc_LIBRARY_VERSION_INFO
LogFile_LIBRARY_VERSION_INFO
FileSys_LIBRARY_VERSION_INFO
AudioCodec_PCM_LIBRARY_VERSION_INFO
GxUSB_LIBRARY_VERSION_INFO
GxPower_LIBRARY_VERSION_INFO
AVFile_ParserAvi_LIBRARY_VERSION_INFO
VideoCodec_MJPG_LIBRARY_VERSION_INFO
WifiCmdParser_LIBRARY_VERSION_INFO
nvt_ivot_ptyd_LIBRARY_VERSION_INFO
ipl_common_LIBRARY_VERSION_INFO
SizeConvert_LIBRARY_VERSION_INFO
ADOA_LIBRARY_VERSION_INFO
AudioCodec_aLaw_LIBRARY_VERSION_INFO
G711_LIBRARY_VERSION_INFO
NameRule_Custom_LIBRARY_VERSION_INFO
AEC_LIBRARY_VERSION_INFO
FsLinux_LIBRARY_VERSION_INFO
mp4log_LIBRARY_VERSION_INFO
AFAlg_LIBRARY_VERSION_INFO
AETask_LIBRARY_VERSION_INFO
PBXFileList_DCF_LIBRARY_VERSION_INFO
AVFile_MakerTs_LIBRARY_VERSION_INFO
Wav_LIBRARY_VERSION_INFO
AEAlg_LIBRARY_VERSION_INFO
AE_PROC_NVT_LIBRARY_VERSION_INFO
DhcpNvt_LIBRARY_VERSION_INFO
GxVideo_LIBRARY_VERSION_INFO
nvtpack_LIBRARY_VERSION_INFO
vdodisp_LIBRARY_VERSION_INFO
WiFiIpc_LIBRARY_VERSION_INFO
NvtUctrl_LIBRARY_VERSION_INFO
AVFile_ParserTs_LIBRARY_VERSION_INFO
WavStudio_LIBRARY_VERSION_INFO
NameRule_DCFFull_LIBRARY_VERSION_INFO
GxSystem_LIBRARY_VERSION_INFO
GxImageFile_LIBRARY_VERSION_INFO
FsIpc_LIBRARY_VERSION_INFO
UCDC_LIBRARY_VERSION_INFO
PBXFile_LIBRARY_VERSION_INFO
GxDSP_LIBRARY_VERSION_INFO
VideoCodec_H264_LIBRARY_VERSION_INFO
FsUitron_LIBRARY_VERSION_INFO
MsdcNvtCb_Disp_LIBRARY_VERSION_INFO
GxOutput_LIBRARY_VERSION_INFO
GxVideoFile_LIBRARY_VERSION_INFO
FileDB_LIBRARY_VERSION_INFO
GxStrg_LIBRARY_VERSION_INFO
PBXFileList_FileDB_LIBRARY_VERSION_INFO
NameRule_Empty_LIBRARY_VERSION_INFO
PBXEdit_LIBRARY_VERSION_INFO
ADPCM_LIBRARY_VERSION_INFO
awb_proc_nvt_LIBRARY_VERSION_INFO
HWAcc_LIBRARY_VERSION_INFO
AVFile_ParserMov_LIBRARY_VERSION_INFO
iq_common_LIBRARY_VERSION_INFO
AudioCodec_AAC_LIBRARY_VERSION_INFO
GxSound_LIBRARY_VERSION_INFO
awb_alg_LIBRARY_VERSION_INFO
GxGfx_LIBRARY_VERSION_INFO
PartLoad_LIBRARY_VERSION_INFO
UsockIpc_LIBRARY_VERSION_INFO
nvtmpp_LIBRARY_VERSION_INFO
CoordTs_LIBRARY_VERSION_INFO
shdr_ctrl_LIBRARY_VERSION_INFO
AGC_LIBRARY_VERSION_INFO
GxTimer_LIBRARY_VERSION_INFO
AFTask_LIBRARY_VERSION_INFO
GxDisplay_LIBRARY_VERSION_INFO
GxInput_LIBRARY_VERSION_INFO
AudioCodec_PPCM_LIBRARY_VERSION_INFO
MsdcNvt_LIBRARY_VERSION_INFO
PStore_LIBRARY_VERSION_INFO
DCF_LIBRARY_VERSION_INFO
AudNR_LIBRARY_VERSION_INFO
AudProc_LIBRARY_VERSION_INFO
NvtDispIpc_LIBRARY_VERSION_INFO
UVAC_LIBRARY_VERSION_INFO
UMSD_LIBRARY_VERSION_INFO
ipl_calibration_LIBRARY_VERSION_INFO
DIS_LIBRARY_VERSION_INFO
VideoCodec_H265_LIBRARY_VERSION_INFO
IQ_PROC_NVT_LIBRARY_VERSION_INFO
AudFilt_LIBRARY_VERSION_INFO
VideoSpriteIpc_LIBRARY_VERSION_INFO
MsdcNvtCb_File_LIBRARY_VERSION_INFO
UsbIpc_LIBRARY_VERSION_INFO
awb_task_LIBRARY_VERSION_INFO
GxImage_LIBRARY_VERSION_INFO
AF_PROC_NVT_LIBRARY_VERSION_INFO
NameRule_FileDB_LIBRARY_VERSION_INFO
IPL_FAKE_LIBRARY_VERSION_INFO
IPL_FAKE_LIBRARY_VERSION_INFO
IPL_FAKE_LIBRARY_VERSION_INFO
ipl_calibration_LIBRARY_VERSION_INFO
Capture_LIBRARY_VERSION_INFO
Photo_LIBRARY_VERSION_INFO
ImageUnit_ImagePipe_LIBRARY_VERSION_INFO
ImageUnit_Cap_LIBRARY_VERSION_INFO
Dhcpelios_LIBRARY_VERSION_INFO
devssdio_LIBRARY_VERSION_INFO
usocketcli_LIBRARY_VERSION_INFO
Iperf_LIBRARY_VERSION_INFO
hfs_LIBRARY_VERSION_INFO
libProfile_LIBRARY_VERSION_INFO
cmdsys_LIBRARY_VERSION_INFO
nvtimeth_LIBRARY_VERSION_INFO
curl_LIBRARY_VERSION_INFO
libp2padaptor_tutk_LIBRARY_VERSION_INFO
Lviewd_LIBRARY_VERSION_INFO
libextras_LIBRARY_VERSION_INFO
libtarget_LIBRARY_VERSION_INFO
nvtp2p_LIBRARY_VERSION_INFO
msdcnvt_LIBRARY_VERSION_INFO
usocket_LIBRARY_VERSION_INFO
FileSysECOS_LIBRARY_VERSION_INFO
PStoreECOS_LIBRARY_VERSION_INFO
manufacturer_LIBRARY_VERSION_INFO
devsrtl8189ftv_LIBRARY_VERSION_INFO
nvtwifi_LIBRARY_VERSION_INFO
nvtrtk_LIBRARY_VERSION_INFO
rtkcmd_LIBRARY_VERSION_INFO
rtkwps_LIBRARY_VERSION_INFO
)
