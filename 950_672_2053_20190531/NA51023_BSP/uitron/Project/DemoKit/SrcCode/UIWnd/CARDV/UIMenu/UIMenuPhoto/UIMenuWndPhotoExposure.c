//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIMenuWndPhotoExposureRes.c"
#include "UIMenuWndPhotoExposure.h"
#include "MenuCommonItem.h"
#include "UISetup.h"
#include "UIFlow.h"

static UINT32 g_uiEVValue = 0; // for EV setting backup

//---------------------UIMenuWndPhotoExposureCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndPhotoExposure)
CTRL_LIST_ITEM(UIMenuWndPhotoExposure_Panel)
CTRL_LIST_END

//----------------------UIMenuWndPhotoExposureCtrl Event---------------------------
INT32 UIMenuWndPhotoExposure_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPhotoExposure_OnClose(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPhotoExposure_OnKeyLeft(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPhotoExposure_OnKeyMenu(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPhotoExposure_OnKeyMode(VControl *, UINT32, UINT32 *);
//INT32 UIMenuWndPhotoExposure_OnKeyShutter2(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndPhotoExposure)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIMenuWndPhotoExposure_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIMenuWndPhotoExposure_OnClose)
EVENT_ITEM(NVTEVT_KEY_LEFT,UIMenuWndPhotoExposure_OnKeyLeft)
EVENT_ITEM(NVTEVT_KEY_MENU,UIMenuWndPhotoExposure_OnKeyMenu)
EVENT_ITEM(NVTEVT_KEY_MODE,UIMenuWndPhotoExposure_OnKeyMode)
//EVENT_ITEM(NVTEVT_KEY_SHUTTER2,UIMenuWndPhotoExposure_OnKeyShutter2)
EVENT_END

INT32 UIMenuWndPhotoExposure_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    g_uiEVValue = SysGetFlag(FL_EV); // backup Color setting
    UxMenu_SetData(&UIMenuWndPhotoExposure_MenuCtrl,MNU_CURITM,g_uiEVValue);
    //Ux_RedrawAllWind();
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPhotoExposure_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_EV,1,g_uiEVValue); // recall EV setting
    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPhotoExposure_OnKeyLeft(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_CloseWindow(&UIMenuWndPhotoExposureCtrl,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPhotoExposure_OnKeyMenu(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_CloseWindow(&MenuCommonItemCtrl,0); // close whole tab menu
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPhotoExposure_OnKeyMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(&UISetupObjCtrl,NVTEVT_EXE_CHANGEDSCMODE,1,DSCMODE_CHGTO_NEXT);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPhotoExposure_OnKeyShutter2(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_SendEvent(&UISetupObjCtrl,NVTEVT_FORCETO_LIVEVIEW_MODE,0);
    return NVTEVT_CONSUME;
}
//---------------------UIMenuWndPhotoExposure_PanelCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndPhotoExposure_Panel)
CTRL_LIST_ITEM(UIMenuWndPhotoExposure_Menu)
CTRL_LIST_END

//----------------------UIMenuWndPhotoExposure_PanelCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndPhotoExposure_Panel)
EVENT_END

//----------------------UIMenuWndPhotoExposure_MenuCtrl Event---------------------------
INT32 UIMenuWndPhotoExposure_Menu_OnKeyUp(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPhotoExposure_Menu_OnKeyDown(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPhotoExposure_Menu_OnKeyEnter(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndPhotoExposure_Menu_OnKeyShutter2(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndPhotoExposure_Menu)
EVENT_ITEM(NVTEVT_KEY_UP,UIMenuWndPhotoExposure_Menu_OnKeyUp)
EVENT_ITEM(NVTEVT_KEY_DOWN,UIMenuWndPhotoExposure_Menu_OnKeyDown)
EVENT_ITEM(NVTEVT_KEY_ENTER,UIMenuWndPhotoExposure_Menu_OnKeyEnter)
EVENT_ITEM(NVTEVT_KEY_SHUTTER2,UIMenuWndPhotoExposure_Menu_OnKeyShutter2)
EVENT_END

INT32 UIMenuWndPhotoExposure_Menu_OnKeyUp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  value;
    Ux_SendEvent(pCtrl,NVTEVT_PREVIOUS_ITEM,0);
    value = UxMenu_GetData(pCtrl,MNU_CURITM);
    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_EV,1,value);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPhotoExposure_Menu_OnKeyDown(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  value;
    Ux_SendEvent(pCtrl,NVTEVT_NEXT_ITEM,0);
    value = UxMenu_GetData(pCtrl,MNU_CURITM);
    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_EV,1,value);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPhotoExposure_Menu_OnKeyEnter(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    g_uiEVValue = UxMenu_GetData(pCtrl,MNU_CURITM); // change EV setting
    Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_EV,1,g_uiEVValue);
    Ux_CloseWindow(&UIMenuWndPhotoExposureCtrl,0);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndPhotoExposure_Menu_OnKeyShutter2(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    // the same behavior as ENTER key!
    return UIMenuWndPhotoExposure_Menu_OnKeyEnter(pCtrl, paramNum, paramArray);
}
