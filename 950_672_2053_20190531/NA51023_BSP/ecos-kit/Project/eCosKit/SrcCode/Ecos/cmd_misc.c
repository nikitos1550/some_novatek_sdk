#include <stdio.h>
#include <stdlib.h>
#include <cyg/kernel/kapi.h>
#include <cyg/infra/maincmd.h>
#include <cyg/infra/mainfunc.h>

// dump heap memory usage
extern void dumpmallinfo(void);
MAINFUNC_ENTRY(heap,argc,argv)
{
    dumpmallinfo();
    return 0;
}


MAINFUNC_ENTRY(help,argc,argv)
{
    listmaincmd();
    return 0;
}
