/*
 * Battery charger driver for X-Powers AXP
 *
 * Copyright (C) 2013 X-Powers, Ltd.
 *  Zhang Donglu <zhangdonglu@x-powers.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */


#if defined(__ECOS)

#include "axp-cfg.h"
#include "axp-mfd-216.h"
#include "axp-sply.h"
//#include "usart1.h"	//打印函数头文件

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          AxpCtrl
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
static BOOL g_axp_debug_ctr = FALSE;
static BOOL g_iSysshutdownKeepPower = FALSE;

static void axp_set_charge(void);

void set_axp_pmu_debug(BOOL flag)
{
	g_axp_debug_ctr = flag;
}

void shutdown_pmu_no_power(void)
{
	g_iSysshutdownKeepPower = FALSE;
}
void shutdown_pmu_keep_power(void)
{
	g_iSysshutdownKeepPower = TRUE;
}

void set_axp_pmu_poweroff(void)
{
	if(g_iSysshutdownKeepPower){
		DBG_DUMP("shutdown_pmu_keep_power !\r\n");
	}
	else{
//control to trun off the power output,except the RTC LDO
		DBG_DUMP("shutdown_pmu_close_power !\r\n");
		axp_set_bits(0x32,0x80); 	//bit7:
	}
}
void set_gps_ldo_vol(uint8_t onoff)
{
			//0.7~3.3v,100mV/step
//ALDO1 = 3.3V
	if(onoff)
	{
		//LDO VOL
		AXP216_write( AXP_ELDO2OUT_VOL, 26);
		//LDO Ctr
	    	axp_set_bits(AXP_LDO_DC_EN2, 0x02);		//BIT1:ELDO2
	}
	else
	{
		axp_clr_bits(AXP_LDO_DC_EN2, 0x02);		//BIT1:ELDO2
	}
}

static void set_sensor_audo_vol(uint8_t onoff)
{
			//0.7~3.3v,100mV/step
//ALDO = 3.3V
	if(onoff)
	{
		//LDO VOL
		AXP216_write( 0x93, 0x1f);
		//LDO Ctr
	    	AXP216_write(AXP_GPIO1_CTL, 0x03);		//BIT0~2:011
	}
	else
	{
	    	axp_set_bits(AXP_GPIO1_CTL, 0x07);		//BIT0~2:111
	}

}

static void set_sensor_ldo_vol(void)
{
			//0.7~3.3v,100mV/step
//ALDO1 = 2.8V
//ALDO2 = 1.8V
//ALDO3 = 1.2V
	//LDO VOL
	AXP216_write( AXP_ALDO1OUT_VOL, 21);
	AXP216_write( AXP_ALDO2OUT_VOL, 11);
	//AXP216_write( AXP_ALDO3OUT_VOL, 5);
	AXP216_write( AXP_ALDO3OUT_VOL, 21);	// miotone_2018810 zejia change 5 to 21
	//LDO Ctr
	axp_set_bits(AXP_LDO_DC_EN1, 0xc0);		//BIT7:ALDO2  BIT6:ALDO1
	axp_set_bits(AXP_LDO_DC_EN2, 0x20);		//BIT5:ALDO3
}

static void axp_enable_irqs( uint64_t irqs)
{
	uint8_t v[9];
	v[0] = 0x00;//(irqs) & 0xff;
	v[1] = 0x00;//((irqs) >> 8) & 0xff;
	v[2] = 0x00;//((irqs) >> 16) & 0xff;
	v[3] = 0x00;//((irqs) >> 24) & 0xff;
	v[4] = 0x00;//((irqs) >> 32) & 0xff;

    AXP216_write( AXP_INTEN1, v[0]);
	DBG_IND("AXP_INTEN1(reg_0x40)=0x%2x",v[0]);
	AXP216_write( AXP_INTEN2, v[1]);
	DBG_IND("AXP_INTEN2(reg_0x41)=0x%2x",v[1]);
	AXP216_write( AXP_INTEN3, v[2]);
	DBG_IND("AXP_INTEN3(reg_0x42)=0x%2x",v[2]);
    AXP216_write( AXP_INTEN4, v[3]);
	DBG_IND("AXP_INTEN4(reg_0x43)=0x%2x",v[3]);
	AXP216_write( AXP_INTEN5, v[4]);
	DBG_IND("AXP_INTEN5(reg_0x44)=0x%2x",v[4]);


}


