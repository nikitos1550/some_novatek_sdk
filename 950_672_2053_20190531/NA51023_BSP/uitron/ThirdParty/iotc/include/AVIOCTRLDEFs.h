/*
 * AVIOCTRLDEFs.h
 *  Define AVIOCTRL Message Type and Context
 *  Created on: 2011-08-12
 *  Author: TUTK
 *
 */

//Change Log:
//  2014-03-24 - 1> Add get local ip of device
//                      Add IOTYPE_USER_IPCAM_GET_LOCAL_IP_REQ
//                          Client request device to return local ip
//                      Add IOTYPE_USER_IPCAM_GET_LOCAL_IP_RESP
//                          Device return to client the local ip
//
//  2014-03-11 - 1> Modify SMsgAVIoctrlSetDropbox strucutre
//
//  2013-12-25 - 1> Add set and get dropbox feature of device.
//                      Add IOTYPE_USER_IPCAM_GET_SAVE_DROPBOX_REQ
//                          Client request device to return dropbox status
//                      Add IOTYPE_USER_IPCAM_GET_SAVE_DROPBOX_RESP
//                          Device return to client the dropbox status
//                      Add IOTYPE_USER_IPCAM_SET_SAVE_DROPBOX_REQ
//                          Client request device to set dropbox info
//                      Add IOTYPE_USER_IPCAM_SET_SAVE_DROPBOX_RESP
//                          Device acknowledge the set dropbox info
//
//  2013-06-25 - 1> Add set and get time zone of device.
//                      Add IOTYPE_USER_IPCAM_GET_TIMEZONE_REQ
//                          Client request device to return time zone
//                      Add IOTYPE_USER_IPCAM_GET_TIMEZONE_RESP
//                          Device return to client the time zone
//                      Add IOTYPE_USER_IPCAM_SET_TIMEZONE_REQ
//                          Client request device to set time zone
//                      Add IOTYPE_USER_IPCAM_SET_TIMEZONE_RESP
//                          Device acknowledge the set time zone request
//
//  2013-06-05 - 1> Add customer defined message type, start from #FF000000
//                  Naming role of message type : IOTYPE_[Company_name]_[function_name]
//                      ex : IOTYPE_TUTK_TEST_REQ, IOTYPE_TUTK_TEST_RESP
//                  Naming role of data structure : [Company_name]_[data_structure_name]
//                      ex : TUTK_SMsgTestReq, TUTK_SMsgTestResp
//
//
//  2013-03-10 - 1> Add flow information collection mechanism.
//                      Add IOTYPE_USER_IPCAM_GET_FLOWINFO_REQ
//                          Device request client to collect flow information.
//                      Add IOTYPE_USER_IPCAM_GET_FLOWINFO_RESP
//                          Client acknowledge device that request is received.
//                      Add IOTYPE_USER_IPCAM_CURRENT_FLOWINFO
//                          Client send collected flow information to device.
//               2> Add IOTYPE_USER_IPCAM_RECEIVE_FIRST_IFRAME command.
//
//  2013-02-19 - 1> Add more detail of status of SWifiAp
//               2> Add more detail description of STimeDay
//
//  2012-10-26 - 1> SMsgAVIoctrlGetEventConfig
//                      Add field: externIoOutIndex, externIoInIndex
//               2> SMsgAVIoctrlSetEventConfig, SMsgAVIoctrlGetEventCfgResp
//                      Add field: externIoOutStatus, externIoInStatus
//
//  2012-10-19 - 1> SMsgAVIoctrlGetWifiResp: -->SMsgAVIoctrlGetWifiResp2
//                      Add status description
//               2> SWifiAp:
//                      Add status 4: selected but not connected
//               3> WI-FI Password 32bit Change to 64bit
//               4> ENUM_AP_ENCTYPE: Add following encryption types
//                      AVIOTC_WIFIAPENC_WPA_PSK_TKIP       = 0x07,
//                      AVIOTC_WIFIAPENC_WPA_PSK_AES        = 0x08,
//                      AVIOTC_WIFIAPENC_WPA2_PSK_TKIP      = 0x09,
//                      AVIOTC_WIFIAPENC_WPA2_PSK_AES       = 0x0A,
//
//               5> IOTYPE_USER_IPCAM_SETWIFI_REQ_2:
//                      Add struct SMsgAVIoctrlSetWifiReq2
//               6> IOTYPE_USER_IPCAM_GETWIFI_RESP_2:
//                      Add struct SMsgAVIoctrlGetWifiResp2

//  2012-07-18 - added: IOTYPE_USER_IPCAM_GETAUDIOOUTFORMAT_REQ, IOTYPE_USER_IPCAM_GETAUDIOOUTFORMAT_RESP
//  2012-05-29 - IOTYPE_USER_IPCAM_DEVINFO_RESP: Modify firmware version
//  2012-05-24 - SAvEvent: Add result type
//  2014-07-07 - Change Dropbox struct SMsgAVIoctrlSetDropbox from 32 bytes to 128byes
//

#ifndef _AVIOCTRL_DEFINE_H_
#define _AVIOCTRL_DEFINE_H_

#define NOVATEK_FRMDATA_TYPE_01  0x701  // FrameData contains raw data
#define NOVATEK_DOWNLOAD_FILE    0x00
#define NOVATEK_DOWNLOAD_PHOTO   0x01

/////////////////////////////////////////////////////////////////////////////////
/////////////////// Message Type Define//////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

