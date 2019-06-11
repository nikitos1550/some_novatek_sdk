#include <stdio.h>

#if 1
#include <cyg/infra/mainfunc.h>
MAINFUNC_ENTRY(info,argc,argv)
#else
int main(int argc, char **argv)
#endif
{
    printf("dumpmallinfo:\n");
    dumpmallinfo();
    
    return 0;
}
