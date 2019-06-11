#include "Type.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <cyg/infra/mainfunc.h>
#include <cyg/nvtipc/NvtIpcAPI.h>
#include <cyg/kernel/kapi.h>
//#include <cyg/hfs/hfs.h>
//#include <cyg/lviewd/lviewd.h>
#include <cyg/infra/diag.h>

#if 0

#define __COREIPC__   1

#define THREAD_DECLARE(name,arglist) static void  name (cyg_addrword_t arglist)
#define THREAD_HANDLE cyg_handle_t
#define THREAD_OBJ cyg_thread
#define THREAD_CREATE(pri, name,thread_handle,fp,p_data,p_stack,size_stack,p_thread_obj)  cyg_thread_create(pri ,fp,(cyg_addrword_t)p_data,name,(void*)p_stack,size_stack,&thread_handle,p_thread_obj)
#define THREAD_RESUME(thread_handle) cyg_thread_resume(thread_handle)
#define THREAD_DESTROY(thread_handle) cyg_thread_suspend(thread_handle);while(!cyg_thread_delete(thread_handle)){/*cyg_thread_delay(1);*/}
#define THREAD_RETURN(thread_handle) cyg_thread_exit()


#define DBG_COLOR_RED "^R"
#define DBG_COLOR_BLUE "^B"
#define DBG_COLOR_YELLOW "^Y"

#define DBG_ERR(fmtstr, args...) diag_printf(DBG_COLOR_RED "ERR:%s() " fmtstr, \
    __func__, ##args)

#define DBG_WRN(fmtstr, args...) diag_printf(DBG_COLOR_YELLOW "WRN:%s() " fmtstr, \
    __func__, ##args)

#if 0
#define DBG_IND(fmtstr, args...)
#define DBG_MSG(fmtstr, args...)
#else
#define DBG_MSG(fmtstr, args...) diag_printf(fmtstr, ##args)
#define DBG_IND(fmtstr, args...) diag_printf("%s(): " fmtstr, \
    __func__, ##args)
#endif

#define DBG_DUMP(fmtstr, args...) diag_printf(fmtstr, ##args)

#define MSQ_NUM 2

#define RANDOM_RANGE(n)     (rand() % (n))

#if __COREIPC__
#define ipc_getPhyAddr(addr)        ((addr) & 0x1FFFFFFF)
#define ipc_getNonCacheAddr(addr)   (((addr) & 0x1FFFFFFF)|0xA0000000)
#else
#define ipc_getPhyAddr(addr)        (addr)
#define ipc_getNonCacheAddr(addr)   (addr)
#endif


static THREAD_HANDLE   Thread1_1;
static THREAD_OBJ      Thread1_1_object;
static UINT8           _ALIGNED(4) Thread1_1Stacks[4096];
static THREAD_HANDLE   Thread1_2;
static THREAD_OBJ      Thread1_2_object;
static UINT8           _ALIGNED(4) Thread1_2Stacks[4096];
static THREAD_HANDLE   Thread1_3;
static THREAD_OBJ      Thread1_3_object;
static UINT8           _ALIGNED(4) Thread1_3Stacks[4096];
static THREAD_HANDLE   Thread1_4;
static THREAD_OBJ      Thread1_4_object;
static UINT8           _ALIGNED(4) Thread1_4Stacks[4096];


static unsigned int   xMsqID[MSQ_NUM]={0};
static unsigned int   xindexIn[MSQ_NUM];
static unsigned int   xindexOut[MSQ_NUM];
static int            gCount=1000;
static int            gdelayMS=10;
static unsigned int   gMsqID;
static UINT32         gSendTo = NVTIPC_SENDTO_CORE1;
static int            gbQueue1RcvCnt = 0;
static int            gbQueue2RcvCnt = 0;


// test command
#define  TEST_MAIN_CMD_ID     0
#define  TEST_SIMPLE_CMD_ID   1
#define  TEST_RACE_CMD_ID     2
#define  TEST_3CORE_RACE_CMD_ID     3

#define  TEST_ERROR           0xFF



static void lviewd_fp(char* cmd);
static void hfs_fp(char* cmd);

