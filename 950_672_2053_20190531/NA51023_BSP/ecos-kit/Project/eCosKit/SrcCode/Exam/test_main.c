#include <stdio.h>
#include <stdlib.h>
#include <cyg/infra/mainfunc.h>
#include "Type.h"

#define TEST_SIMPLE_TWO_PROGRAM 0
#define TEST_MAINCMD            0 //Command System, should open always.
#define TEST_INET               0 //nvtipc open + shared UART0 command system
#define TEST_POSIX              0
#define TEST_CLIB               0
#define TEST_WIFI               0
#define TEST_NVTIPC             0

void test_main(void)
{
    //example eCos application
#if TEST_SIMPLE_TWO_PROGRAM
    extern void test_ecos(void);
    test_ecos();
#endif

#if 0   //ftpd test has done this
    int err = mount( "", "/", "ramfs" );
    if( err < 0 ) printf( "mount fs fail %d\r\n", err );
#endif

#if TEST_WIFI
    rtw_init_wlan0_netdev();
#endif

#if TEST_MAINCMD
    extern void test_runmain(void);
    test_runmain();
#endif

#if TEST_INET
    extern void test_inet(void);
    test_inet();
#endif

#if TEST_POSIX
    extern void test_posix(void);
    test_posix();
#endif

#if TEST_CLIB
    extern void test_clib(void);
    test_clib();
#endif

#if TEST_NVTIPC
    extern void test_nvtipc(void);
    test_nvtipc();
#endif
}

#include <cyg/infra/mainfunc.h>
MAINFUNC_ENTRY(testmem,argc,argv)
{
    UINT32 uiAddr;
    UINT8* pDst = (UINT8*)0xB0000000;
    UINT32 Size = 0x1000000;

    if(argc!=2)
    {
        printf("e.g. testmem 0x85000000\r\n");
        return 0;
    }
    sscanf(argv[1],"0x%0X",&uiAddr);
    pDst = (UINT8*)uiAddr;
    printf("Replace 0x%08X to 0 with size 0x%X\r\n",pDst,Size);
    memset(pDst,0, Size);
    return 0;
}