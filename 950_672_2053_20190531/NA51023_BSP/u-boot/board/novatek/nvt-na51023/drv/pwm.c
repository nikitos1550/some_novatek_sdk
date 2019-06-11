/*
 * Copyright (C) Novatek Microelectronics Corp
 *
 * Arvin Hsu <Arvin_Hsu@novatek.com.tw>
 *
 */
#include <common.h>
#include <config.h>
#include <div64.h>
#include <asm/arch/nvt_common.h>
#include <asm/arch/IOAddress.h>
#include "pwm.h"
#include "pwm-int.h"
#include "pwm-reg.h"

#define REG_CTRL(pwm)		(((pwm) * 0x8) + 0x00 + IOADDR_PWM_REG_BASE)
#define REG_PERIOD(pwm)		(((pwm) * 0x8) + 0x04 + IOADDR_PWM_REG_BASE)
#define REG_EXT_PERIOD(pwm)	(((pwm) * 0x4) + 0x230 + IOADDR_PWM_REG_BASE)
#define REG_INT_ENABLE		(0x258 + IOADDR_PWM_REG_BASE)
#define REG_INT_STS		    (0x268 + IOADDR_PWM_REG_BASE)
#define REG_ENABLE          (0x100 + IOADDR_PWM_REG_BASE)
#define REG_DISABLE         (0x104 + IOADDR_PWM_REG_BASE)
#define REG_LOAD            (0x108 + IOADDR_PWM_REG_BASE)
#define REG_INT2CORE2       (0x254 + IOADDR_PWM_REG_BASE)
#define REG_CLKEN           (0x7c + IOADDR_CG_REG_BASE) 
#define REG_CLKDIV(pwm)     (((pwm >> 1) * 0x4) + 0x50 + IOADDR_CG_REG_BASE) 

#define CTRL_ENABLE(pwm)	(0x1 << pwm)
#define CTRL_INVERT		    (0x1 << 28)
#define CTRL_RELOAD(pwm)	(0x1 << pwm)

static int div = 3;
static int check = 0;

static int vpwmclkdiv[PWMID_NO_TOTAL_CNT] = {
	PWM0_3_CLKDIV,   PWM0_3_CLKDIV,   PWM0_3_CLKDIV,   PWM0_3_CLKDIV,
	PWM4_7_CLKDIV,   PWM4_7_CLKDIV,   PWM4_7_CLKDIV,   PWM4_7_CLKDIV,
	PWM8_11_CLKDIV,   PWM8_11_CLKDIV,   PWM8_11_CLKDIV,  PWM8_11_CLKDIV,
	PWM12_CLKDIV,  PWM13_CLKDIV,  PWM14_CLKDIV,  PWM15_CLKDIV
};

int pwm_enable(int pwm_id)
{
	int val = 0;
  
    val = nvt_readl(REG_INT2CORE2);
    val |= CTRL_ENABLE(pwm_id);
    nvt_writel(val,REG_INT2CORE2);
    
    /*clk enable*/
    val = nvt_readl(REG_CLKEN);
    val |= CTRL_ENABLE(pwm_id);
    nvt_writel(val,REG_CLKEN);
    
    val = nvt_readl(REG_INT_ENABLE);
    val |= CTRL_ENABLE(pwm_id);
    nvt_writel(val,REG_INT_ENABLE);
    
    val = nvt_readl(REG_ENABLE);
    val |= CTRL_ENABLE(pwm_id);
    nvt_writel(val,REG_ENABLE);
    
    /*wait for enable be 1*/
  	while((nvt_readl(REG_ENABLE) & CTRL_ENABLE(pwm_id)) != CTRL_ENABLE(pwm_id));

	check = 1;
    
	return 0;
}

void pwm_disable(int pwm_id)
{
    u32 val;
    
    val = nvt_readl(REG_DISABLE);
    val |= (1 << pwm_id);
    nvt_writel(val,REG_DISABLE);
    
    val = nvt_readl(REG_INT_ENABLE);
    val &= ~(1 << pwm_id);
    nvt_writel(val,REG_INT_ENABLE);
    /*clk disable*/
    val = nvt_readl(REG_CLKEN);
    val &= ~(1 << pwm_id);
    nvt_writel(val,REG_CLKEN);
    
    val = nvt_readl(REG_INT2CORE2);
    val &= ~(1 << pwm_id);
    nvt_writel(val,REG_INT2CORE2);
    
    check = 0;
  	div = 3;
}

