//#include "SysKer.h"
//#include "KerDump.h"
#include <stdio.h>
#include <stdlib.h>
#include <cyg/kernel/kapi.h>

typedef void (*os_exception_handler)(
    unsigned long data,                // user supplied data
    unsigned long exception_number,    // exception being raised
    unsigned long *exception_info      // any exception specific info
);

/*typedef void cyg_exception_handler_t(
    cyg_addrword_t data,
    cyg_code_t   exception_number,
    cyg_addrword_t info
);*/

static void os_sys_default_exception_handle(unsigned long data, unsigned long num, unsigned long *info)
{
    cyg_handle_t    thread_h;
    cyg_thread_info thread_info;
    unsigned long val;

    thread_h = cyg_thread_self();
    cyg_thread_get_info(thread_h, cyg_thread_get_id(thread_h), &thread_info);

    asm volatile ( "di;" );

    HAL_GET_CP0_REGISTER_32(val, 12, 0);

    val &= ~0x01;
    val |= 0x06;

    HAL_SET_CP0_REGISTER_32(val, 12, 0);

    printf("^R*** CPU Exception! cause=0x%08x [EPC: 0x%08lx]\r\n", num, *(info+36));

    asm volatile ( "wait; nop; nop; nop;" );
}

int OS_RegSysException(unsigned long os_exception_number, os_exception_handler handler, unsigned long u32Data)
{
    cyg_exception_handler_t *old_handler;
    cyg_addrword_t old_data;

    if (CYGNUM_HAL_VSR_MAX < os_exception_number)
    {
        printf("Exception number NG!\r\n");
        return -1;
    }

    if (NULL == handler)
    {
        cyg_exception_set_handler(
            os_exception_number,
            (cyg_exception_handler_t *)&os_sys_default_exception_handle,
            (cyg_addrword_t)&u32Data,
            &old_handler,
            &old_data);
    }
    else
    {
        cyg_exception_set_handler(
            os_exception_number,
            (cyg_exception_handler_t *)&handler,
            (cyg_addrword_t)&u32Data,
            &old_handler,
            &old_data);
    }

    return 0;
}

