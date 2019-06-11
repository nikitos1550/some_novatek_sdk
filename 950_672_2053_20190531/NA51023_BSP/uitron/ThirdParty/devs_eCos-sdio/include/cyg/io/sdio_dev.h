#ifndef _SDIO_DEV_H
#define _SDIO_DEV_H

#include <cyg/infra/cyg_type.h>
#include <errno.h>

/**
    @name SDIO IO RW flag definition.

    SDIO IO Read Write flag.

    SDIO IO Read Write flag.

    @note for ISSUE_CMD52, ISSUE_CMD53
*/
//@{
#define SDIODEV_IO_R                           0x0         ///< IO Read flag
#define SDIODEV_IO_W                           0x1         ///< IO Write flag
//@}

/**
    @name SDIO IO RAW (Read After Write) definition.

    @note for ISSUE_CMD52
*/
//@{
#define SDIODEV_IO_DIS_RAW                     0x0         ///< Disable IO RAW function
#define SDIODEV_IO_EN_RAW                      0x1         ///< Enable IO RAW function
//@}

/**
    @name SDIO IO byte/block access definition.

    @note for ISSUE_CMD53
*/
//@{
#define SDIODEV_IO_BYTEMODE                    0x0         ///< IO byte access mode
#define SDIODEV_IO_BLOCKMODE                   0x1         ///< IO block access mode
//@}

/**
    @name SDIO IO OP code definition.

    @note for ISSUE_CMD53
*/
//@{
#define SDIODEV_IO_OPCODE_FIX_ADDR             0           ///< OP code: multi byte R/W to fixed address
#define SDIODEV_IO_OPCODE_INCREMENT_ADDR       1           ///< OP code: multi byte R/W to incrementing address
//@}

/**
    @name SDIO IO function select definition.

    @note for ISSUE_CMD52, ISSUE_CMD53
*/
//@{
#define SDIODEV_IO_FUNNUM0                     0           ///< Function 0 on IO device
#define SDIODEV_IO_FUNNUM1                     1           ///< Function 1 on IO device
#define SDIODEV_IO_FUNNUM2                     2           ///< Function 2 on IO device
#define SDIODEV_IO_FUNNUM3                     3           ///< Function 3 on IO device
#define SDIODEV_IO_FUNNUM4                     4           ///< Function 4 on IO device
#define SDIODEV_IO_FUNNUM5                     5           ///< Function 5 on IO device
#define SDIODEV_IO_FUNNUM6                     6           ///< Function 6 on IO device
#define SDIODEV_IO_FUNNUM7                     7           ///< Function 7 on IO device
//@}

/**
   @name SDIO IO set config operation

   @note for sdio_setConfig
*/
//@}
typedef enum
{
    SDIODEV_SETOP_ISSUECMD52,    ///< issue cmd52
    SDIODEV_SETOP_ISSUECMD53,    ///< issue cmd53
    SDIODEV_SETOP_ENIOINT,       ///< enable io interrupt
    SDIODEV_SETOP_IOINTCB,       ///< hook io interrupt callback
    SDIODEV_SETOP_WAITIOINT,     ///< wait io interrupt
    SDIODEV_SETOP_AUTODISIOINT,  ///< auto clear host io interrupt when receive io interrupt
    SDIODEV_SETOP_BLOCKSIZE,     ///< set host block size


    SDIODEV_GETOP_BLOCKSIZE,     ///< get host block size
    SDIODEV_GETOP_FUNC,          ///< get Func
}SDIODEV_SETCONFIGOP;
//@}

/**
    @name SDIO general call back prototype

    @param[in] bool

    @return bool
*/
//@{
typedef bool (*SDIODEV_CALLBACK_HDL)(bool);
//@}