int pwm_config(int pwm_id, int duty_ns, int period_ns)
{
    unsigned long clkdiv = 0;
	PWM_CFG pwmcfg = {0};
	u64 src_freq = 120000000;
	u64 p = 0;
    u32 reg_pwm_period_buf = 0;
	u32 exp_period_buf = 0;
    u32 val = 0;

    if (pwm_id >= PWMID_NO_TOTAL_CNT) {
		printf("invalid PWM ID 0x%08x\r\n", pwm_id);
		return 1;
	}
    
    if ((pwm_id >= PWMID_NO_0) && (pwm_id <= PWMID_NO_7)) {
		if ((period_ns >= 67) && (period_ns <= 1000000000)) {
			clkdiv = div;
			p = src_freq * period_ns;
			do_div(p, 1000000000);
			do_div(p, (clkdiv + 1));
			pwmcfg.uiPrd = (u32)p;

			while (pwmcfg.uiPrd > 65535) {
				clkdiv += 4;
				p = src_freq * period_ns;
				do_div(p, 1000000000);
				do_div(p, (clkdiv + 1));
				pwmcfg.uiPrd = (u32)p;
			}
			pwmcfg.uiRise = 0;

			if (duty_ns == 0) {
				;
			} else {
				if (period_ns/duty_ns != 0)
					pwmcfg.uiFall = duty_ns/(period_ns/pwmcfg.uiPrd);
			}
		} else {
        	printf("not support this output frequency PWM%d\n", pwm_id);
        	return 1;
		}
	} else {
		if ((period_ns >= 67) && (period_ns <= (36*1000000))) {
			clkdiv = div;
			p = src_freq * period_ns;
			do_div(p, 1000000000);
			do_div(p, (clkdiv + 1));
			pwmcfg.uiPrd = (u32)p;

			while (pwmcfg.uiPrd > 255) {
				clkdiv += 4;
				p = src_freq * period_ns;
				do_div(p, 1000000000);
				do_div(p, (clkdiv + 1));
				pwmcfg.uiPrd = (u32)p;
			}
			pwmcfg.uiRise = 0;

			if (duty_ns == 0) {
				;
			} else {
				if (period_ns/duty_ns != 0)
					pwmcfg.uiFall = duty_ns/(period_ns/pwmcfg.uiPrd);
			}
		} else {
			printf("not support this output frequency PWM%d\n", pwm_id);
			return 1;
		}
	}

	if(check == 0)
		div = clkdiv;
    
    val = nvt_readl(REG_CLKDIV(vpwmclkdiv[pwm_id]));
    val &= ~(0x3FFF << ((vpwmclkdiv[pwm_id] & 0x1) << 4));
    val |= ((clkdiv & 0x3FFF) << ((vpwmclkdiv[pwm_id] & 0x1) << 4));
    nvt_writel(val, REG_CLKDIV(vpwmclkdiv[pwm_id]));

	if (pwmcfg.uiPrd < 2) {
		printf("invalid PWM base period %d MUST 2~255\r\n", pwmcfg.uiPrd);
		return 1;
	}

	reg_pwm_period_buf = ((pwmcfg.uiRise & 0xFF) + ((pwmcfg.uiFall & 0xFF) << 8) + ((pwmcfg.uiPrd & 0xFF) << 16));

	if (pwm_id < 8) {
		exp_period_buf = nvt_readl(REG_EXT_PERIOD(pwm_id));
		exp_period_buf = ((pwmcfg.uiRise >> 8 & 0xFF) + ((pwmcfg.uiFall >> 8 & 0xFF) << 8) + ((pwmcfg.uiPrd >> 8 & 0xFF) << 16));
		nvt_writel(exp_period_buf, REG_EXT_PERIOD(pwm_id));
	}

	nvt_writel(reg_pwm_period_buf, REG_PERIOD(pwm_id));

	return 0;
}

/*
    PWM Clock enable/disable function

    @return Return void
*/
static void pwm_enable_clk(BOOL enable, u32 channel)
{
	u32 pwm_clk_reg = 0;

	HAL_READ_UINT32(IOADDR_CG_PWM_REG_BASE, pwm_clk_reg);

	if(enable)
		pwm_clk_reg |= 1 << channel;
	else
		pwm_clk_reg &= ~(1 << channel);

	HAL_WRITE_UINT32(IOADDR_CG_PWM_REG_BASE, pwm_clk_reg);
}

