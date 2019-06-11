/**
    ipl ext dx cam

    extend library common interface

    @file       ipl_ext_dx_cam.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

//#include "/Lib/DevCamIQS/iqs_common/iqs_setting_flow.h"
#include "pq_obj.h"
#include "pq_obj_drv.h"

#define __MODULE__          DxPQExt
#define __DBGLVL__          4 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" // *=All, [mark]=CustomClass
#include "DebugModule.h"

#if (defined(_IQ_PARAM1_)&&defined(_AWB_PARAM1_) &&defined(_AE_PARAM1_))
static pqs_drv_INFO pqs_drv_name =
{
    _CHIP_NAME_,
    {
        _SENSOR1_,
        _SENSOR2_,
    },
	{
    	_IQ_PARAM1_,
    	_IQ_PARAM2_,
	},
	{
    	_AWB_PARAM1_,
    	_AWB_PARAM2_,
	},
	{
    	_AE_PARAM1_,
    	_AE_PARAM2_,
	}
};
#else
static pqs_drv_INFO pqs_drv_name = {0};
#endif

/**
    iq libext init function
*/
static BOOL pqs_iq_flag[IQ_PARAM_ID_MAX] = {FALSE};

BOOL pqs_iq_check(UINT32 id)
{
	return pqs_iq_flag[id];
}

static IQS_MAP_INFO pqs_iq_search_tab[] = {
#if (defined(_IQ_PARAM_AR0237_EVB_))
    {"IQ_PARAM_AR0237_EVB", iq_param_init_ar0237_evb},
#endif

#if (defined(_IQ_PARAM_AR0330_EVB_))
    {"IQ_PARAM_AR0330_EVB", iq_param_init_ar0330_evb},
#endif

#if (defined(_IQ_PARAM_AR0521_EVB_))
    {"IQ_PARAM_AR0521_EVB", iq_param_init_ar0521_evb},
#endif

#if (defined(_IQ_PARAM_IMX290_EVB_))
    {"IQ_PARAM_IMX290_EVB", iq_param_init_imx290_evb},
#endif
    
#if (defined(_IQ_PARAM_IMX291_EVB_))
    {"IQ_PARAM_IMX291_EVB", iq_param_init_imx291_evb},
#endif

#if (defined(_IQ_PARAM_IMX307_EVB_))
    {"IQ_PARAM_IMX307_EVB", iq_param_init_imx307_evb},
#endif

#if (defined(_IQ_PARAM_IMX377_EVB_))
    {"IQ_PARAM_IMX377_EVB", iq_param_init_imx377_evb},
#endif

#if (defined(_IQ_PARAM_OS05A10_EVB_))
    {"IQ_PARAM_OS05A10_EVB", iq_param_init_os05a10_evb},
#endif

#if (defined(_IQ_PARAM_OV2735_EVB_))
    {"IQ_PARAM_OV2735_EVB", iq_param_init_ov2735_evb},
#endif

#if (defined(_IQ_PARAM_OV4689_EVB_))
    {"IQ_PARAM_OV4689_EVB", iq_param_init_ov4689_evb},
#endif

#if (defined(_IQ_PARAM_PS5250_EVB_))
    {"IQ_PARAM_PS5250_EVB", iq_param_init_ps5250_evb},
#endif

#if (defined(_IQ_PARAM_SC4236_EVB_))
    {"IQ_PARAM_SC4236_EVB", iq_param_init_sc4236_evb},
#endif

#if (defined(_IQ_PARAM_SC5035_EVB_))
    {"IQ_PARAM_SC5035_EVB", iq_param_init_sc5035_evb},
#endif

#if (defined(_IQ_PARAM_SAMPLE_EVB_))
    {"IQ_PARAM_SAMPLE_EVB", iq_param_init_sample_evb},
#endif



	{"END", NULL}
};

static IQS_MAP_INFO *pqs_iq_search(CHAR *search_name)
{
	UINT32 i;
	IQS_MAP_INFO *info;

	info = pqs_iq_search_tab;

	for (i = 0; (strcmp(info[i].name, "END") != 0); i ++) {
		if (strcmp(info[i].name, search_name) == 0) {
            DBG_DUMP("[PQ] pqs_libext_search--> search pqs libext name = %s \r\n", pqs_iq_search_tab[i].name);
			return &info[i];
		}
	}
	return NULL;
}

