/**
 * Novatek AF Algorithm sample.
 * 
 * Reference AF Algorithm Implementation.
 * 
 * @file af_proc_nvt.c
 * @note Nothing.
 *
 * Copyright (C) 2017 Novatek Microelectronics Corp. All rights reserved.
 *
 */

#include "Perf.h"
#include "ipl_ctrl.h"
#include "ae_api.h"
#include "af_task.h"
#include "af_alg.h"
#include "af_ui_info.h"
#include "Lens.h"
#include "af_proc_nvt_int.h"
#if (AF_FACE_DETECTION == ENABLE)
#include "fd_lib.h"
#endif
#include "af_param_evb.h"
#include "ipl_cmd.h"
#include "NvtVerInfo.h"

NVTVER_VERSION_ENTRY(AF_PROC_NVT, 1, 00, 000, 00)

#define __MODULE__          AF_PROC
#define __DBGLVL__          2      // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*"    // *=All, [mark]=CustomClass
#include "DebugModule.h"


static AF_INFO g_AFInfo[AF_PROC_CNT] = 
{
    {
        .bIsInit                    = FALSE,
        .AfSubInfo.bAfEnable        = TRUE,
        .AfSubInfo.eShotMode        = AF_MODE_ONESHOT,    //AF_MODE_ONESHOT AF_MODE_CONTINUOUS
        .ManualSearchRange          = {0},
        .AfSubInfo.eExecuteStatus   = AFALG_STOP,
        .iFrameNum                  = 0,
        .bSetCropWin                = FALSE,
        .AfSubInfo.bFaceFocusEnable = FALSE,
        .AfSubInfo.uiFaceDetNum     = 0,
        .AfSubInfo.uiBlkPixelNum    = 3600,    // 1280x720/16/16
        .AfSubInfo.uiDbgMsg         = 0,       //AF_PRNT_DBG_VA
    },
    {
        .bIsInit                    = FALSE,
        .AfSubInfo.bAfEnable        = TRUE,
        .AfSubInfo.eShotMode        = AF_MODE_ONESHOT,
        .ManualSearchRange          = {0},
        .AfSubInfo.eExecuteStatus   = AFALG_STOP,
        .iFrameNum                  = 0,
        .bSetCropWin                = FALSE,
        .AfSubInfo.bFaceFocusEnable = FALSE,
        .AfSubInfo.uiFaceDetNum     = 0,
        .AfSubInfo.uiBlkPixelNum    = 3600,
        .AfSubInfo.uiDbgMsg         = 0,
    },
};


void AF_DRV_GetCurrentRange(UINT32 id, AF_F af_mode)
{
    UINT8 zoom_sec = Lens_ZoomGetSection(ZOOM_CURR_SECTION);    // 1 ~ 11
    AF_INFO *pinfo = &g_AFInfo[id];

    switch (af_mode) {
    case AF_F_SUPER_MACRO:
    case AF_F_MACRO:
        pinfo->AfSubInfo.iMinPos  = AFALG_MAX(0, Lens_FocusGetTable(zoom_sec, 0) - 50);
        pinfo->AfSubInfo.iMaxPos  = Lens_FocusGetTable(zoom_sec, 7) + 50;    // 100 cm distance
        pinfo->AfSubInfo.iInitPos = Lens_FocusGetTable(zoom_sec, 5);         // 50 cm distance
        break;

    case AF_F_INIFINITE:
        pinfo->AfSubInfo.iMinPos  = Lens_FocusGetTable(zoom_sec, 6);          // 70 cm distance
        pinfo->AfSubInfo.iMaxPos  = Lens_FocusGetTable(zoom_sec, 11) + 50;    // infinnite distance
        pinfo->AfSubInfo.iInitPos = Lens_FocusGetTable(zoom_sec, 11);
        break;

    case AF_F_MANUAL:
        pinfo->AfSubInfo.iMinPos  = AFALG_MAX(pinfo->ManualSearchRange.sManualStartIdx, Lens_FocusGetTable(zoom_sec, 0));
        pinfo->AfSubInfo.iMaxPos  = AFALG_MIN(pinfo->ManualSearchRange.sManualEndIdx, Lens_FocusGetTable(zoom_sec, 11));
        pinfo->AfSubInfo.iInitPos = (pinfo->AfSubInfo.iMinPos + pinfo->AfSubInfo.iMaxPos) / 2;
        break;

    case AF_F_CONTINUEFOCUS:
        pinfo->AfSubInfo.iMinPos  = AFALG_MAX(0, Lens_FocusGetTable(zoom_sec, 5) - 50);    // 50 cm distance
        pinfo->AfSubInfo.iMaxPos  = Lens_FocusGetTable(zoom_sec, 11) + 50;                 // infinnite distance
        pinfo->AfSubInfo.iInitPos = Lens_FocusGetPosition() & 0xFFFF;
        break;

    case AF_F_AUTO:
    default:
        pinfo->AfSubInfo.iMinPos  = AFALG_MAX(0, Lens_FocusGetTable(zoom_sec, 5) - 50);    // 50 cm distance
        pinfo->AfSubInfo.iMaxPos  = Lens_FocusGetTable(zoom_sec, 11) + 50;                 // infinnite distance
        pinfo->AfSubInfo.iInitPos = Lens_FocusGetTable(zoom_sec, 6);                       // 70 cm distance
        break;
    }

    DBG_MSG("AF mode = %d, range = %d ~ %d, iInitPos = %d\r\n", af_mode, pinfo->AfSubInfo.iMinPos, pinfo->AfSubInfo.iMaxPos, pinfo->AfSubInfo.iInitPos);
}


