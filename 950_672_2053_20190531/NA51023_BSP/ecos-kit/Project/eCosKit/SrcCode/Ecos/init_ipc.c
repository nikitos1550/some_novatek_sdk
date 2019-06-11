#include <stdio.h>
#include <stdlib.h>
#include "Type.h"
#include <cyg/nvtipc/NvtIpcAPI.h>
//#include "Live555Common.h"
//#include "Live555CliCommon.h"
//#include <cyg/hfs/hfs.h>
//#include <cyg/lviewd/lviewd.h>
#include <cyg/infra/diag.h>
#include <cyg/cmd/CmdSysAPI.h>
#include <cyg/fs/nvtfs.h>
//#include <cyg/nvtwifi/nvtwifi.h>
//#include <cyg/usocket/usocket_ipc.h>
//#include <cyg/usocket_cli/usocket_cli_ipc.h>
//#include <net/dhcpelios/dhcpelios.h>
//#include <YoukuIpc.h>
//#include "TutkIpcECOS.h"
//#include <axTLS/ssl.h>
//#include <axTLS/crypto.h>
//#include "curl/curl_tool_api.h"
//#include <cyg/sntp/sntp.h>
//#include <cyg/msdcnvt/msdcnvt.h>
#include <cyg/audcodec/audcodec.h>
#if defined(_ASR_ENABLE_)
#include <cyg/asr/asr.h>
#endif

//#NT#2016/04/21#Lincy Lin -begin
//#NT#Support curl (http client)
#if 0 //(_SSLTYPE_==_SSL_OFF_)
static const char * const ssl_unsupported_str = "Error: ssl Feature not supported\n";
EXP_FUNC void STDCALL __attribute__((weak)) MD5_Init(MD5_CTX *ctx)
{
    printf("%s",ssl_unsupported_str);
}

EXP_FUNC void STDCALL __attribute__((weak)) MD5_Update(MD5_CTX *ctx, const uint8_t * msg, int len)
{
    printf("%s",ssl_unsupported_str);
}

EXP_FUNC void STDCALL __attribute__((weak)) MD5_Final(uint8_t *digest, MD5_CTX *ctx)
{
    printf("%s",ssl_unsupported_str);
}
#endif
//#NT#2016/04/21#Lincy Lin -end

#if 1
static SYSCALL_CMD_TBL ipccmd_tbl[]={
{"audcodec",(SYSCALL_CMD_FP)AudCodec_CmdLine},
#if defined(_ASR_ENABLE_)
{"asr",(SYSCALL_CMD_FP)ASR_CmdLine},
#endif
{"",NULL}   // this item should be the last item
};
#endif

void init_ipc(void)
{
#if 1
    int     workbuf;
    NVTIPC_OPEN_OBJ   openObj={0};

    workbuf = (unsigned int)malloc(NvtIPC_CalBuffSize());
    if (!workbuf)
    {
        printf("can't alloc workbuf \r\n");
        return;
    }
    openObj.workBuffAddr = workbuf;
    openObj.workBuffSize = NvtIPC_CalBuffSize();
    openObj.SysCmdTbl = ipccmd_tbl;
    openObj.uartCmdFp = runmain_as_pthread;
    NvtIPC_Open(&openObj);
#endif
}
