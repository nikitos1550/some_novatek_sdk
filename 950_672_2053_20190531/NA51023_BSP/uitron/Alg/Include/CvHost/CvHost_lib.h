/**
    CV Host API for Ceva SmartFrame.

    @file CvHost_lib.h


    Example code: Please check Exam_CvHost.c \n

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _CVHOST_LIB_H_
#define _CVHOST_LIB_H_

#include "CvHost_ipc.h"
#include "CvHost_SmartFrame.h"



/**
   Install required system resource.
*/
extern void CvHost_InstallID(void) _SECTION(".kercfg_text");

/**
   Open CvHost
   
   @return status code.
*/
extern CVHOST_STAT CvHost_Open(CVHOST_SEQHAND *p_seq_handle, UINT32 uiAddrWorkMem, UINT32 uiWorkMemSize);

/**
    Query the size of necessary memory.
*/
extern UINT32 CvHost_GetWorkMemSize(void);

/**
    Add SmartFrame kernel.

    @return status code.
*/
extern CVHOST_STAT CvHost_AddKernel(CVHOST_SEQHAND *p_seq_handle, sf_kernel_index_e kernel_idx, char *kernel, sf_data_type_e src_data_type, sf_data_type_e dst_data_type);

/**
    Set kernel parameter.

    The function sets a single parameter param_idx for a kernel indexed by kernel_idx to value param_val.

    @return status code.
*/
extern CVHOST_STAT CvHost_SetKernelParam(CVHOST_SEQHAND *p_seq_handle, sf_kernel_index_e kernel_idx, sf_param_num_e param_idx, UINT32 param_val);

/**
    Set table parameter of kernel.

    Note that it limits p_table type to UINT8* (from void*).

    @return status code.
*/
extern CVHOST_STAT CvHost_SetKernelTableParam(CVHOST_SEQHAND *p_seq_handle, sf_kernel_index_e kernel_idx, sf_param_num_e param_idx, UINT8* p_table, UINT32 size_in_bytes);

/**
    Set sequence parameter.

    The function sets a single parameter param_idx for SmartFrame sequence seq_handle to value param_val.

    @return status code.
*/
extern CVHOST_STAT CvHost_SetSequenceParam(CVHOST_SEQHAND *p_seq_handle, sf_param_seq_e param_idx, UINT32 param_val);

/**
    SmartFrame sequence execution.

    The function executes the selected sequence on a frame.

    @return status code.
*/
extern CVHOST_STAT CvHost_ExecuteSequence(CVHOST_SEQHAND *p_seq_handle, frame_buf_t *p_src_frame[], UINT32 num_inputs, frame_buf_t *p_dst_frame[], UINT32 num_outputs);

/**
   CvHost close function

   @return status code.
*/
extern CVHOST_STAT CvHost_WaitExecuteDone(CVHOST_SEQHAND *p_seq_handle);

/**
    De-initial SmartFrame sequence.

    @return status code.
*/
extern CVHOST_STAT CvHost_Close(CVHOST_SEQHAND *p_seq_handle);


#endif