SYSCALL_CMD_TBL Syscmd_tbl[]={
{"lviewd", lviewd_fp},
{"hfs",    hfs_fp},
{"",NULL}   // this item should be the last item
};

UINT32 drv_getSysTick(void)
{
    #define TIMER0_COUNTER_REG  0xC0040108

    volatile UINT32* p1 = (volatile UINT32*)(TIMER0_COUNTER_REG);

    return *p1;
}
static void xInitBuff(unsigned int pTmpBuf)
{
    unsigned int i,j,Pattern,Size_Write=0x10000,pBuf;
    char*  pMark;

    for (i=0;i<MSQ_NUM;i++)
    {
        xindexIn[i] = pTmpBuf+i*Size_Write;
        xindexOut[i] = xindexIn[i];
        Pattern = 1 << i;
        pBuf = xindexIn[i];
        memset((void*)pBuf, Pattern, Size_Write);
        for (j=0;j<Size_Write;j+=32)
        {
            pMark = (char*)((unsigned int)pBuf+j);
            *pMark = 0xFF;
        }
    }
    DBG_IND("xindexIn[0]=0x%x,[1]=0x%x\r\n",xindexIn[0],xindexIn[1]);
}

THREAD_DECLARE(thread1_1_main,arg)
{
    int i,j,count=gCount;
    int testNum = NVTIPC_MSG_ELEMENT_SIZE/4;
    int dataSize = testNum*4;
    int test[NVTIPC_MSG_ELEMENT_SIZE/4],msqindex=0;
    unsigned int *pOutBuf = (unsigned int *)xindexOut[msqindex];
    int  rcvSize;
    for(i=0;i<count;i++)
    {
        rcvSize = NvtIPC_MsgRcv(xMsqID[msqindex],&test,dataSize);
        if (rcvSize < 0)
        {
            DBG_ERR("rcvSize %d\r\n",rcvSize);
            break;
        }
        #if 0
        DBG_IND("Rcv ");
        for (j=0;j<rcvSize/4;j++)
        {
            DBG_MSG("t%d=0x%x ",j,test[j]);
        }
        DBG_MSG("\r\n");
        #else
        for (j=0;j<rcvSize/4;j++)
        {
            if ((unsigned int)test[j] != (*pOutBuf))
            {
                DBG_ERR("rcv =0x%x, pOutBuf = 0x%x, value=0x%x\r\n",test[j],(int)pOutBuf,*pOutBuf);
                while (1);
            }
            pOutBuf++;
        }
        #endif
        DBG_MSG("r1 i=%d\r\n",i);
        gbQueue1RcvCnt++;
    }
    THREAD_RETURN(thread1_1);
}
THREAD_DECLARE(thread1_2_main,arg)
{

    int i,j,count=gCount;
    int testNum = NVTIPC_MSG_ELEMENT_SIZE/4;
    int dataSize = testNum*4;
    int test[NVTIPC_MSG_ELEMENT_SIZE/4],msqindex=1;
    unsigned int *pOutBuf = (unsigned int *)xindexOut[msqindex];
    int  rcvSize;
    for(i=0;i<count;i++)
    {
        //cyg_thread_delay(10);
        rcvSize = NvtIPC_MsgRcv(xMsqID[msqindex],&test,dataSize);
        if (rcvSize < 0)
        {
            DBG_ERR("rcvSize %d\r\n",rcvSize);
            break;
        }
        #if 0
        DBG_IND("Rcv ");
        for (j=0;j<rcvSize/4;j++)
        {
            DBG_MSG("t%d=0x%x ",j,test[j]);
        }
        DBG_MSG("\r\n");
        #else
        for (j=0;j<rcvSize/4;j++)
        {
            if ((unsigned int)test[j] != (*pOutBuf))
            {
                DBG_ERR("rcv =0x%x, pOutBuf = 0x%x, value=0x%x\r\n",test[j],(int)pOutBuf,*pOutBuf);
                while (1);
            }
            pOutBuf++;
        }
        #endif
        DBG_MSG("r2 i=%d\r\n",i);
        gbQueue2RcvCnt++;
    }
    THREAD_RETURN(thread1_2);
}