//设置低电保护电压点
static void set_voff_vol(void)
{
	uint8_t val;
	if(SHUTDOWNVOL >= 2600 && SHUTDOWNVOL <= 3300){
		if (SHUTDOWNVOL > 3200){
			val = 0x7;
		}
		else if (SHUTDOWNVOL > 3100){
			val = 0x6;
		}
		else if (SHUTDOWNVOL > 3000){
			val = 0x5;
		}
		else if (SHUTDOWNVOL > 2900){
			val = 0x4;
		}
		else if (SHUTDOWNVOL > 2800){
			val = 0x3;
		}
		else if (SHUTDOWNVOL > 2700){
			val = 0x2;
		}
		else if (SHUTDOWNVOL > 2600){
			val = 0x1;
		}
		else
			val = 0x0;

		axp_update(AXP_VOFF_SET, val, 0x7);

		}
		}



//AXP216初始化函数
void AXP216_init(void)
{
  int var,tmp;
  uint8_t val2,val,val3;
  uint8_t ocv_cap[32],i;
  int Cur_CoulombCounter,rdc;

    POWERIC_INFO PowerICInfo;
    PowerICInfo.I2C_RegBytes = POWERIC_I2C_REGISTER_1BYTE;
    PowerICInfo.I2C_PinMux = I2C_PINMUX_2ND;
    PowerICInfo.I2C_BusClock = I2C_BUS_CLOCK_200KHZ;//I2C_BUS_CLOCK_400KHZ;
    // PowerIC AXP216 I2C slave addresses
    PowerICInfo.I2C_Slave_WAddr = AXP_DEVICES_ADDR;
    PowerICInfo.I2C_Slave_RAddr = (AXP_DEVICES_ADDR+1);
    if(PowerIC_I2C_Init(PowerICInfo) == FALSE)
    {
        DBG_ERR("PowerIC IIC initial Fail!!!\r\n");
    }
	else
    {
        DBG_DUMP("PowerIC IIC initial Success!!!\r\n");
//-------------------------------------------
	set_sensor_ldo_vol();
	set_gps_ldo_vol(TRUE);
	set_sensor_audo_vol(TRUE);

	AXP216_write(0x23, 45);		//DCDC3 1.5v

	AXP216_write(AXP_ADC_CONTROL, 0xE1);		//BIT0:Enable TS function
	//axp_clr_bits(AXP_ADC_CONTROL3, 0x02);		//BIT2: 电池温度检测功能
	AXP216_write(AXP_ADC_CONTROL3, 0x32);		//BIT2: 电池温度检测功能

	AXP216_write(0x38, 0xAC);	//低温门限	:0度
	//AXP216_write(0x39, 0x1E);	//高温门限	:45度
	AXP216_write(0x39, 0x16);	//高温门限	:55度
	//AXP216_write(AXP_ADC_CONTROL3, 0x02);
	AXP216_write(AXP_ADC_CONTROL3, 0x32);

	//AXP216_write(AXP_DC5OUT_VOL,0x04);//DCDC5 OUTPUT 1.2V

	AXP216_read(AXP_ADJUST_PARA,1,&val);
	DBG_DUMP("AXP REG_E8 = %x\r\n",val);
	AXP216_write(AXP_ADJUST_PARA, 0x04);	//20171010

	//AXP216_write( AXP_OFFLEVEL_DELAY, 1);	//off level 	bit0~2:0s/10s/20s/30s/40s/50s/60s/70s
//-------------------------------------------
  axp_set_charge();	 //充电参数设置
  set_voff_vol()  ;	  //Voff电压设定
  axp_enable_irqs(AXP_NOTIFIER_ON);

  /*ADC Set*/
  AXP216_read( AXP_ADC_CONTROL3,1, &val3);
  switch (ADCFREQ/100){
  case 1: val3 &= ~(3 << 6);break;
  case 2: val3 &= ~(3 << 6);val3 |= 1 << 6;break;
  case 4: val3 &= ~(3 << 6);val3 |= 2 << 6;break;
  case 8: val3 |= 3 << 6;break;
  default: break;
  }
  AXP216_write(AXP_ADC_CONTROL3, val3);

  /* usb voltage limit */
  if((USBVOLLIM) && (USBVOLLIMEN)){
    axp_set_bits(AXP_CHARGE_VBUS, 0x40);
  	var = USBVOLLIM * 1000;
  	if(var >= 4000000 && var <=4700000){
    	tmp = (var - 4000000)/100000;
    	AXP216_read(AXP_CHARGE_VBUS,1,&val);
    	val &= 0xC7;
    	val |= tmp << 3;
    	AXP216_write( AXP_CHARGE_VBUS,val);
  	}
  }
  else
    axp_clr_bits( AXP_CHARGE_VBUS, 0x40);

	/*usb current limit*/
  if((USBCURLIM) && (USBCURLIMEN)){
    axp_clr_bits(AXP_CHARGE_VBUS, 0x02);
    var = USBCURLIM * 1000;
  	if(var == 900000)
    	axp_clr_bits(AXP_CHARGE_VBUS, 0x03);
  	else if (var == 500000){
    	axp_set_bits(AXP_CHARGE_VBUS, 0x01);
  	}
  }
  else
    axp_set_bits(AXP_CHARGE_VBUS, 0x03);


  // set lowe power warning/shutdown level
  AXP216_write( AXP_WARNING_LEVEL,((BATLOWLV1-5) << 4)+BATLOWLV2);

  ocv_cap[0]  = OCVREG0;
  ocv_cap[1]  = OCVREG1;
  ocv_cap[2]  = OCVREG2;
  ocv_cap[3]  = OCVREG3;
  ocv_cap[4]  = OCVREG4;
  ocv_cap[5] = OCVREG5;
  ocv_cap[6] = OCVREG6;
  ocv_cap[7] = OCVREG7;
  ocv_cap[8] = OCVREG8;
  ocv_cap[9] = OCVREG9;
  ocv_cap[10] = OCVREGA;
  ocv_cap[11] = OCVREGB;
  ocv_cap[12] = OCVREGC;
  ocv_cap[13] = OCVREGD;
  ocv_cap[14] = OCVREGE;
  ocv_cap[15] = OCVREGF;
  ocv_cap[16] = OCVREG10;
  ocv_cap[17] = OCVREG11;
  ocv_cap[18] = OCVREG12;
  ocv_cap[19] = OCVREG13;
  ocv_cap[20] = OCVREG14;
  ocv_cap[21] = OCVREG15;
  ocv_cap[22] = OCVREG16;
  ocv_cap[23] = OCVREG17;
  ocv_cap[24] = OCVREG18;
  ocv_cap[25] = OCVREG19;
  ocv_cap[26] = OCVREG1A;
  ocv_cap[27] = OCVREG1B;
  ocv_cap[28] = OCVREG1C;
  ocv_cap[29] = OCVREG1D;
  ocv_cap[30] = OCVREG1E;
  ocv_cap[31] = OCVREG1F;

   for(i=0;i<32;i++)
	{
	AXP216_write( 0xC0+i,ocv_cap[i]);
	 }

	/* pok open time set */
	AXP216_read(AXP_POK_SET,1,&val);
	if (PEKOPEN < 1000)
		val &= 0x3f;
	else if(PEKOPEN < 2000){
		val &= 0x3f;
		val |= 0x40;
	}
	else if(PEKOPEN < 3000){
		val &= 0x3f;
		val |= 0x80;
	}
	else {
		val &= 0x3f;
		val |= 0xc0;
	}
	AXP216_write(AXP_POK_SET,val);

	/* pok long time set*/
	if(PEKLONG < 1000)
		tmp = 1000;
	else if(PEKLONG > 2500)
		tmp = 2500;
	else
		tmp = PEKLONG;
	AXP216_read(AXP_POK_SET,1,&val);
	val &= 0xcf;
	val |= (((tmp - 1000) / 500) << 4);
	AXP216_write(AXP_POK_SET,val);

	/* pek offlevel poweroff en set*/
	if(PEKOFFEN)
	{
			tmp = 1;
	}
	else
	{
			tmp = 0;
	}
	AXP216_read(AXP_POK_SET,1,&val);
	val &= 0xf7;
	val |= (tmp << 3);
	AXP216_write(AXP_POK_SET,val);

	/*Init offlevel restart or not */
	if(PEKOFFRESTART)
	{
			axp_set_bits(AXP_POK_SET,0x04); //restart
	}
	else
	{
			axp_clr_bits(AXP_POK_SET,0x04); //not restart
	}

	/* pek delay set */
	AXP216_read(AXP_OFF_CTL,1,&val);
	val &= 0xfc;
	val |= ((PEKDELAY / 8) - 1);
	AXP216_write(AXP_OFF_CTL,val);

	/* pek offlevel time set */
	if(PEKOFF < 4000)
		tmp = 4000;
	else if(PEKOFF > 10000)
		tmp =10000;
	else
		tmp = PEKOFF;
	tmp = (tmp - 4000) / 2000 ;
	AXP216_read(AXP_POK_SET,1,&val);
	val &= 0xfc;
	val |= tmp ;
	AXP216_write(AXP_POK_SET,val);
	/*Init 16's Reset PMU en */
	if(PMURESET)
	{
		axp_set_bits(0x8F,0x08); //enable
	}
	else
	{
		axp_clr_bits(0x8F,0x08); //disable
	}

	/*Init IRQ wakeup en*/
	if(IRQWAKEUP)
	{
		axp_set_bits(0x8F,0x80); //enable
	}
	else
	{
		axp_clr_bits(0x8F,0x80); //disable
	}

	/*Init N_VBUSEN status*/
	if(VBUSEN)
	{
		axp_set_bits(0x8F,0x10); //output
	}
	else
	{
		axp_clr_bits(0x8F,0x10); //input
	}

	/*Init InShort status*/
	if(VBUSACINSHORT)
	{
		axp_set_bits(0x8F,0x60); //InShort
	}
	else
	{
		axp_clr_bits(0x8F,0x60); //auto detect
	}

	/*Init CHGLED function*/
	if(CHGLEDFUN)
	{
		axp_set_bits(0x32,0x08); //control by charger
	}
	else
	{
		axp_clr_bits(0x32,0x08); //drive MOTO
	}

	/*set CHGLED Indication Type*/
	if(CHGLEDTYPE)
	{
		axp_set_bits(0x45,0x10); //Type A
	}
	else
	{
		axp_clr_bits(0x45,0x10); //Type B
	}

	/*Init PMU Over Temperature protection*/
	if(OTPOFFEN)
	{
		axp_set_bits(0x8f,0x04); //enable
	}
	else
	{
		axp_clr_bits(0x8f,0x04); //disable
	}

	/*Init battery capacity correct function*/
	if(BATCAPCORRENT)
	{
		axp_set_bits(0xb8,0x20); //enable
	}
	else
	{
		axp_clr_bits(0xb8,0x20); //disable
	}
	/* Init battery regulator enable or not when charge finish*/
	if(BATREGUEN)
	{
		axp_set_bits(0x34,0x20); //enable
	}
	else
	{
		axp_clr_bits(0x34,0x20); //disable
	}

	if(!BATDET)
		axp_clr_bits(AXP_PDBC,0x40);
	else
		axp_set_bits(AXP_PDBC,0x40);


/* RDC initial */
	AXP216_read( AXP_RDC0,1,&val2);
	if((BATRDC) && (!(val2 & 0x40)))		//如果配置电池内阻，则手动配置
	{
		rdc = (BATRDC * 10000 + 5371) / 10742;
		AXP216_write( AXP_RDC0, ((rdc >> 8) & 0x1F)|0x80);
	    AXP216_write( AXP_RDC1,rdc & 0x00FF);
	}

//初始化RDC，使其提前计算正确的OCV，然后在此处启动计量系统
	AXP216_read(AXP_BATCAP0,1,&val2);
	if((BATCAP) && (!(val2 & 0x80)))
	{
		Cur_CoulombCounter = BATCAP * 1000 / 1456;
		AXP216_write( AXP_BATCAP0, ((Cur_CoulombCounter >> 8) | 0x80));
	    AXP216_write(AXP_BATCAP1,Cur_CoulombCounter & 0x00FF);
	}
	else if(!BATCAP)
	{
		AXP216_write(AXP_BATCAP0, 0x00);
		AXP216_write(AXP_BATCAP1,0x00);
	}


  /*电量读取  */
    AXP216_read(AXP_CAP,1,&val2);
  	//charger->rest_vol = (int) (val2 & 0x7F);
    DBG_IND("now_rest_CAP = %d\n",(val2 & 0x7F));
		}
}