/*
    PWM Polling function

    Status checking point for PWM driver.

    @return Return void
*/
static void pwm_polling(PWM_STS_SELECTION pwm_status, int channel)
{
	u32 tmpStatus;
	union PWM_CTRL_INTSTS       pwm_int_sts;
	union PWM_MS_CTRL_INTSTS    ms_int_sts;
	union PWM_CLKDIV_LOAD_STS   pwm_clkdiv_load_sts;

	switch (pwm_status) {
	case PWM_MS_STS:
		while (1) {
			ms_int_sts.reg = PWM_GETREG(PWM_MS_CTRL_INTSTS_OFS);
			tmpStatus = ms_int_sts.reg & (1 << channel);
			if (tmpStatus) {
				PWM_SETREG(PWM_MS_CTRL_INTSTS_OFS, ms_int_sts.reg);
				break;
			}
		}

		break;
	case PWM_STS:
		while (1) {
			pwm_int_sts.reg = PWM_GETREG(PWM_CTRL_INTSTS_OFS);
			tmpStatus = pwm_int_sts.reg & (1 << channel);
			if (tmpStatus) {
				PWM_SETREG(PWM_CTRL_INTSTS_OFS, pwm_int_sts.reg);
				break;
			}
		}
		break;

	case PWM_CLKLOAD_STS:
		while (1) {
			pwm_clkdiv_load_sts.reg = PWM_GETREG(PWM_CLKDIV_LOAD_STS_OFS);
			tmpStatus = pwm_clkdiv_load_sts.reg & 0x7;
			if (tmpStatus & (1 << channel)) {
				PWM_SETREG(PWM_CLKDIV_LOAD_STS_OFS, (pwm_clkdiv_load_sts.reg & 0x7));
				break;
			}
		}

		break;

	case PWM_TGTCNT_STS:
		while (1) {
			pwm_clkdiv_load_sts.reg = PWM_GETREG(PWM_CLKDIV_LOAD_STS_OFS);
			tmpStatus = ((pwm_clkdiv_load_sts.reg & 0x70000) >> 16);
			if (tmpStatus & (1 << channel)) {
				PWM_SETREG(PWM_CLKDIV_LOAD_STS_OFS, (pwm_clkdiv_load_sts.reg & 0x70000));
				break;
			}
		}

		break;

	default:
		break;

	}
}

/*
    Check PWM ID

    Check whether the PWM ID is valid or not

    @param[in] uiStartBit   Start search bit
    @param[in] uiPWMId      PWM ID (bitwise), one ID at a time

    @return Check ID status
        - @b PWM_INVALID_ID: Not a valid PWM ID
        - @b Other: ID offset (0 ~ 2 for PWM, 4 for CCNT)
*/
static u32 pwm_isValidId(u32 uiStartBit, u32 uiPWMId)
{
	u32 i;
	for(i=uiStartBit; i< (PWM_ALLCH_BITS); i++) {
		if(uiPWMId & (1<<i))
			return i;
	}

	return PWM_INVALID_ID;
}

/*
    Get if PWM enable

    Get specific PWM channel is enabled or not

    @param[in] uiPWMId PWM ID(one PWM id per time)

    @return Start status
        - @b TRUE   : Enabled
        - @b FALSE  : Disabled
*/
static BOOL pwm_pwmIsEn(u32 uiPWMId)
{
	if((PWM_GETREG(PWM_ENABLE_OFS) & uiPWMId) == uiPWMId)
		return TRUE;
	else
		return FALSE;
}

/*
    PWM Set clock rate function

    @return Return void
*/
static void pwm_clk_set_rate(u32 channel, u32 div)
{
	u32 clkdiv_reg = 0;

	if (!div)
		div = 1;

	switch(channel) {
	case PWMID_NO_0:
	case PWMID_NO_1:
	case PWMID_NO_2:
	case PWMID_NO_3:
		HAL_READ_UINT32(IOADDR_CLKRATE_PWM_REG_BASE, clkdiv_reg);
		clkdiv_reg &= ~0x3FFF;
		clkdiv_reg |= div;
		HAL_WRITE_UINT32(IOADDR_CLKRATE_PWM_REG_BASE, clkdiv_reg);
		break;

	case PWMID_NO_4:
	case PWMID_NO_5:
	case PWMID_NO_6:
	case PWMID_NO_7:
		HAL_READ_UINT32(IOADDR_CLKRATE_PWM_REG_BASE, clkdiv_reg);
		clkdiv_reg &= ~0x3FFF0000;
		clkdiv_reg |= div << 16;
		HAL_WRITE_UINT32(IOADDR_CLKRATE_PWM_REG_BASE, clkdiv_reg);
		break;

	case PWMID_NO_8:
	case PWMID_NO_9:
	case PWMID_NO_10:
	case PWMID_NO_11:
		HAL_READ_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0x4, clkdiv_reg);
		clkdiv_reg &= ~0x3FFF;
		clkdiv_reg |= div;
		HAL_WRITE_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0x4, clkdiv_reg);
		break;

	case PWMID_NO_12:
		HAL_READ_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0x4, clkdiv_reg);
		clkdiv_reg &= ~0x3FFF0000;
		clkdiv_reg |= div << 16;
		HAL_WRITE_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0x4, clkdiv_reg);
		break;

	case PWMID_NO_13:
		HAL_READ_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0x8, clkdiv_reg);
		clkdiv_reg &= ~0x3FFF;
		clkdiv_reg |= div;
		HAL_WRITE_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0x8, clkdiv_reg);
		break;

	case PWMID_NO_14:
		HAL_READ_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0x8, clkdiv_reg);
		clkdiv_reg &= ~0x3FFF0000;
		clkdiv_reg |= div << 16;
		HAL_WRITE_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0x8, clkdiv_reg);
		break;

	case PWMID_NO_15:
		HAL_READ_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0xC, clkdiv_reg);
		clkdiv_reg &= ~0x3FFF;
		clkdiv_reg |= div;
		HAL_WRITE_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0xC, clkdiv_reg);
		break;

	default:
		printf("unsupport ID\n");
	}
}

