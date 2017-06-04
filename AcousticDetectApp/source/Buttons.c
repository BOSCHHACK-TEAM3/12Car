/**
 * This software is copyrighted by Bosch Connected Devices and Solutions GmbH, 2016.
 * The use of this software is subject to the XDK SDK EULA
 */

/* module includes ********************************************************** */
//lint -esym(956,*) /* Suppressing "Non const, non volatile static or external variable" lint warning*/

/* system header files */
#include "BCDS_Basics.h"
#include <stdio.h>
#include <stdint.h>

/* additional interface header files */
#include <FreeRTOS.h>
#include <timers.h>
#include "button.h"
#include "XdkBoardHandle.h"
#include "XDKManagement.h"

/* own header files */
#include "Buttons.h"

/* constant definitions ***************************************************** */

/* local variables ********************************************************** */
static BUTTON_handle_tp Button1Handle = (BUTTON_handle_tp) NULL; /**< variable to store button 1 handle */
static BUTTON_handle_tp Button2Handle = (BUTTON_handle_tp) NULL; /**< variable to store button 2 handle */
/* global variables ********************************************************* */

/* inline functions ********************************************************* */

/* local functions ********************************************************** */

/**
 * @brief This will create the handles for button
 *
 * @retval FAILURE Button Handle is not created
 * @retval SUCCESS Button Handle is created
 */
static return_t createButton(void);

/**
 * @brief This will enable the  button
 *
 * @retval FAILURE Button Handle is not enabled
 * @retval SUCCESS Button Handle is enabled
 */
static return_t enableButton(void);

/**
 * @brief This will set the callback for the button
 *
 * @retval FAILURE Callback set failed
 * @retval SUCCESS Callback set successfully
 */
static return_t setButtonCallback(void);
/**
 * @brief     this callback will get triggered whenever any one of button is pressed
 *
 * @param[in] handle handle of the push button
 *
 * @param[in] userParameter parameter of the corresponding handle
 */
static void callback(void *handle, uint32_t userParameter);


/**
 * @brief     this callback will get triggered whenever any one of button is pressed
 * @param[in] handle handle of the push button
 * @param[in] userParameter parameter of the corresponding handle
 */
static void callback(void *handle, uint32_t userParameter)
{
    switch (userParameter)
    {
    /*  Button 1 press/release */
    case CALLBACK_PARAMETER_PB1:
        if (BUTTON_isPressed(handle))
        {

        	printf("BUTTON 1 is pressed\n\r");

        }
        if (BUTTON_isReleased(handle))
        {
        	requestOneTimeDataSend(BUTTON1RELEASED);
        	xspeed=0;yspeed=0;distance_2=0;crash=0;
        	printf("BUTTON 1 is released\n\r");
        }
        break;

        /* Button 2 press/release */
    case CALLBACK_PARAMETER_PB2:
        if (BUTTON_isPressed(handle))
        {

        	printf("BUTTON 2 is pressed\n\r");

        }
        if (BUTTON_isReleased(handle))
        {
        	requestOneTimeDataSend(BUTTON2RELEASED);
        	speed_2=0;distance_2=0;
        	printf("BUTTON 2 is released\n\r");
        }
        break;
    default:
        printf("Button not available \n\r");
        break;
    }
}


/**
 * @brief This will create the handles for button
 *
 * @retval FAILURE Button Handle is not created
 * @retval SUCCESS Button Handle is created
 */
static return_t createButton(void)
{
    return_t returnValue = FAILURE;

    Button1Handle = BUTTON_create(gpioButton1_Handle, GPIO_STATE_OFF);
    if (Button1Handle != NULL)
    {
        Button2Handle = BUTTON_create(gpioButton2_Handle, GPIO_STATE_OFF);
    }
    if (Button2Handle != NULL)
    {
        returnValue = SUCCESS;
    }
    return (returnValue);
}

/**
 * @brief This will enable the  button
 *
 * @retval FAILURE Button Handle is not enabled
 * @retval SUCCESS Button Handle is enabled
 */
static return_t enableButton(void)
{
    return_t returnValue = FAILURE;
    BUTTON_errorTypes_t buttonReturn = BUTTON_ERROR_INVALID_PARAMETER;
    buttonReturn = BUTTON_enable(Button1Handle);
    if (buttonReturn == BUTTON_ERROR_OK)
    {
        buttonReturn = BUTTON_enable(Button2Handle);
    }
    if (buttonReturn == BUTTON_ERROR_OK)
    {
        returnValue = SUCCESS;
    }
    return (returnValue);
}

/**
 * @brief This will set the callback for the button
 *
 * @retval FAILURE Callback set failed
 * @retval SUCCESS Callback set successfully
 */
static return_t setButtonCallback(void)
{
    return_t returnValue = FAILURE;
    BUTTON_errorTypes_t buttonReturn = BUTTON_ERROR_INVALID_PARAMETER;

    buttonReturn = BUTTON_setCallback(Button1Handle, callback, CALLBACK_PARAMETER_PB1);
    if (buttonReturn == BUTTON_ERROR_OK)
    {
        buttonReturn = BUTTON_setCallback(Button2Handle, callback, CALLBACK_PARAMETER_PB2);
    }
    if (buttonReturn == BUTTON_ERROR_OK)
    {
        returnValue = SUCCESS;
    }
    return (returnValue);
}

/* global functions ********************************************************* */
/**
 * @brief This will create and enable both LED and BUTTON handle
 *
 * @retval FAILURE led and button initialization failed
 * @retval SUCCESS led and button initialization success
 */
return_t ButtonInit(void)
{
    return_t returnValue = FAILURE;

    returnValue = createButton();

    if (returnValue == SUCCESS)
    {
        returnValue = enableButton();
    }
    else
    {
        printf("Error in creating button\n\r");

    }
    if (returnValue == SUCCESS)
    {
        returnValue = setButtonCallback();
    }
    else
    {
        printf("Error in enabling button\n\r");
    }
    return (returnValue);
}

/**
 * @brief This is a template function where the user can write his custom application.
 *
 */
//void appInitSystem(xTimerHandle xTimer)
//{
//    return_t returnValue = FAILURE;
//    BCDS_UNUSED(xTimer);
//    returnValue = ledAndButtonInit();
//    if (returnValue != SUCCESS)
//    {
//        printf("Error in led and button initialization\n\r");
//    }
//}

/**
 * @brief This will delete both LED and BUTTON handles
 *
 * @retval FAILURE led and button de-initialization failed
 * @retval SUCCESS led and button de-initialization success
 */
return_t ButtonDeinit(void)
{
    return_t returnValue = FAILURE;
    BUTTON_errorTypes_t buttonReturn = BUTTON_ERROR_INVALID_PARAMETER;

    buttonReturn = BUTTON_delete(Button1Handle);
    if (buttonReturn == BUTTON_ERROR_OK)
    {
        buttonReturn = BUTTON_delete(Button2Handle);
    }
    if (buttonReturn == BUTTON_ERROR_OK)
    {
        returnValue = SUCCESS;
    }

    return (returnValue);
}

/** ************************************************************************* */
