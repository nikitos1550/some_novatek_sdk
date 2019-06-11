#ifndef _SYS_SENSOR
#define _SYS_SENSOR

#include "Type.h"
#include "ImageUnit_VdoIn.h"
#include "ImageUnit_ImagePipe.h"

extern INT32     uiSensorMapping[VDOIN_MAX_NUM];

#define IPL_PATH_1                      SENSOR_MAP_1
#define IPL_PATH_2                      SENSOR_MAP_2


#define IPL_PATH(id) uiSensorMapping[id]

#define SENSOR_MAX_NUM					2 // max 2 sensors

extern void System_InstallSensorcmd(void);

extern ER   System_ChangeSensorName(SENSOR_ID id, CHAR *repalce_name);

#endif
