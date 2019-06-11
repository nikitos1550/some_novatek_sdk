#include <stdio.h>
#include "UIFlow.h"


#define CUSTOM_PHOTO_MENU     0   // set 1 if you want to use custom menu

// --------------------------------------------------------------------------
// OPTIONS
// --------------------------------------------------------------------------
// Photo capture Options
TMDEF_BEGIN_OPTIONS(SELFTIMER)
    TMDEF_OPTION_TEXT(SELFTIMER_SINGLE)
    TMDEF_OPTION_TEXT(SELFTIMER_2SEC)
    TMDEF_OPTION_TEXT(SELFTIMER_5SEC)
    TMDEF_OPTION_TEXT(SELFTIMER_10SEC)
TMDEF_END_OPTIONS()

// Photo Size Options
TMDEF_BEGIN_OPTIONS(PHOTO_SIZE)
    TMDEF_OPTION_TEXT(PHOTO_SIZE_12M)
    TMDEF_OPTION_TEXT(PHOTO_SIZE_10M)
    TMDEF_OPTION_TEXT(PHOTO_SIZE_8M)
    TMDEF_OPTION_TEXT(PHOTO_SIZE_5M)
    TMDEF_OPTION_TEXT(PHOTO_SIZE_3M)
    TMDEF_OPTION_TEXT(PHOTO_SIZE_2MHD)
    TMDEF_OPTION_TEXT(PHOTO_SIZE_VGA)
    TMDEF_OPTION_TEXT(PHOTO_SIZE_1M)
TMDEF_END_OPTIONS()

// Continue shot Options
TMDEF_BEGIN_OPTIONS(CONTINUE_SHOT)
    TMDEF_OPTION_TEXT(CONTINUE_SHOT_OFF)
    TMDEF_OPTION_TEXT(CONTINUE_SHOT_ON)
TMDEF_END_OPTIONS()


// Photo Quality Options
TMDEF_BEGIN_OPTIONS(QUALITY)
    TMDEF_OPTION_TEXT(QUALITY_FINE)
    TMDEF_OPTION_TEXT(QUALITY_NORMAL)
    TMDEF_OPTION_TEXT(QUALITY_ECONOMY)
TMDEF_END_OPTIONS()

// ISO Options
TMDEF_BEGIN_OPTIONS(ISO)
    TMDEF_OPTION_TEXT(ISO_AUTO)
    TMDEF_OPTION_TEXT(ISO_100)
    TMDEF_OPTION_TEXT(ISO_200)
    TMDEF_OPTION_TEXT(ISO_400)
TMDEF_END_OPTIONS()

// Sharpness Options
TMDEF_BEGIN_OPTIONS(SHARPNESS)
    TMDEF_OPTION_TEXT(SHARP_STRONG)
    TMDEF_OPTION_TEXT(SHARP_NORMAL)
    TMDEF_OPTION_TEXT(SHARP_SOFT)
TMDEF_END_OPTIONS()

// WB Options
TMDEF_BEGIN_OPTIONS(WB)
    TMDEF_OPTION_TEXT(WB_AUTO)
    TMDEF_OPTION_TEXT(WB_DAYLIGHT)
    TMDEF_OPTION_TEXT(WB_CLOUDY)
    TMDEF_OPTION_TEXT(WB_TUNGSTEN)
    TMDEF_OPTION_TEXT(WB_FLUORESCENT)
TMDEF_END_OPTIONS()

// Color Options
TMDEF_BEGIN_OPTIONS(COLOR_EFFECT)
    TMDEF_OPTION_TEXT(COLOR_EFFECT_STANDARD)
    TMDEF_OPTION_TEXT(COLOR_EFFECT_MONOCHROME)
    TMDEF_OPTION_TEXT(COLOR_EFFECT_SEPIA)
TMDEF_END_OPTIONS()

// EV Options
TMDEF_BEGIN_OPTIONS(EV)
    TMDEF_OPTION_TEXT(EV_P20)
    TMDEF_OPTION_TEXT(EV_P16)
    TMDEF_OPTION_TEXT(EV_P13)
    TMDEF_OPTION_TEXT(EV_P10)
    TMDEF_OPTION_TEXT(EV_P06)
    TMDEF_OPTION_TEXT(EV_P03)
    TMDEF_OPTION_TEXT(EV_00)
    TMDEF_OPTION_TEXT(EV_N03)
    TMDEF_OPTION_TEXT(EV_N06)
    TMDEF_OPTION_TEXT(EV_N10)
    TMDEF_OPTION_TEXT(EV_N13)
    TMDEF_OPTION_TEXT(EV_N16)
    TMDEF_OPTION_TEXT(EV_N20)
TMDEF_END_OPTIONS()

// Face Detection Options
TMDEF_BEGIN_OPTIONS(FD)
    TMDEF_OPTION_TEXT(FD_OFF)
    TMDEF_OPTION_TEXT(FD_ON)
    TMDEF_OPTION_TEXT(FD_SMILE)
TMDEF_END_OPTIONS()

// Anti-shaking Options
TMDEF_BEGIN_OPTIONS(ANTISHAKE)
    TMDEF_OPTION_TEXT(ANTISHAKE_OFF)
    TMDEF_OPTION_TEXT(ANTISHAKE_ON)
