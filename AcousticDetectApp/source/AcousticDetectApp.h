/**
 * This software is copyrighted by Bosch Connected Devices and Solutions GmbH, 2016.
 * The use of this software is subject to the XDK SDK EULA
 */

/* header definition ******************************************************** */
#ifndef XDK110_ACOUSTICDETECTAPP_H_
#define XDK110_ACOUSTICDETECTAPP_H_

/* local interface declaration ********************************************** */

/* local type and macro definitions */
#define TIMER_AUTORELOAD_ON pdTRUE

#define ONESECONDDELAY      UINT32_C(20)       /* one second is represented by this macro */
#define TIMERBLOCKTIME      UINT32_C(0xffff)     /* Macro used to define blocktime of a timer */
#define TIMEBASE            UINT32_C(0)          /* Macro used to define the Timebase for the ADC */

void initAcousticModule(void);


#endif /* XDK110_ACOUSTICDETECTAPP_H_ */

/** ************************************************************************* */
