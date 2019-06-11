#include "IPL_Cal_KeyStrSample_Int.h"
#include "UIFramework.h"

#if isDemokit
#include "UIGraphics.h"
#include "UIDisplay.h"
#include "UICommon.h"
#else
#include "GxDC.h"
#include "GxGfx.h"
#include "UIResource.h"

#define GxGfx_OSD 0
#endif

#include "GxInput.h"
#include "KeyDef.h"
#include "Delay.h"
#include "GxVideo.h"
#include "GxDisplay.h"
#include "stdio.h"
static URECT m_Rect = {16,16,320,24};

static BVALUE gStringColor = CLRID_IDX_GREEN;

static void Cal_SetStringColor(UINT32 StrColor)
{
    switch(StrColor)
    {
        case UICOL_RED:
            gStringColor = CLRID_IDX_RED;
        break;

        case UICOL_GREEN:
            gStringColor = CLRID_IDX_GREEN;
        break;

        case UICOL_BLUE:
            gStringColor = CLRID_IDX_BLUE;
        break;

        case UICOL_WHITE:
            gStringColor = CLRID_IDX_WHITE;
        break;

        case UICOL_BLACK:
            gStringColor = CLRID_IDX_BLACK;
        break;

        case UICOL_GRAY:
            gStringColor = CLRID_IDX_BLACK50;
        break;

        case UICOL_YELLOW:
            gStringColor = CLRID_IDX_YELLOW;
        break;

        default:
            gStringColor = CLRID_IDX_RED;
        break;
    }
}

static void Cal_ShowString(CHAR *pStr, PURECT pRect, BOOL bClear)
{
    #if isDemokit
    DC**     pCalDCList;
    UI_SetDisplayDirty(TRUE); //set TRUE to force dirty current begin/end

    pCalDCList = (DC**)UI_BeginScreen();

    if(bClear)//clr buf
    {
        GxGfx_SetTextColor(_OSD_INDEX_TRANSPART, _OSD_INDEX_TRANSPART, 0);
        GxGfx_FillRect(pCalDCList[GxGfx_OSD],pRect->x,pRect->y,pRect->x+pRect->w,pRect->y+pRect->h);
    }
    GxGfx_SetShapeColor(_OSD_INDEX_TRANSPART, _OSD_INDEX_TRANSPART, 0);

    GxGfx_SetTextStroke((const FONT*)gDemoKit_Font, FONTSTYLE_NORMAL, SCALE_1X);
    GxGfx_SetTextColor(gStringColor, CLRID_IDX_BLACK50, 0);
    GxGfx_Text(pCalDCList[GxGfx_OSD], pRect->x, pRect->y, pStr);
    UI_EndScreen((UINT32)pCalDCList);
    #endif
}

static void Cal_ClrOSD(void)
{
    ISIZE disp_size;
    URECT Rect;

    Rect.x = Rect.y = 0;
    disp_size = GxVideo_GetDeviceSize(DOUT1);
    Rect.w = disp_size.w;
    Rect.h = disp_size.h;
    Cal_ShowString(NULL,&Rect,TRUE);
}

static void Cal_WaitKey(UINT32 Key)
{
    #if isDemokit
    Input_SetKeyMask(KEY_PRESS, FLGKEY_KEY_MASK_NULL);

    while(1)
    {
        if(GxKey_GetData(GXKEY_NORMAL_KEY) & Key)
            break;
        else
            Delay_DelayMs(33);
    }
    //incase of ui to detect key pressed when key pressed too long.
    //but key still can't be pressed over 2 sec.
    Delay_DelayMs(2000);
    Input_SetKeyMask(KEY_PRESS, FLGKEY_KEY_MASK_DEFAULT);
    #endif
}

static void Null_UI(CalUICB UICB,CHAR* a,UINT32 StrColor)
{
    return;
}

UINT32 msg_cnt = 0;
static void AWB_UI(CalUICB UICB,CHAR* pstr,UINT32 StrColor)
{
    URECT Rect = m_Rect;
    CHAR str[32];
    UINT32 buf_cnt = 0;

    Cal_SetStringColor(StrColor);

    switch(UICB)
    {
        // In the first line print string
        case UICB_START:
            buf_cnt = snprintf(NULL, 0, "                             ");
            snprintf((char*)str, buf_cnt, "                             ");
            if(pstr)
            {
                Rect.y = 0;
                Cal_ShowString(pstr,&Rect,TRUE);
            }
            break;

        // wait any key
        case UICB_WAITKEY:
            buf_cnt = snprintf(NULL, 0, "PRESS OK KEY to continue");
            snprintf((char*)str, buf_cnt+1, "PRESS OK KEY to continue");
            Rect.y = (msg_cnt+1)*30;
            Cal_ShowString(str,&Rect,TRUE);
            Cal_WaitKey(FLGKEY_KEY_MASK);
            buf_cnt = snprintf(NULL, 0, "                             ");
            snprintf((char*)str, buf_cnt, "                             ");
            Cal_ShowString(str,&Rect,TRUE);
            break;

        // On the next line print string
        case UICB_PROCESSING:
            if(pstr)
            {
                Rect.y = (msg_cnt+1)*30;
                msg_cnt++;
                Cal_ShowString(pstr,&Rect,TRUE);
            }
            break;

        // clear all string
        case UICB_CLEAN:
            Cal_ClrOSD();
            msg_cnt = 0;
            break;

        // In the last line of the print string
        case UICB_END:
            if(pstr)
            {
                Rect.y = 200;
                Cal_ShowString(pstr,&Rect,TRUE);
            }
            break;

        // clean the last message
        case UICB_DELETE:
            Rect.y = (msg_cnt)*30;
            buf_cnt = snprintf(NULL, 0, "                             ");
            snprintf((char*)str, buf_cnt, "                             ");
            Cal_ShowString(str,&Rect,TRUE);
            msg_cnt--;
            break;

        case UICB_NONE:
        default:
            break;
    }
}

static void ISO_UI(CalUICB UICB,CHAR* pstr,UINT32 StrColor)
{
    AWB_UI(UICB,pstr,StrColor);
}

static void ECS_UI(CalUICB UICB,CHAR* pstr,UINT32 StrColor)
{
    AWB_UI(UICB,pstr,StrColor);
}

static void DP_B_UI(CalUICB UICB,CHAR* pstr,UINT32 StrColor)
{
    AWB_UI(UICB,pstr,StrColor);
}

static void DP_W_UI(CalUICB UICB,CHAR* pstr,UINT32 StrColor)
{
    DP_B_UI(UICB,pstr,StrColor);
}

static void LENSCEN_UI(CalUICB UICB,CHAR* pstr,UINT32 StrColor)
{
    AWB_UI(UICB,pstr,StrColor);
}

CAL_UICB_OBJ Cal_UI = {

    AWB_UI,
    ISO_UI,
    ECS_UI,
    Null_UI,
    DP_W_UI,
    DP_B_UI,
    LENSCEN_UI,
};



pCAL_UICB_OBJ CAL_UICB(void)
{
    return &Cal_UI;

}
