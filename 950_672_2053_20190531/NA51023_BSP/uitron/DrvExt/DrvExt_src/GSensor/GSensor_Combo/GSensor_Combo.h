
#ifndef _GSENSOR_COMBO_H
#define _GSENSOR_COMBO_H

#include "GSensor.h"
typedef enum
{
    GSENSOR_NONE    = 0,
    GSENSOR_BMA253,
	GSENSOR_SC7A20,
	GSENSOR_SC7A30E,
	GSENSOR_MAX
}GSENSOR_MAKER;

typedef struct
{
     BOOL         (*open)(void);
     BOOL         (*close)(void);
     BOOL         (*GetStatus)(Gsensor_Data *GS_Data);
     BOOL         (*ParkingMode)(void);
     BOOL         (*CrashMode)(void);
     void         (*SetSensitivity)(GSENSOR_SENSITIVITY GSensorSensitivity);
     void         (*GetAxisValue)(INT32 *pX, INT32 *pY, INT32 *pZ);
     void         (*ClearIntSts)(void);
}GSENSOR_OBJ, *PGSENSOR_OBJ;

extern PGSENSOR_OBJ GSensor_SC7A30E_getGSensorObj(void);
extern PGSENSOR_OBJ GSensor_NONE_getGSensorObj(void);
extern PGSENSOR_OBJ GSensor_BMA253_getGSensorObj(void);
extern PGSENSOR_OBJ GSensor_SC7A20_getGSensorObj(void);

#endif

