
#ifndef _UIFLOWMOVIEFUNCS_H
#define _UIFLOWMOVIEFUNCS_H
#include "UIFlow.h"
#include "Type.h"

//------------------------------------------------------------
#define _UIRECAVI_DBG_MSG_              0
#if _UIRECAVI_DBG_MSG_
    #include "Debug.h"
    #define DbgMsg_UIRecAvi(msg)        debug_msg msg
    #define DbgCode_UIRecAvi(x)         x
#else
    #define DbgMsg_UIRecAvi(msg)
    #define DbgCode_UIRecAvi(x)
#endif

#define UIFlowWndMovie_Restart_Rec      0x5a5a

enum _MOV_TASK_STATE {
    MOV_ST_IDLE = 0,        // idle mode
    MOV_ST_MENU ,            // menu mode
    MOV_ST_WARNING_MENU ,
    MOV_ST_QMENU ,           // quick menu state
    MOV_ST_QDMENU,           // quick menu state
    MOV_ST_MENU_SCENE ,           // quick menu state
    MOV_ST_MENU_EXPOSURE,           // quick menu state
    MOV_ST_MENU_VIVILINK,           // quick menu state
    MOV_ST_VIEW ,            // preview mode
    MOV_ST_VIEW_WAIT ,
    MOV_ST_REC ,             // record movie
    MOV_ST_REC_WAIT ,             // record movie
    MOV_ST_PAUSE ,           // pause recording
    MOV_ST_SHUTTER_1 ,        // shutter 1 key pressed state
    MOV_ST_ZOOM=0x80  ,      // zoom state (using MSB, because it could be combined with VIEW/REC state)
};

typedef struct _MOV_TASK_DATA {
    UINT8   State;          // current state
    UINT8   err;            // error status
    UINT16  TimerCount;     // timer count (per 100 ms)
    UINT16  SysTimeCount;   // System time count (per 1s)
    UINT16  MsgCount;       // message display count (per 100 ms)
    BOOL    bEnterMenu;     // enter menu confirm flag
} MOV_TASK_DATA;

#if _ADAS_FUNC_
typedef enum _STOPNGO_WARNMSG_OSD
{
    STOPNGO_NONE_OSD = 0,          ///<
    STOPNGO_GO_OSD = 1,            ///<
    STOPNGO_MOVING_OSD = 2,
    STOPNGO_STOP_OSD = 3,
    STOPNGO_LD_OVER_OSD = 4,
    STOPNGO_LD_LESS_OSD = 5,
    STOPNGO_MV_VALID_OSD = 6,
    STOPNGO_MV_INVALID_OSD = 7
}STOPNGO_WARNMSG_OSD;

