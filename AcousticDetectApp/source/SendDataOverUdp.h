/**
 * This software is copyrighted by Bosch Connected Devices and Solutions GmbH, 2016.
 * The use of this software is subject to the XDK SDK EULA
 */

/* header definition ******************************************************** */
#ifndef XDK110_SENDDATAOVERUDP_H_
#define XDK110_SENDDATAOVERUDP_H_

/* local interface declaration ********************************************** */

/* local type and macro definitions */

#warning Please provide WLAN related configurations, with valid SSID & WPA key and server ip address where packets are to be sent in the below macros.
/** Network configurations */
#define WLAN_CONNECT_WPA_SSID                "naked-Internal"         /**< Macros to define WPA/WPA2 network settings */
#define WLAN_CONNECT_WPA_PASS                "nakedCool"      /**< Macros to define WPA/WPA2 network settings */

//#define WLAN_CONNECT_WPA_SSID                "tootoo"         /**< Macros to define WPA/WPA2 network settings */
//#define WLAN_CONNECT_WPA_PASS                "12345678"      /**< Macros to define WPA/WPA2 network settings */
//#define WLAN_CONNECT_WPA_SSID                "bots"         /**< Macros to define WPA/WPA2 network settings */
//#define WLAN_CONNECT_WPA_PASS                "boschbots"      /**< Macros to define WPA/WPA2 network settings */
//#define WLAN_CONNECT_WPA_SSID                "dlink-A80C"         /**< Macros to define WPA/WPA2 network settings */
//#define WLAN_CONNECT_WPA_PASS                "dbzfa56112"      /**< Macros to define WPA/WPA2 network settings */
//#define WLAN_CONNECT_WPA_SSID                "HUAWEI-B315-DD0E"         /**< Macros to define WPA/WPA2 network settings */
//#define WLAN_CONNECT_WPA_PASS                "EFANJ126EFG"      /**< Macros to define WPA/WPA2 network settings */
//#define WLAN_CONNECT_WPA_SSID                "Connected2Day"         /**< Macros to define WPA/WPA2 network settings */
//#define WLAN_CONNECT_WPA_PASS                "W3Ar3B0sch"      /**< Macros to define WPA/WPA2 network settings */

#define BUFFER_SIZE        UINT8_C(25)
/** IP addressed of server side socket.Should be in long format, E.g: 0xc0a82177 == 192.168.33.119 */
//#define SERVER_IP         UINT32_C(0xC0A80067) //home wifi 192.168.0.103
//#define SERVER_IP         UINT32_C(0xC0A82BD2) //mobile hotspot 192.168.43.210
//#define SERVER_IP         UINT32_C(0x34BB0FE0) //Azure IoT Cloud Singapore: 52.187.15.224
#define SERVER_IP         UINT32_C(0x2A9F857A) //Azure IoT Cloud China: 42.159.133.122
//#define SERVER_IP         UINT32_C(0x36FF82D4) //mushroom: AWS XDK Server 54.255.130.212
//#define SERVER_IP         UINT32_C(0x34DD69CB) //mushroom: AWS HackathonSG  52.221.105.203
//#define SERVER_IP         UINT32_C(0xC0A82B96)
//#define SERVER_IP         UINT32_C(0xC0A80896)  //Canteen SG Raspberry PI gateway IP
#define SERVER_PORT        UINT16_C(8083) //efan is 8082, sensormap use 8083
#define DATA_SEND_INTERVAL        UINT16_C(1) //in second

void initUDP(void);

void requestOneTimeDataSend(unsigned long buttonStatus);

/* local module global variable declarations */

/* local inline function definitions */

#endif /* XDK110_SENDDATAOVERUDP_H_ */

/** ************************************************************************* */
