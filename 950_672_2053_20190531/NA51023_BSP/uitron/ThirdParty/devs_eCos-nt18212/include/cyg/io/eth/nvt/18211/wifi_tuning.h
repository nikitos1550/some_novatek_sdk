
#ifndef _WIFI_TUNING_H
#define _WIFI_TUNING_H

typedef enum _WIFI_TUNING_CMD_TYPE {
    WIFI_TUNING_SET = 0,
    WIFI_TUNING_GET,
    WIFI_TUNING_MAX
}WIFI_TUNING_CMD_TYPE;

typedef enum _WIFI_TUNING_SET_CMD {
    WIFI_TUNING_SET_INTERVAL = 0,
    WIFI_TUNING_SET_START,
    WIFI_TUNING_SET_CMD_MAX
}WIFI_TUNING_SET_CMD;

typedef enum _WIFI_TUNING_GET_CMD {
    WIFI_TUNING_GET_STATUS = 0,
    WIFI_TUNING_GET_CMD_MAX
}WIFI_TUNING_GET_CMD;

struct _wifi_tuning_cmd {
    WIFI_TUNING_CMD_TYPE type;
    union {
        WIFI_TUNING_SET_CMD set;
        WIFI_TUNING_GET_CMD get;
    } cmd_code;
    unsigned int buf_len;
    void *buf;
};
#endif
