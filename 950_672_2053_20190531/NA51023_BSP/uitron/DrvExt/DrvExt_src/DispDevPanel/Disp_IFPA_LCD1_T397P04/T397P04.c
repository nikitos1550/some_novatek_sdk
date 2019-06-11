/*
    Display object object panel configuration parameters for T397P04

    @file       T397P04.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "kernel.h"
#include "Debug.h"
#include "display.h"
#include "dispdev_IFParal.h"
#include "dispdev_panlcomn.h"

#define IFCMD_CMD		0x01000000
#define IFCMD_DATA		0x02000000

#define Delayms(value)	{CMDDELAY_MS, value}
#define wr_cmd(value)	{IFCMD_CMD, value}
#define wr_dat(value)	{IFCMD_DATA, value}

#define PANEL_WIDTH     480
#define PANEL_HEIGHT    800

#define HSYNCT      6 //7// 1//2//5
#define VSYNCT      5 //15// 1//2//5


/*
    Panel Parameters for T30P105
*/
const tPANEL_CMD tCmdStandby[] =
{
  //  {0x2B, 0x00}
};


const tPANEL_CMD tCmdModeRgbd320[] =
{
    wr_cmd(0x11),
    Delayms (120), //Delay 120ms

    wr_cmd(0xFF),
    wr_dat(0x77),
    wr_dat(0x01),
    wr_dat(0x00),
    wr_dat(0x00),
    wr_dat(0x10),

    wr_cmd(0xC0),
    wr_dat(0x63),
    wr_dat(0x00),

    wr_cmd(0xC1),
    wr_dat(0x11),
    wr_dat(0x02),

    wr_cmd(0xC2),
    wr_dat(0x31),
    wr_dat(0x08),

    wr_cmd(0xCC),
    wr_dat(0x10),
    //-------------------------------------Gamma Cluster Setting-------------------------------------------//
    wr_cmd(0xB0),
    wr_dat(0x40),
    wr_dat(0x01),
    wr_dat(0x46),
    wr_dat(0x0D),
    wr_dat(0x13),
    wr_dat(0x09),
    wr_dat(0x05),
    wr_dat(0x09),
    wr_dat(0x09),
    wr_dat(0x1B),
    wr_dat(0x07),
    wr_dat(0x15),
    wr_dat(0x12),
    wr_dat(0x4C),
    wr_dat(0x10),
    wr_dat(0xC8),

    wr_cmd(0xB1),
    wr_dat(0x40),
    wr_dat(0x02),
    wr_dat(0x86),
    wr_dat(0x0D),
    wr_dat(0x13),
    wr_dat(0x09),
    wr_dat(0x05),
    wr_dat(0x09),
    wr_dat(0x09),
    wr_dat(0x1F),
    wr_dat(0x07),
    wr_dat(0x15),
    wr_dat(0x12),
    wr_dat(0x15),
    wr_dat(0x19),
    wr_dat(0x08),

    wr_cmd(0xFF),
    wr_dat(0x77),
    wr_dat(0x01),
    wr_dat(0x00),
    wr_dat(0x00),
    wr_dat(0x11),

    wr_cmd(0xB0),
    wr_dat(0x50),
    //-------------------------------------------Vcom Setting---------------------------------------------------//
    wr_cmd(0xB1),
    wr_dat(0x68),
    //-----------------------------------------End Vcom Setting-----------------------------------------------//
    wr_cmd(0xB2),
    wr_dat(0x07),

    wr_cmd(0xB3),
    wr_dat(0x80),

    wr_cmd(0xB5),
    wr_dat(0x47),

    wr_cmd(0xB7),
    wr_dat(0x85),

    wr_cmd(0xB8),
    wr_dat(0x21),

    wr_cmd(0xB9),
    wr_dat(0x10),

    wr_cmd(0xC1),
    wr_dat(0x78),

    wr_cmd(0xC2),
    wr_dat(0x78),

    wr_cmd(0xD0),
    wr_dat(0x88),
    //---------------------------------End Power Control Registers Initial -------------------------------//
    Delayms (100),
    //---------------------------------------------GIP Setting----------------------------------------------------//
    wr_cmd(0xE0),
    wr_dat(0x00),
    wr_dat(0x00),
    wr_dat(0x02),

    wr_cmd(0xE1),
    wr_dat(0x08),
    wr_dat(0x00),
    wr_dat(0x0A),
    wr_dat(0x00),
    wr_dat(0x07),
    wr_dat(0x00),
    wr_dat(0x09),
    wr_dat(0x00),
    wr_dat(0x00),
    wr_dat(0x33),
    wr_dat(0x33),

    wr_cmd(0xE2),
    wr_dat(0x00),
    wr_dat(0x00),
    wr_dat(0x00),
    wr_dat(0x00),
    wr_dat(0x00),
    wr_dat(0x00),
    wr_dat(0x00),
    wr_dat(0x00),
    wr_dat(0x00),
    wr_dat(0x00),
    wr_dat(0x00),
    wr_dat(0x00),
    wr_dat(0x00),

    wr_cmd(0xE3),
    wr_dat(0x00),
    wr_dat(0x00),
    wr_dat(0x33),
    wr_dat(0x33),

    wr_cmd(0xE4),
    wr_dat(0x44),
    wr_dat(0x44),

    wr_cmd(0xE5),
    wr_dat(0x0E),
    wr_dat(0x2D),
    wr_dat(0xA0),
    wr_dat(0xA0),
    wr_dat(0x10),
    wr_dat(0x2D),
    wr_dat(0xA0),
    wr_dat(0xA0),
    wr_dat(0x0A),
    wr_dat(0x2D),
    wr_dat(0xA0),
    wr_dat(0xA0),
    wr_dat(0x0C),
    wr_dat(0x2D),
    wr_dat(0xA0),
    wr_dat(0xA0),

    wr_cmd(0xE6),
    wr_dat(0x00),
    wr_dat(0x00),
    wr_dat(0x33),
    wr_dat(0x33),

    wr_cmd(0xE7),
    wr_dat(0x44),
    wr_dat(0x44),

    wr_cmd(0xE8),
    wr_dat(0x0D),
    wr_dat(0x2D),
    wr_dat(0xA0),
    wr_dat(0xA0),
    wr_dat(0x0F),
    wr_dat(0x2D),
    wr_dat(0xA0),
    wr_dat(0xA0),
    wr_dat(0x09),
    wr_dat(0x2D),
    wr_dat(0xA0),
    wr_dat(0xA0),
    wr_dat(0x0B),
    wr_dat(0x2D),
    wr_dat(0xA0),
    wr_dat(0xA0),

    wr_cmd(0xEB),
    wr_dat(0x02),
    wr_dat(0x01),
    wr_dat(0xE4),
    wr_dat(0xE4),
    wr_dat(0x44),
    wr_dat(0x00),
    wr_dat(0x40),
    wr_cmd(0xEC),
    wr_dat(0x02),
    wr_dat(0x01),

    wr_cmd(0xED),
    wr_dat(0xAB),
    wr_dat(0x89),
    wr_dat(0x76),
    wr_dat(0x54),
    wr_dat(0x01),
    wr_dat(0xFF),
    wr_dat(0xFF),
    wr_dat(0xFF),
    wr_dat(0xFF),
    wr_dat(0xFF),
    wr_dat(0xFF),
    wr_dat(0x10),
    wr_dat(0x45),
    wr_dat(0x67),
    wr_dat(0x98),
    wr_dat(0xBA),
    //--------------------------------------------End GIP Setting-----------------------------------------------//
    //------------------------------ Power Control Registers Initial End-----------------------------------//
    //------------------------------------------Bank1 Setting----------------------------------------------------//
    wr_cmd(0xFF),
    wr_dat(0x77),
    wr_dat(0x01),
    wr_dat(0x00),
    wr_dat(0x00),
    wr_dat(0x00),

    wr_cmd(0x3A),
    wr_dat(0x55),


    //SSD2828_WritePackageSize(2);
    wr_cmd(0x36),
    wr_dat(0x00),

    wr_cmd(0x29),
};