static void axp_read_adc(struct axp_adc_res *adc)
{
  uint8_t tmp[8];
//
//  axp_reads(charger->master,AXP_VACH_RES,8,tmp);
  adc->vac_res = 0;
  adc->iac_res = 0;
  adc->vusb_res = 0;
  adc->iusb_res = 0;
  AXP216_read(AXP_VBATH_RES,6,tmp);
  adc->vbat_res = ((uint16_t) tmp[0] << 8 )| tmp[1];
  adc->ichar_res = ((uint16_t) tmp[2] << 8 )| tmp[3];
  adc->idischar_res = ((uint16_t) tmp[4] << 8 )| tmp[5];
  AXP216_read(AXP_OCVBATH_RES,2,tmp);
  adc->ocvbat_res = ((uint16_t) tmp[0] << 8 )| tmp[1];
}


static int axp_vbat_to_mV(uint16_t reg)
{
  return ((int)((( reg >> 8) << 4 ) | (reg & 0x000F))) * 1100 / 1000;
}


static  int axp_ocvbat_to_mV(uint16_t reg)
{
  return ((int)((( reg >> 8) << 4 ) | (reg & 0x000F))) * 1100 / 1000;
}


static int axp_vdc_to_mV(uint16_t reg)
{
  return ((int)(((reg >> 8) << 4 ) | (reg & 0x000F))) * 1700 / 1000;
}


