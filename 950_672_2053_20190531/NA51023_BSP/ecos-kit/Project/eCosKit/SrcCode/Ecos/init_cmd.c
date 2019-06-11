#include <stdio.h>
#include <stdlib.h>
#include <cyg/kernel/kapi.h>
#include <cyg/infra/maincmd.h>
#include <cyg/infra/mainfunc.h>
#include <cyg/infra/diag.h>
#include <cyg/cmd/CmdSysAPI.h>
#include <sys/types.h>
#include <pthread.h>
#include "PrjCfg.h"
//--------------------------------------------------------------------------

#define CFG_UART0_AS_CMD_SYS 1

#if(!CFG_UART0_AS_CMD_SYS)
static char thread_stack[PTHREAD_STACK_MIN+8192];
static int retval;

void *pthread_entry_maincmd(void *arg)
{
    retval = runmaincmd((char *)arg);

    pthread_exit( (void *)&retval );
}

int runmain_as_pthread(char *cmdstr)
{
    if(strncmp("?",cmdstr,1)==0)
    {
        listmaincmd();
        return 0;
    }
    pthread_t thread;
    pthread_attr_t attr;
    struct sched_param schedparam;
    int *pretval;

    pthread_attr_init( &attr );
    schedparam.sched_priority = 10;
    pthread_attr_setinheritsched( &attr, PTHREAD_EXPLICIT_SCHED );
    pthread_attr_setschedpolicy( &attr, SCHED_RR );
    pthread_attr_setschedparam( &attr, &schedparam );
    pthread_attr_setstackaddr( &attr, (void *)&thread_stack[sizeof(thread_stack)] );
    pthread_attr_setstacksize( &attr, sizeof(thread_stack) );

    pthread_create( &thread,
                    &attr,
                    pthread_entry_maincmd,
                    (void *)cmdstr);

    pthread_join( thread, (void **)&pretval );

    return *pretval;
}
#endif
//--------------------------------------------------------------------------
/* and now variable (prototype) for the procedure which is the thread itself */
cyg_thread_entry_t runmain_program;

void runmain_program(cyg_addrword_t data)
{
   char cmdstr[256] = "";
   listmaincmd();

   while (1)
   {
        printf("> ");
        gets(cmdstr);
        printf("Command input is: ");
        puts(cmdstr);

        if (strncmp("quit",cmdstr,4)==0)
        {
            printf("Bye!\n");
            break;
        }
        else
        {
            runmain_as_pthread(cmdstr);
        }
   }
}

void init_cmd(void)
{
    #if(!CFG_UART0_AS_CMD_SYS)
    static cyg_thread runmain_thread_s; /* space for thread object */
    static char runmain_stack[4096]; /* 4K stack space for the thread */
    static cyg_handle_t runmain_thread;  /* now the handles for the thread */

    //we have to start a thread to do actual test job and not to do here directly
    {
    cyg_thread_create(27, runmain_program, (cyg_addrword_t) 0,
            "Runmain Thread", (void *) runmain_stack, sizeof(runmain_stack),
            &runmain_thread, &runmain_thread_s);

    cyg_thread_resume(runmain_thread);
    }
    #endif
}

#if (MSG_OUTPUT_UART1_DIRECT == DISABLE)
void cyg_hal_plf_serial_user_putc(void* __ch_data, cyg_uint8 __ch)
{
}
#endif