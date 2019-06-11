/**
    @file       pwm.h
    @ingroup    mIDrvIO_PWM

    @brief      Header file for PWM module
                This file is the header file that define the API for PWM module

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.

*/
#ifndef _PWM_H
#define _PWM_H

/**
    PWM type (PWM or Micro step)
*/
typedef enum
{
    PWM_TYPE_PWM = 0x0,             ///< PWM
    //PWM_TYPE_MICROSTEP,             ///< Micro step
    //PWM_TYPE_CCNT,                  ///< CCNT
    //PWM_TYPE_CNT,
    ENUM_DUMMY4WORD(PWM_TYPE)
} PWM_TYPE;

/**
    PWM clock divid

    @note for pwm_pwmConfigClockDiv()
*/
typedef enum
{
    PWM0_3_CLKDIV   = 0x0,          ///< PWM0~PWM3 clock divid
    PWM4_7_CLKDIV,                  ///< PWM4~PWM7 clock divid
    PWM8_11_CLKDIV,                 ///< PWM8~PWM11 clock divid
    PWM12_CLKDIV,                   ///< PWM12 clock divid
    PWM13_CLKDIV,                   ///< PWM13 clock divid
    PWM14_CLKDIV,                   ///< PWM14 clock divid
    PWM15_CLKDIV,                   ///< PWM15 clock divid
    ENUM_DUMMY4WORD(PWM_CLOCK_DIV)
} PWM_CLOCK_DIV;

/**
    PWM status selection
*/
typedef enum
{
	PWM_MS_STS,		///< PWM micro stepping status
	PWM_STS,		///< PWM status
	PWM_CLKLOAD_STS,	///< PWM clock load status
	PWM_TGTCNT_STS,		///< PWM target count status
//	ENUM_DUMMY4WORD(PWM_STS_SELECTION)
} PWM_STS_SELECTION;

/**
    PWM configuration structure

    @note for pwm_set()
*/
typedef struct
{
    u32  uiPrd;			 ///< Base period, how many PWM clock per period, 2 ~ 255
					///< @note rise <= fall <= base_period
    u32  uiRise;				///< Rising at which clock
					///< @note rise <= fall <= base_period
    u32  uiFall;				///< Falling at which clock
					///< @note rise <= fall <= base_period
    u32  uiOnCycle;				///< Output cycle, 0 ~ 65535
					///< - @b PWM_FREE_RUN: Free Run
					///< - @b Others: How many cycles (PWM will stop after output the cycle counts)
    u32  uiInv;				///< Invert PWM output signal or not
					///< - @b PWM_SIGNAL_NORMAL: Don't invert PWM output signal
					///< - @b PWM_SIGNAL_INVERT: Invert PWM output signal
} PWM_CFG, *PPWM_CFG;

/**
    @name PWM ID

    PWM ID for PWM driver API

    @note for pwm_open(), pwm_set(), pwm_setCCNT(), pwm_setCCNTToutEN(), pwm_wait(), pwm_stop(), pwm_en(), pwm_reload(),\n
              pwm_ms_set(), pwm_ms_stop(), pwm_ms_en(), pwm_getCycleNumber(), pwm_ccntGetCurrentVal(), pwm_ccntGetCurrentVal(),\n
              pwm_ccntEnable()
*/
//@{
    
#define PWMID_0                 0x00000001  ///< PWM ID 0
#define PWMID_1                 0x00000002  ///< PWM ID 1
#define PWMID_2                 0x00000004  ///< PWM ID 2
#define PWMID_3                 0x00000008  ///< PWM ID 3
#define PWMID_4                 0x00000010  ///< PWM ID 4
#define PWMID_5                 0x00000020  ///< PWM ID 5
#define PWMID_6                 0x00000040  ///< PWM ID 6
#define PWMID_7                 0x00000080  ///< PWM ID 7
#define PWMID_8                 0x00000100  ///< PWM ID 8
#define PWMID_9                 0x00000200  ///< PWM ID 9
#define PWMID_10                0x00000400  ///< PWM ID 10
#define PWMID_11                0x00000800  ///< PWM ID 11
#define PWMID_12                0x00001000  ///< PWM ID 12
#define PWMID_13                0x00002000  ///< PWM ID 13
#define PWMID_14                0x00004000  ///< PWM ID 14
#define PWMID_15                0x00008000  ///< PWM ID 15

extern int	pwm_close(u32 uiPWMId, BOOL bWaitAutoDisableDone);
extern void	pwm_setup_start(u32 pwm_id, u32 duty_cycle, u32 signal_inv);
extern void	pwm_reload(u32 pwm_id, u32 duty_cycle, u32 signal_inv);
#endif
