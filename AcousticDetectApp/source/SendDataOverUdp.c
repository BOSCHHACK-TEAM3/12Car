/**
 * This software is copyrighted by Bosch Connected Devices and Solutions GmbH, 2016.
 * The use of this software is subject to the XDK SDK EULA
 */
//lint -esym(956,*) /* Suppressing "Non const, non volatile static or external variable" lint warning*/

/* module includes ********************************************************** */

/* system header files */
#include <stdio.h>
/* additional interface header files */
#include "simplelink.h"
#include "BCDS_Basics.h"
#include "BCDS_Assert.h"

#include "efm32gg390f1024.h"
#include "core_cm3.h"

#include "FreeRTOS.h"
#include "timers.h"
#include "BCDS_WlanConnect.h"
#include "BCDS_NetworkConfig.h"
#include <Serval_Types.h>
#include <Serval_Basics.h>
#include <Serval_Ip.h>
/* own header files */
#include "SendDataOverUdp.h"
#include "stdint.h"
#include "XDKManagement.h"
#include <string.h>



static void wifiConnectGetIP(xTimerHandle xTimer);

static void wifiSend(xTimerHandle xTimer);
/* constant definitions ***************************************************** */
typedef enum returnTypes_e
{
    STATUS_NOT_OK,
    STATUS_OK,
    SOCKET_ERROR,
    SEND_ERROR
} returnTypes_t;

#define ONESECONDDELAY UINT32_C(1000) 	            /**< Macro to represent one second time unit*/
#define FIVESECONDDELAY UINT32_C(5000)              /**< Macro to represent five second time unit*/

#define TIMERBLOCKTIME UINT32_MAX             /**< Macro used to define block time of a timer*/
#define ZERO    UINT8_C(0)  			            /**< Macro to define value zero*/
#define TIMER_AUTORELOAD_ON             UINT32_C(1)             /**< Auto reload of timer is enabled*/
#define TIMER_AUTORELOAD_OFF            UINT32_C(0)             /**< Auto reload of timer is disabled*/
/* local variables ********************************************************** */

/**
 * This buffer holds the data to be sent to server via UDP
 * */
static int32_t bsdBuffer_mau[BUFFER_SIZE] = { (uint32_t) ZERO };
/**
 * Timer handle for connecting to wifi and obtaining the IP address
 */
xTimerHandle wifiConnectTimerHandle_gdt = NULL;
/**
 * Timer handle for periodically sending data over wifi
 */
xTimerHandle wifiSendTimerHandle = NULL;

uint16_t wifiInitialized=FALSE;

/* global variables ********************************************************* */

/* inline functions ********************************************************* */

/* local functions ********************************************************** */

void reInit(xTimerHandle xTimer){
    BCDS_UNUSED(xTimer);
	NVIC_SystemReset();
}
/**
 *  @brief
 *      Function to initialize the wifi network send application. Create timer task
 *      to start WiFi Connect and get IP function after one second. After that another timer
 *      to send data periodically.
 */
void initUDP(void)
{
	xTimerHandle xReInit =  xTimerCreate((char * const ) "reInit", SECONDS(18000), TIMER_AUTORELOAD_ON , NULL, reInit);

    if (xReInit != NULL)
    {
        if ( xTimerStart(xReInit, TIMERBLOCKTIME) != pdTRUE)
        {
//            assert(0==1);
            printf("reset the device --------------------------- \r\n ");

        	NVIC_SystemReset();

        }
    }
    else
    {
//        assert(0==1);
        printf("reset the device --------------------------- \r\n ");
    	NVIC_SystemReset();

    }
    /* create timer task*/
    wifiConnectTimerHandle_gdt = xTimerCreate((char * const ) "wifiConnect", SECONDS(1), TIMER_AUTORELOAD_OFF, NULL, wifiConnectGetIP);
    wifiSendTimerHandle = xTimerCreate((char * const ) "wifiSend", SECONDS(DATA_SEND_INTERVAL), TIMER_AUTORELOAD_ON, NULL, wifiSend);

    if ((wifiConnectTimerHandle_gdt != NULL) && (wifiSendTimerHandle != NULL))
    {
        /*start the wifi connect timer*/
        if ( xTimerStart(wifiConnectTimerHandle_gdt, TIMERBLOCKTIME) != pdTRUE)
        {
//            assert(0==1);
            printf("reset the device --------------------------- \r\n ");

        	NVIC_SystemReset();

        }
    }
    else
    {
        /* Assertion Reason: "Failed to create timer task during initialization"   */
//        assert(0==1);
        printf("reset the device --------------------------- \r\n ");

    	NVIC_SystemReset();

    }

}


