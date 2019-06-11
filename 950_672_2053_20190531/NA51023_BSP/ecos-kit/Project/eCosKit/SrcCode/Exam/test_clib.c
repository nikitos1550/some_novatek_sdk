#include <stdio.h>
#include <limits.h>

FILE *InfoFileHnd = NULL;

void test_vprintf(const char *FmtStr, ...)
{
    va_list ArgList;
    va_start(ArgList, FmtStr);
    vprintf(FmtStr, ArgList);
    va_end(ArgList);
}
 
void test_vfprintf(const char *FmtStr, ...)
{
    va_list ArgList;
    InfoFileHnd = stdout;
    va_start(ArgList, FmtStr);
    vfprintf(InfoFileHnd, FmtStr, ArgList);
    va_end(ArgList);
}
 
void test_clib(void)
{
    printf("Min char = %d\r\n", CHAR_MIN);
    printf("Max char = %d\r\n", CHAR_MAX);
    printf("Min short = %d\r\n", SHRT_MIN);
    printf("Max short = %d\r\n", SHRT_MAX);
    printf("Min int = %d\r\n", INT_MIN);
    printf("Max int = %d\r\n", INT_MAX);
    printf("Min long = %ld\r\n", LONG_MIN);
    printf("Max long = %ld\r\n", LONG_MAX);

    test_vprintf("This is vprintf test - File:%s, line:%d\n",__FILE__,__LINE__); 
    test_vfprintf("This is vfprintf test - File:%s, line:%d\n",__FILE__,__LINE__); 
}