// AVIOCTRL Message Type
typedef enum
{
    IOTYPE_USER_IPCAM_START                     = 0x01FF,
    IOTYPE_USER_IPCAM_STOP                      = 0x02FF,
    IOTYPE_USER_IPCAM_AUDIOSTART                = 0x0300,
    IOTYPE_USER_IPCAM_AUDIOSTOP                 = 0x0301,

    IOTYPE_USER_IPCAM_SPEAKERSTART              = 0x0350,
    IOTYPE_USER_IPCAM_SPEAKERSTOP               = 0x0351,

    IOTYPE_USER_IPCAM_SETSTREAMCTRL_REQ         = 0x0320,
    IOTYPE_USER_IPCAM_SETSTREAMCTRL_RESP        = 0x0321,
    IOTYPE_USER_IPCAM_GETSTREAMCTRL_REQ         = 0x0322,
    IOTYPE_USER_IPCAM_GETSTREAMCTRL_RESP        = 0x0323,

    IOTYPE_USER_IPCAM_SETMOTIONDETECT_REQ       = 0x0324,
    IOTYPE_USER_IPCAM_SETMOTIONDETECT_RESP      = 0x0325,
    IOTYPE_USER_IPCAM_GETMOTIONDETECT_REQ       = 0x0326,
    IOTYPE_USER_IPCAM_GETMOTIONDETECT_RESP      = 0x0327,

    IOTYPE_USER_IPCAM_GETSUPPORTSTREAM_REQ      = 0x0328,   // Get Support Stream
    IOTYPE_USER_IPCAM_GETSUPPORTSTREAM_RESP     = 0x0329,

    IOTYPE_USER_IPCAM_DEVINFO_REQ               = 0x0330,
    IOTYPE_USER_IPCAM_DEVINFO_RESP              = 0x0331,

    IOTYPE_USER_IPCAM_SETPASSWORD_REQ           = 0x0332,
    IOTYPE_USER_IPCAM_SETPASSWORD_RESP          = 0x0333,

    IOTYPE_USER_IPCAM_LISTWIFIAP_REQ            = 0x0340,
    IOTYPE_USER_IPCAM_LISTWIFIAP_RESP           = 0x0341,
    IOTYPE_USER_IPCAM_SETWIFI_REQ               = 0x0342,
    IOTYPE_USER_IPCAM_SETWIFI_RESP              = 0x0343,
    IOTYPE_USER_IPCAM_GETWIFI_REQ               = 0x0344,
    IOTYPE_USER_IPCAM_GETWIFI_RESP              = 0x0345,
    IOTYPE_USER_IPCAM_SETWIFI_REQ_2             = 0x0346,
    IOTYPE_USER_IPCAM_GETWIFI_RESP_2            = 0x0347,

    IOTYPE_USER_IPCAM_SETRECORD_REQ             = 0x0310,
    IOTYPE_USER_IPCAM_SETRECORD_RESP            = 0x0311,
    IOTYPE_USER_IPCAM_GETRECORD_REQ             = 0x0312,
    IOTYPE_USER_IPCAM_GETRECORD_RESP            = 0x0313,

    IOTYPE_USER_IPCAM_SETRCD_DURATION_REQ       = 0x0314,
    IOTYPE_USER_IPCAM_SETRCD_DURATION_RESP      = 0x0315,
    IOTYPE_USER_IPCAM_GETRCD_DURATION_REQ       = 0x0316,
    IOTYPE_USER_IPCAM_GETRCD_DURATION_RESP      = 0x0317,

    IOTYPE_USER_IPCAM_LISTEVENT_REQ             = 0x0318,
    IOTYPE_USER_IPCAM_LISTEVENT_RESP            = 0x0319,

    IOTYPE_USER_IPCAM_RECORD_PLAYCONTROL        = 0x031A,
    IOTYPE_USER_IPCAM_RECORD_PLAYCONTROL_RESP   = 0x031B,

    IOTYPE_USER_IPCAM_FILELIST_REQ              = 0x031C,
    IOTYPE_USER_IPCAM_FILELIST_RESP             = 0x031D,

    IOTYPE_USER_IPCAM_GETAUDIOOUTFORMAT_REQ     = 0x032A,
    IOTYPE_USER_IPCAM_GETAUDIOOUTFORMAT_RESP    = 0x032B,

    IOTYPE_USER_IPCAM_GET_EVENTCONFIG_REQ       = 0x0400,   // Get Event Config Msg Request
    IOTYPE_USER_IPCAM_GET_EVENTCONFIG_RESP      = 0x0401,   // Get Event Config Msg Response
    IOTYPE_USER_IPCAM_SET_EVENTCONFIG_REQ       = 0x0402,   // Set Event Config Msg req
    IOTYPE_USER_IPCAM_SET_EVENTCONFIG_RESP      = 0x0403,   // Set Event Config Msg resp

    IOTYPE_USER_IPCAM_SET_ENVIRONMENT_REQ       = 0x0360,
    IOTYPE_USER_IPCAM_SET_ENVIRONMENT_RESP      = 0x0361,
    IOTYPE_USER_IPCAM_GET_ENVIRONMENT_REQ       = 0x0362,
    IOTYPE_USER_IPCAM_GET_ENVIRONMENT_RESP      = 0x0363,

    IOTYPE_USER_IPCAM_SET_VIDEOMODE_REQ         = 0x0370,   // Set Video Flip Mode
    IOTYPE_USER_IPCAM_SET_VIDEOMODE_RESP        = 0x0371,
    IOTYPE_USER_IPCAM_GET_VIDEOMODE_REQ         = 0x0372,   // Get Video Flip Mode
    IOTYPE_USER_IPCAM_GET_VIDEOMODE_RESP        = 0x0373,

    IOTYPE_USER_IPCAM_FORMATEXTSTORAGE_REQ      = 0x0380,   // Format external storage
    IOTYPE_USER_IPCAM_FORMATEXTSTORAGE_RESP     = 0x0381,

    IOTYPE_USER_IPCAM_PTZ_COMMAND               = 0x1001,   // P2P PTZ Command Msg

    IOTYPE_USER_IPCAM_EVENT_REPORT              = 0x1FFF,   // Device Event Report Msg
    IOTYPE_USER_IPCAM_RECEIVE_FIRST_IFRAME      = 0x1002,   // Send from client, used to talk to device that
                                                            // client had received the first I frame

    IOTYPE_USER_IPCAM_GET_FLOWINFO_REQ          = 0x0390,
    IOTYPE_USER_IPCAM_GET_FLOWINFO_RESP         = 0x0391,
    IOTYPE_USER_IPCAM_CURRENT_FLOWINFO          = 0x0392,

    IOTYPE_USER_IPCAM_GET_TIMEZONE_REQ          = 0x3A0,
    IOTYPE_USER_IPCAM_GET_TIMEZONE_RESP         = 0x3A1,
    IOTYPE_USER_IPCAM_SET_TIMEZONE_REQ          = 0x3B0,
    IOTYPE_USER_IPCAM_SET_TIMEZONE_RESP         = 0x3B1,

    //#NT#2015/06/09#Philex Lin - Begin
    // add query command in liveview
    IOTYPE_USER_IPCAM_LIVEVIEW_OPTIONS_REQ      = 0x03C0,
    IOTYPE_USER_IPCAM_LIVEVIEW_OPTIONS_RESP     = 0x03C1,
    //#NT#2015/06/09#Philex Lin - End

    //#NT#2015/07/28#Philex Lin - Begin
    // add query battery level command
    IOTYPE_USER_IPCAM_GET_BATTERY_LEVEL_REQ     = 0x03C2,
    IOTYPE_USER_IPCAM_GET_BATTERY_LEVEL_RESP    = 0x03C3,
    //#NT#2015/07/28#Philex Lin - Begin

    // dropbox support
    IOTYPE_USER_IPCAM_GET_SAVE_DROPBOX_REQ      = 0x500,
    IOTYPE_USER_IPCAM_GET_SAVE_DROPBOX_RESP     = 0x501,
    IOTYPE_USER_IPCAM_SET_SAVE_DROPBOX_REQ      = 0x502,
    IOTYPE_USER_IPCAM_SET_SAVE_DROPBOX_RESP     = 0x503,

	IOTYPE_USER_IPCAM_SEND_META_START           = 0x200,
    IOTYPE_USER_IPCAM_SEND_META_STOP            = 0x201,

    IOTYPE_USER_IPCAM_PUSH_STREAMING_START      = 0x210,
	IOTYPE_USER_IPCAM_PUSH_STREAMING_START_RESP = 0x211,
    IOTYPE_USER_IPCAM_PUSH_STREAMING_FINISH     = 0x212,
	IOTYPE_USER_IPCAM_PUSH_STREAMING_FINISH_RESP= 0x213,
	IOTYPE_USER_IPCAM_PUSH_STREAMING_CLOSE      = 0x214,

    IOTYPE_USER_IPCAM_GET_FILE_REQ              = 0x600,
    IOTYPE_USER_IPCAM_GET_FILE_RESP             = 0x601,

    // get local ip
    IOTYPE_USER_IPCAM_GET_LOCAL_IP_REQ          = 0x510,
    IOTYPE_USER_IPCAM_GET_LOCAL_IP_RESP         = 0x511,

    //easy set wifi
    IOTYPE_USER_IPCAM_SET_EZWIFI_REQ            = 0x512,
    IOTYPE_USER_IPCAM_SET_EZWIFI_RESP           = 0x513,

    //¦??¨µ¯è
    IOTYPE_HICHIP_CRUISE_START                  =0x600,
    IOTYPE_HICHIP_CRUISE_STOP                   =0x601,
    //«G«???
    IOTYPE_HICHIP_GETBRIGHT_REQ                 =0x602,
    IOTYPE_HICHIP_GETBRIGHT_RESP                =0x603,
    IOTYPE_HICHIP_SETBRIGHT_REQ                 =0x604,
    IOTYPE_HICHIP_SETBRIGHT_RESP                =0x605,
    //?¤ñ«???
    IOTYPE_HICHIP_GETCONTRAST_REQ               =0x606,
    IOTYPE_HICHIP_GETCONTRAST_RESP              =0x607,
    IOTYPE_HICHIP_SETCONTRAST_REQ               =0x608,
    IOTYPE_HICHIP_SETCONTRAST_RESP              =0x609,
    //?¸m¦ì
    IOTYPE_USER_IPCAM_SETPRESET_REQ             = 0x440,
    IOTYPE_USER_IPCAM_SETPRESET_RESP            = 0x441,
    IOTYPE_USER_IPCAM_GETPRESET_REQ             = 0x442,
    IOTYPE_USER_IPCAM_GETPRESET_RESP            = 0x443,

    //??µ¥¬¨¾ºM¨¾??
    //??µ?¤ù¤W?FTP??
    //??µ?¥ó³qª¾??
    IOTYPE_USER_IPCAM_GETGUARD_REQ              = 0x420,
    IOTYPE_USER_IPCAM_GETGUARD_RESP             = 0x421,
    IOTYPE_USER_IPCAM_SETGUARD_REQ              = 0x422,
    IOTYPE_USER_IPCAM_SETGUARD_RESP             = 0x423,
    //FTP?¸m
    IOTYPE_USER_IPCAM_SET_FTP_REQ               = 0x055A,
    IOTYPE_USER_IPCAM_SET_FTP_RESP              = 0x055B,
    IOTYPE_USER_IPCAM_GET_FTP_REQ               = 0x055C,
    IOTYPE_USER_IPCAM_GET_FTP_RESP              = 0x055D,
    //mail?¸m
    IOTYPE_USEREX_IPCAM_GET_SMTP_REQ            =0x4005,
    IOTYPE_USEREX_IPCAM_GET_SMTP_RESP           =0x4006,
    IOTYPE_USEREX_IPCAM_SET_SMTP_REQ            =0x4007,
    IOTYPE_USEREX_IPCAM_SET_SMTP_RESP           =0x4008,

	IOTYPE_USER_IPCAM_LISTWIFIAPALL_REQ         = 0x808,
	IOTYPE_USER_IPCAM_LISTWIFIAPALL_RESP        = 0x809, //resp wifiap list multiple times

    //========================== Ambarella IO Command =======================

    IOTYPE_USER_SETAMBCROPSET_REQ               = 0xFF001004,
    IOTYPE_USER_SETAMBCROPSET_RESP              = 0xFF001005,

    //¨ú±o¥?«emagic zoom ªºª¬ºA
    IOTYPE_USER_GETAMBCROPSTATUS_REQ            = 0xFF001006,
    IOTYPE_USER_GETAMBCROPSTATUS_RESP           = 0xFF001007,

    //¶i¤J°h¥Xmagic zoom,§¹¦¨«á·|¦¬¨ì¦¹³qª¾,§Q¥?³o­??ö³¬°?µe
    IOTYPE_USER_SETAMBCROP_GETAMBCROPIMAGECHANGE_RESP    = 0xFF001008,

    //  Send out NovatekWiFi command
    IOTYPE_USER_IPCAM_NOVATEK_WIFICMD_REQ       = 0x7F000000,
    IOTYPE_USER_IPCAM_NOVATEK_WIFICMD_RESP      = 0x7F000001,

    IOTYPE_USER_NOVATEK_SETCLIENTRECVREADY_REQ  = 0x7F000002,
    IOTYPE_USER_NOVATEK_SETCLIENTRECVREADY_RESP = 0x7F000003,

    //  Upload file from device
    IOTYPE_USER_IPCAM_UPLOAD_FILE_REQ           = 0x7F000004,
    IOTYPE_USER_IPCAM_UPLOAD_FILE_RESP          = 0x7F000005,

    //  Download file From APP
    IOTYPE_USER_IPCAM_DOWNLOAD_FILE_REQ         = 0x7F000006,
    IOTYPE_USER_IPCAM_DOWNLOAD_FILE_RESP        = 0x7F000007,

    IOTYPE_USER_NOVATEK_CUSTOM_REQ				= 0x7F000008,
    IOTYPE_USER_NOVATEK_CUSTOM_RESP				= 0x7F000009,

    IOTYPE_USER_CMD_MAX
}ENUM_AVIOCTRL_MSGTYPE;