/**
    @name SDIO source clock definition
*/
//@{
#define SDIODEV_SRC_CLK_20MHZ      0   ///< No use
#define SDIODEV_SRC_CLK_24MHZ      1   ///< No use
#define SDIODEV_SRC_CLK_40MHZ      2   ///< Source clock 40Mhz, only for SDIO2
#define SDIODEV_SRC_CLK_48MHZ      3   ///< Source clock 48Mhz, SDIO/SDIO2 support
#define SDIODEV_SRC_CLK_60MHZ      4   ///< No use
#define SDIODEV_SRC_CLK_80MHZ      5   ///< Source clock 80Mhz, only for SDIO
#define SDIODEV_SRC_CLK_SSPLLMHZ   6   ///< No use
#define SDIODEV_SRC_CLK_SSPLL2MHZ  7   ///< No use
#define SDIODEV_SRC_CLK_SSPLL3MHZ  8   ///< No use
#define SDIODEV_SRC_CLK_SSPLL4MHZ  9   ///< No use
#define SDIODEV_SRC_CLK_SSPLL5MHZ 10   ///< No use
#define SDIODEV_SRC_CLK_96MHZ     11   ///< Source clock 96Mhz, only for SDIO
#define SDIODEV_SRC_CLK_PLL4      12   ///< Source clock PLL4, SDIO/SDIO2 support
//@}

/**
    @name SDIO speed definition

*/
//@{
#define SDIODEV_BUS_SPEED_MODE_DS  0x01 ///< Default Speed
#define SDIODEV_BUS_SPEED_MODE_HS  0x02 ///< High Seed
//@}


/**
    SDIO configuration identifier

    @note For sdio_setConfigNolookup()
*/
typedef enum
{
    SDIODEV_CONFIG_ID_SRCLK,                ///< Set source clock for SDIO host
                                            ///< Context can be any of:
                                            ///< - @b SDIODEV_SRC_CLK_40MHZ (only SDIO2 support)
                                            ///< - @b SDIODEV_SRC_CLK_48MHZ (Defalut value)
                                            ///< - @b SDIODEV_SRC_CLK_80MHZ (only SDIO support)
                                            ///< - @b SDIODEV_SRC_CLK_96MHZ (only SDIO support)
                                            ///< - @b SDIODEV_SRC_CLK_PLL4
    SDIODEV_CONFIG_ID_BUS_SPEED_MODE,       ///< Set Bus seepd mode for SDIO host
                                            ///< Context can be ORed of:
                                            ///< - @b SDIODEV_BUS_SPEED_MODE_DS (Defalut value)
                                            ///< - @b SDIODEV_BUS_SPEED_MODE_HS (Defalut value)
    SDIODEV_CONFIG_ID_DS_MAX_CLK,           ///< Set maximum clock frequency in Default Speed Mode
                                            ///< Valid value: 1~25000000, unit: Hz
                                            ///< Valid when SDIO_CONFIG_ID_BUS_SPEED_MODE has SDIO_BUS_SPEED_MODE_DS
    SDIODEV_CONFIG_ID_HS_MAX_CLK,           ///< Set maximum clock frequency in High Speed Mode
                                            ///< Valid value: 1~50000000, unit: Hz
                                            ///< Valid when SDIO_CONFIG_ID_BUS_SPEED_MODE has SDIO_BUS_SPEED_MODE_HS
    SDIODEV_CONFIG_ID_CMD0_RST_EN,          ///< Set CMD0 reset enable or disable
    SDIODEV_CONFIG_ID_STR_FUNC,             ///< Set function number that the re-initial skip cmd0,cmd3,cmd5....
    SDIODEV_SET_INFO_VER = 0x00010001,      ///< START Tag for sdio_setConfigNolookup()
    SDIODEV_INFOR_END_TAG = 0x444E4549,     ///< END Tag for sdio_setInfo() ('I''E''N''D')
}SDIODEV_CONFIG_ID;

