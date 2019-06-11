#ifndef __UART_CMD_H__
#define __UART_CMD_H__

typedef struct t_dana_mcu_uart_cmd
{
    unsigned char m_head;           /* 0xAA */
    unsigned char m_version;        /* 0x10 -- 4 bit for main version, 4 bit for sub version, 0x10=v1.0*/
    unsigned char m_cmd;            /* see below */
    unsigned char m_param1;
    unsigned char m_param2;
    unsigned char m_param3;
    unsigned char m_sum;            /* m_head + m_version + m_cmd + m_param1 + m_param2 + m_param3 + m_tail */
    unsigned char m_tail;           /* 0x55 */
}MCU_CMD_FMT;

#define UART_CMD_HEAD               (0xAA)
#define UART_CMD_VER                (0x10)
#define UART_CMD_TAIL               (0x55)

// NVT -> MCU
#define UART_CMD_LED                (0x01)
#define UART_CMD_WIFI_FUNC_SWITCH   (0x02)
#define UART_CMD_VBAT_DETECT        (0x03)
#define UART_CMD_NVT_POWER_OFF      (0x04)
#define UART_CMD_POWER_ON_REASON    (0x05)
#define UART_CMD_PIR_SWITCH         (0x06)
#define UART_CMD_USER               (0x07)
#define UART_CMD_FW_VERSION         (0x0E)
#define UART_CMD_NVT_ACK            (0x0F)

// MCU -> NVT
#define UART_CMD_PIR_DETECTED       (0x10)
#define UART_CMD_WIFI_CONF_BT_PUSH  (0x20)
#define UART_CMD_RING_PUSH          (0x30)
#define UART_CMD_VBAT_DETECT_RSP    (0x40)
#define UART_CMD_POWER_ON_RESON_RSP (0x50)
#define UART_CMD_WAKE_UP_BY_PHONE   (0x60)
#define UART_CMD_FW_VERSION_RSP     (0xE0)
#define UART_CMD_MCU_ACK            (0xF0)

#define UART_LED_BLUE               (0x0)
#define UART_LED_RED                (0x1)
#define UART_LED_BOTH               (0x2)
#define UART_LED_GREEN              (0x3)

#define UART_PAR_OFF                (0x0)
#define UART_PAR_ON                 (0x1)
#define UART_PAR_BLK_F              (0x2)
#define UART_PAR_BLK_S              (0x3)
#define UART_PAR_BLK_B              (0x4)

#define UART_VBAT_OK                (0x0)
#define UART_VBAT_LOW               (0x1)
#define UART_VBAT_EMPTY             (0x2)

#define UART_REASON_PIR              (0x0)
#define UART_REASON_CONFIG           (0x1)
#define UART_REASON_RING             (0x2)
#define UART_REASON_WAKEUP           (0x3)
#define UART_REASON_BATTLOW          (0x4)


#endif

// NVT -> MCU
/* cmd 0x01: LED CMD
 * m_param1 = 0x01, RED LED, m_param1 = 0x00, BLUE LED,
 * m_param2 = 0x01, ON, m_param2 = 0x00, OFF,
 */

/* cmd 0x02: WIFI FUNC SWITCH CMD
 * m_param1 = 0x01, ON, m_param1 = 0x00, OFF,
 */

/* cmd 0x03: VBAT DETECT CMD
 */

/* cmd 0x04: NVT POWER OFF CMD
 */

/* cmd 0x05: NVT CHECK POWER ON REASON CMD
 */

/* cmd 0x06: ENABLE/DISABLE PIR CMD
 * m_param1 = 0x01 ENABLE PIR,
 * m_param1 = 0x00 DISABLE PIR,
 */

/* cmd 0x0F: MCU CMD OK CMD
 */

// MCU -> NVT
/* cmd 0x10: PIR CMD
 */

/* cmd 0x20: WIFI CONFIG MODE CMD
 * m_param1 = 0x01, BUTTON UP, m_param1 = 0x00, BUTTON DOWN,
 */

/* cmd 0x30: RING PUSH CMD
 */

/* cmd 0x40: VBAT DETECT RESULT CMD
 * m_param1 = 0x01, BATTARY LOW POWER, m_param1 = 0x00, BATTARY ENOUGH POWER,
 */

/* cmd 0x50: POWER ON REASON RESPOSE CMD
 * m_param1 = 0x01, PIR
 * m_param1 = 0x02, WIFI CONFIG BUTTON PUSH,
 * m_param1 = 0x03, RING BUTTON PUSH,
 * m_param1 = 0x04, WAKE UP BY PHONE
 * m_param2 = 0x01, FIRST TIME TO POWER ON NVT, 0x00, NOT FIRST TIME TO POWER ON NVT,
 */

/* cmd 0x60: UART_CMD_WAKE_UP_BY_PHONE
 */

/* cmd 0xF0: NVT CMD OK CMD
 */

