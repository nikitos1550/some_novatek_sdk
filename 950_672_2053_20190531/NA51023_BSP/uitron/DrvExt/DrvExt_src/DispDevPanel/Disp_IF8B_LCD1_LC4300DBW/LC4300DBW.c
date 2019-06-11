/*
    Display object object panel configuration parameters for ILI9342C

    @file       ILI9342C.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "display.h"
#include "dispdev_IF8bits.h"
#include "dispdev_panlcomn.h"
#include "Debug.h"

#define IFCMD_CMD		0x01000000
#define IFCMD_DATA		0x02000000

#define Delayms(value)	{CMDDELAY_MS, value}
#define wr_cmd(value)	{IFCMD_CMD, value}
#define wr_dat(value)	{IFCMD_DATA, value}

/*
    Panel Parameters for ILI9342C
*/
//@{
const tPANEL_CMD tCmdStandby[] = {
	{CMDDELAY_US,0x10}
};

const tPANEL_CMD tCmdModeRgbd320[] = {
	/*{CMDDELAY_MS, 100},
	{IFCMD_CMD, 0x01},
	{CMDDELAY_MS, 200},

	{IFCMD_CMD,0xC8},
	{IFCMD_DATA,0xFF},
	{IFCMD_DATA,0x93},
	{IFCMD_DATA,0x42},

	//{IFCMD_CMD, 0x21},

	{IFCMD_CMD, 0x36},
	{IFCMD_DATA,0x00},

	{IFCMD_CMD, 0x3A},
	{IFCMD_DATA,0x55},

	{IFCMD_CMD, 0xC0},
	{IFCMD_DATA,0x15},//{IFCMD_DATA,0x09},//{IFCMD_DATA,0x13},
	{IFCMD_DATA,0x15},//{IFCMD_DATA,0x0E},

	{IFCMD_CMD, 0xC1},
	{IFCMD_DATA,0x01},//{IFCMD_DATA,0x02},

	{IFCMD_CMD, 0xC5},
	{IFCMD_DATA,0xF5},//{IFCMD_DATA,0xD3},


//	{IFCMD_CMD, 0xC7},
//	{IFCMD_DATA,0Xc6},
//	{IFCMD_DATA,0X00},
//	{IFCMD_DATA,0X02},

	{IFCMD_CMD, 0xB1},
	{IFCMD_DATA,0x00},
	{IFCMD_DATA,0x1B},

	{IFCMD_CMD, 0xB4},
	{IFCMD_DATA,0x02},

	{IFCMD_CMD, 0xB0},
	{IFCMD_DATA,0xB1},

	{IFCMD_CMD, 0xf6},
	{IFCMD_DATA,0x00},
	{IFCMD_DATA,0x00},
	{IFCMD_DATA,0x03},

	{IFCMD_CMD, 0xE0},
	{IFCMD_DATA,0x0F},//P01-VP63
	{IFCMD_DATA,0x13},//P02-VP62
	{IFCMD_DATA,0x17},//P03-VP61
	{IFCMD_DATA,0x04},//P04-VP59
	{IFCMD_DATA,0x13},//P05-VP57
	{IFCMD_DATA,0x07},//P06-VP50
	{IFCMD_DATA,0x40},//P07-VP43
	{IFCMD_DATA,0x39},//P08-VP27,36
	{IFCMD_DATA,0x4F},//P09-VP20
	{IFCMD_DATA,0x06},//P10-VP13
	{IFCMD_DATA,0x0D},//P11-VP6
	{IFCMD_DATA,0x0A},//P12-VP4
	{IFCMD_DATA,0x1F},//P13-VP2
	{IFCMD_DATA,0x22},//P14-VP1
	{IFCMD_DATA,0x00},//P15-VP0

	{IFCMD_CMD, 0xE1},
	{IFCMD_DATA,0x00},//P01
	{IFCMD_DATA,0x21},//P02
	{IFCMD_DATA,0x24},//P03
	{IFCMD_DATA,0x03},//P04
	{IFCMD_DATA,0x0F},//P05
	{IFCMD_DATA,0x05},//P06
	{IFCMD_DATA,0x38},//P07
	{IFCMD_DATA,0x32},//P08
	{IFCMD_DATA,0x49},//P09
	{IFCMD_DATA,0x00},//P10
	{IFCMD_DATA,0x09},//P11
	{IFCMD_DATA,0x08},//P12
	{IFCMD_DATA,0x32},//P13
	{IFCMD_DATA,0x35},//P14
	{IFCMD_DATA,0x0F},//P15

	{IFCMD_CMD, 0x11}, //Exit Sleep
	{CMDDELAY_MS, 10},
	{IFCMD_CMD, 0x11}, //Exit Sleep

	{CMDDELAY_MS, 120},

	{IFCMD_CMD, 0x29}, //Display ON
	{CMDDELAY_MS, 180},*/

#if 0
	 Delayms(50),
	 Delayms(100),

	 wr_cmd(0x01),
	 Delayms(120),

	 wr_cmd(0xC8),
	 wr_dat(0xFF),
	 wr_dat(0x93),
	 wr_dat(0x42),

	 wr_cmd(0x36),
	 wr_dat(0xc8),//C8. Rotate dir setting.

	 wr_cmd(0x3A),
	 wr_dat(0x55),	   //18BIT PIX FOMART

	 wr_cmd(0xC0),
	 wr_dat(0x0F),//0F
	 wr_dat(0x0F),//0F

	 wr_cmd(0xC1),
	 wr_dat(0x05),//05

	 wr_cmd(0xC5),
	 wr_dat(0xDA),//DB	FA

	 wr_cmd(0xB4),
	 wr_dat(0x02),//02	inversion

	 wr_cmd(0xB0),
	 wr_dat(0xE3),//e0

	 wr_cmd(0xF6),
	 wr_dat(0x01),//01
	 wr_dat(0x00),
	 wr_dat(0x07),//07

	 wr_cmd(0xE0), //Gamma2.2
	 wr_dat(0x00),
	 wr_dat(0x07),
	 wr_dat(0x0c),
	 wr_dat(0x05),
	 wr_dat(0x12),
	 wr_dat(0x07),
	 wr_dat(0x33),
	 wr_dat(0x76),
	 wr_dat(0x43),
	 wr_dat(0x07),
	 wr_dat(0x0D),
	 wr_dat(0x0B),
	 wr_dat(0x17),
	 wr_dat(0x18),
	 wr_dat(0x0F),


	 wr_cmd(0xE1),
	 wr_dat(0x00),
	 wr_dat(0x25),
	 wr_dat(0x27),
	 wr_dat(0x03),
	 wr_dat(0x0f),
	 wr_dat(0x06),
	 wr_dat(0x3C),
	 wr_dat(0x37),
	 wr_dat(0x4D),
	 wr_dat(0x05),
	 wr_dat(0x0E),
	 wr_dat(0x0B),
	 wr_dat(0x31),
	 wr_dat(0x34),
	 wr_dat(0x0F),

	 wr_cmd(0x11), //Exit Sleep
	 Delayms(120),
	 wr_cmd(0x29), //Display on

	 wr_cmd(0x2C),

	 Delayms(20),

#else
/*
	//RESET_H,
	Delayms(10),
	//RESET_L,
	Delayms(10),
	//RESET_H,
	Delayms(150),
	wr_cmd(0x01),
	Delayms(150),

	wr_cmd(0xC8),
	wr_dat(0xFF),
	wr_dat(0x93),
	wr_dat(0x42),

	wr_cmd(0xC0),
	wr_dat(0x0f), //0F
	wr_dat(0x0f), //0F

	wr_cmd(0xC1),
	wr_dat(0x10), //01

	wr_cmd(0xC5),
	wr_dat(0xB2), //DB

	wr_cmd(0x36),
	wr_dat(0x08), //0xc8

	wr_cmd(0x3A),
	wr_dat(0x66), //18BIT PIX FOMART

	wr_cmd(0xB0),
	wr_dat(0xE0),

	wr_cmd(0xB4),
	wr_dat(0x02), //02 inversion

	wr_cmd(0xB7),
	wr_dat(0x07),

	wr_cmd(0xF6),
	wr_dat(0x01), //01
	wr_dat(0x00),
	wr_dat(0x07),

	wr_cmd(0xE0), //Gamma=2.2
	wr_dat(0x00),
	wr_dat(0x25),
	wr_dat(0x2C),
	wr_dat(0x09),
	wr_dat(0x15),
	wr_dat(0x09),
	wr_dat(0x51),
	wr_dat(0x75),
	wr_dat(0x61),
	wr_dat(0x05),
	wr_dat(0x0C),
	wr_dat(0x06),
	wr_dat(0x30),
	wr_dat(0x32),
	wr_dat(0x0F),

	wr_cmd(0xE1), //Set Gamma
	wr_dat(0x00),
	wr_dat(0x33),
	wr_dat(0x38),
	wr_dat(0x02),
	wr_dat(0x0e),
	wr_dat(0x03),
	wr_dat(0x4b),
	wr_dat(0x32),
	wr_dat(0x5d),
	wr_dat(0x05),
	wr_dat(0x10),
	wr_dat(0x0f),
	wr_dat(0x39),
	wr_dat(0x3b),
	wr_dat(0x0F),

	wr_cmd(0x20),

	wr_cmd(0x11), //Exit Sleep
	Delayms(120),
	wr_cmd(0x29), //Display on

	wr_cmd(0x2C),
*/
#endif
};

