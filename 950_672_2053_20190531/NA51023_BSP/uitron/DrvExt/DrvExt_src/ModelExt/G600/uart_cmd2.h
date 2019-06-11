#ifndef __UART_CMD2_H__
#define __UART_CMD2_H__

typedef struct _MCU2_CMD_FMT
{
    unsigned char m_head;           /* 0xAA */
    unsigned char m_version;        /* 0x10 -- 4 bit for main version, 4 bit for sub version, 0x10=v1.0*/
    unsigned char m_cmd;            /* see below */
    unsigned char m_param1;
    unsigned char m_param2;
    unsigned char m_param3;
    unsigned char m_sum;            /* m_head + m_version + m_cmd + m_param1 + m_param2 + m_param3 + m_tail */
    unsigned char m_tail;           /* 0x55 */
}MCU2_CMD_FMT;

#define UART_SND_OFFSET             2
#define UART_SND_CMDHEAD            (0x01)
#define UART_SND_CMDTAIL            (0x03)
#define UART_SND_DATAHEAD           (0x02)
#define UART_SND_DATANEXT           (0x17)
#define UART_SND_DATATAIL           (0x03)

// NVT -> MCU
#define UART_CMD_RESET              (0x01)
#define UART_CMD_BAUDRATESET        (0x9A)
#define UART_CMD_BLOCKERASE         (0x22)
#define UART_CMD_PROGRAMMING        (0x40)
#define UART_CMD_VERIFY             (0x13)
#define UART_CMD_BLOCKBLANKCHECK    (0x32)
#define UART_CMD_SILICONSIGNATURE   (0xC0)
#define UART_CMD_CHECKSUM           (0xB0)
#define UART_CMD_SECURITYSET        (0xA0)
#define UART_CMD_SECURITYGET        (0xA1)
#define UART_CMD_SECURITYRELEASE    (0xA2)

//UART_CMD_BAUDRATESET
//p1
#define UART_PAR_BAUDRATESET_115200     0x00
#define UART_PAR_BAUDRATESET_250000     0x01
#define UART_PAR_BAUDRATESET_500000     0x02
#define UART_PAR_BAUDRATESET_1000000    0x03
//p2
#define UART_PAR_BAUDRATESET_V369       0x24
#define UART_PAR_BAUDRATESET_V211       0x15

//UART_CMD_BLOCKBLANKCHECK
//p3
#define UART_PAR_CHECKONE           (0x00)
#define UART_PAR_CHECKALL           (0x01)


#define UART_RET_CMDERR             (0x04)
#define UART_RET_PARAMRR            (0x05)
#define UART_RET_ACK                (0x06)
#define UART_RET_CHECKSUMERR        (0x07)
#define UART_RET_VERIFYERR          (0x0F)
#define UART_RET_PROTECTERR         (0x10)
#define UART_RET_NACK               (0x15)
#define UART_RET_ERASEERR           (0x1A)
#define UART_RET_INTERNALERR        (0x1B)
#define UART_RET_WRITEERR           (0x1C)

#define UART_RCV_OFFSET             2
#define UART_RCV_STSHEAD            (0x02)
#define UART_RCV_STSTAIL            (0x03)

// MCU -> NVT
//#define UART_STS_RESULT             (0x10)

#endif