typedef struct
{
    union
    {
        struct
        {
            unsigned long uiReg_Addr;      ///< register address in SDIO IO device
            unsigned char uiRaw;           ///< Read After Write flag
                                           ///< - @b SDIODEV_IO_DIS_RAW: disable RAW function
                                           ///< - @b SDIODEV_IO_EN_RAW: enable RAW function
            unsigned char uiFun_Num;       ///< function number in SDIO IO device
                                           ///< - @b SDIODEV_IO_FUNNUM0: function 0 in SDIO IO device
                                           ///< - @b SDIODEV_IO_FUNNUM1: function 1 in SDIO IO device
                                           ///< - @b SDIODEV_IO_FUNNUM2: function 2 in SDIO IO device
                                           ///< - @b SDIODEV_IO_FUNNUM3: function 3 in SDIO IO device
                                           ///< - @b SDIODEV_IO_FUNNUM4: function 4 in SDIO IO device
                                           ///< - @b SDIODEV_IO_FUNNUM5: function 5 in SDIO IO device
                                           ///< - @b SDIODEV_IO_FUNNUM6: function 6 in SDIO IO device
                                           ///< - @b SDIODEV_IO_FUNNUM7: function 7 in SDIO IO device
            unsigned char uiRw_Flag;       ///< R/W flag
                                           ///< - @b SDIODEV_IO_R: IO read
                                           ///< - @b SDIODEV_IO_W: IO write
            unsigned int  uiByte_Cnt;      ///< Transfer byte count
            unsigned int  uiStuff;         ///< Write Data/Stuff bits
            unsigned char *pData;          ///< pointer to read/write buffer address
        }ISSUE_CMD52;

        struct
        {
            unsigned long uiReg_Addr;      ///< register address in SDIO IO device
            unsigned char uiOp_Code;       ///< IO OP code:
                                           ///< - @b SDIODEV_IO_OPCODE_FIX_ADDR: R/W to fixed address
                                           ///< - @b SDIODEV_IO_OPCODE_INCREMENT_ADDR: R/W to incrementing address
            unsigned char uiBlock;         ///< Block/Byte IO mode
                                           ///< - @b SDIODEV_IO_BYTEMODE: byte access mode
                                           ///< - @b SDIODEV_IO_BLOCKMODE: block access mode
            unsigned char uiFun_Num;       ///< function number in SDIO IO device
                                           ///< - @b SDIODEV_IO_FUNNUM0: function 0 in SDIO IO device
                                           ///< - @b SDIODEV_IO_FUNNUM1: function 1 in SDIO IO device
                                           ///< - @b SDIODEV_IO_FUNNUM2: function 2 in SDIO IO device
                                           ///< - @b SDIODEV_IO_FUNNUM3: function 3 in SDIO IO device
                                           ///< - @b SDIODEV_IO_FUNNUM4: function 4 in SDIO IO device
                                           ///< - @b SDIODEV_IO_FUNNUM5: function 5 in SDIO IO device
                                           ///< - @b SDIODEV_IO_FUNNUM6: function 6 in SDIO IO device
                                           ///< - @b SDIODEV_IO_FUNNUM7: function 7 in SDIO IO device
            unsigned char uiRw_Flag;       ///< R/W flag
                                           ///< - @b SDIODEV_IO_R: IO read
                                           ///< - @b SDIODEV_IO_W: IO write
            unsigned int  uiByte_Cnt;      ///< Transfer byte count
            unsigned int  uiStuff;         ///< Write Data/Stuff bits
            unsigned char *pData;          ///< pointer to read/write buffer address
        }ISSUE_CMD53;

        struct
        {
            bool bEn;                      ///< enable/disable host receive io interrupt
                                           ///< - @b true: enable host receive io interrupt
                                           ///< - @b false: disable host receive io interrupt
        }EN_IOINT;

        struct
        {
            SDIODEV_CALLBACK_HDL fpioint;  ///< hook io interrupt callback
                                           ///< The callback function of io interrupt

        }IOINTCB;


        struct
        {
            bool bBlocking;                ///< wait io interrupt
                                           ///< don't care bBlocking is true or false, it always do blocking.
        }WAIT_IOINT;

        struct
        {
            bool bAutoDis;                 ///< auto disable host io interrupt when receive io interrupt
        }SET_AUTODISIOINT;

        struct
        {
            unsigned long uiblocksize;    ///< block size, unit: byte
        }SET_BLOCK_SIZE;

        struct
        {
            unsigned long uiblocksize;    ///< block size, unit: byte
        }GET_BLOCK_SIZE;

        struct
        {
            unsigned long uifunc;          ///< ocr value
        }GET_FUNC;

    } SEL;

}SDIOSETCONFIG_PARAM,*PSDIOSETCONFIG_PARAM;


extern Cyg_ErrNo sdio_setConfigNolookup(SDIODEV_CONFIG_ID configID, unsigned long configContext);

#endif