static int axp_ibat_to_mA(uint16_t reg)
{
    return ((int)(((reg >> 8) << 4 ) | (reg & 0x000F))) ;
}

static int axp_icharge_to_mA(uint16_t reg)
{
    return ((int)(((reg >> 8) << 4 ) | (reg & 0x000F)));
}

static int axp_iac_to_mA(uint16_t reg)
{
    return ((int)(((reg >> 8) << 4 ) | (reg & 0x000F))) * 625 / 1000;
}

static int axp_iusb_to_mA(uint16_t reg)
{
    return ((int)(((reg >> 8) << 4 ) | (reg & 0x000F))) * 375 / 1000;
}





static void axp_charger_update_state(struct axp_charger *charger)
{
  uint8_t val[2];
  uint16_t tmp;
  AXP216_read(AXP_CHARGE_STATUS,2,val);
  tmp = (val[1] << 8 )+ val[0];
  charger->is_on = (val[1] & AXP_IN_CHARGE) ? 1 : 0;
  charger->fault = val[1];
  charger->bat_det = (tmp & AXP_STATUS_BATEN)?1:0;
  charger->ac_det = (tmp & AXP_STATUS_ACEN)?1:0;
  charger->usb_det = (tmp & AXP_STATUS_USBEN)?1:0;
  charger->usb_valid = (tmp & AXP_STATUS_USBVA)?1:0;
  charger->ac_valid = (tmp & AXP_STATUS_ACVA)?1:0;
  charger->ext_valid = charger->ac_valid | charger->usb_valid;
  charger->bat_current_direction = (tmp & AXP_STATUS_BATCURDIR)?1:0;
  charger->in_short = (tmp& AXP_STATUS_ACUSBSH)?1:0;
  charger->batery_active = (tmp & AXP_STATUS_BATINACT)?1:0;
  charger->int_over_temp = (tmp & AXP_STATUS_ICTEMOV)?1:0;
  AXP216_read(AXP_CHARGE_CONTROL1,1,val);
  charger->charge_on = ((val[0] >> 7) & 0x01);
}



