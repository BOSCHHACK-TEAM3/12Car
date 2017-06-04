/**
 * This software is copyrighted by Bosch Connected Devices and Solutions GmbH, 2016.
 * The use of this software is subject to the XDK SDK EULA
 */

/* module includes ********************************************************** */

/* system header files */
#include "AcousticDetectApp.h"
#include "SendDataOverUdp.h"
#include "EnvironmentalSensor.h"
#include "Magnetometer.h"
#include "LightSensor.h"
#include "Accelerometer.h"
#include "Gyroscope.h"
#include "Gyroscope.h"
#include "Buttons.h"

#include "stdio.h"
#include "FreeRTOS.h"
#include "timers.h"




/**
 * @brief This is a template function where the user can write his custom application.
 *
 */
void deinit(void) {
//efan does not require to enable all sensors
	lightsensorDeinit();
	magnetometerSensorDeinit();
	environmentalSensorDeinit();
	accelerometerSensorDeinit();
	gyroscopeSensorDeinit();
	ButtonDeinit();
}

void appInitSystem(xTimerHandle xTimer) {
	(void) (xTimer);

 	initUDP();

	lightsensorInit();
	magnetometerSensorInit();
	environmentalSensorInit();
	initAcousticModule();
	accelerometerSensorInit();
	gyroscopeSensorInit();
	ButtonInit();




}
/** ************************************************************************* */