/////////////////////////////////////////////////////////////////////////////////
/////////////////// Type ENUM Define ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    AVIOCTRL_OK                 = 0x00,
    AVIOCTRL_ERR                = -0x01,
    AVIOCTRL_ERR_PASSWORD       = AVIOCTRL_ERR - 0x01,
    AVIOCTRL_ERR_STREAMCTRL     = AVIOCTRL_ERR - 0x02,
    AVIOCTRL_ERR_MONTIONDETECT  = AVIOCTRL_ERR - 0x03,
    AVIOCTRL_ERR_DEVICEINFO     = AVIOCTRL_ERR - 0x04,
    AVIOCTRL_ERR_LOGIN          = AVIOCTRL_ERR - 5,
    AVIOCTRL_ERR_LISTWIFIAP     = AVIOCTRL_ERR - 6,
    AVIOCTRL_ERR_SETWIFI        = AVIOCTRL_ERR - 7,
    AVIOCTRL_ERR_GETWIFI        = AVIOCTRL_ERR - 8,
    AVIOCTRL_ERR_SETRECORD      = AVIOCTRL_ERR - 9,
    AVIOCTRL_ERR_SETRCDDURA     = AVIOCTRL_ERR - 10,
    AVIOCTRL_ERR_LISTEVENT      = AVIOCTRL_ERR - 11,
    AVIOCTRL_ERR_PLAYBACK       = AVIOCTRL_ERR - 12,

    AVIOCTRL_ERR_INVALIDCHANNEL = AVIOCTRL_ERR - 0x20,
}ENUM_AVIOCTRL_ERROR; //APP don't use it now


// ServType, unsigned long, 32 bits, is a bit mask for function declareation
// bit value "0" means function is valid or enabled
// in contract, bit value "1" means function is invalid or disabled.
// ** for more details, see "ServiceType Definitation for AVAPIs"
//
// Defined bits are listed below:
//----------------------------------------------
// bit      fuction
// 0        Audio in, from Device to Mobile
// 1        Audio out, from Mobile to Device
// 2        PT function
// 3        Event List function
// 4        Play back function (require Event List function)
// 5        Wi-Fi setting function
// 6        Event Setting Function
// 7        Recording Setting function
// 8        SDCard formattable function
// 9        Video flip function
// 10       Environment mode
// 11       Multi-stream selectable
// 12       Audio out encoding format

// The original enum below is obsoleted.
typedef enum
{
    SERVTYPE_IPCAM_DWH                  = 0x00,
    SERVTYPE_RAS_DWF                    = 0x01,
    SERVTYPE_IOTCAM_8125                = 0x10,
    SERVTYPE_IOTCAM_8125PT              = 0x11,
    SERVTYPE_IOTCAM_8126                = 0x12,
    SERVTYPE_IOTCAM_8126PT              = 0x13,
}ENUM_SERVICE_TYPE;

// AVIOCTRL Quality Type
typedef enum
{
    AVIOCTRL_QUALITY_UNKNOWN            = 0x00,
    AVIOCTRL_QUALITY_MAX                = 0x01, // ex. 640*480, 15fps, 320kbps (or 1280x720, 5fps, 320kbps)
    AVIOCTRL_QUALITY_HIGH               = 0x02, // ex. 640*480, 10fps, 256kbps
    AVIOCTRL_QUALITY_MIDDLE             = 0x03, // ex. 320*240, 15fps, 256kbps
    AVIOCTRL_QUALITY_LOW                = 0x04, // ex. 320*240, 10fps, 128kbps
    AVIOCTRL_QUALITY_MIN                = 0x05, // ex. 160*120, 10fps, 64kbps
}ENUM_QUALITY_LEVEL;


typedef enum
{
    AVIOTC_WIFIAPMODE_NULL              = 0x00,
    AVIOTC_WIFIAPMODE_MANAGED           = 0x01,
    AVIOTC_WIFIAPMODE_ADHOC             = 0x02,
}ENUM_AP_MODE;


typedef enum
{
    AVIOTC_WIFIAPENC_INVALID            = 0x00,
    AVIOTC_WIFIAPENC_NONE               = 0x01, //
    AVIOTC_WIFIAPENC_WEP                = 0x02, //WEP, for no password
    AVIOTC_WIFIAPENC_WPA_TKIP           = 0x03,
    AVIOTC_WIFIAPENC_WPA_AES            = 0x04,
    AVIOTC_WIFIAPENC_WPA2_TKIP          = 0x05,
    AVIOTC_WIFIAPENC_WPA2_AES           = 0x06,

    AVIOTC_WIFIAPENC_WPA_PSK_TKIP  = 0x07,
    AVIOTC_WIFIAPENC_WPA_PSK_AES   = 0x08,
    AVIOTC_WIFIAPENC_WPA2_PSK_TKIP = 0x09,
    AVIOTC_WIFIAPENC_WPA2_PSK_AES  = 0x0A,

}ENUM_AP_ENCTYPE;


// AVIOCTRL Event Type
typedef enum
{
    AVIOCTRL_EVENT_ALL                  = 0x00, // all event type(general APP-->IPCamera)
    AVIOCTRL_EVENT_MOTIONDECT           = 0x01, // motion detect start//==s==
    AVIOCTRL_EVENT_VIDEOLOST            = 0x02, // video lost alarm
    AVIOCTRL_EVENT_IOALARM              = 0x03, // io alarmin start //---s--

    AVIOCTRL_EVENT_MOTIONPASS           = 0x04, // motion detect end  //==e==
    AVIOCTRL_EVENT_VIDEORESUME          = 0x05, // video resume
    AVIOCTRL_EVENT_IOALARMPASS          = 0x06, // IO alarmin end   //---e--

    AVIOCTRL_EVENT_MOVIE                = 0x07, // for Novatek video files
    AVIOCTRL_EVENT_TIME_LAPSE           = 0x08, // for Novatek time lapse video
    AVIOCTRL_EVENT_EMERGENCY            = 0x09, // for Novatek emergency

    AVIOCTRL_EVENT_EXPT_REBOOT          = 0x10, // system exception reboot
    AVIOCTRL_EVENT_SDFAULT              = 0x11, // sd record exception
    AVIOCTRL_EVENT_FULLTIME_RECORDING   = 0x12, // full time recording
    AVIOCTRL_EVENT_EMPTY_BAT            = 0x14, // low battery
    AVIOCTRL_EVENT_EXHAUSTED_BAT        = 0x15, // empty battery
    AVIOCTRL_EVENT_PIR                  = 204, // PIR
    AVIOCTRL_EVENT_USER_CALL            = 2000, // user call
}ENUM_EVENTTYPE;

// AVIOCTRL Record Type
typedef enum
{
    AVIOTC_RECORDTYPE_OFF               = 0x00,
    AVIOTC_RECORDTYPE_FULLTIME          = 0x01,
    AVIOTC_RECORDTYPE_ALARM             = 0x02,
    AVIOTC_RECORDTYPE_MANUAL            = 0x03,
}ENUM_RECORD_TYPE;

// AVIOCTRL Play Record Command
typedef enum
{
    AVIOCTRL_RECORD_PLAY_PAUSE          = 0x00,
    AVIOCTRL_RECORD_PLAY_STOP           = 0x01,
    AVIOCTRL_RECORD_PLAY_STEPFORWARD    = 0x02, //now, APP no use
    AVIOCTRL_RECORD_PLAY_STEPBACKWARD   = 0x03, //now, APP no use
    AVIOCTRL_RECORD_PLAY_FORWARD        = 0x04, //now, APP no use
    AVIOCTRL_RECORD_PLAY_BACKWARD       = 0x05, //now, APP no use
    AVIOCTRL_RECORD_PLAY_SEEKTIME       = 0x06, //now, APP no use
    AVIOCTRL_RECORD_PLAY_END            = 0x07,
    AVIOCTRL_RECORD_PLAY_START          = 0x10,
}ENUM_PLAYCONTROL;

// AVIOCTRL Environment Mode
typedef enum
{
    AVIOCTRL_ENVIRONMENT_INDOOR_50HZ    = 0x00,
    AVIOCTRL_ENVIRONMENT_INDOOR_60HZ    = 0x01,
    AVIOCTRL_ENVIRONMENT_OUTDOOR        = 0x02,
    AVIOCTRL_ENVIRONMENT_NIGHT          = 0x03,
}ENUM_ENVIRONMENT_MODE;

// AVIOCTRL Video Flip Mode
typedef enum
{
    AVIOCTRL_VIDEOMODE_NORMAL           = 0x00,
    AVIOCTRL_VIDEOMODE_FLIP             = 0x01,
    AVIOCTRL_VIDEOMODE_MIRROR           = 0x02,
    AVIOCTRL_VIDEOMODE_FLIP_MIRROR      = 0x03,
}ENUM_VIDEO_MODE;