const tLCD_PARAM tMode[] =
{
    /***********       RGB Dummy-320 MODE         *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            PINMUX_LCDMODE_RGB_PARALL565,			//!< LCDMode
            27.0f,									//!< fDCLK 24.54f,
            40 + 480,								//!< uiHSyncTotalPeriod
            480,									//!< uiHSyncActivePeriod
            6,										//!< uiHSyncBackPorch
            44 + 800,								//!< uiVSyncTotalPeriod
            800,									//!< uiVSyncActivePeriod
            15,										//!< uiVSyncBackPorchOdd  29 15
            15,										//!< uiVSyncBackPorchEven 29.5 15
            480,									//!< uiBufferWidth
            800,									//!< uiWindowHeight
            480,									//!< uiWindowWidth
            800,									//!< uiWindowHeight
            FALSE,									//!< bYCbCrFormat

            /* New added parameters */
            HSYNCT,                           //!< uiHSyncSYNCwidth
            VSYNCT,                           //!< uiVSyncSYNCwidth
        },

        // tIDE_PARAM
        {
            /* Old prototype */
            PINMUX_LCD_SEL_PARALLE_RGB565|PINMUX_LCD_SEL_DE_ENABLE,//PINMUX_LCD_SEL_SERIAL_RGB_8BITS,//!< pinmux_select_lcd;
            ICST_CCIR601,                   //!< icst;
            {FALSE,FALSE},                  //!< dithering[2];
            DISPLAY_DEVICE_AU,              //!< **DONT-CARE**
            IDE_PDIR_RGB,                   //!< pdir;
            IDE_LCD_G,                      //!< odd;
            IDE_LCD_G,                      //!< even;
            TRUE,                           //!< hsinv;
            TRUE,                           //!< vsinv;
            FALSE,                          //!< hvldinv;
            FALSE,                          //!< vvldinv;
            TRUE,                           //!< clkinv;
            FALSE,                          //!< fieldinv;
            FALSE,                          //!< **DONT-CARE**
            FALSE,                          //!< interlace;
            FALSE,                          //!< **DONT-CARE**
            0x40,                           //!< ctrst;
            0x00,                           //!< brt;
            0x58,                           //!< cmults;
            FALSE,                          //!< cex;
            FALSE,                          //!< **DONT-CARE**
            TRUE,                           //!< **DONT-CARE**
            TRUE,                           //!< tv_powerdown;
            {0x00,0x00},                    //!< **DONT-CARE**

            /* New added parameters */
            FALSE,                          //!< YCex
            FALSE,                          //!< HLPF
            {FALSE,FALSE,FALSE},            //!< subpixOdd[3]
            {FALSE,FALSE,FALSE},            //!< subpixEven[3]
            {IDE_DITHER_6BITS,IDE_DITHER_6BITS,IDE_DITHER_6BITS}//!< DitherBits[3]
        },

        (tPANEL_CMD*)tCmdModeRgbd320,                  //!< pCmdQueue
        sizeof(tCmdModeRgbd320)/sizeof(tPANEL_CMD),    //!< nCmd
    }
};
const tPANEL_CMD tCmdRotate_None[] =
{
    //{0x04,0x1b}
    {0x0B,0x00},
};
const tPANEL_CMD tCmdRotate_180[] =
{
    //{0x04,0x1B}
    {0x0B,0x03},

};
const tLCD_ROT tRotate[] =
{
    {DISPDEV_LCD_ROTATE_NONE,(tPANEL_CMD*)tCmdRotate_None,1},
    {DISPDEV_LCD_ROTATE_180,(tPANEL_CMD*)tCmdRotate_180,1}
};
//@}