const tLCD_PARAM tMode[] = {
	/***********       RGB Dummy-320 MODE         *************/
	{
	#if 0
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_RGB_THROUGH,			//!< LCDMode
			27.00f,								//!< fDCLK
			1028,								//!< uiHSyncTotalPeriod
			960,								//!< uiHSyncActivePeriod
			6,//19,									//!< uiHSyncBackPorch
			262,								//!< uiVSyncTotalPeriod
			240,								//!< uiVSyncActivePeriod
			2,//1,									//!< uiVSyncBackPorchOdd
			2,//1,									//!< uiVSyncBackPorchEven
			320,								//!< uiBufferWidth
			240,								//!< uiBufferHeight
			320,								//!< uiWindowWidth
			240,								//!< uiWindowHeight
			FALSE,								//!< bYCbCrFormat

			/* New added parameters */
			0x01,								//!< uiHSyncSYNCwidth
			0x01								//!< uiVSyncSYNCwidth
		},

		// tIDE_PARAM
		{
			/* Old prototype */
			PINMUX_LCD_SEL_SERIAL_RGB_6BITS,	//!< pinmux_select_lcd;
			ICST_CCIR601,						//!< icst;
			{FALSE,FALSE},						//!< dithering[2];
			DISPLAY_DEVICE_CASIO2G,				//!< **DONT-CARE**
			IDE_PDIR_RGB,						//!< pdir;
			IDE_LCD_G,							//!< odd;
			IDE_LCD_G,							//!< even;
			TRUE,								//!< hsinv;
			TRUE,								//!< vsinv;

			FALSE,								//!< hvldinv;
			FALSE,								//!< vvldinv;

			TRUE,								//!< clkinv;
			TRUE,								//!< fieldinv;

			FALSE,								//!< **DONT-CARE**
			FALSE,								//!< interlace;
			FALSE,								//!< **DONT-CARE**
			0x40,								//!< ctrst;
			0x00,								//!< brt;
			0x40,								//!< cmults;
			FALSE,								//!< cex;
			FALSE,								//!< **DONT-CARE**
			TRUE,								//!< **DONT-CARE**
			TRUE,								//!< tv_powerdown;
			{0x00,0x00},						//!< **DONT-CARE**

			/* New added parameters */
			FALSE,								//!< YCex
			FALSE,								//!< HLPF
			{FALSE,FALSE,FALSE},				//!< subpixOdd[3]
			{FALSE,FALSE,FALSE},				//!< subpixEven[3]
			{IDE_DITHER_6BITS,IDE_DITHER_6BITS,IDE_DITHER_6BITS}//!< DitherBits[3]
		},

		(tPANEL_CMD*)tCmdModeRgbd320,				//!< pCmdQueue
		sizeof(tCmdModeRgbd320)/sizeof(tPANEL_CMD),	//!< nCmd
	}
	#else
	// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_RGB_SERIAL, 		//!< LCDMode
			27.00f, 							//!< fDCLK
			1491,								//!< uiHSyncTotalPeriod
			1440,								//!< uiHSyncActivePeriod
			43,//19, 								//!< uiHSyncBackPorch
			292,								//!< uiVSyncTotalPeriod
			272,								//!< uiVSyncActivePeriod
			12,//1,									//!< uiVSyncBackPorchOdd
			12,//1,									//!< uiVSyncBackPorchEven
			1440,								//!< uiBufferWidth
			272,								//!< uiBufferHeight
			1440,								//!< uiWindowWidth
			272,								//!< uiWindowHeight
			FALSE,								//!< bYCbCrFormat

			/* New added parameters */
			0x04,								//!< uiHSyncSYNCwidth
			0x04								//!< uiVSyncSYNCwidth
		},

		// tIDE_PARAM
		{
			/* Old prototype */
			PINMUX_LCD_SEL_SERIAL_RGB_8BITS|PINMUX_LCD_SEL_DE_ENABLE,	//!< pinmux_select_lcd;
			ICST_CCIR601,						//!< icst;
			{FALSE,FALSE},						//!< dithering[2];
			DISPLAY_DEVICE_CASIO2G, 			//!< **DONT-CARE**
			IDE_PDIR_RGB,						//!< pdir;
			IDE_LCD_R,							//!< odd;
			IDE_LCD_R,							//!< even;
			TRUE,								//!< hsinv;
			TRUE,								//!< vsinv;

			FALSE,								//!< hvldinv;
			FALSE,								//!< vvldinv;

			FALSE,								//!< clkinv;
			FALSE,								//!< fieldinv;

			FALSE,								//!< **DONT-CARE**
			FALSE,								//!< interlace;
			FALSE,								//!< **DONT-CARE**
			0x40,								//!< ctrst;
			0x00,								//!< brt;
			0x40,								//!< cmults;
			FALSE,								//!< cex;
			FALSE,								//!< **DONT-CARE**
			TRUE,								//!< **DONT-CARE**
			TRUE,								//!< tv_powerdown;
			{0x00,0x00},						//!< **DONT-CARE**

			/* New added parameters */
			FALSE,								//!< YCex
			FALSE,								//!< HLPF
			{TRUE,TRUE,TRUE},				//!< subpixOdd[3]
			{FALSE,FALSE,FALSE},				//!< subpixEven[3]
			{IDE_DITHER_6BITS,IDE_DITHER_6BITS,IDE_DITHER_6BITS}//!< DitherBits[3]
		},

		(tPANEL_CMD*)tCmdModeRgbd320,				//!< pCmdQueue
		sizeof(tCmdModeRgbd320)/sizeof(tPANEL_CMD), //!< nCmd
	}

	#endif

};