// AVIOCTRL PTZ Command Value
typedef enum
{
    AVIOCTRL_PTZ_STOP                   = 0,
    AVIOCTRL_PTZ_UP                     = 1,
    AVIOCTRL_PTZ_DOWN                   = 2,
    AVIOCTRL_PTZ_LEFT                   = 3,
    AVIOCTRL_PTZ_LEFT_UP                = 4,
    AVIOCTRL_PTZ_LEFT_DOWN              = 5,
    AVIOCTRL_PTZ_RIGHT                  = 6,
    AVIOCTRL_PTZ_RIGHT_UP               = 7,
    AVIOCTRL_PTZ_RIGHT_DOWN             = 8,
    AVIOCTRL_PTZ_AUTO                   = 9,
    AVIOCTRL_PTZ_SET_POINT              = 10,
    AVIOCTRL_PTZ_CLEAR_POINT            = 11,
    AVIOCTRL_PTZ_GOTO_POINT             = 12,

    AVIOCTRL_PTZ_SET_MODE_START         = 13,
    AVIOCTRL_PTZ_SET_MODE_STOP          = 14,
    AVIOCTRL_PTZ_MODE_RUN               = 15,

    AVIOCTRL_PTZ_MENU_OPEN              = 16,
    AVIOCTRL_PTZ_MENU_EXIT              = 17,
    AVIOCTRL_PTZ_MENU_ENTER             = 18,

    AVIOCTRL_PTZ_FLIP                   = 19,
    AVIOCTRL_PTZ_START                  = 20,

    AVIOCTRL_LENS_APERTURE_OPEN         = 21,
    AVIOCTRL_LENS_APERTURE_CLOSE        = 22,

    AVIOCTRL_LENS_ZOOM_IN               = 23,
    AVIOCTRL_LENS_ZOOM_OUT              = 24,

    AVIOCTRL_LENS_FOCAL_NEAR            = 25,
    AVIOCTRL_LENS_FOCAL_FAR             = 26,

    AVIOCTRL_AUTO_PAN_SPEED             = 27,
    AVIOCTRL_AUTO_PAN_LIMIT             = 28,
    AVIOCTRL_AUTO_PAN_START             = 29,

    AVIOCTRL_PATTERN_START              = 30,
    AVIOCTRL_PATTERN_STOP               = 31,
    AVIOCTRL_PATTERN_RUN                = 32,

    AVIOCTRL_SET_AUX                    = 33,
    AVIOCTRL_CLEAR_AUX                  = 34,
    AVIOCTRL_MOTOR_RESET_POSITION       = 35,
}ENUM_PTZCMD;



/////////////////////////////////////////////////////////////////////////////
///////////////////////// Message Body Define ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/*
IOTYPE_USER_IPCAM_START                 = 0x01FF,
IOTYPE_USER_IPCAM_STOP                  = 0x02FF,
IOTYPE_USER_IPCAM_AUDIOSTART            = 0x0300,
IOTYPE_USER_IPCAM_AUDIOSTOP             = 0x0301,
IOTYPE_USER_IPCAM_SPEAKERSTART          = 0x0350,
IOTYPE_USER_IPCAM_SPEAKERSTOP           = 0x0351,
** @struct SMsgAVIoctrlAVStream
*/
typedef struct
{
    unsigned int channel; // Camera Index
    unsigned char reserved[4];
} SMsgAVIoctrlAVStream;


/*
IOTYPE_USER_IPCAM_GETSTREAMCTRL_REQ     = 0x0322,
** @struct SMsgAVIoctrlGetStreamCtrlReq
*/
typedef struct
{
    unsigned int channel;   // Camera Index
    unsigned char reserved[4];
}SMsgAVIoctrlGetStreamCtrlReq;

/*
IOTYPE_USER_IPCAM_SETSTREAMCTRL_REQ     = 0x0320,
IOTYPE_USER_IPCAM_GETSTREAMCTRL_RESP    = 0x0323,
** @struct SMsgAVIoctrlSetStreamCtrlReq, SMsgAVIoctrlGetStreamCtrlResq
*/
typedef struct
{
    unsigned int  channel;  // Camera Index
    unsigned char quality;  //refer to ENUM_QUALITY_LEVEL
    unsigned char reserved[3];
} SMsgAVIoctrlSetStreamCtrlReq, SMsgAVIoctrlGetStreamCtrlResp;

/*
IOTYPE_USER_IPCAM_SETSTREAMCTRL_RESP    = 0x0321,
** @struct SMsgAVIoctrlSetStreamCtrlResp
*/
typedef struct
{
    int result; // 0: success; otherwise: failed.
    unsigned char reserved[4];
}SMsgAVIoctrlSetStreamCtrlResp;


/*
IOTYPE_USER_IPCAM_GETMOTIONDETECT_REQ   = 0x0326,
** @struct SMsgAVIoctrlGetMotionDetectReq
*/
typedef struct
{
    unsigned int channel;   // Camera Index
    unsigned char reserved[4];
}SMsgAVIoctrlGetMotionDetectReq;


/*
IOTYPE_USER_IPCAM_SETMOTIONDETECT_REQ       = 0x0324,
IOTYPE_USER_IPCAM_GETMOTIONDETECT_RESP      = 0x0327,
** @struct SMsgAVIoctrlSetMotionDetectReq, SMsgAVIoctrlGetMotionDetectResp
*/
typedef struct
{
    unsigned int channel;       // Camera Index
    unsigned int sensitivity;   // 0(Disabled) ~ 100(MAX):
                                // index        sensitivity value
                                // 0            0
                                // 1            25
                                // 2            50
                                // 3            75
                                // 4            100
}SMsgAVIoctrlSetMotionDetectReq, SMsgAVIoctrlGetMotionDetectResp;


/*
IOTYPE_USER_IPCAM_SETMOTIONDETECT_RESP  = 0x0325,
** @struct SMsgAVIoctrlSetMotionDetectResp
*/
typedef struct
{
    int result; // 0: success; otherwise: failed.
    unsigned char reserved[4];
}SMsgAVIoctrlSetMotionDetectResp;


/*
IOTYPE_USER_IPCAM_DEVINFO_REQ           = 0x0330,
** @struct SMsgAVIoctrlDeviceInfoReq
*/
typedef struct
{
    unsigned char reserved[4];
}SMsgAVIoctrlDeviceInfoReq;


/*
IOTYPE_USER_IPCAM_DEVINFO_RESP          = 0x0331,
** @struct SMsgAVIoctrlDeviceInfo
*/
typedef struct
{
    unsigned char model[16];    // IPCam mode
    unsigned char vendor[16];   // IPCam manufacturer
    unsigned int version;       // IPCam firmware version   ex. v1.2.3.4 => 0x01020304;  v1.0.0.2 => 0x01000002
    unsigned int channel;       // Camera index
    unsigned int total;         // 0: No cards been detected or an unrecognizeable sdcard that could not be re-formatted.
                                // -1: if camera detect an unrecognizable sdcard, and could be re-formatted
                                // otherwise: return total space size of sdcard (MBytes)

    unsigned int free;          // Free space size of sdcard (MBytes)
    unsigned char reserved[8];  // reserved
}SMsgAVIoctrlDeviceInfoResp;

/*
IOTYPE_USER_IPCAM_SETPASSWORD_REQ       = 0x0332,
** @struct SMsgAVIoctrlSetPasswdReq
*/
typedef struct
{
    char oldpasswd[32];         // The old security code
    char newpasswd[32];         // The new security code
}SMsgAVIoctrlSetPasswdReq;


/*
IOTYPE_USER_IPCAM_SETPASSWORD_RESP      = 0x0333,
** @struct SMsgAVIoctrlSetPasswdResp
*/
typedef struct
{
    int result; // 0: success; otherwise: failed.
    unsigned char reserved[4];
}SMsgAVIoctrlSetPasswdResp;


/*
IOTYPE_USER_IPCAM_LISTWIFIAP_REQ        = 0x0340,
** @struct SMsgAVIoctrlListWifiApReq
*/
typedef struct
{
    unsigned char reserved[4];
}SMsgAVIoctrlListWifiApReq;

typedef struct
{
    char ssid[32];              // WiFi ssid
    char mode;                  // refer to ENUM_AP_MODE
    char enctype;               // refer to ENUM_AP_ENCTYPE
    char signal;                // signal intensity 0--100%
    char status;                // 0 : invalid ssid or disconnected
                                // 1 : connected with default gateway
                                // 2 : unmatched password
                                // 3 : weak signal and connected
                                // 4 : selected:
                                //      - password matched and
                                //      - disconnected or connected but not default gateway
}SWifiAp;

typedef struct
{
	unsigned char reserved[4];
}SMsgAVIoctrlListWifiApAllReq;

/*
IOTYPE_USER_IPCAM_LISTWIFIAPALL_RESP		= 0x0809,
** @struct SMsgAVIoctrlListWifiApAllResp
*/
typedef struct
{
	unsigned int total;		// Total Wifi amount in this search session
	unsigned char index;		// package index, 0,1,2...;
								// because avSendIOCtrl() send package up to 1024 bytes one time, you may want split search results to serveral package to send.
	unsigned char endflag;		// end flag; endFlag = 1 means this package is the last one.
	unsigned char count;		// how much Wifi ssid in this package
	SWifiAp stWifiAp[];
}SMsgAVIoctrlListWifiApAllResp;

/*
IOTYPE_USER_IPCAM_LISTWIFIAP_RESP       = 0x0341,
** @struct SMsgAVIoctrlListWifiApResp
*/
typedef struct
{
    unsigned int number; // MAX number: 1024(IOCtrl packet size) / 36(bytes) = 28
    SWifiAp stWifiAp[1];
}SMsgAVIoctrlListWifiApResp;

/*
IOTYPE_USER_IPCAM_SETWIFI_REQ           = 0x0342,
** @struct SMsgAVIoctrlSetWifiReq
*/
typedef struct
{
    unsigned char ssid[32];         //WiFi ssid
    unsigned char password[32];     //if exist, WiFi password
    unsigned char mode;             //refer to ENUM_AP_MODE
    unsigned char enctype;          //refer to ENUM_AP_ENCTYPE
    unsigned char reserved[10];
}SMsgAVIoctrlSetWifiReq;

