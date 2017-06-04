/**
 * This software is copyrighted by Bosch Connected Devices and Solutions GmbH, 2016.
 * The use of this software is subject to the XDK SDK EULA
 */

/* module includes ********************************************************** */

/* system header files */
#include "BCDS_Basics.h"
#include "BCDS_WlanConnect.h"
#include "BCDS_NetworkConfig.h"
#include "PAL_initialize_ih.h"
#include "PAL_socketMonitor_ih.h"
#include <Serval_HttpClient.h>
#include <Serval_Network.h>

/* additional interface header files */
#include "stdio.h"
#include "FreeRTOS.h"
#include "timers.h"



/* own header files */
#include "XDKManagement.h"
#include "MyHttpClient.h"

/* constant definitions ***************************************************** */
#define PAL_IP_ADDRESS_SIZE UINT8_C(15) /*< Maximum size of the ip address */

/* local variables ********************************************************** */
static uint8_t PI_IPADDRESS[] ={192,168,43,217};
//static uint8_t PI_IPADDRESS[] ={192,168,8,144};


/* global variables ********************************************************* */
static xTimerHandle connectTimerHandle; /**< variable to store timer handle*/
Ip_Address_T destAddr = UINT32_C(0);/*< variable to store the Ip address of the server */
char url_ptr[MAX_URL_SIZE];

/* inline functions ********************************************************* */

/* local functions ********************************************************** */
static retcode_t callbackOnSent(Callable_T *callfunc, retcode_t status);
static retcode_t httpClientResponseCallback(HttpSession_T *httpSession ,Msg_T *msg_ptr, retcode_t status);
static retcode_t wlanConnect(void);

static retcode_t wlanConnect(void) {
	NCI_ipSettings_t myIpSettings;
	char ipAddress[PAL_IP_ADDRESS_SIZE] = { 0 };
	Ip_Address_T* IpaddressHex = Ip_getMyIpAddr();
	WLI_connectSSID_t connectSSID;
	WLI_connectPassPhrase_t connectPassPhrase;
	NCI_return_t ReturnValue = NCI_FAILURE;
	int32_t Result = INT32_C(-1);
	if (WLI_SUCCESS != WLI_init()) {
		return (RC_PLATFORM_ERROR);
	}

	printf("Connecting to %s \r\n ", WLAN_CONNECT_WPA_SSID);

	connectSSID = (WLI_connectSSID_t) WLAN_CONNECT_WPA_SSID;
	connectPassPhrase = (WLI_connectPassPhrase_t) WLAN_CONNECT_WPA_PASS;
	ReturnValue = NCI_setIpDhcp(NULL);
	if (ReturnValue != NCI_SUCCESS) {
		printf("Error in setting IP to DHCP\n\r");
		return (RC_PLATFORM_ERROR);
	}
	if (WLI_SUCCESS == WLI_connectWPA(connectSSID, connectPassPhrase, NULL)) {
		ReturnValue = NCI_getIpSettings(&myIpSettings);
		if (NCI_SUCCESS == ReturnValue) {
			*IpaddressHex = Basics_htonl(myIpSettings.ipV4);
			Result = Ip_convertAddrToString(IpaddressHex, ipAddress);
			if (Result < 0) {
				printf(
						"Couldn't convert the IP address to string format \r\n ");
				return (RC_PLATFORM_ERROR);
			}
			printf("Connected to WPA network successfully \r\n ");
			printf(" Ip address of the device %s \r\n ", ipAddress);
			return (RC_OK);
		} else {
			printf("Error in getting IP settings\n\r");
			return (RC_PLATFORM_ERROR);
		}
	} else {
		return (RC_PLATFORM_ERROR);
	}

}

/* The description is in the configuration header file. */
static retcode_t callbackOnSent(Callable_T *callfunc, retcode_t status) {
	BCDS_UNUSED(callfunc);

	if (status != RC_OK) {
		printf("error occurred in connecting server \r\n");
	}
	return (RC_OK);
}