/**
    Set PWM clock divided by micro step set unit(for micro step usage)

    Set clock divid by micro step set unit where\n
    set0:pwm0-3, set1:pwm4-7, set2:pwm8-11, set3:pwm12-15

    @param[in] uiClkSrc     pwm clock divided source
    @param[in] uiDiv        PWM divid

    @note for PWM_CLOCK_DIV
    @return Set cycle count parameter status
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID or not opened yet
*/
int pwm_pwmConfigClockDiv(PWM_CLOCK_DIV uiClkSrc, u32 uiDiv)
{
	switch(uiClkSrc) {
	case PWM0_3_CLKDIV:
		if((PWM_GETREG(PWM_ENABLE_OFS) & (PWMID_0 | PWMID_1 | PWMID_2 | PWMID_3)) != 0) {
			debug("PWM0~3 are using same clock div, make sure PWM0~3 are all disabled\r\n");
            debug("PWM_GETREG(PWM_ENABLE_OFS) = 0x%x\r\n", PWM_GETREG(PWM_ENABLE_OFS));
			return E_SYS;
		}

		pwm_clk_set_rate(pwm_isValidId(0, PWMID_0), uiDiv);
		break;

	case PWM4_7_CLKDIV:
		if((PWM_GETREG(PWM_ENABLE_OFS) & (PWMID_4 | PWMID_5| PWMID_6 | PWMID_7)) != 0) {
			debug("PWM4~7 are using same clock div, make sure PWM4~7 are all disabled\r\n");
			return E_SYS;
		}

		pwm_clk_set_rate(pwm_isValidId(0, PWMID_4), uiDiv);
		break;

	case PWM8_11_CLKDIV:
		if((PWM_GETREG(PWM_ENABLE_OFS) & (PWMID_8 | PWMID_9| PWMID_10 | PWMID_11)) != 0) {
			debug("PWM8~11 are using same clock div, make sure PWM8~11 are all disabled\r\n");
			return E_SYS;
		}

		pwm_clk_set_rate(pwm_isValidId(0, PWMID_8), uiDiv);
		break;

	case PWM12_CLKDIV:
	case PWM13_CLKDIV:
	case PWM14_CLKDIV:
	case PWM15_CLKDIV:
		if(pwm_pwmIsEn(1<<uiClkSrc) == TRUE) {
			printf("make sure PWM%d are disabled\r\n", uiClkSrc);
			return E_SYS;
		}

		pwm_clk_set_rate(uiClkSrc, uiDiv);
		break;
	default:
		printf("Unknown clock source \r\n");
		return E_SYS;
		break;
	}

	return E_OK;
}

/**
    Wait operation done

    Calling this API to wait until the operation of target channel is done

    @param[in] uiPWMId      PWM ID, one ID at a time
    @param[in] uiPwmType    determine is PWM, CCNT or micro step, wait different flag
    @return Operation status
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID or not opened yet
*/
int pwm_wait(u32 uiPWMId, PWM_TYPE uiPwmType)
{
	u32 uiEnumPWMID;
	int uiResult = E_OK;

	if ((uiEnumPWMID = pwm_isValidId(0, uiPWMId)) == PWM_INVALID_ID) {
		printf("invalid PWM ID!\r\n");
		return E_PAR;
	}

	if(pwm_isValidId(uiEnumPWMID+1, uiPWMId) != PWM_INVALID_ID) {
		printf("more than 1 channel to wait done at one time\r\n");
		return E_SYS;
	}


	if(uiPwmType == PWM_TYPE_PWM)
		pwm_polling(PWM_STS, uiEnumPWMID);
	else
		pwm_polling(PWM_MS_STS, uiEnumPWMID);

	return uiResult;
}

/**
    @name PWM pwm function related API
*/
//@{
/**
    Enable(Start) PWM

    Start PWM based on PWM parameter descriptor set by pwm_set().

    @param[in] uiPWMId PWM ID, could be OR's of any PWM ID

    @return Start status
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID or not opened yet
*/
int pwm_pwmEnable(u32 uiPWMId)
{
	union PWM_ENABLE pwm_ctrl_en;

	if (PWM_GETREG(PWM_ENABLE_OFS) & uiPWMId) {
		printf("pwm_pwmEnable(), do not re enable same channel!! (%x)\r\n", uiPWMId);
		return E_PAR;
	} else {
		pwm_ctrl_en.reg = uiPWMId;
		PWM_SETREG(PWM_ENABLE_OFS, pwm_ctrl_en.reg);
	}
	// Enable PWM_x
	// Because enable bit is in APB clock domain. Need double sync to PWM clock
	// domain. So after PWM enable bit set => read until 1(represent PWM clock domain is active)
	while(!(PWM_GETREG(PWM_ENABLE_OFS) & uiPWMId));

	return E_OK;
}

