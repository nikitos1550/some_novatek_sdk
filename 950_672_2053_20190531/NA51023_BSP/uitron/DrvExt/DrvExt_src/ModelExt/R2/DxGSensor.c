#include "Debug.h"
#include "i2c.h"
#include "DxGSensor.h"

PI2C_OBJ DrvGSensor_GetI2cObj(void)
{
	return i2c_getDrvObject(I2C_ID_I2C3);
}