tLCD_ROT* dispdev_getLcdRotateCmdParal(UINT32 *ModeNumber)
{
    if(tRotate != NULL)
    {
        *ModeNumber = sizeof(tRotate)/sizeof(tLCD_ROT);
    }
    else
    {
        *ModeNumber = 0;
    }
    return (tLCD_ROT*)tRotate;
}

tLCD_PARAM* dispdev_getConfigModeParal(UINT32 *ModeNumber)
{
    *ModeNumber = sizeof(tMode)/sizeof(tLCD_PARAM);
    return (tLCD_PARAM*)tMode;
}

tPANEL_CMD* dispdev_getStandbyCmdParal(UINT32 *CmdNumber)
{
    *CmdNumber = sizeof(tCmdStandby)/sizeof(tPANEL_CMD);
    return (tPANEL_CMD*)tCmdStandby;
}

#if 1
void dispdev_writeToLcdSifParal(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
    UINT32                  uiSIFData;
    DISPDEV_IOCTRL_PARAM    DevIOCtrl;

    uiSIFData = ((((UINT32)addr << 8)|((UINT32)value)) << 16);

    pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
    sif_send(DevIOCtrl.SEL.GET_REG_IF.uiSifCh, uiSIFData, 0, 0);
}

void dispdev_writeToLcdGpioParal(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
    //UINT32                  uiSIFData,j,i;
    UINT32                  j;
    UINT32                  SifClk,SifSen,SifData;
    DISPDEV_IOCTRL_PARAM    DevIOCtrl;

	//DBG_DUMP("^C dispdev_writeToLcdGpioParal!\r\n");
    pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
    SifClk  = DevIOCtrl.SEL.GET_REG_IF.uiGpioClk;
    SifSen  = DevIOCtrl.SEL.GET_REG_IF.uiGpioSen;
    SifData = DevIOCtrl.SEL.GET_REG_IF.uiGpioData;
	//DBG_DUMP("^C SifClk %d, SifSen %d, SifData %d\r\n", SifClk, SifSen, SifData);

    gpio_setDir(SifSen, GPIO_DIR_OUTPUT);
    gpio_setDir(SifClk, GPIO_DIR_OUTPUT);
    gpio_setDir(SifData, GPIO_DIR_OUTPUT);

    gpio_setPin(SifSen);
    gpio_setPin(SifData);
    gpio_setPin(SifClk);

    //uiSIFData = (UINT32)addr << 8 | (UINT32)value;
    //debug_msg("Has not implement dispdev_readFromLcdGpio%x %x  %x\r\n",uiSIFData,addr,value);
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
	}

    Delay_DelayUs(500);
    gpio_setPin(SifSen);

}


void dispdev_readFromLcdGpioParal(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32* p_value)
{
    //debug_err(("Has not implement dispdev_readFromLcdGpio \r\n"));
}
#endif