/* The description is in the configuration header file. */
static retcode_t httpClientResponseCallback(HttpSession_T *httpSession,
		Msg_T *msg_ptr, retcode_t status) {
	BCDS_UNUSED(httpSession);
	retcode_t rc = status;

	if (status != RC_OK) {
		/* Error occurred in downloading the page */
	} else if (msg_ptr == NULL) {
		rc = RC_HTTP_PARSER_INVALID_CONTENT_TYPE;
	} else {
		if (HttpMsg_getStatusCode(msg_ptr) != Http_StatusCode_OK) {
			rc = RC_HTTP_INVALID_RESPONSE;
		} else {
			if (HttpMsg_getContentType(msg_ptr) != Http_ContentType_Text_Html) {
				rc = RC_HTTP_INVALID_RESPONSE;
			} else {
				char const *content_ptr;
				unsigned int len = UINT32_C(0);

				HttpMsg_getContent(msg_ptr, &content_ptr, &len);
				printf("GET Response Content %s length %d \r\n", content_ptr,
						len);
			}
		}
	}

	if (rc != RC_OK) {
		//printf("error occurred in downloading HTML \r\n");
	}
	return (rc);
}
/* The description is in the configuration header file. */
void connectServer(xTimerHandle xTimer) {
	memset(url_ptr, 0, MAX_URL_SIZE);
	snprintf(url_ptr, MAX_URL_SIZE, "%s?a=%s&b=%ld&c=%ld&d=%ld&e=%ld&f=%ld&g=%ld&h=%ld&i=%ld", DEST_URL_PATH,
			THING_ID, noiseLevel,temperature,humidity,pressure,xmagnetic,ymagnetic,zmagnetic,milliLux);

	printf("\tURL_PTR: %s \r\n", url_ptr);

	BCDS_UNUSED(xTimer);

	retcode_t rc = RC_OK;
	Msg_T* msg_ptr;
	Ip_Port_T destPort = (Ip_Port_T) DEST_PORT_NUMBER;
	static Callable_T SentCallable;
	Callable_T * Callable_pointer;
	Callable_pointer = Callable_assign(&SentCallable, callbackOnSent);
	if (Callable_pointer == NULL) {
		printf("Failed Callable_assign\r\n ");
		return;
	}
	rc = HttpClient_initRequest(&destAddr, Ip_convertIntToPort(destPort),
			&msg_ptr);

	if (rc != RC_OK || msg_ptr == NULL) {
		printf("Failed HttpClient_initRequest\r\n ");
		return;
	}

	HttpMsg_setReqMethod(msg_ptr, Http_Method_Get);
	rc = HttpMsg_setReqUrl(msg_ptr, url_ptr);
	if (rc != RC_OK) {
		printf("Failed to fill message \r\n ");
		return;
	}

	rc = HttpClient_pushRequest(msg_ptr, &SentCallable,
			httpClientResponseCallback);
	if (rc != RC_OK) {
		printf("Failed HttpClient_pushRequest \r\n  ");
		return;
	}
}
void initHttpModule(void) {

	retcode_t rc = RC_OK;

	rc = wlanConnect();
	if (RC_OK != rc) {
		printf("Network init/connection failed %i \r\n", rc);
		return;
	}

	rc = PAL_initialize();
	if (RC_OK != rc) {
		printf("PAL and network initialize %i \r\n", rc);
		return;
	}

	PAL_socketMonitorInit();

	/* start client */
	rc = HttpClient_initialize();
	if (rc != RC_OK) {
		printf("Failed to initialize http client\r\n ");
		return;
	}

	Ip_convertOctetsToAddr(PI_IPADDRESS[0], PI_IPADDRESS[1], PI_IPADDRESS[2],
			PI_IPADDRESS[3], &destAddr);



	connectTimerHandle = xTimerCreate((const char * const ) "connectServer",
			SECONDS(30),
			TIMER_AUTORELOAD_ON, NULL, connectServer);

	if (connectTimerHandle != NULL) {
		if (xTimerStart(connectTimerHandle, TIMERBLOCKTIME) != pdTRUE) {
			assert(false);
		}
	}

 }




/* global functions ********************************************************* */

/**
 * @brief This is a template function where the user can write his custom application.
 *
 */


/** ************************************************************************* */