//IOTYPE_USER_IPCAM_SETWIFI_REQ_2       = 0x0346,
typedef struct
{
    unsigned char ssid[32];     // WiFi ssid
    unsigned char password[64]; // if exist, WiFi password
    unsigned char mode;         // refer to ENUM_AP_MODE
    unsigned char enctype;      // refer to ENUM_AP_ENCTYPE
    unsigned char reserved[10];
}SMsgAVIoctrlSetWifiReq2;

/*
IOTYPE_USER_IPCAM_SETWIFI_RESP          = 0x0343,
** @struct SMsgAVIoctrlSetWifiResp
*/
typedef struct
{
    int result; //0: wifi connected; 1: failed to connect
    unsigned char reserved[4];
}SMsgAVIoctrlSetWifiResp;

/*
IOTYPE_USER_IPCAM_GETWIFI_REQ           = 0x0344,
** @struct SMsgAVIoctrlGetWifiReq
*/
typedef struct
{
    unsigned char reserved[4];
}SMsgAVIoctrlGetWifiReq;

/*
IOTYPE_USER_IPCAM_GETWIFI_RESP          = 0x0345,
** @struct SMsgAVIoctrlGetWifiResp //if no wifi connected, members of SMsgAVIoctrlGetWifiResp are all 0
*/
typedef struct
{
    unsigned char ssid[32];     // WiFi ssid
    unsigned char password[32]; // WiFi password if not empty
    unsigned char mode;         // refer to ENUM_AP_MODE
    unsigned char enctype;      // refer to ENUM_AP_ENCTYPE
    unsigned char signal;       // signal intensity 0--100%
    unsigned char status;       // refer to "status" of SWifiAp
}SMsgAVIoctrlGetWifiResp;

//changed: WI-FI Password 32bit Change to 64bit
//IOTYPE_USER_IPCAM_GETWIFI_RESP_2    = 0x0347,
typedef struct
{
 unsigned char ssid[32];     // WiFi ssid
 unsigned char password[64]; // WiFi password if not empty
 unsigned char mode;    // refer to ENUM_AP_MODE
 unsigned char enctype; // refer to ENUM_AP_ENCTYPE
 unsigned char signal;  // signal intensity 0--100%
 unsigned char status;  // refer to "status" of SWifiAp
}SMsgAVIoctrlGetWifiResp2;

/*
IOTYPE_USER_IPCAM_GETRECORD_REQ         = 0x0312,
** @struct SMsgAVIoctrlGetRecordReq
*/
typedef struct
{
    unsigned int channel; // Camera Index
    unsigned char reserved[4];
}SMsgAVIoctrlGetRecordReq;

/*
IOTYPE_USER_IPCAM_SETRECORD_REQ         = 0x0310,
IOTYPE_USER_IPCAM_GETRECORD_RESP        = 0x0313,
** @struct SMsgAVIoctrlSetRecordReq, SMsgAVIoctrlGetRecordResq
*/
typedef struct
{
    unsigned int channel;       // Camera Index
    unsigned int recordType;    // Refer to ENUM_RECORD_TYPE
    unsigned char reserved[4];
}SMsgAVIoctrlSetRecordReq, SMsgAVIoctrlGetRecordResp;

/*
IOTYPE_USER_IPCAM_SETRECORD_RESP        = 0x0311,
** @struct SMsgAVIoctrlSetRecordResp
*/
typedef struct
{
    int result; // 0: success; otherwise: failed.
    unsigned char reserved[4];
}SMsgAVIoctrlSetRecordResp;


/*
IOTYPE_USER_IPCAM_GETRCD_DURATION_REQ   = 0x0316,
** @struct SMsgAVIoctrlGetRcdDurationReq
*/
typedef struct
{
    unsigned int channel; // Camera Index
    unsigned char reserved[4];
}SMsgAVIoctrlGetRcdDurationReq;

/*
IOTYPE_USER_IPCAM_SETRCD_DURATION_REQ   = 0x0314,
IOTYPE_USER_IPCAM_GETRCD_DURATION_RESP  = 0x0317,
** @struct SMsgAVIoctrlSetRcdDurationReq, SMsgAVIoctrlGetRcdDurationResp
*/
typedef struct
{
    unsigned int channel;       // Camera Index
    unsigned int presecond;     // pre-recording (sec)
    unsigned int durasecond;    // recording (sec)
}SMsgAVIoctrlSetRcdDurationReq, SMsgAVIoctrlGetRcdDurationResp;


/*
IOTYPE_USER_IPCAM_SETRCD_DURATION_RESP  = 0x0315,
** @struct SMsgAVIoctrlSetRcdDurationResp
*/
typedef struct
{
    int result; // 0: success; otherwise: failed.
    unsigned char reserved[4];
}SMsgAVIoctrlSetRcdDurationResp;


typedef struct
{
    unsigned short year;    // The number of year.
    unsigned char month;    // The number of months since January, in the range 1 to 12.
    unsigned char day;      // The day of the month, in the range 1 to 31.
    unsigned char wday;     // The number of days since Sunday, in the range 0 to 6. (Sunday = 0, Monday = 1, ...)
    unsigned char hour;     // The number of hours past midnight, in the range 0 to 23.
    unsigned char minute;   // The number of minutes after the hour, in the range 0 to 59.
    unsigned char second;   // The number of seconds after the minute, in the range 0 to 59.
}STimeDay;

/*
IOTYPE_USER_IPCAM_LISTEVENT_REQ         = 0x0318,
** @struct SMsgAVIoctrlListEventReq
*/
typedef struct
{
    unsigned int channel;       // Camera Index
    STimeDay stStartTime;       // Search event from ...
    STimeDay stEndTime;         // ... to (search event)
    unsigned char event;        // event type, refer to ENUM_EVENTTYPE
    unsigned char status;       // 0x00: Recording file exists, Event unreaded
                                // 0x01: Recording file exists, Event readed
                                // 0x02: No Recording file in the event
    unsigned char reserved[2];
}SMsgAVIoctrlListEventReq;


typedef struct
{
    STimeDay stTime;
    unsigned char event;
    unsigned char status;   // 0x00: Recording file exists, Event unreaded
                            // 0x01: Recording file exists, Event readed
                            // 0x02: No Recording file in the event
    unsigned char reserved[2];
}SAvEvent;

/*
IOTYPE_USER_IPCAM_LISTEVENT_RESP        = 0x0319,
** @struct SMsgAVIoctrlListEventResp
*/
typedef struct
{
    unsigned int  channel;      // Camera Index
    unsigned int  total;        // Total event amount in this search session
    unsigned char index;        // package index, 0,1,2...;
                                // because avSendIOCtrl() send package up to 1024 bytes one time, you may want split search results to serveral package to send.
    unsigned char endflag;      // end flag; endFlag = 1 means this package is the last one.
    unsigned char count;        // how much events in this package
    unsigned char reserved[1];
    SAvEvent stEvent[1];        // The first memory address of the events in this package
}SMsgAVIoctrlListEventResp;


/*
IOTYPE_USER_IPCAM_RECORD_PLAYCONTROL    = 0x031A,
** @struct SMsgAVIoctrlPlayRecord
*/
typedef struct
{
    unsigned int channel;   // Camera Index
    unsigned int command;   // play record command. refer to ENUM_PLAYCONTROL
    unsigned int Param;     // command param, that the user defined
    STimeDay stTimeDay;     // Event time from ListEvent
    unsigned char reserved[4];
} SMsgAVIoctrlPlayRecord;

/*
IOTYPE_USER_IPCAM_RECORD_PLAYCONTROL_RESP   = 0x031B,
** @struct SMsgAVIoctrlPlayRecordResp
*/
typedef struct
{
    unsigned int command;   // Play record command. refer to ENUM_PLAYCONTROL
    unsigned int result;    // Depends on command
                            // when is AVIOCTRL_RECORD_PLAY_START:
                            //  result>=0   real channel no used by device for playback
                            //  result <0   error
                            //          -1  playback error
                            //          -2  exceed max allow client amount
    unsigned char reserved[4];
} SMsgAVIoctrlPlayRecordResp; // only for play record start command


/*
IOTYPE_USER_IPCAM_PTZ_COMMAND   = 0x1001,   // P2P Ptz Command Msg
** @struct SMsgAVIoctrlPtzCmd
*/
typedef struct
{
    unsigned char control;  // PTZ control command, refer to ENUM_PTZCMD
    unsigned char speed;    // PTZ control speed
    unsigned char point;    // no use in APP so far. preset position, for RS485 PT
    unsigned char limit;    // no use in APP so far.
    unsigned char aux;      // no use in APP so far. auxiliary switch, for RS485 PT
    unsigned char channel;  // camera index
    unsigned char reserve[2];
} SMsgAVIoctrlPtzCmd;

/*
IOTYPE_USER_IPCAM_EVENT_REPORT  = 0x1FFF,   // Device Event Report Msg
*/
/** @struct SMsgAVIoctrlEvent
 */
typedef struct
{
    STimeDay stTime;
    unsigned long time;     // UTC Time
    unsigned int  channel;  // Camera Index
    unsigned int  event;    // Event Type
    unsigned char reserved[4];
} SMsgAVIoctrlEvent;



#if 0

/*  IOTYPE_USER_IPCAM_GET_EVENTCONFIG_REQ   = 0x0400,   // Get Event Config Msg Request
 */
/** @struct SMsgAVIoctrlGetEventConfig
 */
typedef struct
{
    unsigned int    channel;          //Camera Index
    unsigned char   externIoOutIndex; //extern out index: bit0->io0 bit1->io1 ... bit7->io7;=1: get this io value or not get
    unsigned char   externIoInIndex;  //extern in index: bit0->io0 bit1->io1 ... bit7->io7; =1: get this io value or not get
    char reserved[2];
} SMsgAVIoctrlGetEventConfig;