UINT16 AF_DRV_GetFocusDist(void)
{
    UINT8 i, zoom_sec = Lens_ZoomGetSection(ZOOM_CURR_SECTION);    // 1 ~ 11
    UINT32 curr_dist;

    for (i = 0; i < 11; i++) {
        if ((Lens_FocusGetPosition() & 0xFFFF) < Lens_FocusGetTable(zoom_sec, i)) {
            break;
        }
    }

    if (i == 0) {
        curr_dist = Lens_FocusGetDistTable(0);
    } else if (i == 11) {
        curr_dist = Lens_FocusGetDistTable(10);
    } else {
        UINT32 a_val, b_val, a_intvl, b_intvl;

        a_val = Lens_FocusGetDistTable(i-1);
        b_val = Lens_FocusGetDistTable(i);
        a_intvl = (Lens_FocusGetPosition() & 0xFFFF) - Lens_FocusGetTable(zoom_sec, i-1);
        b_intvl = Lens_FocusGetTable(zoom_sec, i) - (Lens_FocusGetPosition() & 0xFFFF);
        curr_dist = (a_val * b_intvl + b_val * a_intvl) / (a_intvl + b_intvl);
    }

    return (UINT16)curr_dist;
}


#if (USE_NVT_AF_ALG == ENABLE)
static INT32 AF_DRV_FocusDoAction(UINT8 category, INT32 target_pos)
{
    return Lens_FocusDoAction((FOCUS_CATEGORY)category, target_pos);
}


static INT32 AF_DRV_FocusGetPosition(void)
{
    return Lens_FocusGetPosition() & 0xFFFF;
}


static INT32 AF_DRV_FocusGetRange(void)
{
    return Lens_FocusGetRange(FOCUS_RANGE_BOTH);
}


static UINT8 AF_DRV_ZoomGetSection(UINT8 category)
{
    return Lens_ZoomGetSection((ZOOM_SECTION_CATEGORY)category);
}


static UINT32 AF_DRV_LensGetState(void)
{
    return (UINT32)Lens_ModuleGetState();
}


static UINT32 AF_ALG_AeGetLum(UINT32 id)
{
    AEStatus lum_status;

    AE_GetStatus(id, &lum_status);

    return lum_status.LumaAverage;
}


static void AF_TUNE_VaSetParam(UINT16 thr_low, UINT16 thr_high)
{

}
#endif


// call this API after setting whole parameters or changing parameters
static void AF_VA_SetParam2IPL(UINT32 id)
{
    AF_INFO *pinfo = &g_AFInfo[id];

    pinfo->VaInfo.id = (IPL_PROC_ID)id;

    IPL_SetCmd(IPL_SET_VA, (void *)&pinfo->VaInfo);
    IPL_WaitCmdFinish();
}


void AF_VA_SetCrop(UINT32 id, VACROP win_info)
{
    AF_INFO *pinfo = &g_AFInfo[id];

    pinfo->CropWinPos = win_info;
    pinfo->bSetCropWin = TRUE;
}


static UINT32 AF_VA_DumpVaInfo(UINT32 id)
{
    UINT32 i, j, idx, tmp;
    static UINT32 g1h[AF_WIN_HW_X*AF_WIN_HW_Y] = {0}, g1v[AF_WIN_HW_X*AF_WIN_HW_Y] = {0}, g2h[AF_WIN_HW_X*AF_WIN_HW_Y] = {0}, g2v[AF_WIN_HW_X*AF_WIN_HW_Y] = {0},
                  g1h_cnt[AF_WIN_HW_X*AF_WIN_HW_Y] = {0}, g1v_cnt[AF_WIN_HW_X*AF_WIN_HW_Y] = {0}, g2h_cnt[AF_WIN_HW_X*AF_WIN_HW_Y] = {0}, g2v_cnt[AF_WIN_HW_X*AF_WIN_HW_Y] = {0};
    IPL_VA_GET_RST af_win_acc;
    IPL_VA_INFOR curr_va = {0};

    curr_va.id = id;
    curr_va.va_indep_win_rst = NULL;
    curr_va.va_rst = &af_win_acc;

    af_win_acc.p_g1h    = g1h;
    af_win_acc.p_g1v    = g1v;
    af_win_acc.p_g1hcnt = g1h_cnt;
    af_win_acc.p_g1vcnt = g1v_cnt;
    af_win_acc.p_g2h    = g2h;
    af_win_acc.p_g2v    = g2v;
    af_win_acc.p_g2hcnt = g2h_cnt;
    af_win_acc.p_g2vcnt = g2v_cnt;

    IPL_GetCmd(IPL_GET_VA_CUR_INFOR, (void *)&curr_va);

    DBG_DUMP("16x16 VA    00    01    02    03    04    05    06    07    08    09    10    11    12    13    14    15\r\n");

    for (j = 0; j < AF_WIN_HW_Y; j++) {
        DBG_DUMP("%02d      ", j);
        for (i = 0; i < AF_WIN_HW_X; i++) {
            idx = j * AF_WIN_HW_X + i;
            tmp = g1h[idx] + g1v[idx] + g2h[idx] + g2v[idx];
            DBG_DUMP("%6u", tmp);
        }
        DBG_DUMP("\r\n");
    }

    return 0;
}


