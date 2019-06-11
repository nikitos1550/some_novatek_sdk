/*
(C) OOMusou 2006 http://oomusou.cnblogs.com

Filename    : timer.cpp
Compiler    : gcc 4.1.0 on Fedora Core 5
Description : setitimer() set the interval to run function
Synopsis    : #include <sys/time.h>
              int setitimer(int which, const struct itimerval *value, struct itimerval *ovalue);
              struct itimerval {
                struct timerval it_interval;
                struct timerval it_value;
              };

              struct timeval {
                long tv_sec;
                long tv_usec;
              }
Release     : 11/25/2006
*/
#include <stdio.h>    // for printf()
#include <stdlib.h>   // for exit()
#include <unistd.h>   // for pause()
#include <signal.h>   // for signal()
#include <string.h>   // for memset()
#include <sys/time.h> // struct itimeral. setitimer()
#include <cyg/kernel/kapi.h>

//============================================================================

#define USE_POSIX_MAIN      1
#define USE_POSIX_THREAD    1 //must use posix thread in order to make siganl work

#define TEST_DELAY      0
#define TEST_SETITIMER  1
#define TEST_ALARM      1

static alarm_wakeup = 0;

//============================================================================
#define INTERVAL1 1

void printMsg(int num) {
  printf("[%d]%s",num,"Hello SIGALRM!!\n");
  alarm_wakeup = 1;
}

int setitimer_test_main(void)
{
  // Get system call result to determine successful or failed
  int res = 0;

  printf("setitimer_test_main for %d seconds\n",INTERVAL1);

  // Register printMsg to SIGALRM
  signal(SIGALRM, printMsg);

  struct itimerval tick;
  // Initialize struct
  memset(&tick, 0, sizeof(tick));
  // Timeout to run function first time
  tick.it_value.tv_sec = INTERVAL1;  // sec
  tick.it_value.tv_usec = 0; // micro sec.
  // Interval time to run function
#if 0
  tick.it_interval.tv_sec = 0;
  tick.it_interval.tv_usec = 0;
#else
  tick.it_interval.tv_sec = INTERVAL1;
  tick.it_interval.tv_usec = 0;
#endif
  // Set timer, ITIMER_REAL : real-time to decrease timer,
  //                          send SIGALRM when timeout
  res = setitimer(ITIMER_REAL, &tick, NULL);
  if (res) {
    printf("Set timer failed!!\n");
  }

  printf("setitimer_test_main - wait SIGALRM\n");
  // Always sleep to catch SIGALRM signal
  while(1) {
    alarm_wakeup = 0;
    pause();
  }
  printf("setitimer_test_main - sigalled\n");

  return 0;
}

//============================================================================
#define INTERVAL2      10

void alarm_wakeup2 (int i)
{
   printf("alarm_wakeup2 %d sec signal wakeup!!!\n",INTERVAL2);
   alarm_wakeup = 1;
   //exit(0);
}

/*
Output:
  10 sec up partner, Wakeup!!!
*/
int setitimer_test2_main(void)
{
  struct itimerval tout_val;

  printf("setitimer_test2_main for %d seconds\n",INTERVAL2);

  alarm_wakeup = 0;

  signal(SIGALRM,alarm_wakeup2); /* set the Alarm signal capture */

  tout_val.it_interval.tv_sec = 0;
  tout_val.it_interval.tv_usec = 0;
  tout_val.it_value.tv_sec = INTERVAL2; /* set timer for "INTERVAL (10) seconds */
  tout_val.it_value.tv_usec = 0;
  setitimer(ITIMER_REAL, &tout_val,0);

#if 0
  while (!alarm_wakeup)
  {
  }
#else
  pause();
  printf("setitimer wakeup after pause\n");
#endif

  return 0;

}

//============================================================================

#define INTERVAL3 5

int howmany = 0;

void alarm_wakeup3 (int i)
{
   struct itimerval tout_val;

   signal(SIGALRM,alarm_wakeup3);

   howmany += INTERVAL3;

   printf("%d sec up partner, Wakeup!!!\n",howmany);
   tout_val.it_interval.tv_sec = 0;
   tout_val.it_interval.tv_usec = 0;
   tout_val.it_value.tv_sec = INTERVAL3; /* 10 seconds timer */
   tout_val.it_value.tv_usec = 0;

   setitimer(ITIMER_REAL, &tout_val,0);

}

