#include <stdio.h>
#include <stdlib.h>
#include <cyg/kernel/kapi.h>
#include <cyg/infra/maincmd.h>
#include <cyg/infra/mainfunc.h>
#include <cyg/infra/diag.h>
#include <cyg/cpuload/cpuload.h>
#include <unistd.h>

#define THREAD_DECLARE(name,arglist) static void  name (cyg_addrword_t arglist)
#define THREAD_HANDLE cyg_handle_t
#define THREAD_OBJ cyg_thread
#define THREAD_CREATE(name,thread_handle,fp,p_data,p_stack,size_stack,p_thread_obj)  cyg_thread_create(22,fp,(cyg_addrword_t)p_data,name,(void*)p_stack,size_stack,&thread_handle,p_thread_obj)
#define THREAD_RESUME(thread_handle) cyg_thread_resume(thread_handle)
#define THREAD_SUSPEND(thread_handle) cyg_thread_suspend(thread_handle)
#define THREAD_DESTROY(thread_handle) cyg_thread_suspend(thread_handle);while(!cyg_thread_delete(thread_handle)){cyg_thread_delay(1);}
#define THREAD_RETURN return
#define THREAD_EXIT() cyg_thread_exit()

typedef struct _USAGE_CPU_CTRL{
    THREAD_HANDLE   hThread;
    THREAD_OBJ      hThread_Obj;
    u_int32_t       Stack[(PTHREAD_STACK_MIN+1024)/sizeof(u_int32_t)];
    u_int32_t       uiIntervalTime;
}USAGE_CPU_CTRL;

USAGE_CPU_CTRL m_UsageCpuCtrl = {0};

THREAD_DECLARE(UsageCpuThreadProc,lpParam)
{
    while(1)
    {
        cyg_cpuload_start();
        usleep(m_UsageCpuCtrl.uiIntervalTime*1000);
        diag_printf("%d\r\n",cyg_cpuload_get());
    }
    THREAD_EXIT();
}

MAINFUNC_ENTRY(usage,argc,argv)
{
    u_int32_t isValid = 0;

    if(argc>1)
    {
        if(strcmp(argv[1],"cpucfg")==0 && argc==3)
        {
            m_UsageCpuCtrl.uiIntervalTime = atoi(argv[2]);
            diag_printf("set interval time = %d ms\r\n", m_UsageCpuCtrl.uiIntervalTime);
            isValid = 1;
        }
        else if(strcmp(argv[1],"cpustart")==0)
        {
            if(m_UsageCpuCtrl.uiIntervalTime == 0)
            {
                diag_printf("use default interval time = 1000 ms\r\n");
                m_UsageCpuCtrl.uiIntervalTime = 1000;
            }
            if(m_UsageCpuCtrl.hThread == 0)
            {
                u_int32_t uiStackSize = sizeof(m_UsageCpuCtrl.Stack);
                THREAD_CREATE("USAGE_CPU",m_UsageCpuCtrl.hThread,UsageCpuThreadProc,NULL,m_UsageCpuCtrl.Stack,uiStackSize,&m_UsageCpuCtrl.hThread_Obj);
                THREAD_RESUME(m_UsageCpuCtrl.hThread);
            }
            THREAD_RESUME(m_UsageCpuCtrl.hThread);
            isValid = 1;
        }
        else if(strcmp(argv[1],"cpustop")==0)
        {
            THREAD_SUSPEND(m_UsageCpuCtrl.hThread);
            isValid = 1;
        }
    }


    if(!isValid)
    {
        diag_printf("'usage cpucfg [interval time]' to set interval time.\r\n");
        diag_printf("'usage cpustart' to start evaluate\r\n");
        diag_printf("'usage cpustop' to stop\r\n");
    }
    return 0;
}