static void AF_VA_SetWindowCfg(UINT32 id, VA_WINIDX va_win, VACROP win_info)
{
    UINT32 idx = 0;
    AF_INFO *pinfo = &g_AFInfo[id];

    if (va_win == VA_WIN_16X16) {
        pinfo->VaInfo.va_win.uiWinStX = 2 * (win_info.StartPix.x / 2);
        pinfo->VaInfo.va_win.uiWinStY = 2 * (win_info.StartPix.y / 2);
        pinfo->VaInfo.va_win.uiWinNmX = AF_WIN_HW_X;
        pinfo->VaInfo.va_win.uiWinNmY = AF_WIN_HW_Y;
        pinfo->VaInfo.va_win.uiWinSzX = 2 * (win_info.SizeH / 2);
        pinfo->VaInfo.va_win.uiWinSzY = 2 * (win_info.SizeV / 2);
        pinfo->VaInfo.va_win.uiWinSpX = 0;
        pinfo->VaInfo.va_win.uiWinSpX = 0;
    } else if (va_win >= VA_WIN_INDP0) {
        if (va_win == VA_WIN_INDP0) {
            idx = 0;
        } else if (va_win == VA_WIN_INDP1) {
            idx = 1;
        } else if (va_win == VA_WIN_INDP2) {
            idx = 2;
        } else if (va_win == VA_WIN_INDP3) {
            idx = 3;
        } else if (va_win == VA_WIN_INDP4) {
            idx = 4;
        }

        pinfo->VaInfo.va_indep_win[idx].uiWinStX = 2 * (win_info.StartPix.x / 2);
        pinfo->VaInfo.va_indep_win[idx].uiWinStY = 2 * (win_info.StartPix.y / 2);
        pinfo->VaInfo.va_indep_win[idx].uiWinSzX = 2 * (win_info.SizeH / 2);
        pinfo->VaInfo.va_indep_win[idx].uiWinSzY = 2 * (win_info.SizeV / 2);
    }
}


#if (AF_FACE_DETECTION == ENABLE)
static UINT32 AF_VA_Set2FaceWin(UINT32 id)
{
    UINT32 i, det_face_num = 0;
    FD_FACE face_rect[10] = {0};    // must be the same with MAX_FDNUM
    URECT new_coordinate;
    VACROP va_win_info = {0};
    VA_WINIDX win_idx;
    IPL_CUR_INPUT_INFO img_info = {0};

    img_info.id = id;
    IPL_GetCmd(IPL_GET_IPL_CUR_INPUT_INFOR, &img_info);

    new_coordinate.x = new_coordinate.y = 0;
    new_coordinate.w = img_info.in_size_h;
    new_coordinate.h = img_info.in_size_v;

    if ((det_face_num = FD_GetRsltFace(face_rect, &new_coordinate)) != 0) {
        for (i = 0; i < 5; i++) {
            win_idx = (VA_WINIDX)(VA_WIN_INDP0 + i);

            if (i < det_face_num) {
                va_win_info.StartPix.x = face_rect[i].x;
                va_win_info.StartPix.y = face_rect[i].y;
                va_win_info.SizeH = AFALG_MIN(AF_INDP_WIN_W_MAX, face_rect[i].w);
                va_win_info.SizeV = AFALG_MIN(AF_INDP_WIN_H_MAX, face_rect[i].h);

                DBG_MSG("FD#%d @ (%d,%d) with %dx%d\r\n", det_face_num, va_win_info.StartPix.x, va_win_info.StartPix.y, va_win_info.SizeH, va_win_info.SizeV);
            } else {
                va_win_info.StartPix.x = 0;
                va_win_info.StartPix.y = 0;
                va_win_info.SizeH = 0;
                va_win_info.SizeV = 0;
            }

            AF_VA_SetWindowCfg(id, win_idx, va_win_info);
        }
    } else {
        DBG_MSG("face_num = 0!\r\n");
    }

    return AFALG_MIN(5, det_face_num);
}
#endif


static void AF_VA_SetThreshold(UINT32 id, VA_GRP va_grp, UINT16 thr_low, UINT16 thr_high)
{
    AF_INFO *pinfo = &g_AFInfo[id];

    if (va_grp == VA_GRP1_H) {
        pinfo->VaInfo.VaFltrG1.FltrH.uiThL = thr_low;
        pinfo->VaInfo.VaFltrG1.FltrH.uiThH = AFALG_MAX(thr_low, thr_high);
    } else if (va_grp == VA_GRP1_V) {
        pinfo->VaInfo.VaFltrG1.FltrV.uiThL = thr_low;
        pinfo->VaInfo.VaFltrG1.FltrV.uiThH = AFALG_MAX(thr_low, thr_high);
    } else if (va_grp == VA_GRP2_H) {
        pinfo->VaInfo.VaFltrG2.FltrH.uiThL = thr_low;
        pinfo->VaInfo.VaFltrG2.FltrH.uiThH = AFALG_MAX(thr_low, thr_high);
    } else if (va_grp == VA_GRP2_V) {
        pinfo->VaInfo.VaFltrG2.FltrV.uiThL = thr_low;
        pinfo->VaInfo.VaFltrG2.FltrV.uiThH = AFALG_MAX(thr_low, thr_high);
    }
}