/**
    Disable(stop) PWM

    Disable PWM if it is running in free run mode.

    @param[in] uiPWMId PWM ID, one ID at a time
    @return Stop status
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID or not opened yet
*/
int pwm_pwmDisable(u32 uiPWMId)
{
	union PWM_DISABLE pwm_stop;

	// Disable PWM
	if (PWM_GETREG(PWM_ENABLE_OFS) & uiPWMId) {
		pwm_stop.reg = uiPWMId;
		PWM_SETREG(PWM_DISABLE_OFS, pwm_stop.reg);
		while(PWM_GETREG(PWM_ENABLE_OFS) & uiPWMId);
		return E_OK;
	} else {
		printf("pwm_pwmDisable(), do not disable the channel not enable yet!! (%x)\r\n", uiPWMId);
		return E_PAR;
	}

}

/**
    Set PWM parameters.

    This function sets PWM duty cycles, repeat count and signal level.\n
    After set, the specified pwm channel can be started and stopped by API\n
    functions pwm_pwmEnable() and pwm_pwmDisable(). If the on cycle is not PWM_FREE_RUN,\n
    PWM will issue an interrupt after all cycles are done.

    @param[in] uiPWMId  pwm id, one id at a time
    @param[in] pPWMCfg  PWM parameter descriptor

    @return Set parameter status.
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID
*/
int pwm_pwmConfig(u32 uiPWMId, PPWM_CFG pPWMCfg)
{
	u32 uiOffset;
	union PWM_CTRL_REG_BUF reg_pwm_ctrl_buf;
	union PWM_PERIOD_REG_BUF reg_pwm_period_buf;
	union PWM_PERIOD_REG_BUF org_pwm_period_buf;
	union PWM_EXPEBD_PERIOD_REG_BUF exp_period_buf = {0};


	reg_pwm_ctrl_buf.reg = 0;
	reg_pwm_period_buf.reg = 0;

	if((uiOffset = pwm_isValidId(0, uiPWMId)) == PWM_INVALID_ID) {
		printf("invalid PWM ID 0x%08x\r\n", uiPWMId);
		return E_PAR;
	}


	if(uiOffset < 16)
		org_pwm_period_buf.reg = PWM_GETREG(PWM_PWM0_PRD_OFS+uiOffset*PWM_PER_CH_OFFSET);
	else
		org_pwm_period_buf.reg = PWM_GETREG((PWM_PWM0_PRD_OFS+uiOffset*PWM_PER_CH_OFFSET+0x20));




	if(pPWMCfg->uiPrd < 2) {
		printf("invalid PWM base period %d MUST 2~255\r\n", pPWMCfg->uiPrd);
		return E_PAR;
	}

	reg_pwm_ctrl_buf.bit.pwm_on     = pPWMCfg->uiOnCycle;
	reg_pwm_ctrl_buf.bit.pwm_type   = PWM_TYPE_PWM;
	reg_pwm_period_buf.bit.pwm_r    = pPWMCfg->uiRise;
	reg_pwm_period_buf.bit.pwm_f    = pPWMCfg->uiFall;
	reg_pwm_period_buf.bit.pwm_prd  = pPWMCfg->uiPrd;
	reg_pwm_period_buf.bit.pwm_inv  = pPWMCfg->uiInv;

	if(uiOffset < 8) {
		exp_period_buf.reg = PWM_GETREG(PWM0_EXPEND_PERIOD_REG_BUF_OFS + (uiOffset << 2));
		exp_period_buf.bit.pwm_r    = (pPWMCfg->uiRise >> 8) & 0xFF;
		exp_period_buf.bit.pwm_f    = (pPWMCfg->uiFall >> 8) & 0xFF;
		exp_period_buf.bit.pwm_prd  = (pPWMCfg->uiPrd >> 8) & 0xFF;
		PWM_SETREG(PWM0_EXPEND_PERIOD_REG_BUF_OFS + (uiOffset << 2), exp_period_buf.reg);

	}

	if(org_pwm_period_buf.bit.pwm_inv != pPWMCfg->uiInv && pwm_pwmIsEn(uiPWMId)) {
		printf("Try to change Invert signal but PWM [%d] is enabled already\r\n", pwm_isValidId(0, uiPWMId));
		return E_PAR;
	}

	if(uiOffset < 16) {
		PWM_SETREG(PWM_PWM0_PRD_OFS+uiOffset*PWM_PER_CH_OFFSET, reg_pwm_period_buf.reg);
		PWM_SETREG(PWM_PWM0_CTRL_OFS+uiOffset*PWM_PER_CH_OFFSET, reg_pwm_ctrl_buf.reg);
	} else {
		PWM_SETREG((PWM_PWM0_PRD_OFS+uiOffset*PWM_PER_CH_OFFSET+0x20), reg_pwm_period_buf.reg);
		PWM_SETREG((PWM_PWM0_CTRL_OFS+uiOffset*PWM_PER_CH_OFFSET+0x20), reg_pwm_ctrl_buf.reg);
	}

	return E_OK;
}

