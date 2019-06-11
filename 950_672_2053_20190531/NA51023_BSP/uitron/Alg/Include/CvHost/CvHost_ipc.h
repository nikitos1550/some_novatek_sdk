#ifndef _CVHOST_IPC_H
#define _CVHOST_IPC_H


#include "Type.h"
#include "CvHost_SmartFrame.h"

#define CVHOST_CVHOST_VER_MAJOR     1
#define CVHOST_CVHOST_VER_MINOR     1
#define CVHOST_CVHOST_VER_BUILD     0

#define CVHOST_KERNEL_NUM_MAX       3
#define CVHOST_KERNEL_PRMS_MAX      10
#define CVHOST_KERNEL_SIZE_MAX      (17*17)     // in bytes
#define CVHOST_KERNEL_NAME_MAXLEN   100
#define CVHOST_SRC_FRAM_NUM_MAX     3
#define CVHOST_DST_FRAM_NUM_MAX     3


typedef enum _CVHOST_STAT
{
    // MATCH with CEVA definitions
    CVHOST_SUCCEED                          = 0x0000,   //!< Function returned successfully
    CVHOST_FAILED_INVALID_MEMSPACE          = 0x0001,   //!< Memory space invalid
    CVHOST_FAILED_INVALID_SEQ               = 0x0002,   //!< Invalid sequence handle
    CVHOST_FAILED_INVALID_KERNEL            = 0x0004,   //!< Invalid kernel handle
    CVHOST_FAILED_LINK                      = 0x0008,   //!< Failed to link
    CVHOST_FAILED_DATA_TYPE                 = 0x0010,   //!< Invalid data type
    CVHOST_FAILED_NUM_DST_SRC               = 0x0020,   //!< Invalid number
    CVHOST_FAILED_COUNT_OF_PADDING          = 0x0040,   //!< Invalid padding count
    CVHOST_FAILED_LIMITED_DATA              = 0x0080,   //!< ???
    CVHOST_FAILED_TILE_SIZE                 = 0x0100,   //!< Invalid tile size
    CVHOST_FAILED_INVALID_NUM_OF_SET_PARAMS = 0x0200,   //!< Failed to set parameter
    CVHOST_FAILED_INVALID_SIZE              = 0x0400,   //!< Invalid size
    CVHOST_FAILED_NOT_VERIFIED              = 0x0800,   //!< Sequence should be verified first
    CVHOST_FAILED_INVALID_NUM_OF_GET_PARAMS = 0x1000,   //!< Failed to get parameter
    CVHOST_FAILED                           = 0xffff,   //!< General failure

    // NVT definitions
    CVHOST_FAILED_CEVA_VERSION_NOT_MATCH    = 0X7000,  // 28672
    CVHOST_FAILED_NVTLIB_VERSION_NOT_MATCH,
    CVHOST_FAILED_INVALID_KERNEL_INDEX,
    CVHOST_FAILED_INVALID_KERNEL_PRM_INDEX,
    CVHOST_FAILED_INVALID_KERNEL_TABLE_SIZE,
    CVHOST_FAILED_INVALID_SEQPRM,
    CVHOST_FAILED_INVALID_FRAME_NUM,
    CVHOST_FALIED_KERNEL_PRM_DOUBLE_SET,
    CVHOST_FALIED_KERNEL_NAME_TOO_LONG,
    CVHOST_FAILED_WORKMEM_TOO_SMALL,
    CVHOST_FALIED_IPC_LINK,
    CVHOST_FAILED_INSTALL,

    ENUM_DUMMY4WORD(CVHOST_STAT)
} CVHOST_STAT;


typedef struct _CVHOST_SEQHAND
{
    UINT8                kernelFlag[CVHOST_KERNEL_NUM_MAX];
    CHAR                 kernel0[CVHOST_KERNEL_NAME_MAXLEN];
    CHAR                 kernel1[CVHOST_KERNEL_NAME_MAXLEN];
    CHAR                 kernel2[CVHOST_KERNEL_NAME_MAXLEN];
    sf_data_type_e       src_data_type[CVHOST_KERNEL_NUM_MAX];        // kernel input data type
    sf_data_type_e       dst_data_type[CVHOST_KERNEL_NUM_MAX];        // kernel output data type

    UINT32               kernel0_prms[CVHOST_KERNEL_PRMS_MAX];
    UINT32               kernel1_prms[CVHOST_KERNEL_PRMS_MAX];
    UINT32               kernel2_prms[CVHOST_KERNEL_PRMS_MAX];

    UINT8                kernelTableIndex[3];
    UINT8                kernelTable0[CVHOST_KERNEL_SIZE_MAX];        // only allowed one userdefined table for each kernel
    UINT8                kernelTable1[CVHOST_KERNEL_SIZE_MAX];        // only allowed one userdefined table for each kernel
    UINT8                kernelTable2[CVHOST_KERNEL_SIZE_MAX];        // only allowed one userdefined table for each kernel

    UINT8                seq_prmSetFlag[3];     // setting-flags of seq_padtype, seq_padval and seq_priority
    sf_padding_type_e    seq_padtype;
    UINT32               seq_padval;
    UINT32               seq_priority;

    UINT32               srcFrameNum;
    UINT32               srcFrameAddr[CVHOST_SRC_FRAM_NUM_MAX];
    UINT32               srcFrameCols[CVHOST_SRC_FRAM_NUM_MAX];
    UINT32               srcFrameRows[CVHOST_SRC_FRAM_NUM_MAX];
    UINT32               srcFrameOffs[CVHOST_SRC_FRAM_NUM_MAX];

    UINT32               dstFrameNum;
    UINT32               dstFrameAddr[CVHOST_DST_FRAM_NUM_MAX];
    UINT32               dstFrameCols[CVHOST_DST_FRAM_NUM_MAX];
    UINT32               dstFrameRows[CVHOST_DST_FRAM_NUM_MAX];
    UINT32               dstFrameOffs[CVHOST_DST_FRAM_NUM_MAX];

    UINT16               verCvHost[3];      // major, minor, build
    UINT16               verCeva[3];        // major, minor, build
} CVHOST_SEQHAND;


#endif