static void AF_VA_SetEnable(UINT32 id, BOOL enable, VA_WINIDX va_win)
{
    UINT32 idx = 0;
    AF_INFO *pinfo = &g_AFInfo[id];

    if (va_win == VA_WIN_16X16) {
        pinfo->VaInfo.va_out_en = enable;
    } else if (va_win >= VA_WIN_INDP0) {
        if (va_win == VA_WIN_INDP0) {
            idx = 0;
        } else if (va_win == VA_WIN_INDP1) {
            idx = 1;
        } else if (va_win == VA_WIN_INDP2) {
            idx = 2;
        } else if (va_win == VA_WIN_INDP3) {
            idx = 3;
        } else if (va_win == VA_WIN_INDP4) {
            idx = 4;
        }

        pinfo->VaInfo.va_indep_win[idx].bEn = enable;
    }
}


static void AF_VA_SetLineMaxEnable(UINT32 id, BOOL enable, VA_WINIDX va_win, VA_GRP va_grp)
{
    UINT32 idx = 0;
    AF_INFO *pinfo = &g_AFInfo[id];

    if (va_win == VA_WIN_16X16) {
        if (va_grp == VA_GRP1) {
            pinfo->VaInfo.VaFltrG1.bLineMax = enable;
        } else if (va_grp == VA_GRP2) {
            pinfo->VaInfo.VaFltrG2.bLineMax = enable;
        }
    } else if (va_win >= VA_WIN_INDP0) {
        if (va_win == VA_WIN_INDP0) {
            idx = 0;
        } else if (va_win == VA_WIN_INDP1) {
            idx = 1;
        } else if (va_win == VA_WIN_INDP2) {
            idx = 2;
        } else if (va_win == VA_WIN_INDP3) {
            idx = 3;
        } else if (va_win == VA_WIN_INDP4) {
            idx = 4;
        }

        if (va_grp == VA_GRP1) {
            pinfo->VaInfo.va_indep_win[idx].bLineMaxG1 = enable;
        } else if (va_grp == VA_GRP2) {
            pinfo->VaInfo.va_indep_win[idx].bLineMaxG2 = enable;
        }
    }
}


static void AF_VA_SetCntStaEnable(UINT32 id, BOOL enable, VA_WINIDX va_win, VA_GRP va_grp)
{
    AF_INFO *pinfo = &g_AFInfo[id];

    if (va_win == VA_WIN_16X16) {
        if (va_grp == VA_GRP1) {
            pinfo->VaInfo.VaFltrG1.bCnt = enable;
        } else if (va_grp == VA_GRP2) {
            pinfo->VaInfo.VaFltrG2.bCnt = enable;
        }
    }
}


static void AF_VA_SetEdgeParam(UINT32 id, VA_GRP va_grp, IPE_HAL_VA_FLTR_PARAM edge_ker)
{
    IPE_HAL_VA_FLTR_PARAM *pgrp_sel;
    AF_INFO *pinfo = &g_AFInfo[id];

    if (va_grp == VA_GRP1_H) {
        pgrp_sel = &pinfo->VaInfo.VaFltrG1.FltrH;
    } else if (va_grp == VA_GRP1_V) {
        pgrp_sel = &pinfo->VaInfo.VaFltrG1.FltrV;
    } else if (va_grp == VA_GRP2_H) {
        pgrp_sel = &pinfo->VaInfo.VaFltrG2.FltrH;
    } else if (va_grp == VA_GRP2_V) {
        pgrp_sel = &pinfo->VaInfo.VaFltrG2.FltrV;
    } else {
        DBG_MSG("unknown va_grp value!\r\n");
        return;
    }

    memcpy(pgrp_sel, &edge_ker, sizeof(IPE_HAL_VA_FLTR_PARAM));
}


static void AF_VA_UpdateParam8Iso(UINT32 id)
{
    UINT32 i, iso, iso_base = 100, exp_time, iris, tbl_idx = 0;

    AE_GetPrvAEArg(id, &iso, &exp_time, &iris);

    for (i = 0; i < 12; i++) {
        if (iso > iso_base) {
            tbl_idx++;
        } else {
            break;
        }

        iso_base *= 2;
    }

    tbl_idx = AFALG_MIN(tbl_idx, 11);

    // set VA threshold for filter noise
    AF_VA_SetThreshold(id, VA_GRP1_H, g_usVaNoiseThreshold[tbl_idx][0], g_usVaNoiseThreshold[tbl_idx][1]);
    AF_VA_SetThreshold(id, VA_GRP1_V, g_usVaNoiseThreshold[tbl_idx][0], g_usVaNoiseThreshold[tbl_idx][1]);
    AF_VA_SetThreshold(id, VA_GRP2_H, g_usVaNoiseThreshold[tbl_idx][0], g_usVaNoiseThreshold[tbl_idx][1]);
    AF_VA_SetThreshold(id, VA_GRP2_V, g_usVaNoiseThreshold[tbl_idx][0], g_usVaNoiseThreshold[tbl_idx][1]);
}