/*
    IOTYPE_USER_IPCAM_GET_EVENTCONFIG_RESP  = 0x0401,   // Get Event Config Msg Response
    IOTYPE_USER_IPCAM_SET_EVENTCONFIG_REQ   = 0x0402,   // Set Event Config Msg req
*/
/* @struct SMsgAVIoctrlSetEventConfig
 * @struct SMsgAVIoctrlGetEventCfgResp
 */
typedef struct
{
    unsigned int    channel;        // Camera Index
    unsigned char   mail;           // enable send email
    unsigned char   ftp;            // enable ftp upload photo
    unsigned char   externIoOutStatus;   // enable extern io output //bit0->io0 bit1->io1 ... bit7->io7; 1:on; 0:off
    unsigned char   p2pPushMsg;          // enable p2p push msg
    unsigned char   externIoInStatus;    // enable extern io input  //bit0->io0 bit1->io1 ... bit7->io7; 1:on; 0:off
    char            reserved[3];
}SMsgAVIoctrlSetEventConfig, SMsgAVIoctrlGetEventCfgResp;

/*
    IOTYPE_USER_IPCAM_SET_EVENTCONFIG_RESP  = 0x0403,   // Set Event Config Msg resp
*/
/** @struct SMsgAVIoctrlSetEventCfgResp
 */
typedef struct
{
    unsigned int channel;   // Camera Index
    unsigned int result;    // 0: success; otherwise: failed.
}SMsgAVIoctrlSetEventCfgResp;

#endif


/*
IOTYPE_USER_IPCAM_SET_ENVIRONMENT_REQ       = 0x0360,
** @struct SMsgAVIoctrlSetEnvironmentReq
*/
typedef struct
{
    unsigned int channel;       // Camera Index
    unsigned char mode;         // refer to ENUM_ENVIRONMENT_MODE
    unsigned char reserved[3];
}SMsgAVIoctrlSetEnvironmentReq;


/*
IOTYPE_USER_IPCAM_SET_ENVIRONMENT_RESP      = 0x0361,
** @struct SMsgAVIoctrlSetEnvironmentResp
*/
typedef struct
{
    unsigned int channel;       // Camera Index
    unsigned char result;       // 0: success; otherwise: failed.
    unsigned char reserved[3];
}SMsgAVIoctrlSetEnvironmentResp;


/*
IOTYPE_USER_IPCAM_GET_ENVIRONMENT_REQ       = 0x0362,
** @struct SMsgAVIoctrlGetEnvironmentReq
*/
typedef struct
{
    unsigned int channel;   // Camera Index
    unsigned char reserved[4];
}SMsgAVIoctrlGetEnvironmentReq;

/*
IOTYPE_USER_IPCAM_GET_ENVIRONMENT_RESP      = 0x0363,
** @struct SMsgAVIoctrlGetEnvironmentResp
*/
typedef struct
{
    unsigned int channel;       // Camera Index
    unsigned char mode;         // refer to ENUM_ENVIRONMENT_MODE
    unsigned char reserved[3];
}SMsgAVIoctrlGetEnvironmentResp;


/*
IOTYPE_USER_IPCAM_SET_VIDEOMODE_REQ         = 0x0370,
** @struct SMsgAVIoctrlSetVideoModeReq
*/
typedef struct
{
    unsigned int channel;   // Camera Index
    unsigned char mode;     // refer to ENUM_VIDEO_MODE
    unsigned char reserved[3];
}SMsgAVIoctrlSetVideoModeReq;


/*
IOTYPE_USER_IPCAM_SET_VIDEOMODE_RESP        = 0x0371,
** @struct SMsgAVIoctrlSetVideoModeResp
*/
typedef struct
{
    unsigned int channel;   // Camera Index
    unsigned char result;   // 0: success; otherwise: failed.
    unsigned char reserved[3];
}SMsgAVIoctrlSetVideoModeResp;


/*
IOTYPE_USER_IPCAM_GET_VIDEOMODE_REQ         = 0x0372,
** @struct SMsgAVIoctrlGetVideoModeReq
*/
typedef struct
{
    unsigned int channel;   // Camera Index
    unsigned char reserved[4];
}SMsgAVIoctrlGetVideoModeReq;


/*
IOTYPE_USER_IPCAM_GET_VIDEOMODE_RESP        = 0x0373,
** @struct SMsgAVIoctrlGetVideoModeResp
*/
typedef struct
{
    unsigned int  channel;  // Camera Index
    unsigned char mode;     // refer to ENUM_VIDEO_MODE
    unsigned char reserved[3];
}SMsgAVIoctrlGetVideoModeResp;


/*
/IOTYPE_USER_IPCAM_FORMATEXTSTORAGE_REQ         = 0x0380,
** @struct SMsgAVIoctrlFormatExtStorageReq
*/
typedef struct
{
    unsigned int storage;   // Storage index (ex. sdcard slot = 0, internal flash = 1, ...)
    unsigned char reserved[4];
}SMsgAVIoctrlFormatExtStorageReq;


/*
IOTYPE_USER_IPCAM_FORMATEXTSTORAGE_REQ      = 0x0381,
** @struct SMsgAVIoctrlFormatExtStorageResp
*/
typedef struct
{
    unsigned int  storage;  // Storage index
    unsigned char result;   // 0: success;
                            // -1: format command is not supported.
                            // otherwise: failed.
    unsigned char reserved[3];
}SMsgAVIoctrlFormatExtStorageResp;


typedef struct
{
    unsigned short index;       // the stream index of camera
    unsigned short channel;     // the channel index used in AVAPIs, that is ChID in avServStart2(...,ChID)
    char reserved[4];
}SStreamDef;


/*  IOTYPE_USER_IPCAM_GETSUPPORTSTREAM_REQ          = 0x0328,
 */
typedef struct
{
    unsigned char reserved[4];
}SMsgAVIoctrlGetSupportStreamReq;


/*  IOTYPE_USER_IPCAM_GETSUPPORTSTREAM_RESP         = 0x0329,
 */
typedef struct
{
    unsigned int number;        // the quanity of supported audio&video stream or video stream
    SStreamDef streams[1];
}SMsgAVIoctrlGetSupportStreamResp;


/* IOTYPE_USER_IPCAM_GETAUDIOOUTFORMAT_REQ          = 0x032A, //used to speak. but once camera is connected by App, send this at once.
 */
typedef struct
{
    unsigned int channel;       // camera index
    char reserved[4];
}SMsgAVIoctrlGetAudioOutFormatReq;

/* IOTYPE_USER_IPCAM_GETAUDIOOUTFORMAT_RESP         = 0x032B,
 */
typedef struct
{
    unsigned int channel;       // camera index
    int format;                 // refer to ENUM_CODECID in AVFRAMEINFO.h
    char reserved[4];
}SMsgAVIoctrlGetAudioOutFormatResp;

/* IOTYPE_USER_IPCAM_RECEIVE_FIRST_IFRAME           = 0x1002,
 */
typedef struct
{
    unsigned int channel;       // camera index
    char reserved[4];
}SMsgAVIoctrlReceiveFirstIFrame;

/* IOTYPE_USER_IPCAM_GET_FLOWINFO_REQ              = 0x390
 */
typedef struct
{
    unsigned int channel;           // camera index
    unsigned int collect_interval;  // seconds of interval to collect flow information
                                    // send 0 indicates stop collecting.
}SMsgAVIoctrlGetFlowInfoReq;

/* IOTYPE_USER_IPCAM_GET_FLOWINFO_RESP            = 0x391
 */
typedef struct
{
    unsigned int channel;           // camera index
    unsigned int collect_interval;  // seconds of interval client will collect flow information
}SMsgAVIoctrlGetFlowInfoResp;

/* IOTYPE_USER_IPCAM_CURRENT_FLOWINFO              = 0x392
 */
typedef struct
{
    unsigned int channel;                       // camera index
    unsigned int total_frame_count;             // Total frame count in the specified interval
    unsigned int lost_incomplete_frame_count;   // Total lost and incomplete frame count in the specified interval
    unsigned int total_expected_frame_size;     // Total expected frame size from avRecvFrameData2()
    unsigned int total_actual_frame_size;       // Total actual frame size from avRecvFrameData2()
    unsigned int timestamp_ms;                  // Timestamp in millisecond of this report.
    char reserved[8];
}SMsgAVIoctrlCurrentFlowInfo;

/* IOTYPE_USER_IPCAM_GET_TIMEZONE_REQ               = 0x3A0
 * IOTYPE_USER_IPCAM_GET_TIMEZONE_RESP              = 0x3A1
 * IOTYPE_USER_IPCAM_SET_TIMEZONE_REQ               = 0x3B0
 * IOTYPE_USER_IPCAM_SET_TIMEZONE_RESP              = 0x3B1
 */
typedef struct
{
    int cbSize;                         // the following package size in bytes, should be sizeof(SMsgAVIoctrlTimeZone)
    int nIsSupportTimeZone;
    int nGMTDiff;                       // the difference between GMT in hours
    char szTimeZoneString[256];         // the timezone description string in multi-bytes char format
}SMsgAVIoctrlTimeZone;

/*
// dropbox support
IOTYPE_USER_IPCAM_GET_SAVE_DROPBOX_REQ      = 0x500,
IOTYPE_USER_IPCAM_GET_SAVE_DROPBOX_RESP     = 0x501,
*/
typedef struct
{
    unsigned short nSupportDropbox;     // 0:no support/ 1: support dropbox
    unsigned short nLinked;             // 0:no link/ 1:linked
    char szLinkUDID[64];                // Link UDID for App
}SMsgAVIoctrlGetDropbox;


/*
 // dropbox support
 IOTYPE_USER_IPCAM_SET_SAVE_DROPBOX_REQ      = 0x502,
 IOTYPE_USER_IPCAM_SET_SAVE_DROPBOX_RESP     = 0x503,
 */
