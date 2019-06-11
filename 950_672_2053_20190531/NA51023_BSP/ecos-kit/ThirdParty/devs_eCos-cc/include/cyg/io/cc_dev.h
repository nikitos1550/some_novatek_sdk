#ifndef _CC_DEV_H
#define _CC_DEV_H

#include <cyg/infra/cyg_type.h>

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

#endif