void exit_func3 (int i)
{
    signal(SIGINT,exit_func3);
    printf("Bye Bye!!!\n");
    exit(0);
}

/*
Output:
  5 sec up partner, Wakeup!!!
  10 sec up partner, Wakeup!!!
  15 sec up partner, Wakeup!!!
  20 sec up partner, Wakeup!!!
  ^C
  Bye Bye!!!
*/
int setitimer_test3_main(void)
{
  struct itimerval tout_val;

  printf("setitimer_test3_main for %d seconds loop\n",INTERVAL3);

  tout_val.it_interval.tv_sec = 0;
  tout_val.it_interval.tv_usec = 0;
  tout_val.it_value.tv_sec = INTERVAL3; /* 10 seconds timer */
  tout_val.it_value.tv_usec = 0;
  setitimer(ITIMER_REAL, &tout_val,0);

  signal(SIGALRM,alarm_wakeup3); /* set the Alarm signal capture */
  signal(SIGINT,exit_func3);

  printf("setitimer_test3_main - wait SIGALRM\n");
  while (1)
  {
    //printf("!");
  }

  return 0;

}

//============================================================================

/*static*/ void sig_alrm(int signumber)
{
    printf("sig_alrm(%d)!\n",signumber);
    return;
}

int alarm_test_main(void)
{
    printf("alarm_test_main for 10 seconds\n");

    if(signal(SIGALRM,sig_alrm)==SIG_ERR) {
        printf("can't catch SIGALRM!");
    }

    alarm(10);

    pause();

    printf("alarm wakeup after pause\n");

    // do your thing

    return 0;
}

//============================================================================

static char testposix_stack[8192]; /* 8K stack space for the thread */