/**
    Reload PWM

    Reload PWM based on PWM parameter descriptor set by pwm_set().

    @param[in] uiPWMId  PWM ID, could be OR's of any PWM ID

    @return Start status
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID or not opened yet
*/
int pwm_pwmReload(u32 uiPWMId)
{
	union PWM_LOAD pwmLoadReg;

	if(pwm_pwmIsEn(uiPWMId) == FALSE) {
		printf("pwm[0x%08x]not enable yet!\r\n", uiPWMId);
		return E_SYS;
	}

	// Reload PWM
	pwmLoadReg.reg = PWM_GETREG(PWM_LOAD_OFS);
	pwmLoadReg.reg |= uiPWMId;

	// Avoid PWM channel(s) are under reloading(wait until done if under reloading)
	while((PWM_GETREG(PWM_LOAD_OFS) & uiPWMId));
	PWM_SETREG(PWM_LOAD_OFS, pwmLoadReg.reg);

	return E_OK;
}

/*
    Attach PWM driver

    Pre-initialize PWM driver before PWM driver is opened.
    This function should be the very first function to be invoked.

    @param[in] uiEnumPWMID      PWM ID (enum), one ID at a time.

    @return void
*/

static void pwm_attach(u32 uiEnumPWMID)
{
	u32 pwm_pinmux_reg = 0;

	if(uiEnumPWMID >= PWMID_NO_TOTAL_CNT)
		return;

	/*Enable PWM Pinmux*/
	HAL_READ_UINT32(IOADDR_PINMUX_PWM_REG_BASE, pwm_pinmux_reg);

	pwm_pinmux_reg |= (1 << uiEnumPWMID);

	HAL_WRITE_UINT32(IOADDR_PINMUX_PWM_REG_BASE, pwm_pinmux_reg);

	HAL_READ_UINT32(IOADDR_PINMUX2_PWM_REG_BASE, pwm_pinmux_reg);

	pwm_pinmux_reg &= ~(1 << uiEnumPWMID);

	HAL_WRITE_UINT32(IOADDR_PINMUX2_PWM_REG_BASE, pwm_pinmux_reg);

	/*Release reset*/
	HAL_READ_UINT32(IOADDR_RESET_PWM_REG_BASE, pwm_pinmux_reg);

	if (!(pwm_pinmux_reg & 0x100)) {
		pwm_pinmux_reg |= 0x100;

		HAL_WRITE_UINT32(IOADDR_RESET_PWM_REG_BASE, pwm_pinmux_reg);
	}

	pwm_enable_clk(ENABLE, uiEnumPWMID);
}


/*
    Detach PWM driver

    De-initialize PWM driver after SPI driver is closed.

    @param[in] uiEnumPWMID  PWM ID (enum), one ID at a time.

    @return void
*/
static void pwm_detach(u32 uiEnumPWMID)
{
	u32 pwm_pinmux_reg = 0;

	if(uiEnumPWMID >= PWMID_NO_TOTAL_CNT)
		return;

	pwm_enable_clk(DISABLE, uiEnumPWMID);

	/*Disable PWM Pinmux*/
	HAL_READ_UINT32(IOADDR_PINMUX_PWM_REG_BASE, pwm_pinmux_reg);

	pwm_pinmux_reg &= ~(1 << uiEnumPWMID);

	HAL_WRITE_UINT32(IOADDR_PINMUX_PWM_REG_BASE, pwm_pinmux_reg);

	HAL_READ_UINT32(IOADDR_PINMUX2_PWM_REG_BASE, pwm_pinmux_reg);

	pwm_pinmux_reg |= (1 << uiEnumPWMID);

	HAL_WRITE_UINT32(IOADDR_PINMUX2_PWM_REG_BASE, pwm_pinmux_reg);
}