THREAD_DECLARE(thread1_3_main,arg)
{
    int key,i;
    int sndSize,rcvSize;
    unsigned int test[NVTIPC_MSG_ELEMENT_SIZE/4]={0};


    key = NvtIPC_Ftok("t2");
    gMsqID = NvtIPC_MsgGet(key);

    DBG_DUMP("-------- Test  error case begin ------\r\n");
    cyg_thread_delay(1000);

    // send to a invalid queue id
    sndSize = NvtIPC_MsgSnd(4, gSendTo, &test, sizeof(test));
    // receive from a invalid queue id
    rcvSize = NvtIPC_MsgRcv(4,&test, sizeof(test));
    // send msg with null pointer
    sndSize = NvtIPC_MsgSnd(gMsqID, gSendTo, 0, sizeof(test));
    // receive msg with null pointer
    rcvSize = NvtIPC_MsgRcv(gMsqID,0, sizeof(test));
    // send size over limit
    sndSize = NvtIPC_MsgSnd(gMsqID, gSendTo, &test, NVTIPC_MSG_ELEMENT_SIZE+1);
    // send to invalid CPU
    sndSize = NvtIPC_MsgSnd(gMsqID, NVTIPC_SENDTO_MAX+1, &test, sizeof(test));
    // send size less than min
    sndSize = NvtIPC_MsgSnd(gMsqID, gSendTo, &test, 0);
    // receive size less than min
    rcvSize = NvtIPC_MsgRcv(gMsqID,&test, 0);

    // queue full
    for (i=0;i<NVTIPC_MSG_ELEMENT_NUM+1;i++)
    {
        sndSize = NvtIPC_MsgSnd(gMsqID, gSendTo, &test, 1);
    }
    // receive a emtpy queue
    for (i=0;i<NVTIPC_MSG_ELEMENT_NUM+1;i++)
    {
        rcvSize = NvtIPC_MsgRcv(gMsqID,&test, sizeof(test));
    }
    THREAD_RETURN(thread1_3);
}