BOOL pqs_iq_init(UINT32 id, IQS_HANDLE *handle)
{
	IQS_MAP_INFO *iqs_info = pqs_iq_search(pqs_drv_name.iqs_name[id]);

	if (iqs_info == NULL) {
		DBG_ERR("iq libext get fail\r\n");
		return FALSE;
	}

	if (iqs_info->_iqs_param_init == NULL) {
		DBG_ERR("fp NULL fail\r\n");
		return FALSE;
	}

	if (handle != NULL) {
		handle->_iqs_param_init = iqs_info->_iqs_param_init;
		pqs_iq_flag[id] = TRUE;
		return TRUE;
	}
    
	return FALSE;
}

/**
    awb libext init function
*/
static BOOL pqs_awb_flag[AWB_PARAM_ID_MAX] = {FALSE};

BOOL pqs_awb_check(UINT32 id)
{
	return pqs_awb_flag[id];
}

static AWB_MAP_INFO pqs_awb_search_tab[] = {
#if (defined(_AWB_PARAM_AR0237_EVB_))
    {"AWB_PARAM_AR0237_EVB", awb_get_param_ar0237_evb},
#endif

#if (defined(_AWB_PARAM_AR0330_EVB_))
    {"AWB_PARAM_AR0330_EVB", awb_get_param_ar0330_evb},
#endif

#if (defined(_AWB_PARAM_AR0521_EVB_))
    {"AWB_PARAM_AR0521_EVB", awb_get_param_ar0521_evb},
#endif

#if (defined(_AWB_PARAM_IMX290_EVB_))
	{"AWB_PARAM_IMX290_EVB", awb_get_param_imx290_evb},
#endif

#if (defined(_AWB_PARAM_IMX291_EVB_))
	{"AWB_PARAM_IMX291_EVB", awb_get_param_imx291_evb},
#endif

#if (defined(_AWB_PARAM_IMX307_EVB_))
	{"AWB_PARAM_IMX307_EVB", awb_get_param_imx307_evb},
#endif

#if (defined(_AWB_PARAM_IMX377_EVB_))
	{"AWB_PARAM_IMX377_EVB", awb_get_param_imx377_evb},
#endif

#if (defined(_AWB_PARAM_OS05A10_EVB_))
    {"AWB_PARAM_OS05A10_EVB", awb_get_param_os05a10_evb},
#endif

#if (defined(_AWB_PARAM_OV2735_EVB_))
    {"AWB_PARAM_OV2735_EVB", awb_get_param_ov2735_evb},
#endif

#if (defined(_AWB_PARAM_OV4689_EVB_))
    {"AWB_PARAM_OV4689_EVB", awb_get_param_ov4689_evb},
#endif

#if (defined(_AWB_PARAM_PS5250_EVB_))
    {"AWB_PARAM_PS5250_EVB", awb_get_param_ps5250_evb},
#endif

#if (defined(_AWB_PARAM_SC4236_EVB_))
    {"AWB_PARAM_SC4236_EVB", awb_get_param_sc4236_evb},
#endif

#if (defined(_AWB_PARAM_SC5035_EVB_))
    {"AWB_PARAM_SC5035_EVB", awb_get_param_sc5035_evb},
#endif

#if (defined(_AWB_PARAM_SAMPLE_EVB_))
    {"AWB_PARAM_SAMPLE_EVB", awb_get_param_sample_evb},
#endif

	{"END", NULL}
};

static AWB_MAP_INFO* pqs_awb_search(UINT32 id)
{
    UINT32 j;

	for(j=0; (strcmp(pqs_awb_search_tab[j].name, "END")!=0); j++)
    {
        if(strcmp(pqs_drv_name.awb_name[id], pqs_awb_search_tab[j].name) == 0)
        {
            DBG_DUMP("[PQ] awb_libext_search--> search awb libext name = %s \r\n", pqs_awb_search_tab[j].name);
            return &pqs_awb_search_tab[j];
        }
	}
    
	return NULL;
}

BOOL pqs_awb_init(UINT32 id, UINT32 *init)
{
	AWB_MAP_INFO *awb_info = pqs_awb_search(id);
	AWB_HANDLE handle;

	if (awb_info == NULL) {
		DBG_ERR("awb libext get fail\r\n");
		return FALSE;
	}
    
	handle._awb_param_init = awb_info->_awb_param_init;
	handle._awb_param_init(init);
    pqs_awb_flag[id] = TRUE;
    
	return TRUE;
}

/**
    ae libext init function
*/
static BOOL pqs_ae_flag[AE_PARAM_ID_MAX] = {FALSE};