/*
    Internal open flow.

    Internal open function shared by pwm_open() and pwm_open_NoSwitchPinmux().

    @param[in] uiPWMId          PWM ID (bitwise), allow multiple ID at a time.

    @return Open PWM status
        - @b E_OK: Success
        - @b Others: Open PWM failed
*/
static int pwm_openInternal(u32 uiPWMId)
{
	int                      erReturn;
	u32                     uiEnumPWMID;

	union PWM_CTRL_INTEN        pwm_ctrl_int_en;
	union PWM_CTRL_INTSTS       pwm_ctrl_int_sts;

	union PWM_MS_CTRL_INTEN     ms_ctrl_int_en;
	union PWM_MS_CTRL_INTSTS    ms_ctrl_int_sts;


	if ((uiEnumPWMID = pwm_isValidId(0, uiPWMId)) == PWM_INVALID_ID) {
		printf("invalid PWM ID!\r\n");
		return E_PAR;
	}

	// Clear all PWM interrupt status
	pwm_ctrl_int_sts.reg = PWM_GETREG(PWM_CTRL_INTSTS_OFS);
	pwm_ctrl_int_sts.reg |= PWM_INT_STS_ALL;
	PWM_SETREG(PWM_CTRL_INTSTS_OFS, pwm_ctrl_int_sts.reg);

	// Clear all MS interrupt status
	ms_ctrl_int_sts.reg = PWM_GETREG(PWM_MS_CTRL_INTSTS_OFS);
	ms_ctrl_int_sts.reg |= PWM_INT_MS_STS_ALL;
	PWM_SETREG(PWM_MS_CTRL_INTSTS_OFS, ms_ctrl_int_sts.reg);

	// Only enable specific id (opened)
	// Enable specific PWM interrupts
	pwm_ctrl_int_en.reg = PWM_GETREG(PWM_CTRL_INTEN_OFS);
	pwm_ctrl_int_en.reg |= (1<<uiEnumPWMID);
	PWM_SETREG(PWM_CTRL_INTEN_OFS, pwm_ctrl_int_en.reg);

	// Enable specific MS interrupts
	ms_ctrl_int_en.reg = PWM_GETREG(PWM_MS_CTRL_INTEN_OFS);
	ms_ctrl_int_en.reg |= (1<<uiEnumPWMID);
	PWM_SETREG(PWM_MS_CTRL_INTEN_OFS, ms_ctrl_int_en.reg);


	// Enable clock and switch pinmux
	pwm_attach(uiEnumPWMID);

	return erReturn;
}

/*
    Internal close flow.

    Internal close function shared by pwm_close() and pwm_closeNoPatch().

    @param[in] uiPWMId      PWM ID (bitwise), one ID at a time
    @param[in] bWait        driver is closed after waiting PWM signal level back to correct level

    @return Open PWM status
        - @b E_OK: Success
        - @b Others: Close PWM failed
*/
static int pwm_closeInternal(u32 uiPWMId, BOOL bWait)
{
	u32 uiEnumPWMID;
	int erReturn;
	union PWM_CTRL_INTEN        pwm_ctrl_int_en;
	union PWM_MS_CTRL_INTEN     ms_ctrl_int_en;


	if ((uiEnumPWMID = pwm_isValidId(0, uiPWMId)) == PWM_INVALID_ID)
		return E_PAR;

	// Stop at close
	if(bWait == TRUE) {
		if (PWM_GETREG(PWM_ENABLE_OFS) & (1<<uiEnumPWMID)) {
			erReturn = pwm_pwmDisable(uiPWMId);
			if (erReturn != E_OK)
				return erReturn;
		}
		pwm_wait(uiPWMId, PWM_TYPE_PWM);
	} else {
		if (PWM_GETREG(PWM_ENABLE_OFS) & (1<<uiEnumPWMID)) {
			erReturn = pwm_pwmDisable(uiPWMId);
			if (erReturn != E_OK)
				return erReturn;
		}
	}

	// Only enable specific id (opened)
	// Enable specific PWM interrupts
	pwm_ctrl_int_en.reg = PWM_GETREG(PWM_CTRL_INTEN_OFS);
	pwm_ctrl_int_en.reg &= ~(1<<uiEnumPWMID);
	PWM_SETREG(PWM_CTRL_INTEN_OFS, pwm_ctrl_int_en.reg);

	// Enable specific MS interrupts
	ms_ctrl_int_en.reg = PWM_GETREG(PWM_MS_CTRL_INTEN_OFS);
	ms_ctrl_int_en.reg &= ~(1<<uiEnumPWMID);
	PWM_SETREG(PWM_MS_CTRL_INTEN_OFS, ms_ctrl_int_en.reg);

	// Disable clock and switch pinmux back to GPIO
	pwm_detach(uiEnumPWMID);

	return E_OK;
}


/**
    @name PWM public driver API
*/
//@{
/**
    Open specific pwm channel.

    If want to access pwm channel, need call pwm open API, will\n
    lock related semphare, enable clock and interrupt.

    @note   pwm_open do not switch pinmux(config at top API), only obtain semphare,\n
            and enable clock

    @param[in] uiPWMId  pwm id, one id at a time
    @return Open PWM status
        - @b E_OK: Success
        - @b Others: Open PWM failed
*/
int pwm_open(u32 uiPWMId)
{
	return pwm_openInternal(uiPWMId);
}