const tPANEL_CMD tCmdRotate_None[] = {
	//{IFCMD_CMD,0x36},	// Memory Access Control
	//{IFCMD_DATA,0x08},	// {IFCMD_DATA,0xc8},//48
	//{IFCMD_CMD,0x2c},	// memory write
};

const tPANEL_CMD tCmdRotate_180[] = {
	//{IFCMD_CMD,0x36},	// Memory Access Control
	//{IFCMD_DATA,0xc8},	// {IFCMD_DATA,0x08},//88
	//{IFCMD_CMD,0x2c},	// memory write
};

const tLCD_ROT tRotate[] = {
	{DISPDEV_LCD_ROTATE_NONE,(tPANEL_CMD*)tCmdRotate_None,3},
	{DISPDEV_LCD_ROTATE_180,(tPANEL_CMD*)tCmdRotate_180,3}
};
//@}

tLCD_ROT* dispdev_getLcdRotateCmd(UINT32 *ModeNumber)
{
#if 0
    if(tRotate != NULL) {
        *ModeNumber = sizeof(tRotate)/sizeof(tLCD_ROT);
    } else {
        *ModeNumber = 0;
    }
#else
    *ModeNumber = sizeof(tRotate)/sizeof(tLCD_ROT);
#endif
    return (tLCD_ROT*)tRotate;
}