static void simple_ipc(void)
{
    int dataSize = NVTIPC_MSG_ELEMENT_SIZE;
    unsigned int test[NVTIPC_MSG_ELEMENT_SIZE/4]={0};
    int  sndSize;
    int     rcvSize;
    int     rcvMsgcount=0;
    UINT32  ipcTime1,ipcTime2;


    DBG_DUMP("-----------simple_ipc begin-----------\r\n");
    while(1)
    {
        DBG_DUMP("wat rcv \r\n");
        rcvSize = NvtIPC_MsgRcv(xMsqID[0],&test,dataSize);
        if (test[0] == TEST_MAIN_CMD_ID)
        {
            DBG_DUMP("Enter Main Menu\r\n");
            break;
        }
        if (rcvSize < 0)
        {
            DBG_ERR("rcvSize %d\r\n",rcvSize);
            break;
        }
        #if 0
        {
            init j;
            DBG_IND("Rcv ");
            for (j=0;j<rcvSize/4;j++)
            {
                DBG_MSG("t%d=0x%x ",j,test[j]);
            }
            DBG_MSG("\r\n");
        }
        #endif
        rcvMsgcount++;

        ipcTime1 = drv_getSysTick();
        sndSize = NvtIPC_MsgSnd(xMsqID[0], gSendTo, &rcvMsgcount, 4);
        ipcTime2 = drv_getSysTick();
        DBG_DUMP("ipc Snd Time = %d us\r\n",ipcTime2-ipcTime1);
        if (sndSize < 0)
        {
            DBG_ERR("sndSize %d\r\n",sndSize);
            break;
        }
    }
    DBG_DUMP("-----------simple_ipc end------------\r\n");
}
static void race_ipc(int isSndtoDSP)
{
    #if __COREIPC__
    int test_syscmd = 1, test_err = 0;
    #else
    int test_syscmd = 1, test_err = 1;
    #endif
    int i,j,count= gCount, test_poweroff=0,dataSize;
    unsigned int test[NVTIPC_MSG_ELEMENT_SIZE/4]={0};
    int  sndSize;
    int  key1,key2;
    unsigned int  tmpBuf;
    unsigned int  *pInBuf;
    int msqindex, testNum = NVTIPC_MSG_ELEMENT_SIZE/4;
    static   char syscall_str[128];
    static   char uart_str[128];


    DBG_DUMP("---------race_ipc begin--------\r\n");
    tmpBuf = (unsigned int)malloc(0x40000);
    if (!tmpBuf)
    {
        DBG_ERR("can't alloc tmpBuf \r\n");
        return;
    }
    #if 1
    xInitBuff(tmpBuf);
    #else
    xInitBuff(ipc_getNonCacheAddr(tmpBuf));
    #endif
    key1 = NvtIPC_Ftok("t0");
    //xMsqID[0] = NvtIPC_MsgGet(key1);
    key2 = NvtIPC_Ftok("t1");
    xMsqID[1] = NvtIPC_MsgGet(key2);


    gbQueue1RcvCnt = 0;
    gbQueue2RcvCnt = 0;

    #if 1
    THREAD_CREATE(     6,
                       "thread1_1",
                       Thread1_1,
                       thread1_1_main,
                       NULL,
                       &Thread1_1Stacks[0],
                       sizeof(Thread1_1Stacks),
                       &Thread1_1_object
                  );
    THREAD_RESUME(Thread1_1);

    THREAD_CREATE(     6,
                       "thread1_2",
                       Thread1_2,
                       thread1_2_main,
                       NULL,
                       &Thread1_2Stacks[0],
                       sizeof(Thread1_2Stacks),
                       &Thread1_2_object
                  );
    THREAD_RESUME(Thread1_2);
    #endif

    while (!gbQueue1RcvCnt)
        cyg_thread_delay(100);
    for(i=0;i<count;i++)
    {
        #if 1
        msqindex = 0;
        pInBuf=(unsigned int *)xindexIn[msqindex];

        dataSize = (RANDOM_RANGE(testNum)+1)*4;
        for (j=0;j<dataSize/4;j++)
        {
            test[j]=(*pInBuf);
            pInBuf++;
        }
        xindexIn[msqindex]+=dataSize;
        sndSize = NvtIPC_MsgSnd(xMsqID[msqindex], gSendTo, &test, dataSize);
        #if 0
        DBG_IND("Snd ");
        for (j=0;j<dataSize/4;j++)
        {
            DBG_MSG("t%d=0x%x ",j,test[j]);
        }
        DBG_MSG("\r\n");
        #endif

        if (sndSize < 0)
        {
            DBG_ERR("sndSize = %d\r\n",sndSize);
            break;
        }


        msqindex = 1;
        pInBuf=(unsigned int *)xindexIn[msqindex];
        dataSize = (RANDOM_RANGE(testNum)+1)*4;

        for (j=0;j<dataSize/4;j++)
        {
            test[j]=(*pInBuf);
            pInBuf++;
        }
        // data 2
        xindexIn[msqindex]+=dataSize;
        sndSize = NvtIPC_MsgSnd(xMsqID[msqindex], gSendTo, &test, dataSize);
        #if 0
        DBG_IND("Snd ");
        for (j=0;j<dataSize/4;j++)
        {
            DBG_MSG("t%d=0x%x ",j,test[j]);
        }
        DBG_MSG("\r\n");
        #endif

        if (sndSize < 0)
        {
            DBG_ERR("sndSize = %d\r\n",sndSize);
            break;
        }
        if (isSndtoDSP)
        {
            sndSize = NvtIPC_MsgSnd(xMsqID[msqindex], NVTIPC_SENDTO_CORE3, &test, dataSize);
            if (sndSize < 0)
            {
                DBG_ERR("sndSize = %d\r\n",sndSize);
                break;
            }
        }
        #endif
        cyg_thread_delay(gdelayMS-3);
        DBG_MSG("s i=%d\r\n",i);
    }
    while (1)
    {
      if (gbQueue1RcvCnt < gCount || gbQueue1RcvCnt < gCount)
      {
          cyg_thread_delay(2000);
      }
      else
          break;
    }
    cyg_thread_delay(2000);
    NvtIPC_MsgRel(xMsqID[0]);
    NvtIPC_MsgRel(xMsqID[1]);
    cyg_thread_delay(2000);
    THREAD_DESTROY(Thread1_1);
    THREAD_DESTROY(Thread1_2);
    if (test_err)
    {
        // test error case
        //pthread_create(&thread1_3,NULL,thread1_3_main,NULL);
        THREAD_CREATE(     6,
                       "thread1_3",
                       Thread1_3,
                       thread1_3_main,
                       NULL,
                       &Thread1_3Stacks[0],
                       sizeof(Thread1_3Stacks),
                       &Thread1_3_object
                  );
        THREAD_RESUME(Thread1_3);
        cyg_thread_delay(2000);
        DBG_DUMP("Test if can exit wait status when queue is released\r\n");
        NvtIPC_MsgRel(gMsqID);
        DBG_DUMP("-------- Test  error case end ------\r\n");
        THREAD_DESTROY(Thread1_3);
    }
    // test ipc sys cmd
    if (test_syscmd)
    {
        NVTIPC_SYS_MSG  ipcSysMsg = {0};
        //xMsqID[0] = NvtIPC_MsgGet(key1);

        ipcSysMsg.sysCmdID =NVTIPC_SYSCMD_SYSCALL_REQ;
        strcpy(syscall_str,"lviewd -p 8192 -s 50000 -t 9 -f 10 -j 200000");
        ipcSysMsg.DataAddr = (NVTIPC_U32)&syscall_str[0];
        ipcSysMsg.DataSize = strlen(syscall_str)+1;
        NvtIPC_MsgSnd(NVTIPC_SYS_QUEUE_ID, gSendTo, &ipcSysMsg, sizeof(ipcSysMsg));
        //cyg_thread_delay(2000);

        strcpy(syscall_str,"hfs -s 50000 -t 8 -r /sdcard -n 2");
        ipcSysMsg.DataAddr = (NVTIPC_U32)&syscall_str[0];
        ipcSysMsg.DataSize = strlen(syscall_str)+1;
        NvtIPC_MsgSnd(NVTIPC_SYS_QUEUE_ID, gSendTo, &ipcSysMsg, sizeof(ipcSysMsg));
        //cyg_thread_delay(2000);

        strcpy(syscall_str,"live555");
        ipcSysMsg.DataAddr = (NVTIPC_U32)&syscall_str[0];
        ipcSysMsg.DataSize = strlen(syscall_str)+1;
        NvtIPC_MsgSnd(NVTIPC_SYS_QUEUE_ID, gSendTo, &ipcSysMsg, sizeof(ipcSysMsg));
        //cyg_thread_delay(2000);

        ipcSysMsg.sysCmdID =NVTIPC_SYSCMD_UART_REQ;
        strcpy(uart_str,"test uart");
        ipcSysMsg.DataAddr = (NVTIPC_U32)&uart_str[0];
        ipcSysMsg.DataSize = strlen(uart_str)+1;
        NvtIPC_MsgSnd(NVTIPC_SYS_QUEUE_ID, gSendTo, &ipcSysMsg, sizeof(ipcSysMsg));
        //cyg_thread_delay(2000);

        if (test_poweroff)
        {
            ipcSysMsg.sysCmdID = NVTIPC_SYSCMD_POWEROFF_REQ;
            ipcSysMsg.DataAddr = 0;
            NvtIPC_MsgSnd(NVTIPC_SYS_QUEUE_ID, gSendTo, &ipcSysMsg, sizeof(ipcSysMsg));
            cyg_thread_delay(2000);
        }
        //NvtIPC_MsgRel(xMsqID[0]);
    }
    free((void*)tmpBuf);
    DBG_DUMP("--------race_ipc end---------\r\n");
}