/**
    Close specific pwm channel.

    Let other tasks to use specific pwm channel, including set specific channel\n
    disable (finish at an end of cycle). Disable clock, and unlock semphare

    @note Because of pwm_close() include pwm_pwmDisable(), so MUST called behind\n
          pwm_pwmEnable(). In other word, it's illegal that called pwm_close(pwm_ch, TRUE)\n
          without calling pwm_pwnEnable(pwm_ch).

    @param[in] uiPWMId                  pwm id, one id at a time(PWMID_0, PWMID_1 ... PWMID_19)
    @param[in] bWaitAutoDisableDone     wait a complete cycle done or not after pwm disable completed.\n
                                        (system will halt once if close a disabled PWM channel.)

    Example:
    @code
    {
        pwm_open(pwm_ch_x);
        pwm_pwmConfig(pwm_ch_x, xxx);
        pwm_pwmEnable(pwm_ch_x);

        //case1:
        pwm_pwmDisable(pwm_ch_x);
        //Need wait disable done
        pwm_wait(pwm_ch_x);
        //Fill FALSE due to already disable done
        pwm_close(pwm_ch_x, FALSE);

        //case2:
        //Fill TRUE if pwm_close called directly
        pwm_close(pwm_ch_x, TRUE);

        //Flow of case1 are equal to case2
    }
    @endcode
    @return Open PWM status
        - @b E_OK: Success
        - @b Others: Close PWM failed
*/
static int drv_pwm_close(u32 uiPWMId, BOOL bWaitAutoDisableDone)
{
	return pwm_closeInternal(uiPWMId, bWaitAutoDisableDone);
}

static void drv_pwm_setup_start(u32 pwm_id, u32 duty_cycle, u32 inv)
{
	PWM_CFG pwm_info = {0};
	PWM_CLOCK_DIV clock_div_section;

	if (duty_cycle > 100) {
		printf("Duty cycle should not larger than 100\n");
		return;
	}

	if (pwm_id < PWMID_4)
		clock_div_section = PWM0_3_CLKDIV;
	else if (pwm_id < PWMID_8)
		clock_div_section = PWM4_7_CLKDIV;
	else if (pwm_id < PWMID_12)
		clock_div_section = PWM8_11_CLKDIV;
	else if (pwm_id == PWMID_12)
		clock_div_section = PWM12_CLKDIV;
	else if (pwm_id == PWMID_13)
		clock_div_section = PWM13_CLKDIV;
	else if (pwm_id == PWMID_14)
		clock_div_section = PWM14_CLKDIV;
	else
		clock_div_section = PWM15_CLKDIV;

	pwm_open(pwm_id);

	if (pwm_id < PWMID_8) {
		pwm_pwmConfigClockDiv(clock_div_section, 0x2EDF);
		pwm_info.uiPrd = 0x2710;
		pwm_info.uiRise = 0;
		pwm_info.uiFall = (0x2710*duty_cycle) / 100;
	} else {
		pwm_pwmConfigClockDiv(clock_div_section, 0x3FFF);
		pwm_info.uiPrd = 255;
		pwm_info.uiRise = 0;
		pwm_info.uiFall = (255*duty_cycle) / 100;
	}
	pwm_info.uiInv = inv;

	pwm_pwmConfig(pwm_id, &pwm_info);

	pwm_pwmEnable(pwm_id);
}

static void drv_pwm_reload(u32 pwm_id, u32 duty_cycle, u32 inv)
{
	PWM_CFG pwm_info = {0};

	if (duty_cycle > 100) {
		printf("Duty cycle should not larger than 100\n");
		return;
	}

	if (pwm_id < PWMID_8) {
		pwm_info.uiPrd = 0x2710;
		pwm_info.uiRise = 0;
		pwm_info.uiFall = (0x2710*duty_cycle) / 100;
	} else {
		pwm_info.uiPrd = 255;
		pwm_info.uiRise = 0;
		pwm_info.uiFall = (255*duty_cycle) / 100;
	}
	pwm_info.uiInv = inv;

	pwm_pwmConfig(pwm_id, &pwm_info);

	pwm_pwmReload(pwm_id);
}

void pwm_setup_start(u32 pwm_id, u32 duty_cycle, u32 inv)
{
	u32 i = 0;

	for (i = 0; i < PWMID_NO_TOTAL_CNT; i++) {
		if ((pwm_id >> i) & 0x1)
			drv_pwm_setup_start((0x1 << i), duty_cycle, inv);
	}
}

void pwm_reload(u32 pwm_id, u32 duty_cycle, u32 inv)
{
	u32 i = 0;

	for (i = 0; i < PWMID_NO_TOTAL_CNT; i++) {
		if ((pwm_id >> i) & 0x1)
			drv_pwm_reload((0x1 << i), duty_cycle, inv);
	}
}

int pwm_close(u32 uiPWMId, BOOL bWaitAutoDisableDone)
{
	u32 i = 0, value;

	for (i = 0; i < PWMID_NO_TOTAL_CNT; i++) {
		if ((uiPWMId >> i) & 0x1) {
			value = drv_pwm_close((0x1 << i), bWaitAutoDisableDone);
			if (value)
				printf("PWM%d close fail!\n", i);
		}
	}
	return 0;
}