struct axp_adc_res adc;

static void axp_charger_update(struct axp_charger *charger)
{
  uint16_t tmp;
  uint8_t val[2];
	//int bat_temp_mv;
  charger->adc = &adc;
  axp_read_adc(&adc);
  tmp = charger->adc->vbat_res;
  charger->vbat = axp_vbat_to_mV(tmp);
  tmp = charger->adc->ocvbat_res;
  charger->ocv = axp_ocvbat_to_mV(tmp);
   //tmp = charger->adc->ichar_res + charger->adc->idischar_res;
  charger->ibat = ABS(axp_icharge_to_mA(charger->adc->ichar_res)-axp_ibat_to_mA(charger->adc->idischar_res));
  tmp = 00;
  charger->vac = axp_vdc_to_mV(tmp);
  tmp = 00;
  charger->iac = axp_iac_to_mA(tmp);
  tmp = 00;
  charger->vusb = axp_vdc_to_mV(tmp);
  tmp = 00;
  charger->iusb = axp_iusb_to_mA(tmp);
  AXP216_read(AXP_INTTEMP,2,val);
  //DBG_PSY_MSG("TEMPERATURE:val1=0x%x,val2=0x%x\n",val[1],val[0]);
  tmp = (val[0] << 4 ) + (val[1] & 0x0F);
  charger->ic_temp = (int) tmp *1063/10000  - 2667/10;
  charger->disvbat =  charger->vbat;
  charger->disibat =  axp_ibat_to_mA(charger->adc->idischar_res);
//	tmp = charger->adc->ts_res;
//	bat_temp_mv = axp22_vts_to_mV(tmp);
//	charger->bat_temp = axp22_vts_to_temp(bat_temp_mv);
}




 struct axp_charger charger;

 struct axp_charger  axp_charging_monitor(void)
{

