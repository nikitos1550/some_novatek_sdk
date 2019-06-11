#ifndef _NVTPACKINFO_H
#define _NVTPACKINFO_H

#define HEADER_CHKSUM_VERSION 0x16040719U

#if !defined(_WIN32)
typedef struct _GUID {
    UINT32   Data1;
    UINT16   Data2;
    UINT16   Data3;
    UINT8    Data4[8];
} GUID;
#endif

typedef struct _HEADER_RES{
    GUID    guid;
    UINT32  TotalSize;
    UINT32  CheckSum;
    UINT32  ContentSize;
    char    SrcName[16];  ///< original packed bin file name
    char    FileName[16]; ///< target file name
}HEADER_RES;

typedef struct _HEADER_FW{
    GUID    guid;
    UINT32  TotalSize;
    UINT32  CheckSum;
    UINT32  TotalRes;
}HEADER_FW;

typedef struct _HEADER_PARTITION{
    UINT32  Offset;
    UINT32  Size;
    UINT32  PartitionID;
}HEADER_PARTITION;

typedef struct _HEADER_CHKSUM {
    UINT32 uiFourCC ;	///< 'C','K','S','M'
    UINT32 uiVersion;	///< HEADER_CHKSUM header version
    UINT32 uiChkMethod;	///< 0: CheckSum
    UINT32 uiChkValue;	///< check sum value or crc value within HEADER_CHKSUM
    UINT32 uiDataOffset;///< real data starting offset without HEADER_CHKSUM
    UINT32 uiDataSize;  ///< real data size without HEADER_CHKSUM
    UINT32 uiPaddingSize; ///< padding bytes for check sum
    UINT32 uiEmbType;   ///< relate to EMBTYPE_?????
    UINT32 uiReversed[8]; ///< align to 64 bytes
} HEADER_CHKSUM, *PHEADER_CHKSUM;

#ifndef IsEqualGUID
#define IsEqualGUID(rguid1, rguid2) (!memcmp(rguid1, rguid2, sizeof(GUID)))
#endif

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3) ((UINT32)(UINT8)(ch0) | ((UINT32)(UINT8)(ch1) << 8) | ((UINT32)(UINT8)(ch2) << 16) | ((UINT32)(UINT8)(ch3) << 24 ))
#endif
#endif