static void AF_VA_SetActiveWin(UINT32 id)
{
    UINT32 width = 0, height = 0;
    VACROP va_win_info = { 0 };
    IPL_CUR_INPUT_INFO img_info = { 0 };
    AF_INFO *pinfo = &g_AFInfo[id];

    // 16x16 window
    if (pinfo->bSetCropWin == TRUE) {
        width = pinfo->CropWinPos.SizeH;
        height = pinfo->CropWinPos.SizeV;

        va_win_info.StartPix.x = pinfo->CropWinPos.StartPix.x;
        va_win_info.StartPix.y = pinfo->CropWinPos.StartPix.y;

        pinfo->bSetCropWin = FALSE;
    } else {
        img_info.id = id;
        IPL_GetCmd(IPL_GET_IPL_CUR_INPUT_INFOR, &img_info);

        width = img_info.in_size_h;
        height = img_info.in_size_v;

        va_win_info.StartPix.x = 0;
        va_win_info.StartPix.y = 0;
    }

    va_win_info.StartPix.x = 0;
    va_win_info.StartPix.y = 0;
    va_win_info.SizeH = width / AF_WIN_HW_X;
    va_win_info.SizeV = height / AF_WIN_HW_Y;
    pinfo->AfSubInfo.uiBlkPixelNum = va_win_info.SizeH * va_win_info.SizeV;

    AF_VA_SetWindowCfg(id, VA_WIN_16X16, va_win_info);

    // independent #0 window
    if (width >= AF_INDP_WIN_W_MAX) {
        va_win_info.StartPix.x = (width - AF_INDP_WIN_W_MAX) / 2;
        va_win_info.SizeH = AF_INDP_WIN_W_MAX;
    } else {
        va_win_info.StartPix.x = 0;
        va_win_info.SizeH = width;
    }

    if (height >= AF_INDP_WIN_H_MAX) {
        va_win_info.StartPix.y = (height - AF_INDP_WIN_H_MAX ) / 2;
        va_win_info.SizeV = AF_INDP_WIN_H_MAX;
    } else {
        va_win_info.StartPix.y = 0;
        va_win_info.SizeV = height;
    }

    AF_VA_SetWindowCfg(id, VA_WIN_INDP0, va_win_info);
}


void AF_VA_Setup(UINT32 id)
{
    VACROP va_win_info = {0};
    IPE_HAL_VA_FLTR_PARAM va_fltr_coef = { .uiThL = g_usVaNoiseThreshold[0][0], .uiThH = g_usVaNoiseThreshold[0][1] };
    AF_INFO *pinfo = &g_AFInfo[id];

    AF_VA_SetActiveWin(id);

    va_win_info.StartPix.x = 0;
    va_win_info.StartPix.y = 0;
    va_win_info.SizeH = 4 * AF_WIN_HW_X;
    va_win_info.SizeV = 4 * AF_WIN_HW_Y;
    AF_VA_SetWindowCfg(id, VA_WIN_INDP1, va_win_info);
    AF_VA_SetWindowCfg(id, VA_WIN_INDP2, va_win_info);
    AF_VA_SetWindowCfg(id, VA_WIN_INDP3, va_win_info);
    AF_VA_SetWindowCfg(id, VA_WIN_INDP4, va_win_info);

    // enable 16x16 and independent windows 0
    AF_VA_SetEnable(id, TRUE, VA_WIN_16X16);
#if (AF_FACE_DETECTION == ENABLE)
    AF_VA_SetEnable(id, TRUE, VA_WIN_INDP0);
    AF_VA_SetEnable(id, TRUE, VA_WIN_INDP1);
    AF_VA_SetEnable(id, TRUE, VA_WIN_INDP2);
    AF_VA_SetEnable(id, TRUE, VA_WIN_INDP3);
    AF_VA_SetEnable(id, TRUE, VA_WIN_INDP4);
#else
    AF_VA_SetEnable(id, FALSE, VA_WIN_INDP0);
    AF_VA_SetEnable(id, FALSE, VA_WIN_INDP1);
    AF_VA_SetEnable(id, FALSE, VA_WIN_INDP2);
    AF_VA_SetEnable(id, FALSE, VA_WIN_INDP3);
    AF_VA_SetEnable(id, FALSE, VA_WIN_INDP4);
#endif

    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_16X16, VA_GRP1);
    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_16X16, VA_GRP2);
    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_INDP0, VA_GRP1);
    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_INDP0, VA_GRP2);
    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_INDP1, VA_GRP1);
    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_INDP1, VA_GRP2);
    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_INDP2, VA_GRP1);
    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_INDP2, VA_GRP2);
    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_INDP3, VA_GRP1);
    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_INDP3, VA_GRP2);
    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_INDP4, VA_GRP1);
    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_INDP4, VA_GRP2);

    AF_VA_SetCntStaEnable(id, TRUE, VA_WIN_16X16, VA_GRP1);
    AF_VA_SetCntStaEnable(id, TRUE, VA_WIN_16X16, VA_GRP2);

    va_fltr_coef.FltrSize = IPE_VA_FLTR_SIZE_3;
    va_fltr_coef.FltrSymm = IPE_VA_FLTR_MIRROR;
    va_fltr_coef.uiTapA = 2;
    va_fltr_coef.iTapB = -1;
    va_fltr_coef.iTapC = 0;
    va_fltr_coef.iTapD = 0;
    va_fltr_coef.uiDiv = AF_VA_DIV;
    AF_VA_SetEdgeParam(id, VA_GRP1_H, va_fltr_coef);

    va_fltr_coef.FltrSize = IPE_VA_FLTR_SIZE_3;
    va_fltr_coef.FltrSymm = IPE_VA_FLTR_MIRROR;
    va_fltr_coef.uiTapA = 2;
    va_fltr_coef.iTapB = -1;
    va_fltr_coef.iTapC = 0;
    va_fltr_coef.iTapD = 0;
    va_fltr_coef.uiDiv = AF_VA_DIV;
    AF_VA_SetEdgeParam(id, VA_GRP1_V, va_fltr_coef);

    va_fltr_coef.FltrSize = IPE_VA_FLTR_SIZE_3;
    va_fltr_coef.FltrSymm = IPE_VA_FLTR_MIRROR;
    va_fltr_coef.uiTapA = 2;
    va_fltr_coef.iTapB = 0;
    va_fltr_coef.iTapC = -1;
    va_fltr_coef.iTapD = 0;
    va_fltr_coef.uiDiv = AF_VA_DIV;
    AF_VA_SetEdgeParam(id, VA_GRP2_H, va_fltr_coef);

    va_fltr_coef.FltrSize = IPE_VA_FLTR_SIZE_3;
    va_fltr_coef.FltrSymm = IPE_VA_FLTR_MIRROR;
    va_fltr_coef.uiTapA = 2;
    va_fltr_coef.iTapB = 0;
    va_fltr_coef.iTapC = -1;
    va_fltr_coef.iTapD = 0;
    va_fltr_coef.uiDiv = AF_VA_DIV;
    AF_VA_SetEdgeParam(id, VA_GRP2_V, va_fltr_coef);