TMDEF_END_OPTIONS()


// Quick Review Options
TMDEF_BEGIN_OPTIONS(QUICK_REVIEW)
    TMDEF_OPTION_TEXT(QV_OFF)
    TMDEF_OPTION_TEXT(QV_2SEC)
    TMDEF_OPTION_TEXT(QV_5SEC)
TMDEF_END_OPTIONS()

// Date Stamp Options
TMDEF_BEGIN_OPTIONS(DATE_STAMP)
    TMDEF_OPTION_TEXT(DATE_STAMP_OFF)
    TMDEF_OPTION_TEXT(DATE_STAMP_DATE)
    TMDEF_OPTION_TEXT(DATE_STAMP_DATETIME)
TMDEF_END_OPTIONS()

// --------------------------------------------------------------------------
// ITEMS
// --------------------------------------------------------------------------
#if CUSTOM_PHOTO_MENU
static int MenuCustom_PhotoEV(UINT32 uiMessage, UINT32 uiParam)
{
    Ux_OpenWindow(&UIMenuWndPhotoExposureCtrl, 0);
    return TMF_PROCESSED;
}

static int MenuCustom_PhotoWB(UINT32 uiMessage, UINT32 uiParam)
{
    Ux_OpenWindow(&UIMenuWndPhotoWBCtrl, 0);
    return TMF_PROCESSED;
}

static int MenuCustom_PhotoColor(UINT32 uiMessage, UINT32 uiParam)
{
    Ux_OpenWindow(&UIMenuWndPhotoColorCtrl, 0);
    return TMF_PROCESSED;
}
#endif

// Photo Menu Items
TMDEF_BEGIN_ITEMS(PHOTO)
    TMDEF_ITEM_TEXTID(SELFTIMER)
    TMDEF_ITEM_TEXTID(PHOTO_SIZE)
    TMDEF_ITEM_TEXTID(CONTINUE_SHOT)
    TMDEF_ITEM_TEXTID(QUALITY)
    TMDEF_ITEM_TEXTID(SHARPNESS)
    TMDEF_ITEM_TEXTID(WB)
    TMDEF_ITEM_TEXTID(COLOR_EFFECT)
    TMDEF_ITEM_TEXTID(ISO)
    TMDEF_ITEM_TEXTID(EV)
//    TMDEF_ITEM_TEXTID(FD)
    TMDEF_ITEM_TEXTID(ANTISHAKE)
    TMDEF_ITEM_TEXTID(QUICK_REVIEW)
    TMDEF_ITEM_TEXTID(DATE_STAMP)
TMDEF_END_ITEMS()

// --------------------------------------------------------------------------
// PAGES
// --------------------------------------------------------------------------
// Photo Menu Pages
TMDEF_BEGIN_PAGES(PHOTO)
    TMDEF_PAGE_TEXT_ICON(PHOTO)
    TMDEF_PAGE_TEXT_ICON(PHOTO) //reserved for setup menu,replace dynamic in MenuCommonItem
TMDEF_END_PAGES()

TMDEF_EMNU(gPhotoMenu, PHOTO, Photo_MenuCallback)

// --------------------------------------------------------------------------
// Menu Callback
// --------------------------------------------------------------------------
int Photo_MenuCallback(UINT32 uiMessage, UINT32 uiParam)
{
    UINT16  uwItemId;
    UINT16  uwOption;

    if (uiMessage == TMM_CONFIRM_OPTION)
    {
        uwItemId = LO_WORD(uiParam);
        uwOption = HI_WORD(uiParam);

        switch (uwItemId)
        {
        case IDM_SELFTIMER:
            Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_SELFTIMER, 1, uwOption);
            break;

        case IDM_PHOTO_SIZE:
            if (UI_GetData(FL_CardStatus) == CARD_REMOVED)
            {
                Ux_CloseWindow(&MenuCommonItemCtrl, 0);
                Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CAPTURE_SIZE, 1, PHOTO_SIZE_3M);
                Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_NO_CARD,FLOWWRNMSG_TIMER_2SEC);
                Delay_DelayMs(1000);
                break;
            }
            Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CAPTURE_SIZE, 1, uwOption);
            break;

        case IDM_QUALITY:
            Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_QUALITY, 1, uwOption);
            break;

        case IDM_CONTINUE_SHOT:
            Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CONTSHOT, 1, uwOption);
            break;

        case IDM_EV:
            Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_EV, 1, uwOption);
            break;

        case IDM_ISO:
            Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_ISO, 1, uwOption);
            break;

        case IDM_SHARPNESS:
            Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_SHARPNESS, 1, uwOption);
            break;

        case IDM_WB:
            Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_WB, 1, uwOption);
            break;

        case IDM_COLOR_EFFECT:
            Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_COLOR, 1, uwOption);
            break;

        case IDM_ANTISHAKE:
            Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_ANTISHAKING, 1, uwOption);
            break;

        case IDM_FD:
            Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_FD, 1, uwOption);
            break;

        case IDM_QUICK_REVIEW:
            Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_PREVIEW, 1, uwOption);
            break;

        case IDM_DATE_STAMP:
            Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_DATE_PRINT, 1, uwOption);
            break;

        default:
            Setup_MenuCallback(uiMessage, uiParam);
            break;
        }
    }

    return TMF_PROCESSED;
}
