#ifndef __NA51023_TIME_H__
#define __NA51023_TIME_H__
#include <common.h>

unsigned int na51023_time_start;

static inline unsigned int getTM0Time(void)
{
	unsigned int val = 0;

	val = *((volatile unsigned int *)(0xc0040108));
	return val;
}

static inline unsigned int getTM0InitTime(void)
{
	return na51023_time_start;
}

static inline void TM0Init(void)
{
	na51023_time_start = 0;

	na51023_time_start = *((volatile unsigned int *)(0xc0040108));
}
#endif /* __NA51023_TIME_H__ */