#if (AF_FACE_DETECTION == ENABLE)
    if (IPL_AlgGetUIInfo(id, IPL_SEL_FD_IMM) == SEL_FD_ON) {
        pinfo->AfSubInfo.uiFaceDetNum = AF_VA_Set2FaceWin(id);
        pinfo->AfSubInfo.bFaceFocusEnable = TRUE;
    } else {
        pinfo->AfSubInfo.uiFaceDetNum = 0;
        pinfo->AfSubInfo.bFaceFocusEnable = FALSE;
    }
#else
    pinfo->AfSubInfo.bFaceFocusEnable = FALSE;
#endif

    pinfo->VaInfo.VAOutSel = IPE_VA_OUT_BOTH;

    AF_VA_UpdateParam8Iso(id);

    AF_VA_SetParam2IPL(id);
}


static void AF_VA_Release(UINT32 id)
{
    VACROP va_win_info;

    va_win_info.StartPix.x = 0;
    va_win_info.StartPix.y = 0;
    va_win_info.SizeH = 4 * AF_WIN_HW_X;
    va_win_info.SizeV = 4 * AF_WIN_HW_Y;
    AF_VA_SetWindowCfg(id, VA_WIN_16X16, va_win_info);

    AF_VA_SetEnable(id, FALSE, VA_WIN_16X16);
    AF_VA_SetEnable(id, FALSE, VA_WIN_INDP0);
    AF_VA_SetEnable(id, FALSE, VA_WIN_INDP1);
    AF_VA_SetEnable(id, FALSE, VA_WIN_INDP2);
    AF_VA_SetEnable(id, FALSE, VA_WIN_INDP3);
    AF_VA_SetEnable(id, FALSE, VA_WIN_INDP4);

    AF_VA_SetParam2IPL(id);
}


#if (USE_NVT_AF_ALG == DISABLE)
static void AFALG_Process(UINT32 id)
{
#if 0
    UINT32 va;
    static IPE_HAL_INDEP_VA_WIN_RSLT indep_win_acc;
    IPL_VA_INFOR curr_va = {0};

    // get current indepdent window #0 VA value
    curr_va.id = id;
    curr_va.va_indep_win_rst = &indep_win_acc;
    curr_va.va_rst = NULL;

    IPL_GetCmd(IPL_GET_VA_CUR_INFOR, (void *)&curr_va);

    va = indep_win_acc.indep_win_rst[0].uiVaG1H + indep_win_acc.indep_win_rst[0].uiVaG1V + indep_win_acc.indep_win_rst[0].uiVaG2H + indep_win_acc.indep_win_rst[0].uiVaG2V;
#else
    NVTAFALG_GetVaSta(id);
#endif
}
#else    // (USE_NVT_AF_ALG == ENABLE)
static void AF_SetAlgParam(UINT32 id, AFALG_CFG_ITEM item, AFALG_SET_PARAM set_param)
{
    NVTAFALG_SetAlgParam(id, item, set_param);
}


static AFALG_SET_PARAM AF_GetAlgParam(UINT32 id, AFALG_CFG_ITEM item)
{
    return NVTAFALG_GetAlgParam(id, item);
}
#endif