static void test_error(void)
{
    UINT32 i;
    char   tmpStr[10];
    for (i = 0; i < NVTIPC_MSG_QUEUE_NUM+1; i++)
    {
        sprintf(tmpStr,"t%d",i);
        NvtIPC_MsgGet(NvtIPC_Ftok(tmpStr));
    }
}

THREAD_DECLARE(thread1_4_main,arg)
{
    int dataSize = NVTIPC_MSG_ELEMENT_SIZE;
    unsigned int test[NVTIPC_MSG_ELEMENT_SIZE/4]={0};
    int     workbuf;
    int     rcvSize;
    NVTIPC_OPEN_OBJ   openObj={0};
    int     key1;

    workbuf = (unsigned int)malloc(NvtIpc_CalBuffSize());
    if (!workbuf)
    {
        DBG_ERR("can't alloc workbuf \r\n");
        return;
    }
    //openObj.workBuffAddr = ipc_getNonCacheAddr(workbuf);
    openObj.workBuffAddr = workbuf;
    openObj.workBuffSize = NvtIpc_CalBuffSize();
    openObj.SysCmdTbl = Syscmd_tbl;
    NvtIpc_Open(&openObj);
    key1 = NvtIPC_Ftok("t0");
    xMsqID[0] = NvtIPC_MsgGet(key1);
    //simple_ipc();
    while(1)
    {
        rcvSize = NvtIPC_MsgRcv(xMsqID[0],&test,dataSize);
        if (rcvSize < 0)
        {
            DBG_ERR("rcvSize %d\r\n",rcvSize);
        }
        DBG_DUMP("Cmd id %d\r\n",test[0]);
        switch (test[0])
        {
            case TEST_SIMPLE_CMD_ID:
                simple_ipc();
                break;
            case TEST_RACE_CMD_ID:
                race_ipc(0);
                // get message queue again because race_ipc will release it.
                xMsqID[0] = NvtIPC_MsgGet(key1);
                break;
            case TEST_3CORE_RACE_CMD_ID:
                race_ipc(1);
                // get message queue again because race_ipc will release it.
                xMsqID[0] = NvtIPC_MsgGet(key1);
            case TEST_ERROR:
                test_error();
                break;
            default:
                DBG_ERR("Unknown cmd id %d\r\n",test[0]);
        }
    }
    THREAD_RETURN(thread1_4);
}