	uint8_t	val,temp_val[4];

	uint8_t	batt_max_cap_val[3];
	int	batt_max_cap,coulumb_counter;


	axp_charger_update_state(&charger);
	axp_charger_update(&charger);

	AXP216_read( AXP_CAP,1,&val);
	charger.rest_vol	= (int)	(val & 0x7F);

	AXP216_read(0xe2,2,temp_val);
	coulumb_counter = (((temp_val[0] & 0x7f) <<8) + temp_val[1])*1456/1000;
	AXP216_read(0xe0,2,temp_val);
	batt_max_cap = (((temp_val[0] & 0x7f) <<8) + temp_val[1])*1456/1000;

	/* Avoid the power stay in 100% for a long time. */
	if(coulumb_counter > batt_max_cap){
		batt_max_cap_val[0] = temp_val[0] | (0x1<<7);
		batt_max_cap_val[1] = temp_val[1];
		AXP216_write(0xe2,batt_max_cap_val[0]);
		AXP216_write(0xe3,batt_max_cap_val[1]);
		DBG_DUMP("Axp22 coulumb_counter = %d\n",batt_max_cap);

		}
#if 1
	if((axp_debug)&&(g_axp_debug_ctr)){
#else
	if(axp_debug){
#endif
		DBG_DUMP("*************** AXP216 Debug********************* \r\n");
		DBG_DUMP("charger->ic_temp = %d\r\n",charger.ic_temp);
		DBG_DUMP("charger->vbat = %d\r\n",charger.vbat);
		DBG_DUMP("charger->ibat = %d\r\n",charger.ibat);
		DBG_DUMP("charger->ocv = %d\r\n",charger.ocv);
		DBG_DUMP("charger->disvbat = %d\r\n",charger.disvbat);
		DBG_DUMP("charger->disibat = %d\r\n",charger.disibat);
		DBG_DUMP("charger->rest_vol = %d\r\n",charger.rest_vol); //电量百分比
		AXP216_read(0xba,2,temp_val);
		DBG_DUMP("AXP Rdc = %d\r\n",(((temp_val[0] & 0x1f) <<8) + temp_val[1])*10742/10000);
		AXP216_read(0xe0,2,temp_val);
		DBG_DUMP("AXP batt_max_cap = %d\r\n",(((temp_val[0] & 0x7f) <<8) + temp_val[1])*1456/1000);
		AXP216_read(0xe2,2,temp_val);
		DBG_DUMP("AXP coulumb_counter = %d\r\n",(((temp_val[0] & 0x7f) <<8) + temp_val[1])*1456/1000);
		AXP216_read(0xb8,1,temp_val);
		DBG_DUMP("AXP REG_B8 = %x\r\n",temp_val[0]);
		AXP216_read(0xe4,2,temp_val);
		DBG_DUMP("AXP OCV_percentage = %d\r\n",(temp_val[0] & 0x7f));
		DBG_DUMP("AXP Coulumb_percentage = %d\r\n",(temp_val[1] & 0x7f));
		DBG_DUMP("charger->is_on = %d\r\n",charger.is_on);	 //充电指示
		DBG_DUMP("charger->bat_current_direction = %d\r\n",charger.bat_current_direction);
		DBG_DUMP("charger->charge_on = %d\r\n",charger.charge_on);//充电使能
		DBG_DUMP("charger->ext_valid = %d\r\n",charger.ext_valid);

		AXP216_read(0x33,1,temp_val);
		DBG_DUMP("AXP REG_33 = %x\r\n",temp_val[0]);
		temp_val[0]= temp_val[0]&0x0f ;
		DBG_DUMP("pmu_chgcur = %d\r\n",temp_val[0]*150+300);  //充电电流配置

		AXP216_read(0x31,1,temp_val);
		DBG_DUMP("AXP REG_31 = %x\r\n",temp_val[0]);
		AXP216_read(AXP_CHARGE_VBUS,1,temp_val);
		DBG_DUMP("AXP REG_30 = %x\r\n",temp_val[0]);


		DBG_DUMP("********************END********************* \r\n");
	}
	return  charger;
}