static void AF_SetParam(UINT32 id)
{
    AF_INFO *pinfo = &g_AFInfo[id];

    pinfo->uiId = id;

    if (pinfo->AfSubInfo.eWindowCfg == AFALG_WINDOW_CENTER) {    // single window AF
        memcpy(pinfo->AfSubInfo.ucWindowWeight, g_ucAfWinWeight8x8_4, sizeof(g_ucAfWinWeight8x8_4));
    } else if (pinfo->AfSubInfo.eWindowCfg == AFALG_WINDOW_MULTI) {    // multi window AF
        memcpy(pinfo->AfSubInfo.ucWindowWeight, g_ucAfWinWeight8x8_64, sizeof(g_ucAfWinWeight8x8_64));
    } else {
        memcpy(pinfo->AfSubInfo.ucWindowWeight, g_ucAfWinWeight8x8_36, sizeof(g_ucAfWinWeight8x8_36));
        DBG_ERR("must choose AF window weighting!\r\n");
    }
}


AF_CBMSG_FP g_AfProcCbfp = NULL;    // callback funtion to notify UI

void AF_RegisterCB(AF_CBMSG_FP cb_func)
{
    g_AfProcCbfp = cb_func;
}


ER AF_Run(AF_PROC_ID id, BOOL wait_finish)
{
    static BOOL ever_entry = FALSE;
    UINT32 af_mode;
    ER rt_status = E_OK;
#if (AF_FACE_DETECTION == ENABLE)
    AF_INFO *pinfo = &g_AFInfo[id];
#endif

    if (ever_entry == FALSE) {
        ever_entry = TRUE;
        AF_VA_Setup(id);    // setup IPE VA config
    }

    // set AF active window
    AF_VA_SetActiveWin(id);

    // get searching range
    af_mode = af_get_ui_info((IPL_PROC_ID)id, AF_UI_F);
    AF_DRV_GetCurrentRange(id, af_mode);

    AF_VA_UpdateParam8Iso(id);

    if (AF_GetCtrlObjStatus(id) >= AF_TASK_STATUS_OPEN) {
#if (AF_FACE_DETECTION == ENABLE)
        if (IPL_AlgGetUIInfo(id, IPL_SEL_FD_IMM) == SEL_FD_ON) {
            pinfo->AfSubInfo.uiFaceDetNum = AF_VA_Set2FaceWin(id);
            pinfo->AfSubInfo.bFaceFocusEnable = TRUE;
        } else {
            pinfo->AfSubInfo.uiFaceDetNum = 0;
            pinfo->AfSubInfo.bFaceFocusEnable = FALSE;
        }
#endif

        AF_VA_SetParam2IPL(id);
        IPL_Wait_Vd((IPL_PROC_BIT_ID)(1 << id), FALSE, 200);

        NVTAFALG_ResetInfo(id);    // reset AF info before AF is started
    } else {
        DBG_MSG("AF doesn't open yet(%d)!\r\n", id);
    }

    return rt_status;
}


ER AF_Release(AF_PROC_ID id, BOOL wait_finish)
{
    ER rt_status = E_OK;
    AF_INFO *pinfo = &g_AFInfo[id];

    pinfo->AfSubInfo.eExecuteStatus = AFALG_QUIT;

    AF_VA_Release(id);    // release IPE VA

    if (wait_finish == TRUE) {
        AF_WaitIdle(id);
    }

    return rt_status;
}


void AF_SetManualRange(UINT32 id, INT16 start_pos, INT16 end_pos, INT8 step_size)
{
    AF_DRV_MANUAL manual_setting;
    AF_INFO *pinfo = &g_AFInfo[id];

    manual_setting.sManualStartIdx = start_pos;
    manual_setting.sManualEndIdx = end_pos;
    manual_setting.cManualStep = step_size;

    pinfo->ManualSearchRange = manual_setting;
}


UINT32 AF_WriteDebugInfo(UINT32 id, UINT32 addr)
{
    UINT32 buf_cnt;
    UINT8 zoom_sec = Lens_ZoomGetSection(ZOOM_CURR_SECTION);    // 1 ~ 11
    AF_INFO *pinfo = &g_AFInfo[id];

    buf_cnt = snprintf(NULL, 0, "\r\n\r\n\r\nAF Info------------------------------------------------------------------------------------------------------------------------\r\n");
    addr += snprintf((char *)addr, buf_cnt+1, "\r\n\r\n\r\nAF Info------------------------------------------------------------------------------------------------------------------------\r\n");

    buf_cnt = snprintf(NULL, 0, "ID=%d, ZoomSec=%d\r\n");
    addr += snprintf((char *)addr, buf_cnt+1, "ID=%d, ZoomSec=%d\r\n", pinfo->uiId, zoom_sec);

#if (USE_NVT_AF_ALG == ENABLE)
    NVTAFALG_WriteDebugInfo(id, &addr);
#endif

    buf_cnt = snprintf(NULL, 0, "\r\n");
    addr += snprintf((char *)addr, buf_cnt+1, "\r\n");

    return addr;
}


