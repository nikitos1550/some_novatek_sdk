#include <stdio.h>
#include <stdlib.h>
#include "Type.h"
#include "test_main.h"
#include "init_ipc.h"
//#include "init_inet.h"
#include "init_cmd.h"
//#include <cyg/io/sdio_linux.h>

//missing part when linking with libstdc++.a
#include <sys/reent.h>
#ifndef __ATTRIBUTE_IMPURE_PTR__
#define __ATTRIBUTE_IMPURE_PTR__
#endif
#ifndef __ATTRIBUTE_IMPURE_DATA__
#define __ATTRIBUTE_IMPURE_DATA__
#endif
struct _reent __ATTRIBUTE_IMPURE_DATA__ _impure_data = _REENT_INIT(_impure_data);
struct _reent *__ATTRIBUTE_IMPURE_PTR__ _impure_ptr = &_impure_data;

#define PAUSE_FOR__DEBUG        0
#define REG_SYS_EXCEPTION       1
#define SUPPORT_IPC             1
#define SUPPORT_INET            0
//Start dhcp server or client from uIRON rather than start network at ecos initialization.
#define RUN_DHCP_IPC            0
#define RUN_TEST_MAIN           0

typedef void (*os_exception_handler)(
    UINT32 data,                // user supplied data
    UINT32 exception_number,    // exception being raised
    UINT32 *exception_info      // any exception specific info
);

extern INT32 OS_RegSysException(UINT32 os_exception_number, os_exception_handler handler, UINT32 u32Data);

//
//cyg_user_start() is appliction main entry for eCos platform
//
void cyg_user_start(void)
{
#if REG_SYS_EXCEPTION
    {
        unsigned int uiIdx;
        //Install default exception handler
        for (uiIdx=0; uiIdx<15; uiIdx++)
        {
            OS_RegSysException(uiIdx, NULL, 0);
        }
    }
#endif

#if PAUSE_FOR__DEBUG
    static int a=0;

    while(!a)
    {
        a=a+a;
    }
#endif
    printf("Hello, eCos world!!!!!!!!!!\n");

#if SUPPORT_IPC
    init_ipc();
#endif

#if (SUPPORT_INET && (0 == RUN_DHCP_IPC))
    printf("Start init_inet for ftpd\n");
    init_inet();
#else
    //Start dhcp server or client from uIRON rather than start network at ecos initialization.
    printf("==== NOT Start init_inet for ftpd ===\n");
#endif

    //command system
    init_cmd();

#if RUN_TEST_MAIN
    test_main();
#endif


    printf("Ready for eCos scheduling...\n");
}

