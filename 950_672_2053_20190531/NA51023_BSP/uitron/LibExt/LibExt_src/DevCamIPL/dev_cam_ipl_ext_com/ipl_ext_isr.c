/**
    ISR sample.

    @file       ipl_ext_isr.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "ipl_ext_common.h"
#include "dis_alg.h"

void IPL_IsrSIECB(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx)
{
	if (IntStatus & SIE_FLDEND) {
		if (idx > 1) {
			AE_TrigEvent(IPL_UTI_CONV2_AE_ID(Id), IntStatus);
			AWB_TrigEvent(IPL_UTI_CONV2_AWB_ID(Id));
		}
	}
#if 0   //Jarkko tmp
	//customized op
	if (IntStatus & SIE_FLDEND) {
		if (idx > 1) {  //skip 3A for first frame(garbage)
			AE_TrigEvent(IPL_UTI_CONV2_AE_ID(Id), IntStatus);
			AWB_TrigEvent(IPL_UTI_CONV2_AWB_ID(Id));

			if (IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_WDR) {
				ICFFunc_TrigEvent(Id);
			}
		}
	}

	DISALG_PARAM disInfo = {0};
	IPC_QINFO QInfo = {0};
	static UINT32 PrvAddr = 0;

	if (IntStatus & SIE_DRAM4_END) {
		if (IPL_CtrlGetInfor(Id, IPLCTRL_DIS_EN) == ENABLE) {
			QInfo = IPL_CTRLQuery(Id, ICF_ETHOUT_RDY);

			disInfo.frame_cnt = idx;
			disInfo.InSizeH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H);

			disInfo.InLineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_ETH_SIZE_HLOS);
			disInfo.InSizeV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_V);
			if (PrvAddr == 0) {
				disInfo.InAdd0 = QInfo.Rdy_Eth.Addr;
			} else {
				disInfo.InAdd0 = PrvAddr;
			}
			disInfo.InAdd1 = QInfo.Rdy_Eth.Addr;

			if (dis_getTriggerStatus() == FALSE) {
				dis_setDisInfor(&disInfo);
				PrvAddr = QInfo.Rdy_Eth.Addr;
			}

			if (dis_getTriggerStatus() == FALSE) {
				dis_setDisInfor(&disInfo);
			}
		}
	}

	if (IntStatus & SIE_BP1) {
	}
	if (IntStatus & SIE_BP2) {
	}
#endif
}

void IPL_IsrRHECB(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx)
{
	if (IntStatus & RHE_FMD) {
		if (idx > 2) {
			if ((IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_WDR) || (IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_SHDR) || (IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_DEFOG)) {
				ICFFunc_TrigEvent(Id);
			}
		}
	}
}

void IPL_IsrIMECB(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx)
{
	if (IntStatus & IME_FMD) {
		IQS_Ctrl_ISR(Id);
	}
}

IPL_DIS_CB ipl_dis_cb = NULL;

void IPL_Set_Dis_CB(IPL_DIS_CB cb)
{
	ipl_dis_cb = cb;
}

void IPL_IsrIPECB(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx)
{
	DISALG_PARAM disInfo = {0};
	IPC_QINFO QInfo = {0};
	static UINT32 PrvAddr = 0;

	if (IntStatus & IPE_FMD) {
		if ((idx > 1) && (Id == IPL_ID_1)) {
			AF_TrigEvent(IPL_UTI_CONV2_AF_ID(Id));
		}

		if (ipl_dis_cb != NULL) {
	        if (IPL_CtrlGetInfor(Id, IPLCTRL_IPE_ETH_OUT_EN) == ENABLE) {
	            QInfo = IPL_Ctrl_Query(Id, ICF_IPE_ETHOUT_RDY);

	            disInfo.frame_cnt = idx;
	            disInfo.InSizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_SIZE_H);
	            disInfo.InLineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_ETH_OUT_SIZE_HLOS);
	            disInfo.InSizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_SIZE_V);

	            if (PrvAddr == 0) {
	                disInfo.InAdd0 = QInfo.Rdy_Eth.addr[1];
	            } else {
	                disInfo.InAdd0 = PrvAddr;
	            }
	            disInfo.InAdd1 = QInfo.Rdy_Eth.addr[1];

	            if (dis_getTriggerStatus() == FALSE) {
	                //dis_setDisInfor(&disInfo);
	                ipl_dis_cb(&disInfo);
	                PrvAddr = QInfo.Rdy_Eth.addr[1];
	            }
	        }
		}
	}
}

void IPL_IsrIFECB(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx)
{
	if (IntStatus & IFE_FMD) {
	}
}


void IPL_IsrDCECB(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx)
{
	if (IntStatus & DCE_FMD) {
		IQS_TrigEvent(IPL_UTI_CONV2_IQS_ID(Id));
	}
}
