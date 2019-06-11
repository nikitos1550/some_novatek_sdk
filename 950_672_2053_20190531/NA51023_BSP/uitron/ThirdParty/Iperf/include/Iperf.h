/**
    Iperf module

    Network tool for measuring bandwidth.

    @file       Iperf.h
    @ingroup    mIIperf

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/
#ifndef _IPERF_H
#define _IPERF_H
/**
    @addtogroup mIIPERF
*/
//@{

typedef void (*FPREPORTCB)(double StartTime, double EndTime, double Bandwidth);

__externC void Iperf_Open( int argc, char **argv );

__externC void Iperf_Close(void);

__externC void Iperf_RunClient( int argc, char **argv );

__externC float Iperf_GetResult(void);//Mbits/sec

__externC void Iperf_RegReportCB(FPREPORTCB fpReportCB);

//@}
#endif
