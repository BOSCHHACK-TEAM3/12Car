/**
 * This software is copyrighted by Bosch Connected Devices and Solutions GmbH, 2016.
 * The use of this software is subject to the XDK SDK EULA
 */

/* header definition ******************************************************** */
#ifndef XDK110_MYHTTPCLIENT_H_
#define XDK110_MYHTTPCLIENT_H_

/* local interface declaration ********************************************** */

/* local type and macro definitions */
#define TIMER_AUTORELOAD_ON pdTRUE

#define ONESECONDDELAY      UINT32_C(20)       /* one second is represented by this macro */
#define TIMERBLOCKTIME      UINT32_C(0xffff)     /* Macro used to define blocktime of a timer */
#define TIMEBASE            UINT32_C(0)          /* Macro used to define the Timebase for the ADC */

#define CONNECT_TIME_INTERVAL           UINT32_C(10000)          /**< Macro to represent connect time interval */
//#define WLAN_CONNECT_WPA_SSID                "HUAWEI-B315-DD0E"         /**< Macros to define WPA/WPA2 network settings */
//#define WLAN_CONNECT_WPA_PASS                "EFANJ126EFG"      /**< Macros to define WPA/WPA2 network settings */

//#define WLAN_CONNECT_WPA_SSID                "SGPMAC0001"         /**< Macros to define WPA/WPA2 network settings */
//#define WLAN_CONNECT_WPA_PASS                "12345678"      /**< Macros to define WPA/WPA2 network settings */
#define WLAN_CONNECT_WPA_SSID                "tootoo"         /**< Macros to define WPA/WPA2 network settings */
#define WLAN_CONNECT_WPA_PASS                "12345678"      /**< Macros to define WPA/WPA2 network settings */

#define DEST_PORT_NUMBER        		UINT16_C(8083)            /**< Macro to define the unsecure Server Port number */
#define MAX_URL_SIZE					150
#define DEST_URL_PATH					"/proxy/forward"

void initHttpModule(void) ;




#endif /* XDK110_MYHTTPCLIENT_H_ */

/** ************************************************************************* */