void AF_Init(UINT32 id, UINT32 current_status)
{
    UINT32 ui_status = 0, af_mode;
#if (USE_NVT_AF_ALG == ENABLE)
    UINT32 i;
    AFALG_SET_PARAM set_param = {0}, get_param = {0};
#endif
    AF_INFO *pinfo = &g_AFInfo[id];

    if (pinfo->bIsInit == TRUE) {
        return;
    }

    if (pinfo->AfSubInfo.uiDbgMsg & AF_PRNT_DBG_PERF_MEASURE) {
        Perf_Open();
        pinfo->AfSubInfo.uiPerfTime = Perf_GetCurrent();
    }

    if (g_AfProcCbfp) {
        g_AfProcCbfp(AF_CBMSG_AF_START, (void *)&ui_status);
    }

    switch (af_get_ui_info(id, AF_UI_WINDOW)) {
    case AF_WINDOW_AUTO:
        pinfo->AfSubInfo.eWindowCfg = AFALG_WINDOW_MULTI;
        break;

    case AF_WINDOW_MANUAL:
        pinfo->AfSubInfo.eWindowCfg = AFALG_WINDOW_MANUAL;
        break;

    case AF_WINDOW_CENTER:
        pinfo->AfSubInfo.eWindowCfg = AFALG_WINDOW_CENTER;
        break;

    default:
        DBG_ERR("exception AF Window!\r\n");
        break;
    }

#if (USE_NVT_AF_ALG == ENABLE)
    NVTAFALG_RegFocusDoActionCB(&AF_DRV_FocusDoAction);
    NVTAFALG_RegFocusGetPositionCB(&AF_DRV_FocusGetPosition);
    NVTAFALG_RegFocusGetRangeCB(&AF_DRV_FocusGetRange);
    NVTAFALG_RegZoomGetSectionCB(&AF_DRV_ZoomGetSection);
    NVTAFALG_RegLensGetStateCB(&AF_DRV_LensGetState);
    NVTAFALG_RegAeGetLumCB(&AF_ALG_AeGetLum);
    NVTAFALG_RegVaSetParamCB(&AF_TUNE_VaSetParam);

    NVTAFALG_SetAfInfoPtr(id, &pinfo->AfSubInfo, &pinfo->VaInfo);

    set_param.iAfAlgParam[0] = AF_SEARCH_METHOD;
    AF_SetAlgParam(id, AFALG_CFG_METHOD, set_param);

    set_param.iAfAlgParam[0] = AF_MAX_SEARCH_COUNT;
    AF_SetAlgParam(id, AFALG_CFG_TIME_OUT_NUM, set_param);

    set_param.iAfAlgParam[0] = AF_ROUGH_STEP_SIZE;
    set_param.iAfAlgParam[1] = AF_FINE_STEP_SIZE;
    set_param.iAfAlgParam[2] = AF_FINAL_STEP_SIZE;
    AF_SetAlgParam(id, AFALG_CFG_STEP_SIZE, set_param);

    set_param.iAfAlgParam[0] = AF_THRES_ROUGH_PERCENT;
    set_param.iAfAlgParam[1] = AF_THRES_FINE_PERCENT;
    set_param.iAfAlgParam[2] = AF_THRES_FINAL_PERCENT;
    set_param.iAfAlgParam[3] = AF_THRES_RESTART_PERCENT;
    AF_SetAlgParam(id, AFALG_CFG_THRS, set_param);

    for (i = AFALG_CFG_METHOD; i < AFALG_CFG_MAX_ITEM; i++) {
        memset(&get_param, 0, sizeof(AFALG_SET_PARAM));

        get_param = AF_GetAlgParam(id, (AFALG_CFG_ITEM)i);
        DBG_MSG("NVTAFALG param = %d, %d, %d, %d\r\n", get_param.iAfAlgParam[0], get_param.iAfAlgParam[1], get_param.iAfAlgParam[2], get_param.iAfAlgParam[3]);
    }
#else
    NVTAFALG_SetAfInfoPtr(id, &pinfo->AfSubInfo, &pinfo->VaInfo);
#endif

    // get searching range
    af_mode = af_get_ui_info((IPL_PROC_ID)id, AF_UI_F);
    AF_DRV_GetCurrentRange(id, af_mode);

#if (USE_NVT_AF_ALG == ENABLE)
    NVTAFALG_Init(id);
    //NVTAFALG_ResetInfo(id);    // start NVTAFALG_Process()
#else
    NVTAFALG_InitVa(id);
#endif

    AF_SetParam(id);

    AF_VA_DumpInfoFP(AF_VA_DumpVaInfo);
    AF_TriggerFP(AF_Run);

    pinfo->bIsInit = TRUE;

    PRINT_AF(AF_PRNT_DBG_PERF_MEASURE, "AF init time = %d msec\r\n", (Perf_GetCurrent()-pinfo->AfSubInfo.uiPerfTime));
}


void AF_Process(UINT32 id, UINT32 current_status)
{
    UINT32 exe_status;
    AF_INFO *pinfo = &g_AFInfo[id];

    pinfo->iFrameNum++;

    if ((pinfo->AfSubInfo.eExecuteStatus == AFALG_RUN) || (pinfo->AfSubInfo.eExecuteStatus == AFALG_END)) {
#if (USE_NVT_AF_ALG == DISABLE)
        AFALG_Process(id);
#else
        NVTAFALG_Process(id);
#endif

        if (pinfo->AfSubInfo.eExecuteStatus == AFALG_END) {
            if (g_AfProcCbfp) {
                exe_status = pinfo->AfSubInfo.uiResultStatus;
                g_AfProcCbfp(AF_CBMSG_AF_DONE, (void *)&exe_status);
            }
        }
    }
}