typedef struct
{
    unsigned short nLinked;             // 0:no link/ 1:linked
    char szLinkUDID[64];                // UDID for App
    char szAccessToken[128];             // Oauth token
    char szAccessTokenSecret[128];       // Oauth token secret
    char szAppKey[128];                  // App Key (reserved)
    char szSecret[128];                  // Secret  (reserved)
    char reserved[2];
}SMsgAVIoctrlSetDropbox;


// get local ip
//IOTYPE_USER_IPCAM_GET_LOCAL_IP_REQ          = 0x510,
typedef struct
{
    unsigned int channel;                       // camera index
}SMsgAVIoctrlGetLocalIPReq;


//IOTYPE_USER_IPCAM_GET_LOCAL_IP_RESP         = 0x511,
typedef struct
{
    unsigned int channel;                       // camera index
    char szLocalIP[32];                         // device local ip
}SMsgAVIoctrlGetLocalIPResp;


//¦??¨µ¯è ///////////////////////////////////////////////////////////
typedef enum
{
    AVIOCTRL_CRUISEMODE_VERTICAL   =0x00,//¤W¤U¨µ¯è
    AVIOCTRL_CRUISEMODE_HORIZONTAL =0x01,//¥ª¥k¨µ¯è
}ENUM_CRUISE_MODE;
/*IOTYPE_HICHIP_CRUISE_START=0x600
 */
typedef struct
{
    unsigned int channel;      // Camera Index
    unsigned char mode;        // refer to ENUM_CRUISE_MODE
    unsigned char reserved[3];
}SMsgAVIoctrlCruiseStart;
/*IOTYPE_HICHIP_CRUISE_STOP=0x601
 */
typedef struct
{
    unsigned int channel; // Camera Index
    unsigned char reserved[4];
} SMsgAVIoctrlCruiseStop;


//«G«???////////////////////////////////////////////////////////////
/*IOTYPE_HICHIP_GETBRIGHT_REQ=0x602
 */
typedef struct
{
    unsigned int channel; // Camera Index
    unsigned char reserved[4];
} SMsgAVIoctrlGetBrightReq;
/*IOTYPE_HICHIP_GETBRIGHT_RESP=0x603
 IOTYPE_HICHIP_SETBRIGHT_REQ=0x604
 */
typedef struct
{
    unsigned int channel; // Camera Index
    unsigned char bright; // refer to ENUM_BRIGHT_LEVEL
    unsigned char reserved[3];
} SMsgAVIoctrlSetBrightReq, SMgAVIoctrlGetBrightResp;
/* AVIOCTRL BRIGHT Type */
typedef enum
{
    AVIOCTRL_BRIGHT_MAX            = 0x01,
    AVIOCTRL_BRIGHT_HIGH           = 0x02,
    AVIOCTRL_BRIGHT_MIDDLE         = 0x03,
    AVIOCTRL_BRIGHT_LOW            = 0x04,
    AVIOCTRL_BRIGHT_MIN            = 0x05,
}ENUM_BRIGHT_LEVEL;
/*IOTYPE_HICHIP_SETBRIGHT_RESP=0x605
 */
typedef struct
{
    unsigned int result; // 0: success; otherwise: failed.
    unsigned char reserved[4];
} SMsgAVIoctrSeltBrightResp;

//?¤ñ«???////////////////////////////////////////////////////////////
/*IOTYPE_HICHIP_GETCONTRAST_REQ=0x606
 */
typedef struct
{
    unsigned int channel; // Camera Index
    unsigned char reserved[4];
} SMsgAVIoctrlGetContrastReq;
/*IOTYPE_HICHIP_GETCONTRAST_RESP=0x607
 IOTYPE_HICHIP_SETCONTRAST_REQ=0x608
 */
typedef struct
{
    unsigned int channel; // Camera Index
    unsigned char contrast; // refer to ENUM_CONTRAST_LEVEL
    unsigned char reserved[3];
} SMsgAVIoctrlSetContrastReq, SMgAVIoctrlGetContrastResp;
/* AVIOCTRL CONTRAST Type */
typedef enum
{
    AVIOCTRL_CONTRAST_MAX            = 0x01,
    AVIOCTRL_CONTRAST_HIGH           = 0x02,
    AVIOCTRL_CONTRAST_MIDDLE         = 0x03,
    AVIOCTRL_CONTRAST_LOW            = 0x04,
    AVIOCTRL_CONTRAST_MIN            = 0x05,
}ENUM_CONTRAST_LEVEL;
/*IOTYPE_HICHIP_SETCONTRAST_RESP=0x609
 */
typedef struct
{
    unsigned int result; // 0: success; otherwise: failed.
    unsigned char reserved[4];
} SMsgAVIoctrSeltContrastResp;

//?¸m¦ì////////////////////////////////////////////////////////////
typedef struct
{
    unsigned int channel;   // AvServer Index
    unsigned int nPresetIdx;    // ³]©w¦?¹w¸m?Iindex    ¼?­?½d³ò 0~3(ªí¥?¹w¸m?I1~4)

}
SMsgAVIoctrlSetPresetReq,SMsgAVIoctrlGetPresetResp;

/* IOTYPE_USER_IPCAM_SETPRESET_RESP             = 0x441
 */
typedef struct
{
    int result; // ¦^¶?­?   0: success; otherwise: failed
    unsigned char reserved[4];
}SMsgAVIoctrlSetPresetResp;

/* IOTYPE_USER_IPCAM_GETPRESET_REQ              = 0x442
 */
typedef struct
{
    unsigned int channel;   // AvServer Index
    unsigned int nPresetIdx;    // ¸ü¤J¹w¸m?Iindex  ¼?­?½d³ò 0~3(ªí¥?¹w¸m?I1~4)
}SMsgAVIoctrlGetPresetReq;


//??µ¥¬¨¾ºM¨¾??////////////////////////////////////////////////////////////
//??µ?¤ù¤W?FTP??////////////////////////////////////////////////////////////
//??µ?¥ó³qª¾??////////////////////////////////////////////////////////////
/* IOTYPE_USER_IPCAM_GETGUARD_REQ               = 0x420
 */
typedef struct
{
    unsigned int channel;       // AvServer Index
    unsigned char reserved[4];
} SMsgAVIoctrlGetGuardReq;

/* IOTYPE_USER_IPCAM_GETGUARD_RESP              = 0x421
 * IOTYPE_USER_IPCAM_SETGUARD_REQ               = 0x422
 */

typedef struct
{
    unsigned int channel;       // AvServer Index
    unsigned char alarm_motion_armed;        // ²¾????? ON=1, OFF=0
    unsigned char alarm_motion_sensitivity; // 1(MIN) ~ 100(MAX)   ?¦?TUTKªºSMsgAVIoctrlSetMotionDetectReqªºsensitivity ­?©w?°µ¡C
    unsigned char alarm_preset;  /*??µ???¸m¦ì 0¡G??¡A1¡ã4¡G?????­?¦ì*/
    unsigned char alarm_mail;   /*??µ??¥ó³qª¾ 0¡G¸T¤î¡F1¡G¤¹?                   */

    unsigned int alarm_upload_interval;   /*??µ?¤W??¤ùªº?¹j¡]¬í¡^¡A0¡G¸T¤î              */
} SMsgAVIoctrlGetGuardResp, SMsgAVIoctrlSetGuardReq;

/* IOTYPE_USER_IPCAM_SETGUARD_RESP              = 0x423
 */
typedef struct
{
    int result; // ¦^¶?­?   0: success; otherwise: failed.
    unsigned char reserved[4];

} SMsgAVIoctrlSetGuardResp;



//FTP?¸m/////////////////////////////////////////////////////////
typedef struct
{
    unsigned int channel;       // Camera Index
    unsigned char ftpServer[68] ; // 10.1.1.1
    int ftpPort;                   // 21
    unsigned char userName[20];
    unsigned char password[20];
    unsigned char path[68];
    int  passiveMode;// 0 - off, 1 - on
}SMsgAVIoctrlSetFtpReq, SMsgAVIoctrlGetFtpResp;

typedef struct
{
    unsigned int channel;       // Camera Index
    int result; //0: ok ; 1: failed
}SMsgAVIoctrlSetFtpResp;

typedef struct
{
    unsigned int channel;       // Camera Index
    unsigned char reserved[4];
}SMsgAVIoctrlGetFtpReq;


//mail?¸m////////////////////////////////////////////////////////
typedef struct {
    unsigned int channel;       // Camera Index
    unsigned char reserved[4];
} SMsgAVIoctrlExGetSmtpReq;

typedef struct {
    unsigned int channel;       // Camera Index
    char sender[64];        /*?¥óªº?°eª?                                      */
    char receiver1[64];   /*?¥óªº±µ¦¬ª?                                    */
    char server[64];          /*?¥óªA?¾¹¦a§}                                    */
    unsigned int port;  /*?¥óªA?º?¤f                                      */
    unsigned int mail_tls;          /*¬O§_¨?¥?  tls  ????, 0¡G¤£¡F1¡GTLS¡F2¡GSTARTLS*/
    char user[32];     /*?¥óªA?¾¹µn?¥??                                */
    char pwd[32];      /*?¥óªA?¾¹µn?±K?                                */
} SMsgAVIoctrlExSetSmtpReq, SMsgAVIoctrlExGetSmtpResp;

typedef struct
{
    int result; //0: ok ; 1: failed
    unsigned char reserved[4];
} SMsgAVIoctrlExSetSmtpResp;


//============================ Ambarella IO Command ===========================