#if USE_POSIX_THREAD
#include <sys/types.h>
#include <pthread.h>
static int retval = 0;
void *testposix_program(void *arg)
#else
static cyg_handle_t testposix_thread;  /* now the handles for the thread */
static cyg_thread testposix_thread_s; /* space for thread object */
/* and now variable (prototype) for the procedure which is the thread itself */
cyg_thread_entry_t testposix_program;
void testposix_program(cyg_addrword_t data)
#endif
{
#if TEST_DELAY
    clock_t start_t, end_t;
    double total_t;
    struct timeval tv,tv2;

    start_t = clock();
    gettimeofday(&tv,0);
    cyg_thread_delay(400);  //CYGNUM_HAL_RTC_DENOMINATOR/10
    gettimeofday(&tv2,0);
    end_t = clock();
    printf("cyg_thread_delay(400):(%d,%d)\n",tv.tv_sec, tv.tv_usec);
    printf("cyg_thread_delay(400) done:(%d,%d)\n",tv2.tv_sec, tv2.tv_usec);
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("Total time taken by CPU: start:%ld,end:%ld;%f\n", start_t, end_t, total_t  );

    start_t = clock();
    gettimeofday(&tv,0);
    cyg_thread_delay(4000);  //CYGNUM_HAL_RTC_DENOMINATOR
    gettimeofday(&tv,0);
    end_t = clock();
    printf("cyg_thread_delay(4000):(%d,%d)\n",tv.tv_sec, tv.tv_usec);
    printf("cyg_thread_delay(4000) done:(%d,%d)\n",tv2.tv_sec, tv2.tv_usec);
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("Total time taken by CPU: start:%ld,end:%ld;%f\n", start_t, end_t, total_t  );

    start_t = clock();
    gettimeofday(&tv,0);
    cyg_thread_delay(40000);  //10*CYGNUM_HAL_RTC_DENOMINATOR
    gettimeofday(&tv,0);
    end_t = clock();
    printf("cyg_thread_delay(40000):(%d,%d)\n",tv.tv_sec, tv.tv_usec);
    printf("cyg_thread_delay(40000) done:(%d,%d)\n",tv2.tv_sec, tv2.tv_usec);
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("Total time taken by CPU: start:%ld,end:%ld;%f\n", start_t, end_t, total_t  );

    gettimeofday(&tv,0);
    HAL_DELAY_US(20);
    gettimeofday(&tv2,0);
    printf("HAL_DELAY_US(20):(%d,%d)\n",tv.tv_sec, tv.tv_usec);
    printf("HAL_DELAY_US(20) done:(%d,%d)\n",tv2.tv_sec, tv2.tv_usec);

    gettimeofday(&tv,0);
    HAL_DELAY_US(1000);
    gettimeofday(&tv2,0);
    printf("HAL_DELAY_US(1000):(%d,%d)\n",tv.tv_sec, tv.tv_usec);
    printf("HAL_DELAY_US(1000) done:(%d,%d)\n",tv2.tv_sec, tv2.tv_usec);

    gettimeofday(&tv,0);
    HAL_DELAY_US(100000);
    gettimeofday(&tv2,0);
    printf("HAL_DELAY_US(100000):(%d,%d)\n",tv.tv_sec, tv.tv_usec);
    printf("HAL_DELAY_US(100000) done:(%d,%d)\n",tv2.tv_sec, tv2.tv_usec);

    gettimeofday(&tv,0);
    HAL_DELAY_US(1000000);
    gettimeofday(&tv2,0);
    printf("HAL_DELAY_US(1000000):(%d,%d)\n",tv.tv_sec, tv.tv_usec);
    printf("HAL_DELAY_US(1000000) done:(%d,%d)\n",tv2.tv_sec, tv2.tv_usec);

    gettimeofday(&tv,0);
    HAL_DELAY_US(10000000);
    gettimeofday(&tv2,0);
    printf("HAL_DELAY_US(10000000):(%d,%d)\n",tv.tv_sec, tv.tv_usec);
    printf("HAL_DELAY_US(10000000) done:(%d,%d)\n",tv2.tv_sec, tv2.tv_usec);

 #if 0  //we have no Delay_DelayMs() in pure eCos platform
    gettimeofday(&tv,0);
    Delay_DelayMs(100);
    gettimeofday(&tv2,0);
    printf("Delay_DelayMs(100):(%d,%d)\n",tv.tv_sec, tv.tv_usec);
    printf("Delay_DelayMs(100) done:(%d,%d)\n",tv2.tv_sec, tv2.tv_usec);

    gettimeofday(&tv,0);
    Delay_DelayMs(1000);
    gettimeofday(&tv2,0);
    printf("Delay_DelayMs(1000):(%d,%d)\n",tv.tv_sec, tv.tv_usec);
    printf("Delay_DelayMs(1000) done:(%d,%d)\n",tv2.tv_sec, tv2.tv_usec);

    gettimeofday(&tv,0);
    Delay_DelayMs(10000);
    gettimeofday(&tv2,0);
    printf("Delay_DelayMs(10000):(%d,%d)\n",tv.tv_sec, tv.tv_usec);
    printf("Delay_DelayMs(10000) done:(%d,%d)\n",tv2.tv_sec, tv2.tv_usec);
#endif
#endif

    while (1)
    {
#if TEST_SETITIMER
    //setitimer_test_main();
    setitimer_test2_main();
    //setitimer_test3_main();
#endif

#if TEST_ALARM
    alarm_test_main();
#endif
    }

#if USE_POSIX_THREAD
    pthread_exit( (void *)&retval );
#endif
}

void test_posix(void)
{
    //we have to start a thread to do actual test job and not to do here directly
#if USE_POSIX_THREAD
    pthread_t thread;
    pthread_attr_t attr;
    struct sched_param schedparam;

    pthread_attr_init( &attr );
    schedparam.sched_priority = 10;
    pthread_attr_setinheritsched( &attr, PTHREAD_EXPLICIT_SCHED );
    pthread_attr_setschedpolicy( &attr, SCHED_RR );
    pthread_attr_setschedparam( &attr, &schedparam );
    pthread_attr_setstackaddr( &attr, (void *)&testposix_stack[sizeof(testposix_stack)] );
    pthread_attr_setstacksize( &attr, sizeof(testposix_stack) );

    pthread_create( &thread,
                    &attr,
                    testposix_program,
                    (void *)NULL);
#else
    cyg_thread_create(5, testposix_program, (cyg_addrword_t) 0,
            "Testposix Thread", (void *) testposix_stack, sizeof(testposix_stack),
            &testposix_thread, &testposix_thread_s);

    cyg_thread_resume(testposix_thread);
#endif
}

#if USE_POSIX_MAIN
//Posix main runs in a thread
int main(int argc, char **argv)
{
    printf("User supplied posix main!!!\n");

    //We are actually in task scheduling here

    return 0;
}
#endif