 static void axp_set_charge(void)
{
  uint8_t val=0x00;
  uint8_t tmp=0x00;
  uint8_t CHG_EN;
  int chgpretime,chgcsttime,chgcur ;

	CHG_EN=CHGEN;
	chgpretime=CHGPRETIME;
	chgcsttime = CHGCSTTIME;
	chgcur=	STACHGCUR;


	if(CHGVOL < 4200000){
		val &= ~(3 << 5);
	}else if (CHGVOL<4240000){
		val &= ~(3 << 5);
		val |= 1 << 5;
	}else if (CHGVOL<4350000){
		val &= ~(3 << 5);
		val |= 1 << 6;
	}else
		val |= 3 << 5;


	if(STACHGCUR == 0)
		CHG_EN = 0;

    if(chgcur< 300000)
      chgcur = 300000;
    else if(chgcur > 2550000)
     chgcur = 2550000;

    val |= (chgcur - 300000) / 150000 ;

    if(ENDCHGRATE == 10){
      val &= ~(1 << 4);
    }
    else {
      val |= 1 << 4;
    }

    val &= 0x7F;

    val |= CHG_EN << 7;

    if(chgpretime < 30)
      chgpretime = 30;
    if(chgcsttime < 360)
      chgcsttime = 360;

    tmp = ((((chgpretime - 40) / 10) << 6)  \
      | ((chgcsttime - 360) / 120));

	AXP216_write(AXP_CHARGE_CONTROL1,val);
	axp_update(AXP_CHARGE_CONTROL2,tmp,0xC2);

}

#endif