tLCD_PARAM* dispdev_getConfigMode(UINT32 *ModeNumber)
{
    *ModeNumber = sizeof(tMode)/sizeof(tLCD_PARAM);
    return (tLCD_PARAM*)tMode;
}

tPANEL_CMD* dispdev_getStandbyCmd(UINT32 *CmdNumber)
{
    *CmdNumber = sizeof(tCmdStandby)/sizeof(tPANEL_CMD);
    return (tPANEL_CMD*)tCmdStandby;
}

#if 1
void dispdev_writeToLcdSif(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
    UINT32                  uiSIFData;
    DISPDEV_IOCTRL_PARAM    DevIOCtrl;

    uiSIFData = ((((UINT32)addr << 8)|((UINT32)value)) << 16);

    pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
    sif_send(DevIOCtrl.SEL.GET_REG_IF.uiSifCh, uiSIFData, 0, 0);
}

void dispdev_writeToLcdGpio(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
	//UINT32                  uiSIFData,j;
	UINT32                  j;
	UINT32                  SifClk, SifSen, SifData;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;

	pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
	SifClk  = DevIOCtrl.SEL.GET_REG_IF.uiGpioClk;
	SifSen  = DevIOCtrl.SEL.GET_REG_IF.uiGpioSen;
	SifData = DevIOCtrl.SEL.GET_REG_IF.uiGpioData;

	gpio_setDir(SifSen, GPIO_DIR_OUTPUT);
	gpio_setDir(SifClk, GPIO_DIR_OUTPUT);
	gpio_setDir(SifData, GPIO_DIR_OUTPUT);

	gpio_setPin(SifSen);
	gpio_setPin(SifData);
	gpio_setPin(SifClk);

	//uiSIFData = (UINT32)addr << 8 | (UINT32)value;

	Delay_DelayUs(500);

	gpio_clearPin(SifSen);

	if (addr == IFCMD_CMD) {
		value = (value & 0x00ff);
		for (j = 9; j > 0; j--) {
			if (((value >> (j - 1)) & 0x01)) {
				gpio_setPin(SifData);
			} else {
				gpio_clearPin(SifData);
			}

			Delay_DelayUs(100);
			gpio_clearPin(SifClk);
			Delay_DelayUs(200);
			gpio_setPin(SifClk);
			Delay_DelayUs(100);
		}
	} else if (addr == IFCMD_DATA) {
		value = ((value & 0x00ff) | 0x100);
		for (j = 9; j > 0; j--) {
			if (((value >> (j - 1)) & 0x01)) {
				gpio_setPin(SifData);
			} else {
				gpio_clearPin(SifData);
			}

			Delay_DelayUs(100);
			gpio_clearPin(SifClk);
			Delay_DelayUs(200);
			gpio_setPin(SifClk);
			Delay_DelayUs(100);
		}
	} else if (addr == CMDDELAY_MS) {
		Delay_DelayUs(value * 1000);
	} else if (addr == CMDDELAY_US) {
		Delay_DelayUs(value);
	}

	Delay_DelayUs(500);
	gpio_setPin(SifSen);
}

void dispdev_readFromLcdGpio(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32* p_value)
{
}
#endif