MAINFUNC_ENTRY(nvtipc, argc, argv)
{

    THREAD_CREATE(     8,
                       "thread1_4",
                       Thread1_4,
                       thread1_4_main,
                       NULL,
                       &Thread1_4Stacks[0],
                       sizeof(Thread1_4Stacks),
                       &Thread1_4_object
                  );
    THREAD_RESUME(Thread1_4);
    return 0;
}

static void lviewd_fp(char* cmd)
{
    int   portNum = 80, threadPriority = 6, maxJpgSize = 200*1024 , frameRate=30, sockBufSize=50*1024;
    char  *delim=" ";
    cyg_lviewd_install_obj  lviewObj={0};


    DBG_DUMP("lviewd_fp cmd = %s\r\n",cmd);
    char *test = strtok(cmd, delim);
    while (test != NULL)
    {
        if (!strcmp(test, "-p"))
        {
            test = strtok(NULL, delim);
            if (test != NULL)
            {
                portNum = atoi(test);
            }
        }
        else if (!strcmp(test, "-t"))
        {
            test = strtok(NULL, delim);
            if (test != NULL)
            {
                threadPriority = atoi(test);
            }
        }
        else if (!strcmp(test, "-f"))
        {
            test = strtok(NULL, delim);
            if (test != NULL)
            {
                frameRate = atoi(test);
            }
        }
        else if (!strcmp(test, "-s"))
        {
            test = strtok(NULL, delim);
            if (test != NULL)
            {
                sockBufSize = atoi(test);
            }
        }
        else if (!strcmp(test, "-j"))
        {
            test = strtok(NULL, delim);
            if (test != NULL)
            {
                maxJpgSize = atoi(test);
            }
        }
        test = strtok(NULL, delim);
    }

    DBG_DUMP("portNum=%d,threadPriority=%d,maxJpgSize=%d,frameRate=%d,sockBufSize=%d\r\n",portNum,threadPriority,maxJpgSize,frameRate,sockBufSize);

    // set port number
    lviewObj.portNum = portNum;
    // set http live view server thread priority
    lviewObj.threadPriority = threadPriority;
    // set the maximum JPG size that http live view server to support
    lviewObj.maxJpgSize = maxJpgSize;
    // live view streaming frame rate
    lviewObj.frameRate = frameRate;
    // socket buffer size
    lviewObj.sockbufSize = sockBufSize;
    // reserved parameter , no use now, just fill NULL.
    lviewObj.arg = NULL;
    // install the CB functions and set some parameters
    cyg_lviewd_install(&lviewObj);
    // start the http live view server
    cyg_lviewd_startup();

}
static void hfs_fp(char* cmd)
{
    cyg_hfs_open_obj  hfsObj={0};
    int         portNum = 80, threadPriority = 6,sockBufSize = 51200, chkwpd = 0, getcustom = 0, genDirList = 0, maxClientNum=0;
    int         sharedMemAddr = 0;
    char        *delim=" ";

    DBG_DUMP("hfs_fp cmd = %s\r\n",cmd);

    char *test = strtok(cmd, delim);
    while (test != NULL)
    {
        // port number
        if (!strcmp(test, "-p"))
        {
            test = strtok(NULL, delim);
            if (test != NULL)
            {
                portNum = atoi(test);
            }
        }
        // thread priority
        else if (!strcmp(test, "-t"))
        {
            test = strtok(NULL, delim);
            if (test != NULL)
            {
                threadPriority = atoi(test);
            }
        }
        // root path
        else if (!strcmp(test, "-r"))
        {
            test = strtok(NULL, delim);
            if (test != NULL)
            {
                strncpy(hfsObj.rootdir, test, CYG_HFS_ROOT_DIR_MAXLEN);
            }
        }
        // socket buffer size
        else if (!strcmp(test, "-s"))
        {
            test = strtok(NULL, delim);
            if (test != NULL)
            {
                sockBufSize = atoi(test);
            }
        }
        // shared memory address
        else if (!strcmp(test, "-m"))
        {
            test = strtok(NULL, delim);
            if (test != NULL)
            {
                sharedMemAddr = atoi(test);
            }
        }
        // check password callback
        else if (!strcmp(test, "-w"))
        {
            chkwpd = 1;
        }
        // get custom callback
        else if (!strcmp(test, "-c"))
        {
            getcustom = 1;
        }
        // generate dirlist callback
        else if (!strcmp(test, "-d"))
        {
            genDirList = 1;
        }
        else if (!strcmp(test, "-n"))
        {
            test = strtok(NULL, delim);
            if (test != NULL)
            {
                maxClientNum = atoi(test);
            }
        }
        // key
        else if (!strcmp(test, "-k"))
        {
            test = strtok(NULL, delim);
            if (test != NULL)
            {
                strncpy(hfsObj.key, test, CYG_HFS_KEY_MAXLEN);
            }
        }
        test = strtok(NULL, delim);
    }
    if (chkwpd)
    {
        hfsObj.check_pwd = (cyg_hfs_check_password  *)4;
    }
    if (getcustom)
    {
        hfsObj.getCustomData = (cyg_hfs_getCustomData   *)4;
    }
    if (genDirList)
    {
        hfsObj.genDirListHtml = (cyg_hfs_gen_dirlist_html *)4;
    }
    // set port number
    hfsObj.portNum = portNum;
    // set thread priority
    hfsObj.threadPriority = threadPriority;
    strcpy(hfsObj.key, "testkey");
    // set socket buff size
    hfsObj.sockbufSize = sockBufSize;
    hfsObj.sharedMemAddr= sharedMemAddr;
    hfsObj.maxClientNum = maxClientNum;

    DBG_DUMP("hfsObj portNum= %d, threadPriority=%d, sockBufSize=%d, sharedMemAddr=0x%x, maxClientNum=%d\r\n",portNum,threadPriority,sockBufSize,sharedMemAddr,maxClientNum);
    cyg_hfs_open(&hfsObj);
}

#endif
