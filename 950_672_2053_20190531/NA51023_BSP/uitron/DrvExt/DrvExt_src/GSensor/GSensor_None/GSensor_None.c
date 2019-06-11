#include "GSensor.h"

#define __MODULE__          GSensor_None
#define __DBGLVL__          1      // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*"    //*=All, [mark]=CustomClass
#include "DebugModule.h"


BOOL GSensor_open(void)
{
    DBG_IND("\r\n");
    return TRUE;
}

BOOL GSensor_close(void)
{
    return TRUE;
}



BOOL GSensor_GetStatus(Gsensor_Data *GS_Data)
{
    DBG_IND("\r\n");
    return FALSE;
}

BOOL GSensor_ParkingMode(void)
{
    return FALSE;
}

BOOL GSensor_CrashMode(void)
{
    return FALSE;
}

void GSensor_SetSensitivity(GSENSOR_SENSITIVITY GSensorSensitivity)
{

}

void GSensor_GetAxisValue(INT32 *pX, INT32 *pY, INT32 *pZ)
{
}