//¶}±? magic zoom
// IOTYPE_USER_SETAMBCROPSET_REQ (0xFF001004)
typedef struct
{
    unsigned int x;     // x-axis from left
    unsigned int y;     // y-axis from top
    unsigned int width; // the width of cropped area
    unsigned int height; // the height of cropped area
    float        zoomScale;// ¹?¤ù©ñ¤j¤ñ²v
    unsigned char channel; // camera index
    unsigned char reserved[3];
} SMsgAVIoctrlSetAmbCropSetReq;

//IOTYPE_USER_SETAMBCROPSET_RESP (0xFF001005)
typedef struct
{
    unsigned int result;    // ¦^¶?­?   0: success; otherwise: failed.
    unsigned char reserved[4];
} SMsgAVIoctrlSetAmbCropSetResp;



//¨ú±o magic zoom ª¬ºA
//IOTYPE_USER_GETAMBCROPSTATUS_REQ (0xFF001006)
typedef struct
{
    unsigned char channel; // camera index
    unsigned char reserved[3];
} SMsgAVIoctrlGetAmbCropStatusReq;


//IOTYPE_USER_GETAMBCROPSTATUS_RESP (0xFF001007)
typedef struct
{
    unsigned int status; // ¦^¶?­? 1: enable; 0: disanle.
    unsigned int x;     // x-axis from left
    unsigned int y;     // y-axis from top
    unsigned int width; // the width of cropped area
    unsigned int height; // the height of cropped area
    float        zoomScale;// ¹?¤ù©ñ¤j¤ñ²v
    unsigned char reserved[4];
} SMsgAVIoctrlGetAmbCropStatusResp;


//IOTYPE_USER_SETAMBCROP_GETAMBCROPIMAGECHANGE_RESP (0xFF001008)
typedef struct
{
    unsigned int status;    // ¦^¶?­?   1: success; 0: fail.
    unsigned char reserved[4];
} SMsgAVIoctrlGetAmbCropImageChangeReq;

/*
IOTYPE_USER_IPCAM_PUSH_STREAMING_START		= 0x210,
** @struct SMsgAVIoctrlPushStream
*/
typedef struct
{
	unsigned int channel;	// Camera Channel
	char szDeviceUID[20];	// Device UID
	unsigned int  event; 	// Event Type, refer to ENUM_EVENTTYPE
	unsigned char reserved[4];
} SMsgAVIoctrlPushStream;

/*
    // handle NOVATEK wifi commands
    IOTYPE_USER_IPCAM_NOVATEK_WIFICMD_REQ               = 0x7F000000,
    IOTYPE_USER_IPCAM_NOVATEK_WIFICMD_RESP              = 0x7F000001,
 */
typedef struct
{
    unsigned int  channel; // AvServer Index
    unsigned int  nCmdIdentify;    // (Option) a number let app can distinguish the response, 0 (zero) means disable
    unsigned int  nAVChannel4LongReq; // (Option) if large sentdata transfer required when request, 0 (zero) means disable
    unsigned int  nCmdID;  // The same as Command ID value defined in  Doc: Novatek_IM_wifi_command list.xls
    unsigned int  nTotalCount; // The total amount of io command transfer times for this wifi command
    unsigned int  nIndex;    // The index of io command transfer times for this wifi command
    unsigned int  nDataLength; // The valid data length in bytes of the param
    unsigned char param[972];   // The parameter string
} SMsgAVIoctrlWifiCmdReq;


typedef struct
{
    unsigned int  channel; // AvServer Index
    unsigned int  nCmdIdentify;    // (Option) a number let app can distinguish the response        0 (zero) means disable
    unsigned int  nAVChannel4LongResp;    // (Option) if large received data transfer required when response 0 (zero) means disable
    unsigned int  nCmdID;  // The same as Command ID value defined in Doc: Novatek_IM_wifi_command list.xls
    unsigned int  nTotalCount; // The total amount of io command transfer times for this wifi command
    unsigned int  nIndex;  // The index of io command transfer times for this wifi command
    unsigned int  nRespLength; // The valid response length in bytes of the param
    unsigned char response[972];    // The response string
} SMsgAVIoctrlWifiCmdResp;


/*
 IOTYPE_USER_IPCAM_FILELIST_REQ     = 0x031C;
 ** @struct SMsgAVIoctrlFileListReq
 */
typedef struct
{
    unsigned int channel;       // Camera Index
    STimeDay stStartTime;       // Search event from ...
    STimeDay stEndTime;         // ... to (search event)
    unsigned char event;        // event type, refer to ENUM_EVENTTYPE
    unsigned char status;       // 0x00: Recording file exists, Event unreaded
    // 0x01: Recording file exists, Event readed
    // 0x02: No Recording file in the event
    unsigned char type;          // type of file 0:video, 1:photo
    unsigned char reserved[9];
}SMsgAVIoctrlFileListReq;

typedef struct
{
    STimeDay stTime;
    unsigned char event;
    unsigned char status;   // 0x00: Recording file exists, Event unreaded
    // 0x01: Recording file exists, Event readed
    // 0x02: No Recording file in the event
    unsigned char type;      // type of file 0:video, 1:photo
    unsigned char length;     // path's length
    unsigned char FilePath[72];  // file's path
}SAvFile;

/*
 IOTYPE_USER_IPCAM_FILELIST_RESP     = 0x031D;
 ** @struct SMsgAVIoctrlFileListResp
 */
typedef struct
{
    unsigned int  channel;      // Camera Index
    unsigned int  total;        // Total event amount in this search session
    unsigned char index;        // package index, 0,1,2...;
    // because avSendIOCtrl() send package up to 1024 bytes one time, you may want split search results to serveral package to send.
    unsigned char endflag;      // end flag; endFlag = 1 means this package is the last one.
    unsigned char count;        // how much events in this package
    unsigned char type;       // type of file 0:video, 1:photo
    SAvFile stFile[1];      // The first memory address of the events in this package
}SMsgAVIoctrlFileListResp;

/*
 IOTYPE_USER_NOVATEK_SETCLIENTRECVREADY_REQ  = 0x7F000002
 */
typedef struct
{
    unsigned int channel;    // AvServer Index
    unsigned int reserved;

}SMsgAVIoctrlSetClientRecvReadyReq;

/*
 IOTYPE_USER_NOVATEK_SETCLIENTRECVREADY_RESP  = 0x7F000003
 */
typedef struct
{
    unsigned int channel; // AvServer Index
    unsigned int result;  // 0: success ; != 0: failure

} SMsgAVIoctrlSetClientRecvReadyResp;


typedef struct tagNovatekFrmDataHdr {
    unsigned int nDataType;                   // set NOVATEK_FRMDATA_TYPE_01
    unsigned int nFrmIdx;                 // frame no. sequence
    unsigned int nRecordCountInFrame;     // (Option) For some DataType aim to specific data struct to each element.0 (zero) means disable
    unsigned char nIsBeginFrame;            // means this is Begin frame
    unsigned char nIsEndFrame;              // means this is End frame
    unsigned char nIsMultipleRecord;            // (Option) For some DataType aim to specific data structto each element.0 (zero) means disable
    unsigned char resreved;
} SNovatekFrmDataHdr, *LPSNovatekFrmDataHdr;

/*
 IOTYPE_USER_IPCAM_UPLOAD_FILE_REQ  = 0x7F000004
 */
typedef struct
{
    unsigned int channel; // AvServer Index
    unsigned int length;  // path length
    unsigned char path[72]; // file path
    unsigned char purpose;  // 0: for download file, 1: for show photo
    unsigned char reserved[7];
} SMsgAVIoctrlUploadReq;

/*
 IOTYPE_USER_IPCAM_DOWNLOAD_FILE_REQ  = 0x7F000006
 */
typedef struct
{
    unsigned int channel; // AvServer Index
    unsigned int result;  // the channel which avServStart
} SMsgAVIoctrlDownloadReq;

/*
 IOTYPE_USER_IPCAM_UPLOAD_FILE_RESP  = 0x7F000005
 */
typedef struct
{
    unsigned int channel; // AvServer Index
    unsigned int result;  // the channel which avServStart
    unsigned char purpose;
    unsigned char reserved[7];
} SMsgAVIoctrlUploadResp;

//#NT#2015/06/09#Philex Lin - Begin
// add query command in liveview
/*
 IOTYPE_USER_IPCAM_LIVEVIEW_OPTIONS_REQ      = 0x03C0
 IOTYPE_USER_IPCAM_GET_BATTERY_LEVEL_REQ     = 0x03C2
 */
typedef struct
{
    unsigned int channel;           // AvServer Index
    unsigned char reversed[4];

} SMsgAVIoctrlLiveviewOptionsReq,SMsgAVIoctrlGetBatteryLevelReq;

/*
 IOTYPE_USER_IPCAM_LIVEVIEW_OPTIONS_RESP     = 0x03C1
 */
typedef struct
{
    unsigned int channel;           // AvServer Index
    unsigned char quality;          // reference by 0x0323
    unsigned char envMode;          // reference by 0x0363
    unsigned char contrast;         // reference by 0x607
    unsigned char brightness;       // reference by 0x063
    unsigned char videoMode;        // reference by 0x373
    unsigned char battery;          // reference by custom=1&cmd=3019
    unsigned char wdr;              // reference by custom=1&cmd=2020
    unsigned char timelapse;        // reference by custom=1&cmd=2020
    unsigned char reversed[4];
} SMsgAVIoctrlLiveviewOptionsResp;

/*
 IOTYPE_USER_IPCAM_GET_BATTERY_LEVEL_RESP     = 0x03C3
 */
typedef struct
{
    unsigned int channel;           // AvServer Index
    unsigned char battery;          // reference by custom=1&cmd=3019
    unsigned char reversed[3];
} SMsgAVIoctrlGetBatteryLevelResp;

//#NT#2015/06/09#Philex Lin - End
#endif
