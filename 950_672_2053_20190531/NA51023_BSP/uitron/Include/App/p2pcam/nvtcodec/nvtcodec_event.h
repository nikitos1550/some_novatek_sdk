#ifndef _NVTCODEC_EVENT_H
#define _NVTCODEC_EVENT_H
/*
    Copyright (c) 2017~  Novatek Microelectronics Corporation

    @file nvtcodec_event.h

    @version

    @date
*/
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


#define IPCAM_EVENT_FD	0x01
#define IPCAM_EVENT_MD	0x02
#define IPCAM_EVENT_VQA 0x03
#define IPCAM_EVENT_PD 	0x04
#define IPCAM_EVENT_MD_SAMPLE_660 0x05
#define IPCAM_EVENT_BC  0x06
#define IPCAM_EVENT_FTG  0x07

#define	CUSTOM_MD_MAX_SUB_REGION_NUM 4
#define	CUSTOM_MD_MAX_OBJ_NUM 128
#define CUSTOM_MD_USR_MAX_EVT_SIZE 10800
#define FD_NUM_MAX	36
#define PD_MAX_NUM	32
#define BC_MAX_NUM	1024
#define SET_FLAG(flag, val)     flag|=(1<<(val))
#define CLEAR_FLAG(flag, val)   flag&=(~(1<<(val)))
#define IS_FLAG(flag, val)      (((flag)&(1<<(val)))!=0)
#define NVT_FTG_NUM_MAX					(36)
#define NVT_FTG_MAX_LANDMARK			(68)       ///< max landmark detection number
#define CUSTOM_BC_USR_MAX_EVT_SIZE 120*90
#define CUSTOM_BC_Y_DATA_SIZE 2048//320*240

typedef struct
{
	long	tv_sec;		/* seconds */
	long	tv_usec;	/* microseconds */
} TIME_VAL ;

typedef struct {
	UINT32  id;     ///< id
	UINT32  x;      ///< X coordinate of the top-left point of the rectangle
	UINT32  y;      ///< Y coordinate of the top-left point of the rectangle
	UINT32  w;      ///< Rectangle width
	UINT32  h;      ///< Rectangle height
} EVENT_URECT, *EVENT_PURECT;


typedef struct {
	char Event;
	TIME_VAL TimeStamp;
	UINT32 uiFaceNumber;
	EVENT_URECT info[FD_NUM_MAX];
} IPCAM_FD_INFO;

typedef struct {
	INT32 start_x;
	INT32 start_y;
	INT32 end_x;
	INT32 end_y;
	INT32 label;
	UINT32 reserved[6];
} USR_OBJ_T;

typedef struct {
	UINT32 motion_alarm;        ///< alarm
	UINT32 in_num;
	UINT32 out_num;
	UINT32 obj_num;
	UINT8 status;
	//UINT32 cross_decision;       ///< cross decision info
	UINT32 sub_motion_alarm[CUSTOM_MD_MAX_SUB_REGION_NUM]; ///< sub alarm
	USR_OBJ_T obj[CUSTOM_MD_MAX_OBJ_NUM]; ///< obj info
	UINT8 evt_map_addr[CUSTOM_MD_USR_MAX_EVT_SIZE];        ///< evt map
	UINT32 tp_result;
	UINT32 reserved[5];
} USR_RST;

typedef struct {
	char Event;
	TIME_VAL TimeStamp;
	USR_RST md_reslt;
} IPCAM_MD_RESULT;

typedef struct {
	char Event;
	TIME_VAL TimeStamp;
	BOOL TooLight;
	BOOL TooDark;
	BOOL Blur;
} IPCAM_VQA_RESULT;

typedef struct
{
	UINT32 people_counts;
	EVENT_URECT target[PD_MAX_NUM];
	EVENT_URECT targetcoord;
} PD_RESULT;

typedef struct
{
	char Event;
	TIME_VAL TimeStamp;
	PD_RESULT pd_result;
} IPCAM_PD_RESULT;

typedef struct{
	char Event;
	BOOL bTrigger;
}IPCAM_MD_SAMPLE_660_STATUS;

typedef struct {
	UINT32 targetnum;
	EVENT_URECT target[BC_MAX_NUM];
	UINT32 evt_width;
	UINT32 evt_height;
	UINT8 evt_map_addr[CUSTOM_BC_USR_MAX_EVT_SIZE];

} BC_RESULT;

typedef struct {
	char event;
	TIME_VAL TimeStamp;
	BC_RESULT result;
} IPCAM_BC_RESULT;

typedef struct
{
	// FTG location
    UINT32  x;								///< result x
    UINT32  y;								///< result y
    UINT32  w;								///< result w
    UINT32  h;								///< result h
    UINT32  id;								///< result id
    UINT32  pose;						    ///< result pose 0:front view 1:right side 2:left side
    // FTG clarity
    UINT16  luma;							///< result face average luma range [0, 255]
	UINT16  clarity;						///< result face average clarity range [0, 255]
	UINT16  contrast;						///< result face average contrast range [0, 255]
	// FTG landmark
	INT32   lstate;							///< if landmark success is 1, failed is -1;
	INT32   yaw;							///< result face yaw angle
	INT32	pitch;							///< result face pitch angle
	INT32	roll;							///< result face roll angle
	INT32	lscore;							///< result face landmark score range [-1024 1024]
	INT32   lnumber;						///< result face landmark number
	INT16	lpoints[NVT_FTG_MAX_LANDMARK*2];///< result face landmark data[x1,y1,x2,y2,...xn,yn]
} FTG_RESULT;

typedef struct
{
	char Event;
	TIME_VAL TimeStamp;
    UINT8  uiHasLandMark;
	UINT32 uiFaceNumber;
	FTG_RESULT info[10];
} IPCAM_FTG_RESULT;

#ifdef __cplusplus
}
#endif
/* ----------------------------------------------------------------- */
#endif /* _NVTCODEC_EVENT_H  */
