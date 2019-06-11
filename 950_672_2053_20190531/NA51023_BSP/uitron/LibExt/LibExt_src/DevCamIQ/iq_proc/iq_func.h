#ifndef _IQ_FUNC_H_
#define _IQ_FUNC_H_

typedef enum {
    IQS_SIE_OBSUB,
	IQS_RHE_OBSUB,
	IQS_IFE_OBSUB
} IQS_SUBOB_MODE;

typedef enum {
	IQS_RHE_CG,
	IQS_DCE_CG,
	IQS_IFE_CG
} IQS_CG_MODE;

typedef enum {
	IQS_Y_OFF,
	IQS_Y_MANUAL,
	IQS_Y_AUTO
} IQS_YCURVE_MODE;

/**
    IQ_SETTING_FUNC_C.
*/
typedef struct _SENSOR_MODE_PARAM {
	void(*init_func)(UINT32 id, UINT32 mode_idx);
}SENSOR_MODE_PARAM;

extern UINT32 iq_get_iso_range(UINT32 Id, IQS_FLOW_MSG MsgID, UINT32 *IsoValue, UINT32 *IsoStart, UINT32 *IsoEnd, UINT32 *IQIdxL, UINT32 *IQIdxH);
extern UINT32 iq_get_iso_range_adj(UINT32 Id, IQS_FLOW_MSG MsgID, UINT32 *IsoValue, UINT32 *IsoStart, UINT32 *IsoEnd, UINT32 *IQIdxL, UINT32 *IQIdxH);
extern UINT32 iq_get_iso_idx(UINT32 Id, IQS_FLOW_MSG MsgID);
extern UINT32 iq_get_sensor_idx(UINT32 Id);
extern UINT32 iq_check_capture(IQS_FLOW_MSG MsgID);
extern BOOL iq_check_rgbir(UINT32 Id);
extern UINT32 iq_get_ob_sub_mode(UINT32 Id);
extern UINT32 iq_get_cg_mode(UINT32 Id);
extern UINT32 iq_get_ycurve_mode(UINT32 Id);
extern void iq_get_dce_param(UINT32 Id, UINT32 MsgID, UINT32 ZoomIdx, DCE_Distortion *pDistortion, DIST *pDce_Dist, Fact_Norm *pRadious);
extern iq_param_table* iq_tbl[Total_SENIDNum];
extern void iq_set_mode2tbl(UINT32 id, iq_param_table* piq_tbl);
#endif
