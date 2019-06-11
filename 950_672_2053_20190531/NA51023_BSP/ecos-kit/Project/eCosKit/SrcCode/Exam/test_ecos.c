#include <stdio.h>
#include <stdlib.h>
#include <cyg/kernel/kapi.h>
#include <time.h>

#define CFG_TWIN_TEST_THREADS   0
#define CFG_TIME_TEST           0
#define CFG_BUSY_TEST           1

//check config
#if (CFG_TIME_TEST || CFG_BUSY_TEST)
    #if (CFG_TWIN_TEST_THREADS)
    #error "CFG_TWIN_TEST_THREADS shall be 0 for time test"
    #endif
#endif


/* now declare (and allocate space for) some kernel objects,
 like the thread we will use */

static cyg_thread test_thread_s; /* space for thread object */
static char test_stack[8192]; /* 8K stack space for the thread */
static cyg_handle_t test_thread;  /* now the handles for the thread */

#if CFG_TWIN_TEST_THREADS
static cyg_thread test2_thread_s; /* space for second thread object */
static char test2_stack[8192]; /* 8K stack space for second thread */
static cyg_handle_t test2_thread;  /* now the handles for the second thread */
#endif

/* and now variable (prototype) for the procedure which is the thread itself */
cyg_thread_entry_t test_program;
cyg_mutex_t cliblock;

/* this is the test thread where the actual test procedure goes */
void test_program(cyg_addrword_t data)
{

    int message = (int) data;
    int delay;

#if CFG_TIME_TEST
    printf("see the next message to start a 10 sec delay\r\n");
    printf("b: %llu, %d\r\n",cyg_current_time(),clock());
    cyg_thread_delay(10000);
    printf("e: %llu, %d\r\n",cyg_current_time(),clock());
#endif

#if CFG_BUSY_TEST
    {
        int i=0,j;
        int size = 0x400000;
        unsigned char* pSrc = (unsigned char*)0x9D000000;
        unsigned char* pDst = (unsigned char*)0x9D800000;
        while(1)
        {
            printf("testing(%d)...\r\n",i);
            memset(pSrc,0,size);
            //check zero
            for(j=0;j<size;j++)
            {
                if(pSrc[j]!=0)
                {
                    printf("..[FAIL] memset to 0 fail.\r\n");
                    while(1);
                }
            }
            //fill numbers
            for(j=0;j<size;j++)
            {
                pSrc[j] = (unsigned char)(j%256);
            }
            //check numbers
            for(j=0;j<size;j++)
            {
                if(pSrc[j]!=(unsigned char)(j%256))
                {
                    printf("..[FAIL] read numbers fail.\r\n");
                    while(1);
                }
            }
            //memcpy
            memcpy(pDst,pSrc,size);
            //check numbers
            if(memcmp(pDst,pSrc,size)!=0)
            {
                printf("..[FAIL] memcmp fail.\r\n");
                    while(1);
            }

            printf("..[PASS]\r\n");
            i++;
        }
    }
#endif


    cyg_mutex_lock(&cliblock);
    printf("Beginning test_program; thread data is %d\n", message);
    cyg_mutex_unlock(&cliblock);

    cyg_thread_delay(200);

    for (;;) {
        /* delay = 1000 + (rand() % 50); */
        delay = 5000*(1+message) + (rand() % 50);

        /* note: printf() must be protected by a
           call to cyg_mutex_lock() */
        cyg_mutex_lock(&cliblock);

        {
            cyg_tick_count_t tick;
            tick = cyg_current_time();
            printf("(%llu) Thread %d: and now a delay of %d clock ticks\n",
                        tick, message, delay);
        }

        cyg_mutex_unlock(&cliblock);
        cyg_thread_delay(delay);
    }
}

/* we install our own startup routine for the test thread */
void test_ecos(void)
{
    //we have to start a thread to do actual test job and not to do here directly
    cyg_thread_create(21, test_program, (cyg_addrword_t) 1,
            "Thread test", (void *) test_stack, 8192,
            &test_thread, &test_thread_s);

    cyg_thread_resume(test_thread);

#if CFG_TWIN_TEST_THREADS
    //we share same thread body here only for demonstration
    cyg_thread_create(4, test_program, (cyg_addrword_t) 2,
            "Thread test 2", (void *) test2_stack, 8192,
            &test2_thread, &test2_thread_s);

    cyg_thread_resume(test2_thread);
#endif
}
