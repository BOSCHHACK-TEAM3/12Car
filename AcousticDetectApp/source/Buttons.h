/**
 * This software is copyrighted by Bosch Connected Devices and Solutions GmbH, 2016.
 * The use of this software is subject to the XDK SDK EULA
 */

/* header definition ******************************************************** */
#ifndef XDK110_LEDSANDBUTTONS_H_
#define XDK110_LEDSANDBUTTONS_H_

/* local interface declaration ********************************************** */

/* local type and macro definitions */
#define ZERO_VALUE             UINT32_C(0)        /**< Macro to represent zero value */
#define CALLBACK_PARAMETER_PB1 UINT32_C(0x11)     /**< Macro represents callback parameter for Button1 */
#define CALLBACK_PARAMETER_PB2 UINT32_C(0x12)     /**< Macro represents callback parameter for Button2 */

/** enum to represent return status */
typedef enum return_e
{
    FAILURE,
    SUCCESS,
} return_t;

/* local function prototype declarations */
return_t ButtonInit(void);
return_t ButtonDeinit(void);


/* local module global variable declarations */

/* local inline function definitions */

#endif /* XDK110_LEDSANDBUTTONS_H_ */

/** ************************************************************************* */