typedef struct _ADAS_OSD_COORDINATE{
    UINT32 uiROIx1;
    UINT32 uiROIy1;
    UINT32 uiROIx2;
    UINT32 uiROIy2;
    UINT32 uiROIx3;
    UINT32 uiROIy3;
    UINT32 uiROIx4;
    UINT32 uiROIy4;
    UINT32 uiVPx1;       //Vanishing Point
    UINT32 uiVPy1;       //Vanishing Point
    UINT32 uiVPx2;       //Vanishing Point
    UINT32 uiVPy2;       //Vanishing Point
    UINT32 uiVPNewx1;       //Vanishing Point
    UINT32 uiVPNewy1;       //Vanishing Point
    UINT32 uiVPNewx2;       //Vanishing Point
    UINT32 uiVPNewy2;       //Vanishing Point
    UINT32 uiVPDefx1;       //Vanishing Point
    UINT32 uiVPDefy1;       //Vanishing Point
    UINT32 uiVPDefx2;       //Vanishing Point
    UINT32 uiVPDefy2;       //Vanishing Point
    UINT32 uiLaneLx1;
    UINT32 uiLaneLy1;
    UINT32 uiLaneLx2;
    UINT32 uiLaneLy2;
    UINT32 uiLaneRx1;
    UINT32 uiLaneRy1;
    UINT32 uiLaneRx2;
    UINT32 uiLaneRy2;
    UINT32 uiCarLx1;
    UINT32 uiCarLy1;
    UINT32 uiCarLx2;
    UINT32 uiCarLy2;
    UINT32 uiCarRx1;
    UINT32 uiCarRy1;
    UINT32 uiCarRx2;
    UINT32 uiCarRy2;
    UINT32 AutoVPRsltX;
    UINT32 AutoVPRsltY;
    UINT32 AutoVPLinex1[4];
    UINT32 AutoVPLiney1[4];
    UINT32 AutoVPLinex2[4];
    UINT32 AutoVPLiney2[4];
    UINT32 DetectRangx1;
    UINT32 DetectRangy1;
    UINT32 DetectRangx2;
    UINT32 DetectRangy2;
    UINT32 DetectRangx3;
    UINT32 DetectRangy3;
    UINT32 DetectRangx4;
    UINT32 DetectRangy4;
    UINT32 SnGRoi1x1;
    UINT32 SnGRoi1y1;
    UINT32 SnGRoi1x2;
    UINT32 SnGRoi1y2;
    UINT32 SnGRoi1x3;
    UINT32 SnGRoi1y3;
    UINT32 SnGRoi1x4;
    UINT32 SnGRoi1y4;
    UINT32 SnGRoi2X1;
    UINT32 SnGRoi2Y1;
    UINT32 SnGRoi2X4;
    UINT32 SnGRoi2Y4;
    STOPNGO_WARNMSG_OSD uiStopStatus;             ///<
    STOPNGO_WARNMSG_OSD uiGoStatus;               ///<
    STOPNGO_WARNMSG_OSD uiMovingStatus;           ///<
    STOPNGO_WARNMSG_OSD uiLdStatus;
    STOPNGO_WARNMSG_OSD uiMvValidFlag;
}ADAS_OSD_COORDINATE, ADAS_COORDINATE;
#endif

extern MOV_TASK_DATA gMovData;

extern void   FlowMovie_SetVivilinkData(UINT32 uiVivilinkData);
extern UINT8  FlowMovie_GetVivilinkData(void);
extern UINT8  FlowMovie_GetMovDataState(void);
extern void   FlowMovie_StartRec(void);
extern void   FlowMovie_StopRec(void);
extern void   FlowMovie_StopRecSelfTimer(void);
extern UINT8  FlowMovie_GetDataState(void);
extern void   FlowMovie_SetRecMaxTime(UINT32 RecMaxTime);
extern UINT32 FlowMovie_GetRecMaxTime(void);
extern void   FlowMovie_SetRecCurrTime(UINT32 RecCurrTime);
extern UINT32 FlowMovie_GetRecCurrTime(void);
extern UINT32 FlowMovie_GetSelfTimerID(void);
extern BOOL   FlowMovie_IsStorageErr(BOOL IsCheckFull);
extern void   FlowMovie_GetPictureInMovie(void);
#if _ADAS_FUNC_
//#NT#2016/06/01#KCHong -begin
//#NT#Change 1st param to pointer type and add 3rd paramter due to FlowMovie_AdasProcess() need to distinguish differnt CB event.
//extern void   FlowMovie_AdasProcess(ADAS_OSD_COORDINATE AdasOsdCoord, ADAS_APPS_RESULT_INFO *pAdasInfo);
extern void   FlowMovie_AdasProcess(ADAS_OSD_COORDINATE* pAdasOsdCoord, ADAS_APPS_RESULT_INFO *pAdasInfo, UINT32 EvtType);
//#NT#2016/06/01#KCHong -end
extern UINT32 FlowMovie_GetAdasOsdIno(void);
#endif
extern BOOL   FlowMovie_ChkDrawStoreFullFolderFull(void);

extern void   FlowMovie_OnTimer1SecIndex(void);

//Callback function
extern void FlowMovie_RawEncCB(UINT32 uiEventID);
#endif //_UIFLOWMOVIEFUNCS_H