BOOL pqs_ae_check(UINT32 id)
{
	return pqs_ae_flag[id];
}

#if (defined(_AE_PARAM1_))
AES_MAP_INFO pqs_ae_search_tab[] = {
#if (defined(_AE_PARAM_AR0237_EVB_))
    {"AE_PARAM_AR0237_EVB", ae_get_param_ar0237_evb},
#endif

#if (defined(_AE_PARAM_AR0330_EVB_))
    {"AE_PARAM_AR0330_EVB", ae_get_param_ar0330_evb},
#endif

#if (defined(_AE_PARAM_AR0521_EVB_))
    {"AE_PARAM_AR0521_EVB", ae_get_param_ar0521_evb},
#endif

#if (defined(_AE_PARAM_IMX290_EVB_))
    {"AE_PARAM_IMX290_EVB", ae_get_param_imx290_evb},
#endif

#if (defined(_AE_PARAM_IMX291_EVB_))
    {"AE_PARAM_IMX291_EVB", ae_get_param_imx291_evb},
#endif

#if (defined(_AE_PARAM_IMX307_EVB_))
    {"AE_PARAM_IMX307_EVB", ae_get_param_imx307_evb},
#endif

#if (defined(_AE_PARAM_IMX317_EVB_))
    {"AE_PARAM_IMX317_EVB", ae_get_param_imx317_evb},
#endif

#if (defined(_AE_PARAM_IMX377_EVB_))
    {"AE_PARAM_IMX377_EVB", ae_get_param_imx377_evb},
#endif

#if (defined(_AE_PARAM_OS05A10_EVB_))
    {"AE_PARAM_OS05A10_EVB", ae_get_param_os05a10_evb},
#endif

#if (defined(_AE_PARAM_OV2735_EVB_))
    {"AE_PARAM_OV2735_EVB", ae_get_param_ov2735_evb},
#endif

#if (defined(_AE_PARAM_OV4689_EVB_))
    {"AE_PARAM_OV4689_EVB", ae_get_param_ov4689_evb},
#endif

#if (defined(_AE_PARAM_PS5250_EVB_))
    {"AE_PARAM_PS5250_EVB", ae_get_param_ps5250_evb},
#endif

#if (defined(_AE_PARAM_SC4236_EVB_))
    {"AE_PARAM_SC4236_EVB", ae_get_param_sc4236_evb},
#endif

#if (defined(_AE_PARAM_SC5035_EVB_))
    {"AE_PARAM_SC5035_EVB", ae_get_param_sc5035_evb},
#endif

#if (defined(_AE_PARAM_SAMPLE_EVB_))
    {"AE_PARAM_SAMPLE_EVB", ae_get_param_sample_evb},
#endif

    {"END", NULL}
};
#endif

#define AES_MAP_UART_EN DISABLE
static AES_MAP_INFO *pqs_ae_search(UINT32 id)
{
    UINT32 j;

	for(j=0; (strcmp(pqs_ae_search_tab[j].name, "END")!=0); j++)
    {
        if(strcmp(pqs_drv_name.aes_name[id], pqs_ae_search_tab[j].name) == 0)
        {
            DBG_DUMP("[PQ] ae_libext_search--> search ae libext name = %s \r\n", pqs_ae_search_tab[j].name);
            return &pqs_ae_search_tab[j];
        }
	}

#if AES_MAP_UART_EN
    DBG_DUMP("AES_name\r\n");
    for(i=0; i<AE_ID_MAX_NUM; i++)
    {
        DBG_DUMP("%s\r\n", pqs_drv_name.aes_name[i]);
    }
    DBG_DUMP("\r\n");

    DBG_ERR("AES_ID_MAP = {");
   /* for(i=0; i<AE_ID_MAX_NUM; i++)
    {
        DBG_DUMP("%d, ", aes_id_map[i]);
    }
    DBG_DUMP("}\r\n");*/
#endif

	return NULL;
}

BOOL pqs_ae_init(UINT32 id, UINT32 *init)
{
	AES_MAP_INFO *ae_info = pqs_ae_search(id);
	AE_HANDLE handle;

	if (ae_info == NULL) {
		DBG_ERR("ae libext get fail\r\n");
		return FALSE;
	}
    
	handle._ae_param_init = ae_info->_ae_param_init;
	handle._ae_param_init(init);
    pqs_ae_flag[id] = TRUE;
    
	return TRUE;
}

/**
    pqs libext setting name
*/
pqs_drv_INFO *pqs_get_drv_name(void)
{

	return &pqs_drv_name;
}
