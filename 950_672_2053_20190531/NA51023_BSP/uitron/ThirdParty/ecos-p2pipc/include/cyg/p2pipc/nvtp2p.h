#ifndef CYGONCE_CYG_P2P_NVT_H
#define CYGONCE_CYG_P2P_NVT_H

//-------------------------------------------------------------------------
//NvtP2PECOS API return value
#define NVTP2P_RET_OK           0
#define NVTP2P_RET_OPENED       1
#define NVTP2P_RET_ERR          (-1)
#define NVTP2P_RET_NO_FUNC      (-2)

#define P2PIPC_BUF_CHK_SIZE    (4)
#define P2PIPC_BUF_TAG         MAKEFOURCC('P','2','P','2')

//-------------------------------------------------------------------------
//IPC command id and parameters
typedef enum{
    NVTP2P_CMDID_INVALID_APP = 0x00000000,
    NVTP2P_CMDID_SERVER_START,
    NVTP2P_CMDID_SERVER_STOP,
    NVTP2P_CMDID_REQUEST_APP,
    NVTP2P_CMDID_NOTIFY_APP,
    NVTP2P_CMDID_STREAM_APP,
    NVTP2P_CMDID_SYSREQ_ACK_APP,
    NVTP2P_CMDID_UNINIT,
}NVTP2P_APP_CMDID;

typedef enum{
    NVTP2P_CMDID_INVALID_DEVICE = 0x00000000,
    NVTP2P_CMDID_REQUEST_DEVICE,
    NVTP2P_CMDID_NOTIFY_DEVICE,
    NVTP2P_CMDID_STREAM_DEVICE,
    NVTP2P_CMDID_SYSREQ_ACK_DEVICE,
}NVTP2P_DEVICE_CMDID;

typedef enum {
	// Request Device id
    NVTP2P_REQID_DEV_WIFICMD = 0x00000001,
    NVTP2P_REQID_DEV_CUSTOM,
	// Request APP id
    NVTP2P_REQID_APP_SLEEPINFO,
} NVTP2P_REQID;

typedef enum {
    NVTP2P_NTFID_DEV_ONLINE_NUM = 0x00000001,
	NVTP2P_NTFID_DEV_LOGGED_IN,
	NVTP2P_NTFID_DEV_SET_SLEEP,
	NVTP2P_NTFID_DEV_OTA_END, //0: success, 1: fail
    NVTP2P_NTFID_APP_TEMP,
} NVTP2P_NTFID;

typedef struct{
    NVTP2P_APP_CMDID CmdId;
    unsigned int     Arg;
    int              SubCmd;
    int              length;
}NVTP2P_APP_MSG;

typedef struct{
    NVTP2P_DEVICE_CMDID CmdId;
    unsigned int        Arg;
    int              SubCmd;
    int              length;
}NVTP2P_DEVICE_MSG;

typedef struct {
    unsigned long SPSPPS_Addr;
    unsigned long SPSPPS_Size;
} NVTP2P_STREAM_EXT_V;

typedef struct {
    unsigned long SampleRate;
    unsigned long CodecType; //PCM(0x1), AAC(0x2)
} NVTP2P_STREAM_EXT_A;

typedef struct {
	int bIsRecPlay;
    int bIsAudio;
    int ChanId;
    unsigned long Addr;
    unsigned long Size;
    unsigned long TimeStamp;
    unsigned long SVCLayerId;
    union {
        NVTP2P_STREAM_EXT_V ext_v;
        NVTP2P_STREAM_EXT_A ext_a;
    };
} NVTP2P_STREAM_APP_S;

typedef struct {
    char* pframeinfo;
	int frm_no;
} NVTP2P_STREAM_DEV_EXT_A;

typedef struct {
    char* pframeinfo;
	int frm_no;
} NVTP2P_STREAM_DEV_EXT_V;

typedef struct {
    char *pdata;
	int size;
	union {
	NVTP2P_STREAM_DEV_EXT_V ext_v;
	NVTP2P_STREAM_DEV_EXT_A ext_a;
	};
}NVTP2P_STREAM_DEV_S;

//structure definitions of request ids
typedef struct {
    unsigned int  buf_addr;
    unsigned int  buf_size;
    unsigned int  ret_datacnt;
}NVTP2P_REQID_APP_SLEEP_INFO_S;

//callback id
typedef enum{
    _NVT_P2P_EVENT_SERVER_CONNECTED  =  0x00000000,
    _NVT_P2P_EVENT_SERVER_DISCONNECTED,
    _NVT_P2P_EVENT_SERVER_ERROR,
    _NVT_P2P_EVENT_CLIENT_CONNECTED,
    _NVT_P2P_EVENT_CLIENT_DISCONNECTED,
    _NVT_P2P_EVENT_CLIENT_ERROR,
    _NVT_P2P_EVENT_DATA_ERROR,
    _NVT_P2P_EVENT_NOTIFY_ERROR,
    _NVT_P2P_EVENT_VENDOR_SPECIFIC,
}_NVTP2P_EVENTID;

typedef struct{
    _NVTP2P_EVENTID evtId;
    unsigned int Arg;
}_NVTP2P_EVENTID_MSG;

#endif // CYGONCE_CYG_P2P_NVT_H
// End of nvtp2p.h