/**
 * @brief Opening a UDP client side socket and sending data on a server port
 *
 * This function opens a UDP socket and tries to connect to a Server SERVER_IP
 * waiting on port SERVER_PORT.
 * Then the function will send periodic UDP packets to the server.
 * 
 * @param[in] port
 *					destination port number
 *
 * @return         returnTypes_t:
 *                                  SOCKET_ERROR: when socket has not opened properly
 *                                  SEND_ERROR: when 0 transmitted bytes or send error
 *                                  STATUS_OK: when UDP sending was successful
 */

static returnTypes_t bsdUdpClient(uint16_t port, uint32_t buttonStatus)
{
    SlSockAddrIn_t Addr;
    uint16_t AddrSize = (uint16_t) ZERO;
    int16_t SockID = (int16_t) ZERO;
    int16_t Status = (int16_t) ZERO;

    /* copies the dummy data to send array , the first array element is the running counter to track the number of packets send so far*/
    bsdBuffer_mau[0] = (int32_t) 64+strlen(THING_ID);  //32 bytes to send 8 data defined in XDKManagement.h file


    bsdBuffer_mau[1] = speed;//noiseLevel;
    bsdBuffer_mau[2] = temperature;
    bsdBuffer_mau[3] = humidity;
    bsdBuffer_mau[4] = distance;//pressure;
    bsdBuffer_mau[5] = crash;//xmagnetic;
    bsdBuffer_mau[6] = ymagnetic;
    bsdBuffer_mau[7] = zmagnetic;
    bsdBuffer_mau[8] = degree;//milliLux;

    bsdBuffer_mau[9] = xAccelData;
    bsdBuffer_mau[10] = yAccelData;
    bsdBuffer_mau[11] = zAccelData;
    bsdBuffer_mau[12] = xGyroscopeData;
    bsdBuffer_mau[13] = yGyroscopeData;
    bsdBuffer_mau[14] = zGyroscopeData;
    bsdBuffer_mau[15] = buttonStatus;



    char * p = &bsdBuffer_mau[0];
    memcpy( &p[64], THING_ID, strlen(THING_ID) );


    Addr.sin_family = SL_AF_INET;
    Addr.sin_port = sl_Htons((uint16_t) port);
    Addr.sin_addr.s_addr = sl_Htonl(SERVER_IP);
    AddrSize = sizeof(SlSockAddrIn_t);

    SockID = sl_Socket(SL_AF_INET, SL_SOCK_DGRAM, (uint32_t) ZERO); /**<The return value is a positive number if successful; other wise negative*/
    if (SockID < (int16_t) ZERO)
    {
        /* error case*/
        return (SOCKET_ERROR);
    }

    Status = sl_SendTo(SockID, bsdBuffer_mau, BUFFER_SIZE * sizeof(int32_t), (uint32_t) ZERO, (SlSockAddr_t *) &Addr, AddrSize);/**<The return value is a number of characters sent;negative if not successful*/
    printf("sl_SendTo is called \r\n ");

    /*Check if 0 transmitted bytes sent or error condition*/
    if (Status <= (int16_t) ZERO)
    {
        Status = sl_Close(SockID);
        if (Status < 0)
        {
            return (SEND_ERROR);
        }
        return (SEND_ERROR);
    }
    Status = sl_Close(SockID);
    if (Status < 0)
    {
        return (SEND_ERROR);
    }
    return (STATUS_OK);
}
/**
 *  @brief
 *      Function to periodically send data over WiFi as UDP packets. This is run as an Auto-reloading timer.
 *
 *  @param [in ] xTimer - necessary parameter for timer prototype
 */
static void wifiSend(xTimerHandle xTimer)
{
    BCDS_UNUSED(xTimer);
    if (STATUS_OK != bsdUdpClient(SERVER_PORT, 0))
    {
        /* assertion Reason:  "Failed to  send udp packet" */
//        assert(0==1);
        printf("wifiSend error , reset the device --------------------------- \r\n ");

    	NVIC_SystemReset();

    }
}

void requestOneTimeDataSend(unsigned long buttonStatus){
	if(wifiInitialized){

		bsdUdpClient(SERVER_PORT,buttonStatus);

	}
}


/**
 *  @brief
 *      Function to connect to wifi network and obtain IP address
 *
 *  @param [in ] xTimer
 */
static void wifiConnectGetIP(xTimerHandle xTimer)
{
    BCDS_UNUSED(xTimer);

    NetworkConfig_IpSettings_T myIpSettings;
    memset(&myIpSettings, (uint32_t) 0, sizeof(myIpSettings));
    char ipAddress[PAL_IP_ADDRESS_SIZE] = { 0 };
    Ip_Address_T* IpaddressHex = Ip_getMyIpAddr();
    WlanConnect_SSID_T connectSSID;
    WlanConnect_PassPhrase_T connectPassPhrase;
    Retcode_T ReturnValue = (Retcode_T)RETCODE_FAILURE;
    int32_t Result = INT32_C(-1);

    if (RETCODE_OK != WlanConnect_Init())
    {
        printf("Error occurred initializing WLAN \r\n ");
    	NVIC_SystemReset();
        return;
    }

    printf("Connecting to %s \r\n ", WLAN_CONNECT_WPA_SSID);

    connectSSID = (WlanConnect_SSID_T) WLAN_CONNECT_WPA_SSID;
    connectPassPhrase = (WlanConnect_PassPhrase_T) WLAN_CONNECT_WPA_PASS;
    ReturnValue = NetworkConfig_SetIpDhcp(NULL);
    if (ReturnValue)
    {
        printf("Error in setting IP to DHCP\n\r");
    	NVIC_SystemReset();
        return;
    }

    if (RETCODE_OK == WlanConnect_WPA(connectSSID, connectPassPhrase, NULL))
    {
        ReturnValue = NetworkConfig_GetIpSettings(&myIpSettings);
        if (RETCODE_OK == ReturnValue)
        {
            *IpaddressHex = Basics_htonl(myIpSettings.ipV4);
            Result = Ip_convertAddrToString(IpaddressHex, ipAddress);
            if (Result < 0)
            {
                printf("Couldn't convert the IP address to string format \r\n ");
            	NVIC_SystemReset();
                return;
            }
            printf("Connected to WPA network successfully. \r\n ");
            printf(" Ip address of the device: %s \r\n ", ipAddress);
        }
        else
        {
            printf("Error in getting IP settings\n\r");
        	NVIC_SystemReset();
            return;
        }
    }
    else
    {
        printf("Error occurred connecting %s \r\n ", WLAN_CONNECT_WPA_SSID);
    	NVIC_SystemReset();
        return;
    }
	wifiInitialized=TRUE;

    /* After connection start the wifi sending timer*/
    if (xTimerStart(wifiSendTimerHandle, TIMERBLOCKTIME) != pdTRUE)
    {
//        assert(0==1);
        printf("reset the device --------------------------- \r\n ");

    	NVIC_SystemReset();

    }
}

/* global functions ********************************************************* */

/** ************************************************************************* */
